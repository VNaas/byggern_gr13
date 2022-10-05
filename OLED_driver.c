#include "OLED_driver.h"
#include "fonts.h"
#include <ctype.h>

volatile char* cmd_ptr = (char*) OLED_COMMAND_BASE;
volatile char* data_ptr = (char*) OLED_DATA_BASE;
volatile int fontsize = 4;

void OLED_init(void){
    OLED_write_command(0xae); // display off
    OLED_write_command(0xa1); //segment remap
    OLED_write_command(0xda); //common pads hardware: alternative
    OLED_write_command(0x12);
    OLED_write_command(0xc8); //common output scan direction:com63~com0
    OLED_write_command(0xa8); //multiplex ration mode:63
    OLED_write_command(0x3f);
    OLED_write_command(0xd5); //display divide ratio/osc. freq. mode
    OLED_write_command(0x80);
    OLED_write_command(0x81); //contrast control
    OLED_write_command(0x50);
    OLED_write_command(0xd9); //set pre-charge period
    OLED_write_command(0x21);
    OLED_write_command(0x20); //Set Memory Addressing Mode
    OLED_write_command(0x02);
    OLED_write_command(0xdb); //VCOM deselect level mode
    OLED_write_command(0x30);
    OLED_write_command(0xad); //master configuration
    OLED_write_command(0x00);
    OLED_write_command(0xa4); //out follows RAM content
    OLED_write_command(0xa6); //set normal display
    OLED_write_command(0xaf); // display on
}

void OLED_change_font_size(int size){
    if(size == 4 || size == 5 || size == 8){
        fontsize = size;
    }
    else{
        printf("Invalid fontsize. Valid sizes: 4, 5, 8.\r\n");
    }
}

void OLED_reset(void){
    //make whole screen light or dark?
    for (int i=0; i<8; i++){
        OLED_clear_line(i);
    }
    //perhaps return position to upper left corner?
    OLED_pos(0, 0);
}

void OLED_home(void){

}

void OLED_goto_line(int line){
    //assuming line equals page
    OLED_pos(line, 0);
}

void OLED_clear_line(uint8_t line){
    //assuming line equals page
    OLED_goto_line(line);
    for(int i=0; i<127; i++){
        OLED_write_data(0b00000000);
    }
}

void toBinary(uint8_t a)
{
    uint8_t i;

    for(i=0x80;i!=0;i>>=1)
        printf("%c",(a&i)?'1':'0');
}
void OLED_pos(uint8_t row, uint8_t col){
    OLED_write_command((0xB0 + row)); //
    uint8_t lower = col & 0b1111;
    uint8_t higher = (col & 0b11110000 ) >> 4;
    higher |= 0b10000;
    OLED_write_command(lower);
    OLED_write_command(higher);

}

void OLED_write_data(char c){
    data_ptr[0] = c;
}

void OLED_write_command(char c){
    cmd_ptr[0] = c;
}

void OLED_print(char* c){
    char* t;

    switch (fontsize)
    {
    case 4:
        for(t = c; *t != '\0'; t++){
            for(int i = 0; i < fontsize; i++){
                OLED_write_data( *(*(font4+*(t)-32))+i );
            }
        }
        break;
    case 5:
        for(t = c; *t != '\0'; t++){
            for(int i = 0; i < fontsize; i++){
                OLED_write_data( *(*(font5+*(t)-32))+i );
            }
        }
        break;
    case 8:
        for(t = c; *t != '\0'; t++){
            for(int i = 0; i < fontsize; i++){
                OLED_write_data( *(*(font8+*(t)-32))+i );
            }
        }
        break;
    default:
        printf("Error printing character\r\n");
        break;
    }
    // TODO: test
}

void OLED_set_brightness(uint8_t level){
    if(level > 255 || level < 0 ){
        printf("Brightness out of range, setting max. requested level: %d\r\n", level);
        OLED_write_command(0x81);
        OLED_write_command(255);
        return;
    }
    OLED_write_command(0x81);
    OLED_write_command(level);
}