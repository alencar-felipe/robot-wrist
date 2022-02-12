#ifndef __USART_H
#define __USART_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "stm32f103x6.h"
#include "gpio.h"

void usart_setup();
void usart_write(char *data, size_t len);
void usart_read(char *data, size_t len);

int _write(int fd, char *ptr, int len); 
int _read (int fd, char *ptr, int len);

#endif