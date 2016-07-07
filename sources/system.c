#include "includes.h"

u8 g_ucSysInitFault = 0; // 系统初始化错误标志.

VER_INFO_ST g_stVerInfo;


// BUILD TIME.

u8 g_ucBoardAddr = 0x01; // TODO: 增加单板地址配置.

/***********************************************************
 * 函数名称: CvtBuildDate
 * 函数功能: 转换预处理宏__DATE__的格式为整数
 * 输入参数: pscDateStr:    日期字符串(__DATE__生成)
 * 输出参数: pstBuildDate:  年/月/日
 * 返 回 值: 无.
 * 备    注: 
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
 * 函数名称: VersionInit
 * 函数功能: 版本信息初始化
 * 输入参数: 无.
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: 
***********************************************************/
void VersionInit(void)
{
    // 软件版本信息
    g_stVerInfo.m_ucSoftVerMajor = SW_VER_MAJOR;
    g_stVerInfo.m_ucSoftVerMinor = SW_VER_MINOR;
    g_stVerInfo.m_usSoftVerBuild = SW_VER_BUILD;
    CvtBuildDate(__DATE__, &g_stVerInfo.m_stBuildDate);

    // 硬件上ID编码从1开始.
    g_stVerInfo.m_scHardVerion = 'A';
    g_stVerInfo.m_scBomVersion = 0; 
}


// 系统硬件初始化.
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

// 数据初始化.
void Data_Init(void)
{
    VersionInit();
}

/***********************************************************
 * 函数名称: System_Init
 * 函数功能: 系统初始化
 * 输入参数: 无.
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: 
***********************************************************/
void System_Init(void)
{
    Hardware_Init();
    Data_Init();
}
