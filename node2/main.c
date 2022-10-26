#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_node_2/can_controller.h"
#include "LED.h"
#include "sam.h"

// void test_can()
// {
//     CAN_MESSAGE msg;
//     while(1){
//         if(!can_receive(&msg,0)){
//             for(int i = 0; i< msg.data_length; i ++)
//             {
//                 printf("%c",msg.data[i]);
//             }
//             printf("\r\n");
//         }
//     }
// }


int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    configure_uart();
    LED_init();
    LED_yellowOn();
    printf("Hello World");
    can_init_def_tx_rx_mb();
    CAN_MESSAGE received_msg;
    CAN_MESSAGE msg;
    while(1){
        if(!can_receive(&msg,0)){
            for(int i = 0; i< msg.data_length; i ++)
            {
                printf("%d",msg.data[i]);
            }
    //         printf("\r\n");
        }
    }
}