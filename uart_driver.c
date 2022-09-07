#include "uart_driver.h"

void USART_init(unsigned int ubrr){
    /* Set BAUD RATE */
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char)ubrr;
    /* Endable receiver and transmitter */
    UCSRB = (1 << RXEN) | (1 << TXEN);
    /* */
    UCRSC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0)
}

void USART_transmit(unsigned char data){
    /* Wait for empty transmit buffer */
    while( !(UCSRA & (1<<UDRE)) ){ // While not UDRE bit in UCSRA ie if not data empty, wait
        // Wait
    }
    /* Put data into buffer, sends the data */
    UDR = data;
}


unsigned char USART_receive(void){
    /* Wait for data to be received */
    while ( !(UCRSA & (1 << RXC)) ){ // If not Receive Complete
        // Wait
    }
    return UDR;
}
