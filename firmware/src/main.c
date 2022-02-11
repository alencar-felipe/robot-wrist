#include <stdio.h>

#include "main.h"
#include "usart.h"
#include "adc.h"

int main() {
    copy_data();
    clock_setup();
    gpio_setup();
    usart_setup();
	//adc_setup();

	//malloc(10);

    while (1) {
        // Set the output bit.
        GPIOC->ODR = (1 << 13);
        wait();
        // Reset it again.
        GPIOC->ODR = (0 << 13);
        wait();

		//int v = (int) adc_read();
		printf("a");
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

void gpio_setup()
{
    // Enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Enable GPIOB clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // Enable GPIOC clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    
    // Configure GPIO C pin 13 as output.
    GPIOC->CRH = ((0x44444444 // The reset value
        & ~(0xfU << 20))  // Clear out the bits for pin 13
        |  (0b10 << 20)); // Set both MODE bits, leave CNF at 0
}

void wait() 
{
    // Do some NOPs for a while to pass some time.
    for (unsigned int i = 0; i < 200000; ++i) __asm__ volatile ("nop");
}