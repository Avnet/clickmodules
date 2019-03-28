/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

/**
* @file   flame.hpp
* @brief  Class for handling the FlameClick

* @author James Flynn
*
* @date   28-Feb-2019
*/

#ifndef __FLAMECLICK_HPP__
#define __FLAMECLICK_HPP__

#include "flame.h"

class Flame {
  private:
    gpio_handle_t flamePin;
    gpio_handle_t intPin;
    FLAME*        fptr;

    void init(void) {
        gpio_init(GPIO_PIN_XYZ, &flamePin);
        gpio_init(GPIO_PIN_XYZ, &intPin);
        }

    int intStatus(void) {
        int val;
        gpio_read(intPin,&val);
        return val;
        }

    int status(void) {
        int val;
        gpio_read(flamePin,&val);
        return val;
        }

  public:
    Flame(void) : (flamePin(0), intPin(0), flameState(false), intCB(NULL) 
        { 
        fptr = open_flamedetect( status, intStatus, init);
        }

    ~Flame(void) { 
        gpio_deinit(&flamePin);
        gpio_deinit(&intPin);
        close_flamedetect(fptr);
        }

    bool status(void) { return flame_status(fptr); }

    bool intStatus(void) { return flame_intstatus(fptr); }

    void setIntCallback(void (*i)(int *v)) { flame_setcallback( fptr,  v); }

};

#endif // __FLAMECLICK_HPP__

