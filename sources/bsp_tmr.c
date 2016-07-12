#include "includes.h"


void Tmr_Init(void)
{
    // ��ʱ��1�����������. ��������Ϊ5ms.
    T1CONbits.TON   = 0;    // ��ֹ��ʱ��.
    T1CONbits.TGATE = 0;    // ��ֹ�ſ�ʱ��ģʽ
    T1CONbits.TCKPS = 2;    // Ԥ��Ƶ��.(64��Ƶ. Fpre = 40M / 64 = 625000.
    TMR1            = 0;    // ���㶨ʱ��.
    PR1             = 6249; // Ftmr1 = 100Hz.

    IPC0bits.T1IP   = 0x01; // �ж����ȼ� =1.
    IFS0bits.T1IF   = 0x00; // �����жϱ�־.
    IEC0bits.T1IE   = 0x01; // �ж�����.
    T1CONbits.TON   = 1;    // ������ʱ��.
}


u8 g_ucTickCount = 0;

u8 g_uc10msCnt = 0;

void FAST_ISR _T1Interrupt( void )
{   
    static u16 s_ucMsgPeriod = 0;
    
    IFS0bits.T1IF = 0;
    ++g_ucTickCount;
	

    // ��10ms�����ϱ��������͵ı���.
    if (++s_ucMsgPeriod >= TIME_TO_TICKS(AUTO_SND_INTERVAL_MS))
    {
        s_ucMsgPeriod = 0;
        North_Auto_Send();
    }
    else
    {
        NorthTxMsgProcess();
    }
	
}

// end of file

