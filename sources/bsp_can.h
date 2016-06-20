#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#define FCAN  	40000000    // FCAN = Fosc / 2 = 80M /2 = 40M.
#define BITRATE 1000000     // 1000kbps. = 1M kbps.
#define NTQ 	20		    // 20 Time Quanta in a Bit Time
#define BRP_VAL		((FCAN/(2*NTQ*BITRATE))-1)

/* CAN Message Buffer Configuration */
#define  ECAN2_MSG_BUF_LENGTH   32

// 分配给DMA硬件的缓冲区.
typedef unsigned int ECAN2MSGBUF [ECAN2_MSG_BUF_LENGTH][8]; 
extern ECAN2MSGBUF  ecan2msgBuf __attribute__((space(dma)));

#define CAN_BUF_ADDR(ch) (&(ecan2msgBuf[ch&0x1F][0]))


#define CAN_DATA_LEN            8       // CAN数据长度

// 定义CAN帧格式类型(标准帧, 扩展帧)
typedef enum tagCanFrameFormat
{
    FRAME_FORMAT_STD = 0,       // 标准帧
    FRAME_FORMAT_EXT = 1,       // 扩展帧
    FRAME_FORMAT_RTR = 2,       // 远程帧
}CAN_FRAME_FORMAT_E;

// 定义CAN消息格式
typedef struct tagCanRxMsg
{
    u32 m_ulFrameID;                    // 帧ID(消息ID)
    CAN_FRAME_FORMAT_E m_eFrameFormat;  // 帧类型
    u8  m_ucDataLen;                    // 数据域长度字节数
    u8  m_aucData[CAN_DATA_LEN];        // 数据域内容
}CAN_MSG_ST;


#define NORTH_CAN_TX_MSG_MAX_NUM      40      // CAN发送消息最大数目
#define NORTH_CAN_RX_MSG_MAX_NUM      40      // CAN接收消息最大数目

// 定义北向接收消息队列
typedef struct tagNorthRxMsgQueue
{
    CAN_MSG_ST m_astRxMsg[NORTH_CAN_RX_MSG_MAX_NUM];  // 接收消息
    u8 m_ucWrPtr;       // 队列写指针
    u8 m_ucRdPtr;       // 队列读指针
}NORTH_RX_MSG_QUEUE_ST;


#define _W(_p,offset) *(((u16*)_p)+offset)
#define _B(_p,offset) *(((u8*)_p)+offset)

// 数据打包.
// 小端模式.

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

