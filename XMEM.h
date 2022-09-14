#ifndef XMEM_H
#define XMEM

#define XMEM_BASE 0x1800
#define XMEM_SIZE 0x800

void XMEM_init();

void XMEM_write(uint8_t data, uint16_t addr);

uint8_t XMEM_read(uint16_t addr);

#endif