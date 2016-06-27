#include "includes.h"

// 下面的宏定义必须放在程序之外.(非SFR,section在prog区).
// 使用Posc作为时钟源.双速启动.

_FOSCSEL(FNOSC_FRC);             	// 配置外部晶振
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT ); // 配置时钟特性
_FWDT(FWDTEN_OFF);                              // 关闭看门狗
_FICD(ICS_PGD3);                                // 使用调试端口3

// 系统工作时钟初始化.
void Clk_Init(void)
{
    // 晶振Fposc = 10M.
    // N1 = 2 =  (PLLPRE +2 )   ,  PLLPRE = 0
    // N2 = 2 =  (PLLPOST+1 )*2 ,  PLLPOST= 0
    // M  = 32=  (PLLDIV + 2)   ,  PLLDIV = 30
    // 此时.Fin = Fposc / N1 = 5M.  (合理范围 0.8M ~   8M)
    //      Vco = Fin * M = 160M.   (合理范围 100M ~ 200M)
    //      Fosc= Fposc * M / (N1 * N2) = 80M. (合理范围 <= 80M)
	PLLFBD = 78; // M = 43
	CLKDIVbits.PLLPOST=0; // N2 = 2
	CLKDIVbits.PLLPRE=0; // N1 = 2
    __builtin_write_OSCCONH(0x03);
	__builtin_write_OSCCONL(0x01);

#if 0
    // 双速启动时先以RC为源启动,后会自动切换到选择的时钟.
    // 等待系统时钟稳定.
    while (_COSC != _NOSC)
	{
		if (++j < 100000)
		{
			break; // 超时退出.
 		}
	}
	
    if (j >= 100000)
    {
        g_ucSysInitFault = 1; // 时钟切换失败.
    }
#endif 
}

// end of file.
