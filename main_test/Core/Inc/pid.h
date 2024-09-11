#ifndef PID_H
#define PID_H_

#include "main.h"

typedef struct {
    uint32_t last_updated_ts;
    float set_point;
    float integral_error;
    float last_error;
    float k_p;
    float k_i;
    float k_d;
    float max_output;
    float min_output;
    float neg_deadzone;
    float pos_deadzone;
    // for debugging:
    float p_term;
    float i_term;
    float d_term;
} PID_t;

void pid_init(PID_t *p, float k_p, float k_i, float k_d, float min_out, float max_out);

float pid_compute_control_action(PID_t *p, float est_output, float *est_out_derivative);
void pid_update_ts(PID_t *p);

/* Useful setter functions */
void pid_set_setpoint(PID_t *p, float set_point);

/* Useful getter functions */
float pid_get_setpoint(PID_t *p);
float pid_get_error(PID_t *p);
static float pid_get_current_sampling_time(PID_t *p);

#endif
