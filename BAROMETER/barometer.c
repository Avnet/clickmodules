/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

/**
*   @file   barometer.c
*   @brief  code for managing the ST LPS25HB sensor.  The LPS25HB sensor 
*           (https://www.st.com/resource/en/datasheet/lps25hb.pdf) provides both 
*           Barometer and Temperature readings.
*
*   @author James Flynn
*
*   @date   28-Feb-2019
*/

#include "barometer.h"

/*! --------------------------------------------------------------------------------
* @brief    Open a barometer object to work with.  This function malloc's heap so it
*           must be paired with close_barometer to avoid memory leaks.
*
* @param    rb  Pointer to the function to called to read a byte from the device. 
*               Expecting to read a Register Address over I2C
* @param    wb  Pointer to the function to called to write a byte to the device.
*               Expecting to write a single byte to a register Address over I2C
* @returns  A pointer to a BAROMETER type (see barometer.h)
*/
BAROMETER* open_barometer(uint8_t (*rb)(uint8_t a), void (*wb)(uint8_t a, uint8_t v)) 
{
    BAROMETER* __pbar = (BAROMETER*) malloc(sizeof(BAROMETER));
    if( __pbar == NULL )
        return NULL;

    __pbar->rd_byte = rb;
    __pbar->wr_byte = wb;

    uint8_t addr = 0x20;
    uint8_t reg = __pbar->rd_byte(0x20) | 0xb0; //turn device on & configure
    wb( addr, reg );

    return __pbar;
}

/*! --------------------------------------------------------------------------------
* @brief    __temp is a function that is used internally when reading the 
*           temperature values.
*
* @param    __pbar  a pointer to a BAROMETER struction so the I2C read/write routines
*           can be accessed.
* @returns  returns the temperature in Celsius. If an error occurs, a value representing
*           zero degrees kelvin (-273.15) is returned because it is impossible to achieve
*/
float __temp(BAROMETER* __pbar) 
{
    float t = ZERO_KELVIN; // degrees Celsius, physically impossible-to-reach temperature of zero kelvin

    if( __pbar->rd_byte(0x27) & 0x01 ){
        uint8_t l  = __pbar->rd_byte(0x2b);
        uint8_t h  = __pbar->rd_byte(0x2c);
        t = ((h << 8 | l )<<((sizeof(int)-2)*8)) >> ((sizeof(int)-2)*8);
        t = 42.5 + (t/480.0);
        }
    return t;
}

/*! --------------------------------------------------------------------------------
* @brief    close_barometer pairs with the open_barometer function. It free's malloc'ed
*           memory.
*
* @param    b  a pointer to a BAROMETER struction
* @returns  none.
*/
void  close_barometer(BAROMETER* b) { free(b); }

/*! --------------------------------------------------------------------------------
* @brief    barometer_who_am_i returns the LPS25HB identification
*
* @param    __pbar  a pointer to a BAROMETER struction
* @returns  the SLPS25HB identification value
*/
int   barometer_who_am_i(BAROMETER* __pbar) { return __pbar->rd_byte(0x0f); }

/*! --------------------------------------------------------------------------------
* @brief    barometer_get_tempC returns the LPS25HB temperature reading
*
* @param    __pbar  a pointer to a BAROMETER struction
* @returns  none.
*/
float barometer_get_tempC(BAROMETER* __pbar) { return __temp(__pbar); }

/*! --------------------------------------------------------------------------------
* @brief    barometer_get_tempF returns the LPS25HB temperature reading in fahrenheit
*
* @param    __pbar  a pointer to a BAROMETER struction
* @returns  float.
*/
float barometer_get_tempF(BAROMETER* __pbar) { return (__temp(__pbar) * (float)1.8+32); } //convert to fahrenheit

/*! --------------------------------------------------------------------------------
* @brief    barometer_get_pressure returns the LPS25HB atomspheric pressure reading
*
* @param    __pbar  a pointer to a BAROMETER struction
* @returns  float.
*/
float barometer_get_pressure(BAROMETER* __pbar)  //in mbar
{
    float press = -1;
    if( __pbar->rd_byte(0x27) & 0x02 ){
        uint8_t xl = __pbar->rd_byte(0x28);
        uint8_t l  = __pbar->rd_byte(0x29);
        uint8_t h  = __pbar->rd_byte(0x2a);
        int counts = ((h << 16 | l<<8 | xl)<<((sizeof(int)-3)*8)) >> ((sizeof(int)-3)*8);
        press = (float)counts / 4096.0;
        }
    return press;
}



