#include "includes.h"

u16 usEncoderABS;

u16 usCnt = 0;

CAN_MSG_ST stMsg;

u8 g_ucSendByte = 0;

int main(void)
{
    int  i =0 ;
    
    System_Init();
    
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
				_LATG6 = !_LATG6;
				_LATG7 = !_LATG7;
				_LATG8 = !_LATG8;
				usCnt = 0;
				// SendByte(++g_ucSendByte);
			}

			ReadQeiPos();
		}
	}
}
// end of file.

