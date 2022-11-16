#include "ADC.h"
#include "sam.h"
#include "toBinary.h"
#include "can_node_2/can_controller.h"

int first_msg = 1;
static int score = 0;
void ADC_init()
{
    // ENABLE PERIPHERAL CLOCK
    PMC->PMC_PCR |= PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_ADC << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1 << (ID_ADC - 32);


    ADC->ADC_CR = ADC_CR_SWRST;
    ADC->ADC_IDR = ~(0ul);        // Disable all interrupts
    ADC->ADC_IER = ADC_IER_COMPE;
    // ADC->ADC_IER |= ADC_IER_EOC0;
    ADC->ADC_MR = ADC_MR_FREERUN | (ADC_MR_TRACKTIM_Pos); // ADC_clock = MCK/2 = 42 MHz, freerun mode

    ADC->ADC_EMR |= ADC_EMR_CMPMODE_LOW |
                    ADC_EMR_CMPALL      |
                    ADC_EMR_CMPFILTER(0);

    ADC->ADC_CWR = ADC_CWR_LOWTHRES(500); // Hopefully 1V threshold?
    ADC->ADC_CHER = ADC_CHER_CH0;

	NVIC_EnableIRQ(ADC_IRQn);
    ADC->ADC_CR = ADC_CR_START;
}

void ADC_Handler(void)
{
    uint32_t adc_isr = ADC->ADC_ISR;
    // toBinary(adc_isr);

    //printf("ISR: %d \n\r", adc_isr);
    if(adc_isr & ADC_ISR_COMPE){
        //printf("Comparison interrupt: \n\r");
        int val = ADC->ADC_CDR[0];

        printf("ADC val: %d\n\r",val);

        CAN_MESSAGE IR_interrupted;
        IR_interrupted.id = 53;
        IR_interrupted.data_length = 1;
        IR_interrupted.data[0] = 1;

        can_send(&IR_interrupted, 1); //TODO: få hjelp med denne
    }
    else
    {
        // printf("No COMPE: %d\n\r", ADC->ADC_CDR[0]);
    }
	NVIC_ClearPendingIRQ(ID_ADC);
}
/* When a conversion is completed, the resulting 12-bit
digital value is stored in the Channel Data Register
(ADC_CDRx) of the current channel and in the ADC Last
Converted Data Register (ADC_LCDR).  */

/* Reading one of the ADC_CDR registers clears the
corresponding EOC bit. Reading ADC_LCDR clears the DRDY
bit and EOC bit corresponding to the last converted channel.
*/