/***********************************************************************************
    实现控制方法.

    //主要是位置采样 + 输出控制.
    // 基本控制方法:
    // 1. 通过位置采样估算速度,估算减速距离.
    // 2. 运行目标位置.
    // 3. 停止位置.(可能的停止位置,通过需要减速区域时,以低速通过).
    // 4. 加速距离的记录.
    // 5. 减速距离的记录.

    // 命令执行过程仅为命令解析.

**/


#include "includes.h"



// 速度和位置计算.(单位是0.0001转/分)
void CalcPosSpeed(void)
{
    // 
    if (g_stCfgParams.m_ucEncoderType == E_ENCODER_TYPE_REL_ABZ)
    {
        
    }
}

// 位移控制.
void MovementControl(void)
{
    
}

// 资源管理.
void ResourceManagement(void)
{
    
}

// 清空命令队列.
void ParseCommand(void)
{
}


// 整体控制.包括命令解析.输入信号.输出动作.
void MotorControl(void)
{
    
}
