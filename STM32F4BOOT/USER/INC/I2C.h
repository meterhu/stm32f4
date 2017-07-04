#ifndef __I2C_H
#define __I2C_H

#ifdef I2C_GLOBAL
#define I2C_EXT
#else
#define I2C_EXT extern
#endif
/**************************************************************************************************/
#define I2C_Addr		0xA0
#define I2C_Speed              200000
#define I2C1_ADDRESS7    		0x01

/**************************************************************************************************/

//unsigned char I2C_Read(I2C_TypeDef *I2Cx,unsigned char I2C_Addr,unsigned char addr,unsigned char *buf,unsigned char num);
//
//unsigned char I2C_Write(I2C_TypeDef *I2Cx,unsigned char I2C_Addr,unsigned char addr,unsigned char *buf,unsigned char num);
//void I2C1_EV_IRQHandler(void);

void I2C_EE_BufferWrite(u8 *pbuffer, u8 WriteAddr, u16 NumByteToWrite);
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_WaitRepromstandbystate(void);
void I2C_EE_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);

#endif

