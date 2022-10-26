#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_node_2/can_controller.h"
#include "LED.h"
#include "sam.h"
#include "PWM.h"
#include "timer.h"

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
    LED_yellowOn();
    int LEDon = 1;
    printf("Hello World\n\r");
    can_init_def_tx_rx_mb();
    PWM_init();
    while (1)
    {
        CAN_MESSAGE received_msg;
        if (!can_receive(&received_msg, 0))
        {
            printf("MSG ID: %d\n\r", received_msg.id);
            if (LEDon)
            {
                LED_yellowOff();
                LEDon = 0;
            }
            else
            {
                LED_yellowOn();
                LEDon = 1;
            }
        }
        if (!can_receive(&received_msg, 1))
        {
            printf("\n\rData id: %d", received_msg.id);
            printf("\n\rData length: %d\n\r", received_msg.data_length);
            for (int i = 0; i < received_msg.data_length; i++)
            {
                // printf("sgjklag");
                printf("%d\n\r", received_msg.data[i]);
            }
            printf("\n\r");
        }
    }
}