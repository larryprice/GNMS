/**
 * \file
 *
 * \brief The main loop for the GNMS microcontroller code for the 32UC3B164
 *
 */

/* C includes */
#include <stdbool.h>

/* ASF includes */
#include <board.h>
#include <gpio.h>
#include <usart.h>
#include <cycle_counter.h>

/* GNMS includes */
#include "gnms_pwm.h"
#include "gnms_buttons.h"
#include "gnms_sci.h"
#include "gnms_adc.h"
#include "conf_board.h"

#define LED0_PIN AVR32_PIN_PA03

int main (void)
{
	// PM and PWM stuff
	volatile avr32_pm_t *ourPM;
	
	ourPM = GNMS_PM;
	gnms_pm_init(ourPM);
	gnms_pwm_init();
	gnms_sci_init();
	gnms_button_init();
	gnms_adc_init();
	
	#if 0
	volatile avr32_gpio_port_t* led0_gpio_port = &AVR32_GPIO.port[LED0_PIN/32];
	led0_gpio_port->oderc = 1 << LED0_PIN;
	led0_gpio_port->gpers = 1 << LED0_PIN;
	led0_gpio_port->puerc = 1 << LED0_PIN;
	led0_gpio_port->ovrc = 1 << LED0_PIN;
	led0_gpio_port->oders = 1 << LED0_PIN;
	
	while (true) {
		gpio_toggle_pin(LED0_PIN);
		cpu_delay_ms(500, 12000000);
	}
	#endif
	
	#if GNMS_DEBUG == 1
	gnms_sci_write_line(GNMS_POWER_ON_STRING);
	#endif
	
	cpu_delay_ms(5000, CLOCK_SPEED);
	send_IR_Command(gnms_power_str);
		
	unsigned long curTemp = 0; //gnms_adc_get_temp();
	enum gnmsFanSpeed_e fanSpeed = GNMS_FAN_SPEED_NA;
	unsigned long sciVar = 0;
	unsigned long adcVar = 500000;
	
	while (true) {
		// button press detection
		cur_proj_left  = gpio_get_pin_value(PROJECTOR_LEFT);
		cur_proj_right = gpio_get_pin_value(PROJECTOR_RIGHT);
		cur_proj_up    = gpio_get_pin_value(PROJECTOR_UP);
		cur_proj_down  = gpio_get_pin_value(PROJECTOR_DOWN);
		cur_proj_menu  = gpio_get_pin_value(PROJECTOR_MENU);
		cur_proj_enter = gpio_get_pin_value(PROJECTOR_ENTER);
		cur_proj_exit  = gpio_get_pin_value(PROJECTOR_EXIT);
		cur_vol_up	   = gpio_get_pin_value(VOLUME_UP);
		cur_vol_dn	   = gpio_get_pin_value(VOLUME_DN);
		
		if (cur_proj_left != prev_proj_left) {
			if (cur_proj_left != 0) {
				send_IR_Command(gnms_left_str);
				#if GNMS_DEBUG == 1
					gnms_sci_write_line(GNMS_DEBUG_STR_LEFT);
				#endif
			}				
			prev_proj_left = cur_proj_left;
		}
		if (cur_proj_right != prev_proj_right) {
			if (cur_proj_right != 0) {
				send_IR_Command(gnms_right_str);
				#if GNMS_DEBUG == 1
					gnms_sci_write_line(GNMS_DEBUG_STR_RIGHT);
				#endif
			}				
			prev_proj_right = cur_proj_right;	
		}
		if (cur_proj_up != prev_proj_up) {
			if (cur_proj_up != 0) {
				send_IR_Command(gnms_up_str);
				#if GNMS_DEBUG == 1
					gnms_sci_write_line(GNMS_DEBUG_STR_UP);
				#endif
			}
			prev_proj_up = cur_proj_up;	
		}
		if (cur_proj_down != prev_proj_down) {
			if (cur_proj_down != 0) {
				send_IR_Command(gnms_down_str);
				#if GNMS_DEBUG == 1
					gnms_sci_write_line(GNMS_DEBUG_STR_DOWN);
				#endif
			}
			prev_proj_down = cur_proj_down;
		}
		if (cur_proj_menu != prev_proj_menu) {
			if (cur_proj_menu != 0) {
				send_IR_Command(gnms_menu_str);
				#if GNMS_DEBUG == 1
					gnms_sci_write_line(GNMS_DEBUG_STR_MENU);
				#endif
			}
			prev_proj_menu = cur_proj_menu;
		}
		if (cur_proj_enter != prev_proj_enter) {
			if (cur_proj_enter != 0) {
				send_IR_Command(gnms_enter_str);
				#if GNMS_DEBUG == 1
					gnms_sci_write_line(GNMS_DEBUG_STR_ENTER);
				#endif
			}				
			prev_proj_enter = cur_proj_enter;
		}
		if (cur_proj_exit != prev_proj_exit) {
			if (cur_proj_exit != 0) {
				send_IR_Command(gnms_esc_str);
				#if GNMS_DEBUG == 1
					gnms_sci_write_line(GNMS_DEBUG_STR_EXIT);
				#endif
			}
			prev_proj_exit = cur_proj_exit;
		}
		
		if (cur_vol_up != prev_vol_up) {
			if (cur_vol_up != 0) {
				gnms_sci_write_line(GNMS_VOLUME_UP_STRING);
			}
			prev_vol_up = cur_vol_up;
		}
		
		if (cur_vol_dn != prev_vol_dn) {
			if (cur_vol_dn != 0) {
				gnms_sci_write_line(GNMS_VOLUME_DN_STRING);
			}
			prev_vol_dn = cur_vol_dn;
		}
		
		if (adcVar > 500000) {
			adcVar = 0;
			// temperature check
			curTemp = gnms_adc_get_high();
			if (curTemp >= GNMS_TEMP_HIGH) {
				if (fanSpeed != GNMS_FAN_SPEED_HIGH) {
					//run the fans on high
					gnms_turn_fans_on_high();
					fanSpeed = GNMS_FAN_SPEED_HIGH;
					#if GNMS_DEBUG == 1
						gnms_sci_write_line(GNMS_DEBUG_STR_HIGH);
					#endif
				}
			} else if (curTemp >= GNMS_TEMP_MED) {
				if (fanSpeed != GNMS_FAN_SPEED_MED) {
					// run the fans on med
					gnms_turn_fans_on_med();
					fanSpeed = GNMS_FAN_SPEED_MED;
					#if GNMS_DEBUG == 1
						gnms_sci_write_line(GNMS_DEBUG_STR_MED);
					#endif
				}
			} else if (curTemp >= GNMS_TEMP_LOW) {
				if (fanSpeed != GNMS_FAN_SPEED_LOW) {
					// turn fans low
					gnms_turn_fans_on_low();
					fanSpeed = GNMS_FAN_SPEED_LOW;
					#if GNMS_DEBUG == 1
						gnms_sci_write_line(GNMS_DEBUG_STR_LOW);
					#endif
				}
			} else if (curTemp < GNMS_TEMP_LOW) {
				if (fanSpeed != GNMS_FAN_SPEED_OFF) {
					gnms_turn_fans_off();
					fanSpeed = GNMS_FAN_SPEED_OFF;
					#if GNMS_DEBUG == 1
						gnms_sci_write_line(GNMS_DEBUG_STR_OFF);
					#endif
				}				
			}
			
			
		}
		
		if (sciVar > 10000) {
			sciVar = 0;
			if (gnms_sci_check_for_input() == true) {
				#if GNMS_DEBUG == 1
					gnms_sci_write_line(GNMS_POWER_DN_STRING);
				#endif
				break;
			}
		}
		sciVar++;
		adcVar++;
	}
	
	// power off the projector by sending it the power signal twice
	#if GNMS_DEBUG == 1
		gnms_sci_write_line(GNMS_DEBUG_STR_PROJ);
	#endif
	send_IR_Command(gnms_power_str);
	cpu_delay_ms(300, CLOCK_SPEED);
	send_IR_Command(gnms_power_str);
	
	adc_disable(adc,GNMS_ADC_0);
	adc_disable(adc,GNMS_ADC_1);
	
	while (true) {
		// wait for shut down to complete
	}
}
