#ifndef __NORTH_CAN_MSG_PROCESS_H__
#define __NORTH_CAN_MSG_PROCESS_H__

// 定义CAN发送错误码
typedef enum tagCanTxErrCode
{
    RTN_OK      = TRUE,
    RTN_ERROR   = FALSE,    
}CAN_TX_ERR_CODE_E;


#define NORTH_CAN_TX_MSG_MAX_NUM      40      // CAN发送消息最大数目
#define NORTH_CAN_RX_MSG_MAX_NUM      40      // CAN接收消息最大数目

// 定义北向接收消息队列
typedef struct tagNorthRxMsgQueue
{
    CAN_MSG_ST m_astRxMsg[NORTH_CAN_RX_MSG_MAX_NUM];  // 接收消息
    u8 m_ucWrPtr;       // 队列写指针
    u8 m_ucRdPtr;       // 队列读指针
}NORTH_RX_MSG_QUEUE_ST;

// 定义发送消息队列
typedef struct tagNorthTxMsgQueue
{
    CAN_MSG_ST m_astTxMsg[NORTH_CAN_TX_MSG_MAX_NUM];  // 发送消息
    u8 m_ucWrPtr;       // 队列写指针
    u8 m_ucRdPtr;       // 队列读指针
}NORTH_TX_MSG_QUEUE_ST;



// 定义消息ID联合体
typedef union tagCanMsgIDUnion
{
    u32 m_ulMsgID;              // 消息ID(整型表示)
    struct
    {
#if 0    
        /**********************************************
        * CPU   大端模式:数据的高位，保存在内存的低地址中
        * 地址  0x0000 0x0001 0x0002 0x0003
        * 数据  0x11   0x22   0x33   0x44
        * 实际值:0x11223344
        **********************************************/
        //bits[24:31]                                                        内存地址
        u32 m_btProtoNO : 5;    // [24:28]: 协议号(协议类型)            --->  0x0000
        u32 m_btRsv3V0  : 3;    // [29:31]: 保留位(填充0)                        |   
		//bits[16:23]                                                            |
        u32 m_btDstAddr : 8;    // [16:23]: 目标节点地址                --->  0x0001
        //bits[8:15]                                                             |  
        u32 m_btCmdID   : 8;    // [8:15]:  命令ID                      --->  0x0002
        //bits[0:7]                                                              |
        u32 m_btCntFlag : 1;    // [0]:     CNT位(0: 最后1帧, 1: 还有下1帧)      |
        u32 m_btRsv6V1  : 6;    // [1:6]:   保留位(填充1)               --->  0x0003  
        u32 m_btMsgDir  : 1;    // [7]:     消息方向(0: S->M, 1: M->S)           
#else

        /**********************************************
        * CPU   小端模式:数据的高位，保存在内存的高地址中
        * 地址  0x0000 0x0001 0x0002 0x0003
        * 数据  0x44   0x33   0x22   0x11
        * 实际值:0x11223344
        **********************************************/
        //bits[0:7]                                                          内存地址
        u32 m_btCntFlag : 1;    // [0]:     CNT位(0: 最后1帧, 1: 还有下1帧)
        u32 m_btRsv6V1  : 6;    // [1:6]:   保留位(填充1)              --->  0x0000
        u32 m_btMsgDir  : 1;    // [7]:     消息方向(0: S->M, 1: M->S)          |
        //bits[8:15]                                                            |
        u32 m_btCmdID   : 8;    // [8:15]:  命令ID                     --->  0x0001
		//bits[16:23]                                                           |
        u32 m_btDstAddr : 8;    // [16:23]: 目标节点地址               --->  0x0002
        //bits[24:31]                                                           |
        u32 m_btProtoNO : 5;    // [24:28]: 协议号(协议类型)           --->  0x0003 
        u32 m_btRsv3V0  : 3;    // [29:31]: 保留位(填充0)
#endif
    }m_btMsgID;                 // 消息ID(位域表示)
}CAN_MSG_ID_UN;

//  定义协议号定义.
#define PROTO_NO_UPPER_TO_GMC   0x10 // 上位机和GMC的通信协议号.
#define PROTO_NO_CCU_TO_GMC     0x11 // 上层控制器和GMC的通信协议号.


// 定义应答结果
typedef enum tagAckResult
{
    ACK_OK = 0,         // 应答成功
    ACK_ERR_CMD_ID,     // 命令ID错误
    ACK_ERR_SIG_ID,     // 信号ID错误
    ACK_ERR_PARAM,      // 参数错误
}ACK_RESULT_E;

// 定义命令ID
typedef enum tagCmdID
{
    CMD_ID_AUTO_SEND      = 0x00,       // 主动上报

    CMD_ID_SET_ACCELERATE = 0x10,       // 加速曲线设置
    CMD_ID_GET_ACCELERATE = 0x20,       // 加速曲线查询
    
    CMD_ID_BAT_GET_FIXED  = 0x40,       // 批量获取固有信息
    CMD_ID_SIN_GET_FIXED  = 0x41,       // 单信号获取固有信息

    CMD_ID_BAT_GET_STATUS = 0x50,       // 批量获取状态信息
    CMD_ID_SIN_GET_STATUS = 0x51,       // 单信号获取状态信息
    
    CMD_ID_BAT_SET_PARAM  = 0x80,       // 批量设置参数
    CMD_ID_SIN_SET_PARAM  = 0x81,       // 单信号设置参数

    CMD_ID_BAT_GET_PARAM  = 0x90,       // 批量获取参数
    CMD_ID_SIN_GET_PARAM  = 0x91,       // 单信号获取参数
    CMD_ID_RANGE_GET_PARAM  = 0x91,     // 信号范围获取参数

    
    
}CMD_ID_E;

// 定义消息方向
typedef enum tagMsgDir
{
    DIR_S2M = 0,                // 从机 -> 主机
    DIR_M2S = 1,                // 主机 -> 从机
}MSG_DIR_E;

typedef enum tagSignalList
{
    SIG_ID_FM_VERSION  = 0x001, // 包括软硬件版本.
    SIG_ID_BUILD_DATE  = 0x002, // 编译时间.

    
    // auto send.
    SIG_ID_WORK_STATUS    = 0x100,
    SIG_ID_SPEED_INFO     = 0x101,
    SIG_ID_ABS_POS        = 0x102,
    SIG_ID_REL_POS        = 0x103,

    // SIG_ID_REL_POS + 1 ~ SIG_ID_ELETRIC_STAUS-1 reserved for future usage.
    
    // STATUS.
    SIG_ID_ELETRIC_STAUS  = 0x120,
    SIG_ID_DI_STATUS_1    = 0x121,
    SIG_ID_DI_STATUS_2    = 0x122,
    SIG_ID_DO_STATUS_1    = 0x123,
    SIG_ID_DO_STATUS_2    = 0x124,

    SIG_ID_AI_STATUS_1    = 0x125,
    SIG_ID_AI_STATUS_2    = 0x126,
    SIG_ID_AI_STATUS_3    = 0x127,
    SIG_ID_AI_STATUS_4    = 0x128,
    
    SIG_ID_AO_STAUTS_1    = 0x129,
    SIG_ID_AO_STAUTS_2    = 0x12A,

    // Control Params.
    // output.
    SIG_ID_CTRL_DO_1      = 0x150,
    SIG_ID_CTRL_DO_2      = 0x151,
    SIG_ID_CTRL_AO_1      = 0x152,
    SIG_ID_CTRL_AO_2      = 0x153,

    SIG_ID_CTRL_SW_1      = 0x154,
    SIG_ID_CTRL_SW_2      = 0x155,

    // 电机控制.
    SIG_ID_CTRL_MOTOR_1     = 0x160, // 调试用.
    SIG_ID_CTRL_MOTOR_2     = 0x161, // 调试用.

    // 加速曲线的参数设置.

    SIG_ID_SET_ACC_CURVE    = 0x180, // 

    // J命令操作.
    SIG_ID_CTRL_JCMD_START  = 0x040,
    SIG_ID_CTRL_JCMD_1      = 0x040,
    SIG_ID_CTRL_JCMD_2      = 0x041,
    SIG_ID_CTRL_JCMD_3      = 0x042,
    //
    SIG_ID_CTRL_JCMD_MAX    = 0x0FF,
    
    
}SIG_ID_E;


extern NORTH_RX_MSG_QUEUE_ST g_stNorthRxMsgQueue;
extern NORTH_TX_MSG_QUEUE_ST g_stNorthTxMsgQueue;

// 自动上报数据的周期.
#define AUTO_SND_INTERVAL_MS        200


extern void North_Auto_Send(void);
extern void NorthCanRxMsgProcess(void);
extern void NorthRxMsgHandler(CAN_MSG_ST * pstMsg);
extern void NorthTxMsgProcess(void);

extern void NorthCtrlOutput(u8 aucMsg[]);
extern void NorthUploadStatus(u8 aucMsg[]);
extern void NorthUploadVersionInfo(u8 aucMsg[]);

#endif 
// end of file.
