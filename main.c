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
// static int uart_putchar(char c, FILE *stream);
// static FILE uart_str = FDEV_SETUP_STREAM(USART_transmit, USART_receive, _FDEV_SETUP_RW);
// // uart_putchar(char c, FILE *stream)
// {


//   if (c == '0')
//     uart_putchar('', stream);


//   loop_until_bit_is_set(UCSRA, UDRE);


//   UDR = c; how about now


//   return 0;
// }

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
