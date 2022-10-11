#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

void SPI_init();

char SPI_read();

void SPI_write();

/*
READ                0000 0011
READ RX BUFFER      1001 0nm0
WRITE               0000 0010
RTS                 1000 0abc
READ STATUS         1010 0000
BIT MODIFY          0000 0101
*/
#endif /* SPI_DRIVER_H */