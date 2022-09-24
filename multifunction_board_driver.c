#include "multifunction_board_driver.h"
#include "ADC_driver.h"
#include "macros.h"
#include <avr/io.h>
#define F_CPU 4915200
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>



volatile int x_offset;
volatile int y_offset;

volatile struct joy_pos joystick_position;
volatile int left_slider;
volatile int right_slider;

struct joy_pos joystick_getPos(){
    return joystick_position;
}

enum joy_dir joystick_get_direction(){
    int threshold = 5;
    int x = joystick_position.x_pos;
    int y = joystick_position.y_pos;
    if ( x >= y){
        if(x < - threshold ){
            return LEFT;
        }
        if(x > threshold ){
            return RIGHT;
        }
    }
    else if (x < y)
    {
        if(y < - threshold ){
            return DOWN;
        }
        if(y > threshold ){
            return UP;
        }
    }
    return NEUTRAL;
}

int slider_left_get_pos(){
    return left_slider;
}

int slider_right_get_pos(){
    return right_slider;
}

void joystick_calibrate(void){
    ADC_start_conversion();
    _delay_ms(100);
    struct ADC_data adc_data = ADC_get_data();
    x_offset = adc_data.ch_0;
    y_offset = adc_data.ch_1;
}



void set_1024_prescaler(){
    set_bit(TCCR1B,CS12);
    clear_bit(TCCR1B,CS11);
    set_bit(TCCR1B,CS10);
}

void multifunction_board_init(){
    clear_bit(DDRB,PB0);  // right button
    clear_bit(DDRB,PB1);  // left button
    clear_bit(DDRB,PB2);  // joystick_button
    joystick_calibrate();

    cli();
    TCNT1 = 65535-(F_CPU/1024)/60;  // 60 Hz refresh rate
    TCCR1A = 0b00000000;
    set_1024_prescaler();
    set_bit(TIMSK,TOIE1);    // Enable timer1 overflow interrupt(TOIE1)

    sei();

}

ISR(TIMER1_OVF_vect){
    ADC_start_conversion();
    struct ADC_data adc_data = ADC_get_data();
    joystick_position.x_pos = (adc_data.ch_0 - x_offset) * 200 / 231;
    joystick_position.y_pos = (adc_data.ch_1 - y_offset) * 200 / 231;
    left_slider = (adc_data.ch_2 - 100) * 100 / 231;
    right_slider = (adc_data.ch_3 - 100) * 100 / 231;
    printf("x-pos: %i\r\n",joystick_position.x_pos);

    TCNT1 = 65535-(F_CPU/1024)/60;  // 60 Hz refresh rate, reset timer

}