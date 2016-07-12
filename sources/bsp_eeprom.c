#include "includes.h"

void EE_Init(void)
{
	I2C2_Init(); 	// EEPROM ʹ�õ���I2C2.
	DISABLE_EE_WP;	// disable write protection function.
}

void EE_Write_Byte( u32 addr,unsigned char WRDATA )   //д 24C64 1�ֽ�
{
	unsigned char AddrH = 0;         // �ߵ�ַ
	unsigned char AddrL = 0;         // �͵�ַ
    unsigned char ucCtrlByte = 0;
    // I2C_EEPROM�����ֽڽṹ:
    // [7] [6] [5] [4] [3] [2] [1] [0]
    //  1   0   1   0  A2  A1  A0  R/W_
    ucCtrlByte = ((EEPROM_ADDR << 1) & 0x0E);
    // �ߵ�ַ  
    if (addr > 65535)
    {
        ucCtrlByte |= 0x08;
    }      
    ucCtrlByte |= 0xA0;     // ������


	AddrL = (unsigned char)(addr);   // ��Щת��
	AddrH = (unsigned char)((addr >>8)&0xFF);// 2M. = 
	
	I2C2_Idle();                     // ���߿��У�
  	I2C2_Start();                    // I2C2 ����
	I2C2_Done();                     // ���� MI2C2IF
				
	I2C2_Write(ucCtrlByte);                // Write Control+Write
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	I2C2_Write(AddrH);               // д EEPROM �ߵ�ַ
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	I2C2_Write(AddrL);               // д EEPROM �͵�ַ
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	I2C2_Write(WRDATA);              // Write Data to EEPROM
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	I2C2_Stop();                     // I2C����ֹͣ
	I2C2_Done();                     // ���� MI2C2IF
}

//-------------------------------------------------------------------------------//
// I2C2 ����д����ֽ�
//-------------------------------------------------------------------------------//
void EE_SEQU_Write(u32 addr,unsigned char length,unsigned char *dptr)
{	
	unsigned char AddrH = 0;         // �ߵ�ַ
	unsigned char AddrL = 0;         // �͵�ַ
    unsigned char ucCtrlByte = 0;
    
    ucCtrlByte = ((EEPROM_ADDR << 1) & 0x0E);
    // �ߵ�ַ  
    if (addr > 65535)
    {
        ucCtrlByte |= 0x08;
    }      
    
    ucCtrlByte |= 0xA0;     // ������
    

	AddrL = (unsigned char)(addr);   // ��Щת��
	AddrH = (unsigned char)((addr >>8)&0x1F);//8K
	
	I2C2_Idle();                     // ���߿��У�
  	I2C2_Start();                    // I2C2 ����
	I2C2_Done();                     // ���� MI2C2IF

    
	I2C2_Write(ucCtrlByte);          // Write Control+Write
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	I2C2_Write(AddrH);               // д EEPROM �ߵ�ַ
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF
	
	I2C2_Write(AddrL);               // д EEPROM �͵�ַ
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	while (length != 0)              // Check write completed ?
	{
		I2C2_Write(*dptr);           // Write data to EEPROM
		while(I2C2STATbits.ACKSTAT); // wait Acknowledge from EEPROM
		I2C2_Done();	             // ���� MI2C2IF
		dptr++;                      // Point to next byte
		length--;
	}
	
	I2C2_Stop();                     // I2C����ֹͣ
	I2C2_Done();                     // ���� MI2C2IF
}
//-------------------------------------------------------------------------------//
// I2C2 ���� "��" 1���ֽ�
//-------------------------------------------------------------------------------//
unsigned char EE_Read_Byte(u32 addr)
{
	unsigned char RD_DATA;           // ��ʱ����
	unsigned char AddrH = 0;         // �ߵ�ַ
	unsigned char AddrL = 0;         // �͵�ַ
    unsigned char ucCtrlByte = 0;
    // I2C_EEPROM�����ֽڽṹ:
    // [7] [6] [5] [4] [3] [2] [1] [0]
    //  1   0   1   0  A2  A1  A0  R/W_
    ucCtrlByte = ((EEPROM_ADDR << 1) & 0x0E);
    // �ߵ�ַ  
    if (addr > 65535)
    {
        ucCtrlByte |= 0x08;
    }      
    ucCtrlByte |= 0xA0;     // ������


	AddrL = (unsigned char)(addr);   // ��Щת��
	AddrH = (unsigned char)((addr >>8)&0xFF);
	
	I2C2_Idle();                     // ���߿��У�
  	I2C2_Start();                    // I2C2 ����
	I2C2_Done();                     // ���� MI2C2IF
				
	I2C2_Write(ucCtrlByte);                // Write Control+Write

	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	I2C2_Write(AddrH);               // д EEPROM �ߵ�ַ
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	I2C2_Write(AddrL);               // д EEPROM �͵�ַ
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

   	I2C2_Restart();                  // initiate Restart condition
	I2C2_Done();                     // ���� MI2C2IF

	I2C2_Write(ucCtrlByte | 0x01);   // Write Control+Read
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	RD_DATA = I2C2_Read();           // ��ȡ1�ֽ�����
	I2C2_Done();                     // ���� MI2C2IF

	I2C2_NotAck();                   // ��Ӧ��  NACK
	I2C2_Done();	                 // ���� MI2C2IF
                     
	I2C2_Stop();                     // I2C����ֹͣ
	I2C2_Done();                     // ���� MI2C2IF

	return(RD_DATA);                 // ���ض�ȡ����
}
//-------------------------------------------------------------------------------//
// I2C2 ���� "��" ����ֽ�
//-------------------------------------------------------------------------------//
void EE_SEQU_Read(u32 addr,unsigned char length,unsigned char *dptr)
{
	unsigned char AddrH = 0;         // �ߵ�ַ
	unsigned char AddrL = 0;         // �͵�ַ
    unsigned char ucCtrlByte;
    // I2C_EEPROM�����ֽڽṹ:
    // [7] [6] [5] [4] [3] [2] [1] [0]
    //  1   0   1   0  A2  A1  A0  R/W_
    ucCtrlByte = ((EEPROM_ADDR << 1) & 0x0E);
    // �ߵ�ַ  
    if (addr > 65535)
    {
        ucCtrlByte |= 0x08;
    }      
    ucCtrlByte |= 0xA0;     // ������


	AddrL = (unsigned char)(addr);   // ��Щת��
	AddrH = (unsigned char)((addr >>8)&0xFF);// 2M. = 
	
	I2C2_Idle();                     // ���߿��У�
  	I2C2_Start();                    // I2C2 ����
	I2C2_Done();                     // ���� MI2C2IF
				
	I2C2_Write(ucCtrlByte);                // Write Control+Write

	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	I2C2_Write(AddrH);               // д EEPROM �ߵ�ַ
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF
	
	I2C2_Write(AddrL);               // д EEPROM �͵�ַ
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

   	I2C2_Restart();                  // initiate Restart condition
	I2C2_Done();                     // ���� MI2C2IF

    // ���ݷ���Ϊ��.
	I2C2_Write(ucCtrlByte|0x01);                // Write Control+Read
	while( I2C2STATbits.ACKSTAT );   // �ȴ� ACK
	I2C2_Done();                     // ���� MI2C2IF

	while (length!=0)
	{
		*dptr = I2C2_Read();         // ������ȡ���뻺����
		I2C2_Done();	
		dptr++;	
		length--;
				
		if (length == 0) I2C2_NotAck();   // ��Ӧ�� NACK
		else             I2C2_Ack();      // Ӧ��   ACK
		
		I2C2_Done();
	}

	I2C2_Stop();                     // I2C����ֹͣ
	I2C2_Done();                     // ���� MI2C2IF
}


/***********************************************************
 * ��������: EEPROM_WriteBytes
 * ��������: ��EEPROM��ֱ��д��һ���ֽ���
 * �������: ulMemAddr:     EEPROM�����ڲ���ַ
 *           pucSrcData:     Դ���ݵ�ַ(Ϊ���ݲ�ͬ��������, ʹ��void *)
 *           ulDataNum:     ��д���ݳ����ֽ���
 * �������: ��.
 * �� �� ֵ: ��.
 * ��    ע: ���������ֱ�Ӷ�EEPROM��д����, ע��:
 *           ����EEPROM���ڲ�д�߼���Ҫ10ms�ӳ�, ��˽���ʹ�ö���д.
 *           ���������ڳ�ʼ�����ж�EEPROM�û�����Ҫ�������ؽ������.
***********************************************************/
// #pragma MESSAGE DISABLE C12056      // ��ֹ"���ƴ�����Ҫ�ϲ�"�ĸ澯(ҳд����)
void EEPROM_WriteBytes(u32 ulMemAddr, u8 *pucSrcData, u32 ulDataNum)
{
    u16 usWriteNum;         // ÿ��д���ֽ���
    
    if ((ulMemAddr + ulDataNum >= EEPROM_SIZE)
        || (ulDataNum == 0))
    {
        return ;
    }

    // ����ҳд��ַ�����, ֱ�Ӳ���ҳд
    if (ulMemAddr / EEPROM_PAGE_SIZE == (ulMemAddr + ulDataNum - 1) / EEPROM_PAGE_SIZE)
    {
        EE_SEQU_Write(ulMemAddr, (u16)ulDataNum, pucSrcData);
        DelayMS(5);
        ClearWdt();
    }
    // ������ҳд�����, �ֳɶ�ҳ����ҳд
    else
    {
        // д��1ҳ
        usWriteNum = (u16)(EEPROM_PAGE_SIZE - ulMemAddr % EEPROM_PAGE_SIZE);
        EE_SEQU_Write(ulMemAddr,usWriteNum ,pucSrcData);
        DelayMS(5);
        ClearWdt();
        ulMemAddr  += usWriteNum;
        pucSrcData += usWriteNum;
        ulDataNum  -= usWriteNum;
            
        // д�м�ҳ
        while (ulDataNum >= EEPROM_PAGE_SIZE)
        {
            EE_SEQU_Write(ulMemAddr,EEPROM_PAGE_SIZE,pucSrcData);
            DelayMS(5);
            ClearWdt();
            ulMemAddr  += EEPROM_PAGE_SIZE;
            pucSrcData += EEPROM_PAGE_SIZE;
            ulDataNum  -= EEPROM_PAGE_SIZE;
        }

        // д���1ҳ
        if (ulDataNum > 0)
        {
            EE_SEQU_Write(ulMemAddr,(u16)ulDataNum,pucSrcData);
            DelayMS(5);
            ClearWdt();
        }                  
    }        
}

// void EE_SEQU_Read(u32 addr,unsigned char length,unsigned char *dptr)
void EEPROM_ReadBytes(u32 ulMemAddr, u8 *pucDstData, u32 ulDataNum)
{
    EE_SEQU_Read(ulMemAddr,ulDataNum,pucDstData);
}