#include "stm32f103x6.h"
#include "main.h"
#include "usart.h"

// Symbols defined in the linker script
extern unsigned long _eram;
extern char _etext, _sdata, _edata, _sbss, _ebss;

// Interrupt vectors
uint32_t (* const vector_table[])
__attribute__ ((section(".interrupt_vectors"))) = {
	(uint32_t *) &_eram,             /* 0x000 Stack Pointer                   */
	(uint32_t *) main,               /* 0x004 Reset                           */
	0,                               /* 0x008 Non maskable interrupt          */
	0,                               /* 0x00C HardFault                       */
	0,                               /* 0x010 Memory Management               */
	0,                               /* 0x014 BusFault                        */
	0,                               /* 0x018 UsageFault                      */
	0,                               /* 0x01C Reserved                        */
	0,                               /* 0x020 Reserved                        */
	0,                               /* 0x024 Reserved                        */
	0,                               /* 0x028 Reserved                        */
	0,                               /* 0x02C System service call             */
	0,                               /* 0x030 Debug Monitor                   */
	0,                               /* 0x034 Reserved                        */
	0,                               /* 0x038 PendSV                          */
	0,                               /* 0x03C System tick timer               */
	0,                               /* 0x040 Window watchdog                 */
	0,                               /* 0x044 PVD through EXTI Line detection */
	0,                               /* 0x048 Tamper                          */
	0,                               /* 0x04C RTC global                      */
	0,                               /* 0x050 FLASH global                    */
	0,                               /* 0x054 RCC global                      */
	0,                               /* 0x058 EXTI Line0                      */
	0,                               /* 0x05C EXTI Line1                      */
	0,                               /* 0x060 EXTI Line2                      */
	0,                               /* 0x064 EXTI Line3                      */
	0,                               /* 0x068 EXTI Line4                      */
	0,                               /* 0x06C DMA1_Ch1                        */
	0,                               /* 0x070 DMA1_Ch2                        */
	0,                               /* 0x074 DMA1_Ch3                        */
	0,                               /* 0x078 DMA1_Ch4                        */
	0,                               /* 0x07C DMA1_Ch5                        */
	0,                               /* 0x080 DMA1_Ch6                        */
	0,                               /* 0x084 DMA1_Ch7                        */
	0,                               /* 0x088 ADC1 and ADC2 global            */
	0,                               /* 0x08C CAN1_TX                         */
	0,                               /* 0x090 CAN1_RX0                        */
	0,                               /* 0x094 CAN1_RX1                        */
	0,                               /* 0x098 CAN1_SCE                        */
	0,                               /* 0x09C EXTI Lines 9:5                  */
	0,                               /* 0x0A0 TIM1 Break                      */
	0,                               /* 0x0A4 TIM1 Update                     */
	0,                               /* 0x0A8 TIM1 Trigger and Communication  */
	0,                               /* 0x0AC TIM1 Capture Compare            */
	0,                               /* 0x0B0 TIM2                            */
	0,                               /* 0x0B4 TIM3                            */
	0,                               /* 0x0B8 TIM4                            */
	0,                               /* 0x0BC I2C1 event                      */
	0,                               /* 0x0C0 I2C1 error                      */
	0,                               /* 0x0C4 I2C2 event                      */
	0,                               /* 0x0C8 I2C2 error                      */
	0,                               /* 0x0CC SPI1                            */
	0,                               /* 0x0D0 SPI2                            */
	0,                               /* 0x0D4 USART1                          */
	0,                               /* 0x0D8 USART2                          */
	0,                               /* 0x0DC USART3                          */
	0,                               /* 0x0E0 EXTI15_10                       */
	0,                               /* 0x0E4 RTCAlarm                        */
	0,                               /* 0x0E8 USBWakeup                       */
	0,                               /* 0x0EC TIM8_BRK                        */
	0,                               /* 0x0F0 TIM8_UP                         */
	0,                               /* 0x0F4 TIM8_TRG_COM                    */
	0,                               /* 0x0F8 TIM8_CC                         */
	0,                               /* 0x0FC ADC3                            */
	0,                               /* 0x100 FSMC                            */
	0,                               /* 0x104 SDIO                            */
	0,                               /* 0x108 TIM5                            */
	0,                               /* 0x10C SPI3                            */
	0,                               /* 0x110 USART4                          */
	0,                               /* 0x114 USART5                          */
	0,                               /* 0x118 TIM6                            */
	0,                               /* 0x11C TIM7                            */
	0,                               /* 0x120 DMA2_Ch1                        */
	0,                               /* 0x124 DMA2_Ch2                        */
	0,                               /* 0x128 DMA2_Ch3                        */
	0,                               /* 0x12C DMA2_Ch4                        */
};

int main() {
    copy_data();
    clock_setup();
    gpio_setup();
    usart_setup();

    while (1) {
        // Set the output bit.
        GPIOC->ODR = (1 << 13);
        wait();
        // Reset it again.
        GPIOC->ODR = (0 << 13);
        wait();

        char c= 'a';
        usart_read(&c, 1);
        usart_write(&c, 1);
    }
}

void copy_data()
{	
	char *src = &_etext;
	char *dst = &_sdata;

	// ROM has data at end of text; copy it
	while (dst < &_edata) *dst++ = *src++;

	// Zero bss
	for (dst = &_sbss; dst< &_ebss; dst++) *dst = 0;
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