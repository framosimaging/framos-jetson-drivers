&nbsp;
[Release Notes l4t‐r36.4.4](https://github.com/framosimaging/framos-jetson-drivers/wiki/Release-Notes-l4t%E2%80%90r38.4)


# Short procedure

This list describes which Jetpack/L4T "Nvidia tag" and "Framos branch" to use for the desired Jetpack/L4T release.

If using target build, the "Framos branch" is used to checkout to compatible Framos drivers source code for the desired Jetpack/L4T.

If using cross-compilation, the "Nvidia tag" is used to checkout to correct tag of the Jetpack/L4T kernel source code and the "Framos branch" to checkout to compatible Framos drivers source code.

| Jetpack / L4T version |    Nvidia tag   | Framos branch  |  Platforms           |
|-----------------------|-----------------|----------------|----------------------|
| * 7.1 / 38.4          | jetson_38.4     | l4t-r38.4      | AGX Thor             |
| 6.2.1 / 36.4.4        | jetson_36.4.4   | l4t-r36.4.4    | Orin (AGX, Nano, NX) |
| 6.2 / 36.4.3          | jetson_36.4.3   | l4t-r36.4.3    | Orin (AGX, Nano, NX) |
| 6.1 / 36.4            | jetson_36.4     | l4t-r36.4      | Orin (AGX, Nano, NX) |
| 6.0 / 36.3            | jetson_36.3     | l4t-r36.3      | Orin (AGX, Nano, NX) |

* This is beta release without target build and isp file support.

## 1. Get & Install Framos drivers
Two methods:
* [Using Framos source code on a host system (Ubuntu 22.04)](https://github.com/framosimaging/framos-jetson-drivers/wiki/Clone,-Cross%E2%80%90Compile,-Install-and-flash-on-host-system(Ubuntu-22.04))

## 2. Configuration of Image Sensors on the Jetson platform (Target System)
Two methods:

* [Interactive version](https://github.com/framosimaging/framos-jetson-drivers/wiki/Interactive-version)

  or

* [Command line version](https://github.com/framosimaging/framos-jetson-drivers/wiki/Command-line-version)

## 3. Run streaming software
- **Optional:** [Apply the ISP override file]( Not supported in beta release)
- See [framos-jetson-libsv](https://github.com/framosimaging/framos-jetson-libsv/tree/l4t-r38.4) GitHub

# For detailed guide and additional options and descriptions - [FRAMOS Sensor Module Ecosystem ‐ Driver User Guide](https://github.com/framosimaging/framos-jetson-drivers/wiki/FRAMOS-Sensor-Module-Ecosystem-%E2%80%90-Driver-User-Guide)
