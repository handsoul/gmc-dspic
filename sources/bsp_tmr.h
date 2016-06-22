#ifndef __TMR_H__
#define __TMR_H__


#define FREQ_FOSC 80000000
#define FREQ_FCY  (FREQ_FOSC/2)


extern void Tmr_Init(void);
extern u8 g_ucTickCount;
#endif
// end of file

