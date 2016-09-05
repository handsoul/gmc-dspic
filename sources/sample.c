#include "includes.h"
// 采样.
// DI
// DO.
// 调试的IO输出.
// 对输出调试命令的支持.

SYS_RUNINFO_ST g_stSysRunInfo;


// 数字采样.
void DI_Sample(void)
{
    g_stSysRunInfo.m_aucDI[DI_IN0] = DI_INPUT0;
    g_stSysRunInfo.m_aucDI[DI_IN1] = DI_INPUT1;
    g_stSysRunInfo.m_aucDI[DI_IN2] = DI_INPUT2;
    g_stSysRunInfo.m_aucDI[DI_IN3] = DI_INPUT3;
    g_stSysRunInfo.m_aucDI[DI_IN4] = DI_INPUT4;
    g_stSysRunInfo.m_aucDI[DI_IN5] = DI_INPUT5;
    g_stSysRunInfo.m_aucDI[DI_IN6] = DI_INPUT6;
    g_stSysRunInfo.m_aucDI[DI_IN7] = DI_INPUT7;
    
    g_stSysRunInfo.m_ulAbsEncoder = ENCODER_ABS_VALUE;
}


// 模拟采样.
void Ana_Sample(void)
{
    // 采样四个通道.
    g_stSysRunInfo.m_usSys5vPower  = GetADCResult(23);//(u16)((u32)2 * GetADCResult(23) * 3300 / 4096);
    g_stSysRunInfo.m_usSys24vPower = GetADCResult(22);//(u16)((u32)12 * GetADCResult(22) * 3300 / 4096);

	g_stSysRunInfo.m_ausAI[0] = GetADCResult(16);//(u16)((u32)12 * GetADCResult(16) * 3300 / 4096);
	g_stSysRunInfo.m_ausAI[1] = GetADCResult(17);//(u16)((u32)12 * GetADCResult(17) * 3300 / 4096);
	g_stSysRunInfo.m_ausAI[2] = GetADCResult(18);//(u16)((u32)12 * GetADCResult(18) * 3300 / 4096);
	g_stSysRunInfo.m_ausAI[3] = GetADCResult(19);//(u16)((u32)12 * GetADCResult(19) * 3300 / 4096);
}

// 采样任务.
void Task_Sample(void)
{
    DI_Sample();
    Ana_Sample();
}


// 每隔10ms估算速度.

