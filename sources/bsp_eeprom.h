#ifndef __BSP_EEPROM_H__
#define __BSP_EEPROM_H__

#include "includes.h"

#define EEPROM_ADDR         0           // EEPROM������ַ.�鿴ԭ��ͼ����.
#define EEPROM_SIZE         0x20000     // EEPROM��С
#define EEPROM_PAGE_SIZE    128         // EEPROMҳ��С

void EE_Init(void);
void EEPROM_ReadBytes(u32 ulMemAddr, u8 *pucDstData, u32 ulDataNum);

void EEPROM_WriteBytes(u32 ulMemAddr, u8 *pucSrcData, u32 ulDataNum);

void EE_SEQU_Write(u32 addr,unsigned char length,unsigned char *dptr);
void EE_SEQU_Read(u32 addr,unsigned char length,unsigned char *dptr);
unsigned char EE_Read_Byte(u32 addr);
void EE_Write_Byte( u32 addr,unsigned char WRDATA );
#endif // 
// end of file

