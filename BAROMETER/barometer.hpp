/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

/**
* @file   barometer.hpp
* @brief  Class for managing a ST LPS25HB sensor.
*         the class simply pulls the 'C' functions into a comprehensive call so the 
*         LPS25HB can be treated as a single object.

* @author James Flynn
*
* @date   28-Feb-2019
*/

#ifndef __BAROMETER_HPP__
#define __BAROMETER_HPP__

#include "barometer.h"

class Barometer : public i2c_interface {
  private:
    friend class i2c_interface;
    BAROMETER* _bptr;

    static uint8_t lps25hb_read_byte(uint8_t reg_addr) {
        return (get_i2c_handle())->read_i2c(LPS25HB_SAD, reg_addr);
        }

    static void lps25hb_write_byte(uint8_t reg_addr, uint8_t value) {
        uint8_t buffer_sent[2] = {reg_addr, value};
        (get_i2c_handle())->write_i2c(LPS25HB_SAD, buffer_sent, 2);
        }

  public:
    Barometer(void) { _bptr = open_barometer(&lps25hb_read_byte, &lps25hb_write_byte); }
    ~Barometer(void) { close_barometer(_bptr); }

    int who_am_i(void) { return barometer_who_am_i(_bptr); } 
    float get_pressure(void) { return barometer_get_pressure(_bptr); }
    float get_tempC(void) { return barometer_get_tempC(_bptr); }
    float get_tempF(void) { return barometer_get_tempF(_bptr); }
};

#endif // __BAROMETER_HPP__

