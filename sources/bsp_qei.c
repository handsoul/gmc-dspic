#include "includes.h"

s32 g_slQEIIndexCnt = 0;
static bool s_bIndexUpdatedFlag = FALSE;


// QEI的输入.作为输入反馈信号.

/*  滤波器滤波时间参数N的计算方法.
 *  根据电气手册.A/B/Z信号的上升沿和下降沿最小为3 * N * Tcy.
 *  设计目标的最大转速为6000r/min = 100r/s.
 *  设计最大细分数为10000.此时对应 上升沿和下降沿之间至少包含 3 * Tcy.
 *  Pmin = 3 * Tcy + (3 * N * Tcy)*2 = 3 * (1 + 2N ) * Tcy.
 *  Fcy = 40 * 10e6.
 *  保留余量后.令最大细分数为20000. 此时tmin = 40000000 / ( 100 * 20000) = 40M / 2M = 20 = 3 * (1+2N) * Tcy.
 *  此时 N = 2.
 *  设置不同细分数时.为了使用最合理的N值.且保留余量(2倍)的情况下.
 *  3 * (1 + 2N ) * Tcy = Tmin = 1 / (D * 2 * 100)
 *  3 * (1 + 2N ) = Fcy / (D * 2 * 100)
 *  N = (40,000,000 /( D * 1.2 * 100) * 3 - 1) / 2
      = ( 200,000 / 3 * D -1 ) / 2
    D为编码器的细分数.(N = 1,2,4,8)
	D = 16384, N = 1.5 = 2 (最大细分数 = 26666)
    D = 10000, N = 2.8 = 2.
    D = 8192 , N = 3.56 = 4.
    D = 4096 , N = 7.63 = 8.
	D = 1024 , N = 32.05=32.
 */


// QEI模块初始化.(由于该模块是选择性开启.默认不开启-调试模式下默认开启)
void QEI_Init(void)
{
    // 使用Z信号清零PosCnt.
    // 使用索引信号的中断
    // 精度使用4x
    QEI1CONbits.SWPAB   = 0x00; // 
    QEI1CONbits.PCDOUT  = 0x00;
    QEI1CONbits.POSRES  = 0x00; // Z复位.


    // 滤波器配置.
    DFLT1CONbits.CEID   = 0; // 禁止错误中断.
    DFLT1CONbits.QEOUT  = 0; // 禁止QEAx/QEBx输出.
    DFLT1CONbits.QECK   = 32;// 数字滤波器时钟

    // 使能QEI.
    QEI1CONbits.QEIM    = 0x05; // x2, 使用Z复位.

    MAXCNT              = 2 * QEI_MAXCNT - 1;
	
	// IFS3bits.QEIIF     = 1;
	IEC3bits.QEIIE       = 1;
}

s32 ReadQeiPos(void)
{
    s32 slPos = 0;
    do
    {
        s_bIndexUpdatedFlag = FALSE;
        slPos = g_slQEIIndexCnt * QEI_MAXCNT + (POS1CNT>>1);
    }while(s_bIndexUpdatedFlag == TRUE);

    g_stSysRunInfo.m_slRelEncoder=slPos;
	g_stSysRunInfo.m_slRelPosition = POSCNT;

	QEI1CONbits.CNTERR = 0;

	return slPos;
}

void FAST_ISR  _QEIInterrupt(void)
{
	// Reset INT0 interrupt flag 
	IFS3bits.QEIIF = 0;
	QEI1CONbits.CNTERR = 0;

	g_slQEIIndexCnt += ((QEI1CONbits.UPDN > 0) ? 1 : -1);

	s_bIndexUpdatedFlag = TRUE;
}

//end of file

