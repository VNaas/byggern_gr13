#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#define ADC_BASE 0x1400

struct ADC_data
{
    uint8_t ch_0;
    uint8_t ch_1;
    uint8_t ch_2;
    uint8_t ch_3;
};

void ADC_init(void);

void ADC_start_conversion(void);

struct ADC_data ADC_get_data(void);

#endif /* ADC_DRIVER_H */