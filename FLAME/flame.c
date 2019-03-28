/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

#include "flame.h"

/**
*   @file   flame.c
*   @brief  'C' code for MicroE Flame Click module.
*
*   @author James Flynn
*
*   @date   28-Feb-2019
*/


FLAME* open_flamedetect( float (*st)(void), int (*intSt)(void), void (*i)(void) )
{
    FLAME* ptr = (FLAME*)malloc(sizeof(FLAME));

    if( ptr ) {
        ptr->stat        =st;     //> status callback function
        ptr->intStat     =intSt;  //> interrupt status callback function
        ptr->init        =i;      //> initialization function
        ptr->cb          =NULL;   //> callback function for when a change in interrupt status occurs
        ptr->flameState  =0;      //> internal status of flame detection
        ptr->intLineState=0;      //> current state of the interrupt line
        }
    ptr->init();
    return ptr;
}

void   close_flamedetect( FLAME *ptr )
{
    free(ptr);
}

float   flame_status( FLAME* ptr)
{ 
    return ptr->stat();
}


int    flame_intstatus( FLAME* ptr)
{
    int current_state = ptr->intStat();
    if( ptr->intLineState != current_state && ptr->cb != NULL )
        ptr->cb(current_state);
    ptr->intLineState = current_state;
    return current_state;
}

void*  flame_setcallback( FLAME* ptr, void (*cb)(int) )
{
    void* tptr = ptr->cb;
    ptr->cb = cb;
    return tptr;
}


