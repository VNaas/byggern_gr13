#include "MCP2515_driver.h"
#include "avr/io.h"
#include "macros.h"
#include "SPI_driver.h"
#include "stdio.h"

char MCP2515_read(char address)
{
    clear_bit(PORTB, PB0);

    SPI_write(MCP_READ);
    SPI_write(address);
    char result = SPI_read();

    set_bit(PORTB, PB0);

    return SPI_read();
}

void MCP2515_write(char[] byteArray, char address, char length)
{
    clear_bit(PORTB, PB0);

    SPI_write(MCP_WRITE);
    SPI_write(address);
    for (i = 0; i < length; i++)
    {
        SPI_write(byteArray[i]);
    }
    set_bit(PORTB, PB0);
}

void MCP2515_set_mode(char mode)
{
    MCP2515_write(MCP_CANSTAT, mode);
}

void MCP2515_reset()
{
    clear_bit(PORTB, PB0);
    SPI_write(MCP_RESET);
    set_bit(PORTB, PB0);
}

/**
 * @brief Initializes the CAN controller (and SPI), returning 0 on success, 1 if there is an issue
 *
 * @return char
 */
char MCP2515_init()
{
    char value;
    SPI_init();
    MCP2515_reset();
    // self-test
    value = MCP2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_CONFIG)
    {
        printf("MCP2515 is NOT in configuration mode after reset! \r\n");
        return 1;
    }

    // MCP2515_set_mode(MODE_LOOPBACK) ?
    return 0;
}

char CMP2515_read_status()
{
    clear_bit(PORTB, PB0);
    char result = SPI_read();
    set_bit(PORTB, PB0);

    return result;
}

void MCP2515_bit_modify(char mask, char data, char address)
{
    clear_bit(PORTB, PB0);

    SPI_write(MCP_BITMOD);
    SPI_write(address);
    SPI_write(mask);
    SPI_write(data);

    set_bit(PORTB, PB0);
}

/**
 * @brief initiates message transmission sequence for the selected buffer.
 * TX0: 0b001
 * TX1: 0b010
 * TX2: 0b100
 * all: 0b111
 *
 * @param buffer
 */
void MCP2515_request_to_send(char buffer)
{
    clear_bit(PORTB, PB0);

    SPI_write(0x80 | buffer);

    set_bit(PORTB, PB0);
}
