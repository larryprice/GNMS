/**
 * \file
 *
 * \brief User board configuration template
 *
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

// clock speed
#define CLOCK_SPEED	12000000

// PM
#define GNMS_PM  AVR32_PM_ADDRESS

// External oscillator startup time
#define BOARD_XOSC_STARTUP_US  AVR32_PM_OSCCTRL0_STARTUP_2048_RCOSC

// PWM -> Generic
#define GNMS_PWM_MASK	0xA
#define GNMS_PWM_CLK_DIVA 80
#define GNMS_PWM_CLK_PREA 4

// PWM -> Projector defines
#define GNMS_PWM_PROJECTOR_MASK 0x8
#define GNMS_PWM_PROJECTOR_PIN	AVR32_PWM_3_1_PIN
#define GNMS_PWM_PROJECTOR_FUNC AVR32_PWM_3_1_FUNCTION
#define GNMS_PROJ_PRD 316
#define GNMS_PROJ_DTY GNMS_PROJ_PRD/2
#define GNMS_PWM_PROJ_CHAN 3

// PWM -> Fan control defines
#define GNMS_PWM_FAN_PIN  AVR32_PWM_1_0_PIN
#define GNMS_PWM_FAN_FUNC AVR32_PWM_1_0_FUNCTION
#define GNMS_PWM_FAN_MASK 0x2
#define GNMS_FAN_PRD      12000
#define GNMS_FAN_DTY_OFF  1000
#define GNMS_FAN_DTY_LOW  9000
#define GNMS_FAN_DTY_MED  10000
#define GNMS_FAN_DTY_HIGH 11000
#define GNMS_PWM_FAN_CHAN 1

// button defines
#define PROJECTOR_LEFT  AVR32_PIN_PA20
#define PROJECTOR_RIGHT AVR32_PIN_PA11
#define PROJECTOR_UP    AVR32_PIN_PA16
#define PROJECTOR_DOWN  AVR32_PIN_PA15
#define PROJECTOR_MENU  AVR32_PIN_PA12
#define PROJECTOR_ENTER AVR32_PIN_PA14
#define PROJECTOR_EXIT  AVR32_PIN_PA13
#define VOLUME_UP		AVR32_PIN_PA22
#define VOLUME_DN		AVR32_PIN_PA21

// USART defines
#  define GNMS_USART                 (&AVR32_USART1)
#  define GNMS_USART_RX_PIN          AVR32_USART1_RXD_0_0_PIN
#  define GNMS_USART_RX_FUNCTION     AVR32_USART1_RXD_0_0_FUNCTION
#  define GNMS_USART_TX_PIN          AVR32_USART1_TXD_0_0_PIN
#  define GNMS_USART_TX_FUNCTION     AVR32_USART1_TXD_0_0_FUNCTION
#  define GNMS_USART_CLOCK_MASK      AVR32_USART1_CLK_PBA
#  define GNMS_PDCA_CLOCK_HSB        AVR32_PDCA_CLK_HSB
#  define GNMS_PDCA_CLOCK_PB         AVR32_PDCA_CLK_PBA

// ADC defines
#define GNMS_ADC_AREF_0 3300
#define GNMS_ADC_AREF_1 0
#define GNMS_ADC_0_PIN  AVR32_ADC_AD_0_PIN
#define GNMS_ADC_0_FUNC AVR32_ADC_AD_0_FUNCTION
#define GNMS_ADC_0 0
#define GNMS_ADC_1_PIN  AVR32_ADC_AD_1_PIN
#define GNMS_ADC_1_FUNC AVR32_ADC_AD_1_FUNCTION
#define GNMS_ADC_1 1
#define GNMS_ADC        (&AVR32_ADC)

#define GNMS_TEMP_HIGH 140   // 36
#define GNMS_TEMP_MED  120   // 32
#define GNMS_TEMP_LOW  100   // 28

// debugging defines
#define GNMS_DEBUG 1

// strings
#define GNMS_VOLUME_UP_STRING "V++\r\n"					        // output signal to turn up volume
#define GNMS_VOLUME_DN_STRING "V--\r\n"					        // output signal to turn down volume
#define GNMS_POWER_DN_SIGNAL  'P'						        // input signal to power down
#define GNMS_POWER_ON_STRING  "DEBUG: GNMS powering up\r\n"
#define GNMS_POWER_DN_STRING  "DEBUG: GNMS powering down!\r\n"
#define GNMS_DEBUG_STR_PROJ   "DEBUG: Sending projector power signals\r\n"
#define GNMS_DEBUG_STR_LEFT   "DEBUG: Left\r\n"
#define GNMS_DEBUG_STR_RIGHT  "DEBUG: Right\r\n"
#define GNMS_DEBUG_STR_UP     "DEBUG: Up\r\n"
#define GNMS_DEBUG_STR_DOWN   "DEBUG: Down\r\n"
#define GNMS_DEBUG_STR_MENU   "DEBUG: Menu\r\n"
#define GNMS_DEBUG_STR_ENTER  "DEBUG: Enter\r\n"
#define GNMS_DEBUG_STR_EXIT   "DEBUG: Exit\r\n"
#define GNMS_DEBUG_STR_OFF    "DEBUG: No Fans\r\n"
#define GNMS_DEBUG_STR_LOW    "DEBUG: Low Fans\r\n"
#define GNMS_DEBUG_STR_MED    "DEBUG: Med Fans\r\n"
#define GNMS_DEBUG_STR_HIGH    "DEBUG: High fans\r\n"
#define GNMS_DEBUG_HB		  "DEBUG: Tick\r\n"

enum gnmsFanSpeed_e {
	GNMS_FAN_SPEED_NA=4,
	GNMS_FAN_SPEED_HIGH=3,
	GNMS_FAN_SPEED_MED=2,
	GNMS_FAN_SPEED_LOW=1,
	GNMS_FAN_SPEED_OFF=0
};

#endif // CONF_BOARD_H
