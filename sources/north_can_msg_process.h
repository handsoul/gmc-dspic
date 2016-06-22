#ifndef __NORTH_CAN_MSG_PROCESS_H__
#define __NORTH_CAN_MSG_PROCESS_H__



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
#endif

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
	
	SID_ID_MISC_INFO   = 0x120,//�¶ȵ�������Ϣ.
}SIG_ID_E;


extern void NorthCanRxMsgProcess(void);
extern void NorthRxMsgHandler(CAN_MSG_ST * pstMsg);
#endif 
// end of file.
