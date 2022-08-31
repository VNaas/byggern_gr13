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

int main(void)
{
    /* Replace with your application code */
	//char cDDRA;
	//cDDRA = DDRA;
	DDRA |= 0b00000001;
	//char cPORTA;
	//cPORTA = PORTA;
	PORTA |= 0b00000001;
    while (1) 
    {
		
    }
}

