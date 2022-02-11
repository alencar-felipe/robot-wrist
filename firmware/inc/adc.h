#ifndef __ADC_H
#define __ADC_H

#include <stdint.h>

#include "stm32f103x6.h"

void adc_setup();
uint16_t adc_read();

#endif