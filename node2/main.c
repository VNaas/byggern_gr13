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



int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer
    configure_uart();
    LED_init();
    LED_greenOff();
    LED_yellowOff();
    CAN_reader_init();
    can_init_def_tx_rx_mb();
    PWM_init();
    ADC_init();
    motor_init();
    solenoid_init();
    CAN_MESSAGE msg;
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
                    x = scale_measurement(y);
                r = msg.data[0];
                u = PID(x, r);
                control_motor(u);
                set_PWM(msg.data[1]);

                if (msg.data[2])
                {
                    trigger_solenoid();
                    LED_toggleGreen();
                }
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
    }
}