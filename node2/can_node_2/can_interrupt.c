/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"

#include "../uart_and_printf/printf-stdarg.h"

#include "can_controller.h"
#include "../PWM.h"
#include "../motor_driver.h"
#include "../solenoid.h"
#include "LED.h"

#define DEBUG_INTERRUPT 0

volatile CAN_MESSAGE can_message;
volatile int joy_pos_flag = 1;
volatile int btn_flag = 0;

int get_btn_flag()
{
	return btn_flag;
}
int get_joy_pos_flag()
{
	if (joy_pos_flag)
		LED_greenOn();
	return joy_pos_flag;
}
void clear_btn_flag() { btn_flag = 0; }
void clear_joy_pos_flag() { joy_pos_flag = 0; }
CAN_MESSAGE get_can_message() { return can_message; }

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval
 */
void CAN0_Handler(void)
{

	if (DEBUG_INTERRUPT)
		printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR;

	// RX interrupt
	if (can_sr & (CAN_SR_MB1 | CAN_SR_MB2)) // Only mailbox 1 and 2 specified for receiving
	{
		CAN_MESSAGE message;
		if (can_sr & CAN_SR_MB1) // Mailbox 1 event
		{
			can_receive(&message, 1);
		}
		else if (can_sr & CAN_SR_MB2) // Mailbox 2 event

		{
			can_receive(&message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		switch (message.id)
		{
		case CAN_ID_JOY_POS:
			LED_toggleYellow();
			joy_pos_flag = 1;
			// set_PWM(message.data[1]);
			// control_motor_from_joy_pos(message.data[0]);
			can_message = message;
			break;

		case CAN_ID_BTN_PRESS:
			btn_flag = 1;
            trigger_solenoid();
			break;

		case CAN_ID_MOTOR_ENABLE: //this has the wrong value in can_interrupt.h
			if (message.data[0] == 1){
				motor_enable();
			}
			else{
				motor_disable();
			}
			break;


		default:
			printf("can message id: %d", message.id);
			break;
		}
		if (DEBUG_INTERRUPT)
			printf("message id: %d\n\r", message.id);
		if (DEBUG_INTERRUPT)
			printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++)
		{
			if (DEBUG_INTERRUPT)
				printf("%d ", message.data[i]);
		}
		if (DEBUG_INTERRUPT)
			printf("\n\r");
	}

	if (can_sr & CAN_SR_MB0)
	{
		if (DEBUG_INTERRUPT)
			printf("CAN0 MB0 ready to send \n\r");

		// Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;
	}

	if (can_sr & CAN_SR_ERRP)
	{
		if (DEBUG_INTERRUPT)
			printf("CAN0 ERRP error\n\r");
	}
	if (can_sr & CAN_SR_TOVF)
	{
		if (DEBUG_INTERRUPT)
			printf("CAN0 timer overflow\n\r");
	}

	NVIC_ClearPendingIRQ(CAN0_IRQn);
	// sei();*/
}
