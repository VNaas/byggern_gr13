#include "SPI_driver.h"
#include "macros.h"
#include "avr/io.h"

void SPI_init()
{

    /* Set slave select and SCK as output */
    set_bit(DDRB, PB4); // *SS
    set_bit(DDRB, PB7); // SCK

    // io pin directions
    clear_bit(DDRB, PB6);   // MISO
    set_bit(DDRB, PB5);     // MOSI

    set_bit(SPCR, MSTR);    // Set Master mode
    set_bit(SPCR, SPR0);    // clock rate f_osc/16

    set_bit(PORTB,PB4);     // *SS high
    set_bit(SPCR,SPE);      //SPI enable
}

char SPI_read()
{
    SPDR = 0x01;    // Any data
    loop_until_bit_is_set(SPSR,SPIF);
    return SPDR;
}

void SPI_write(char data){
    SPDR = data;
    loop_until_bit_is_set(SPSR, SPIF);
}