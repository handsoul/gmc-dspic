#include "includes.h"

s32 g_slQEIIndexCnt = 0;
static bool s_bIndexUpdatedFlag = FALSE;


// QEI������.��Ϊ���뷴���ź�.

/*  �˲����˲�ʱ�����N�ļ��㷽��.
 *  ���ݵ����ֲ�.A/B/Z�źŵ������غ��½�����СΪ3 * N * Tcy.
 *  ���Ŀ������ת��Ϊ6000r/min = 100r/s.
 *  ������ϸ����Ϊ10000.��ʱ��Ӧ �����غ��½���֮�����ٰ��� 3 * Tcy.
 *  Pmin = 3 * Tcy + (3 * N * Tcy)*2 = 3 * (1 + 2N ) * Tcy.
 *  Fcy = 40 * 10e6.
 *  ����������.�����ϸ����Ϊ20000. ��ʱtmin = 40000000 / ( 100 * 20000) = 40M / 2M = 20 = 3 * (1+2N) * Tcy.
 *  ��ʱ N = 2.
 *  ���ò�ͬϸ����ʱ.Ϊ��ʹ��������Nֵ.�ұ�������(2��)�������.
 *  3 * (1 + 2N ) * Tcy = Tmin = 1 / (D * 2 * 100)
 *  3 * (1 + 2N ) = Fcy / (D * 2 * 100)
 *  N = (40,000,000 /( D * 1.2 * 100) * 3 - 1) / 2
      = ( 200,000 / 3 * D -1 ) / 2
    DΪ��������ϸ����.(N = 1,2,4,8)
	D = 16384, N = 1.5 = 2 (���ϸ���� = 26666)
    D = 10000, N = 2.8 = 2.
    D = 8192 , N = 3.56 = 4.
    D = 4096 , N = 7.63 = 8.
	D = 1024 , N = 32.05=32.
 */


// QEIģ���ʼ��.(���ڸ�ģ����ѡ���Կ���.Ĭ�ϲ�����-����ģʽ��Ĭ�Ͽ���)
void QEI_Init(void)
{
    // ʹ��Z�ź�����PosCnt.
    // ʹ�������źŵ��ж�
    // ����ʹ��4x
    QEI1CONbits.SWPAB   = 0x00; // 
    QEI1CONbits.PCDOUT  = 0x00;
    QEI1CONbits.POSRES  = 0x00; // Z��λ.


    // �˲�������.
    DFLT1CONbits.CEID   = 0; // ��ֹ�����ж�.
    DFLT1CONbits.QEOUT  = 0; // ��ֹQEAx/QEBx���.
    DFLT1CONbits.QECK   = 32;// �����˲���ʱ��

    // ʹ��QEI.
    QEI1CONbits.QEIM    = 0x05; // x2, ʹ��Z��λ.

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

