#ifndef __CFG_MGMT_H__
#define __CFG_MGMT_H__

#include "includes.h"

typedef struct tagSysCfgStruct
{
    u8 m_ucEncoderType;   // position sensor type.
    
}SYS_PARAMS_ST;


extern SYS_PARAMS_ST g_stCfgParams;

#endif 
// end of file.