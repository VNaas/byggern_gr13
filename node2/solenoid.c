#include "solenoid.h"
#include "sam.h"

void solenoid_init(){
    //enable A0 pin on shield as ouput
    PIOA->PIO_PER |= PIO_PA16;
    PIOA-> PIO_OER |= PIO_PA16;
    //set as high as it is active low
    PIOA-> PIO_SODR |= PIO_PA16;

}

void trigger_solenoid(){
    //give solenois a pulse by setting the pin low for some time
    PIOA->PIO_CODR |=  PIO_PA16;
    _delay_ms(100);
    //back to high
    PIOA-> PIO_SODR |= PIO_PA16;

    //call this function on button interrupt somehow? Are we sending that through can? don't think so, but I think we need to.
}