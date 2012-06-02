/*
 * gnms_buttons.c
 *
 * Created: 3/8/2012 12:00:16 PM
 *  Author: team6
 */ 

#include "gnms_buttons.h"

void gnms_button_init() {
	gpio_enable_pin_glitch_filter(PROJECTOR_LEFT);
	gpio_enable_pin_glitch_filter(PROJECTOR_RIGHT);
	gpio_enable_pin_glitch_filter(PROJECTOR_UP);
	gpio_enable_pin_glitch_filter(PROJECTOR_DOWN);
	gpio_enable_pin_glitch_filter(PROJECTOR_MENU);
	gpio_enable_pin_glitch_filter(PROJECTOR_EXIT);
	gpio_enable_pin_glitch_filter(PROJECTOR_ENTER);
	gpio_enable_pin_glitch_filter(VOLUME_UP);
	gpio_enable_pin_glitch_filter(VOLUME_DN);
	
	
	prev_proj_left  = gpio_get_pin_value(PROJECTOR_LEFT);
	prev_proj_right = gpio_get_pin_value(PROJECTOR_RIGHT);
	prev_proj_up    = gpio_get_pin_value(PROJECTOR_UP);
	prev_proj_down  = gpio_get_pin_value(PROJECTOR_DOWN);
	prev_proj_menu  = gpio_get_pin_value(PROJECTOR_MENU);
	prev_proj_enter = gpio_get_pin_value(PROJECTOR_ENTER);
	prev_proj_exit  = gpio_get_pin_value(PROJECTOR_EXIT);
	prev_vol_up		= gpio_get_pin_value(VOLUME_UP);
	prev_vol_dn		= gpio_get_pin_value(VOLUME_DN);
	
	cur_proj_left  = prev_proj_left;
	cur_proj_right = prev_proj_right;
	cur_proj_up    = prev_proj_up;
	cur_proj_down  = prev_proj_down;
	cur_proj_menu  = prev_proj_menu;
	cur_proj_enter = prev_proj_enter;
	cur_proj_exit  = prev_proj_exit;
	cur_vol_up	   = prev_vol_up;
	cur_vol_dn	   = prev_vol_dn;
}