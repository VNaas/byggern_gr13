#include "SRAM_driver.h"
#include "macros.h"
#include <avr/io.h>
#include <stdio.h>

int XMEM_write(uint8_t data, uint16_t addr){
    volatile char* ext_mem = (char*) XMEM_BASE;
        if(addr >= XMEM_SIZE){
        printf("Writing to address outside SRAM\n");
        return 0;   // TODO: ERROR HANDLING
    }
    ext_mem[addr] = data;
    return 1;   // TODO: ERROR HANDLING
}

uint8_t XMEM_read(uint16_t addr){
    volatile char* ext_mem = (char*) XMEM_BASE;
    if(addr >= XMEM_SIZE){
        printf("Reading from address outside SRAM\n");
        return 0;   // TODO: ERROR HANDLING
    }

    uint8_t data = ext_mem[addr];
    return data;
}