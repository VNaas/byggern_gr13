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
// #include "timer.h"
// #include "timerv2.h"
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
    motor_disable();
    solenoid_init();
    LED_greenOff();
    LED_yellowOff();
    int position;
    CAN_MESSAGE msg;
    int busy = 1;
    while (1)
    {
        busy = can_receive(&msg, 1);
        if (!busy)
        {
            switch (msg.id)
            {
            case CAN_ID_JOY_POS:
                set_PWM(msg.data[1]);
                control_motor_from_joy_pos(msg.data[0]);
                break;

            case CAN_ID_BTN_PRESS:
                trigger_solenoid();
                LED_toggleGreen();
                break;

            case CAN_ID_MOTOR_ENABLE:
                motor_enable();
                break;

            case CAN_ID_MOTOR_DISABLE:
                motor_disable();
                break;

            default:
                printf("can message id: %d\n\r", msg.id);
                break;
            }
        }

        //read_decoder();
    }
}