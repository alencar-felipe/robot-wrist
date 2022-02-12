#ifndef __STEPPER_H
#define __STEPPER_H

#include <stdbool.h>
#include <string.h>

#include "stm32f103x6.h"
#include "gpio.h"

typedef struct
{
    GPIO_TypeDef *gpio_ports[4];
    uint32_t gpio_pins[4];
    int index;
    int steps;
} stepper_t;

void stepper_setup(stepper_t *stepper, GPIO_TypeDef *gpio_ports[4],
    uint32_t gpio_pins[4]);
void stepper_step(stepper_t *stepper, int steps);
bool stepper_is_idle(stepper_t *stepper);
void stepper_off(stepper_t *stepper);

void stepper_update(stepper_t *stepper);

#endif