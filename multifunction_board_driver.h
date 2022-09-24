#ifndef MULTIFUNCTION_BOEARD_DRIVER_H
#define MULTIFUNCTION_BOEARD_DRIVER_H

#include <stdlib.h>
#include "inttypes.h"
struct joy_pos
{
    int8_t x_pos;
    int8_t y_pos
};

enum joy_dir{LEFT,RIGHT,UP,DOWN,NEUTRAL}

void joystick_init();

void joystick_calibrate(void);

struct joy_pos joystick_getPos();

enum joy_dir joystick_get_direction();

int slider_left_get_pos();

int slider_right_get_pos();


void joystick_getDir();


#endif /* MULTIFUNCTION_BOEARD_DRIVER_H */