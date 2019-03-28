/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

/**
*   @file   hts221.cpp
*   @brief  standard 'C' header for ST HTS221 sensor.
*
*   @author James Flynn
*
*   @date   1-Oct-2018
*/

#ifndef __HTS221_H__
#define __HTS221_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HTS221_SAD         0x5F    // slave address

#define WHO_AM_I           0x0F
#define I_AM_HTS221        0xBC    //This read-only register contains the device identifier, set to BCh

#define AVERAGE_REG        0x10    // To configure humidity/temperature average.
#define AVERAGE_DEFAULT    0x1B

/*
 * [7] PD: power down control
 * (0: power-down mode; 1: active mode)
 *
 * [6:3] Reserved
 *
 * [2] BDU: block data update
 * (0: continuous update; 1: output registers not updated until MSB and LSB reading)
 *
 * The BDU bit is used to inhibit the output register update between the reading of the upper
 * and lower register parts. In default mode (BDU = ?0?), the lower and upper register parts are
 * updated continuously. If it is not certain whether the read will be faster than output data rate,
 * it is recommended to set the BDU bit to ?1?. In this way, after the reading of the lower (upper)
 * register part, the content of that output register is not updated until the upper (lower) part is
 * read also.
 *
 * [1:0] ODR1, ODR0: output data rate selection (see table 17)
 */

#define CTRL_REG1          0x20
#define POWER_UP           0x80
#define BDU_SET            0x4
#define ODR0_SET           0x1   // setting sensor reading period 1Hz

#define CTRL_REG2          0x21
#define CTRL_REG3          0x22
#define REG_DEFAULT        0x00

#define STATUS_REG         0x27
#define TEMPERATURE_READY  0x1
#define HUMIDITY_READY     0x2

#define HUMIDITY_L_REG     0x28
#define HUMIDITY_H_REG     0x29
#define TEMP_L_REG         0x2A
#define TEMP_H_REG         0x2B

/*
 * calibration registry should be read for temperature and humidity calculation.
 * Before the first calculation of temperature and humidity,
 * the master reads out the calibration coefficients.
 * will do at init phase
 */
#define CALIB_START        0x30
#define CALIB_END          0x3F
/*
#define CALIB_T0_DEGC_X8   0x32
#define CALIB_T1_DEGC_X8   0x33
#define CALIB_T1_T0_MSB    0x35
#define CALIB_T0_OUT_L     0x3C
#define CALIB_T0_OUT_H     0x3D
#define CALIB_T1_OUT_L     0x3E
#define CALIB_T1_OUT_H     0x3F
 */

typedef struct hts221_t {
    int     active;
    int     bduActive;

    //calibration data is saved in the following variables...
    unsigned char _h0_rH, _h1_rH;
    unsigned int  _T0_degC, _T1_degC;
    unsigned int  _H0_T0, _H1_T0;
    unsigned int  _T0_OUT, _T1_OUT;

    uint8_t (*rd_byte)(uint8_t a);
    void    (*wr_byte)(uint8_t a, uint8_t v);
    } HTS221;

#ifdef __cplusplus
extern "C" {
#endif

HTS221* open_hts221(uint8_t (*rb)(uint8_t a), void (*wb)(uint8_t a, uint8_t v));
void     close_hts221(HTS221* p);
int     hts221_who_am_i(HTS221* ptr);
void    hts221_activate(HTS221* ptr);
void    hts221_deactivate(HTS221* ptr);
int     hts221_bduActivate(HTS221* ptr);
int     hts221_active(HTS221* ptr);
int     hts221_bduDeactivate(HTS221* ptr);
int     hts221_bduState(HTS221* ptr);
double  hts221_readHumidity(HTS221* ptr);
double  hts221_readTemperature(HTS221* ptr);

#ifdef __cplusplus
}
#endif

#endif // __HTS221_H__

