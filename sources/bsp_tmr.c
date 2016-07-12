#include "includes.h"


void Tmr_Init(void)
{
    // 定时器1用于任务调度. 任务周期为5ms.
    T1CONbits.TON   = 0;    // 禁止定时器.
    T1CONbits.TGATE = 0;    // 禁止门控时钟模式
    T1CONbits.TCKPS = 2;    // 预分频器.(64分频. Fpre = 40M / 64 = 625000.
    TMR1            = 0;    // 清零定时器.
    PR1             = 6249; // Ftmr1 = 100Hz.

    IPC0bits.T1IP   = 0x01; // 中断优先级 =1.
    IFS0bits.T1IF   = 0x00; // 清零中断标志.
    IEC0bits.T1IE   = 0x01; // 中断允许.
    T1CONbits.TON   = 1;    // 启动定时器.
}


u8 g_ucTickCount = 0;

u8 g_uc10msCnt = 0;

void FAST_ISR _T1Interrupt( void )
{   
    static u16 s_ucMsgPeriod = 0;
    
    IFS0bits.T1IF = 0;
    ++g_ucTickCount;
	

    // 以10ms周期上报主动发送的报文.
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

