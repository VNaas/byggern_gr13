#include "ADC_driver.h"
#include <avr/io.h>
#include "macros.h"
#include <avr/interrupt.h>

volatile char ADC_data;

void ADC_init(void){
    set_bit(DDRD,DDD4);     // Set PD4 as output? No, BUSY pin will signal conversion
    set_bit(PORTD4, XCK0);  // Meant to set PD4 to output clock signal for ADC
    cli();
    // Enable interrupt on pin that receives signal that ADC conversion is completed
    // Busy pin git confersion finished
    // koble til og enable interrupt
    sei();
}

/**
 * @brief Interrupt Service Routine
 * Creates an isp for when ADC finishes conversion of analog signal.
 * We can then read the data stored in ADC_data.
 */
ISR(ADC_vect){
    ADC_data = 0; // Which register stores converted signal??
}

