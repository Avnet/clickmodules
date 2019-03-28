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

#ifndef __FLAME_H__
#define __FLAME_H__

#include <stdint.h>
#include <stdlib.h>

#define FLAME_SAD      0x5d

typedef struct flame_t {
    float  (*stat)(void);
    int    (*intStat)(void);
    void   (*init)(void);
    void   (*cb)(int);
    int    flameState;
    int    intLineState;
    } FLAME;

#ifdef __cplusplus
extern "C" {
#endif

FLAME* open_flamedetect( float (*stat)(void), int (*intStat)(void), void (*init)(void) );
void   close_flamedetect( FLAME *ptr );
float  flame_status( FLAME* ptr);
int    flame_intstatus( FLAME* ptr);
void*  flame_setcallback( FLAME* ptr, void (*cb)(int) );

#ifdef __cplusplus
}
#endif

#endif // __FLAME_H__



