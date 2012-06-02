/*
 * gnms_adc.h
 *
 * Created: 3/9/2012 1:57:28 PM
 *  Author: team6
 */ 


#ifndef GNMS_ADC_H_
#define GNMS_ADC_H_

#include "conf_board.h"
#include <gpio.h>
#include <adc.h>

static unsigned long gnms_adc0_val = 0;
static unsigned long gnms_adc1_val = 0;

static volatile avr32_adc_t *adc = GNMS_ADC;
static const gpio_map_t ADC_0_MAP = {GNMS_ADC_0_PIN, GNMS_ADC_0_FUNC};
static const gpio_map_t ADC_1_MAP = {GNMS_ADC_1_PIN, GNMS_ADC_1_FUNC};

void gnms_adc_init();
unsigned long gnms_adc_get_high();
unsigned long gnms_adc_get_temp();

#endif /* GNMS_ADC_H_ */