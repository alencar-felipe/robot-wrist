#include "gpio.h"

void gpio_setup(GPIO_TypeDef *gpio, gpio_config_t mode, uint32_t pin)
{
    __IO uint32_t *cr;
    
    if(gpio == GPIOA) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    } else if(gpio == GPIOB) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    } else if(gpio == GPIOC) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    } else if(gpio == GPIOD) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
    } else {
        return;
    }

    if(pin > 7) {
        cr = &gpio->CRH;
        pin -= 8;
    } else {
        cr = &gpio->CRL;
    } 
    
    *(cr) &= ~(0b1111 << 4*pin);
    *(cr) |= (mode << 4*pin);
}

void gpio_write(GPIO_TypeDef *gpio, uint32_t pin, gpio_value_t value) 
{
    if(value != LOW) {
        gpio->BSRR = 1 << pin;
    } else {
        gpio->BSRR = 1 << (pin + 16);
    }
}

void gpio_toggle(GPIO_TypeDef *gpio, uint32_t pin) 
{
    uint32_t odr =  gpio->ODR;
    pin = 1 << pin;

    gpio->BSRR = ((odr & pin) << 16) | (~odr & pin);
}