// ����.
// DI
// DO.
// ���Ե�IO���.
// ��������������֧��.

SYS_RUNINFO_ST g_stSysRunInfo;


// ���ֲ���.
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
    
}

// ģ�����.
void Ana_Sample(void)
{
    
}

// ��������.
void Task_Sample(void)
{
    DI_Sample();
    Ana_Sample();
}

