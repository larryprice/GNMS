/**
 * \file
 *
 * \brief User board definition template
 *
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

/* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#define BOARD_OSC0_HZ             12000000
#define BOARD_OSC0_TYPE           XOSC_TYPE_XTAL
#define BOARD_OSC0_IS_XTAL        true
#define BOARD_OSC0_STARTUP_US     17000

#endif // USER_BOARD_H
