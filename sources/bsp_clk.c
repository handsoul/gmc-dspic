#include "includes.h"

// ����ĺ궨�������ڳ���֮��.(��SFR,section��prog��).
// ʹ��Posc��Ϊʱ��Դ.˫������.

_FOSCSEL(FNOSC_FRC);             	// �����ⲿ����
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT ); // ����ʱ������
_FWDT(FWDTEN_OFF);                              // �رտ��Ź�
_FICD(ICS_PGD3);                                // ʹ�õ��Զ˿�3

// ϵͳ����ʱ�ӳ�ʼ��.
void Clk_Init(void)
{
    // ����Fposc = 10M.
    // N1 = 2 =  (PLLPRE +2 )   ,  PLLPRE = 0
    // N2 = 2 =  (PLLPOST+1 )*2 ,  PLLPOST= 0
    // M  = 32=  (PLLDIV + 2)   ,  PLLDIV = 30
    // ��ʱ.Fin = Fposc / N1 = 5M.  (����Χ 0.8M ~   8M)
    //      Vco = Fin * M = 160M.   (����Χ 100M ~ 200M)
    //      Fosc= Fposc * M / (N1 * N2) = 80M. (����Χ <= 80M)
	PLLFBD = 78; // M = 43
	CLKDIVbits.PLLPOST=0; // N2 = 2
	CLKDIVbits.PLLPRE=0; // N1 = 2
    __builtin_write_OSCCONH(0x03);
	__builtin_write_OSCCONL(0x01);

#if 0
    // ˫������ʱ����RCΪԴ����,����Զ��л���ѡ���ʱ��.
    // �ȴ�ϵͳʱ���ȶ�.
    while (_COSC != _NOSC)
	{
		if (++j < 100000)
		{
			break; // ��ʱ�˳�.
 		}
	}
	
    if (j >= 100000)
    {
        g_ucSysInitFault = 1; // ʱ���л�ʧ��.
    }
#endif 
}

// end of file.
