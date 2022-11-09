#include "DAC.h"
#include "sam.h"


void DAC_init(){

    //perhaps disable pio and enable peripheral functions??

    //enable clock for dac
    PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_DACC << PMC_PCR_PID_Pos);
    PMC->PMC_PCER1 |= 1 << (ID_ADC - 31);

    //we want freerunning mode: disable external trigger mode
    //select channel 1
    //half-word transer mode
    //disable tag
    DACC-> DACC_MR = DACC_MR_USER_SEL_CHANNEL1 | DACC_MR_TRGEN_DIS | DACC_MR_WORD_HALF | DACC_MR_TAG_DIS | DACC_MR_STARTUP_0;


    //enable DACing on channel 0
    DACC-> DACC_CHER = DACC_CHER_CH1;
    //must diable somewhere too

}
//må kanskje ha DACC_handler
void DAC_write(uint16_t data){
    /*n free running mode, conversion starts as soon as at least one channel is enabled and data is written in the DACC
    Conversion Data Register*/

    DACC -> DACC_CDR = data;

    while(!DACC->DACC_ISR & DACC_ISR_EOC);

}

/*When a conversion is completed, the resulting analog value
is available at the selected DACC channel output and
the EOC bit in the DACC Interrupt Status Register, is set.
Reading the DACC_ISR register clears the EOC bit.*/