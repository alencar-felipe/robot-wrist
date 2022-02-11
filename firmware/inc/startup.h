#ifndef __STARTUP_H
#define __STARTUP_H

#include <stdint.h>
#include <sys/stat.h>
#include <errno.h>

#include "stm32f103x6.h"

#define MAX_HEAP_SIZE (0x200)
#define __WEAK __attribute__((weak))

uint32_t reset_handler();
int main(); //weak

void *_sbrk_r(struct _reent *reent, ptrdiff_t diff);

#endif