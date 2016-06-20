#include "includes.h"

int iHandleCnts = 0;

void NorthCanRxMsgProcess(void)
{
    volatile u8 ucRxMsgQueueWrPtr;
    MUTEX_SET_VALUE(ucRxMsgQueueWrPtr, g_stNorthRxMsgQueue.m_ucWrPtr);

    while (g_stNorthRxMsgQueue.m_ucRdPtr != ucRxMsgQueueWrPtr)
    {
        NorthRxMsgHandler(&g_stNorthRxMsgQueue.m_astRxMsg[g_stNorthRxMsgQueue.m_ucRdPtr]);
        g_stNorthRxMsgQueue.m_ucRdPtr = (u8)((g_stNorthRxMsgQueue.m_ucRdPtr + 1) % NORTH_CAN_RX_MSG_MAX_NUM);
        MUTEX_SET_VALUE(ucRxMsgQueueWrPtr, g_stNorthRxMsgQueue.m_ucWrPtr);
    }
}



void NorthRxMsgHandler(CAN_MSG_ST * pstMsg)
{
	
    if (pstMsg->m_ulFrameID == 0x100 && pstMsg->m_eFrameFormat == FRAME_FORMAT_STD)
    {
        // ���õ��ת�ٺͷ���.
        SetMotorPwm(pstMsg->m_aucData[0],pstMsg->m_aucData[1]);
		++iHandleCnts;
    }
}


// �ϱ�������Ϣ.����������������ϱ�һ����.
// ��ѯʱ,�ϱ�ȫ����Ϣ.
void NorthUploadStatus(void)
{
    // �ϱ�λ��
    
}

// ���þ����IO/���.
void NorthCtrlOutput(void)
{
}

// ���ò���.

void NorthSetParams(void)
{
}

// ��ѯ����.todo: �����������.
void NorthUploadParams(void)
{
}

// 
