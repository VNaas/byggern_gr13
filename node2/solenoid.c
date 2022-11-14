#include "solenoid.h"
#include "sam.h"
#include "timer.h"
#include "LED.h"
void solenoid_init(){
    //enable A0 pin on shield as ouput
    PIOA->PIO_PER |= PIO_PA16;
    PIOA-> PIO_OER |= PIO_PA16;
    //set as high as it is active low
    PIOA-> PIO_SODR |= PIO_PA16;

}

void trigger_solenoid(){
    //give solenoid a pulse by setting the pin low for some time
    PIOA-> PIO_CODR |= PIO_PA16;
    LED_yellowOn();
    _delay_ms(500);
    LED_yellowOff();
    // printf("Yo\n\r");
    // //back to high
     PIOA->PIO_SODR |=  PIO_PA16;


}