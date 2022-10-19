#ifndef SRAM_DRIVER_H
#define SRAM_DRIVER_H

#include <stdlib.h>
#include <inttypes.h>
#define XMEM_BASE 0x1800
#define XMEM_SIZE 0x800

int XMEM_write(uint8_t data, uint16_t addr);

uint8_t XMEM_read(uint16_t addr);

#endif /* SRAM_DRIVER_H */ 