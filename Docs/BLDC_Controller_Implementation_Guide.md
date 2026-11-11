# Resume-aligned BLDC controller integration

This addition covers the resume-listed controller functions as an **application firmware scaffold**:
- Hall sensor edge interrupt callback and six-step commutation table
- Hardware timer PWM duty update
- Forward/reverse direction selection (only while stopped)
- RPM estimate from Hall transition period and motor pole-pair count
- PID speed controller (`pid_speed.c`)
- Existing INA219 and battery-voltage monitoring modules remain in the repository

## Critical architecture distinction
The Hall commutation module is for a **three-phase inverter / BLDC gate-driver stage**. It is not code that directly drives motor phases from STM32 pins. Use a proper gate driver with dead-time, shoot-through protection, current limiting and hardware shutdown. The low-side GPIO pins in this scaffold are logic inputs to a suitable driver only.

An off-the-shelf ESC generally performs its own commutation internally. In ESC mode, the STM32 sends a throttle command (servo-style pulse) and reads Hall/RPM telemetry only if the ESC exposes it. Do not use the six-step phase commutation module and ESC throttle output to control the same motor simultaneously.

## CubeMX configuration required
- Configure three timer PWM channels for the inverter's high-side command inputs. The example assumes `TIM_HandleTypeDef *pwm_timer` and three channel constants are supplied.
- Configure Hall inputs H1/H2/H3 with EXTI on both edges.
- Configure low-side driver logic outputs as GPIO.
- Configure ADC/I2C peripherals as described in `CubeMX_configuration.md`.
- Set `pole_pairs` to the actual motor pole-pair count.
- Set timer period and PWM channel mapping to match the gate-driver interface.

In `HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)`, call `BLDC_HallEdgeISR(&motor)` when any Hall input changes. Keep this ISR short in a production design; preferably capture timestamp and Hall state in the ISR, then perform commutation in a tightly bounded high-priority control task/interrupt after validating timing.

## PID integration
Call `PID_Update(&speed_pid, target_rpm, BLDC_GetRPM(&motor), dt_seconds)` at a fixed control-loop interval, then pass the returned 0..1000 duty to `BLDC_SetDuty`. Tune gains only on a safe, current-limited test bench. Add anti-windup, ramp limits, stall detection and independent overcurrent shutdown before real operation.

## What is not validated
Hall phase sequence tables vary with motor wiring and sensor placement. The table here is a starting example only and must be verified against the motor's Hall truth table and phase order. RPM derived from HAL_GetTick() has millisecond resolution and is unsuitable for high-speed precision control; use a hardware timer input-capture timestamp for production. The supplied code is not compiled or tested on a physical motor. It is not flight-ready.
