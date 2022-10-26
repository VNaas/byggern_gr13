#ifndef MPC2515_DRIVER_H
#define MPC2515_DRIVER_H

#include "mcp2515.h"
#include "inttypes.h"

#define CNF1 0x2A
#define CNF2 0x29
#define CNF3 0x28


char MCP2515_read(char address);

void MCP2515_write(char address,char *data, char length);

void MCP2515_set_mode(char mode);

void MCP2515_reset();

char MCP2515_init();

char MCP2515_read_status();

void MCP2515_bit_modify(char mask, char data, char address);

void MCP2515_request_to_send(char buffer);

#endif /* MPC2515_DRIVER_H */