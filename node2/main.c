#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_node_2/can_controller.h"
#include "can_node_2/can_interrupt.h"
#include "LED.h"
#include "sam.h"
#include "PWM.h"
#include "ADC.h"
#include "timer.h"
#include "motor_driver.h"
#include "DAC.h"
#include "solenoid.h"

// void test_can()
// {
//     CAN_MESSAGE msg;
//     while(1){
//         if(!can_receive(&msg,0)){
//             for(int i = 0; i< msg.data_length; i ++)
//             {
//                 printf(msg.data[i]);
//             }
//             printf("\r\n");
//         }
//     }
// }

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer
    configure_uart();
    LED_init();
    int LEDon = 1;
    // printf("Hello World\n\r");
    can_init_def_tx_rx_mb();
    PWM_init();
    ADC_init();
    motor_init();
    motor_enable();
    solenoid_init();
    LED_greenOff();
    LED_yellowOff();
    int position;
    CAN_MESSAGE msg;

    while (1)
    {
        // if (get_joy_pos_flag())
        // {
        //     // printf("joy_pos\n\r");
        //     LED_yellowOn();
        //     msg = get_can_message();
        //     set_PWM(msg.data[1]);
        //     control_motor_from_joy_pos(msg.data[0]);
        //     clear_joy_pos_flag();
        // }
        // if(get_btn_flag())
        // {
        //     printf("btn\n\r");
        //     trigger_solenoid();
        //     clear_btn_flag();
        // }
        _delay_ms(100);
        LED_toggleYellow();
    }
}