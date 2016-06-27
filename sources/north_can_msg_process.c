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

    // ����ͨ��Э��Ž��д��� (���濼�Ƿŵ�CAN�˲���)
    if (unCanMsgID.m_btMsgID.m_btProtoNO != PROTO_NO_UPPER_TO_GMC)
    {
        return;
    }

	// 
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
        ucTxMsgResult = CAN0_SendMsg(&g_stNorthTxMsgQueue.m_astTxMsg[g_stNorthTxMsgQueue.m_ucRdPtr]);
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
    u16 i;
    u16 usSigID;
    
    unCanMsgID.m_ulMsgID = 0;
    unCanMsgID.m_btMsgID.m_btCntFlag = 1;       // ǰN-1֡���1, ���1֡���0.
    unCanMsgID.m_btMsgID.m_btRsv6V1  = (u8)0x3F;    // ����λ���1
    unCanMsgID.m_btMsgID.m_btMsgDir  = DIR_S2M; // ��Ϣ���䷽��
    unCanMsgID.m_btMsgID.m_btCmdID   = CMD_ID_AUTO_SEND;  // ����ID
    unCanMsgID.m_btMsgID.m_btRsv3V0  = 0x0;     // ����λ���0

    unCanMsgID.m_btMsgID.m_btProtoNO = (u8)PROTO_NO_UPPER_TO_GMC;    // Э���
    unCanMsgID.m_btMsgID.m_btDstAddr = g_ucBoardAddr   ;        // Ŀ���ַ�̶�Ϊ1 

    // ����֡�̶�ʹ����չ֡(29λID), �����򳤶ȹ̶�Ϊ8.    
    SwapBigLittleEndian(&stCanTxMsg.m_ulFrameID, 4);
    stCanTxMsg.m_ulFrameID    = unCanMsgID.m_ulMsgID;
    stCanTxMsg.m_eFrameFormat = FRAME_FORMAT_EXT;
    stCanTxMsg.m_ucDataLen    = CAN_DATA_LEN;

    // �������ڲ���̫��.���׵������߸����ʹ���.
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



// �ϱ�������Ϣ.����������������ϱ�һ����.
// ��ѯʱ,�ϱ�ȫ����Ϣ
void NorthUploadStatus(void)
{
    
}

// ���þ����IO/���.
void NorthCtrlOutput(void)
{
    // �������.
    
    
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
