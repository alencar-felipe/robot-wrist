#include "adc.h"

void _adc_setup(ADC_TypeDef *adc, uint8_t channel);

void adc_setup()
{
    gpio_setup(GPIOB, GPIO_ANALOG, 0); //PB0 -> CH_8
    gpio_setup(GPIOB, GPIO_ANALOG, 1); //PB1 -> CH_9

	// Set prescaler to 6 (72 MHz -> 12 MHz)
    RCC-> CFGR &= ~RCC_CFGR_ADCPRE;
	RCC-> CFGR |= RCC_CFGR_ADCPRE_1;
   
	RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN | // Enable alternate function clock.
                     RCC_APB2ENR_IOPAEN | // Enable GPIOA clock.
                     RCC_APB2ENR_ADC1EN | // Enable ADC1 clock.
                     RCC_APB2ENR_ADC2EN); // Enable ADC2 clock.    

	_adc_setup(ADC1, 8); //PB0 -> CH_8
    _adc_setup(ADC2, 9); //PB1 -> CH_9
}

void _adc_setup(ADC_TypeDef *adc, uint8_t channel)
{
    // Enable End of Conversion (EOC) interrupt
	adc->CR1 |= ADC_CR1_EOSIE;

	// Set conversion sequence length (1)
	adc->SQR1 = (0 << ADC_SQR1_L_Pos);

	// Set conversion sequence
	adc->SQR3 = (channel << ADC_SQR3_SQ1_Pos);

	// Set up software trigger to start conversion
	adc->CR2 |= (ADC_CR2_EXTSEL  | // Select SWSTART as trigger
                  ADC_CR2_EXTTRIG | // Enable external trigger
                  ADC_CR2_CONT    | // Enable continuous conversion
                  ADC_CR2_ADON);    // Enable A/D conversion

	// Calibration reset and start
	// Optional for better accuracy.
	adc->CR2 |= ADC_CR2_RSTCAL;
	while((ADC1->CR2 & ADC_CR2_RSTCAL));
	adc->CR2 |= ADC_CR2_CAL;
	while((ADC1->CR2 & ADC_CR2_CAL));
    
	// Start conversion with software trigger
	adc->CR2 |= ADC_CR2_SWSTART;
}

uint16_t adc_read(ADC_TypeDef *adc) {
    return adc->DR;
}