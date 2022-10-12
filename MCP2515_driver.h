#ifndef MPC2515_DRIVER_H
#define MPC2515_DRIVER_H

#include "mcp2515.h"
#include "inttypes.h"

uint8_t MCP2515_read(uint8_t address);

void MCP2515_write(uint8_t address,uint8_t *data, uint8_t length);

void MCP2515_set_mode(uint8_t mode);

void MCP2515_reset();

uint8_t MCP2515_init();

uint8_t MCP2515_read_status();

void MCP2515_bit_modify(uint8_t mask, uint8_t data, uint8_t address);

void MCP2515_request_to_send(uint8_t buffer);

#endif /* MPC2515_DRIVER_H */