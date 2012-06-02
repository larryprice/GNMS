/*
 * gnms_adc.c
 *
 * Created: 3/9/2012 2:02:31 PM
 *  Author: team6
 */ 

#include "gnms_adc.h"

void gnms_adc_init() {
	gpio_enable_module(ADC_0_MAP, sizeof(ADC_0_MAP) / sizeof(ADC_0_MAP[0]));
	gpio_enable_module(ADC_1_MAP, sizeof(ADC_1_MAP) / sizeof(ADC_1_MAP[0]));
	adc_configure(adc);
	adc_enable(adc, GNMS_ADC_0);
	adc_enable(adc, GNMS_ADC_1);
	adc_start(adc);
}

unsigned long gnms_adc_get_high() {
	// if we're not running a conversion, start one up
	if ((adc_check_eoc(adc, GNMS_ADC_0) == true) && (adc_check_eoc(adc, GNMS_ADC_1) == true)) {
		gnms_adc0_val = adc_get_value(adc, GNMS_ADC_0);
		gnms_adc1_val = adc_get_value(adc, GNMS_ADC_1);
		adc_start(adc);
	}

	if (gnms_adc0_val > gnms_adc1_val) {
		return gnms_adc0_val;
	} else {
		return gnms_adc1_val;
	}
}

unsigned long gnms_adc_get_temp() {
	return ((gnms_adc_get_high() * (3300 / 1024)) / 10);
}
