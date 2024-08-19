#!/usr/bin/env python3

import curses
import os
import re
import sys
import math
import importlib
import subprocess

sys.path.insert(1, "/opt/nvidia/jetson-io")
from Jetson import board
from Utils import dtc
import Headers
jetsonio = importlib.import_module("jetson-io")


def lanes_to_string(lanes):
    return f"{lanes} lane" if lanes == "1" else f"{lanes} lanes"


class Board(board.Board):
    def __init__(self):
        super().__init__()
        dtbos = dtc.find_compatible_dtbo_files(self.compat.split(),
                                               "/boot/framos/dtbo")
        self.board_headers = board._board_load_headers(Headers.HDRS, dtbos)


class PortConfiguration(object):
    def __init__(self, port, sensors):
        self.port = port
        self.sensors = sensors


class SensorConfiguration(object):
    def __init__(self, sensor, lanes):
        self.sensor = sensor
        self.lanes = lanes


class LaneConfiguration(object):
    def __init__(self, lane, gmsl):
        self.lane = lane
        self.gmsl = gmsl


class PageMenu(jetsonio.Menu):
    def __init__(self, win, title, maxwidth=0, page_size=15):
        super().__init__(win, title, maxwidth)
        self.page_size = page_size
        self.pages = []
        self.page_index = 0

    def left(self):
        if self.page_index != 0:
            self.page_index -= 1
            self._refresh()

    def right(self):
        if self.page_index < (len(self.pages) - 1):
            self.page_index += 1
            self._refresh()

    def show(self, page_items, footer_items):

        paging_required = (len(page_items) + len(footer_items)) > self.page_size
        if paging_required:
            page_footer_size = 2 #empty line + line with page number
            items_per_page = self.page_size - len(footer_items) - page_footer_size
            if items_per_page < 1:
                raise RuntimeError(f"Page size {self.page_size} too low to display menu")
            page_count = math.ceil(len(page_items) / items_per_page)
            padded_page_items = page_items
            while len(padded_page_items) < (page_count * items_per_page):
                padded_page_items.append(jetsonio.MenuItem(None))
            for i in range(page_count):
                start = i * items_per_page
                end = i * items_per_page + items_per_page
                page = page_items[start:end]
                page += footer_items
                page.append(jetsonio.MenuItem(None))
                page_footer_prefix = "<-" if i != 0 else "  "
                page_footer_suffix = "->" if i != page_count - 1 else "  "
                page_footer = f"{page_footer_prefix} {i + 1}/{page_count} {page_footer_suffix}"
                page.append(jetsonio.MenuItem(page_footer))
                self.pages.append(page)
        else:
            self.pages.append(page_items + footer_items)

        self.win.show()
        self.win.addstr_centre(2, self.title)

        while True:
            items = self.pages[self.page_index]
            self.update(items)
            key = self.win.win.getch()

            if key in [curses.KEY_ENTER, ord(' '), ord('\n')]:
                if self.select(items):
                    break

            elif key == curses.KEY_UP:
                self.up(items)

            elif key == curses.KEY_DOWN:
                self.down(items)

            elif key == curses.KEY_LEFT:
                self.left()
            
            elif key == curses.KEY_RIGHT:
                self.right()

        self.win.hide()

    def _refresh(self):
        self.win.clear()
        self.win.show()
        self.win.addstr_centre(2, self.title)


class GMSLMenu(object):
    def __init__(self, screen, fpa, port, sensor, lanes, gmsls, h, w):
        self.screen = screen
        self.fpa = fpa
        self.port = port
        self.sensor = sensor
        self.lanes = lanes
        self.gmsls = gmsls
        self.h = h
        self.w = w
        self.go_back = False
        self.menu = None
        self.selection = None

    def _create_menu(self):
        self.menu_gmsls = []
        self.title = f"Configure GMSL for {self.lanes} lanes on {self.sensor} on {self.port} on FPA-{self.fpa}:"
        self.subwin = jetsonio.Window(self.screen, self.h, self.w, 2, 2)
        self.menu = jetsonio.Menu(self.subwin, self.title)
        for gmsl_option in self.gmsls:
            item = jetsonio.MenuItemAction(gmsl_option, True, self._select, gmsl_option)
            self.menu_gmsls.append(item)
        self.menu_gmsls.append(jetsonio.MenuItem(None))
        item = jetsonio.MenuItemAction("Back", True, self.exit)
        self.menu_gmsls.append(item)

    def _select(self, gmsl_arg):
        self.selection = gmsl_arg[0]
        self.exit()

    def get_selection(self):
        return self.selection

    def discard(self):
        self.selection = None

    def exit(self):
        self.go_back = True

    def show(self):
        if self.menu is None:
            self._create_menu()
        while True:
            if self.go_back:
                self.go_back = False
                break
            self.subwin.win.clear()
            self.menu.show(self.menu_gmsls)


class LaneMenu(object):
    def __init__(self, screen, fpa, port, sensor, lanes, h, w):
        self.screen = screen
        self.fpa = fpa
        self.port = port
        self.sensor = sensor
        self.lanes = lanes
        self.h = h
        self.w = w
        self.go_back = False
        self.menu = None
        self.lanes_menu = []
        self.operations_menu = []
        self.selection = None

    def _create_menu(self):
        self.gmsl_menus = dict() 
        self.title = f"Configure number of lanes for {self.sensor} on {self.port} on FPA-{self.fpa}:"
        self.subwin = jetsonio.Window(self.screen, self.h, self.w, 2, 2)
        self.menu = PageMenu(self.subwin, self.title)
        for lane, gmsls in self.lanes.items():
            self.gmsl_menus[lane] = GMSLMenu(self.screen, self.fpa, self.port, self.sensor, lane, gmsls, self.h, self.w)
            item = jetsonio.MenuItemAction(lanes_to_string(lane), True, self._select, lane)
            self.lanes_menu.append(item)
        self.operations_menu.append(jetsonio.MenuItemAction("None", True, self._select, None))
        self.operations_menu.append(jetsonio.MenuItem(None))
        item = jetsonio.MenuItemAction("Back", True, self.exit)
        self.operations_menu.append(item)

    def _select(self, lane_arg):
        selection = lane_arg[0]
        if selection is None:
            self.selection = None
            self.exit()
        else:
            gmsl_menu = self.gmsl_menus[selection]
            gmsl_menu.show()
            if gmsl_menu.get_selection() is not None:
                self.selection = selection
                self.exit()

    def get_selection(self):
        lane = self.selection
        if lane is None:
            return None
        gmsls = self.gmsl_menus[lane].get_selection()
        if gmsls is None:
            return None
        return LaneConfiguration(lane, gmsls)

    def discard(self):
        self.selection = None

    def exit(self):
        self.go_back = True

    def show(self):
        if self.menu is None:
            self._create_menu()
        
        while True:
            if self.go_back: 
                self.go_back = False
                break
            self.subwin.win.clear()
            self.menu.show(self.lanes_menu, self.operations_menu)


class SensorMenu(object):
    def __init__(self, screen, fpa, port, sensors, h, w):
        self.screen = screen
        self.fpa = fpa
        self.port = port
        self.sensors = sensors
        self.h = h
        self.w = w
        self.go_back = False
        self.menu = None
        self.sensors_menu = []
        self.operations_menu = []
        self.selection = None

    def _create_menu(self):
        self.lane_menus = dict()
        self.title = f"Configure an image sensor on {self.port} on FPA-{self.fpa}:"
        self.subwin = jetsonio.Window(self.screen, self.h, self.w, 2, 2)
        self.menu = PageMenu(self.subwin, self.title)
        for sensor, lanes in self.sensors.items():
            self.lane_menus[sensor] = LaneMenu(self.screen, self.fpa, self.port, sensor, lanes, self.h, self.w)
            item = jetsonio.MenuItemAction(sensor, True, self._select, sensor)
            self.sensors_menu.append(item)
        self.operations_menu.append(jetsonio.MenuItemAction("None", True, self._select, None))
        self.operations_menu.append(jetsonio.MenuItem(None))
        item = jetsonio.MenuItemAction("Back", True, self.exit)
        self.operations_menu.append(item)

    def _select(self, sensor_arg):
        selection = sensor_arg[0]
        if selection is None:
            self.selection = None
            self.exit()
        else:
            lane_menu = self.lane_menus[selection]
            lane_menu.show()
            if lane_menu.get_selection() is not None:
                self.selection = selection
                self.exit()

    def get_selection(self):
        sensor = self.selection
        if sensor is None:
            return None
        lanes = self.lane_menus[sensor].get_selection()
        if lanes is None:
            return None
        return SensorConfiguration(sensor, lanes)

    def discard(self):
        self.selection = None

    def exit(self):
        self.go_back = True

    def show(self):
        if self.menu is None:
            self._create_menu()

        while True:
            if self.go_back:
                self.go_back = False
                break
            self.subwin.win.clear()
            self.menu.show(self.sensors_menu, self.operations_menu)


class PortMenu(object):
    def __init__(self, screen, fpa, ports, h, w):
        self.screen = screen
        self.fpa = fpa
        self.ports = ports
        self.h = h
        self.w = w
        self.go_back = False
        self.menu = None
        self.ports_menu = []
        self.ports_menu_save = []
        self.operations_menu = []
        self.selection = None
        self.optional_actions_added = False

    def _create_menu(self):
        self.sensor_menus = dict()
        self.title = f"Configure camera connector on FPA-{self.fpa}:"
        self.subwin = jetsonio.Window(self.screen, self.h, self.w, 2, 2)
        self.menu = PageMenu(self.subwin, self.title)
        for port, sensors in self.ports.items():
            self.sensor_menus[port] = SensorMenu(self.screen, self.fpa, port, sensors, self.h, self.w)
            item = jetsonio.MenuItemAction(f"Configure {port}", True, self._select, port)
            self.ports_menu.append(item)
            item = jetsonio.MenuItemAction(f"Configure {port}", True, self._select, port)
            self.ports_menu_save.append(item)
        self.operations_menu.append(jetsonio.MenuItemAction("None", True, self._select, None))
        self.operations_menu.append(jetsonio.MenuItem(None))
        item = jetsonio.MenuItemAction("Back", True, self.exit)
        self.operations_menu.append(item)

    def _select(self, port_arg):
        selection = port_arg[0]
        if selection is None:
            self.selection = None
            self.exit()
        else:
            sensor_menu = self.sensor_menus[selection]
            sensor_menu.show()
            if sensor_menu.get_selection() is not None:
                self.selection = selection
                self.exit()

    def get_selection(self):
        port = self.selection
        if port is None:
            return None
        sensors = self.sensor_menus[port].get_selection()
        if sensors is None:
            return None
        return PortConfiguration(port, sensors)

    def discard(self):
        self.selection = None

    def exit(self):
        self.go_back = True

    def show(self):
        if self.menu is None:
            self._create_menu()

        while True:
            if self.go_back:
                self.go_back = False
                break
            self.subwin.win.clear()
            self.menu.show(self.ports_menu, self.operations_menu)


class MainMenu(object):
    def __init__(self, screen, main, h, w):
        self.main = main
        self.menu_default = []
        self.sensor_menus = {}
        self.port_menus = {}
        self.jetson = Board()
        self.headers = dict()
        self.selected_FPA = "defaultFPA"
        self.configuration_selected = False
        self.display_FPA_menu = False
        self.current_board = ""
        self.extLinuxFile = "/boot/extlinux/extlinux.conf"
        self.base_DTB = ""
        self.board_dtbo_template = ""

        self.current_configuration = dict()

        title = "Select attached FPA to your Jetson device:"
        self.subwin = jetsonio.Window(screen, h, w, 2, 2)
        self.menu = jetsonio.Menu(self.subwin, title)

        for fpa, ports in self._get_configurations().items():
            port_menu = PortMenu(screen, fpa, ports, h, w)
            self.port_menus[fpa] = port_menu
            item = jetsonio.MenuItemAction(f"FPA-{fpa}", True, port_menu.show)
            self.menu_default.append(item)

        item = jetsonio.MenuItemAction("Exit", False, self.exit)
        self.menu_default.append(item)

    def _get_configurations(self):
        self.jetson.get_board_headers()
        if "Jetson AGX CSI Connector" in self.jetson.board_headers:
            self.jetson.set_active_header("Jetson AGX CSI Connector")
            self.current_board = "Jetson AGX Orin"
        elif "Jetson 24pin CSI Connector" in self.jetson.board_headers:
            self.jetson.set_active_header("Jetson 24pin CSI Connector")
            self.current_board = "Jetson Orin Nano/NX"
        else:
             messages = []
             messages.append("Unknown board or no overlays found in /boot/framos/dtbo/ directory!")
             messages.append("Press any key to exit")
             self.main.print_and_wait(messages)
             self.exit(messages, False)

        configurations = dict()
        for header in self.jetson.hw_addon_get():
            match = re.match(r"^Framos ([FPA]+)\-([a-zA-Z0-9\/\.]+)$", header)
            if match:
                fpa = match.group(2)
                if fpa not in configurations:
                    configurations[fpa] = dict()

        if not configurations:
            # without FPA overlay, other overlays don't have any function
            messages = []
            messages.append("A FPA overlay not found in /boot/framos/dtbo/ directory!")
            messages.append("This script cannot continue.")
            self.main.print_and_wait(messages)
            self.exit(messages, False)

        for header in self.jetson.hw_addon_get():
            match = re.match(r"^Framos ([a-zA-Z0-9_]+)-([a-zA-Z0-9]+)-([1-9]*)Lane$", header)
            if match:
                for FPAoverlay in configurations.keys():
                    sensor = match.group(1)
                    port = match.group(2)
                    lanes = match.group(3)

                    if port not in configurations[FPAoverlay]:
                        configurations[FPAoverlay][port] = dict()
                    if sensor not in configurations[FPAoverlay][port]:
                        configurations[FPAoverlay][port][sensor] = dict()
                    if lanes not in configurations[FPAoverlay][port][sensor]:
                        configurations[FPAoverlay][port][sensor][lanes] = list()
                        configurations[FPAoverlay][port][sensor][lanes].append("No GMSL")

        gmsls = dict()
        for header in self.jetson.hw_addon_get():
            match = re.match(r"^Framos ([GMSL]+)-([A-Z]+)([0-9]+)", header)
            if match:
                gmslPort = match.group(2) + match.group(3)
                if gmslPort not in gmsls:
                    gmsls[gmslPort] = "GMSL"

        for gmslPortIt, gmslProperty in gmsls.items():
            for fpa, ports in configurations.items():
                for port, sensors in ports.items():
                    if gmslPortIt == port:
                        for sensor, lanes in sensors.items():
                            for lane in lanes:
                               configurations[fpa][port][sensor][lane].append("GMSL")

        return configurations

    def _configurations_are_default(self):
        default = True
        self.configuration_selected = False

        for idx, item in enumerate(self.port_menus.items()):
            self.configuration_selected |= False
            fpa, menu = item
            selection = menu.get_selection()
            if selection:
                default = False
                self.selected_FPA = fpa

                if not self.port_menus[fpa].optional_actions_added:
                    item = jetsonio.MenuItemAction('Save and reboot to reconfigure', False, self._update_extlinux_file_reboot)
                    self.port_menus[fpa].ports_menu_save.append(item)
                    item = jetsonio.MenuItemAction('Save and exit without rebooting', False, self._update_extlinux_file_no_reboot)
                    self.port_menus[fpa].ports_menu_save.append(item)
                    item = jetsonio.MenuItemAction('Discard all changes', True, self._discard_all)
                    self.port_menus[fpa].ports_menu_save.append(item)
                    item = jetsonio.MenuItemAction("Exit", False, self.exit)
                    self.port_menus[fpa].ports_menu_save.append(item)
                    self.port_menus[fpa].optional_actions_added = True

                    item = jetsonio.MenuItemAction("Back", True, self.exit_submenu)
                    self.port_menus[fpa].ports_menu.append(item)

                port_index = 0
                for port, sensors in menu.sensor_menus.items():
                    self.current_configuration[port] = dict()
                    selection = sensors.get_selection()
                    if selection is None:
                        self.port_menus[fpa].ports_menu_save[port_index].set_caption(f"Configure {port}")
                        self.configuration_selected |= False
                    else:
                        self.menu.title = f"Configure camera connector on FPA-{self.selected_FPA}"
                        caption = f"Re-configure {port}-"
                        caption += f"{selection.sensor}-"
                        self.current_configuration[port]["sensor"] = selection.sensor
                        caption += f"{selection.lanes.lane}Lanes-"
                        self.current_configuration[port]["lanes"] = selection.lanes.lane
                        caption += f"{selection.lanes.gmsl}"
                        caption += f" on FPA-{self.selected_FPA}"
                        self.current_configuration[port]["gmsl"] = selection.lanes.gmsl
                        self.port_menus[fpa].ports_menu_save[port_index].set_caption(caption)
                        self.configuration_selected |= True
                    port_index += 1
        return default

    def exit(self, messages=[], reboot=False):
        self.subwin.win.clear()
        if len(messages) > 0:
            self.main.print_and_wait(messages)
        if reboot:
            os.system('reboot')
        else:
            sys.exit(0)

    def exit_submenu(self):
        self.display_FPA_menu = True
        self.selected_FPA = "defaultFPA"
        self.configuration_selected = False
        self.subwin.win.clear()
        self.show()

    def show(self):
        while True:
            self.subwin.win.clear()
            if self.display_FPA_menu:
                self.display_FPA_menu = False
                self.menu.show(self.menu_default)
            if self._configurations_are_default():
                self.menu.show(self.menu_default)
            else:
                if self.configuration_selected:
                    self.menu.show(self.port_menus[self.selected_FPA].ports_menu_save)
                else:
                    self.menu.show(self.port_menus[self.selected_FPA].ports_menu)

    def _update_extlinux_file(self):
        if self.current_board == "Jetson AGX Orin":
            self.board_dtbo_template = "tegra234-p3737-camera-fr_"
        else:
            self.board_dtbo_template = "tegra234-p3767-camera-p3768-fr_"

        FDTentryExists = subprocess.check_output(['cat /boot/extlinux/extlinux.conf | grep "FDT" | wc -l'], shell=True)
        if FDTentryExists == b'0\n':
            self._update_base_fdt()

        overlayEntryExists = subprocess.check_output(['cat /boot/extlinux/extlinux.conf | grep "OVERLAYS" | wc -l'], shell=True)
        if overlayEntryExists == b'0\n':
            self._prepare_overlays_in_extlinux()

        self._update_fpa_configuration()

        self._update_port_configuration()

    def _update_extlinux_file_no_reboot(self):
        self._update_extlinux_file()
        messages = []
        messages.append("/boot/extlinux/extlinux.conf updated.")
        messages.append("Reboot the System to apply changes.")
        messages.append("Press any key to exit")
        self.exit(messages, False)

    def _update_extlinux_file_reboot(self):
        self._update_extlinux_file()
        messages = []
        messages.append("/boot/extlinux/extlinux.conf updated.")
        messages.append("Press any key to reboot the System now"
                        " or CTRL-C to abort")
        self.exit(messages, True)

    def _update_base_fdt(self):
        self.base_DTB = subprocess.check_output(["basename /boot/dtb/*"], shell=True).decode('utf-8').replace('\n', '')

        updateCmd = "sed -i \"/MENU LABEL primary kernel/{N;N;s/$/\\n \ \ \ \ \ FDT \/boot\/dtb\/"
        updateCmd += self.base_DTB
        updateCmd += "/}\" "
        updateCmd += self.extLinuxFile
        subprocess.run([updateCmd], shell=True)

    def _prepare_overlays_in_extlinux(self):
        updateCmd = "sed -i '/FDT/{s/$/\\n \ \ \ \ \ OVERLAYS/}' "
        updateCmd += self.extLinuxFile
        subprocess.run([updateCmd], shell=True)

    def _update_fpa_configuration(self):
        fpaDTBO = self.board_dtbo_template + "fpa";
        fpaDTBO += "_"
        fpaDTBO += self.selected_FPA.lower()
        fpaDTBO = fpaDTBO.replace('.', '')
        fpaDTBO = fpaDTBO.replace('/', '_')
        fpaDTBO += "-"
        fpaDTBO += "overlay.dtbo"

        if not self._selected_dtbo_exists(fpaDTBO):
            messages = []
            messages.append(f"{fpaDTBO} not found in /boot/framos/dtbo/ directory")
            messages.append("This script cannot continue.")
            messages.append("Press any key to exit.")
            self.exit(messages, False)

        updateCmd = f"sed -i 's/OVERLAYS.*/OVERLAYS \/boot\/framos\/dtbo\/{fpaDTBO}/' "
        updateCmd += self.extLinuxFile
        subprocess.run([updateCmd], shell=True)

    def _update_port_configuration(self):
        for port, sensor in self.current_configuration.items():
            portDTBO = self.board_dtbo_template
            if sensor:
                portDTBO = self.board_dtbo_template + sensor["sensor"].lower()
                portDTBO += "-"
                portDTBO += port.lower()
                portDTBO += "-"
                portDTBO += sensor["lanes"].lower()
                portDTBO += "lane-overlay.dtbo"
                

                if not self._selected_dtbo_exists(portDTBO):
                    messages = []
                    messages.append(f"{portDTBO} not found in /boot/framos/dtbo directory")
                    messages.append("This overlay will not be applied.")
                    continue
                updateCmd = f"sed -i '/OVERLAYS/ s/$/,\/boot\/framos\/dtbo\/{portDTBO}/' "
                updateCmd += self.extLinuxFile
                subprocess.run([updateCmd], shell=True)

                if sensor['gmsl'] == "GMSL":
                    gmslDTBO = self.board_dtbo_template + port.lower()
                    gmslDTBO += "-gmsl-overlay.dtbo"

                    if not self._selected_dtbo_exists(gmslDTBO):
                        messages = []
                        messages.append(f"{gmslDTBO} not found in /boot/framos/dtbo directory")
                        messages.append("This overlay will not be applied.")

                    updateCmd = f"sed -i '/OVERLAYS/ s/$/,\/boot\/framos\/dtbo\/{gmslDTBO}/' "
                    updateCmd += self.extLinuxFile
                    subprocess.run([updateCmd], shell=True)

    def _selected_dtbo_exists(self, dtbo):
        dtboPath = "/boot/framos/dtbo/" + dtbo
        return os.path.exists(dtboPath)

    def _discard_all(self):
        self.menu.title = "Select attached FPA to your Jetson device:"
        for port in self.port_menus[self.selected_FPA].sensor_menus.values():
            port.discard()


class MainWindow(jetsonio.MainWindow):
    def __init__(self, screen, h, w):
        title = " Jetson Config Camera v2.0.0 "
        self.main = jetsonio.Window(screen, h, w, 1, 1)
        self.main.win.border('|', '|', '=', '=', ' ', ' ', ' ', ' ')
        self.main.addstr_centre(0, title)
        self.main.show()

        y, x = screen.getmaxyx()
        if h > y or w > x:
            messages = []
            messages.append("Failed to resize terminal!")
            messages.append("Please try executing 'resize' and try again.")
            messages.append("Press any key to exit")
            self.print_and_wait(messages)
            self.main.win.getch()
            sys.exit(1)


class JetsonCsi(object):
    def __init__(self, stdscreen):
        height = 50
        width = 80
        curses.resizeterm(height, width)
        self.win = MainWindow(stdscreen, height - 2, width - 10)

        try:
            self.menu = MainMenu(stdscreen, self.win, height - 4, width - 12)
            self.menu.show()
        except KeyboardInterrupt:
           sys.exit(0)
        except Exception as error:
            messages = []
            messages.append("FATAL ERROR!")
            messages.append(str(error))
            messages.append("Press any key to terminate")
            self.win.print_and_wait(messages)
            sys.exit(1)


if __name__ == '__main__':
    curses.wrapper(JetsonCsi)
