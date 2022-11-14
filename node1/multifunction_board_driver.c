#include "multifunction_board_driver.h"
#include "ADC_driver.h"
#include "macros.h"
#include <avr/io.h>
#define F_CPU 4915200
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "CAN_driver.h"

#define REFRESH_RATE 30
volatile int x_offset;
volatile int y_offset;

volatile struct joy_pos joystick_position;
volatile char left_slider;
volatile char right_slider;

volatile int button_flag = 0;
volatile int send = 0;


int joystick_get_button_flag(){
    return button_flag;
}

void joystick_reset_button_flag(){
    button_flag = 0;
}


struct joy_pos joystick_getPos(){
    return joystick_position;
}

enum joy_dir joystick_get_direction(){
    int threshold = 10;
    int x = joystick_position.x_pos;
    int y = joystick_position.y_pos;
    if ( abs(x) >= abs(y)){
        if(x < - threshold ){
            return LEFT;
        }
        if(x > threshold ){
            return RIGHT;
        }
    }
    else if (abs(x) < abs(y))
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

    clear_bit(DDRD,PB3);  // joystick button


    joystick_calibrate();

    cli();
    TCNT1 = 65535-(F_CPU/1024)/REFRESH_RATE;  // 60 Hz refresh rate
    TCCR1A = 0b00000000;
    set_1024_prescaler();
    set_bit(TIMSK,TOIE1);    // Enable timer1 overflow interrupt(TOIE1)

    // Interrupt on rising edge PD3 (INT1)
    set_bit(MCUCR,ISC10);
    set_bit(MCUCR,ISC11);
    // Enable interrupt on PD3 (INT1)
    set_bit(GICR,INT1);


    sei();

}

void send_joy_pos()
{
    CAN_message msg;
    msg.id = 1;
    msg.length = 2;
    msg.data[0] = joystick_position.x_pos;
    msg.data[1] = joystick_position.y_pos;
    msg.data[2] = left_slider;
    msg.data[3] = right_slider;
    CAN_transmit(msg);

}

void joystick_start_sending()
{
    send = 1;
}

joystick_stop_sending()
{
    send = 0;
}

ISR(INT1_vect){
    button_flag = 1;

    CAN_message msg;
    msg.id = 2;
    msg.length = 1;
    msg.data[0] = 1;

    CAN_transmit(msg);
}

ISR(TIMER1_OVF_vect){
    ADC_start_conversion();
    struct ADC_data adc_data = ADC_get_data();
    joystick_position.x_pos = (adc_data.ch_0 - x_offset) * 200 / 255;
    joystick_position.y_pos = (adc_data.ch_1 - y_offset) * 200 / 255;
    left_slider = (adc_data.ch_2) * 100 / 255;
    right_slider = (adc_data.ch_3) * 100 / 255;
    // printf("x_pos: %d \r\n",joystick_position.x_pos);
    if(send) send_joy_pos();
    printf("joy_pos sent: %d\r\n", joystick_position.x_pos);

    TCNT1 = 65535-(F_CPU/1024)/REFRESH_RATE;  // reset timer

}