#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H
#include "DAC.h"


enum motor_direction {
   LEFT,
   RIGHT
};

void motor_init(void);

void motor_enable();

void motor_disable();

void set_motor_speed(uint16_t value);

void set_motor_direction(enum motor_direction dir);

int read_decoder();

void control_motor_from_joy_pos(int8_t joy_pos);




#endif /* MOTOR_DRIVER_H */