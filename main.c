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
#include <stdio.h>


#include <stdio.h>
static int uart_putchar(char c, FILE *stream);
static FILE uart_str = FDEV_SETUP_STREAM(USART_transmit, USART_receive, _FDEV_SETUP_RW);
static int
// uart_putchar(char c, FILE *stream)
// {


//   if (c == '0')
//     uart_putchar('', stream);


//   loop_until_bit_is_set(UCSRA, UDRE);


//   UDR = c;


//   return 0;
// }

int main(void)
{
	/* Turn on LED in PA0 */
	// DDRA |= 0b00000001;
	// PORTA |= 0b00000001;
    // while (1)
    // {}
	// FILE* mystream = fdevopen(&USART_transmit, &USART_receive)
	USART_init(MYUBRR);
	stdout = &uart_str;
	// unsigned char myResponse = USART_receive();
	printf("Hello world!");
}

	