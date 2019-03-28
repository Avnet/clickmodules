
#include "relay.h"

RELAY* open_relay( void (*rstate)(RELAY* ptr), void (*init)(void) )
{
    RELAY* ptr = (RELAY*)malloc(sizeof(RELAY));

    if( ptr ) {
        ptr->init = init;         //> initialize relay I/O
        ptr->rstate=rstate;   //> read/set relay I/O state
        ptr->relay1_status = 0;   //> status (0/1) of relay 1
        ptr->relay2_status = 0;   //> status (0/1) of relay 2
        }
    ptr->init();
    return ptr;
}


void   close_relay( RELAY *ptr )
{
    free(ptr);
}

int    relaystate( RELAY* ptr, Relay r)
{
    int s = -1;
    switch( r ) {
        case relay1_rd:
            s = ptr->relay1_status;
            break;

        case relay1_set:
            s = ptr->relay1_status = 1;
            ptr->rstate(ptr);
            break;

        case relay1_clr:
            s = ptr->relay1_status = 0;
            ptr->rstate(ptr);
            break;

        case relay2_rd:
            s = ptr->relay2_status;
            break;

        case relay2_set:
            s = ptr->relay2_status = 1;
            ptr->rstate(ptr);
            break;

        case relay2_clr:
            s = ptr->relay2_status = 0;
            ptr->rstate(ptr);
            break;
        }


    return s;
}


