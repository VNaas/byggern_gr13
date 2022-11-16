#include <avr/io.h>
#define F_CPU 4915200
#include <util/delay.h>

#include "game.h"

#include "CAN_driver.h"
#include "multifunction_board_driver.h"
#include "OLED_driver.h"

#define IR_INTERRUPTED_ID 6 //some other value probably



/*called on play press*/
void play_game(){
    CAN_message msg;



    //send can msg of motor enable
    CAN_message motor_enable;
    motor_enable.id = 5; 
    motor_enable.length = 1;
    motor_enable.data[0] = 1;
    CAN_transmit(motor_enable);



    multifunction_board_start_sending();

    OLED_reset();
	OLED_pos(3, 8);
	OLED_print("    LET'S PLAY    ");

    uint32_t seconds_lasted = 0;
    uint16_t ms_from_second_increase = 0;


    while (1)
    {
        _delay_ms(1);
        ms_from_second_increase ++;
        if(ms_from_second_increase == 1000){
            seconds_lasted ++;
            ms_from_second_increase = 0;
            // printf("a sec has passed\n\r");
        }
        if(CAN_getFlag()){
            msg = CAN_receive();

            printf("received can msg with id: %d\n\r", msg.id);
            if (msg.id == IR_INTERRUPTED_ID){
                break;
            }
        }
    }
    multifunction_board_stop_sending();

    CAN_message motor_disable;
    motor_disable.id = 7;
    motor_disable.length = 1;
    motor_disable.data[0] = 0;

    //print game over
    OLED_reset();
    OLED_pos(2, 8);
	OLED_print("    GAME OVER    ");
    OLED_pos(4, 8);
    //OLED_print("You lasted for %d seconds", seconds_lasted);

    //return to menu on button press?

    while (!joystick_get_button_flag());



}