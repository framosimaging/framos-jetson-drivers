#!/bin/bash

# This script builds and install: out-of-tree kernel modules & DTBs


# color output
red=$'\e[31m'
grn=$'\e[32m'
yel=$'\e[33m'
blu=$'\e[34m'
mag=$'\e[35m'
cyn=$'\e[36m'
normal=$'\e[0m'


# export env
export L4T_SOURCE=${PWD}/../source


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
    if [ ! -d /boot/framos/dtbo ]; then
        sudo -E mkdir -p /boot/framos/dtbo
    fi
    pushd ${L4T_SOURCE} &> /dev/null
    echo ; echo; echo -e "${yel}device tree install ... ${normal}"
    sudo -E cp nvidia-oot/device-tree/platform/generic-dts/dtbs/*fr_*.dtbo /boot/framos/dtbo/.
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
    build_dtree
    build_oot_modules
)}

# build DTBs, kernel Image, in-tree kernel modules & out-of-tree kernel modules
function install_all() {(
    set -e
    install_dtree
    install_oot_modules
)}


echo; echo; echo -e "${grn}L4T_SOURCE = ${L4T_SOURCE}"


echo; echo "${cyn}C O M M A N D S:"
echo -e "${red}build_dtree${normal}:	\t\tbuild device tree & device tree overlays"
echo -e "${red}install_dtree${normal}:	\t\tinstall device tree & device tree overlays"
echo -e "${red}build_oot_modules${normal}:\t\tbuild out-of-tree kernel modules"
echo -e "${red}install_oot_modules${normal}:\t\tinstall out-of-tree kernel modules"
echo -e "${red}build_all${normal}:	\t\tbuild all"
echo -e "${red}install_all${normal}:	\t\tinstall all"

echo
