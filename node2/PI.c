#include "PI.h"

static double Kp = 3.0;
static double Ki = 0.1;
static double dt = 0.1;
static double i_error = 0;

uint16_t PI(uint16_t y, uint16_t r)
{
    double up = Kp*(y-r);
    i_error += dt*(y-r);
    double ui = Ki*i_error;
    return up+ui;
}

//u = PI_controller(måling, joystick input)