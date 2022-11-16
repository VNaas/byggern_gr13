/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */
#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_

#include "can_node_2/can_controller.h"
#define CAN_ID_JOY_POS 13 // 2 on node 1?
#define CAN_ID_JOY_DIR
#define CAN_ID_BTN_PRESS 21 //3 on node 1?
#define CAN_ID_MOTOR_ENABLE 45 //5 on node 1
#define CAN_ID_MOTOR_DISABLE 61 //7 on  node 1


void CAN0_Handler       ( void );

int get_btn_flag();
int get_joy_pos_flag();
void clear_btn_flag();
void clear_joy_pos_flag();

CAN_MESSAGE get_can_message();




#endif /* CAN_INTERRUPT_H_ */