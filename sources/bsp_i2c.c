#include "includes.h"


bool I2C0_StartDataTransmit(void)
{
}

bool I2C0_StopDataTransmit(void)
{
}

bool I2C0_WaitTransDone(void)
{
}

bool I2C0_WaitAck(void)
{
}



/***********************************************************
 * ��������: I2C0_WriteBytes
 * ��������: I2Cд����
 * �������: ucCtrlByte:    �����ֽ�
 *           usWriteAddr:   ��д��ַ
 *           ucAddrType:    ��д��ַ����(0: 8bits, 1: 16bits)
 *           pucSrcData:    ��дԴ����ָ��
 *           ulDataNum:     ��д���ݳ����ֽ���
 * �������: ��.
 * �� �� ֵ: �Ƿ�����ʱ. 0��ʾû�з�����ʱ. 1��ʾ��������ʱ.
 * ��    ע: I2Cд���ݵĹ���:
 *           1.��������;
 *           2.���Ϳ����ֽ�(����Ϊд, �ȴ���Ӧ, ��ͬ);
 *           3.���ʹ�д��ַ;
 *           4.���ʹ�д��������;
 *           5.ֹͣ����;
***********************************************************/
u8 I2C0_WriteBytes(u8 ucCtrlByte, u16 usWriteAddr, u8 ucAddrType, 
                         u8 *pucSrcData, u32 ulDataNum)
{
    u32 i;
    
    u32 ulTimeout = 0;    

    // 1.�������ݴ���
    I2C0_StartDataTransmit();

    // 2.���Ϳ����ֽ�
    I2C0_DATA = (ucCtrlByte & 0xFE);    // [0]: 0, д����
    ulTimeout += I2C0_WaitAck();
    
    // 3.���ʹ�д��ַ
    if (ucAddrType == I2C_ADDR_TYPE_16BITS)     // 16λ��ַģʽʱ�ȷ��͸�8λ
    {
        I2C0_DATA = (u8)(usWriteAddr >> 8);
        ulTimeout += I2C0_WaitAck();
    }
    I2C0_DATA = (u8)usWriteAddr;
    ulTimeout += I2C0_WaitAck();

    // 4.������������
    for (i = 0; i < ulDataNum; ++i)
    {
        I2C0_DATA = pucSrcData[i];
        ulTimeout += I2C0_WaitAck();
    }

    // 5.ֹͣ���ݴ���
    I2C0_StopDataTransmit();
    
    return (u8)(ulTimeout > 0);
}

/***********************************************************
 * ��������: I2C0_ReadBytes
 * ��������: I2C������
 * �������: ucCtrlByte:    �����ֽ�
 *           usReadAddr:    ������ַ
 *           ucAddrType:    ������ַ����(0: 8bits, 1: 16bits)
 *           pucDstData:    ���������ݷ��õ�Ŀ��ָ��
 *           ucDataNum:     ���������ݳ����ֽ���
 * �������: ��.
 * �� �� ֵ: ��.
 * ��    ע: I2C�����ݵĹ���:
 *           1.��������;
 *           2.���Ϳ����ֽ�(����Ϊд, �ȴ���Ӧ, ��ͬ);
 *           3.���ʹ�����ַ;  ---- ��һ�㲻Ҫ����, ��д������ַ��ȥ.
 *           4.�ظ�������ʼ����(��������������), ���Ϳ����ֽ�(����Ϊ��);
 *           5.����Ϊ����ģʽ, ��������(ǰN-1��������Ӧ��['0'], ���һ��������Ӧ��['1']);
 *           6.ֹͣ����;
***********************************************************/
u8 I2C0_ReadBytes(u8 ucCtrlByte, u16 usReadAddr, u8 ucAddrType, 
                        u8 *pucDstData, u32 ulDataNum)
{
    u32 i;
    u32 ulTimeout = 0;
   
    // 1.�������ݴ���
    I2C0_StartDataTransmit();
   
    // 2.���Ϳ����ֽ�
    I2C0_DATA = (ucCtrlByte & 0xFE);    // [0]: 0, д����
    ulTimeout += I2C0_WaitAck();
    
    // 3.���ʹ�����ַ
    if (ucAddrType == I2C_ADDR_TYPE_16BITS)     // 16λ��ַģʽʱ�ȷ��͸�8λ
    {
        I2C0_DATA = (u8)(usReadAddr >> 8);
        ulTimeout += I2C0_WaitAck();
    }
    I2C0_DATA = (u8)usReadAddr;
    ulTimeout += I2C0_WaitAck();

    // 4. �ظ�������ʼ����(��������������)
    I2C0_RPT_ST_AG;
    I2C0_DATA = (ucCtrlByte | 0x01); // ���Ϳ����ֽ�(����Ϊ��)
    ulTimeout += I2C0_WaitAck();
   
    // 5.����Ϊ����ģʽ, ��������
    I2C0_DIR_RX;
    // ��������ģʽ��, ��IBDR�Ĵ�������ʼ������(�ο������ֲ�), ʵ�����ݶ���
    i = I2C0_DATA;
    // ǰN-1���ֽ�, ����Ӧ��(I2C0_StartDataTransmit����ʹ��)
    for (i = 0; i < ulDataNum - 1; ++i)
    {
        ulTimeout += I2C0_WaitTransDone();
        // ȡ���Ĵ����е�����
        pucDstData[i] = I2C0_DATA;
    }
    // �������һ���ֽڲ�Ӧ��
    I2C0_ACK_DISABLE;
    ulTimeout += I2C0_WaitTransDone();
      
    // 5.ֹͣ���ݴ���
    I2C0_StopDataTransmit();
    // ȡ�����յ����һ���ֽ�
    pucDstData[i] = I2C0_DATA;
    
    return (u8)(ulTimeout > 0);
}


