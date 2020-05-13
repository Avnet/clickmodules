/** \file max30102.cpp ******************************************************
*
* Project: MAXREFDES117#
* Filename: max30102.cpp
* Description: This module is an embedded controller driver for the MAX30102
*
* Revision History:
*\n 1-18-2016 Rev 01.00 GL Initial release.
*\n 12-22-2017 Rev 02.00 Significantlly modified by Robert Fraczkiewicz
*
* --------------------------------------------------------------------
*
* This code follows the following naming conventions:
*
* char              ch_pmod_value
* char (array)      s_pmod_s_string[16]
* float             f_pmod_value
* int32_t           n_pmod_value
* int32_t (array)   an_pmod_value[16]
* int16_t           w_pmod_value
* int16_t (array)   aw_pmod_value[16]
* uint16_t          uw_pmod_value
* uint16_t (array)  auw_pmod_value[16]
* uint8_t           uch_pmod_value
* uint8_t (array)   auch_pmod_buffer[16]
* uint32_t          un_pmod_value
* int32_t *         pn_pmod_value
*
* ------------------------------------------------------------------------- */
/*******************************************************************************
* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/
#include "max30102.h"
#include "algorithm_by_RF.h"

//extern i2c_handle_t  i2c_handle;
static int (*_i2c_read)( uint8_t addr, uint16_t count, uint8_t* ptr );
static void (*_i2c_write)( uint8_t addr, uint16_t count, uint8_t* ptr );


/**
* \brief        Write a value to a MAX30102 register
* \par          Details
*               This function writes a value to a MAX30102 register
*
* \param[in]    uch_addr    - register address
* \param[in]    uch_data    - register data
*
* \retval       1 on success
*/
int maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
{
    _i2c_write(uch_addr, 1, &uch_data);
//    uint8_t  buff[2];
//    buff[0] = uch_addr;
//    buff[1] = uch_data;
//    i2c_write(i2c_handle, MAX30101_SAD, buff, 2, I2C_STOP);
    return 1;
}

/**
* \brief        Read a MAX30102 register
* \par          Details
*               This function reads a MAX30102 register
*
* \param[in]    uch_addr    - register address
* \param[out]   puch_data    - pointer that stores the register data
*
* \retval       1 on success
*/
int maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
{
    _i2c_read(uch_addr, 1, puch_data);
//    i2c_write(i2c_handle, MAX30101_SAD, &uch_addr, 1, I2C_NO_STOP);
//    i2c_read(i2c_handle, MAX30101_SAD, puch_data, 1);
    return 1;
}

void maxim_max30102_i2c_setup( int (*rd)( uint8_t addr, uint16_t count, uint8_t* ptr ), void (*wr)( uint8_t addr, uint16_t count, uint8_t* ptr ))
{
    _i2c_read = rd;
    _i2c_write= wr;
}


/**
* \brief        Initialize the MAX30102
* \par          Details
*               This function initializes the MAX30102
*
* \param        None
*
* \retval       1 on success
*/
int maxim_max30102_init()
{
  if(!maxim_max30102_write_reg(REG_INTR_ENABLE_1,0b11010000)) // INTR setting
    return 0;
  if(!maxim_max30102_write_reg(REG_INTR_ENABLE_2,0x00))
    return 0;
  if(!maxim_max30102_write_reg(REG_FIFO_WR_PTR,0x00))  //FIFO_WR_PTR[4:0]
    return 0;
  if(!maxim_max30102_write_reg(REG_OVF_COUNTER,0x00))  //OVF_COUNTER[4:0]
    return 0;
  if(!maxim_max30102_write_reg(REG_FIFO_RD_PTR,0x00))  //FIFO_RD_PTR[4:0]
    return 0;
  if(!maxim_max30102_write_reg(REG_FIFO_CONFIG,0x4f))  //sample avg = 4, fifo rollover=0, fifo almost full = 17
    return 0;
  if(!maxim_max30102_write_reg(REG_MODE_CONFIG,0x03))   //0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
    return 0;
  if(!maxim_max30102_write_reg(REG_SPO2_CONFIG,0x27))  // SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (411uS)
    return 0;
  
  if(!maxim_max30102_write_reg(REG_LED1_PA,0x24))   //Choose value for ~ 7mA for LED1
    return 0;
  if(!maxim_max30102_write_reg(REG_LED2_PA,0x24))   // Choose value for ~ 7mA for LED2
    return 0;
  if(!maxim_max30102_write_reg(REG_PILOT_PA,0x7f))   // Choose value for ~ 25mA for Pilot LED
    return 0;
  return 1;
}

/**
* \brief        Read a set of samples from the MAX30102 FIFO register
* \par          Details
*               This function reads a set of samples from the MAX30102 FIFO register
*
* \param[out]   *pun_red_led   - pointer that stores the red LED reading data
* \param[out]   *pun_ir_led    - pointer that stores the IR LED reading data
*
* \retval       1 on success
*/
int maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
  uint32_t un_temp;
  uint8_t uch_temp;
  uint8_t tmp[6];
  uint8_t addr = REG_FIFO_DATA;

  *pun_ir_led=0;
  *pun_red_led=0;
  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
  maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);

    _i2c_read(addr, 6, tmp);
//  i2c_write(i2c_handle, MAX30101_SAD, &addr, 1, I2C_NO_STOP);
//  i2c_read(i2c_handle, MAX30101_SAD, tmp, 6);

  un_temp=tmp[0];

  un_temp<<=16;
  *pun_red_led+=un_temp;
  un_temp=tmp[1];
  un_temp<<=8;
  *pun_red_led+=un_temp;
  un_temp=tmp[2];
  *pun_red_led+=un_temp;
  un_temp=tmp[3];
  un_temp<<=16;
  *pun_ir_led+=un_temp;
  un_temp=tmp[4];
  un_temp<<=8;
  *pun_ir_led+=un_temp;
  un_temp=tmp[5];
  *pun_ir_led+=un_temp;

  *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
  *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]
  return 1;
}

/**
* \brief        Reset the MAX30102
* \par          Details
*               This function resets the MAX30102
*
* \param        None
*
* \retval       1 on success
*/
int maxim_max30102_reset()
{
    if(!maxim_max30102_write_reg(REG_MODE_CONFIG,0x40))
        return 0;
    else
        return 1;    
}

uint8_t max30102_get_part_id()
{
    uint8_t tread;
    maxim_max30102_read_reg(0xff, &tread);
    return tread;
}

uint8_t max30102_get_revision()
{
    uint8_t tread;
    maxim_max30102_read_reg(0xfe, &tread);
    return tread;
}

void max301024_shut_down(int  yes)
{
    uint8_t temp;
    maxim_max30102_read_reg(REG_MODE_CONFIG, &temp);
    if (yes)
        temp |= (1 << 7);
    else
        temp &= ~(1 << 7);
    maxim_max30102_write_reg(REG_MODE_CONFIG,temp);
}

int max30102_data_available()
{
    uint8_t reg;

    maxim_max30102_read_reg(REG_INTR_STATUS_1, &reg);
//    if (reg & 0b10000000)
//        Log_Debug("data_available=%02X!\n", reg);
    return (reg & 0b10000000);
}

int max30102_finger_detected()
{
    uint8_t reg;


    if (!maxim_max30102_write_reg(REG_PROX_INT_THRESH, 0xff))
        return 0;
    if (!maxim_max30102_write_reg(REG_INTR_ENABLE_1, 0b00010000)) // INTR setting
        return 0;

    maxim_max30102_read_reg(REG_INTR_STATUS_1, &reg);
//    if (reg & 0b00010000)
//        Log_Debug("finger detect=%02X!\n", reg);

    if (!maxim_max30102_write_reg(REG_INTR_ENABLE_1, 0b11000000)) // INTR setting
        return 0;
    return (reg & 0b00010000);
}
