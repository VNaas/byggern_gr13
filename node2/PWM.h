#ifndef PWM_H
#define PWM_H
#include "inttypes.h"

void PWM_init();

void set_PWM(int8_t joy_pos);

#endif /* PWM_H */