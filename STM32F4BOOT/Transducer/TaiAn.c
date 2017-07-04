//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            �ļ�����:              
 *
 *                ժҪ: 
 *
 *            ����ʱ��: 2006-09-20  
 *
 *            �޸�ʱ��: 2006-09-20
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
#include "MyLib.h"
#include "UART.h"
#include "transducer.h"


//================================================================================================
/*************************************************************************************************
 *
 *         ��������: 
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
UINT16 CRCCheck(UINT8 *data, UINT8 len)
{
    UINT8   j;
    UINT16  reg_crc;
    
    reg_crc = 0xFFFF;
    
    while (len--)
    {
        reg_crc ^= *data++;
        
        for (j = 0; j < 8; j++)
        {
            if (reg_crc & 0x01)
            {
                reg_crc = (reg_crc >> 1) ^ 0xA001;
                           
            }
            else
            {
                reg_crc >>= 1;
            
            }
        
        }
    
    
    }

    return reg_crc;


}




//================================================================================================
/*************************************************************************************************
 *
 *         ��������: 
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
void TaiAnRun(UINT16 rev_fwd, UINT16 rev_num)
{

    UINT8   i;
    UINT8   buff[20];
    UINT16  tmp_crc;
  
    Wait(60000); 
    Wait(60000);
	Wait(60000);
	Wait(60000);  
	Wait(60000);    
	
	//////OSTimeDly(100);
		
    //д��Ƶ��
	buff[0] = 0x01;
	buff[1] = 0x06;  //д�뵥������
	
	buff[2] = 0x01;  //��ַ
	buff[3] = 0x02;
	
	//buff[2] = 0x09;  //��ַ
	//buff[3] = 0x07;
	    
	rev_num *= 10;
	rev_num /= 3;
	        
	buff[4] = rev_num >> 8;
	buff[5] = rev_num & 0xFF;
	    
	tmp_crc = CRCCheck(buff, 6);
	    
	buff[6] = tmp_crc & 0xFF;   //��λ����
	buff[7] = tmp_crc >> 8;
	       
	for (i = 0; i < 8; i++)
	{
	    UARTSendByte(TRANSD_CHNNL, buff[i]);
	    Wait(10);
	    
	}   
	     
	
    Wait(60000); 
    Wait(60000);
	Wait(60000);
	Wait(60000);  
	Wait(60000); 
	Wait(60000); 
    Wait(60000); 
  	  
    buff[0] = 0x01;
    buff[1] = 0x06;  //д�뵥������
    
    buff[2] = 0x01;  //��ַ
    buff[3] = 0x01; 
    
    //buff[2] = 0x20;  //��ַ
    //buff[3] = 0x00;  
    
    buff[4] = 0;
       
    if (rev_fwd == FWD_RUN)
    {   
        buff[5] = 0x01;
    }
    else if (rev_fwd == REV_RUN)
    {
        buff[5] = 0x03;
    
    }
    
    tmp_crc = CRCCheck(buff, 6);
    
    buff[6] = tmp_crc & 0xFF;   //��λ����
    buff[7] = tmp_crc >> 8;
    
    for (i = 0; i < 8; i++)
    {
        UARTSendByte(TRANSD_CHNNL, buff[i]);
        Wait(10);
    
    }   
   
    
          
}

//================================================================================================
/*************************************************************************************************
 *
 *         ��������: 
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
void TaiAnStop(UINT16 rev_fwd)
{
    UINT8   i;
    UINT8   buff[20];
    UINT16  tmp_crc;
    
    buff[0] = 0x01;
    buff[1] = 0x06;  //д�뵥������
    buff[2] = 0x01;  //��ַ
    buff[3] = 0x01;
    
    buff[4] = 0x00;
    
    if (rev_fwd == FWD_STOP)
    {   
        buff[5] = 0x0;
    }
    else if (rev_fwd == REV_STOP)
    {
        buff[5] = 0x02;
    
    }
    
    tmp_crc = CRCCheck(buff, 6);
    
    buff[6] = tmp_crc & 0xFF;   //��λ����
    buff[7] = tmp_crc >> 8;
    
    
    for (i = 0; i < 8; i++)
    {
        UARTSendByte(TRANSD_CHNNL, buff[i]);
        Wait(10);
    
    }    
   
  
} 

