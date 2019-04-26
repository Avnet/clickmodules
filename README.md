---

# MikroE Click Board Library
## Overview
This repository contains sensor code that can be reused across multiple platforms. Generally, the code is broken down into two files (three if c++ is included):

1. The basic sensor/driver code. Implemented in C.  All I/O is performed by calling user specified functions to allow support for different platforms.

2. A basic C header which defines constants and function prototypes

3. Optionally, a C++ header which defines a Class that incorporates all functionality needed to interact with the sensors--including I/O functions.
  
## Click Modules Currently Supported

- Barometer Click ([https://www.mikroe.com/barometer-click](https://www.mikroe.com/barometer-click))

- Temp&Hum ([https://www.mikroe.com/temp-hum-click](https://www.mikroe.com/temp-hum-click))

- OLED-B Click ([https://www.mikroe.com/oled-b-click](https://www.mikroe.com/oled-b-click))

- LightRanger Click ([https://www.mikroe.com/lightranger-click](https://www.mikroe.com/lightranger-click))

- FLAME Click ([https://www.mikroe.com/flame-click](https://www.mikroe.com/flame-click))

- RELAY Click ([https://www.mikroe.com/relay-click](https://www.mikroe.com/relay-click))

## Platforms Tested Against
Example code using the click module code in this repository is available at https://github.com/Avnet/clickboard_demos.  Currently there are demos for:

* AT&T Starter Kit 2 (http://cloudconnectkits.org/product/lte-starter-kit-2) 
* Azure Sphere (http://cloudconnectkits.org/product/azure-sphere-starter-kit) 
* Ultra96 (tbd...)


