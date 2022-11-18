#include "PID_controller.h"

static double Kp = 1;
static double Ki = 0;
static double Kd = 0;
static double dt = (double)1 / 30;
static double i_error = 0;
static double e_prev = 0;

int8_t PID(int8_t y, int8_t r)
{
    int8_t e = (r - y);
    double delta_e = (e - e_prev) / dt;
    i_error += dt * e;
    double u_p = Kp * e;
    double u_i = Ki * i_error;
    double u_d = Kd * delta_e;
    e_prev = e;
    double sum = u_p + u_i + u_d;
    return (int8_t)sum;
}
