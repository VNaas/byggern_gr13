#include "LED.h"
#include <stdarg.h>
#include "sam.h"

static uint32_t yellowOn = 0;
static uint32_t greenOn = 0;

void LED_init()
{
    PIOA->PIO_PER = PIO_PA19 | PIO_PA20;
    PIOA->PIO_OER = PIO_PA19 | PIO_PA20;
}

void LED_yellowOn()
{
    PIOA->PIO_SODR = PIO_PA19;
    yellowOn = 1;
}

void LED_yellowOff()
{
    PIOA->PIO_CODR = PIO_PA19;
    yellowOn = 0;
}

void LED_toggleYellow()
{
    if (yellowOn)
    {
        PIOA->PIO_CODR = PIO_PA19;
        yellowOn = 0;
    }
    else
    {
        PIOA->PIO_SODR = PIO_PA19;
        yellowOn = 1;
    }
}

void LED_greenOn()
{
    PIOA->PIO_SODR = PIO_PA20;
    greenOn = 1;
}

void LED_greenOff()
{
    PIOA->PIO_CODR = PIO_PA20;
    greenOn = 0;
}

void LED_toggleGreen()
{
    if (greenOn)
    {
        PIOA->PIO_CODR = PIO_PA20;
        greenOn = 0;
    }
    else
    {
        PIOA->PIO_SODR = PIO_PA20;
        greenOn = 1;
    }
}