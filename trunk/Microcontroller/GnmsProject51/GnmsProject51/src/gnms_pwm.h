/*
 * gnms_projector_control.h
 *
 * Created: 3/6/2012 3:28:57 PM
 *  Author: team6
 */ 


#ifndef GNMS_PROJECTOR_CONTROL_H_
#define GNMS_PROJECTOR_CONTROL_H_

#include <gpio.h>
#include <pm.h>
#include <pwm.h>
#include <cycle_counter.h>
#include <conf_board.h>

#define IR_COMM_LEN 32

static const char *gnms_power_str  = "11000001101010100000100111110110";
static const char *gnms_menu_str   = "11000001101010100101100110100110";
static const char *gnms_esc_str    = "11000001101010100010000111011110";
static const char *gnms_enter_str  = "11000001101010101010000101011110";
static const char *gnms_up_str     = "11000001101010100000110111110010";
static const char *gnms_down_str   = "11000001101010100100110110110010";
static const char *gnms_left_str   = "11000001101010101100110100110010";
static const char *gnms_right_str  = "11000001101010101000110101110010";

void gnms_pm_init(volatile avr32_pm_t *pm);
void gnms_pwm_init();

void send_IR_Command(char *comm);

void gnms_turn_fans_off();
void gnms_turn_fans_on_low();
void gnms_turn_fans_on_med();
void gnms_turn_fans_on_high();


#endif /* GNMS_PROJECTOR_CONTROL_H_ */