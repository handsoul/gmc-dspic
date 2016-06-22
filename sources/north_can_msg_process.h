#ifndef __NORTH_CAN_MSG_PROCESS_H__
#define __NORTH_CAN_MSG_PROCESS_H__



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
#endif

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
	SIG_ID_CELL_VOLT_1 = 0x100,
	SIG_ID_CELL_VOLT_2 = 0x101,
	SIG_ID_CELL_VOLT_3 = 0x102,
	SIG_ID_CELL_VOLT_4 = 0x103,
	SIG_ID_CELL_VOLT_5 = 0x104,
	SIG_ID_CELL_VOLT_6 = 0x105,
	SIG_ID_CELL_VOLT_7 = 0x106,
	SIG_ID_CELL_VOLT_8 = 0x107,

	SIG_ID_CELL_TEMP_1 = 0x110,
	SIG_ID_CELL_TEMP_2 = 0x111,
	
	SID_ID_MISC_INFO   = 0x120,//温度等其他信息.
}SIG_ID_E;


extern void NorthCanRxMsgProcess(void);
extern void NorthRxMsgHandler(CAN_MSG_ST * pstMsg);
#endif 
// end of file.
