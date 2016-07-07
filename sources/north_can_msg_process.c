#include "includes.h"

NORTH_RX_MSG_QUEUE_ST g_stNorthRxMsgQueue;
NORTH_TX_MSG_QUEUE_ST g_stNorthTxMsgQueue;

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
    
    // SwapBigLittleEndian(&unCanMsgID, 4);

    // 根据通信协议号进行处理 (后面考虑放到CAN滤波中)
    if (unCanMsgID.m_btMsgID.m_btProtoNO != PROTO_NO_UPPER_TO_GMC)
    {
        return;
    }

    // 地址校验.
    if (unCanMsgID.m_btMsgID.m_btDstAddr != g_ucBoardAddr 
	    && unCanMsgID.m_btMsgID.m_btDstAddr != 0xFF ) // 0xFF作为广播地址.
    {
        return;
    }

    // 方向.
    if (unCanMsgID.m_btMsgID.m_btMsgDir != DIR_M2S)
    {
        return;
    }

    if (unCanMsgID.m_btMsgID.m_btCmdID == CMD_ID_BAT_GET_STATUS)
    {
        NorthUploadStatus(pstCanRxMsg->m_aucData);
    }
    else if(unCanMsgID.m_btMsgID.m_btCmdID == CMD_ID_SIN_SET_PARAM)//设置参数.
    {
        NorthCtrlOutput(pstCanRxMsg->m_aucData);
    }
    else if(unCanMsgID.m_btMsgID.m_btCmdID == CMD_ID_BAT_GET_FIXED)
    {
        NorthUploadVersionInfo(pstCanRxMsg->m_aucData);
    }
    
    
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
        ucTxMsgResult = SendCanMsg(&g_stNorthTxMsgQueue.m_astTxMsg[g_stNorthTxMsgQueue.m_ucRdPtr],0);
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
    
    unCanMsgID.m_ulMsgID = 0;
    unCanMsgID.m_btMsgID.m_btCntFlag = 0;			// for periodic frames.
    unCanMsgID.m_btMsgID.m_btRsv6V1  = (u8)0x3F;
    unCanMsgID.m_btMsgID.m_btMsgDir  = DIR_S2M;
    unCanMsgID.m_btMsgID.m_btCmdID   = CMD_ID_AUTO_SEND;
    unCanMsgID.m_btMsgID.m_btRsv3V0  = 0x0;

    unCanMsgID.m_btMsgID.m_btProtoNO = (u8)PROTO_NO_UPPER_TO_GMC;
    unCanMsgID.m_btMsgID.m_btDstAddr = g_ucBoardAddr;

    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    stCanTxMsg.m_eFrameFormat = FRAME_FORMAT_EXT;
    stCanTxMsg.m_ucDataLen    = CAN_DATA_LEN;

    // Signal ID = SIG_ID_WORK_STATUS
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_WORK_STATUS);
    stCanTxMsg.m_aucData[2] = g_stSysRunInfo.m_ucSysWorkSt;
    stCanTxMsg.m_aucData[3] = ++g_stSysRunInfo.m_ucHeartBeatCnt;

    if(g_stSysRunInfo.m_ucHeartBeatCnt >= 100)
    {
        g_stSysRunInfo.m_ucHeartBeatCnt = 0;
    }
    
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
void NorthUploadStatus(u8 aucMsg[])
{
    UNUSED_ARGUEMENT(aucMsg); // 防止编译器警告变量未使用.

    CAN_MSG_ST stCanTxMsg;    
    CAN_MSG_ID_UN unCanMsgID;
    
    unCanMsgID.m_ulMsgID = 0;
    unCanMsgID.m_btMsgID.m_btCntFlag = 1;
    unCanMsgID.m_btMsgID.m_btRsv6V1  = (u8)0x3F;
    unCanMsgID.m_btMsgID.m_btMsgDir  = DIR_S2M;
    unCanMsgID.m_btMsgID.m_btCmdID   = CMD_ID_BAT_GET_STATUS;
    unCanMsgID.m_btMsgID.m_btRsv3V0  = 0x0;

    unCanMsgID.m_btMsgID.m_btProtoNO = (u8)PROTO_NO_UPPER_TO_GMC;
    unCanMsgID.m_btMsgID.m_btDstAddr = g_ucBoardAddr;

    // 发送帧固定使用扩展帧(29位ID), 数据域长度固定为8.
    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    stCanTxMsg.m_eFrameFormat = FRAME_FORMAT_EXT;
    stCanTxMsg.m_ucDataLen    = CAN_DATA_LEN;

    /*
    // STATUS.
    SIG_ID_ELETRIC_STAUS  = 0x104,
    SIG_ID_DI_STATUS_1    = 0x105,
    SIG_ID_DI_STATUS_2    = 0x106,
    SIG_ID_DO_STATUS_1    = 0x107,
    SIG_ID_DO_STATUS_2    = 0x108,

    SIG_ID_AI_STATUS_1    = 0x109,
    SIG_ID_AI_STATUS_2    = 0x10A,
    SIG_ID_AI_STATUS_3    = 0x10B,
    SIG_ID_AI_STATUS_4    = 0x10C,
    
    SIG_ID_AO_STAUTS_1    = 0x110,
    SIG_ID_AO_STAUTS_2    = 0x111,

    */

    // 报文周期不宜太短.容易导致总线负载率过高.
    // Signal ID = SIG_ID_ELETRIC_STAUS
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_ELETRIC_STAUS);
    UNPACK16_BE(stCanTxMsg.m_aucData+2,g_stSysRunInfo.m_usSys24vPower);
    UNPACK16_BE(stCanTxMsg.m_aucData+4,g_stSysRunInfo.m_usSys5vPower);
    UNPACK16_BE(stCanTxMsg.m_aucData+6,g_stSysRunInfo.m_ssBoardTemp);
    NorthSendMsgQueue(&stCanTxMsg);

    // Signal ID = SIG_ID_DI_STATUS_1
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_DI_STATUS_1);
    PACK_BIT0(g_stSysRunInfo.m_aucDI,DI_NUM,stCanTxMsg.m_aucData[2]);
    NorthSendMsgQueue(&stCanTxMsg);
    // Signal ID = SIG_ID_DO_STATUS_1
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_DO_STATUS_1);
    PACK_BIT0(g_stSysRunInfo.m_aucDI,DI_NUM,stCanTxMsg.m_aucData[2]);
    NorthSendMsgQueue(&stCanTxMsg);
    // Signal ID = SIG_ID_AI_STATUS_1
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_AI_STATUS_1);
    UNPACK16_BE(stCanTxMsg.m_aucData+2,g_stSysRunInfo.m_ausAI[0]);
    UNPACK16_BE(stCanTxMsg.m_aucData+4,g_stSysRunInfo.m_ausAI[1]);
    UNPACK16_BE(stCanTxMsg.m_aucData+6,g_stSysRunInfo.m_ausAI[2]);
    NorthSendMsgQueue(&stCanTxMsg);
    // Signal ID = SIG_ID_AI_STATUS_2
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_AI_STATUS_2);
    UNPACK16_BE(stCanTxMsg.m_aucData+2,g_stSysRunInfo.m_ausAI[3]);
    NorthSendMsgQueue(&stCanTxMsg);


    // 最后一帧
    unCanMsgID.m_btMsgID.m_btCntFlag = 0;
    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    // SwapBigLittleEndian(&stCanTxMsg.m_ulFrameID, 4);
    // 发送帧固定使用扩展帧(29位ID), 数据域长度固定为8.
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_AO_STAUTS_1);
    UNPACK16_BE(stCanTxMsg.m_aucData+2,g_stSysRunInfo.m_ausAO[0]);
    UNPACK16_BE(stCanTxMsg.m_aucData+4,g_stSysRunInfo.m_ausAO[1]);
    NorthSendMsgQueue(&stCanTxMsg);
}


#define SET_DO_VAL(CH,VAL)\
    DO_OUT##CH = VAL

#define SET_SW_VAL(CH,VAL)\
    DRV_SW##CH = VAL

void SetDOStatus(u8 ucChNo, u8 ucVal)
{
    if (ucVal != 0 && ucVal != 1)
    {
        return;
    }

    switch(ucChNo)
    {
        case DO_CH0:
        {
            SET_DO_VAL(0,ucVal);
            break;
        }
        case DO_CH1:
        {
            SET_DO_VAL(1,ucVal);
            break;
        }
        case DO_CH2:
        {
            SET_DO_VAL(2,ucVal);
            break;
        }
        case DO_CH3:
        {
            SET_DO_VAL(3,ucVal);
            break;
        }
        case DO_CH4:
        {
            SET_DO_VAL(4,ucVal);
            break;
        }
        case DO_CH5:
        {
            SET_DO_VAL(5,ucVal);
            break;
        }
        case DO_CH6:
        {
            SET_DO_VAL(6,ucVal);
            break;
        }
        case DO_CH7:
        {
            SET_DO_VAL(7,ucVal);
            break;
        }
        default:
            break;
    }
}

void SetSwStatus(u8 ucChNo, u8 ucVal)
{
    if (ucVal != 0 && ucVal != 1)
    {
        return;
    }

    switch(ucChNo)
    {
        case DO_SW0:
        {
            SET_SW_VAL(0,ucVal);
            break;
        }
        case DO_SW1:
        {
            SET_SW_VAL(1,ucVal);
            break;
        }
        case DO_SW2:
        {
            SET_SW_VAL(2,ucVal);
            break;
        }
        case DO_SW3:
        {
            SET_SW_VAL(3,ucVal);
            break;
        }
        case DO_SW4:
        {
            SET_SW_VAL(4,ucVal);
            break;
        }
        case DO_SW5:
        {
            SET_SW_VAL(5,ucVal);
            break;
        }
        case DO_SW6:
        {
            SET_SW_VAL(6,ucVal);
            break;
        }
        case DO_SW7:
        {
            SET_SW_VAL(7,ucVal);
            break;
        }
        default:
            break;
    }
}


// 设置具体的IO/输出.
void NorthCtrlOutput(u8 aucMsg[])
{
    CAN_MSG_ST stCanTxMsg;    
    CAN_MSG_ID_UN unCanMsgID;
    u16 usSigID;
    u8  i = 0;
    
    // CAN_ID中相同的部分先填充
    unCanMsgID.m_ulMsgID = 0;
    unCanMsgID.m_btMsgID.m_btCntFlag = 0;       // 前N-1帧填充1, 最后1帧填充0.
    unCanMsgID.m_btMsgID.m_btRsv6V1  = 0x3F;    // 保留位填充1
    unCanMsgID.m_btMsgID.m_btMsgDir  = DIR_S2M; // 消息传输方向
    unCanMsgID.m_btMsgID.m_btCmdID   = CMD_ID_SIN_SET_PARAM;  // 命令ID
    unCanMsgID.m_btMsgID.m_btRsv3V0  = 0x0;     // 保留位填充1

    unCanMsgID.m_btMsgID.m_btProtoNO = PROTO_NO_UPPER_TO_GMC;    // 协议号
    unCanMsgID.m_btMsgID.m_btDstAddr = g_ucBoardAddr;        // 目标地址固定为1 

    // 发送帧固定使用扩展帧(29位ID), 数据域长度固定为8.    
    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    SwapBigLittleEndian(&stCanTxMsg.m_ulFrameID, 4);
    stCanTxMsg.m_eFrameFormat = FRAME_FORMAT_EXT;
    stCanTxMsg.m_ucDataLen    = CAN_DATA_LEN;
    
    // 根据信号处理.
    usSigID = (u16)(((aucMsg[0] & 0x0F) << 8)|(aucMsg[1]) );


    switch(usSigID)
    {
        case SIG_ID_CTRL_DO_1:
        {
			for(i = 0 ;i < 6;i++)
			{
			    SetDOStatus(i,aucMsg[2+i]);
			}
            break;
        }
        case SIG_ID_CTRL_DO_2:
        {   
            for(i = 0 ;i < 2;i++)
			{
			    SetDOStatus(i+6,aucMsg[2+i]);
			}
            break;
        }
        case SIG_ID_CTRL_AO_2:
        {
            break;
        }
        case SIG_ID_CTRL_SW_1:
        {
            for(i = 0 ;i < 6;i++)
			{
			    SetSwStatus(i,aucMsg[2+i]);
			}
            break;
        }
        case SIG_ID_CTRL_SW_2:
        {
            for(i = 0 ;i < 2;i++)
			{
			    SetSwStatus(i+6,aucMsg[2+i]);
			}
            break;
        }
        case SIG_ID_CTRL_MOTOR_1:
        {
            break;
        }
        case SIG_ID_CTRL_MOTOR_2:
        {
            break;
        }        
        default:
        {
            usSigID |= 0x1000;      // 信号ID错误
            break;
        }
    }

    stCanTxMsg.m_aucData[0] = HIBYTE(usSigID);
    stCanTxMsg.m_aucData[1] = LOBYTE(usSigID);
    MemorySet(&stCanTxMsg.m_aucData[2], 0, CAN_DATA_LEN-2);
    NorthSendMsgQueue(&stCanTxMsg);
    
}

// 设置参数.
void NorthSetParams(void)
{
}

// 查询参数.todo: 引入参数管理.
void NorthUploadParams(void)
{
    
}

// 上报版本信息.
void NorthUploadVersionInfo(u8 aucMsg[])
{
    UNUSED_ARGUEMENT(aucMsg); // 防止编译器警告变量未使用.

    CAN_MSG_ST stCanTxMsg;    
    CAN_MSG_ID_UN unCanMsgID;
    
    unCanMsgID.m_ulMsgID = 0;
    unCanMsgID.m_btMsgID.m_btCntFlag = 1;
    unCanMsgID.m_btMsgID.m_btRsv6V1  = (u8)0x3F;
    unCanMsgID.m_btMsgID.m_btMsgDir  = DIR_S2M;
    unCanMsgID.m_btMsgID.m_btCmdID   = CMD_ID_BAT_GET_FIXED;
    unCanMsgID.m_btMsgID.m_btRsv3V0  = 0x0;

    unCanMsgID.m_btMsgID.m_btProtoNO = (u8)PROTO_NO_UPPER_TO_GMC;
    unCanMsgID.m_btMsgID.m_btDstAddr = g_ucBoardAddr;

    // 发送帧固定使用扩展帧(29位ID), 数据域长度固定为8.
    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    stCanTxMsg.m_eFrameFormat = FRAME_FORMAT_EXT;
    stCanTxMsg.m_ucDataLen    = CAN_DATA_LEN;


    // Signal ID = SIG_ID_FM_VERSION
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);
    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_FM_VERSION);
    stCanTxMsg.m_aucData[2] = g_stVerInfo.m_ucSoftVerMajor;
    stCanTxMsg.m_aucData[3] = g_stVerInfo.m_ucSoftVerMinor;
    UNPACK16_BE(stCanTxMsg.m_aucData+4,g_stVerInfo.m_usSoftVerBuild);
    stCanTxMsg.m_aucData[6] = g_stVerInfo.m_scHardVerion;
    stCanTxMsg.m_aucData[7] = g_stVerInfo.m_scBomVersion;
    NorthSendMsgQueue(&stCanTxMsg);

    // 最后一帧
    unCanMsgID.m_btMsgID.m_btCntFlag = 0;
    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    // 发送帧固定使用扩展帧(29位ID), 数据域长度固定为8
    // Signal ID = SIG_ID_AO_STATUS_1
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);

    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_BUILD_DATE);
    UNPACK16_BE(stCanTxMsg.m_aucData+2,g_stVerInfo.m_stBuildDate.m_usBuildYear);
    stCanTxMsg.m_aucData[4] = g_stVerInfo.m_stBuildDate.m_ucBuildMonth;
    stCanTxMsg.m_aucData[5] = g_stVerInfo.m_stBuildDate.m_ucBuildDate;
    
    NorthSendMsgQueue(&stCanTxMsg);
    
}
