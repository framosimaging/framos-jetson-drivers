&nbsp;

Supported Nvidia Jetson platforms:
  - Jetson AGX Orin developer kit
  - Jetson Orin Nano/NX developer kit
 
# Compile and installation on target system(Jetson platform)

## Procedure:
Prerequisites:
- Nvidia Jetson platform (Target System) flashed with Nvidia JetPack 6.0 (L4T 36.3)
- Nvidia Jetson platform (Target System) connected to Internet
- Install packages required for building:
    ```
    sudo apt install flex bison libssl-dev
    ```

1. Clone the Framos git repository to the Home directory on the Jetson platform:
    ```
    cd ~
    
    git clone https://github.com/framosgmbh/framos-jetson-drivers.git
    ```

2. Compile kernel modules(nvidia-oot) and device tree overlays
    ```
    cd ~/framos-jetson-drivers/build
    
    source target_build.sh
    build_all
    ```
3. Install kernel modules(nvidia-oot) and device tree overlays
    ```
    install_all
    ```
    NOTE: After installation, nvidia-oot kernel modules are installed in */lib/modules/5.15.136-tegra/updates/...* and device tree overlays in */boot/framos/dtbo/...*

4. Copy scripts for configuration of Image Sensors to /usr/bin/ :
    ```
    sudo cp ~/framos-jetson-drivers/tools/jetson-config-camera*.py /usr/bin/.
    ```
5. [Configuration of Image Sensors on the Jetson platform (Target System)](https://github.com/framosgmbh/framos-jetson-drivers/wiki/Configuration-of-Image-Sensors-on-the-Jetson-platform-(Target-System))

# Cross-Compile on host system(Ubuntu 22.04)
## Procedure:
Prerequisites:
- Installed Ubuntu 22.04 OS on Host System.
- Installed Nvidia JetPack 6.0 - It can be downloaded from [SDK Manager](https://developer.nvidia.com/embedded/downloads#?search=SDK) with its [SDK Manager Download Instructions](https://docs.nvidia.com/sdk-manager/download-run-sdkm/index.html). Log in required.
- Downloaded Kernel Source L4T 36.3 - refer to [Kernel Customization](https://docs.nvidia.com/jetson/archives/r36.3/DeveloperGuide/SD/Kernel/KernelCustomization.html) chapter in Nvidia Jetson Development Guide. It is recommended to follow the instructions in the [Obtaining the kernel Sources with Git](https://docs.nvidia.com/jetson/archives/r36.3/DeveloperGuide/SD/Kernel/KernelCustomization.html#obtaining-the-kernel-sources) section. Checkout every git repository to initial commit of L4T r36.3 (tag: jetson_36.3).
    ```
    cd <install-path>/Linux_for_Tegra/source
    ./source_sync.sh -k -t jetson_36.3
    ```  
- Downloaded & Extracted Toolchain - refer to the [Jetson Linux Toolchain](https://docs.nvidia.com/jetson/archives/r36.3/DeveloperGuide/AT/JetsonLinuxToolchain.html) chapter in the Nvidia Jetson Development Guide. [Bootlin Toolchain GCC 11.3 Direct Download link](https://developer.nvidia.com/downloads/embedded/l4t/r36_release_v3.0/toolchain/aarch64--glibc--stable-2022.08-1.tar.bz2)

- Run a script from Nvidia that installs the packages required for flashing:
    ```
    sudo <install-path>/Linux_for_Tegra/tools/l4t_flash_prerequisites.sh
    ```
- Install packages required for building:
    ```
    sudo apt install flex bison libssl-dev
    ```  

1. Export full path of *Linux_for_tegra*, *Linux_for_Tegra source* and *toolchain* directories:
    ```
    export L4T_DIR=<install-path>/Linux_for_Tegra
    export L4T_SOURCE=<install-path>/Linux_for_Tegra/source
    export TOOLCHAIN_DIR=<install-path>/<toolchain>
    ```
2. Clone the Framos git repository to the Home directory on the host system:
    ```
    cd ~
    
    git clone https://github.com/framosgmbh/framos-jetson-drivers.git
    ```
3. Copy & replace Nvidia clean L4T source files & folders with Framos modified files & folders from github
    ```
    cp -r ~/framos-jetson-drivers/source/hardware $L4T_SOURCE/.
    cp -r ~/framos-jetson-drivers/source/nvidia-oot $L4T_SOURCE/.
    ```
4. Copy script for building to $L4T_DIR path:
    ```
    cp ~/framos-jetson-drivers/build/cross-compile_build.sh $L4T_DIR
    ```
5. Cross-compile kernel image, kernel modules(nvidia-oot) and device tree overlays
    ```
    cd $L4T_DIR

    source cross-compile_build.sh
    build_all
    ```
6. Install kernel image, kernel modules(nvidia-oot) and device tree overlays to the $L4T_DIR/rootfs
    ```
    install_all
    ```
7. Copy scripts for configuration of Image Sensors to $L4T_DIR/rootfs/usr/bin/ :
    ```
    sudo cp ~/framos-jetson-drivers/tools/jetson-config-camera*.py $L4T_DIR/rootfs/usr/bin/.
    ```
8. Flash Jetson platform (Target System)
   - for AGX Orin Developer Kit
        ```
        cd $L4T_DIR
        
        sudo ./flash.sh jetson-agx-orin-devkit internal
        ```
    - for Orin Nano Developer Kit (**with microSD card**)
        ```
        cd $L4T_DIR
        
        sudo ./flash.sh jetson-orin-nano-devkit internal
        ```
    - for Orin Nano Developer Kit or Orin NX SOM with Orin Nano carrier board(**with NVME**)
        ```
        cd $L4T_DIR
        
        sudo ./flash.sh jetson-orin-nano-devkit-nvme internal
        ```
9. [Configuration of Image Sensors on the Jetson platform (Target System)](https://github.com/framosgmbh/framos-jetson-drivers/wiki/Configuration-of-Image-Sensors-on-the-Jetson-platform-(Target-System))
