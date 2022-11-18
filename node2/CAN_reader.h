#ifndef TIMERV2_H
#define TIMERV2_H
#include "can_node_2/can_controller.h"

void CAN_reader_init();

// void _delay_ms_v2(int ms);

// void _delay_us_v2(int us);

CAN_MESSAGE get_CAN_msg();

int get_CAN_flag();

void clear_CAN_flag();

#endif /* TIMERV2_H */