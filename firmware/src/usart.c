#include "usart.h"

void usart_setup()
{
	gpio_setup(GPIOA, GPIO_ALT, 9); // PB9 -> TX
	gpio_setup(GPIOA, GPIO_INPUT, 10); // PB10 -> RX

    // Enable alternate function clock
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // Enable UART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	// Enable USART
	USART1->CR1 |= USART_CR1_UE;
	// Word length - leave default (8 data)
	USART1->CR1 |= USART_CR1_M;
	// Number of stop bits - leave default (1 stop)
	USART1->CR2 |= USART_CR2_STOP_1 ;
	// Baud rate
	// BRR should be 468.75 for 9600 baud rate
	// Thus manista is 468 (0x1d4) and fraction is 12 (0xc) (12/16 is .75)
	// Making it 0x1d4c
	USART1->BRR = 0x1d4c;
	// Transmitter enable
	USART1->CR1 |= USART_CR1_TE;
	// Receiver enable
	USART1->CR1 |= USART_CR1_RE;
}

void usart_write(char *data, size_t len)
{
    for(;len>0; len--, data++) {
		USART1->DR = *data;
		while(!(USART1->SR & USART_SR_TXE));
	}
}

void usart_read(char *data, size_t len)
{
    for(;len>0; len--, data++) {
		while(!(USART1->SR & USART_SR_RXNE));
		(*data) = USART1->DR  & (uint8_t) 0x00FF;
	}
}

int _write(int fd, char *ptr, int len)
{
	usart_write(ptr, len);
	return len;
}

int _read (int fd, char *ptr, int len) 
{
  usart_read(ptr, len);
  return len;
}