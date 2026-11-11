#ifndef PID_SPEED_H
#define PID_SPEED_H
#include <stdint.h>
typedef struct { float kp,ki,kd; float integral,previous_error; float out_min,out_max; } PID_t;
void PID_Init(PID_t *p,float kp,float ki,float kd,float min,float max);
uint16_t PID_Update(PID_t *p,float setpoint,float measured,float dt_seconds);
#endif
