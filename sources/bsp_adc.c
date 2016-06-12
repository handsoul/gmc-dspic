#include "includes.h"


// ADC驱动.

void ADC_Init(void)
{
    //精度选择-12bit.
    
    AD1CON1 = 0x0200; // 工作在12位模式.手动采样.
    // Tcy = 12.5ns.
    // Min(Tad) = 75ns. (ADCS+1) = 6.
    AD1CON2 = 0x0105;
    // 通道配置.
    AD1CHS0 = 0x0000;
    
    // ADC功能引脚配置.
    // AN16 - A_Input0.
    // AN17 - A_Input1.
    // AN18 - A_Input2.
    // AN19 - A_Input3.
    ADC1PCFH = 0x000F;

    // 禁止中断.开启ADC.
    A1CON1Bits.AD1IE = 0;
    A1CON1Bits.ADON = 1;
    
}

// 注: 为了使用12bit采样精度.必须使用通道0.

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

    AD1CON1Bits.DONE = 0;// 清零转换完成位.
    AD1CON1Bits.SAMP = 1;// 启动转换.
    
    for(i =0;i < 8;i++)
    {
        while(AD1CON1Bits.DONE != 1)
        {
            if (++j > 10000) // 防止转换超时.
            {
                break;
            }
        }

        ucResult += ADC1BUF0;
    }

    ucResult << 3; // /=8

    return ucResult;
}
