# MPLAB IDE generated this makefile for use with GNU make.
# Project: LX-100PIN-TMR1.mcp
# Date: Sun Jul 03 11:04:42 2016

AS = pic30-as.exe
CC = pic30-gcc.exe
LD = pic30-ld.exe
AR = pic30-ar.exe
HX = pic30-bin2hex.exe
RM = rm

bin/LX-100PIN-TMR1.hex : bin/LX-100PIN-TMR1.cof
	$(HX) "bin\LX-100PIN-TMR1.cof"

bin/LX-100PIN-TMR1.cof : objects/bsp_485.o objects/bsp_can.o objects/bsp_clk.o objects/bsp_gpio.o objects/bsp_eeprom.o objects/bsp_i2c.o objects/bsp_pwm.o objects/bsp_tmr.o objects/bsp_wdt.o objects/main.o objects/system.o objects/bsp_qei.o objects/north_can_msg_process.o objects/public_functions.o objects/sample.o objects/bsp_uart.o
	$(CC) -mcpu=33FJ128MC710 "objects\bsp_485.o" "objects\bsp_can.o" "objects\bsp_clk.o" "objects\bsp_gpio.o" "objects\bsp_eeprom.o" "objects\bsp_i2c.o" "objects\bsp_pwm.o" "objects\bsp_tmr.o" "objects\bsp_wdt.o" "objects\main.o" "objects\system.o" "objects\bsp_qei.o" "objects\north_can_msg_process.o" "objects\public_functions.o" "objects\sample.o" "objects\bsp_uart.o" -o"bin\LX-100PIN-TMR1.cof" -Wl,-Tp33FJ128MC710.gld,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=__ICD2RAM=1,-Map="bin\LX-100PIN-TMR1.map",--report-mem

objects/bsp_485.o : sources/bsp_485.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_485.c" -o"objects\bsp_485.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/bsp_can.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/bsp_can.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_can.c" -o"objects\bsp_can.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/bsp_clk.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/bsp_clk.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_clk.c" -o"objects\bsp_clk.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/bsp_gpio.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/bsp_gpio.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_gpio.c" -o"objects\bsp_gpio.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/bsp_eeprom.o : sources/bsp_eeprom.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_eeprom.c" -o"objects\bsp_eeprom.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/bsp_i2c.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/bsp_i2c.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_i2c.c" -o"objects\bsp_i2c.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/bsp_pwm.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/bsp_pwm.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_pwm.c" -o"objects\bsp_pwm.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/bsp_tmr.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/bsp_tmr.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_tmr.c" -o"objects\bsp_tmr.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/bsp_wdt.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/bsp_wdt.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_wdt.c" -o"objects\bsp_wdt.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/main.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/main.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\main.c" -o"objects\main.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/system.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/system.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\system.c" -o"objects\system.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/bsp_qei.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/bsp_qei.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_qei.c" -o"objects\bsp_qei.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/north_can_msg_process.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/north_can_msg_process.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\north_can_msg_process.c" -o"objects\north_can_msg_process.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/public_functions.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/public_functions.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\public_functions.c" -o"objects\public_functions.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/sample.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/sample.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\sample.c" -o"objects\sample.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

objects/bsp_uart.o : sources/public_functions.h sources/system.h d:/program\ files/microchip/mplabc30/v3.30/include/stdlib.h d:/program\ files/microchip/mplabc30/v3.30/include/stddef.h d:/program\ files/microchip/mplabc30/v3.30/include/string.h sources/sample.h sources/north_can_msg_process.h sources/bsp_uart.h sources/bsp_wdt.h sources/bsp_adc.h sources/bsp_eeprom.h sources/bsp_i2c.h sources/bsp_485.h sources/bsp_can.h sources/bsp_qei.h sources/bsp_tmr.h sources/bsp_pwm.h sources/includes.h sources/bsp_gpio.h sources/bsp_clk.h sources/types.h d:/program\ files/microchip/mplabc30/v3.30/support/dsPIC33F/h/p33FJ128MC710.h sources/includes.h sources/bsp_uart.c
	$(CC) -mcpu=33FJ128MC710 -x c -c "sources\bsp_uart.c" -o"objects\bsp_uart.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

clean : 
	$(RM) "objects\bsp_485.o" "objects\bsp_can.o" "objects\bsp_clk.o" "objects\bsp_gpio.o" "objects\bsp_eeprom.o" "objects\bsp_i2c.o" "objects\bsp_pwm.o" "objects\bsp_tmr.o" "objects\bsp_wdt.o" "objects\main.o" "objects\system.o" "objects\bsp_qei.o" "objects\north_can_msg_process.o" "objects\public_functions.o" "objects\sample.o" "objects\bsp_uart.o" "bin\LX-100PIN-TMR1.cof" "bin\LX-100PIN-TMR1.hex"

