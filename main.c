/*
 * Byggern13.c
 *
 * Created: 31.08.2022 10:30:12
 * Author : jennl
 */

#include <avr/io.h>
#include "avr/iom162.h"
#include "stdio.h"
#include <avr/sleep.h>
#include "uart_driver.h"

int main(void)
{
	/* Turn on LED in PA0 */
	// DDRA |= 0b00000001;
	// PORTA |= 0b00000001;
    // while (1)
    // {}
	USART_init(MYUBRR);
	unsigned char myMessage = 'c';
	USART_transmit(myMessage);
	unsigned char myResponse = USART_receive();
	printf(myResponse);
}

	