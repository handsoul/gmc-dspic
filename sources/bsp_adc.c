#include "includes.h"

// ADC驱动.

void ADC_Init(void)
{
    //精度选择-12bit.
    
    AD1CON1 = 0x04E0; // 工作在12位模式.自动AD转换时间.手动启动转换过程.
    // Tcy = 12.5ns.
    // Min(Tad) = 75ns. (ADCS+1) = 6.
    AD1CON3 = 0x1F06;

    // 通道配置.
    AD1CHS0 = 0x0017;
    
    // ADC功能引脚配置.
    // AN16 - A_Input0.
    // AN17 - A_Input1.
    // AN18 - A_Input2.
    // AN19 - A_Input3.
    // AN22 - 5V 检测.
    // AN23 - 24V检测.
	AD1PCFGL = 0xFFFF;
    AD1PCFGH = 0xFF30;

    // 禁止中断.开启ADC.
    _AD1IE = 0;
    AD1CON1bits.ADON = 1;
    
}

// 注: 为了使用12bit采样精度.必须使用通道0.

u16 usTimeoutCnt = 0;


// 仅使用通道0进行采样.
u16 GetADCResult(u8 ucCh)
{
    u16 i = 0, j = 0;
    u16 ucResult = 0;
    
    if (ucCh >= 32)
    {
        return 0;
    }

    AD1CHS0 = ucCh;

    AD1CON1bits.DONE = 0;// 清零转换完成位.
    AD1CON1bits.SAMP = 1;// 启动转换.

    for(i =0,j = 0,ucResult = 0;i < 8;i++)
    {
        while(AD1CON1bits.DONE != 1)
        {
            if (++j > 10000) // 防止转换超时.
            {	
				++usTimeoutCnt;
                break;
            }
        }

		j = 0;
        ucResult += ADC1BUF0;
    }

    ucResult /= 8;

    return ucResult;
}
