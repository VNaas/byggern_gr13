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
#include "CAN_reader.h"
#include "motor_driver.h"
#include "DAC.h"
#include "solenoid.h"
#include "PID_controller.h"

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
    solenoid_init();
    LED_greenOff();
    LED_yellowOff();
    int position;
    CAN_MESSAGE msg;
    int busy = 1;

    printf("Hei\n\r");
    // trigger_solenoid();
    timer_v2_init();
    int16_t y; // Decoder value
    int8_t x;  // Scaled position [-100,100]
    int8_t u;  // Input to motor
    int8_t r;  // Reference signal

    while (1)
    {

        if (get_CAN_flag())
        {
            clear_CAN_flag();
            msg = get_CAN_msg();
            switch (msg.id)
            {
            case CAN_ID_JOY_AND_BTN:
                y = read_decoder();
                if (y != 0)
                    LED_toggleYellow();
                x = scale_measurement(y);
                r = msg.data[0];
                u = PID(x, r);
                control_motor(u);
                set_PWM(msg.data[1]);

                CAN_MESSAGE test_msg;
                test_msg.id = 0b110111;
                test_msg.data_length = 1;
                test_msg.data[0] = 1;
                can_send(&test_msg, 0);
                if (msg.data[2])
                    trigger_solenoid();
                break;

            case CAN_ID_BTN_PRESS:
                // LED_toggleGreen();
                LED_greenOn();
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
        // else
        // LED_toggleYellow();
        //     read_decoder();
        // LED_toggleGreen();
    }
}