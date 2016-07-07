#ifndef __NORTH_CAN_MSG_PROCESS_H__
#define __NORTH_CAN_MSG_PROCESS_H__

// ����CAN���ʹ�����
typedef enum tagCanTxErrCode
{
    RTN_OK      = TRUE,
    RTN_ERROR   = FALSE,    
}CAN_TX_ERR_CODE_E;


#define NORTH_CAN_TX_MSG_MAX_NUM      40      // CAN������Ϣ�����Ŀ
#define NORTH_CAN_RX_MSG_MAX_NUM      40      // CAN������Ϣ�����Ŀ

// ���山�������Ϣ����
typedef struct tagNorthRxMsgQueue
{
    CAN_MSG_ST m_astRxMsg[NORTH_CAN_RX_MSG_MAX_NUM];  // ������Ϣ
    u8 m_ucWrPtr;       // ����дָ��
    u8 m_ucRdPtr;       // ���ж�ָ��
}NORTH_RX_MSG_QUEUE_ST;

// ���巢����Ϣ����
typedef struct tagNorthTxMsgQueue
{
    CAN_MSG_ST m_astTxMsg[NORTH_CAN_TX_MSG_MAX_NUM];  // ������Ϣ
    u8 m_ucWrPtr;       // ����дָ��
    u8 m_ucRdPtr;       // ���ж�ָ��
}NORTH_TX_MSG_QUEUE_ST;



// ������ϢID������
typedef union tagCanMsgIDUnion
{
    u32 m_ulMsgID;              // ��ϢID(���ͱ�ʾ)
    struct
    {
#if 0    
        /**********************************************
        * CPU   ���ģʽ:���ݵĸ�λ���������ڴ�ĵ͵�ַ��
        * ��ַ  0x0000 0x0001 0x0002 0x0003
        * ����  0x11   0x22   0x33   0x44
        * ʵ��ֵ:0x11223344
        **********************************************/
        //bits[24:31]                                                        �ڴ��ַ
        u32 m_btProtoNO : 5;    // [24:28]: Э���(Э������)            --->  0x0000
        u32 m_btRsv3V0  : 3;    // [29:31]: ����λ(���0)                        |   
		//bits[16:23]                                                            |
        u32 m_btDstAddr : 8;    // [16:23]: Ŀ��ڵ��ַ                --->  0x0001
        //bits[8:15]                                                             |  
        u32 m_btCmdID   : 8;    // [8:15]:  ����ID                      --->  0x0002
        //bits[0:7]                                                              |
        u32 m_btCntFlag : 1;    // [0]:     CNTλ(0: ���1֡, 1: ������1֡)      |
        u32 m_btRsv6V1  : 6;    // [1:6]:   ����λ(���1)               --->  0x0003  
        u32 m_btMsgDir  : 1;    // [7]:     ��Ϣ����(0: S->M, 1: M->S)           
#else

        /**********************************************
        * CPU   С��ģʽ:���ݵĸ�λ���������ڴ�ĸߵ�ַ��
        * ��ַ  0x0000 0x0001 0x0002 0x0003
        * ����  0x44   0x33   0x22   0x11
        * ʵ��ֵ:0x11223344
        **********************************************/
        //bits[0:7]                                                          �ڴ��ַ
        u32 m_btCntFlag : 1;    // [0]:     CNTλ(0: ���1֡, 1: ������1֡)
        u32 m_btRsv6V1  : 6;    // [1:6]:   ����λ(���1)              --->  0x0000
        u32 m_btMsgDir  : 1;    // [7]:     ��Ϣ����(0: S->M, 1: M->S)          |
        //bits[8:15]                                                            |
        u32 m_btCmdID   : 8;    // [8:15]:  ����ID                     --->  0x0001
		//bits[16:23]                                                           |
        u32 m_btDstAddr : 8;    // [16:23]: Ŀ��ڵ��ַ               --->  0x0002
        //bits[24:31]                                                           |
        u32 m_btProtoNO : 5;    // [24:28]: Э���(Э������)           --->  0x0003 
        u32 m_btRsv3V0  : 3;    // [29:31]: ����λ(���0)
#endif
    }m_btMsgID;                 // ��ϢID(λ���ʾ)
}CAN_MSG_ID_UN;

//  ����Э��Ŷ���.
#define PROTO_NO_UPPER_TO_GMC   0x10 // ��λ����GMC��ͨ��Э���.
#define PROTO_NO_CCU_TO_GMC     0x11 // �ϲ��������GMC��ͨ��Э���.


// ����Ӧ����
typedef enum tagAckResult
{
    ACK_OK = 0,         // Ӧ��ɹ�
    ACK_ERR_CMD_ID,     // ����ID����
    ACK_ERR_SIG_ID,     // �ź�ID����
    ACK_ERR_PARAM,      // ��������
}ACK_RESULT_E;

// ��������ID
typedef enum tagCmdID
{
    CMD_ID_AUTO_SEND      = 0x00,       // �����ϱ�

    CMD_ID_SET_ACCELERATE = 0x10,       // ������������
    CMD_ID_GET_ACCELERATE = 0x20,       // �������߲�ѯ
    
    CMD_ID_BAT_GET_FIXED  = 0x40,       // ������ȡ������Ϣ
    CMD_ID_SIN_GET_FIXED  = 0x41,       // ���źŻ�ȡ������Ϣ

    CMD_ID_BAT_GET_STATUS = 0x50,       // ������ȡ״̬��Ϣ
    CMD_ID_SIN_GET_STATUS = 0x51,       // ���źŻ�ȡ״̬��Ϣ
    
    CMD_ID_BAT_SET_PARAM  = 0x80,       // �������ò���
    CMD_ID_SIN_SET_PARAM  = 0x81,       // ���ź����ò���

    CMD_ID_BAT_GET_PARAM  = 0x90,       // ������ȡ����
    CMD_ID_SIN_GET_PARAM  = 0x91,       // ���źŻ�ȡ����
    CMD_ID_RANGE_GET_PARAM  = 0x91,     // �źŷ�Χ��ȡ����

    
    
}CMD_ID_E;

// ������Ϣ����
typedef enum tagMsgDir
{
    DIR_S2M = 0,                // �ӻ� -> ����
    DIR_M2S = 1,                // ���� -> �ӻ�
}MSG_DIR_E;

typedef enum tagSignalList
{
    SIG_ID_FM_VERSION  = 0x001, // ������Ӳ���汾.
    SIG_ID_BUILD_DATE  = 0x002, // ����ʱ��.

    
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

    // �������.
    SIG_ID_CTRL_MOTOR_1     = 0x160, // ������.
    SIG_ID_CTRL_MOTOR_2     = 0x161, // ������.

    // �������ߵĲ�������.

    SIG_ID_SET_ACC_CURVE    = 0x180, // 

    // J�������.
    SIG_ID_CTRL_JCMD_START  = 0x040,
    SIG_ID_CTRL_JCMD_1      = 0x040,
    SIG_ID_CTRL_JCMD_2      = 0x041,
    SIG_ID_CTRL_JCMD_3      = 0x042,
    //
    SIG_ID_CTRL_JCMD_MAX    = 0x0FF,
    
    
}SIG_ID_E;


extern NORTH_RX_MSG_QUEUE_ST g_stNorthRxMsgQueue;
extern NORTH_TX_MSG_QUEUE_ST g_stNorthTxMsgQueue;

// �Զ��ϱ����ݵ�����.
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
