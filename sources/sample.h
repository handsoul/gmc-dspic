#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include "includes.h"

//ϵͳ����״̬
typedef struct tagSysRunInfo
{
    u8  m_aucDI[DI_NUM];
    u8  m_aucAI[4];
    u32 m_ulAbsEncoder; // ����ֵ��������ԭʼֵ.
    s32 m_ulRelEncoder; // ABZ������.

    s32 m_slSpeed;
    s32 m_slAbsPosition;
    s32 m_slRelPosition;
    
}SYS_RUNINFO_ST;


extern void Task_Sample(void);
extern SYS_RUNINFO_ST g_stSysRunInfo;
#endif
// end of file

