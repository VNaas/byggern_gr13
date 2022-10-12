#include "CAN_driver.h"
#include "MCP2515_driver.h"
#include <avr/interrupt.h>

void CAN_init(){
    MCP2515_init();

    mcp_2515_write(MCP_CANINTE, MCP_RX_INT);
    MCP2515_set_mode(MODE_LOOPBACK);


    // set_bit(DDRD, PD2) // Set interrupt pin input
    cli();
    // Interrupt on rising edge PD2 (INT0)
    set_bit(MCUCR,ISC00);
    set_bit(MCUCR,ISC01);
    // Enable interrupt on PD2 (INT0)
    set_bit(GICR,INT0);

    sei();
}

ISR(INT0_vect){
    // READ CANINTF to see what triggered the interrupt on MCP

}