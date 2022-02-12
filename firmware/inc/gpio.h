#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f103x6.h"

typedef enum
{
    GPIO_ANALOG = 0b0000,           //analog mode
    GPIO_INPUT = 0b0100,            //floating input (reset state) 
    GPIO_INPUT_PULL = 0b1000,       //input pull-up/pull-down
    GPIO_OUT = 0b0011,              //output push-pull
    GPIO_OUT_ODRAIN = 0b0111,       //output open-drain
    GPIO_ALT = 0b1011,              //alternate push-pull
    GPIO_ALT_ODRAIN = 0b1111        //alternate open-drain
} gpio_config_t;

typedef enum
{
    LOW = 0,
    HIGH = 1
} gpio_value_t;

void gpio_setup(GPIO_TypeDef *gpio, gpio_config_t mode, uint32_t pin);
void gpio_write(GPIO_TypeDef *gpio, uint32_t pin, gpio_value_t value);
void gpio_toggle(GPIO_TypeDef *gpio, uint32_t pin);

#endif