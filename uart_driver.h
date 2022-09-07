
#include <avr/io.h>
#include "avr/iom162.h"
#include "stdio.h"
#include <avr/sleep.h>

#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1       // See ATmega162 pg 169 for table, /16 is async normal mode

void USART_init(unsigned int ubrr);

int USART_transmit(unsigned char data, FILE* stream);

unsigned char USART_receive(void, FILE* stream);

#endif /* UART_DRIVER_H */