#include "timerv2.h"
#include "sam.h"
#include "sam/interrupt.h"
#include "sam/interrupt/interrupt_sam_nvic.h"

static us_ticks = 0;

void timerv2_init()
{
    // // Disable Interrupts on PB25
    // PIOB->PIO_IDR |= PIO_PB25A_RTS0;
    
    // Enable Peripheral B function on PB25
    PIOB->PIO_ABSR |= PIO_PB25B_TIOA0;

    // disable PIO from controlling PB25
    PIOB->PIO_PDR |= PIO_PB25A_RTS0;

    // enable clock for TC0
    PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_TC1 << PMC_PCR_PID_Pos);
    PMC->PMC_PCER1 |= 1 << (ID_TC0 - 31);
}

void timer_start_us()
{

    TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN;
}

void _delay_ms_v2(int ms);

void _delay_us_v2(int us)
{
    printf("Delaying\n\r");
    TC0->TC_CHANNEL[0].TC_CV = 1;
    TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN;
}

void TC0_Handler(void)
{
    printf("Interrupt on TC0_Handler");
}