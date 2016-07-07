#ifndef __INCLUDES_H__
#define __INCLUDES_H__

// 设备相关
#include <p33FJ128MC710.h>
// 类型定义
#include "types.h"


// 驱动.
#include "bsp_clk.h"
#include "bsp_gpio.h"
#include "bsp_pwm.h"
#include "bsp_tmr.h"
#include "bsp_qei.h" 
#include "bsp_can.h"
#include "bsp_485.h"
#include "bsp_i2c.h"
#include "bsp_eeprom.h"
#include "bsp_adc.h"
#include "bsp_pwm.h"
#include "bsp_wdt.h"
#include "bsp_uart.h"
#include "bsp_adc.h"


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
