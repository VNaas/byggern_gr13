/*
 * Byggern13.c
 *
 * Created: 31.08.2022 10:30:12
 * Author : jennl
 */

#include <avr/io.h>
#define F_CPU 4915200
#include <util/delay.h>
#include "uart_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include "ADC_driver.h"
#include "XMEM.h"
#include "multifunction_board_driver.h"
#include "OLED_driver.h"
#include "menu.h"
#include "CAN_driver.h"
#include "SPI_driver.h"
// #include <stdarg.h>
// #include "uart_and_printf/uart.h"
// #include "uart_and_printf/printf-stdarg.h"

// #include "sam.h"


void SRAM_test(void)
{
	volatile char *ext_ram = (char *)0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\r\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++)
	{
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value)
		{
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++)
	{
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value)
		{
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \r\n%4d errors in write phase and \r\n%4d errors in retrieval phase\r\n\r\n", write_errors, retrieval_errors);
}

void latch_test(void)
{
	/* --------------latch test:--------------------*/
	DDRA |= 0b11111111; // alle er output pins
	DDRE |= (1 << PE1); // HUSK at ting må være output pins

	PORTA |= 0b11111111; // all pins high

	PORTE |= (1 << PE1); // latch disabled
	_delay_ms(3000);

	PORTE &= ~(1 << PE1); // latch enable, all outputs of latch should be high regardless of input
	PORTA &= ~((1 << PA0) | (1 << PA1) | (1 << PA7));
}

int main(void)
{
	USART_init(MYUBRR);
	XMEM_init();
	// SRAM_test();
	ADC_init();
	OLED_init();
	OLED_reset();
    CAN_init();

	// SystemInit();

    // WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    // configure_uart();

	multifunction_board_init();
	printf("Hello\r\n");


	menu();
	//multifunction_board_start_sending();

	// CAN_message transmitMsg;
	// // transmitMsg.id = 4;
	// transmitMsg.data[0] = 'B';
	// transmitMsg.data[1] = 'O';
	// transmitMsg.data[2] = 'Y';
	// transmitMsg.data[3] = '\0';
	// // transmitMsg.data[4] = 'h';
	// // transmitMsg.data[5] = 'h';
	// // transmitMsg.data[6] = 'h';
	// // transmitMsg.data[7] = '\0';
	// transmitMsg.length = 4;
	// int counter = 0;
	while (1)
	{
		// counter = (counter +1 ) % 0xFF;
		// transmitMsg.id = counter;
		// printf("Transmitting\r\n");
		// CAN_transmit(transmitMsg);
		// if(CAN_getFlag())
		// {
		// 	CAN_message receiveMsg = CAN_receive();
		// 	printf("Received: \r\n\t ID: %d\r\n\t",receiveMsg.id);
		// 	printf("Data: ");
		// 	char* p_c = receiveMsg.data;
		// 	printf(receiveMsg.data);
		// 	printf("\r\n");
		// }
		_delay_ms(1000);
	}
	return 0;
}
