#include "includes.h"

u8 g_ucSysInitFault = 0; // ϵͳ��ʼ�������־.

VER_INFO_ST g_stVerInfo;


// BUILD TIME.

u8 g_ucBoardAddr = 0x01; // TODO: ���ӵ����ַ����.

/***********************************************************
 * ��������: CvtBuildDate
 * ��������: ת��Ԥ�����__DATE__�ĸ�ʽΪ����
 * �������: pscDateStr:    �����ַ���(__DATE__����)
 * �������: pstBuildDate:  ��/��/��
 * �� �� ֵ: ��.
 * ��    ע: 
***********************************************************/
void CvtBuildDate(const char *pscDateStr, BUILD_DATE_ST *pstBuildDate)
{
    const char *apscMonthMap[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                          "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    u8 i;
    for (i = 0; i < ITEM_OF(apscMonthMap); ++i)
    {
        if (MemoryCompare(pscDateStr, apscMonthMap[i], 3) == 0)
        {
            pstBuildDate->m_ucBuildMonth = i + 1;
            break;
        }
    }             

    pstBuildDate->m_ucBuildDate = (u8)atoi(&pscDateStr[4]);            
    pstBuildDate->m_usBuildYear = (u16)atoi(&pscDateStr[7]);
}

/***********************************************************
 * ��������: VersionInit
 * ��������: �汾��Ϣ��ʼ��
 * �������: ��.
 * �������: ��.
 * �� �� ֵ: ��.
 * ��    ע: 
***********************************************************/
void VersionInit(void)
{
    // ����汾��Ϣ
    g_stVerInfo.m_ucSoftVerMajor = SW_VER_MAJOR;
    g_stVerInfo.m_ucSoftVerMinor = SW_VER_MINOR;
    g_stVerInfo.m_usSoftVerBuild = SW_VER_BUILD;
    CvtBuildDate(__DATE__, &g_stVerInfo.m_stBuildDate);

    // Ӳ����ID�����1��ʼ.
    g_stVerInfo.m_scHardVerion = 'A';
    g_stVerInfo.m_scBomVersion = 0; 
}


// ϵͳӲ����ʼ��.
void Hardware_Init(void)
{
	ADC_Init();
    Clk_Init();
	Tmr_Init();
	GPIO_Init();
	CAN_Init();
    PWM_Init();
    Uart_Init();
    
}

// ���ݳ�ʼ��.
void Data_Init(void)
{
    VersionInit();
}

/***********************************************************
 * ��������: System_Init
 * ��������: ϵͳ��ʼ��
 * �������: ��.
 * �������: ��.
 * �� �� ֵ: ��.
 * ��    ע: 
***********************************************************/
void System_Init(void)
{
    Hardware_Init();
    Data_Init();
}
