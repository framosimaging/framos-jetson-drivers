#!/usr/bin/env python3

import argparse
import sys
import importlib
import re
import os
import subprocess

sys.path.insert(1, "/opt/nvidia/jetson-io")
from Jetson import board
from Utils import dtc
import Headers
config_by_hardware = importlib.import_module("config-by-hardware")

extlinux_cfg = "/boot/extlinux/extlinux.conf"
dtbo_default_path = "/boot/framos/dtbo/"
dtbo_default_path_esc = "\/boot\/framos\/dtbo\/"


class Board(board.Board):
    def __init__(self):
        super().__init__()
        dtbos = dtc.find_compatible_dtbo_files(self.compat.split(),
                                               "/boot/framos/dtbo")
        self.board_headers = board._board_load_headers(Headers.HDRS, dtbos)

def dtbo_file_exists(file):
    return os.path.exists(dtbo_default_path + file)

def update_base_fdt(base_fdt):
        updateCmd = "sed -i '/MENU LABEL primary kernel/{N;N;s/$/\\n \ \ \ \ \ FDT \/boot\/dtb\/"
        updateCmd += base_fdt
        updateCmd += "/}' "
        updateCmd += extlinux_cfg
        subprocess.run([updateCmd], shell=True)

def prepare_dt_overlays():
        updateCmd = "sed -i '/FDT/{s/$/\\n \ \ \ \ \ OVERLAYS/}' "
        updateCmd += extlinux_cfg
        subprocess.run([updateCmd], shell=True)

def main():
    parser = argparse.ArgumentParser(None, None, "Jetson Config Camera v2.0.0")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-n", "--name", nargs='*', help="\"<csi-module>\" ...")
    group.add_argument("-l", "--list", help="List of CSI modules", action='store_true')
    args = parser.parse_args()
    board_dtbo_template = ""
    base_dtb = ""
    selected_fpa = ""
    fpa_dtbo = ""

    jetson = Board()
    jetson.get_board_headers()
    if "Jetson AGX CSI Connector" in jetson.board_headers:
        header = "Jetson AGX CSI Connector"
        board_dtbo_template = "tegra234-p3737-camera-fr_" 
    elif "Jetson 24pin CSI Connector" in jetson.board_headers:
        header = "Jetson 24pin CSI Connector"
        board_dtbo_template = "tegra234-p3767-camera-p3768-fr_"
    else:
        print("Unknown board or no overlays found!")
        sys.exit(1)

    base_dtb = subprocess.check_output(["basename /boot/dtb/*"], shell=True).decode('utf-8').replace('\n', '')
    dtbos = []

    if args.list:
        config_by_hardware.show_hardware(jetson, header)

    fpa_selected = False
    if args.name:
        for arg in args.name:
            if not fpa_selected:
                match = re.match(r"^Framos ([FPAfpa]+)\-([0-9a-zA-z\/\.]+)$", arg)
                if match:
                    selected_fpa = match.group(2).lower()
                    fpa_selected = True
                    fpa_dtbo = board_dtbo_template + "fpa_" + selected_fpa + "-overlay.dtbo"
                    fpa_dtbo = fpa_dtbo.replace('/', '_')
                    if (fpa_dtbo.count('.') == 2):
                        fpa_dtbo = fpa_dtbo.replace('.', '', 1)

            match = re.match(r"^Framos ([a-zA-Z0-9]+)\-([0-9a-zA-Z]+)\-([0-9a-zA-z]+)([-a-zA-Z]*)", arg)
            if (match):
                sensor = match.group(1).lower()
                port = match.group(2).lower()
                lanes = match.group(3).lower()
                gmslSelected = match.group(4).replace('-', '').lower()

                dtbo = board_dtbo_template + sensor + "-" + port + "-" + lanes + "-overlay.dtbo"
                dtbos.append(dtbo)

                if gmslSelected:
                    gmslDtbo = board_dtbo_template + port + "-gmsl-overlay.dtbo"
                    dtbos.append(gmslDtbo)

        if not selected_fpa:
            print("FPA overlay not specified. The script cannot continue.")
            sys.exit(1)
        dtbos.insert(0, fpa_dtbo)

        for dtbo in dtbos:
            if not dtbo_file_exists(dtbo):
                print(f"{dtbo} not found in {dtbo_default_path}.")
                print("This script cannot continue.")
                sys.exit(1)

        FDTentryExists = subprocess.check_output(['cat /boot/extlinux/extlinux.conf | grep "FDT" | wc -l'], shell=True)
        if FDTentryExists == b'0\n':
            update_base_fdt(base_dtb)

        overlayEntryExists = subprocess.check_output(['cat /boot/extlinux/extlinux.conf | grep "OVERLAYS" | wc -l'], shell=True)
        if overlayEntryExists == b'0\n':
            prepare_dt_overlays()

        final_dtbo_configuration = ""
        for dtbo in dtbos:
            final_dtbo_configuration += dtbo_default_path_esc
            final_dtbo_configuration += dtbo
            final_dtbo_configuration += ","
        final_dtbo_configuration = final_dtbo_configuration[:-1]
        updateCmd = f"sed -i 's/.*OVERLAYS.*/\ \ \ \ \ \ OVERLAYS {final_dtbo_configuration}/' "
        updateCmd += extlinux_cfg
        subprocess.run([updateCmd], shell=True)


        print(f"{extlinux_cfg} updated.")
        print("Reboot the System to apply changes.")

if __name__ == '__main__':
    try:        
        main()
    except Exception as error:
        print(error)
