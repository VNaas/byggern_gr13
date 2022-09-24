#include "ADC_driver.h"
#include "macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile struct ADC_data adc_data;
volatile char* adc_ptr = (char*) ADC_BASE;

void set_ctc_mode(void){
    clear_bit(TCCR3B,WGM33);
    set_bit(TCCR3B,WGM32);
    clear_bit(TCCR3A,WGM31);
    clear_bit(TCCR3A,WGM30);
}

void set_phase_correct_mode(void){
    set_bit(TCCR3A,WGM30);
    clear_bit(TCCR3A,WGM31);
    set_bit(TCCR3B,WGM32);
    set_bit(TCCR3B,WGM33);
}

void set_no_prescaling(void){
    clear_bit(TCCR3B, CS32);
    clear_bit(TCCR3B, CS31);
    set_bit(TCCR3B, CS30);
}

void set_toggle_on_compare_match(void){
    clear_bit(TCCR3A,COM3A1);
    set_bit(TCCR3A,COM3A0);
}
/**
 * @brief Initialize ADC
 *
 */
void ADC_init(void){
    set_bit(DDRD,DDD4);         // Set PD4 as output
    set_ctc_mode();
    set_no_prescaling();
    set_toggle_on_compare_match();
    
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
    adc_ptr[0] = 0x00;
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
    adc_data.ch_0 = adc_ptr[0x00];
    adc_data.ch_1 = adc_ptr[0x00];
    adc_data.ch_2 = adc_ptr[0x00];
    adc_data.ch_3 = adc_ptr[0x00];
}

