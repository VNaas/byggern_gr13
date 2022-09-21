#include "macros.h"
#include <avr/io.h>
#include <avr/iom162.h>
#include <stdio.h>
#include <avr/sleep.h>
#include "uart_driver.h"

void USART_init(unsigned int ubrr){
    /* Set BAUD RATE */
    UBRR0H = (unsigned char)(ubrr>>8);  // fill 8 highest bits from 16 bit number
    UBRR0L = (unsigned char)ubrr;       // fill rest of bits

    /* Endable receiver and transmitter */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    /* 2 stop bits */
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

    fdevopen(USART_transmit,USART_receive);
}

int USART_transmit(unsigned char data){
    /* Wait for empty transmit buffer */
    loop_until_bit_is_set(UCSR0A,UDRE0);
    /* Put data into buffer, sends the data */
    UDR0 = data;
    return 0;
}


unsigned char USART_receive(void){
    /* Wait for data to be received */
    loop_until_bit_is_set(UCSR0A,RXC0);
    return UDR0;
}
