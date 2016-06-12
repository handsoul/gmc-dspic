#include "includes.h"

u8 g_ucSysInitFault = 0; // 系统初始化错误标志.

void System_Init(void)
{
    Clk_Init();
	Tmr_Init();
	GPIO_Init();	
}

