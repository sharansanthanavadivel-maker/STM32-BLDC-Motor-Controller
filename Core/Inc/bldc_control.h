#ifndef BLDC_CONTROL_H
#define BLDC_CONTROL_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum { BLDC_DIR_FORWARD = 0, BLDC_DIR_REVERSE = 1 } BLDC_Direction;
typedef enum { BLDC_STOPPED = 0, BLDC_RUNNING, BLDC_HALL_ERROR } BLDC_State;

typedef struct {
    TIM_HandleTypeDef *pwm_timer;
    uint32_t pwm_channel[3];       /* U, V, W PWM channels */
    GPIO_TypeDef *hall_port;
    uint16_t hall_pin[3];          /* H1, H2, H3 */
    GPIO_TypeDef *low_side_port;
    uint16_t low_side_pin[3];      /* U, V, W low-side enables */
    uint32_t pwm_period;
    uint8_t pole_pairs;
    volatile uint32_t last_hall_tick;
    volatile uint32_t last_edge_tick;
    volatile uint32_t edge_period_ms;
    volatile uint32_t rpm;
    volatile uint8_t hall_code;
    volatile BLDC_Direction direction;
    volatile BLDC_State state;
    volatile uint16_t duty_permille; /* 0..1000 */
    volatile uint32_t commutation_count;
} BLDC_Controller;

void BLDC_Init(BLDC_Controller *m);
void BLDC_HallEdgeISR(BLDC_Controller *m);
void BLDC_SetDirection(BLDC_Controller *m, BLDC_Direction direction);
void BLDC_SetDuty(BLDC_Controller *m, uint16_t duty_permille);
void BLDC_Stop(BLDC_Controller *m);
uint32_t BLDC_GetRPM(BLDC_Controller *m);
void BLDC_Commutate(BLDC_Controller *m, uint8_t hall, BLDC_Direction direction);

#endif
