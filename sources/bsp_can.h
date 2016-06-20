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

#define CAN_BUF_ADDR(ch) (&(ecan2msgBuf[ch&0x1F][0]))


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


#define _W(_p,offset) *(((u16*)_p)+offset)
#define _B(_p,offset) *(((u8*)_p)+offset)

// ���ݴ��.
// С��ģʽ.

#define PACK_CAN_MSG(SID,EID_05,EID_6_17,SRR,IDE,RTR,RB0,RB1,DLC,PBUF,DSRC) \
{\
    u8 __i = 0;\
     _W(PBUF,0) = ((SID&0x3FF)<<2)|((SRR&1)<<1)|(IDE&0x01);\
     _W(PBUF,1) = (EID_6_17&0xFFF);\
     _W(PBUF,2) = ((EID_05&0x1F)<<10)|((RTR&0x01)<<9)|((RB1&0x01)<<8)|((RB0&0x01)<<4)|(DLC&0x0F);\
     for (__i = 0;__i < DLC;__i++)\
     {\
        _B(PBUF,6+__i) = _B(DSRC,__i);\
     }\
}


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

