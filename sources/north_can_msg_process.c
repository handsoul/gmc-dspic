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



void NorthRxMsgHandler(CAN_MSG_ST * pstCanRxMsg)
{

    CAN_MSG_ID_UN unCanMsgID;
    u16 eSignalID;
    
    CMD_ID_E  eCmdIDType;
    
    unCanMsgID.m_ulMsgID = pstCanRxMsg->m_ulFrameID;
    eSignalID= (((u16)0x0F&pstCanRxMsg->m_aucData[0]) << 8)|pstCanRxMsg->m_aucData[1]; 
    
    SwapBigLittleEndian(&unCanMsgID, 4);

    // ����ͨ��Э��Ž��д��� (���濼�Ƿŵ�CAN�˲���)
    if (unCanMsgID.m_btMsgID.m_btProtoNO != PROTO_NO_UPPER_TO_GMC)
    {
        return;
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
