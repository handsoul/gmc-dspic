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
        // 设置电机转速和方向.
        SetMotorPwm(pstMsg->m_aucData[0],pstMsg->m_aucData[1]);
		++iHandleCnts;
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
