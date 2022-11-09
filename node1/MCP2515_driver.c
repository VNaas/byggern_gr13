#include "MCP2515_driver.h"
#include "macros.h"
#include "avr/io.h"
#include "SPI_driver.h"
#include "stdio.h"
#include "OLED_driver.h"
#define F_CPU 4915200
#include <util/delay.h>

char MCP2515_read(char address)
{
    clear_bit(PORTB, PB4);

    SPI_write(MCP_READ);
    SPI_write(address);
    char result = SPI_read();

    set_bit(PORTB, PB4);

    return result;
}

void MCP2515_write(char address, char *data, char length)
{
    clear_bit(PORTB, PB4);
    SPI_write(MCP_WRITE);
    SPI_write(address);
    for (int i = 0; i < length; i++)
    {
        // Get printed expected result in data and address
        SPI_write(data[i]);
    }
    set_bit(PORTB, PB4);
}

void MCP2515_set_mode(char mode)
{
    MCP2515_bit_modify(MODE_MASK, mode, MCP_CANCTRL);
}

void MCP2515_reset()
{
    clear_bit(PORTB, PB4);
    SPI_write(MCP_RESET);
    set_bit(PORTB, PB4);
    _delay_ms(10);
}

#define BRP 0
#define PRSEG 0
#define PHSEG1 3
#define PHSEG2 0
#define BTL 7

// void toBinary(char a)
// {
//     char i;

//     for(i=0x80;i!=0;i>>=1)
//         printf("%c",(a&i)?'1':'0');
// }

void MCP2512_setBaudRate()
{
    int8_t baud_conf = 0;
    char cnf1_val = 0;
    char cnf2_val = 0;
    char cnf3_val = 0;

    cnf1_val |= 0x3 << BRP;     // Baud rate prescaler bits BRP<5:0>

    cnf2_val |= 0x1 << PRSEG;   // Propagation segment length
    cnf2_val |= 0x6 << PHSEG1;
    cnf2_val |= 0x1 << BTL;

    cnf3_val |= 0x5 << PHSEG2; // 00000101
    // printf("\r\n");
    // toBinary(cnf1_val);
    // printf("\r\n");
    // toBinary(cnf2_val);
    // printf("\r\n");
    // toBinary(cnf3_val);


    MCP2515_write(MCP_CNF1,&cnf1_val,1);
    MCP2515_write(MCP_CNF2,&cnf2_val,1);
    MCP2515_write(MCP_CNF3,&cnf3_val,1);
}

/**
 * @brief Initializes the CAN controller (and SPI), returning 0 on success, 1 if there is an issue
 *
 * @return char
 */
char MCP2515_init()
{
    SPI_init();
    MCP2515_reset();

    // self-test
    char value = MCP2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_CONFIG)
    {
        printf("MCP2515 is NOT in configuration mode after reset! \r\n");
        return 1;
    }

    return 0;
}

char MCP2515_read_status()
{
    clear_bit(PORTB, PB4);
    SPI_write(MCP_READ_STATUS);
    char result = SPI_read();
    set_bit(PORTB, PB4);

    return result;
}

void MCP2515_bit_modify(char mask, char data, char address)
{
    clear_bit(PORTB, PB4);

    SPI_write(MCP_BITMOD);
    SPI_write(address);
    SPI_write(mask);
    SPI_write(data);

    set_bit(PORTB, PB4);
}

/**
 * @brief initiates message transmission sequence for the selected buffer.
 *
 * @param buffer
 */
void MCP2515_request_to_send(char transmitBuffer)
{
    clear_bit(PORTB, PB4);
    SPI_write(transmitBuffer);
    set_bit(PORTB, PB4);
}
