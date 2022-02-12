#include "main.h"

#define STEPS_PER_REV (4096)
#define OPAMP_GAIN (33.0)
#define SHUNT (0.5) //ohm

stepper_t stepper1, stepper2;

void clock_setup();
void gpios_setup();
void steppers_setup();
void timer_setup();
void current_sense();
void move();
void tim3_handler();
void wait();
void enable_interrupt(IRQn_Type IRQn);
void disable_interrupt(IRQn_Type IRQn);

char c;
int d;
float rot, arm;
float sens1 = 0, sens2 = 0;

int main() {
    clock_setup();
	gpios_setup();
	steppers_setup();
	timer_setup();
    usart_setup();
	adc_setup();

    while (1) {
		gpio_toggle(GPIOC, 13);
        wait();

		scanf(" %c%d", &c, &d);	

		if(c != 'W') d = 100;

		printf("%d\r\n", d);

		switch(d) {
			case 0: //move
				scanf("%f %f", &rot, &arm);
				printf("%f %f\n", rot, arm);
				move();
				break;
			
			case 1: //current sense
				printf("%.3f %.3f\r\n", sens1, sens2);
				break;

			default:
				printf("Error.\r\n");
		}
    }

	return 0;
}

void clock_setup(void)
{
	// Necessary wait states for Flash for high speeds
	FLASH->ACR = 0x12;
	// Enable HSE
	RCC->CR |= (1 << 16);
	// Wait untill HSE settles down
	while (!(RCC->CR & (1 << 17)));
	// Select Prediv1 as PLL source
	RCC->CFGR |= (1 << 16);
	// Set PLL1 multiplication factor to 9
	RCC->CFGR |= (0b0111 << 18);
	// Set APB1 to 36MHz
	RCC->CFGR |= 1 << 10;
	// Enable PLL
	RCC->CR |= (1 << 24);
	// Wait untill PLL settles down
	while (!(RCC->CR & (1 << 25)));
	// Finally, choose PLL as the system clock
	RCC->CFGR |= (0b10 << 0);
}

void gpios_setup()
{
	// built-in led
	gpio_setup(GPIOC, GPIO_OUT, 13);

	// stepper drivers
	for(uint8_t i = 0; i <= 7; i++) {
		gpio_setup(GPIOA, GPIO_OUT, i);	
	}
}

void steppers_setup()
{
	GPIO_TypeDef *gpio_ports[4];
    uint32_t gpio_pins[4];

	for(int i = 0; i < 4; i++) {
		gpio_ports[i] = GPIOA;
		gpio_pins[i] = i;
	}

	stepper_setup(&stepper1, gpio_ports, gpio_pins);

	for(int i = 0; i < 4; i++) {
		gpio_ports[i] = GPIOA;
		gpio_pins[i] = i + 4;
	}

	stepper_setup(&stepper2, gpio_ports, gpio_pins);
}

void timer_setup()
{
	// Enable clock for that module for TIM3. Bit1 in RCC APB1ENR register
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	// Reset CR1 just in case
	TIM3->CR1 = 0x0000;
	// fCK_PSC / (PSC[15:0] + 1)
	// 72 Mhz / (719 + 1) = 100 khz timer clock speed
	TIM3->PSC = 719;
	// Should generate 2 ms interrupts
	TIM3->ARR = 200;
	// Update Interrupt Enable
	TIM3->DIER |= TIM_DIER_UIE;
	// Priority level 1
	NVIC->IPR[TIM3_IRQn] = 0x10;
	// Enable TIM3 from NVIC register
	enable_interrupt(TIM3_IRQn);
	// Finally enable TIM3 module
	TIM3->CR1 |= TIM_CR1_CEN;
}

void current_sense() 
{
	uint16_t raw1, raw2;
	float tmp1, tmp2;

	// Get raw ADC reading
	raw1 = adc_read(ADC1);
	raw2 = adc_read(ADC2);

	//Conversion to Amperes
	tmp1 = ((float) raw1)*(3.3 / (4095.0 * OPAMP_GAIN * SHUNT)); 
	tmp2 = ((float) raw2)*(3.3 / (4095.0 * OPAMP_GAIN * SHUNT)); 

	//Low pass filter
	sens1 = (tmp1 + 4*sens1)/5;
	sens2 = (tmp2 + 4*sens2)/5;
}

void move()
{
	int steps1, steps2;
	int rot_steps, arm_steps;
	
	rot_steps = rot * STEPS_PER_REV / 360.0;
	arm_steps = arm * STEPS_PER_REV / 360.0;

	steps1 = arm_steps + rot_steps;
	steps2 = arm_steps - rot_steps;

	printf("%d %d\n", steps1, steps2);

	stepper_step(&stepper1, steps1);
	stepper_step(&stepper2, -steps2);
}

void tim3_handler()
{
	current_sense();
	stepper_update(&stepper1);
	stepper_update(&stepper2);
	TIM3->SR &= ~TIM_SR_UIF;
}

void wait() 
{
    // Do some NOPs for a while to pass some time.
    for (unsigned int i = 0; i < 200000; ++i) __asm__ volatile ("nop");
}

void enable_interrupt(IRQn_Type IRQn)
{
	NVIC->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

void disable_interrupt(IRQn_Type IRQn)
{
	NVIC->ICER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}
