#include "includes.h"

u8 g_ucSysInitFault = 0; // ϵͳ��ʼ�������־.

void System_Init(void)
{
    Clk_Init();
	Tmr_Init();
	GPIO_Init();	
}

