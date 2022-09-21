#include "ADC_driver.h"
#include <avr/io.h>
#include "macros.h"
#include <avr/interrupt.h>
#include "XMEM.h"



volatile struct ADC_data adc_data;

enum ADC_channel{CH_0,CH_1,CH_2,CH_3}


void ADC_init(void){
    set_bit(DDRD,DDD4);         // Set PD4 as output

    // Set COM3A0:1 to 01 "Toggle OC3A on compare match"
    set_bit(TCCR3A,COM3A0);
    clear_bit(TCCR3A,COM3A1);

    // Set Waveform generation mode to 1101 PWM Phase correct,TOP=OCR3A, update OCR3A at TOP
    set_bit(TCCR3B,WGM33);
    set_bit(TCCR3B,WGM32);
    clear_bit(TCCR3A,WGM31);
    set_bit(TCCR3A,WGM30);

    // Set Clock Select 001 clk/1 no prescaling
    clear_bit(TCCR3B,CS32);
    clear_bit(TCCR3B,CS32);
    set_bit(TCCR3B,CS30);

    // Set TOP to value 0x00 => F_CLK/2
    OCR3A = 0x00;

    cli();
    // Interrupt on rising edge PE0
    set_bit(EMCUCR,ISC2);
    // Enable interrupt on PE0
    set_bit(GICR,INT2);
    sei();

}

/**
 * @brief Initiates ADC conversion. When conversion is finished, INT2_vect is called and adc_data is updated.
 *
 */
void ADC_start_conversion(){
    XMEM_write(0,ADC_BASE);
}

struct ADC_data ADC_get_data(void){
    return adc_data;
}


/**
 * @brief Interrupt Service Routine
 * Creates an isp for when ADC finishes conversion of analog signal.
 * We can then read the data stored in ADC_data.
 */
ISR(INT2_vect){
    adc_data.ch_0 = XMEM_read(ADC_BASE);
    adc_data.ch_1 = XMEM_read(ADC_BASE);
    adc_data.ch_2 = XMEM_read(ADC_BASE);
    adc_data.ch_3 = XMEM_read(ADC_BASE);
}

