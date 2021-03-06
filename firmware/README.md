# Firmware

## About

This firmware runs on an [STM32F103C6T6](https://www.st.com/en/microcontrollers-microprocessors/stm32f103c6.html).
The main highlight of this code is that it does not use the Hardware 
Abstraction Layer (HAL) provided by the STM32CubeMX software. Therefore, we
configured all the registers manually. (We also made the linker and startup code
from scratch)

The code listens for incoming gcode commands from the USART and, when the robot 
receives a command, it takes the necessary action: move the motors, change the
coordinate origin or measure current.

Peripherals used: ADC1, ADC2, GPIO, TIM3, USART1. 

## Custom Gcode
### W0: Move
Performs a movement.

#### Usage
```
W0 [rotation] [arm]
```

#### Parameters
- [rotation]: rotation angle in degrees
- [arm]: arm angle in degrees

#### Example
```
W0 180.0 0.0
```

### W1: Set position
Set the current position to the values specified. (Does not move the motors)

#### Usage
```
W1 [rotation] [arm]
```
#### Parameters
- [rotation]: rotation angle in degrees
- [arm]: arm angle in degrees

#### Example
```
W1 0.0 0.0
```

### W2: Get electric currents value
Gets currents of both motors in amperes.

#### Usage
```
W2
```

#### Example

```
W2
0.141 0.136
```

## Build

```
$ make clean && make build
```

## Flash

```
$ make flash
```
## Debug

```
$ openocd -f stlink_bluepill.cfg &
$ arm-none-eabi-gdb build/robot-wrist.elf
(gdb) target remote :3333
```

Reset: ```monitor reset halt```

## References

- https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
- https://mensi.ch/blog/articles/bare-metal-c-programming-the-blue-pill-part-1
- https://github.com/fcayci/stm32f1-bare-metal
- https://vivonomicon.com/2020/06/28/bare-metal-stm32-programming-part-10-uart-communication/
- https://stackoverflow.com/questions/22317477/setting-up-heap-in-memory-for-arm-embedded-system
- https://github.com/IDWizard/uln2003