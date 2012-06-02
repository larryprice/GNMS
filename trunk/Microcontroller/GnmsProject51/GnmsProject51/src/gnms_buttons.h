/*
 * gnms_buttons.h
 *
 * Created: 3/8/2012 11:59:32 AM
 *  Author: team6
 */ 


#ifndef GNMS_BUTTONS_H_
#define GNMS_BUTTONS_H_

#include "conf_board.h"
#include <gpio.h>

static int prev_proj_left;
static int prev_proj_right;
static int prev_proj_up;
static int prev_proj_down;
static int prev_proj_menu;
static int prev_proj_enter;
static int prev_proj_exit;
static int prev_vol_up;
static int prev_vol_dn;

static int cur_proj_left;
static int cur_proj_right;
static int cur_proj_up;
static int cur_proj_down;
static int cur_proj_menu;
static int cur_proj_enter;
static int cur_proj_exit;
static int cur_vol_up;
static int cur_vol_dn;

void gnms_button_init();


#endif /* GNMS_BUTTONS_H_ */