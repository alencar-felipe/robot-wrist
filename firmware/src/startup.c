#include <startup.h>

// Symbols defined in the linker script
extern unsigned long _eram;
extern char _etext, _sdata, _edata, _sbss, _ebss;

// Interrupt vectors
uint32_t (* const vector_table[])
__attribute__ ((section(".interrupt_vectors"))) = {
	(uint32_t *) &_eram,             /* 0x000 Stack Pointer                   */
	(uint32_t *) reset_handler,      /* 0x004 Reset                           */
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

// HEAP
static char base_heap[MAX_HEAP_SIZE];
static char *heap = base_heap;

void copy_data();

uint32_t reset_handler() {
    copy_data();
	__libc_init_array();
    main();
    while(1);
}

__WEAK int main() {
    //EMPTY
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

caddr_t _sbrk(int incr) 
{
	char *prev_heap;
  
  	prev_heap = heap;

	if ((heap + incr) > base_heap + MAX_HEAP_SIZE) 
	{
		errno = ENOMEM;
		return (caddr_t) -1;
	}
  
  heap += incr;

  return (caddr_t) prev_heap;
}

int _close(int file) { return -1; }
 
int _fstat(int file, struct stat *st) {
 st->st_mode = S_IFCHR;
 return 0;
}
 
int _isatty(int file) { return 1; }
 
int _lseek(int file, int ptr, int dir) { return 0; }
 
int _open(const char *name, int flags, int mode) { return -1; }