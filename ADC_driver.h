#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H
#include <stdlib.h>
#include <inttypes.h>

#define ADC_BASE 0x1400

struct ADC_data
{
    uint8_t ch_0;   // x dir
    uint8_t ch_1;   // y dir
    uint8_t ch_2;   // left slider
    uint8_t ch_3;   // right slider
};

void ADC_init(void);

void ADC_start_conversion(void);

struct ADC_data ADC_get_data(void);

#endif /* ADC_DRIVER_H */