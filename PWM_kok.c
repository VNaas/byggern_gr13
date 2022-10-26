#include "pwm.h"
#include "sam/sam3x/include/sam.h"


#define MCK         84E6

#define PERIOD_A    20E-3
#define DIVA        84
#define CLK_A       1E6
#define CPRDA       (int) (PERIOD_A * MCK / DIVA)

#define DIVB        84
#define CLK_B       1E6


void pwm_init() {
    // enable peripheral function B for PIN45 and PIN44
    PIOC->PIO_ABSR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;

    // disable PIO from controlling PIN44 and PIN45
    PIOC->PIO_PDR |= PIO_PC19B_PWMH5 | PIO_PC18B_PWMH6;

    // enable clock for PWM:    DIV = 0 (clk = MCK), CMD = 0 (read), PID = 36 (PWM)
    PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PWM << PMC_PCR_PID_Pos);
    PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);

    // enable PWM channels
    PWM->PWM_ENA = PWM_ENA_CHID5 | PWM_ENA_CHID6;

    // set PWM clock A to 1 MHz and clock B to 84 MHz
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(DIVA);
    PWM->PWM_CLK |= PWM_CLK_PREB(0) | PWM_CLK_DIVB(DIVB);

    // assign clocks to respective channels: clock A for channel 5 (PIN45), clock B for channel 6 (PIN44)
    // waveform set to be left-aligned
    PWM->PWM_CH_NUM[CHANNEL_PIN45].PWM_CMR = PWM_CMR_CPRE_CLKA;
    PWM->PWM_CH_NUM[CHANNEL_PIN44].PWM_CMR = PWM_CMR_CPRE_CLKB;
    
    // set PIN45 to a period to 20 ms, giving a frequency of 50 Hz
    PWM->PWM_CH_NUM[CHANNEL_PIN45].PWM_CPRD = PWM_CPRD_CPRD(CPRDA);

    // set PIN44 to not initially have a pulse
    PWM->PWM_CH_NUM[CHANNEL_PIN44].PWM_CPRD = PWM_CPRD_CPRD(0);
}


void pwm_set_duty_cycle(float duty_cycle, unsigned int channel) {
    if ((duty_cycle < 0) || (duty_cycle > 1)) {
        return;
    }

    if (channel == CHANNEL_PIN44 || channel == CHANNEL_PIN45) {
        int CDTY = 0;
        CDTY = (int) (PWM->PWM_CH_NUM[channel].PWM_CPRD*(1 - duty_cycle));
        PWM->PWM_CH_NUM[channel].PWM_CDTY = PWM_CDTY_CDTY(CDTY);
    }
}


void pwm_set_frequency(int freq, unsigned int channel) {
    if (channel == CHANNEL_PIN44) {
        if (freq == 0) {
            PWM->PWM_CH_NUM[channel].PWM_CPRD = PWM_CPRD_CPRD(0);
        }
        else {
            int CPRD = (int) (MCK / (freq * DIVB));
            PWM->PWM_CH_NUM[channel].PWM_CPRD = PWM_CPRD_CPRD(CPRD);
        }
    }
}