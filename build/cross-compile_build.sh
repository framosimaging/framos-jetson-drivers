#!/bin/bash

# This script builds and install: kernel Image, in-tree kernel modules, out-of-tree kernel modules & DTBs


# color output
red=$'\e[31m'
grn=$'\e[32m'
yel=$'\e[33m'
blu=$'\e[34m'
mag=$'\e[35m'
cyn=$'\e[36m'
normal=$'\e[0m'


# check for exported variables
if [ -z "${L4T_DIR}" ]; then
    echo "Location of the L4T_DIR does not exist! Please export location of the L4T_DIR."
    return
fi
if [ -z "${L4T_SOURCE}" ]; then
	export L4T_SOURCE=${L4T_DIR}/source
fi
if [ -z "${TOOLCHAIN_DIR}" ]; then
	export TOOLCHAIN_DIR=${L4T_SOURCE}/toolchain
fi


# check for directories
if [ ! -d "$L4T_DIR" ]; then
    echo "${red}Invalid directory${normal} $L4T_DIR."
    echo "Please export location of the ${red}\"Linux_for_Tegra\"${normal} directory as a ${yel}L4T_DIR${normal} variable."
    return
fi
if [ ! -d "$L4T_SOURCE" ]; then
    echo "${red}Invalid directory${normal} $L4T_SOURCE."
    echo "Please export location of the ${red}\"sources\"${normal} directory as a ${yel}L4T_SOURCE${normal} variable."
    return
fi
if [ ! -d "$TOOLCHAIN_DIR" ]; then
    echo "${red}Invalid directory${normal} $TOOLCHAIN_DIR."
    echo "Please export location of the ${red}\"toolchain\"${normal} directory as a ${yel}TOOLCHAIN_DIR${normal} variable."
    return
fi


# export env
export CROSS_COMPILE=${TOOLCHAIN_DIR}/bin/aarch64-buildroot-linux-gnu-
export KERNEL_HEADERS=${L4T_SOURCE}/kernel/kernel-jammy-src
export INSTALL_MOD_PATH=${L4T_DIR}/rootfs/


# build DTBs
function build_dtree() {(
    set -e
    pushd ${L4T_SOURCE} &> /dev/null
    echo ; echo; echo -e "${yel}start DTBs build ... ${normal}"
    make dtbs
    popd &> /dev/null
)}

# install DTBs
function install_dtree() {(
    set -e
    pushd ${L4T_DIR} &> /dev/null
    if [ ! -d rootfs/boot/framos/dtbo ]; then
        sudo -E mkdir -p rootfs/boot/framos/dtbo
    fi
    pushd ${L4T_SOURCE} &> /dev/null
    echo ; echo; echo -e "${yel}device tree install ... ${normal}"
    sudo -E cp nvidia-oot/device-tree/platform/generic-dts/dtbs/*fr_*.dtbo ${L4T_DIR}/rootfs/boot/framos/dtbo/.
    popd &> /dev/null
)}


# build kernel Image & in-tree kernel modules
function build_kernel() {(
    set -e
    pushd ${L4T_SOURCE} &> /dev/null
    echo ; echo; echo -e "${yel}start kernel Image & in-tree kernel modules build ... ${normal}"
    make -C kernel
    popd &> /dev/null
)}

# install kernel Image & in-tree kernel modules
function install_kernel() {(
    set -e
    pushd ${L4T_SOURCE} &> /dev/null
    echo ; echo; echo -e "${yel}kernel Image & in-tree kernel modules install ... ${normal}"
    sudo -E make install -C kernel
    cp kernel/kernel-jammy-src/arch/arm64/boot/Image ${L4T_DIR}/kernel/Image
    popd &> /dev/null
)}


# build out-of-tree kernel modules
function build_oot_modules() {(
    set -e
    pushd ${L4T_SOURCE} &> /dev/null
    echo ; echo; echo -e "${yel}start out of tree kernel modules build ... ${normal}"
    make modules
    popd &> /dev/null
)}

# install out-of-tree kernel modules
function install_oot_modules() {(
    set -e
    pushd ${L4T_SOURCE} &> /dev/null
    echo ; echo; echo -e "${yel}out of tree kernel modules install ... ${normal}"
    sudo -E make modules_install
    popd &> /dev/null
)}

# build DTBs, kernel Image, in-tree kernel modules & out-of-tree kernel modules
function build_all() {(
    set -e
    build_kernel
    build_dtree
    build_oot_modules
)}

# build DTBs, kernel Image, in-tree kernel modules & out-of-tree kernel modules
function install_all() {(
    set -e
    install_kernel
    install_dtree
    install_oot_modules
)}


# clean all output-binary files
function clean_all() {(
	pushd ${L4T_SOURCE} &> /dev/null
	make clean
	popd &> /dev/null
)}


echo; echo; echo -e "${grn}L4T_DIR = ${L4T_DIR}"
echo -e "${grn}L4T_SOURCE = ${L4T_SOURCE}"
echo -e "${grn}TOOLCHAIN_DIR = ${TOOLCHAIN_DIR}"
echo -e "${grn}CROSS_COMPILE = ${CROSS_COMPILE}"
echo -e "${grn}KERNEL_HEADERS = ${KERNEL_HEADERS}"
echo -e "${grn}INSTALL_MOD_PATH = ${INSTALL_MOD_PATH}"


echo; echo "${cyn}C O M M A N D S:"
echo -e "${red}build_dtree${normal}:	\t\tbuild device tree & device tree overlays"
echo -e "${red}install_dtree${normal}:	\t\tinstall device tree & device tree overlays"
echo -e "${red}build_kernel${normal}:	\t\tbuild kernel Image & in-tree kernel modules"
echo -e "${red}install_kernel${normal}:	\t\tinstall kernel Image & in-tree kernel modules"
echo -e "${red}build_oot_modules${normal}:\t\tbuild out-of-tree kernel modules"
echo -e "${red}install_oot_modules${normal}:\t\tinstall out-of-tree kernel modules"
echo -e "${red}build_all${normal}:	\t\tbuild all"
echo -e "${red}install_all${normal}:	\t\tinstall all"
echo -e "${red}clean_all${normal}:	\t\tclean all"

echo
