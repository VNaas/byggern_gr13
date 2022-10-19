#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include "avr/io.h"
#include "inttypes.h"

typedef struct CAN_message
{
    uint8_t id;
    uint8_t data[8];
    uint8_t length;
}CAN_message;

void CAN_init();


void CAN_transmit(CAN_message msg);

CAN_message CAN_receive();

int CAN_getFlag();

#endif /* CAN_DRIVER_H */