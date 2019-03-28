---

# MikroE Click Board Library
## Overview
This repository contains sensor code that can be reused across multiple platforms—both. Generally, the code is broken down into two files (three if c++ also):

1. The basic sensor/driver code. Implemented in C, with all I/O performed by calling abstracted read/write functions.

2. A basic C header which defines constants and function prototypes

3. Optionally, a basic C++ header which defines a Class that incorporates all functionality needed to interact with the sensors. This includes the read/write functions.
  
Examples show how the code is used/verified in the various Platform sub-directories.

## Click Modules Currently Supported

- Barometer Click ([https://www.mikroe.com/barometer-click](https://www.mikroe.com/barometer-click))

- Temp&Hum ([https://www.mikroe.com/temp-hum-click](https://www.mikroe.com/temp-hum-click))

- OLED-B Click ([https://www.mikroe.com/oled-b-click](https://www.mikroe.com/oled-b-click))

- LightRanger Click ([https://www.mikroe.com/lightranger-click](https://www.mikroe.com/lightranger-click))

- FLAME Click ([https://www.mikroe.com/flame-click](https://www.mikroe.com/flame-click))

- RELAY Click ([https://www.mikroe.com/relay-click](https://www.mikroe.com/relay-click))

## Platforms Tested Against
### AT&T Starter Kit 2 using the LTE IoT Breakout Carrier (ATTSK2 sub-directory). 
### Avnet Azure Sphere MT3620 Starter Kit

