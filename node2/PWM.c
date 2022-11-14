#include "PWM.h"
#include "sam.h"

#define MCK 84E6

#define PERIOD_A 20E-3
#define DIVA 84
// #define CLK_A       1E6
#define CPRDA (int)(PERIOD_A * MCK / DIVA)
#define DIVB 84
#define PWM_period(ms) ((int)(ms * 84E6 / (84 * 1000)))
// #define CLK_B       1E6

void PWM_init()
{
    // enable peripheral function B for PIN45 and PIN44
    PIOC->PIO_ABSR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;

    // disable PIO from controlling PIN44 and PIN45
    PIOC->PIO_PDR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;

    // enable clock for PWM:    DIV = 0 (clk = MCK), CMD = 0 (read), PID = 36 (PWM)
    PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PWM << PMC_PCR_PID_Pos);
    PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);

    // enable PWM channels

    // set PWM clock A to 1 MHz and clock B to 1 MHz
    // PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(DIVA);
    PWM->PWM_CLK = 84 << PWM_CLK_DIVA_Pos | 0 << PWM_CLK_PREA_Pos;
    // PWM->PWM_CLK |= PWM_CLK_PREB(0) | PWM_CLK_DIVB(DIVB);

    // assign clocks to respective channels: clock A for channel 5 (PIN45), clock B for channel 6 (PIN44)
    // waveform set to be left-aligned
    // PWM->PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CPRE_CLKA;
    PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;

    // set PIN45 to a period to 20 ms, giving a frequency of 50 Hz - (MCK * clk_period) / X (X = prescaler value 2)
    PWM->PWM_CH_NUM[5].PWM_CPRD = PWM_period(20);
    // PWM->PWM_CH_NUM[5].PWM_CPRD = PWM_CPRD_CPRD(CPRDA);
    PWM->PWM_CH_NUM[5].PWM_CDTY = PWM_period(1.5);

    PWM->PWM_ENA = PWM_ENA_CHID5; // | PWM_ENA_CHID6;

    PWM->PWM_IER1 = PWM_IER1_CHID0; // Enable Counter Event interrupts on channel 0
    // set PIN44 to not initially have a pulse
}

void set_PWM(int8_t joy_pos)
{
    // printf("joy_pos: %d\n\r", joy_pos);
    int min_xpos = -128;
    int max_xpos = 68;
    double period;
    if(abs(joy_pos) < 17) PWM->PWM_CH_NUM[5].PWM_CDTY = PWM_period(1.5);
    if (joy_pos > 0)
    {
        period = (double)joy_pos / (2 * max_xpos) + 1.5;
        if(period > 2.1) period =  2.1;
    }
    else
    {
        period = (double)joy_pos / (2 * abs(min_xpos)) + 1.5;
        if(period < 0.9) period = 0.9;
    }
    PWM->PWM_CH_NUM[5].PWM_CDTY = PWM_period(period);
}