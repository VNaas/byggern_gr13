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

void latch_test(void){
	/* --------------latch test:--------------------*/
	DDRA |= 0b11111111; //alle er output pins
	DDRE |= (1<<PE1); //HUSK at ting må være output pins

	PORTA |= 0b11111111; //all pins high

	PORTE |= (1 << PE1); //latch disabled
	_delay_ms(3000); 

	PORTE &= ~(1 << PE1); //latch enable, all outputs of latch should be high regardless of input
	PORTA &=~((1 << PA0 ) | (1 << PA1 ) | (1 << PA7 ));
}

int main(void)
{
	/* Turn on LED in PA0 */
	DDRA |= 0b11111111;
	PORTA |= 0b00000001;

	/*latch enabling*/
	// PORTE |= 0b0; //Adress Latch Enable set to low

	// while (1)
    // {}



	// FILE* mystream = fdevopen(&USART_transmit, &USART_receive)
	USART_init(MYUBRR);
	// stdout = &uart_str;
	// unsigned char myResponse = USART_receive();
	while(1){
			USART_transmit('c');
			_delay_ms(1000);
	}
	return 0;

}
