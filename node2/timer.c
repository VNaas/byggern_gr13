#include "timer.h"
#include "sam.h"
#include "sam/interrupt.h"
#include "sam/interrupt/interrupt_sam_nvic.h"
#include "LED.h"
// #define F_TC 2.562E6
#define F_MCK 84E6
static int us_ticks = 0;

void timer_init()
{
    PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_TC3 << PMC_PCR_PID_Pos);
    PMC->PMC_PCER0 |= 1 << (ID_TC3);

    /*  Disable TC clock */
    TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;
    /*  Disable interrupts */
    TC1->TC_CHANNEL[0].TC_IDR = 0xFFFFFFFF;
    /*  Clear status register */
    TC1->TC_CHANNEL[0].TC_SR;
    /*  Set mode */
    TC1->TC_CHANNEL[0].TC_CMR |= TC_CMR_WAVE |
                                 TC_CMR_WAVSEL_UP_RC |
                                 TC_CMR_TCCLKS_TIMER_CLOCK1; // 42MHz
    /* Set RC */
    TC1->TC_CHANNEL[0].TC_RC = (int)((F_MCK / 8) / 1E6 +1);
    /* Counter start value */
    TC1->TC_CHANNEL[0].TC_CV = 0;
    /* TC start */
    TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    /* Enable interrupts */
    TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
    NVIC_EnableIRQ(TC3_IRQn);
}

void _delay_us(int us)
{
    us_ticks = us;

    timer_init();
    while (us_ticks)
        ;
}

void _delay_ms(int ms)
{
    _delay_us(1000*ms);
}

void TC3_Handler(void)
{
    us_ticks--;
    if (!us_ticks)
    {
        TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;
        NVIC_DisableIRQ(TC3_IRQn);
        LED_toggleGreen();
    }
    else
        // timerv2_init();
        TC1->TC_CHANNEL[0].TC_SR;
}