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
    for (unsigned int i = 0; i < 250000; ++i) __asm__ volatile ("nop");
}

void main() {
    // Enable port C clock gate.
    //*((volatile unsigned int *)0x40021018) |= (1 << 4);
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