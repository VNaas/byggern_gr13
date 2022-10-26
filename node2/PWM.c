#include "PWM.h"
#include "sam.h"

#define MCK         84E6

#define PERIOD_A    20E-3
#define DIVA        84
// #define CLK_A       1E6
#define CPRDA       (int) (PERIOD_A * MCK / DIVA)
#define DIVB        84
#define PWM_period(ms) ((int)(ms*84E6/(84*1000)))
// #define CLK_B       1E6

void PWM_init()
{

    // /* Select peripheral B on PA8.
    // Writing 1 to this pin enables Peripheral function B
    // (A: 0, B: 1)*/
    // PIOC->PIO_ABSR |= PIO_PC19B_PWMH5;

    // /*Disable the (PIO) of the PWMH5 pin so that the peripheral controller can use them*/
    // PIOA->PIO_PDR = PIO_PC19B_PWMH5;

    // /* Unlock user interface */
    // // PWM->PWM_WPCR = PWM_WPCR_WPRG5;

    // /* Enable Clock for PWM in PMC */
    // PMC->PMC_PCR |= PMC_PCR_EN | (ID_PWM << PMC_PCR_PID_Pos); // PWM MCK is 84MHz
    // PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);
    // PWM->PWM_ENA = PWM_ENA_CHID5; // Enable channel 5

    // // config of clock generator
    // // PWM->PWM_CLK = 1 << PWM_CLK_DIVA_Pos | 1 << PWM_CLK_PREA_Pos; // CLKA = MCK/2
    // PWM->PWM_CLK = 1 << PWM_CLK_PREA(0) | PWM_CLK_DIVA(84); // no pre-scaling, divide by 84 => CLKA = 1MHz

    // // selection of clock for each channel
    // // By default left-aligned
    // PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_CLKA;
    // // REG_PWM_CMR5 = 1 << PWM_CMR_CPRE_MCK_DIV_2; // channel 0 uses MCK/2

    // // config of period for channels
    // // PWM->PWM_CH_NUM[5].PWM_CPRD = 840000; // (MCK * clk_period) / X (X = prescaler value 2)
    // PWM->PWM_CH_NUM[5].PWM_CPRD = 20000; // 1MHz / 20 000 = 50Hz

    // PWM->PWM_CH_NUM[5].PWM_CDTY = 63000; // (MCK * 0.0015) / X (Duty cycle 1.5ms is center position.)



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

    PWM->PWM_ENA = PWM_ENA_CHID5;// | PWM_ENA_CHID6;

    PWM->PWM_IER1 = PWM_IER1_CHID0; // Enable Counter Event interrupts on channel 0
    // set PIN44 to not initially have a pulse
}