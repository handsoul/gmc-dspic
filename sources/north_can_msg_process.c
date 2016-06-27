#include "includes.h"

NORTH_RX_MSG_QUEUE_ST g_stNorthRxMsgQueue;
NORTH_TX_MSG_QUEUE_ST g_stNorthRxMsgQueue;

void NorthCanRxMsgProcess(void)
{
    volatile u8 ucRxMsgQueueWrPtr;
    MUTEX_SET_VALUE(ucRxMsgQueueWrPtr,ucRxMsgQueueWrPtr);

    while (g_stNorthRxMsgQueue.m_ucRdPtr != g_stNorthRxMsgQueue.m_ucWrPtr)
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
    
    
    unCanMsgID.m_ulMsgID = pstCanRxMsg->m_ulFrameID;
    eSignalID= (((u16)0x0F&pstCanRxMsg->m_aucData[0]) << 8)|pstCanRxMsg->m_aucData[1]; 
    
    SwapBigLittleEndian(&unCanMsgID, 4);

    // 根据通信协议号进行处理 (后面考虑放到CAN滤波中)
    if (unCanMsgID.m_btMsgID.m_btProtoNO != PROTO_NO_UPPER_TO_GMC)
    {
        return;
    }

	// 
}


/***********************************************************
 * 函数名称: NorthTxMsgProcess
 * 函数功能: 北向发送消息处理
 * 输入参数: 无.
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: 发送机制在检测到发送队列中有待处理的发送消息后,
 *           取出相关消息发送.
 *           每次只发送1条消息, 避免总线异常时长期占用CPU.
***********************************************************/
void NorthTxMsgProcess(void)
{
    u8 ucTxMsgResult = RTN_ERROR;
    static u8 s_ucTxErrCount = 0;
    
    if (g_stNorthTxMsgQueue.m_ucRdPtr != g_stNorthTxMsgQueue.m_ucWrPtr)
    {            
        // 取出队首消息并发送. 一次只处理一条消息
        ucTxMsgResult = CAN0_SendMsg(&g_stNorthTxMsgQueue.m_astTxMsg[g_stNorthTxMsgQueue.m_ucRdPtr]);
        // 消息发送成功后, 更新读指针
        if (ucTxMsgResult == RTN_OK)
        {
            s_ucTxErrCount = 0;
            g_stNorthTxMsgQueue.m_ucRdPtr = (u8)((g_stNorthTxMsgQueue.m_ucRdPtr + 1) % NORTH_CAN_TX_MSG_MAX_NUM);
        }
    }    
}


/***********************************************************
 * 函数名称: AddNorthTxEvtMsgToQueue
 * 函数功能: 北向CAN发送事件性消息入列
 * 输入参数: pstCanTxMsg:   发送消息指针.
 * 输出参数: 无.
 * 返 回 值: 无.
 * 备    注: 本函数直接添加发送消息到队列中.
***********************************************************/
void NorthSendMsgQueue(CAN_MSG_ST *pstCanTxMsg)
{
    CAN_MSG_ST stCanTxMsg = *pstCanTxMsg;
    
    if (g_stNorthTxMsgQueue.m_ucRdPtr == (g_stNorthTxMsgQueue.m_ucWrPtr + 1))
    {
        return;
    }

    // 复制消息内容到发送队列
    g_stNorthTxMsgQueue.m_astTxMsg[g_stNorthTxMsgQueue.m_ucWrPtr] = stCanTxMsg;
    // 更新发送队列写指针
    g_stNorthTxMsgQueue.m_ucWrPtr = (u8)((g_stNorthTxMsgQueue.m_ucWrPtr + 1) % NORTH_CAN_TX_MSG_MAX_NUM);
}


// 自动上报. 最大8个 //目前的条件下.主动上报报文数目不应超出7条(使用当前的机制最好)
void North_Auto_Send(void)
{
    CAN_MSG_ST stCanTxMsg;    
    CAN_MSG_ID_UN unCanMsgID;
    u16 i;
    u16 usSigID;
    
    unCanMsgID.m_ulMsgID = 0;
    unCanMsgID.m_btMsgID.m_btCntFlag = 1;       // 前N-1帧填充1, 最后1帧填充0.
    unCanMsgID.m_btMsgID.m_btRsv6V1  = (u8)0x3F;    // 保留位填充1
    unCanMsgID.m_btMsgID.m_btMsgDir  = DIR_S2M; // 消息传输方向
    unCanMsgID.m_btMsgID.m_btCmdID   = CMD_ID_AUTO_SEND;  // 命令ID
    unCanMsgID.m_btMsgID.m_btRsv3V0  = 0x0;     // 保留位填充0

    unCanMsgID.m_btMsgID.m_btProtoNO = (u8)PROTO_NO_UPPER_TO_GMC;    // 协议号
    unCanMsgID.m_btMsgID.m_btDstAddr = g_ucBoardAddr   ;        // 目标地址固定为1 

    // 发送帧固定使用扩展帧(29位ID), 数据域长度固定为8.    
    SwapBigLittleEndian(&stCanTxMsg.m_ulFrameID, 4);
    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    stCanTxMsg.m_eFrameFormat = FRAME_FORMAT_EXT;
    stCanTxMsg.m_ucDataLen    = CAN_DATA_LEN;

    // 报文周期不宜太短.容易导致总线负载率过高.
    // Signal ID = SIG_ID_WORK_STATUS
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_WORK_STATUS);
    SendCanMsg(&stCanTxMsg,0);

    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_SPEED_INFO);
    UNPACK32_BE(stCanTxMsg.m_aucData+2,g_stSysRunInfo.m_slSpeed);    
    SendCanMsg(&stCanTxMsg,0);
    
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_ABS_POS);
    UNPACK32_BE(stCanTxMsg.m_aucData+2,g_stSysRunInfo.m_slAbsPosition);    
    SendCanMsg(&stCanTxMsg,0);
    
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_REL_POS);
    UNPACK32_BE(stCanTxMsg.m_aucData+2,g_stSysRunInfo.m_slRelPosition);
    SendCanMsg(&stCanTxMsg,0);
    
}



// 上报所有信息.正常情况下是主动上报一部分.
// 查询时,上报全部信息
void NorthUploadStatus(void)
{
    
}

// 设置具体的IO/输出.
void NorthCtrlOutput(void)
{
    // 输出控制.
    
    
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
