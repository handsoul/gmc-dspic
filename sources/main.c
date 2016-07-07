#include "includes.h"

u16 usEncoderABS;

u16 usCnt = 0;

CAN_MSG_ST stMsg;

u8 g_ucSendByte = 0;

int main(void)
{
    int  i =0 ;
    
    System_Init();
    MOTOR_EN  = 1;
	MOTOR_DIR = 0;

    stMsg.m_ulFrameID = 0x50;
    stMsg.m_ucDataLen = 8;
    stMsg.m_eFrameFormat = FRAME_FORMAT_STD;
    
    for(i = 0 ;i <8 ;i++)
    {
        stMsg.m_aucData[i] = i ;
    }
    
	while(1)
	{
	    // 实时任务.
        NorthCanRxMsgProcess();
        Task_Sample();
        
	    if (g_ucTickCount > 0)
		{
			g_ucTickCount = 0;

			// 周期性任务.
			++usCnt;
			if (usCnt >= 100)
			{
				usCnt = 0;
				// SendByte(++g_ucSendByte);
			}
		}
	}
}
// end of file.

