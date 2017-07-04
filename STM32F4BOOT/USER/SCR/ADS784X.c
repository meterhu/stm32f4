//===============================================================================================
/************************************************************************************************
*                                                                                                     
*            �ļ�����:              
*
*                ժҪ: 
*                   ��������	��ӦIO�ܽ�	    ����/����
*                   SPI1_NSS	    PA11	    ����
*                   SPI1_SCK	    PA10	    ����
*                   SPI1_MISO	    PA9         ����
*                   SPI1_MOSI	    PA12	    ����
*
*            ����ʱ��: 2009-07-02  
*
*            �޸�ʱ��: 
*            �޸�ԭ��:
*
*            
*
*                ����: ������
*
*************************************************************************************************/
//===============================================================================================
#include "stm32f4xx.h"
#include "stm32f4_std_libs.h"
#include "mystd.h"
#include "m_config.h"
#include "InitHW.h"
#include "UART.h"
#include "UserLib.h"
#include "ADS784X.h"



/*************************************************************************************************
*
*         ��������: 
*
*         ��������: ��ʼ��AD���GPIO�ܽ�
*
*         ��ڲ���: 
*
*         ���ڲ���: ��
*
*             ˵��:
*
*************************************************************************************************/
void InitADS784xPin(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
/**************************************************
*   SPI1_NSS	    PA11	    ����
*   SPI1_SCK	    PA10	    ����
*   SPI1_MISO	    PA9         ����
*   SPI1_MOSI	    PA12	    ����   
*********************************************/    
    
    /* SPI1_NSS \SPI1_SCK\SPI1_MOSI*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* SPI1_MISO*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
}


/*************************************************************************************************
*
*         ��������: ADS784xAD
*
*         ��������: 
*
*         ��ڲ���: 
*
*         ���ڲ���: ��
*
*             ˵��:
*
*************************************************************************************************/
void  TestADIO(void)
{
    while(1)
    {
        AD_DO_LO(); 
        AD_DO_HI();  
        
        AD_CS_LO();   
        AD_CS_HI();   
        
        AD_CLK_LO();   
        AD_CLK_HI();  
        
    }

//  AD_DI()       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) 
  
}



/*************************************************************************************************
*
*         ��������: ADS784xAD
*
*         ��������: 
*
*         ��ڲ���: 
*
*         ���ڲ���: ��
*
*             ˵��:
*
*************************************************************************************************/
u16  ADS784xAD(u8  cmd)
{
    u8  i;
    u16 data;
   __IO u32 tmp;
    
    //TestADIO();
     
    AD_CS_LO();
    
    
    SPI_DLY();/////
    for (i = 0; i < 8; i++)
    {
        if (cmd & (0x80 >> i))
        {
            AD_DO_HI();
            
        }
        else
        {
            AD_DO_LO();
        }
        
        ////SPI_DLY();  //////////////////////8.23
        AD_CLK_LO();
        SPI_DLY();
        
        AD_CLK_HI();
        SPI_DLY();
        
    }
    
    Delay(80000);
    
    
    //�����Ƕ�����
    
    data = 0;
    for (i = 0; i < 12; i++)
    {  
        
        AD_CLK_HI();
        
        SPI_DLY();
        
        AD_CLK_LO();
        
        
        SPI_DLY();
        
        tmp = AD_DI();
        
        if (AD_DI())
        {
            data++;
            
        }
        
        data <<= 1;
        
        SPI_DLY();  
        
        //9��ʱ�Ӻ��ж�������    
        
    }
    
    AD_CS_HI();  
    
    return data;
    
}
