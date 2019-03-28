/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

/**
*   @file   barometer.h
*   @brief  standard 'C' header for Mikroe Relay Click.
*
*   @author James Flynn
*
*   @date   28-Feb-2019
*/

#ifndef __RELAY_H__
#define __RELAY_H__

#include <stdint.h>
#include <stdlib.h>

typedef enum Relay { 
    relay1_rd =0,                      //> read relay 1
    relay2_rd,                      //> read relay 1
    relay1_set,                     //> set relay 1 state to 1
    relay2_set,                     //> set relay 2 state to 1
    relay1_clr,                     //> clear relay 1 clear to 1
    relay2_clr                      //> clear relay 2 clear to 1
    } Relay;

typedef struct relay_t {
    void (*init)(void);             //> function to initialize relay I/O
    void (*rstate)(struct relay_t *);  //> function to set the relay I/O state
    int  relay1_status;             //> reflects the state of relay 1
    int  relay2_status;             //> reflects the state of relay 2
    } RELAY;

#ifdef __cplusplus
extern "C" {
#endif

RELAY* open_relay( void (*rstate)(RELAY* ptr), void (*init)(void) );
void   close_relay( RELAY *ptr );
int    relaystate( RELAY* ptr, Relay r);

#ifdef __cplusplus
}
#endif

#endif // __RELAY_H__



