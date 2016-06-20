#include "includes.h"

u16 usEncoderABS;

u16 usCnt = 0;

CAN_MSG_ST stMsg;

int main(void)
{
    int  i =0 ;
    
    System_Init();
    dma1init();
    dma3init();
    ecan2init();
    PWM_Init();
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
	    // usEncoderABS = ENCODER_ABS_VALUE;
        // clearIntrflags();

        NorthCanRxMsgProcess();
        
	    if (g_ucTickCount > 0)
		{
			g_ucTickCount = 0;

			// MOTOR_DIR = !MOTOR_DIR;			

			// MOTOR_EN  = !MOTOR_EN;

			// MOTOR_STEP= !MOTOR_STEP;			

			++usCnt;
			if (usCnt >= 100)
			{
				usCnt = 0;

				stMsg.m_eFrameFormat = FRAME_FORMAT_STD;
                stMsg.m_ulFrameID++;
                for(i = 0 ;i <8 ;i++)
                {
                    stMsg.m_aucData[i] = stMsg.m_ulFrameID + i;
                }

                SendCanMsg(&stMsg,0);

                stMsg.m_ulFrameID++;
                for(i = 0 ;i <8 ;i++)
                {
                    stMsg.m_aucData[i] = stMsg.m_ulFrameID + i;
                }

                SendCanMsg(&stMsg,1);
                
				// 
				stMsg.m_ulFrameID++;
                for(i = 0 ;i <8 ;i++)
                {
                    stMsg.m_aucData[i] = stMsg.m_ulFrameID + i;
                }
				stMsg.m_eFrameFormat = FRAME_FORMAT_EXT;

                SendCanMsg(&stMsg,1);
                /*ecan2WriteMessage();
                C2TR01CONbits.TXREQ0=1;
                C2TR01CONbits.TXREQ1=1;
                C2TR23CONbits.TXREQ2=1;
                C2TR23CONbits.TXREQ3=1;
                C2TR45CONbits.TXREQ4=1;
                C2TR45CONbits.TXREQ5=1;
				*/
			}
		}
	}
}
// end of file.

