#include "stepper.h"

const gpio_value_t half_step[8][4] = {
    {LOW, LOW, LOW, HIGH},
    {LOW, LOW, HIGH, HIGH},
    {LOW, LOW, HIGH, LOW},
    {LOW, HIGH, HIGH, LOW},
    {LOW, HIGH, LOW, LOW},
    {HIGH, HIGH, LOW, LOW},
    {HIGH, LOW, LOW, LOW},
    {HIGH, LOW, LOW, HIGH}
};

void stepper_setup(stepper_t *stepper, GPIO_TypeDef *gpio_ports[4],
    uint32_t gpio_pins[4]) 
{
    memcpy(stepper->gpio_ports, gpio_ports, 4*sizeof(GPIO_TypeDef *));
    memcpy(stepper->gpio_pins, gpio_pins, 4*sizeof(uint32_t));

    for(int i = 0; i < 4; i++) {
        gpio_setup(gpio_ports[i], GPIO_OUT, gpio_pins[i]);
        gpio_write(gpio_ports[i], gpio_pins[i], LOW);
    }
}

void stepper_step(stepper_t *stepper, int steps)
{
    stepper->steps += steps;
}

bool stepper_is_idle(stepper_t *stepper)
{
    return (stepper->steps == 0);
}

void stepper_off(stepper_t *stepper)
{
    stepper->steps = 0;

    for(int i = 0; i < 4; i++) {
        gpio_write(stepper->gpio_ports[i], stepper->gpio_pins[i], LOW);
    }
}

void stepper_update(stepper_t *stepper)
{
    int *steps = &stepper->steps;
    int *index = &stepper->index;

    if(*steps > 0) {
        (*steps)--;
        (*index)--;
        if(*index < 0) (*index) = 7; 

    } else if(*steps < 0) {
        (*steps)++;
        (*index)++;
        if(*index > 7) *index = 0;
    } else {
        return;
    }

    for(int j = 0; j < 4; j++) {
        gpio_write(stepper->gpio_ports[j], stepper->gpio_pins[j],
            half_step[*index][j]);
    }
}