#include "includes.h"

u16 usEncoderABS;

u16 usCnt = 0;

CAN_MSG_ST stMsg;

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
	    // ʵʱ����.
        NorthCanRxMsgProcess();
        Task_Sample();

        
	    if (g_ucTickCount > 0)
		{
			g_ucTickCount = 0;


			// ����������.
			
			++usCnt;
			if (usCnt >= 100)
			{
				usCnt = 0;
				North_Auto_Send();
			}
		}
	}
}
// end of file.

