
#include <avr/io.h>
#include <avr/interrupt.h>
init(){

    // Interrupt on rising edge PD2 (INT0)
    set_bit(MCUCR,ISC00);
    set_bit(MCUCR,ISC01);
    // Enable interrupt on PD2 (INT0)
    set_bit(GICR,INT0);

    sei();
}


ISR