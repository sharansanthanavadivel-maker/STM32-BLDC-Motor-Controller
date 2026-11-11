#include "pid_speed.h"
void PID_Init(PID_t *p,float kp,float ki,float kd,float min,float max) {
    p->kp=kp; p->ki=ki; p->kd=kd; p->integral=0; p->previous_error=0;
    p->out_min=min; p->out_max=max;
}
uint16_t PID_Update(PID_t *p,float sp,float pv,float dt) {
    if (dt<=0.0f) return 0;
    float e=sp-pv;
    p->integral += e*dt;
    float derivative=(e-p->previous_error)/dt;
    float out=p->kp*e+p->ki*p->integral+p->kd*derivative;
    p->previous_error=e;
    if(out<p->out_min) out=p->out_min;
    if(out>p->out_max) out=p->out_max;
    return (uint16_t)out;
}
