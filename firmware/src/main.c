#include "stm32f103x6.h"

// This is the symbol defined in the linker script.
extern unsigned long _end_of_ram;

// We'll implement main a bit further down but we need the symbol for
// the initial program counter / start address.
void main();

// This is our interrupt vector table that goes in the dedicated section.
__attribute__ ((section(".interrupt_vectors"), used))
void (* const interrupt_vectors[])(void) = {
    // The first 32bit value is actually the initial stack pointer.
    // We have to cast it to a function pointer since the rest of the array
    // would all be function pointers to interrupt handlers.
    (void (*)(void))((unsigned long)&_end_of_ram),

    // The second 32bit value is the initial program counter aka. the
    // function you want to run first.
    main
};

void wait() {
    // Do some NOPs for a while to pass some time.
    for (unsigned int i = 0; i < 200000; ++i) __asm__ volatile ("nop");
}

void main() {
    systemClockSetup();

    // Enable port A clock gate.
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN_Msk;

    // Enable port B clock gate.
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN_Msk;

    // Enable port C clock gate.
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN_Msk;
    
    // Configure GPIO C pin 13 as output.
    GPIOC->CRH = ((0x44444444 // The reset value
        & ~(0xfU << 20))  // Clear out the bits for pin 13
        |  (0b10 << 20)); // Set both MODE bits, leave CNF at 0

    while (1) {
        // Set the output bit.
        GPIOC->ODR = (1 << 13);
        wait();
        // Reset it again.
        GPIOC->ODR = (0 << 13);
        wait();
    }
}


void systemClockSetup(void)
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
