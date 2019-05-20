---

# MikroE Click Board Software Library
## Version
* 1.0 - Initial Release

## Overview
This repository contains sensor code which is used in various Mikroe Click Boards.  The code is written in standard 'C' and architected so that it can be reused across multiple platforms. Generally, the code performs all the functionality assoicated with the sensor, and makes calls to user defined I/O functions. 

While specific functionallity is dependent upon the sensor being implmeented, in general, All sensors have the following functions:
1. <xxx>_open - This function allocates any needed memory and captures the function address(s) to be called for all sensor input and output then returns a pointer to a unique object that is used in subsequent sensor function calls.  For example, the BAROMETER sensor contains the following function:

     BAROMETER* open_barometer(uint8_t (*rb)(uint8_t a), void (*wb)(uint8_t a, uint8_t v))

The user is expected to provide a read-byte and write-byte function to the sensor.  The sensor software doesn't concern itself with how read-byte and write-byte are implemented, only that the read-byte function accepts a register address and returns the byte read from that address.  Similarly, the write-byte function expects to be provided a register address and a value that should be written to that register.  For this particular sensor, the I/O is I2C but the implementation of the I/O for a specific platform will vary.
2. close_<xxx> - This function will close and free any memory that was malloced when the sensor was opened. For example, using the BAROMETER sensor example:

     close_barometer(BAROMETER* b)

This functioin will take the object that was returned with it was opened and clean-up any utilized memory prior to returning.

## Click Modules Currently Supported
Click Board Name | Sensor Utilized | Source Code Included | I/O utilized | 

Barometer Click ([https://www.mikroe.com/barometer-click](https://www.mikroe.com/barometer-click)) | 
LPS25HB         ([https://www.st.com/en/mems-and-sensors/lps25hb.html](https://www.st.com/en/mems-and-sensors/lps25hb.html])) | 
barometer.c  barometer.h  barometer.hpp 
| I2C |

Temp&Hum ([https://www.mikroe.com/temp-hum-click](https://www.mikroe.com/temp-hum-click)) |
HTS221   ([https://www.st.com/en/mems-and-sensors/hts221.html](https://www.st.com/en/mems-and-sensors/hts221.html)) |
hts221.c  hts221.h  hts221.hpp |
I2C |

LightRanger 2 Click ([https://www.mikroe.com/lightranger-2-click](https://www.mikroe.com/lightranger-2-click)) |
VL53L0X ([https://www.st.com/content/st_com/en/products/imaging-and-photonics-solutions/proximity-sensors/vl53l0x.html](https://www.st.com/content/st_com/en/products/imaging-and-photonics-solutions/proximity-sensors/vl53l0x.html)) |
├── core
│   ├── inc
│   │   ├── vl53l0x_api_calibration.h
│   │   ├── vl53l0x_api_core.h
│   │   ├── vl53l0x_api.h
│   │   ├── vl53l0x_api_ranging.h
│   │   ├── vl53l0x_api_strings.h
│   │   ├── vl53l0x_def.h
│   │   ├── vl53l0x_device.h
│   │   ├── vl53l0x_interrupt_threshold_settings.h
│   │   └── vl53l0x_tuning.h
│   └── src
│       ├── vl53l0x_api.c
│       ├── vl53l0x_api_calibration.c
│       ├── vl53l0x_api_core.c
│       ├── vl53l0x_api_ranging.c
│       └── vl53l0x_api_strings.c
└── platform
    ├── inc
    │   ├── vl53l0x_i2c_platform.h
    │   ├── vl53l0x_platform.h
    │   ├── vl53l0x_platform_log.h
    │   └── vl53l0x_types.h
    └── src
        └── vl53l0x_platform.c |
I2C |



FLAME Click ([https://www.mikroe.com/flame-click](https://www.mikroe.com/flame-click))
PT334-6B ([


RELAY Click ([https://www.mikroe.com/relay-click](https://www.mikroe.com/relay-click))

OLED-B Click ([https://www.mikroe.com/oled-b-click](https://www.mikroe.com/oled-b-click)) |
SSD1306 ([http://www.solomon-systech.com/en/product/display-ic/oled-driver-controller/ssd1306/](http://www.solomon-systech.com/en/product/display-ic/oled-driver-controller/ssd1306/)) |
Avnet_GFX.c  Avnet_GFX.h  glcdfont.c  oledb.hpp  oledb_ssd1306.c  oledb_ssd1306.h |
SPI |


## Platforms Tested Against
Example code using the click module code in this repository is available at https://github.com/Avnet/clickboard_demos.  Currently there are demos for:

* AT&T Starter Kit 2 (http://cloudconnectkits.org/product/lte-starter-kit-2) 
* Azure Sphere (http://cloudconnectkits.org/product/azure-sphere-starter-kit) 
* Ultra96 (tbd...)


