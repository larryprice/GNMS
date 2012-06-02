/*
 * gnms_projector_control.c
 *
 * Created: 3/6/2012 3:31:37 PM
 *  Author: team6
 */ 

#include <gnms_pwm.h>

void gnms_pm_init(volatile avr32_pm_t *pm){
	pm_switch_to_osc0(pm, CLOCK_SPEED, BOARD_XOSC_STARTUP_US);
}

void gnms_pwm_init(){
	pwm_opt_t *pwmMod;
	
	avr32_pwm_channel_t fanChan;
	avr32_pwm_channel_t projChan;
	
	projChan.cmr = 0;
	projChan.cdty = GNMS_PROJ_DTY;
	projChan.cprd = GNMS_PROJ_PRD;
	
	fanChan.cmr = 0;
	fanChan.cdty = GNMS_FAN_DTY_OFF;
	fanChan.cprd = GNMS_FAN_PRD;
	fanChan.CMR.cpol = PWM_POLARITY_HIGH;
	
	pwmMod->diva = GNMS_PWM_CLK_DIVA;    //DivA 
	pwmMod->prea = GNMS_PWM_CLK_PREA;    //PreA         Equation -> 12,000,000/4/79 ~ 38,000
	pwmMod->divb = 0;
	pwmMod->preb = 0;  //Turn off b clock since we are not going to need it.
	
	pwm_init(pwmMod);
	pwm_channel_init(GNMS_PWM_PROJ_CHAN, &projChan);
	pwm_channel_init(GNMS_PWM_FAN_CHAN, &fanChan);
	
	gpio_enable_module_pin(GNMS_PWM_PROJECTOR_PIN, GNMS_PWM_PROJECTOR_FUNC); // projector led
	gpio_enable_module_pin(GNMS_PWM_FAN_PIN, GNMS_PWM_FAN_FUNC);             // fan control
	
	pwm_start_channels(GNMS_PWM_MASK);                                       // start both channels
}

void send_IR_Command(char *comm){
	int i,j;
	for(j = 0; j < 2; j++){
		pwm_start_channels((unsigned long)GNMS_PWM_PROJECTOR_MASK);
		cpu_delay_ms(9, CLOCK_SPEED);
		pwm_stop_channels((unsigned long)GNMS_PWM_PROJECTOR_MASK);
		cpu_delay_us(4500, CLOCK_SPEED);
		for(i = 0; i < IR_COMM_LEN; i++){
			pwm_start_channels((unsigned long)GNMS_PWM_PROJECTOR_MASK);
			cpu_delay_us(562, CLOCK_SPEED);
			pwm_stop_channels((unsigned long)GNMS_PWM_PROJECTOR_MASK);
			if(comm[i] == '0'){
				cpu_delay_us(562, CLOCK_SPEED);
			}else{
				cpu_delay_us(1687, CLOCK_SPEED);
			}
			
		}
		
		cpu_delay_ms(40, CLOCK_SPEED);
	}
}


void gnms_turn_fans_off() {
	avr32_pwm_channel_t pwmChan;
	
	pwmChan.cmr = 0;
	pwmChan.cdty = GNMS_FAN_DTY_OFF;
	pwmChan.cprd = GNMS_FAN_PRD;
	
	pwm_stop_channels(GNMS_PWM_FAN_MASK);
	pwm_channel_init(GNMS_PWM_FAN_CHAN, &pwmChan);
	pwm_start_channels(GNMS_PWM_FAN_MASK);
}

void gnms_turn_fans_on_low() {
	avr32_pwm_channel_t pwmChan;
	
	pwmChan.cmr = 0;
	pwmChan.cdty = GNMS_FAN_DTY_LOW;
	pwmChan.cprd = GNMS_FAN_PRD;
	
	pwm_stop_channels(GNMS_PWM_FAN_MASK);
	pwm_channel_init(GNMS_PWM_FAN_CHAN, &pwmChan);
	pwm_start_channels(GNMS_PWM_FAN_MASK);
}

void gnms_turn_fans_on_med() {
	avr32_pwm_channel_t pwmChan;
	
	pwmChan.cmr = 0;
	pwmChan.cdty = GNMS_FAN_DTY_MED;
	pwmChan.cprd = GNMS_FAN_PRD;
	
	pwm_stop_channels(GNMS_PWM_FAN_MASK);
	pwm_channel_init(GNMS_PWM_FAN_CHAN, &pwmChan);
	pwm_start_channels(GNMS_PWM_FAN_MASK);
}

void gnms_turn_fans_on_high() {
	avr32_pwm_channel_t pwmChan;
	
	pwmChan.cmr = 0;
	pwmChan.cdty = GNMS_FAN_DTY_HIGH;
	pwmChan.cprd = GNMS_FAN_PRD;
	
	pwm_stop_channels(GNMS_PWM_FAN_MASK);
	pwm_channel_init(GNMS_PWM_FAN_CHAN, &pwmChan);
	pwm_start_channels(GNMS_PWM_FAN_MASK);
}

