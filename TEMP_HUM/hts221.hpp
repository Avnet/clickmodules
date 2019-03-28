/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

/**
*   @file   hts221.cpp
*   @brief  class definition for the ST HTS221 sensor,
*
*   @author James Flynn
*
*   @date   1-Oct-2018
*/

#ifndef __HTS221_HPP__
#define __HTS221_HPP__

#include "hts221.h"

class Hts221 : public i2c_interface
{
private:
    friend class i2c_interface;
    HTS221* _ptr;
    static uint8_t hts221_read_byte(uint8_t reg_addr) {
        return (get_i2c_handle())->read_i2c(HTS221_SAD, reg_addr);
        }

    static void hts221_write_byte(uint8_t reg_addr, uint8_t value) {
        uint8_t buffer_sent[2] = {reg_addr, value};
        (get_i2c_handle())->write_i2c(HTS221_SAD, buffer_sent, 2);
        }

public:
    void Activate(void) { hts221_activate(_ptr); }
    void Deactivate(void) { hts221_deactivate(_ptr); }

    bool bduActivate(void) { return hts221_bduActivate(_ptr); }
    bool bduDeactivate(void){ return hts221_bduDeactivate(_ptr); }

    double readHumidity(void) { return hts221_readHumidity(_ptr); }
    double readTemperature(void) { return hts221_readTemperature(_ptr); }

    int active(void) { return hts221_active(_ptr); }
    int bduState(void) { return hts221_bduState(_ptr); }

    int who_am_i(void) { return hts221_who_am_i(_ptr); }

    Hts221() {
        _ptr = open_hts221(hts221_read_byte, hts221_write_byte);
        }

    ~Hts221() {
         close_hts221(_ptr);
         }
};

#endif // __HTS221_HPP__

