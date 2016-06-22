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

    // 根据通信协议号进行处理 (后面考虑放到CAN滤波中)
    if (unCanMsgID.m_btMsgID.m_btProtoNO != PROTO_NO_UPPER_TO_GMC)
    {
        return;
    }

}


// 上报所有信息.正常情况下是主动上报一部分.
// 查询时,上报全部信息.
void NorthUploadStatus(void)
{
    // 上报位置
    
}

// 设置具体的IO/输出.
void NorthCtrlOutput(void)
{
}

// 设置参数.

void NorthSetParams(void)
{
}

// 查询参数.todo: 引入参数管理.
void NorthUploadParams(void)
{
}

// 
