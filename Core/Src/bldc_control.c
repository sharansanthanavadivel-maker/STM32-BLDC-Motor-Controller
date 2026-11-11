#include "bldc_control.h"

/* Six-step Hall sequence table. Validate phase order and Hall wiring on your motor.
 * Each row selects one high-side PWM phase and one low-side conducting phase.
 * 0xFF means disabled. The third phase is floating.
 */
typedef struct { uint8_t high_pwm; uint8_t low_on; } Step;
static const Step forward_steps[8] = {
    {0xFF,0xFF}, {0,1}, {2,1}, {2,0}, {1,0}, {1,2}, {0,2}, {0xFF,0xFF}
};
/* Reverse sequence is the forward table traversed in reverse; this lookup is
 * a common Hall-code mapping and MUST be verified against actual phase wiring. */
static const Step reverse_steps[8] = {
    {0xFF,0xFF}, {1,0}, {1,2}, {0,2}, {0,1}, {2,1}, {2,0}, {0xFF,0xFF}
};

static void all_off(BLDC_Controller *m) {
    for (unsigned i=0;i<3;i++) {
        __HAL_TIM_SET_COMPARE(m->pwm_timer, m->pwm_channel[i], 0);
        HAL_GPIO_WritePin(m->low_side_port, m->low_side_pin[i], GPIO_PIN_RESET);
    }
}
static uint8_t read_hall(BLDC_Controller *m) {
    uint8_t h=0;
    for (unsigned i=0;i<3;i++)
        if (HAL_GPIO_ReadPin(m->hall_port,m->hall_pin[i]) == GPIO_PIN_SET)
            h |= (uint8_t)(1U<<i);
    return h;
}
void BLDC_Init(BLDC_Controller *m) {
    all_off(m);
    m->state=BLDC_STOPPED; m->rpm=0; m->duty_permille=0;
    m->last_edge_tick=HAL_GetTick(); m->edge_period_ms=0;
}
void BLDC_SetDuty(BLDC_Controller *m, uint16_t duty) {
    if (duty>1000U) duty=1000U;
    m->duty_permille=duty;
    if (m->state==BLDC_RUNNING)
        BLDC_Commutate(m, m->hall_code, m->direction);
}
void BLDC_SetDirection(BLDC_Controller *m, BLDC_Direction d) {
    /* Never reverse while energized. Stop, wait for zero speed, then change. */
    if (m->state != BLDC_STOPPED || m->rpm != 0U) return;
    m->direction=d;
}
void BLDC_Stop(BLDC_Controller *m) {
    all_off(m); m->state=BLDC_STOPPED; m->duty_permille=0;
}
void BLDC_Commutate(BLDC_Controller *m, uint8_t hall, BLDC_Direction d) {
    all_off(m);
    if (hall==0U || hall==7U) { m->state=BLDC_HALL_ERROR; return; }
    Step s = (d==BLDC_DIR_FORWARD) ? forward_steps[hall] : reverse_steps[hall];
    if (s.high_pwm>2U || s.low_on>2U) { m->state=BLDC_HALL_ERROR; return; }
    uint32_t compare = (m->pwm_period * m->duty_permille)/1000U;
    __HAL_TIM_SET_COMPARE(m->pwm_timer,m->pwm_channel[s.high_pwm],compare);
    HAL_GPIO_WritePin(m->low_side_port,m->low_side_pin[s.low_on],GPIO_PIN_SET);
    m->state=BLDC_RUNNING; m->hall_code=hall; m->commutation_count++;
}
void BLDC_HallEdgeISR(BLDC_Controller *m) {
    uint32_t now=HAL_GetTick();
    uint32_t dt=now-m->last_edge_tick;
    if (dt>0U) m->edge_period_ms=dt;
    m->last_edge_tick=now;
    m->hall_code=read_hall(m);
    /* Six Hall transitions per electrical revolution; mechanical RPM uses pole pairs. */
    if (m->edge_period_ms>0U && m->pole_pairs>0U)
        m->rpm=(60000U/(m->edge_period_ms*6U*m->pole_pairs));
    BLDC_Commutate(m,m->hall_code,m->direction);
}
uint32_t BLDC_GetRPM(BLDC_Controller *m) {
    if ((HAL_GetTick()-m->last_edge_tick)>500U) m->rpm=0U;
    return m->rpm;
}
