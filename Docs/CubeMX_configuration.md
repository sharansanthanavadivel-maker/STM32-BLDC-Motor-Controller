# STM32CubeMX configuration

Target: NUCLEO-F401RE / STM32F401RET6.

Configure:
- I2C1: PB8=SCL, PB9=SDA, 100 kHz, 7-bit addressing.
- ADC1: IN0 / PA0, 12-bit, single conversion.
- TIM1 CH1: PA8, PWM mode 1, 1 MHz timer counter, ARR=19999 for 50 Hz.
- GPIO outputs: PB0 buzzer, PB1 red, PB2 green, PB10 blue.
- Ensure all grounds are common. Use a proper voltage divider and ensure PA0 never exceeds 3.3 V.

The timer clock prescaler depends on the actual APB2 timer clock. Set PSC so the timer counter ticks at 1 MHz; set ARR=19999. Do not assume a prescaler value without checking the CubeMX clock tree.

The supplied `main.c` assumes generated handles and functions named `hi2c1`, `hadc1`, `htim1`, `SystemClock_Config`, `MX_GPIO_Init`, `MX_I2C1_Init`, `MX_ADC1_Init`, and `MX_TIM1_Init`. Merge with CubeMX-generated code as appropriate.
