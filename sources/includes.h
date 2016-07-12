#ifndef __INCLUDES_H__
#define __INCLUDES_H__

// �豸���
#include <p33FJ128MC710.h>
// ���Ͷ���
#include "types.h"

// ����.
#include "bsp_clk.h"        // ϵͳʱ������.
#include "bsp_gpio.h"       // GPIO.
#include "bsp_pwm.h"        // PWM����.
#include "bsp_tmr.h"        // ��ʱ��.
#include "bsp_qei.h"        // QEI�ӿ�.
#include "bsp_can.h"        // CAN.
#include "bsp_i2c.h"        // I2C
#include "bsp_eeprom.h"     // EE
#include "bsp_adc.h"        // ADC
#include "bsp_wdt.h"        // WDT.
#include "bsp_uart.h"       // UART.

// ͨ�Ŵ���.
#include "north_can_msg_process.h"

// ����.
#include "sample.h"

// system c library.
//#include "unistd.h"
#include <string.h>
#include <stdlib.h>

// ����.
#include "system.h"
#include "public_functions.h"

#endif 
