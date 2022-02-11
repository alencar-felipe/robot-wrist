#include <stdio.h>

#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "adc.h"

int main() {
    copy_data();
    clock_setup();
	main_gpio_setup();
    usart_setup();
	adc_setup();

    while (1) {
        // Set the output bit.
        GPIOC->ODR = (1 << 13);
        wait();
        // Reset it again.
        GPIOC->ODR = (0 << 13);
        wait();

		int a = (int) adc_read(ADC1);
		int b = (int) adc_read(ADC2);
		printf("%d %d\r\n", a, b);
        //usart_write("robot-wrist\r\n", 13);
    }

	return 0;
}

void clock_setup(void)
{
	// Necessary wait states for Flash for high speeds
	FLASH->ACR = 0x12;
	// Enable HSE
	RCC->CR |= (1 << 16);
	// Wait untill HSE settles down
	while (!(RCC->CR & (1 << 17)));
	// Select Prediv1 as PLL source
	RCC->CFGR |= (1 << 16);
	// Set PLL1 multiplication factor to 9
	RCC->CFGR |= (0b0111 << 18);
	// Set APB1 to 36MHz
	RCC->CFGR |= 1 << 10;
	// Enable PLL
	RCC->CR |= (1 << 24);
	// Wait untill PLL settles down
	while (!(RCC->CR & (1 << 25)));
	// Finally, choose PLL as the system clock
	RCC->CFGR |= (0b10 << 0);
}

void main_gpio_setup()
{
	// built-in led
	gpio_setup(GPIOC, GPIO_OUT, 13);

	// stepper drivers
	for(uint8_t i = 0; i <= 7; i++) {
		gpio_setup(GPIOA, GPIO_OUT, i);	
	}
}

void wait() 
{
    // Do some NOPs for a while to pass some time.
    for (unsigned int i = 0; i < 200000; ++i) __asm__ volatile ("nop");
}