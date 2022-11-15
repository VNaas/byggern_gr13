#include <avr/io.h>
#define F_CPU 4915200
#include <util/delay.h>

#include "game.h"

#include "CAN_driver.h"
#include "multifunction_board_driver.h"
#include "OLED.h"

#define IR_INTERRUPTED_ID 3 //some other value probably



/*called on play press*/
void play_game(){
    CAN_message msg;

    multifunction_board_start_sending();

    //send can msg of motor enable
    CAN_message motor_enable;
    motor_enable.id = 4;
    motor_enable.length = 1;
    motor_enable.data[0] = 1;

    CAN_transmit(msg);


    OLED_reset();
	OLED_pos(3, 8);
	OLED_print("    LET'S PLAY    ");

    uint32_t seconds_lasted = 0;
    uint8_t ms_from_second_increase = 0;


    while (1)
    {
        _delay_ms(1000);
        ms_from_second_increase ++;
        if(ms_from_second_increase == 1000){
            seconds_lasted ++;
            ms_from_second_increase = 0;
        }
        if(CAN_getFlag()){
            msg = CAN_receive();
            if (msg){
                printf("received can msg with id: %d", msg.id);
                if (msg.id == IR_INTERRUPTED_ID){
                    break;
                }
            }
        }

    }

    //print game over
    OLED_reset();
    OLED_pos(2, 8);
	OLED_print("    GAME OVER    ");
    OLED_pos(4, 8);
    OLED_print("You lasted for %d seconds", seconds_lasted);

    //return to menu on button press?

    multifunction_board_stop_sending();

    CAN_message motor_disable;
    motor_disable.id = 4;
    motor_disable.length = 1;
    motor_disable.data[0] = 0;



}