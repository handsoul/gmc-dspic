#include "includes.h"


/*
 * 0 - 7 可配置为接收/发送缓冲区.
 * 0 -14 可用于指定对应过滤器的地址.(每4个过滤器共用一个区域)
 * 15-31 只能用于FIFO
 * FIFO 可配置为4,6,8,12,16.24,32个深度.
 * 目前认可的做法.
 * 1. 预留一个接收缓冲区用于特殊作用.除过滤器1之外，其他过滤器的报文一律指向该位置.
 * 2. 0-7设置为发送缓冲区.
 * 3. 8-31设置为接收FIFO缓冲区.
 * 基本解决
 */

// 配置CAN通信.

// 缓冲区用于发送.
ECAN2MSGBUF ecan2msgBuf __attribute__((space(dma),aligned(ECAN2_MSG_BUF_LENGTH*16)));

void dma1init(void)
{

    // SIZE = 0 , 按照字(2字节)传输数据.
    // DIR  = 1 , 从DPSRAM读取数据.写入外设.
    // HALF = 0 , 数据传输一半时不产生中断.
    // AMODE= 2 , 外设间接寻址模式.
    // MODE = 0 , 正常工作模式.禁止乒乓操作.
    DMA1CON=0x2020;     

    // 配置外设寄存器地址: ECAN2发送寄存器.
    DMA1PAD=(int)&C2TXD;

    // 重复(N+1)个size大小的数据之后认为数据传输完成. 
    // CAN报文的组织方式是16个字节.N=16/2 - 1 = 7.
    DMA1CNT=0x0007;	

    // 选择与DMA通道1绑定的外设事件.ECAN2发送数据请求.
    DMA1REQ=0x0047;

    // 选择数据基地址.
    DMA1STA= __builtin_dmaoffset(ecan2msgBuf);		

    _DMA1IP = 1;
    _DMA1IE = 1;
    
    // 使能DMA1.
    DMA1CONbits.CHEN=1;
}


void dma3init(void)
{
    // SIZE = 0 , 按照字(2字节)传输数据.
    // DIR  = 0 , 从DPSRAM读取数据.写入外设.
    // HALF = 0 , 数据传输一半时不产生中断.
    // AMODE= 2 , 外设间接寻址模式.
    // MODE = 0 , 正常工作模式.禁止乒乓操作.
    DMA3CON=0x0020;

    // 配置外设寄存器地址: ECAN2接收寄存器.
    DMA3PAD=(int)&C2RXD;

    // CAN报文的组织方式是16个字节.N=16/2 - 1 = 7.
    DMA3CNT=0x0007;

    // 选择与DMA通道1绑定的外设事件.ECAN2接收数据就绪.
    DMA3REQ=0x0037;

    // DMA专用RAM区域分配的DPSRAM基地址.
    DMA3STA=__builtin_dmaoffset(ecan2msgBuf);	

    _DMA3IP = 7; 
    // 中断允许.
    _DMA3IE = 1;

    // 使能
    DMA3CONbits.CHEN=1;
    
}

// CAN时钟配置.
void ecan2ClkInit(void)
{

	C2CTRL1bits.CANCKS = 0x1;

/*
Bit Time = (Sync Segment + Propagation Delay + Phase Segment 1 + Phase Segment 2)=20*TQ
Phase Segment 1 = 8TQ
Phase Segment 2 = 6Tq
Propagation Delay = 5Tq
Sync Segment = 1TQ
*/

	/* Synchronization Jump Width set to 4 TQ */
	C2CFG1bits.SJW = 0x3;
	/* Baud Rate Prescaler */
	C2CFG1bits.BRP = BRP_VAL;
	
	/* Phase Segment 1 time is 8 TQ */
	C2CFG2bits.SEG1PH=0x7;
	/* Phase Segment 2 time is set to be programmable */
	C2CFG2bits.SEG2PHTS = 0x1;
	/* Phase Segment 2 time is 6 TQ */
	C2CFG2bits.SEG2PH = 0x5;
	/* Propagation Segment time is 5 TQ */
	C2CFG2bits.PRSEG = 0x4;
	/* Bus line is sampled three times at the sample point */
	C2CFG2bits.SAM = 0x1;
               
}


/* ECAN Transmit Message Buffer Configuration

Inputs:
buf	-> Transmit Buffer Number

txIdentifier ->	

Extended Identifier (29-bits) : 0b000f ffff ffff ffff ffff ffff ffff ffff
								     |____________|_____________________|
									        SID10:0           EID17:0



Standard Identifier (11-bits) : 0b0000 0000 0000 0000 0000 0fff ffff ffff
														    |___________|
															      SID10:0

Standard Message Format: 
											Word0 : 0b000f ffff ffff ffff
													     |____________|||___
 									        				SID10:0   SRR   IDE     

											Word1 : 0b0000 0000 0000 0000
														   |____________|
															  EID17:6

											Word2 : 0b0000 00f0 0000 ffff
													  |_____||	  	 |__|
													  EID5:0 RTR   	  DLC
										
																  
																	
				Extended Message Format: 
											Word0 : 0b000f ffff ffff ffff
													     |____________|||___
 									        				SID10:0   SRR   IDE     

											Word1 : 0b0000 ffff ffff ffff
														   |____________|
															  EID17:6

											Word2 : 0bffff fff0 0000 ffff
													  |_____||	  	 |__|
													  EID5:0 RTR   	  DLC

ide -> "0"  Message will transmit standard identifier
	   "1"  Message will transmit extended identifier



remoteTransmit -> "0" Message transmitted is a normal message
				  "1" Message transmitted is a remote message

				Standard Message Format: 
											Word0 : 0b000f ffff ffff ff1f
													     |____________|||___
 									        				SID10:0   SRR   IDE     

											Word1 : 0b0000 0000 0000 0000
														   |____________|
															  EID17:6

											Word2 : 0b0000 0010 0000 ffff
													  |_____||	  	 |__|
													  EID5:0 RTR   	  DLC
										
																  
																	
				Extended Message Format: 
											Word0 : 0b000f ffff ffff ff1f
													     |____________|||___
 									        				SID10:0   SRR   IDE     

											Word1 : 0b0000 ffff ffff ffff
														   |____________|
															  EID17:6

											Word2 : 0bffff ff10 0000 ffff
													  |_____||	  	 |__|
													  EID5:0 RTR   	  DLC

*/

void ecan2WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit){

unsigned long word0=0, word1=0, word2=0;
unsigned long sid10_0=0, eid5_0=0, eid17_6=0;


if(ide)
	{
		eid5_0  = (txIdentifier & 0x3F);
		eid17_6 = (txIdentifier>>6) & 0xFFF;
		sid10_0 = (txIdentifier>>18) & 0x7FF;
		word1 = eid17_6;
	}
	else
	{
		sid10_0 = (txIdentifier & 0x7FF);
	}
	
	
	if(remoteTransmit==1) { 	// Transmit Remote Frame

		word0 = ((sid10_0 << 2) | ide | 0x2);
		word2 = ((eid5_0 << 10)| 0x0200);}

	else {
		
		word0 = ((sid10_0 << 2) | ide);
		word2 = (eid5_0 << 10);
	     }
			
// Obtain the Address of Transmit Buffer in DMA RAM for a given Transmit Buffer number

if(ide)
	ecan2msgBuf[buf][0] = (word0 | 0x0002);
else
	ecan2msgBuf[buf][0] = word0;

	ecan2msgBuf[buf][1] = word1;
	ecan2msgBuf[buf][2] = word2;
}


/* ECAN Transmit Data

Inputs :
buf -> Transmit Buffer Number

dataLength -> Length of Data in Bytes to be transmitted

data1/data2/data3/data4 ->  Transmit Data Bytes 

*/

void ecan2WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4){

	ecan2msgBuf[buf][2] = ((ecan2msgBuf[buf][2] & 0xFFF0) + dataLength) ;
	
	ecan2msgBuf[buf][3] = data1;
	ecan2msgBuf[buf][4] = data2;
	ecan2msgBuf[buf][5] = data3;
	ecan2msgBuf[buf][6] = data4;

}

/*------------------------------------------------------------------------------
 	Disable RX Acceptance Filter
	void ecan1DisableRXFilter(int n)
*/

/*
n -> Filter number [0-15]
*/

void ecan2DisableRXFilter(int n)
{
unsigned int *fltEnRegAddr;
   C2CTRL1bits.WIN=1;
   fltEnRegAddr = (unsigned int *)(&C2FEN1);
  *fltEnRegAddr = (*fltEnRegAddr) & (0xFFFF - (0x1 << n));
   C2CTRL1bits.WIN=0;

}


/* ECAN2 buffer loaded with Identifiers and Data */

void ecan2WriteMessage(void){


/* Writing the message for Transmission

ecan2WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
ecan2WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);

buf -> Transmit Buffer Number

txIdentifier -> SID<10:0> : EID<17:0>

ide = 0 -> Message will transmit standard identifier
ide = 1 -> Message will transmit extended identifier

remoteTransmit = 0 -> Normal message
remoteTransmit = 1 -> Message will request remote transmission

dataLength -> Data length can be from 0 to 8 bytes

data1, data2, data3, data4 -> Data words (2 bytes) each


*/

ecan2WriteTxMsgBufId(0,0x1FFEFFFF,1,0);
ecan2WriteTxMsgBufData(0,8,0xAAAA,0xAAAA,0xAAAA,0xAAAA);

ecan2WriteTxMsgBufId(1,0x1FFEFFFF,1,0);
ecan2WriteTxMsgBufData(1,8,0xBBBB,0xBBBB,0xBBBB,0xBBBB);

ecan2WriteTxMsgBufId(2,0x1FFEFFFF,1,0);
ecan2WriteTxMsgBufData(2,8,0xCCCC,0xCCCC,0xCCCC,0xCCCC);

ecan2WriteTxMsgBufId(3,0x1FFEFFFF,1,0);
ecan2WriteTxMsgBufData(3,8,0xDDDD,0xDDDD,0xDDDD,0xDDDD);

ecan2WriteTxMsgBufId(4,0x1FFEFFFF,1,0);
ecan2WriteTxMsgBufData(4,8,0xEEEE,0xEEEE,0xEEEE,0xEEEE);

ecan2WriteTxMsgBufId(5,0x1FFEFFFF,1,0);
ecan2WriteTxMsgBufData(5,8,0xFFFF,0xFFFF,0xFFFF,0xFFFF);

}



void ecan2init(void)
{
    // 进入配置模式.
	C2CTRL1bits.REQOP=4;
	while(C2CTRL1bits.OPMODE!=4);

    // 配置波特率.
	C2CTRL1bits.CANCKS = 0x1;

	// 同步跳转宽度为4个TQ.
	C2CFG1bits.SJW = 0x3;
	// 预分频器.
	C2CFG1bits.BRP = BRP_VAL;
	// 相位段1,设置为8 TQ
	C2CFG2bits.SEG1PH=0x7;
	// 相位段2长度为可设置
	C2CFG2bits.SEG2PHTS = 0x1;
	// 相位段2设置为6 TQ
	C2CFG2bits.SEG2PH = 0x5;
	// 传输段设置为5 TQ
	C2CFG2bits.PRSEG = 0x4;
	// 采样点位置采样三次.
	C2CFG2bits.SAM = 0x1;

	

    // 配置接收FIFO
	C2FCTRLbits.FSA     = 8;		// FIFO从缓冲区8开始
	C2FCTRLbits.DMABS   = 6;		// FIFO的结束地址


    // 接收滤波设置.
    C2CTRL1bits.WIN = 1;
    
    // 仅开启滤波器1.
    C2FEN1bits.FLTEN = 0xFFFF; 

    // 屏蔽器配置.
    C2RXM0SID = 0x0000;
    C2RXM1SID = 0x0000;
    C2RXM2SID = 0x0000;

    C2RXM0EID = 0x0000;
    C2RXM1EID = 0x0000;
    C2RXM2EID = 0x0000;

    // 屏蔽器选择.
    C2FMSKSEL1 = 0x0000;
    C2FMSKSEL2 = 0x0000;
 
    // 所有接受数据都存放在FIFO的缓冲区中.
    C2BUFPNT1   = 0xFFFF;
    C2BUFPNT2   = 0xFFFF;
    C2BUFPNT3   = 0xFFFF;
    C2BUFPNT4   = 0xFFFF;
    
    C2CTRL1bits.WIN = 0;

    // 工作在正常模式.
	C2CTRL1bits.REQOP=0;
	while(C2CTRL1bits.OPMODE!=0);
	
	C2RXFUL1=C2RXFUL2=C2RXOVF1=C2RXOVF2=0x0000;
    // 	0到7配置为发送缓冲区.
	C2TR01CONbits.TXEN0=1;			// ECAN2, Buffer 0 is a Transmit Buffer 
	C2TR01CONbits.TXEN1=1;			// ECAN2, Buffer 1 is a Transmit Buffer
	C2TR23CONbits.TXEN2=1;			// ECAN2, Buffer 2 is a Transmit Buffer 
	C2TR23CONbits.TXEN3=1;			// ECAN2, Buffer 3 is a Transmit Buffer 
	C2TR45CONbits.TXEN4=1;			// ECAN2, Buffer 4 is a Transmit Buffer 
	C2TR45CONbits.TXEN5=1;			// ECAN2, Buffer 5 is a Transmit Buffer 
	C2TR67CONbits.TXEN6=1;			// ECAN2, Buffer 6 is a Transmit Buffer
	C2TR67CONbits.TXEN7=1;			// ECAN2, Buffer 7 is a Transmit Buffer

	// 配置发送缓冲区的优先级.
	C2TR01CONbits.TX0PRI=0b11;
	C2TR01CONbits.TX1PRI=0b11; 		// Message Buffer 1 Priority Level
	C2TR23CONbits.TX2PRI=0b11; 		// Message Buffer 2 Priority Level
	C2TR23CONbits.TX3PRI=0b11; 		// Message Buffer 3 Priority Level
	C2TR45CONbits.TX4PRI=0b11; 		// Message Buffer 4 Priority Level
	C2TR45CONbits.TX5PRI=0b11; 		// Message Buffer 5 Priority Level
	C2TR67CONbits.TX6PRI=0b11; 		// Message Buffer 6 Priority Level
	C2TR67CONbits.TX7PRI=0b11; 		// Message Buffer 7 Priority Level

    // 中断允许位.
    _C2IE = 1;
    _C2IP = 1;

    // 允许发送和接收中断.
    C2INTEbits.TBIE = 1;
    C2INTEbits.RBIE = 1;
    
}


void clearIntrflags(void){
/* Clear Interrupt Flags */

	IFS0=0;
	IFS1=0;
	IFS2=0;
	IFS3=0;
	IFS4=0;
}


// 中断.
void FAST_ISR _DMA0Interrupt(void)
{
   IFS0bits.DMA0IF = 0;          // Clear the DMA0 Interrupt Flag;
}

void FAST_ISR _DMA1Interrupt(void)
{
   IFS0bits.DMA1IF = 0;          // Clear the DMA1 Interrupt Flag;
}

void FAST_ISR _DMA2Interrupt(void)
{
   IFS1bits.DMA2IF = 0;          // Clear the DMA2 Interrupt Flag;
}

void FAST_ISR _DMA3Interrupt(void)
{
   IFS2bits.DMA3IF = 0;          // Clear the DMA3 Interrupt Flag;
}

void FAST_ISR _C1Interrupt(void)  
{    
	IFS2bits.C1IF = 0;        // clear interrupt flag
	if(C1INTFbits.TBIF)
    { 
    	C1INTFbits.TBIF = 0;
    } 
 
    if(C1INTFbits.RBIF)
    {      
		C1INTFbits.RBIF = 0;
	}
}

// 发送缓冲区是否空的查询.
// 优先级最高的发送缓冲区,最后使用.避免出现高优先级缓冲区一直忙.低优先级缓冲区报文无法发出的情况.
inline void TriggerSend(u8 ucChNo)
{
#define TX_SFR_BASE_ADDR ((u8*)&C2TR01CONbits)    
    *(TX_SFR_BASE_ADDR + ucChNo) = *(TX_SFR_BASE_ADDR + ucChNo) | 0x08;
#undef TX_SFR_BASE_ADDR
}

inline u8 GetNextFreeCanBuf(void)
{
    static u8 _t[256] = 
    {
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,7,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,
        0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,8,
    };

    u8 ucBufSt =( (C2TR01CONbits.TXREQ0 << 0)
                |(C2TR01CONbits.TXREQ1 << 1)
                |(C2TR23CONbits.TXREQ2 << 2)
                |(C2TR23CONbits.TXREQ3 << 3)
                |(C2TR45CONbits.TXREQ4 << 4)
                |(C2TR45CONbits.TXREQ5 << 5)
                |(C2TR67CONbits.TXREQ6 << 6)
                |(C2TR67CONbits.TXREQ7 << 7));
    // 1表示正在发送.0表示空闲.
    // 寻找ucBufSt的最低0bit位.

    return _t[ucBufSt];
}



//  发送CAN报文
bool SendCanMsg(CAN_MSG_ST * pstMsg,u8 ucMsgBufNo)
{
    u8 i = GetNextFreeCanBuf();

    if (i >= 8)
    {
        return FALSE; // 缓冲区满.
    }

    // PACK_CAN_MSG(SID,SRR,IDE,RTR,RB0,RB1,DLC,PBUF,DSRC)
    if (pstMsg->m_eFrameFormat == FRAME_FORMAT_STD)
    {
        PACK_CAN_MSG((pstMsg->m_ulFrameID&0x7FFL),0,0,0,0,0,0,0,pstMsg->m_ucDataLen,CAN_BUF_ADDR(i),pstMsg->m_aucData);
    }
    else if (pstMsg->m_eFrameFormat == FRAME_FORMAT_EXT)
    {
        PACK_CAN_MSG((pstMsg->m_ulFrameID>>18),(pstMsg->m_ulFrameID&0x3FL),((pstMsg->m_ulFrameID>>6)&0xFFF),0,1,0,0,0,pstMsg->m_ucDataLen,CAN_BUF_ADDR(i),pstMsg->m_aucData);
    }

    TriggerSend(i);

    return TRUE;
}

void ReadCanMsg(const u16 * pusData,CAN_MSG_ST * pstMsg)
{
    u8 ucIdeFlag;
    u8 ucSrrFlag;

    ucIdeFlag = (u8)pusData[0] & 0x0001;
    ucSrrFlag = (u8)pusData[0] & 0x0002;
    // 解析定义.
    if (ucIdeFlag == 0) // 标准帧.
    {
        pstMsg->m_ulFrameID = (pusData[0] & 0x1FFC) >> 2;
		pstMsg->m_eFrameFormat = FRAME_FORMAT_STD;
    }
    else
    {
        pstMsg->m_ulFrameID = ((u32)(pusData[0] & 0x1FFC) << 16)
                              | ((u32)(pusData[1] & 0x0FFF) << 6)
                              | ((u32)(pusData[1] & 0xFC00) >> 10);
        pstMsg->m_eFrameFormat = FRAME_FORMAT_EXT;
    }

    if (ucSrrFlag == 1)
    {
        pstMsg->m_eFrameFormat = FRAME_FORMAT_RTR;
        pstMsg->m_ucDataLen = 0;
    }
    else
    {
        pstMsg->m_ucDataLen = pusData[2] & 0x000F;
        pstMsg->m_aucData[0] = (u8)((pusData[3]>>0) & 0x00FF);
        pstMsg->m_aucData[1] = (u8)((pusData[3]>>8) & 0x00FF);
        pstMsg->m_aucData[2] = (u8)((pusData[4]>>0) & 0x00FF);
        pstMsg->m_aucData[3] = (u8)((pusData[4]>>8) & 0x00FF);
        pstMsg->m_aucData[4] = (u8)((pusData[5]>>0) & 0x00FF);
        pstMsg->m_aucData[5] = (u8)((pusData[5]>>8) & 0x00FF);
        pstMsg->m_aucData[6] = (u8)((pusData[6]>>0) & 0x00FF);
        pstMsg->m_aucData[7] = (u8)((pusData[6]>>8) & 0x00FF);
    }
    
}

void  FAST_ISR _C2Interrupt(void)  
{
    static CAN_MSG_ST stCanRxMsg;       // 接收消息
    u8 ucFnrb;
    
	IFS3bits.C2IF = 0;        // clear interrupt flag

	if(C2INTFbits.TBIF)
    { 
		C2INTFbits.TBIF = 0;
    } 
    
    if(C2INTFbits.RBIF)
    {   
        C2INTFbits.RBIF = 0;
        
        ucFnrb = C2FIFO & 0x03F;

        // 清空对应的bit.
        if (ucFnrb >= 16)
        {
            C2RXFUL2 &= (u16)~((u16)0x01 << (ucFnrb-16));
        }
        else
        {
            C2RXFUL1 &= (u16)~((u16)0x01 << ucFnrb);
        }
        
        ReadCanMsg((u16*)ecan2msgBuf[ucFnrb],&stCanRxMsg);

        // 接收消息加入到消息队列中, 更新消息队列指针
        g_stNorthRxMsgQueue.m_astRxMsg[g_stNorthRxMsgQueue.m_ucWrPtr] = stCanRxMsg;
        g_stNorthRxMsgQueue.m_ucWrPtr = (u8)(g_stNorthRxMsgQueue.m_ucWrPtr + 1) % NORTH_CAN_RX_MSG_MAX_NUM;
    }
}


// CAN硬件初始化.
void CAN_Init(void)
{
    dma1init();
    dma3init();
    ecan2init();
}

// end of file
