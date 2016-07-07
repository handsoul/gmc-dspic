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

    // ����ͨ��Э��Ž��д��� (���濼�Ƿŵ�CAN�˲���)
    if (unCanMsgID.m_btMsgID.m_btProtoNO != PROTO_NO_UPPER_TO_GMC)
    {
        return;
    }

    // ��ַУ��.
    if (unCanMsgID.m_btMsgID.m_btDstAddr != g_ucBoardAddr 
	    && unCanMsgID.m_btMsgID.m_btDstAddr != 0xFF ) // 0xFF��Ϊ�㲥��ַ.
    {
        return;
    }

    // ����.
    if (unCanMsgID.m_btMsgID.m_btMsgDir != DIR_M2S)
    {
        return;
    }

    if (unCanMsgID.m_btMsgID.m_btCmdID == CMD_ID_BAT_GET_STATUS)
    {
        NorthUploadStatus(pstCanRxMsg->m_aucData);
    }
    else if(unCanMsgID.m_btMsgID.m_btCmdID == CMD_ID_SIN_SET_PARAM)//���ò���.
    {
        NorthCtrlOutput(pstCanRxMsg->m_aucData);
    }
    else if(unCanMsgID.m_btMsgID.m_btCmdID == CMD_ID_BAT_GET_FIXED)
    {
        NorthUploadVersionInfo(pstCanRxMsg->m_aucData);
    }
    
    
}


/***********************************************************
 * ��������: NorthTxMsgProcess
 * ��������: ��������Ϣ����
 * �������: ��.
 * �������: ��.
 * �� �� ֵ: ��.
 * ��    ע: ���ͻ����ڼ�⵽���Ͷ������д�����ķ�����Ϣ��,
 *           ȡ�������Ϣ����.
 *           ÿ��ֻ����1����Ϣ, ���������쳣ʱ����ռ��CPU.
***********************************************************/
void NorthTxMsgProcess(void)
{
    u8 ucTxMsgResult = RTN_ERROR;
    static u8 s_ucTxErrCount = 0;
    
    if (g_stNorthTxMsgQueue.m_ucRdPtr != g_stNorthTxMsgQueue.m_ucWrPtr)
    {            
        // ȡ��������Ϣ������. һ��ֻ����һ����Ϣ
        ucTxMsgResult = SendCanMsg(&g_stNorthTxMsgQueue.m_astTxMsg[g_stNorthTxMsgQueue.m_ucRdPtr],0);
        // ��Ϣ���ͳɹ���, ���¶�ָ��
        if (ucTxMsgResult == RTN_OK)
        {
            s_ucTxErrCount = 0;
            g_stNorthTxMsgQueue.m_ucRdPtr = (u8)((g_stNorthTxMsgQueue.m_ucRdPtr + 1) % NORTH_CAN_TX_MSG_MAX_NUM);
        }
    }    
}


/***********************************************************
 * ��������: AddNorthTxEvtMsgToQueue
 * ��������: ����CAN�����¼�����Ϣ����
 * �������: pstCanTxMsg:   ������Ϣָ��.
 * �������: ��.
 * �� �� ֵ: ��.
 * ��    ע: ������ֱ����ӷ�����Ϣ��������.
***********************************************************/
void NorthSendMsgQueue(CAN_MSG_ST *pstCanTxMsg)
{
    CAN_MSG_ST stCanTxMsg = *pstCanTxMsg;
    
    if (g_stNorthTxMsgQueue.m_ucRdPtr == (g_stNorthTxMsgQueue.m_ucWrPtr + 1))
    {
        return;
    }

    // ������Ϣ���ݵ����Ͷ���
    g_stNorthTxMsgQueue.m_astTxMsg[g_stNorthTxMsgQueue.m_ucWrPtr] = stCanTxMsg;
    // ���·��Ͷ���дָ��
    g_stNorthTxMsgQueue.m_ucWrPtr = (u8)((g_stNorthTxMsgQueue.m_ucWrPtr + 1) % NORTH_CAN_TX_MSG_MAX_NUM);
}


// �Զ��ϱ�. ���8�� //Ŀǰ��������.�����ϱ�������Ŀ��Ӧ����7��(ʹ�õ�ǰ�Ļ������)
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


// �ϱ�������Ϣ.����������������ϱ�һ����.
// ��ѯʱ,�ϱ�ȫ����Ϣ
void NorthUploadStatus(u8 aucMsg[])
{
    UNUSED_ARGUEMENT(aucMsg); // ��ֹ�������������δʹ��.

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

    // ����֡�̶�ʹ����չ֡(29λID), �����򳤶ȹ̶�Ϊ8.
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

    // �������ڲ���̫��.���׵������߸����ʹ���.
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


    // ���һ֡
    unCanMsgID.m_btMsgID.m_btCntFlag = 0;
    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    // SwapBigLittleEndian(&stCanTxMsg.m_ulFrameID, 4);
    // ����֡�̶�ʹ����չ֡(29λID), �����򳤶ȹ̶�Ϊ8.
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


// ���þ����IO/���.
void NorthCtrlOutput(u8 aucMsg[])
{
    CAN_MSG_ST stCanTxMsg;    
    CAN_MSG_ID_UN unCanMsgID;
    u16 usSigID;
    u8  i = 0;
    
    // CAN_ID����ͬ�Ĳ��������
    unCanMsgID.m_ulMsgID = 0;
    unCanMsgID.m_btMsgID.m_btCntFlag = 0;       // ǰN-1֡���1, ���1֡���0.
    unCanMsgID.m_btMsgID.m_btRsv6V1  = 0x3F;    // ����λ���1
    unCanMsgID.m_btMsgID.m_btMsgDir  = DIR_S2M; // ��Ϣ���䷽��
    unCanMsgID.m_btMsgID.m_btCmdID   = CMD_ID_SIN_SET_PARAM;  // ����ID
    unCanMsgID.m_btMsgID.m_btRsv3V0  = 0x0;     // ����λ���1

    unCanMsgID.m_btMsgID.m_btProtoNO = PROTO_NO_UPPER_TO_GMC;    // Э���
    unCanMsgID.m_btMsgID.m_btDstAddr = g_ucBoardAddr;        // Ŀ���ַ�̶�Ϊ1 

    // ����֡�̶�ʹ����չ֡(29λID), �����򳤶ȹ̶�Ϊ8.    
    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    SwapBigLittleEndian(&stCanTxMsg.m_ulFrameID, 4);
    stCanTxMsg.m_eFrameFormat = FRAME_FORMAT_EXT;
    stCanTxMsg.m_ucDataLen    = CAN_DATA_LEN;
    
    // �����źŴ���.
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
            usSigID |= 0x1000;      // �ź�ID����
            break;
        }
    }

    stCanTxMsg.m_aucData[0] = HIBYTE(usSigID);
    stCanTxMsg.m_aucData[1] = LOBYTE(usSigID);
    MemorySet(&stCanTxMsg.m_aucData[2], 0, CAN_DATA_LEN-2);
    NorthSendMsgQueue(&stCanTxMsg);
    
}

// ���ò���.
void NorthSetParams(void)
{
}

// ��ѯ����.todo: �����������.
void NorthUploadParams(void)
{
    
}

// �ϱ��汾��Ϣ.
void NorthUploadVersionInfo(u8 aucMsg[])
{
    UNUSED_ARGUEMENT(aucMsg); // ��ֹ�������������δʹ��.

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

    // ����֡�̶�ʹ����չ֡(29λID), �����򳤶ȹ̶�Ϊ8.
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

    // ���һ֡
    unCanMsgID.m_btMsgID.m_btCntFlag = 0;
    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    // ����֡�̶�ʹ����չ֡(29λID), �����򳤶ȹ̶�Ϊ8
    // Signal ID = SIG_ID_AO_STATUS_1
    memset(stCanTxMsg.m_aucData,0,CAN_DATA_LEN);

    UNPACK16_BE(stCanTxMsg.m_aucData,SIG_ID_BUILD_DATE);
    UNPACK16_BE(stCanTxMsg.m_aucData+2,g_stVerInfo.m_stBuildDate.m_usBuildYear);
    stCanTxMsg.m_aucData[4] = g_stVerInfo.m_stBuildDate.m_ucBuildMonth;
    stCanTxMsg.m_aucData[5] = g_stVerInfo.m_stBuildDate.m_ucBuildDate;
    
    NorthSendMsgQueue(&stCanTxMsg);
    
}
