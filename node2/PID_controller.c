#include "PID_controller.h"

static double Kp = 0.75;
static double Ki = 0.001;
static double Kd = 0;
static double dt = (double)1 / 30;
static double i_error = 0;
static int8_t e_prev = 0;

int8_t PID(int8_t y, int8_t r)
{
    int e = ((int)r - (int)y);
    if (abs(e) < 5)
        e = 0;
    double delta_e = (e - e_prev) / dt;
    i_error += dt * e;
    int8_t u_p = Kp * e;
    int8_t u_i = Ki * i_error;
    int8_t u_d = Kd * delta_e;
    e_prev = e;
    int8_t sum = u_p + u_i + u_d;
    if (sum > 100)
    {
        return 100;
    }
    if (sum < -100)
    {
        return -100;
    }
    return (int8_t)sum;
}
