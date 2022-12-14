
#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include <stdlib.h>
#include "inttypes.h"


#define OLED_COMMAND_BASE 0x1000
#define OLED_DATA_BASE 0x1200

void OLED_init(void);

void OLED_reset(void);

void OLED_home(void);

void OLED_goto_line(int line);

void OLED_clear_line(uint8_t line);

void OLED_pos(uint8_t row, uint8_t col);

void OLED_write_data(char c); // volatile

void OLED_write_command(char c);

void OLED_print(char* c);

void OLED_print_number(uint32_t c);

void OLED_set_brightness(uint8_t level);

void OLED_print_arrow(uint8_t row, uint8_t col);

void OLED_delete_arrow(uint8_t row);

void toBinary(uint8_t a);

#endif /* OLED_DRIVER_H */