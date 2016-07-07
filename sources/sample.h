#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include "includes.h"

//系统运行状态
typedef struct tagSysRunInfo
{
    u8  m_aucDI[DI_NUM];
    u8  m_aucDO[DO_NUM];
    u16 m_ausAI[4];
    u16 m_ausAO[2];
    u32 m_ulAbsEncoder; // 绝对值编码器的原始值.
    s32 m_ulRelEncoder; // ABZ编码器.

    s32 m_slSpeed;
    s32 m_slAbsPosition;
    s32 m_slRelPosition;

    u16 m_usSys5vPower; // 系统5V状态.
    u16 m_usSys24vPower;// 系统24V状态.
    s16 m_ssBoardTemp;  // 系统温度状态.
    u8  m_ucSysWorkSt;  // 系统工作状态码

    u8  m_ucHeartBeatCnt;
    
}SYS_RUNINFO_ST;


extern void Task_Sample(void);
extern SYS_RUNINFO_ST g_stSysRunInfo;

extern void ADC_Init(void);
extern u16 GetADCResult(u8 ucCh);

#endif
// end of file

