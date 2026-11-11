#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/* NUCLEO-F401RE default pin plan; adjust to match CubeMX. */
#define ESC_GPIO_Port       GPIOA
#define ESC_Pin             GPIO_PIN_8       /* TIM1_CH1 */

#define VBAT_GPIO_Port      GPIOA
#define VBAT_Pin            GPIO_PIN_0       /* ADC1_IN0 */

#define BUZZER_GPIO_Port    GPIOB
#define BUZZER_Pin          GPIO_PIN_0

#define LED_R_GPIO_Port     GPIOB
#define LED_R_Pin           GPIO_PIN_1
#define LED_G_GPIO_Port     GPIOB
#define LED_G_Pin           GPIO_PIN_2
#define LED_B_GPIO_Port     GPIOB
#define LED_B_Pin           GPIO_PIN_10

#define INA219_I2C_ADDR     (0x40U << 1)
#define OLED_I2C_ADDR       (0x3CU << 1)

/* PLACEHOLDER thresholds: calibrate for the actual propulsion system. */
#define VBAT_DIVIDER_RATIO  2.0f
#define VBAT_MIN_V          10.5f
#define CURRENT_MIN_A       0.10f
#define CURRENT_MAX_A       8.00f
#define TEST_DURATION_MS    1500U
#define ESC_MIN_US          1000U
#define ESC_TEST_US         1100U

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void MX_ADC1_Init(void);
void MX_TIM1_Init(void);

typedef enum {
    SYS_IDLE = 0,
    SYS_TESTING,
    SYS_PASS,
    SYS_LOW_BATTERY,
    SYS_LOW_CURRENT,
    SYS_OVERCURRENT,
    SYS_SENSOR_ERROR
} SystemState_t;

#endif
