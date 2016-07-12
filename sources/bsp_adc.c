#include "includes.h"

// ADC����.

void ADC_Init(void)
{
    //����ѡ��-12bit.
    
    AD1CON1 = 0x04E0; // ������12λģʽ.�Զ�ADת��ʱ��.�ֶ�����ת������.
    // Tcy = 12.5ns.
    // Min(Tad) = 75ns. (ADCS+1) = 6.
    AD1CON3 = 0x1F06;

    // ͨ������.
    AD1CHS0 = 0x0017;
    
    // ADC������������.
    // AN16 - A_Input0.
    // AN17 - A_Input1.
    // AN18 - A_Input2.
    // AN19 - A_Input3.
    // AN22 - 5V ���.
    // AN23 - 24V���.
	AD1PCFGL = 0xFFFF;
    AD1PCFGH = 0xFF30;

    // ��ֹ�ж�.����ADC.
    _AD1IE = 0;
    AD1CON1bits.ADON = 1;
    
}

// ע: Ϊ��ʹ��12bit��������.����ʹ��ͨ��0.

u16 usTimeoutCnt = 0;


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

    AD1CON1bits.DONE = 0;// ����ת�����λ.
    AD1CON1bits.SAMP = 1;// ����ת��.

    for(i =0,j = 0,ucResult = 0;i < 8;i++)
    {
        while(AD1CON1bits.DONE != 1)
        {
            if (++j > 10000) // ��ֹת����ʱ.
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
