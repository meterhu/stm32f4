/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------------------------------------------------------------------------------------------------
** Modified by:                Zhangrong
** Modified date:              2009-6-8
** Version:
** Descriptions:               ϵ��оƬӲ��I2C�������������Ҫ���ú�I2C���߽ӿ�(I2C���Ź��ܺ�
**                             I2C�жϣ�����ʹ��I2C��ģʽ)
*********************************************************************************************************/
#include "stm32f4xx.h"  
#include <stdio.h>   
#include "I2C.h"
#include "uart.h"

#define I2C_PageSize      8

/*******************************************************************************
*��������: IIC_Init
*������void
*����ֵ:void
*��������: ��ʼ��
*****************************************************************************/

void IIC_Init(void)
{	
  	GPIO_InitTypeDef  GPIO_InitStructure;//GPIO��ʼ���ṹ��
	I2C_InitTypeDef  I2C_InitStructure;

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//����ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7;//��ӦGPIO����
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//״̬Ϊ���
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//IO�ٶ�Ϊ100MHZ
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
  	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

	/*********** I2C periphral configuration **********/
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; // fixed
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;  // fixed
	I2C_InitStructure.I2C_OwnAddress1 = I2C1_ADDRESS7;  // user parameter
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; // fixed
	#ifdef SLAVE_10BIT_ADDRESS  
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;  // user define
	#else
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	#endif
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed; // user parameter
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
  			     
}

/*******************************************************************************
*��������: Test_I2C
*������void
*����ֵ:void
*��������: ��д����
****************************************************************************/
void Test_I2C(void)
{
    
    uint8_t writebuff[32];
	uint8_t readbuff[32];
    uint8_t i;
    char  str[100];
	   
    for (i = 0; i < 32; i++)
    {
        writebuff[i] = i;

    }

    
    I2C_EE_BufferWrite(writebuff, 0, 32); 
	I2C_EE_BufferRead(readbuff, 0, 32); 
      
    //����ͨ�����ڴ�ӡ����
	for(i=0;i<32;i++)
	{
	   if(readbuff[i]!=writebuff[i])
	   {
	   	  sprintf(str,"%d\r\n", readbuff[i]);
          UsartPrintf(USART1,str);
	   }
	}
	UsartPrintf(USART1,"OK"); 


}
/*******************************************************************************
*��������: Test_I2C
*������void
*����ֵ:void
*��������: ��������������д��I2C_EEPROM��
****************************************************************************/
void I2C_EE_BufferWrite(u8 *pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfpage = 0;
  u8 NumOfSigle = 0;
  u8 Addr = 0;
  u8 count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfpage =  NumByteToWrite / I2C_PageSize;
  NumOfSigle = NumByteToWrite % I2C_PageSize;
  
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfpage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSigle);
  	  I2C_EE_WaitRepromstandbystate();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
	    while(NumOfpage--)
		{
		  I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 	      
	   	  I2C_EE_WaitRepromstandbystate();
		  WriteAddr += I2C_PageSize;
		  pBuffer += I2C_PageSize;
		}
		if(NumOfSigle !=0)
		{
		   I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSigle); 	      
	   	   I2C_EE_WaitRepromstandbystate();
		}
       
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfpage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSigle);
      I2C_EE_WaitRepromstandbystate();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
	  NumByteToWrite = count;
	  NumOfpage =  NumByteToWrite % I2C_PageSize;
	  NumOfSigle = NumByteToWrite % I2C_PageSize;
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitRepromstandbystate();
		WriteAddr += count;
		pBuffer += count;
      }
	  while(NumOfpage--)
	  {
		  I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 	      
	   	  I2C_EE_WaitRepromstandbystate();
		  WriteAddr += I2C_PageSize;
		  pBuffer += I2C_PageSize;
	  }
	  if(NumOfSigle !=0)
	  {
		   I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSigle); 	      
	   	   I2C_EE_WaitRepromstandbystate();
	  }
    }
  }  

}
/*******************************************************************************
*��������: I2C_EE_PageWrite
*������void
*����ֵ:void
*��������: ��EEPROM��һ��ѭ����д����ֽ֣���һ��д����ֽ������ܳ���EEPROMҳ�Ĵ�С
****************************************************************************/
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
  while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
  /*Send START Condition*/
  I2C_GenerateSTART(I2C1, ENABLE);
  /*Test on EV5 and clear it*/	
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
  /*Send EEPROM address for write*/	
  I2C_Send7bitAddress(I2C1,I2C_Addr,I2C_Direction_Transmitter);
  /*Test on EV6 and clear it*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  /*Send EEPROM's internal address to write to*/
  I2C_SendData(I2C1,WriteAddr);
  /*Test on EV8 and clear it*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /*while there is data to be write*/
  while(NumByteToWrite--)
  {
    /*Send the current byte*/
  	I2C_SendData(I2C1,*pBuffer);

	pBuffer++;
  }	
  /*Test on EV8 and clear it*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));	
	
  I2C_GenerateSTOP(I2C1,ENABLE);
  
}
/*******************************************************************************
*��������: I2C_EE_WaitRepromstandbystate
*������void
*����ֵ:void
*��������: 
****************************************************************************/
void I2C_EE_WaitRepromstandbystate(void)
{
  vu16 SR1_Tmp = 0;

  do
  {
  	/*Send START Condition*/
    I2C_GenerateSTART(I2C1, ENABLE);
	/*read I2C1 SR1 register*/
	SR1_Tmp = I2C_ReadRegister(I2C1,I2C_Register_SR1);
	/*send EEPROM address for write*/
	I2C_Send7bitAddress(I2C1,I2C_Addr,I2C_Direction_Transmitter);

  }
  while(!(I2C_ReadRegister(I2C1,I2C_Register_SR1)&0x0002));
  /*clear AF flag*/
  I2C_ClearFlag(I2C1,I2C_FLAG_AF);
  I2C_GenerateSTOP(I2C1,ENABLE);

}
/*******************************************************************************
*��������: I2C_EE_BufferRead
*������void
*����ֵ:void
*��������: ��EEPROM��һ��������ֽ�
****************************************************************************/
void I2C_EE_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead)
{
  while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
  /*Send START Condition*/
  I2C_GenerateSTART(I2C1, ENABLE);
  /*Test on EV5 and clear it*/	
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
  /*Send EEPROM address for write*/	
  I2C_Send7bitAddress(I2C1,I2C_Addr,I2C_Direction_Transmitter);
  /*Test on EV6 and clear it*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /*clear EV6 by setting again the PE bit*/
  I2C_Cmd(I2C1,ENABLE);
  /*Send EEPROM's internal address to write to*/
  I2C_SendData(I2C1,ReadAddr);
  /*Test on EV8 and clear it*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /*Send START Condition a second time*/
  I2C_GenerateSTART(I2C1, ENABLE);

  /*Test on EV5 and clear it*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));	
  /*Send EEPROM address for read*/	
  I2C_Send7bitAddress(I2C1,I2C_Addr,I2C_Direction_Receiver);
  /*Test on EV6 and clear it*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));	
	 

  /*while there is data to be write*/
  while(NumByteToRead)
  {
    if(NumByteToRead == 1)
	{
	  I2C_AcknowledgeConfig(I2C1,DISABLE);
	  I2C_GenerateSTOP(I2C1,ENABLE);
	}
    /*Test on EV7 and clear it*/
	if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
	  /*Send the current byte*/
  	  

	  *pBuffer =	I2C_ReceiveData(I2C1);
	   pBuffer ++;
	   NumByteToRead--;
	}
  }	
  I2C_AcknowledgeConfig(I2C1,ENABLE);
  
  
}


