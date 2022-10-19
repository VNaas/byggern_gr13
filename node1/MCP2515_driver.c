#include "MCP2515_driver.h"
#include "macros.h"
#include "avr/io.h"
#include "SPI_driver.h"
#include "stdio.h"
#include "OLED_driver.h"
#define F_CPU 4915200
#include <util/delay.h>

uint8_t MCP2515_read(uint8_t address)
{
    clear_bit(PORTB, PB4);

    SPI_write(MCP_READ);
    SPI_write(address);
    uint8_t result = SPI_read();

    set_bit(PORTB, PB4);

    return result;
}

void MCP2515_write(uint8_t address, uint8_t *data, uint8_t length)
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

void MCP2515_set_mode(uint8_t mode)
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
void MCP2512_setBaudRate()
{
    int8_t baud_conf = 0;
    uint8_t cnf1_val;
    uint8_t cnf2_val;
    uint8_t cnf3_val;

    cnf1_val |= 3 << BRP;     // Baud rate prescaler bits BRP<5:0> : 125 000

    cnf2_val |= 1 << PRSEG;          // Propagation segment length 2* T_Q
    cnf2_val |= 6 << PHSEG1;
    cnf2_val |= 1 << BTL;

    cnf3_val |= 5 << PHSEG2;

    MCP2515_write(CNF1,&cnf1_val,1);
    MCP2515_write(CNF2,&cnf2_val,1);
    MCP2515_write(CNF3,&cnf3_val,1);
}

/**
 * @brief Initializes the CAN controller (and SPI), returning 0 on success, 1 if there is an issue
 *
 * @return uint8_t
 */
uint8_t MCP2515_init()
{
    SPI_init();
    MCP2515_reset();

    // self-test
    uint8_t value = MCP2515_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_CONFIG)
    {
        printf("MCP2515 is NOT in configuration mode after reset! \r\n");
        return 1;
    }

    return 0;
}

uint8_t MCP2515_read_status()
{
    clear_bit(PORTB, PB4);
    SPI_write(MCP_READ_STATUS);
    uint8_t result = SPI_read();
    set_bit(PORTB, PB4);

    return result;
}

void MCP2515_bit_modify(uint8_t mask, uint8_t data, uint8_t address)
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
void MCP2515_request_to_send(uint8_t transmitBuffer)
{
    clear_bit(PORTB, PB4);
    SPI_write(transmitBuffer);
    set_bit(PORTB, PB4);
}
