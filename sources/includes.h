#ifndef __INCLUDES_H__
#define __INCLUDES_H__

// 设备相关
#include <p33FJ128MC710.h>
// 类型定义
#include "types.h"

// 驱动.
#include "bsp_clk.h"        // 系统时钟配置.
#include "bsp_gpio.h"       // GPIO.
#include "bsp_pwm.h"        // PWM驱动.
#include "bsp_tmr.h"        // 定时器.
#include "bsp_qei.h"        // QEI接口.
#include "bsp_can.h"        // CAN.
#include "bsp_i2c.h"        // I2C
#include "bsp_eeprom.h"     // EE
#include "bsp_adc.h"        // ADC
#include "bsp_wdt.h"        // WDT.
#include "bsp_uart.h"       // UART.

// 通信处理.
#include "north_can_msg_process.h"

// 采样.
#include "sample.h"

// system c library.
//#include "unistd.h"
#include <string.h>
#include <stdlib.h>

// 其他.
#include "system.h"
#include "public_functions.h"

#endif 
