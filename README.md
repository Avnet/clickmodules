---

# MikroE Click Board Software Library
## Version
* 1.0 - Initial Release

## Overview
This repository contains sensor code which is used in various Mikroe Click Boards.  The code is written in standard 'C' and architected so that it can be reused across multiple platforms. Generally, the code performs all the functionality assoicated with the sensor, and makes calls to user defined I/O functions. 

While specific functionality is dependent upon the sensor being implemented, in general, All sensors have the following functions:
1. \<xxx>_open - Allocate needed memory and capture function address(s) to be called for sensor input and output; returns a pointer to a unique object for use in subsequent sensor function calls.

For example, the BAROMETER sensor contains the following function:
```  
BAROMETER* open_barometer(uint8_t (*rb)(uint8_t a), void (*wb)(uint8_t a, uint8_t v
))
```
The user provides a read-byte and write-byte function for the sensor.  The clickmodule software requires  the read-byte function accepts a register address and returns the byte read and the write-byte function expects to be provided a register address and a value that should be written to the register.  For this particular sensor, the I/O is I2C (varies depending on the sensor used). 
3. close_\<xxx> - This function will close and free any memory that was allocated when the sensor was opened. For example, using the BAROMETER sensor example:
```
close_barometer(BAROMETER* b)
```
This function uses a pointer to the BAROMETER object and cleans memory and sensor settings before returning.

## Click Modules Currently Supported

| Click Board Name | Sensor Utilized | Source Code Included | I/O utilized | 
|:----------------:|:----------------|-------------:|:---------:|:-------------|
[barometer-click](https://www.mikroe.com/barometer-click) | [LPS25Hb](https://www.st.com/en/mems-and-sensors/lps25hb.html]) | barometer.c  barometer.h  barometer.hpp | I2C |
[Temp&Hum](https://www.mikroe.com/temp-hum-click) | [HTS221](https://www.st.com/en/mems-and-sensors/hts221.html) | hts221.c   hts221.h   hts221.hpp | I2C |
[LightRanger 2](https://www.mikroe.com/lightranger-2-click) | [VL53L0X](https://www.st.com/content/st_com/en/products/imaging-and-photonics-solutions/proximity-sensors/vl53l0x.html) | All under LIGHTRANGER | I2C | 
|[FLAME Click](https://www.mikroe.com/flame-click) | [PT334-6B](https://github.com/Avnet/clickmodules/blob/v1.0/FLAME/PT334-6B.pdf) | flame.c  flame-click-manual-v100.pdf  flame.h  flame.hpp  PT334-6B.pdf | GPIO |
| [HEARTRATE 4 Click](https://www.mikroe.com/heart-rate-4-click) | [MAX30101](https://www.maximintegrated.com/en/products/sensors/MAX30101.html) | algorithm_by_RF.c  algorithm_by_RF.h  max30102.c  max30102.h | I2C |
| [RELAY Click](https://www.mikroe.com/relay-click) | [G6D Miniature Relay](https://github.com/Avnet/clickmodules/blob/v1.0/RELAY/g6d-datasheet.pdf) | g6d-datasheet.pdf  relay.c  relay-click-schematic-v100-a.pdf  relay.h | GPIO |
| [OLED-B Click](https://www.mikroe.com/oled-b-click) | [SSD1306](http://www.solomon-systech.com/en/product/display-ic/oled-driver-controller/ssd1306/) | Avnet_GFX.c  Avnet_GFX.h  glcdfont.c  oledb.hpp  oledb_ssd1306.c  oledb_ssd1306.h | SPI |


## Platforms Tested With
Click Module code in this repository has been tested using the following Avnet Boards:

| Board                       |  Click Modules Tested |
|:-----------------------| :--------------------------|
| [AT&T Starter Kit 2](http://cloudconnectkits.org/product/lte-starter-kit-2) | FLAME, RELAY, OLED-B, BAROMETER, TEMP&HUMI, LightRanger2, HEARTRATE4 |
| [Azure Sphere](http://cloudconnectkits.org/product/azure-sphere-starter-kit) | FLAME, RELAY, OLED-B, BAROMETER, TEMP&HUMI, LightRanger2, HEARTRATE4 |
| [Ultra96](tbd...) | FLAME, RELAY*, BAROMETER, TEMP&HUMI, LightRanger2, HEARTRATE4 |

*Partial implementation
