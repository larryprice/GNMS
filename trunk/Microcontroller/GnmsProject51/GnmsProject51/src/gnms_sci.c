/*
 * gnms_sci.c
 *
 * Created: 3/8/2012 12:13:21 PM
 *  Author: team6
 */ 

#include "gnms_sci.h"

void gnms_sci_init() {
	static const gpio_map_t USART_GPIO_MAP = {
		{
			GNMS_USART_RX_PIN, GNMS_USART_RX_FUNCTION
		},
		{
			GNMS_USART_TX_PIN, GNMS_USART_TX_FUNCTION
		}
	};
	
	static const usart_options_t usart_opts = { 
		.baudrate = 9600,
		.charlength = 8,
		.paritytype = USART_NO_PARITY,
		.stopbits = USART_1_STOPBIT,
		.channelmode = USART_NORMAL_CHMODE
	};
	
	gpio_enable_module(USART_GPIO_MAP, sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));
	usart_init_rs232(GNMS_USART, &usart_opts, CLOCK_SPEED);
}

void gnms_sci_write_line(char* sci_str) {
	usart_write_line(GNMS_USART, sci_str);
}

bool gnms_sci_check_for_input() {
	int* input_char;
	int usart_result = usart_read_char(GNMS_USART, input_char);
	
	bool result = false;
	
	if (usart_result == USART_SUCCESS) {
		
		if (*input_char == GNMS_POWER_DN_SIGNAL) {
			// tell the main function to power down
			result = true;
		}
	} else if (usart_result == USART_RX_ERROR) {
		// Recover from the error
		gnms_sci_init();
	} 
	
	return result;
}