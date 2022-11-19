/*
 * Byggern13.c
 *
 * Created: 31.08.2022 10:30:12
 * Author : jennl
 */

#include <avr/io.h>
#define F_CPU 4915200
#include <util/delay.h>
#include "uart_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include "ADC_driver.h"
#include "XMEM.h"
#include "multifunction_board_driver.h"
#include "OLED_driver.h"
#include "menu.h"
#include "CAN_driver.h"
#include "SPI_driver.h"


void latch_test(void)
{
	/* --------------latch test:--------------------*/
	DDRA |= 0b11111111; // alle er output pins
	DDRE |= (1 << PE1); // HUSK at ting må være output pins

	PORTA |= 0b11111111; // all pins high

	PORTE |= (1 << PE1); // latch disabled
	_delay_ms(3000);

	PORTE &= ~(1 << PE1); // latch enable, all outputs of latch should be high regardless of input
	PORTA &= ~((1 << PA0) | (1 << PA1) | (1 << PA7));
}

int main(void)
{
	USART_init(MYUBRR);
	XMEM_init();
	ADC_init();
	OLED_init();
	OLED_reset();
    CAN_init();

	multifunction_board_init();


	menu();

	while (1)
	{
		_delay_ms(1000);
	}
	return 0;
}
