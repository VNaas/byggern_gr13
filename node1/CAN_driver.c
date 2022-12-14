#include "CAN_driver.h"
#include "MCP2515_driver.h"
#include "macros.h"
#include <avr/interrupt.h>
#include "OLED_driver.h"


#define MCP_TXB0CTRL_ID (MCP_TXB0CTRL + 0x1)    // 0x31
#define MCP_TXB0CTRL_DLC (MCP_TXB0CTRL + 0x5)   // 0x35
#define MCP_TXB0CTRL_DATA (MCP_TXB0CTRL + 0x6)  // 0x36

#define MCP_TXB1CTRL_ID (MCP_TXB1CTRL + 0x1)    // 0x41
#define MCP_TXB1CTRL_DLC (MCP_TXB1CTRL + 0x5)   // 0x45
#define MCP_TXB1CTRL_DATA (MCP_TXB1CTRL + 0x6)  // 0x46

#define MCP_TXB2CTRL_ID (MCP_TXB2CTRL + 0x1)    // 0x51
#define MCP_TXB2CTRL_DLC (MCP_TXB2CTRL + 0x5)   // 0x55
#define MCP_TXB2CTRL_DATA (MCP_TXB2CTRL + 0x6)  // 0x56

#define MCP_TXB1CTRL_ID (MCP_TXB1CTRL + 0x1)
#define MCP_TXB1CTRL_DLC 0x35
#define MCP_TXB1CTRL_DATA 0x36
#define MCP_TXB2CTRL_ID 0x31
#define MCP_TXB2CTRL_DLC 0x35
#define MCP_TXB2CTRL_DATA 0x36

volatile int receiveFlag;

void CAN_init()
{
    MCP2515_init();
    MCP2515_bit_modify(0b11, MCP_RX_INT, MCP_CANINTE); // enable all intrrupts
    MCP2512_setBaudRate();

    MCP2515_set_mode(MODE_NORMAL);
    char value = MCP2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_NORMAL)
    {
        printf("MCP2515 is NOT in normal mode after can init! \r\n");
    }
    MCP2515_bit_modify(0b01100000, 0xff, MCP_RXB0CTRL); // 11 = turn off filters

    clear_bit(DDRD, PD2); // Set interrupt pin input
    cli();
    // Interrupt on falling edge PD2 (INT0)
    clear_bit(MCUCR, ISC00);
    set_bit(MCUCR, ISC01);
    // Enable interrupt on PD2 (INT0)
    set_bit(GICR, INT0);

    sei();
}

void CAN_transmit(CAN_message msg)
{
    //printf("Can send: \n\r");
    char TX0ctrl = MCP2515_read(MCP_TXB0CTRL);
    char TX1ctrl = MCP2515_read(MCP_TXB1CTRL);
    char TX2ctrl = MCP2515_read(MCP_TXB2CTRL);
    // probably overkill to check if we are ready for a new transmission
    // maybe check CANINTF.TXnIF
    while (1)
    {
        if (!(TX0ctrl & 0b1000))
        {
            MCP2515_write(MCP_TXB0CTRL_ID, &msg.id, 1);
            MCP2515_write(MCP_TXB0CTRL_DLC, &msg.length, 1);
            MCP2515_write(MCP_TXB0CTRL_DATA, &msg.data, msg.length);
            MCP2515_request_to_send(MCP_RTS_TX0);
            return;
        }
        else if (!(TX1ctrl & 0b1000))
        {
            MCP2515_write(MCP_TXB1CTRL_ID, &msg.id, 1);
            MCP2515_write(MCP_TXB1CTRL_DLC, &msg.length, 1);
            MCP2515_write(MCP_TXB1CTRL_DATA, &msg.data, msg.length);
            MCP2515_request_to_send(MCP_RTS_TX1);
            return;
        }
        else if (!(TX2ctrl & 0b1000))
        {
            MCP2515_write(MCP_TXB2CTRL_ID, &msg.id, 1);
            MCP2515_write(MCP_TXB2CTRL_DLC, &msg.length, 1);
            MCP2515_write(MCP_TXB2CTRL_DATA, &msg.data, msg.length);
            MCP2515_request_to_send(MCP_RTS_TX2);
            return;
        }
    }
}

CAN_message CAN_receive()
{
    int buffer = MCP2515_read(MCP_CANINTF) && 0b11;
    CAN_message msg;
    char bufAddress;
    switch (buffer)
    {
    case 0b01:
        bufAddress = MCP_RXB0CTRL;
        break;
    case 0b10:
        bufAddress = MCP_RXB1CTRL;
        break;
    default:
        bufAddress = MCP_RXB0CTRL;
        break;
    }
    msg.id = MCP2515_read(bufAddress + 1);
    for (int i = 0; i < 8; i++)
    {
        msg.data[i] = MCP2515_read(bufAddress + 6 + i);
    }
    receiveFlag = 0;
    MCP2515_bit_modify(buffer, 0b0, MCP_CANINTF); // remove flag
    return msg;
}

int CAN_getFlag()
{
    return receiveFlag;
}

ISR(INT0_vect)
{
    receiveFlag = 1;
}
