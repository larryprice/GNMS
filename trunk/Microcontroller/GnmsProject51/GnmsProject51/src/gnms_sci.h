/*
 * gnms_sci.h
 *
 * Created: 3/8/2012 12:10:29 PM
 *  Author: team6
 */ 


#ifndef GNMS_SCI_H_
#define GNMS_SCI_H_

#include <gpio.h>
#include <usart.h>
#include <stdbool.h>
#include "conf_board.h"

void gnms_sci_init();

void gnms_sci_write_line(char* sci_str);

bool gnms_sci_check_for_input();

#endif /* GNMS_SCI_H_ */