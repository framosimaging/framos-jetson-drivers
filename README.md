&nbsp;
[Release Notes l4t‐r36.4.3](https://github.com/framosimaging/framos-jetson-drivers/wiki/Release-Notes-l4t%E2%80%90r36.4.3)


# Short procedure

This list describes which "Jetpack/L4T source_sync tag" and "framos-jetson-drivers branch" to use for the desired Jetpack/L4T release.

If using target build, the "framos-jetson-drivers branch" is used to checkout to compatible Framos drivers source code for the desired Jetpack/L4T.

If using cross-compilation, the "source_sync tag" is used to checkout to correct tag of the Jetpack/L4T kernel source code and the "framos-jetson-drivers branch" to checkout to compatible Framos drivers source code.

| Jetpack / L4T version | source_sync tag | framos-jetson-drivers branch |
|-----------------------|-----------------|------------------------------|
| 6.1 / 36.4.3          | jetson_36.4.3   | l4t-r36.4.3                  |
| 6.1 / 36.4            | jetson_36.4     | l4t-r36.4                    |
| 6.0 / 36.3            | jetson_36.3     | l4t-r36.3                    |

## 1. Get & Install Framos drivers
Two methods:
* [Using Framos source code on target system(Jetson platform)](https://github.com/framosimaging/framos-jetson-drivers/wiki/Clone,-Compile-and-Install-on-target-system(Jetson-platform))

  or

* [Using Framos source code on host system(Ubuntu 22.04)](https://github.com/framosimaging/framos-jetson-drivers/wiki/Clone,-Cross%E2%80%90Compile,-Install-and-flash-on-host-system(Ubuntu-22.04))

## 2. Configuration of Image Sensors on the Jetson platform (Target System)
Two methods:

* [Interactive version](https://github.com/framosimaging/framos-jetson-drivers/wiki/Interactive-version)

  or

* [Command line version](https://github.com/framosimaging/framos-jetson-drivers/wiki/Command-line-version)

## 3. Run streaming software
See [framos-jetson-libsv](https://github.com/framosimaging/framos-jetson-libsv) GitHub

***When using the IMX636 event based sensor, use [OpenEB software GitHub](https://github.com/framosimaging/openeb)


# For detailed guide and additional options and descriptions - [FRAMOS Sensor Module Ecosystem ‐ Driver User Guide](https://github.com/framosimaging/framos-jetson-drivers/wiki/FRAMOS-Sensor-Module-Ecosystem-%E2%80%90-Driver-User-Guide)
