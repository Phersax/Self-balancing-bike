#include "pid.h"
#include "math.h"

static float pid_get_current_sampling_time(PID_t *p);

float p_action, i_action, d_action, error_debug;


void pid_init(PID_t *p, float k_p, float k_i, float k_d, float min_out, float max_out){
    p->k_p = k_p;
    p->k_i = k_i;
    p->k_d = k_d;
    p->integral_error = 0;
    p->last_error = 0;
    p->last_updated_ts = 0;
    p->min_output = min_out;
    p->max_output = max_out;
}


void pid_set_setpoint(PID_t *p, float set_point){
    p->set_point = set_point;
}


float pid_compute_control_action(PID_t *p, float est_output, float *est_out_derivative){
    float delta_T;
    float error, integral_error;
    float u;
    static float last_u;
    float derivative;
    float alpha = 0.9;
    float k_p = p->k_p;

    error = pid_get_setpoint(p) - est_output;

    delta_T = pid_get_current_sampling_time(p);

    if (p->k_d != 0.0){
        if (est_out_derivative == NULL){
            derivative = (1 - alpha) * (error - p->last_error)/delta_T + alpha * p->d_term/(p->k_d * k_p);
        } else {
            derivative = (1 - alpha) * (*est_out_derivative) + alpha * p->d_term/(p->k_d * k_p);
        }
    } else {
        derivative = 0;
    }

    if (error > p->pos_deadzone || error < p->neg_deadzone){
        integral_error = p->integral_error + delta_T * error;
    } else {
        integral_error = p->integral_error;
    }

    u = k_p * (error + p->k_i * integral_error + p->k_d * derivative);
    p->p_term = k_p * error;
    p->i_term = k_p * p->k_i * integral_error;
    p->d_term = k_p * p->k_d * derivative;

    p_action = p->p_term;
    i_action = p->i_term;
    d_action = p->d_term;

    // reset wind-up
    if (u < p->min_output) {
	u = p->min_output;
    } else if (u > p->max_output) {
	u = p->max_output;
    } else {
	p->integral_error = integral_error;
    }

    last_u = u;

    p->last_error = error;
    error_debug = error;
    pid_update_ts(p);
    return u;
}


float pid_get_error(PID_t *p){
	return p->last_error;
}


float pid_get_setpoint(PID_t *p){
    return p->set_point;
}


static float pid_get_current_sampling_time(PID_t *p){
	return (float) (HAL_GetTick() - p->last_updated_ts)/1000.0;
}


void pid_update_ts(PID_t *p){
	p->last_updated_ts = HAL_GetTick();
}
