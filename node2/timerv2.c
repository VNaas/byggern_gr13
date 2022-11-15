#include "timerv2.h"
#include "sam.h"
#include "sam/interrupt.h"
#include "sam/interrupt/interrupt_sam_nvic.h"
#define F_TC = 2.562E6
static int ms_ticks = 0;
static int initialized = 0;

void timerv2_init()
{
    PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_TC1 << PMC_PCR_PID_Pos);
    PMC->PMC_PCER1 |= 1 << (ID_TC3 - 31);

    TC1->TC_CHANNEL[0].TC_CMR |=    TC_CMR_WAVE | 
                                    TC_CMR_WAVSEL_UP_RC |
                                    TC_CMR_TCCLKS_TIMER_CLOCK3; // 2.652MHz 
    TC1->TC_CHANNEL[0].TC_RC = (int)(F_TC / 1000 -1);
    TC0->TC_CHANNEL[0].TC_CV = 0;
    TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
    TC1->TC_CHANNEL[0].TC_IDR = ~TC_IER_CPCS;
    NVIC_EnableIRQ(TC3_IRQn);

}


void _delay_ms_v2(int ms)
{
    if(!initialized)
    {
        timerv2_init();
        initialized = 1;
    }
    ms_ticks = ms;
    TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN;
    TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG;
    while(ms_ticks);
    TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKDIS;


}

void TC3_Handler(void)
{
    ms_ticks --;
}