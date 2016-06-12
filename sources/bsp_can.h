#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#define FCAN  	40000000    // FCAN = Fosc / 2 = 80M /2 = 40M.
#define BITRATE 1000000     // 1000kbps. = 1M kbps.
#define NTQ 	20		    // 20 Time Quanta in a Bit Time
#define BRP_VAL		((FCAN/(2*NTQ*BITRATE))-1)

/* CAN Message Buffer Configuration */
#define  ECAN2_MSG_BUF_LENGTH   32

// �����DMAӲ���Ļ�����.
typedef unsigned int ECAN2MSGBUF [ECAN2_MSG_BUF_LENGTH][8]; 
extern ECAN2MSGBUF  ecan2msgBuf __attribute__((space(dma)));



#define CAN_DATA_LEN            8       // CAN���ݳ���

// ����CAN֡��ʽ����(��׼֡, ��չ֡)
typedef enum tagCanFrameFormat
{
    FRAME_FORMAT_STD = 0,       // ��׼֡
    FRAME_FORMAT_EXT = 1,       // ��չ֡
    FRAME_FORMAT_RTR = 2,       // Զ��֡
}CAN_FRAME_FORMAT_E;

// ����CAN��Ϣ��ʽ
typedef struct tagCanRxMsg
{
    u32 m_ulFrameID;                    // ֡ID(��ϢID)
    CAN_FRAME_FORMAT_E m_eFrameFormat;  // ֡����
    u8  m_ucDataLen;                    // �����򳤶��ֽ���
    u8  m_aucData[CAN_DATA_LEN];        // ����������
}CAN_MSG_ST;


#define NORTH_CAN_TX_MSG_MAX_NUM      40      // CAN������Ϣ�����Ŀ
#define NORTH_CAN_RX_MSG_MAX_NUM      40      // CAN������Ϣ�����Ŀ

// ���山�������Ϣ����
typedef struct tagNorthRxMsgQueue
{
    CAN_MSG_ST m_astRxMsg[NORTH_CAN_RX_MSG_MAX_NUM];  // ������Ϣ
    u8 m_ucWrPtr;       // ����дָ��
    u8 m_ucRdPtr;       // ���ж�ָ��
}NORTH_RX_MSG_QUEUE_ST;


extern NORTH_RX_MSG_QUEUE_ST g_stNorthRxMsgQueue;

extern void ecan2init(void);
extern void dma1init(void);
extern void dma3init(void);

void ecan2WriteMessage(void);


extern void ecan2WriteRxAcptFilter(int n, long identifier, unsigned int exide,unsigned int bufPnt,unsigned int maskSel);
extern void ecan2WriteRxAcptMask(int m, long identifierMask, unsigned int mide,unsigned int exide);

extern void ecan2WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
extern void ecan2WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);

extern void ecan2DisableRXFilter(int n);

#endif // __BSP_CAN_H__
// end of file
