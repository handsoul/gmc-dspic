#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include "includes.h"

//ϵͳ����״̬
typedef struct tagSysRunInfo
{
    u8  m_aucDI[DI_NUM];
    u8  m_aucDO[DO_NUM];
    u16 m_ausAI[4];
    u16 m_ausAO[2];
    u32 m_ulAbsEncoder; // ����ֵ��������ԭʼֵ.
    s32 m_ulRelEncoder; // ABZ������.

    s32 m_slSpeed;
    s32 m_slAbsPosition;
    s32 m_slRelPosition;

    u16 m_usSys5vPower; // ϵͳ5V״̬.
    u16 m_usSys24vPower;// ϵͳ24V״̬.
    s16 m_ssBoardTemp;  // ϵͳ�¶�״̬.
    u8  m_ucSysWorkSt;  // ϵͳ����״̬��

    u8  m_ucHeartBeatCnt;
    
}SYS_RUNINFO_ST;


extern void Task_Sample(void);
extern SYS_RUNINFO_ST g_stSysRunInfo;

extern void ADC_Init(void);
extern u16 GetADCResult(u8 ucCh);

#endif
// end of file

