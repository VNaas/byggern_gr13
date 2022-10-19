#include "LED.h"
#include <stdarg.h>
#include "sam.h"

void LED_init()
{
    PIOA -> PIO_PER = PIO_PA19 | PIO_PA20;
    PIOA -> PIO_OER = PIO_PA19 | PIO_PA20;
}

void LED_yellowOn()
{
    PIOA -> PIO_SODR = PIO_PA19;
}

void LED_yellowOff()
{
    PIOA -> PIO_CODR = PIO_PA19;
}

void LED_greenOn()
{
    PIOA -> PIO_SODR = PIO_PA20;
}

void LED_greenOff()
{
    PIOA -> PIO_CODR = PIO_PA20;
}
