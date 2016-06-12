#ifndef __NORTH_CAN_MSG_PROCESS_H__
#define __NORTH_CAN_MSG_PROCESS_H__

#define MUTEX_SET_VALUE(a, b)       \
    do          \
    {           \
        a = b;  \
    } while (a != b)

extern void NorthCanRxMsgProcess(void);
extern void NorthRxMsgHandler(CAN_MSG_ST * pstMsg);
#endif 
// end of file.
