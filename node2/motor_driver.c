#include "motor_driver.h"
#include "sam.h"
#include "timer.h"
#include "toBinary.h"

#define EN (1 << 9)
#define DIR (1 << 10)
#define SEL (1 << 2)
#define NOT_OE (1 << 0)
#define NOT_RST (1 << 1)
#define ENCODER_DATA_PINS (0b11111111 << 1)

volatile uint16_t max_decoder_value;
volatile uint16_t min_decoder_value;

void motor_init(void)
{
    DAC_init();

    // MJ1 contains only input pins
    // PIOD pins must therefore be enabled as outputs:

    // NOT_OE = pin 25 = PD0
    // NOT_RST = pin 26 = PD1
    // SEL = pin 27 = PD2
    // EN =  pin 30 = PD9
    // DIR = pin 32 = PD10

    PIOD->PIO_PER |= EN | DIR | SEL | NOT_OE | NOT_RST;
    PIOD->PIO_OER |= EN | DIR | SEL | NOT_OE | NOT_RST;

    // MJ2 on the motorbox only contains output pins,
    // so all PIOC pins must be inputs

    // pin DO0-DO7 = pin 33-40 = PC1-PC8
    PIOC->PIO_PER |= ENCODER_DATA_PINS;
    PIOC->PIO_ODR |= ENCODER_DATA_PINS;
    PIOD->PIO_CODR |= NOT_RST;
    _delay_us(20);
    PIOD->PIO_SODR |= NOT_RST;

    // enable PIOC clock
    PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PIOC << PMC_PCR_PID_Pos);
    PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PIOD << PMC_PCR_PID_Pos);
    PMC->PMC_PCER0 |= 1 << (ID_PIOC);
    PMC->PMC_PCER0 |= 1 << (ID_PIOD);

    // toBinary((char)(PIOD->PIO_OSR &0xFF));
    PIOD->PIO_SODR = EN;

    control_motor(-40);
    _delay_ms(3000);
    reset_decoder();
    control_motor(40);
    _delay_ms(3000);
    control_motor(0);
    max_decoder_value = read_decoder();
}

void motor_enable()
{
    PIOD->PIO_SODR = EN;
    printf("motor enabled\n\r");
}

void motor_disable()
{
    set_motor_speed(0);
    PIOD->PIO_CODR |= EN;
    printf("motor disabled\n\r");
}

void set_motor_speed(uint16_t value)
{
    DAC_write(value);
}

int8_t scale_measurement(int16_t measurement)
{
    int8_t x = (200 * (measurement - max_decoder_value / 2) / max_decoder_value);
    if (x > 100)
        x = 100;
    if (x < -100)
        x = -100;
    return x;
}

void set_motor_direction(enum motor_direction dir)
{
    if (dir == LEFT)
    {
        PIOD->PIO_CODR |= DIR;
    }
    else if (dir == RIGHT)
    {
        PIOD->PIO_SODR |= DIR;
    }
}
void reset_decoder()
{
    PIOD->PIO_CODR |= NOT_RST;
    _delay_us(20);
    PIOD->PIO_SODR |= NOT_RST;
}
int read_decoder()
{

    // set !OE low to sample and hold the encoder value
    PIOD->PIO_CODR |= NOT_OE;

    // set SEL low to output high byte
    PIOD->PIO_CODR |= SEL;

    // wait 20us for output to settle
    _delay_us(20);

    // // read MJ2 to get high byte
    uint32_t high_byte = (PIOC->PIO_PDSR & ENCODER_DATA_PINS) >> 1;
    // // printf("high_byte: %d\n\r", high_byte);

    // // set SEL high to output low byte:
    PIOD->PIO_SODR |= SEL;

    // wait 20us for output to settle
    _delay_us(20);

    // // read MJ2 to get low value:
    uint32_t low_byte = (PIOC->PIO_PDSR & ENCODER_DATA_PINS) >> 1;
    // // printf("low_byte: %d\n\r", low_byte);

    // Reset encoder
    // PIOD->PIO_CODR |= NOT_RST;
    // _delay_us(20);
    // PIOD->PIO_SODR |= NOT_RST;

    // set not_oe to high:
    PIOD->PIO_SODR = NOT_OE;

    // combine high_byte and low_byte into result:
    uint16_t result = ((high_byte << 8) | low_byte);
    printf("decoder_result: %d\n\r", result);
    // maybe fix overflow?

    return result;
}

void control_motor(int8_t x_pos)
{
    int max_xpos = 100;
    int min_xpos = 100;

    int speed = 0;

    if (x_pos < 0)
    {
        set_motor_direction(LEFT);
        speed = (abs(x_pos) * (int)(4096 / abs(min_xpos)));
        if (speed >= 4096)
        {
            speed = 4095;
        }
    }
    else
    {
        set_motor_direction(RIGHT);
        speed = (abs(x_pos) * (int)(4096 / abs(max_xpos)));
        if (speed >= 4096)
        {
            speed = 4095;
        }
    }

    uint16_t uint_speed = (uint16_t)(speed);
    // printf("uint_speed: %d \n\r", uint_speed);
    set_motor_speed(uint_speed);
}
