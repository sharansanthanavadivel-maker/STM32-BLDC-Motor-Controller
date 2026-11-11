# STM32 BLDC Motor Failure Detection for Drone Safety

**Target:** STM32 Nucleo-F401RE (STM32F401RET6), STM32CubeIDE, STM32 HAL, C  
**Status:** Firmware starter implementation — not yet compiled or validated on hardware.

## Features
- INA219 current measurement over I2C
- Battery voltage measurement through a suitably rated voltage divider/module and ADC
- SSD1306 128x64 OLED status display over I2C
- RGB LED and buzzer fault indication
- ESC signal generation using a timer PWM output
- Configurable pre-flight test window and fault thresholds

## Important safety limitations
This is a development scaffold, not a flight-certified safety system. Remove the propeller before any bench test. Use a current sensor, wiring, connectors, voltage divider and power supply rated for the actual system. Many INA219 breakout boards are not suitable for high-current drone propulsion circuits. Do not route motor current through an underrated breakout.
The thresholds in `main.c` are placeholders and must be calibrated for the exact motor, ESC, battery and test setup. Current alone cannot reliably distinguish every motor, ESC, wiring or propeller fault. The ESC output is disabled on detected faults, but this must be independently verified on the actual ESC.
The code expects STM32CubeIDE-generated HAL startup/system files and a configured project for the Nucleo-F401RE. This repository supplies application source and driver files, not ST's HAL library.

## Pin plan (default; verify against your board and CubeMX)
| Function | STM32 pin | Peripheral |
|---|---|---|
| I2C SCL/SDA | PB8 / PB9 | I2C1 AF4 |
| ESC signal | PA8 | TIM1_CH1 |
| Battery voltage sense | PA0 | ADC1_IN0 |
| Buzzer | PB0 | GPIO output |
| RGB LED R/G/B | PB1 / PB2 / PB10 | GPIO outputs |

Configure I2C1 at 100 kHz, ADC1 channel 0, TIM1 channel 1 for a 50 Hz servo-style ESC signal, and the GPIO pins as listed. Set the project clock using the Nucleo-F401RE board configuration. Confirm alternate-function mapping in CubeMX before building.

## Files
- `Core/Src/main.c` — application logic and STM32 HAL entry point
- `Core/Inc/main.h` — pin and threshold configuration
- `Drivers/INA219` — INA219 I2C driver
- `Drivers/SSD1306` — minimal SSD1306 text/status driver interface and implementation

## Build
1. Create an STM32CubeIDE project for **NUCLEO-F401RE**.
2. Configure the peripherals and pins from the table above.
3. Copy `Core/Inc`, `Core/Src`, and `Drivers` into the project, preserving folders.
4. Add driver `.c` files to the build and include their directories.
5. Ensure the generated `main.c` uses the supplied application entry point (merge `SystemClock_Config`, peripheral init functions and MSP code as appropriate).
6. Build, then validate with propeller removed and a current-limited supply.

The HAL handles, peripheral initialization functions and clock setup are deliberately expected from CubeMX-generated project code. This keeps the application portable across CubeMX clock configurations.

## Attribution
Inspired by the project concept “BLDC Motor Failure Detection System for Drone Safety,” *Electronics For You*, April 2026, pp. 104–107. This is an independent STM32 implementation; no magazine source code or figures are reproduced.
