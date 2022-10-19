#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_node_2/can_controller.h"
#include "LED.h"
#include "sam.h"


int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    configure_uart();
    LED_init();
    LED_yellowOn();
    printf("Hello World\n\r");
    int baudRate = 100000;
    can_init_def_tx_rx_mb(baudRate);
    while (1)
    {
        /* code */
    }
    
}