#include "uart_driver.h"
#include "macros.h"

void USART_init(unsigned int ubrr){
    /* Set BAUD RATE */
    UBRR0H = (unsigned char)(ubrr>>8);  // fill 8 highest bits from 16 bit number
    UBRR0L = (unsigned char)ubrr;       // fill rest of bits

    /* Endable receiver and transmitter */
    UCSR0B = (1 << RXEN) | (1 << TXEN);

    /* 2 stop bits */
    UCRS0C = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);

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
    loop_until_bit_is_set(UCRS0A,RXC0);
    return UDR0;
}
