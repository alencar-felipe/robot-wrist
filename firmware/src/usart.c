#include "usart.h"

void usart_setup()
{
    // Enable alternate function clock
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	// Enable GPIOA clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
    // Enable UART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	// Make GPIOA Pin 9,10 (PA9, PA10) alternate-function output (0b1010)
	GPIOA->CRH &= 0xFFFFF00F;
	GPIOA->CRH |= 0x000008B0;

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
