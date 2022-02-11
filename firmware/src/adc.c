#include "adc.h"

void adc_setup()
{
    // ADC clock can be at 14 Mhz max. 
    // Since APB2 is at 72 MHz, divide it by 6 to make the ADC run at 12 Mhz
	
    // Ensure ADCPRE is zero
	RCC-> CFGR &= ~RCC_CFGR_ADCPRE;
	// Set prescaler to 6
	RCC-> CFGR |= RCC_CFGR_ADCPRE_1;

    // Enable alternate function clock. Bit 0 in RCC APB2ENR register
	RCC->APB2ENR |= (1 << 0);
	// Enable GPIOD clock. Bit 5 in RCC APB2ENR register
	RCC->APB2ENR |= (1 << 5);
	// Enable GPIOC clock. Bit 4 in RCC APB2ENR register
	RCC->APB2ENR |= (1 << 4);
	// Enable clock for ADC1 clock. Bit 9 in RCC APB2ENR register
	RCC->APB2ENR |= (1 << 9);

	// Make PB0 and PB1 analog input (0b0000)
	GPIOC->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0 |
                    GPIO_CRL_MODE1 | GPIO_CRL_CNF1);        

	// Enable End of Conversion (EOC) interrupt
	ADC1->CR1 |= (1 << 5);

	// One conversion
	ADC1->SQR1 = 0x00000000;

	// Choose the analog channel to read
	// Since we want channel 8 to be the first
	//   conversion we write 8 to SQ1 bits (3:0)
	//   which is in SQR3 register. For multiple conversions
	//   keep writing the channel numbers to SQx bits.
	ADC1->SQR3 = (8 << 0);

	// Set up software trigger to start conversion
	ADC1->CR2 |= (7 << 17);  // Select SWSTART as trigger
	ADC1->CR2 |= (1 << 20);  // Enable external trigger

	// Enable continuous conversion
	ADC1->CR2 |= (1 << 1);

	//enable_interrupt(ADC1_2_IRQn);

	// Enable A/D conversion
	ADC1->CR2 |= (1 << 0);

	// Calibration reset and start
	// Optional for better accuracy.
	ADC1->CR2 |= (1 << 3);
	while((ADC1->CR2 & (1 << 3)));
	ADC1->CR2 |= (1 << 2);
	while((ADC1->CR2 & (1 << 2)));

	// Start conversion with software trigger
	ADC1->CR2 |= (1<<22);
}

uint16_t adc_read() {
    return ADC1->DR;
}