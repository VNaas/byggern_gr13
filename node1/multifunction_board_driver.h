#ifndef MULTIFUNCTION_BOEARD_DRIVER_H
#define MULTIFUNCTION_BOEARD_DRIVER_H

#include <stdlib.h>
#include "inttypes.h"
struct joy_pos
{
    char x_pos;
    char y_pos;
};

enum joy_dir{LEFT,RIGHT,UP,DOWN,NEUTRAL};

int joystick_get_button_flag();

void joystick_reset_button_flag();

void multifunction_board_init();

void joystick_calibrate(void);

struct joy_pos joystick_getPos();

enum joy_dir joystick_get_direction();

int slider_left_get_pos();

int slider_right_get_pos();

void multifunction_board_start_sending();

void multifunction_board_stop_sending();



#endif /* MULTIFUNCTION_BOEARD_DRIVER_H */