/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

/**
*   @file   barometer.h
*   @brief  standard 'C' header for ST LPS25HB sensor.
*
*   @author James Flynn
*
*   @date   28-Feb-2019
*/

#ifndef __BAROMETER_H__
#define __BAROMETER_H__

#include <stdint.h>
#include <stdlib.h>

#define LPS25HB_SAD      (0x5d)
#define LPS25HB_WHO_AM_I 0xbd

#define ZERO_KELVIN      -273.15

typedef struct barometer_t {
    uint8_t (*rd_byte)(uint8_t raddr);
    void    (*wr_byte)(uint8_t raddr, uint8_t val);
    } BAROMETER;

#ifdef __cplusplus
extern "C" {
#endif

BAROMETER* open_barometer(uint8_t (*rb)(uint8_t a), void (*wb)(uint8_t a, uint8_t v));
float __temp(BAROMETER* __pbar);
void  close_barometer(BAROMETER* b);
int   barometer_who_am_i(BAROMETER* __pbar);
float barometer_get_tempC(BAROMETER* __pbar);
float barometer_get_tempF(BAROMETER* __pbar);
float barometer_get_pressure(BAROMETER* __pbar);

#ifdef __cplusplus
}
#endif

#endif // __BAROMETER_H__


