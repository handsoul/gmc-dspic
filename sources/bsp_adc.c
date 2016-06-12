#include "includes.h"


// ADC����.

void ADC_Init(void)
{
    //����ѡ��-12bit.
    
    AD1CON1 = 0x0200; // ������12λģʽ.�ֶ�����.
    // Tcy = 12.5ns.
    // Min(Tad) = 75ns. (ADCS+1) = 6.
    AD1CON2 = 0x0105;
    // ͨ������.
    AD1CHS0 = 0x0000;
    
    // ADC������������.
    // AN16 - A_Input0.
    // AN17 - A_Input1.
    // AN18 - A_Input2.
    // AN19 - A_Input3.
    ADC1PCFH = 0x000F;

    // ��ֹ�ж�.����ADC.
    A1CON1Bits.AD1IE = 0;
    A1CON1Bits.ADON = 1;
    
}

// ע: Ϊ��ʹ��12bit��������.����ʹ��ͨ��0.

// ��ʹ��ͨ��0���в���.
u16 GetADCResult(u8 ucCh)
{
    u16 i = 0, j = 0;
    u16 ucResult = 0;
    
    if (ucCh >= 32)
    {
        return 0;
    }

    AD1CHS0 = ucCh;

    AD1CON1Bits.DONE = 0;// ����ת�����λ.
    AD1CON1Bits.SAMP = 1;// ����ת��.
    
    for(i =0;i < 8;i++)
    {
        while(AD1CON1Bits.DONE != 1)
        {
            if (++j > 10000) // ��ֹת����ʱ.
            {
                break;
            }
        }

        ucResult += ADC1BUF0;
    }

    ucResult << 3; // /=8

    return ucResult;
}
