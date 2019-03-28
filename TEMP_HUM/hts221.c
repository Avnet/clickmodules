/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

/**
*   @file   hts221.cpp
*   @brief  code for managing the ST HTS221 sensor. 
*
*   @author James Flynn
*
*   @date   1-Oct-2018
*/

#include <unistd.h>
#include "hts221.h"

/*! --------------------------------------------------------------------------------
* @brief    __getCalibration  a function that performs the specified HTS221
*           calibration procedure to obtain accurate measurments. This function 
*           is only used internally.
*
* @param    ptr  Pointer to a HTS221 object
* @returns  0 on failure, 1 when sucessful
*/
static int __getCalibration(HTS221* ptr)
{
    uint8_t data;
    uint16_t tmp;

    for (int reg=CALIB_START; reg<=CALIB_END; reg++) {
        if ((reg!=CALIB_START+8) && (reg!=CALIB_START+9) && (reg!=CALIB_START+4)) {
            data = ptr->rd_byte(reg);
            switch (reg) {
                case CALIB_START:      //0x30
                    ptr->_h0_rH = data;
                    break;
                case CALIB_START+1:    //0x31
                    ptr->_h1_rH = data;
                    break;
                case CALIB_START+2:    //0x32
                    ptr->_T0_degC = data;
                    break;
                case CALIB_START+3:    //0x30
                    ptr->_T1_degC = data;
                    break;
                case CALIB_START+5:    //0x35
                    tmp = ptr->_T0_degC & 0x00ff;
                    ptr->_T0_degC = ((uint16_t)data & 0x0003)<<8;
                    ptr->_T0_degC |= tmp;
    
                    tmp = ptr->_T1_degC & 0x00ff;;
                    ptr->_T1_degC = ((uint16_t)data&0x000C)<<6;
                    ptr->_T1_degC |= tmp;
                    break;
                case CALIB_START+6:     //0x36
                    ptr->_H0_T0 = data;
                    break;
                case CALIB_START+7:      //0x37
                    ptr->_H0_T0 |= ((int)data)<<8;
                    if( ptr->_H0_T0 & 0x8000 )
                        ptr->_H0_T0 |= 0xffff0000;
                    break;
                case CALIB_START+0xA:    //0x3a
                    ptr->_H1_T0 = data;
                    break;
                case CALIB_START+0xB:    //0x3b
                    ptr->_H1_T0 |= ((int)data)<<8;
                    if( ptr->_H1_T0 & 0x8000 )
                        ptr->_H1_T0 |= 0xffff0000;
                    break;
                case CALIB_START+0xC:    //0x3c
                    ptr->_T0_OUT = data;
                    break;
                case CALIB_START+0xD:    //0x3d
                    ptr->_T0_OUT |= ((int)data)<<8;
                    if( ptr->_T0_OUT & 0x8000 )
                        ptr->_T0_OUT |= 0xffff0000;
                    break;
                case CALIB_START+0xE:    //0x3e
                    ptr->_T1_OUT = data;
                    break;
                case CALIB_START+0xF:    //0x3f
                    ptr->_T1_OUT |= ((int)data)<<8;
                    if( ptr->_T1_OUT & 0x8000 )
                        ptr->_T1_OUT |= 0xffff0000;
                    break;
                case CALIB_START+8:
                case CALIB_START+9:
                case CALIB_START+4:
                    //DO NOTHING
                    break;
    
                // cover any possible error
                default:
                    return 0;
                } 
            } 
        }  
    return 1;
    }


/*! --------------------------------------------------------------------------------
* @brief    Open a hts221 object to work with.  This function malloc's heap so it
*           must be paired with close_hts221 to avoid memory leaks.
*
* @param    rb  Pointer to the function to called to read a byte from the device. 
*               Expecting to read a Register Address over I2C
* @param    wb  Pointer to the function to called to write a byte to the device.
*               Expecting to write a single byte to a register Address over I2C
* @returns  A pointer to a HTS221 type (see hts221.h)
*/
HTS221* open_hts221(uint8_t (*rb)(uint8_t a), void (*wb)(uint8_t a, uint8_t v))
{
    HTS221* _hts221 = (HTS221*) malloc(sizeof(HTS221));
    if( _hts221 == NULL )
        return NULL;

    memset(_hts221, 0x00, sizeof(HTS221));

    _hts221->rd_byte = rb;
    _hts221->wr_byte = wb;

    return _hts221;
}


/*! --------------------------------------------------------------------------------
* @brief    close_hts221 pairs with the open_hts221 function. It free's malloc'ed
*           memory.
*
* @param    p  a pointer to a HTS221 struct
* @returns  none.
*/
void close_hts221(HTS221* p)
{
    hts221_deactivate(p);
    free(p);
}

/*! --------------------------------------------------------------------------------
* @brief    hts221_who_am_i returns the HTS221 identification
*
* @param    ptr  a pointer to a HTS221 struct
* @returns  the HTS221 identification value
*/
int hts221_who_am_i(HTS221* ptr) { return ptr->rd_byte(0x0f); }


/*! --------------------------------------------------------------------------------
* @brief    hts221_activate powers up the HTS221 
*
* @param    ptr  a pointer to a HTS221 struct
* @returns  none
*/
void hts221_activate(HTS221* ptr) 
{
    uint8_t data;
    if( !ptr )
        return;
    data = ptr->rd_byte(CTRL_REG1);
    data |= POWER_UP;
    data |= ODR0_SET;
    ptr->wr_byte(CTRL_REG1, data);
    __getCalibration(ptr);
    ptr->active = 1;
}

/*! --------------------------------------------------------------------------------
* @brief    hts221_deactivate powers down the HTS221 
*
* @param    ptr  a pointer to a HTS221 struct
* @returns  none
*/
void hts221_deactivate(HTS221* ptr) 
{
    uint8_t data;

    data = ptr->rd_byte(CTRL_REG1);
    data &= ~POWER_UP;
    ptr->wr_byte(CTRL_REG1, data);
    ptr->active = 0;
}

/*! --------------------------------------------------------------------------------
* @brief    hts221_active returns the HTS221 power state
*
* @param    ptr  a pointer to a HTS221 struct
* @returns  0 if powered down, 1 if operating
*/
int hts221_active(HTS221* ptr) 
{
    return ptr->active;
}

/*! --------------------------------------------------------------------------------
* @brief    hts221_bduActivate enabled Block Data Update. See 'H' file for description
*
* @param    ptr  a pointer to a HTS221 struct
* @returns  1 always.
*/
int hts221_bduActivate(HTS221* ptr) 
{
    uint8_t data;

    data = ptr->rd_byte(CTRL_REG1);
    data |= BDU_SET;
    ptr->wr_byte(CTRL_REG1, data);
    return ptr->bduActive = 1;
}

/*! --------------------------------------------------------------------------------
* @brief    hts221_bduDeactivate disables Block Data Update. See 'H' file for description
*
* @param    ptr  a pointer to a HTS221 struct
* @returns  1 always.
*/
int hts221_bduDeactivate(HTS221* ptr) 
{
    uint8_t data;

    data = ptr->rd_byte(CTRL_REG1);
    data &= ~BDU_SET;
    ptr->wr_byte(CTRL_REG1, data);
    ptr->bduActive = 0;
    return 1;
}

/*! --------------------------------------------------------------------------------
* @brief    hts221_bduState disables Block Data Update. See 'H' file for description
*
* @param    ptr  a pointer to a HTS221 struct
* @returns  current state of BDU (0 disabled, 1 enabled)
*/
int hts221_bduState(HTS221* ptr) 
{
    return ptr->bduActive;
}

/*! --------------------------------------------------------------------------------
* @brief    hts221_readHumidity returns the HTS221 humidity reading
*
* @param    ptr  a pointer to a HTS221 struct
* @returns  double.
*/
double hts221_readHumidity(HTS221* ptr) 
{
    uint8_t  data   = 0;
    uint16_t h_out  = 0;
    double   _humid = -1.0;
    double   h_tmp  = 0.0;

    if( !ptr->active )
        hts221_activate(ptr);

    while( !(ptr->rd_byte(STATUS_REG) & HUMIDITY_READY ) )
        /*usleep(25000)*/;

    data = ptr->rd_byte(HUMIDITY_H_REG);
    h_out = data << 8;  // MSB
    data = ptr->rd_byte(HUMIDITY_L_REG);
    h_out |= data;      // LSB

    // Decode Humidity with x2 multiple removed
    h_tmp = ((int16_t)(ptr->_h1_rH) - (int16_t)(ptr->_h0_rH))/2.0;          

    // Calculate humidity in decimal of grade centigrades i.e. 15.0 = 150.
    _humid  = (double)(((int16_t)h_out - (int16_t)ptr->_H0_T0) * h_tmp) / 
              (double)((int16_t)ptr->_H1_T0 - (int16_t)ptr->_H0_T0);
    h_tmp   = (double)((int16_t)ptr->_h0_rH) / 2.0;                         // remove x2 multiple
    _humid += h_tmp;                                                        // provide signed % measurement unit
    return _humid;
}

/*! --------------------------------------------------------------------------------
* @brief    hts221_readTemperature returns the HTS221 temperature reading
*
* @param    ptr  a pointer to a HTS221 struct
* @returns  double.
*/
double hts221_readTemperature(HTS221* ptr) 
{
    uint8_t data   = 0;
    uint16_t t_out = 0;
    double deg     = 0.0;
    double _temp   = -1.0;

    if( !ptr->active )
        hts221_activate(ptr);

    while( !((ptr->rd_byte(STATUS_REG) & TEMPERATURE_READY) ) )
        /*usleep(25000)*/;

    data= ptr->rd_byte(TEMP_H_REG);
    t_out = data  << 8; // MSB
    data = ptr->rd_byte(TEMP_L_REG);
    t_out |= data;      // LSB

    // Decode Temperature with x8 multiplier removed
    deg    = (double)((int16_t)(ptr->_T1_degC) - (int16_t)(ptr->_T0_degC))/8.0; 

    // Calculate Temperature in decimal of grade centigrades i.e. 15.0 = 150.
    _temp = (double)(((int16_t)t_out - (int16_t)ptr->_T0_OUT) * deg) / 
            (double)((int16_t)ptr->_T1_OUT - (int16_t)ptr->_T0_OUT);
    deg   = (double)((int16_t)ptr->_T0_degC) / 8.0;     
    _temp += deg;  // signed celsius measurement unit

    return _temp;
}

