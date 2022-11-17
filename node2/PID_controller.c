#include "PID_controller.h"

static double Kp = 3.0;
static double Ki = 0.01;
static double Kd = 0.01;
static double dt = 0.1;
static double i_error = 0;
static double e_prev = 0;


uint16_t PID(uint16_t y, uint16_t r)
{
    uint16_t e = (y-r);
    double delta_e = (e-e_prev)/dt;
    i_error += dt*e;
    double u_p = Kp*e;
    double u_i = Ki*i_error;
    e_prev = e;
    return u_p+u_i;
}
