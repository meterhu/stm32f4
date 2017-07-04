//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            �ļ�����: KangWo.c              
 *
 *                ժҪ: ���ֱ�Ƶ������ 
 *
 *            ����ʱ��: 2006-09-20  
 *
 *            �޸�ʱ��: 2006-09-20
 *            �޸�ԭ��:
 *
 *            
 *                ˵��:
 *                      ��Ƶ��Ӧ���������趨:
 *                      H-78: ������ַ�趨Ϊ01
 *                      H-79:��У�� - 2
 *                      H-80:������Ϊ4800 - 2
 *                      H-81:����Ƶ��Ϊ�ӻ� - 0
 *                      H-83:Ϊ1,RS-485ͨѶ����ģʽΪά������״̬
 *                      ����B-1(Ϊ7),B-3(Ϊ3��4)��Ҫ������Ӧ���趨
 *
 *                ����: ������
 *
 *************************************************************************************************/
//===============================================================================================
#include "stm32f4xx.h" 
#include "stm32f4_std_libs.h"
#include "MyLib.h"
#include "44B0X.h"
#include "UART.h"
#include "transducer.h"

#define KANGWO
#define AD3000

#ifdef AD3000


/*
��ַ:0001H 
����:
0001H����ת���� 
0002H����ת���� 
0003H��ͣ�� 
0004H������ͣ�� 
0005H�����ϸ�λ 
*/

/*
��ַ:0002H
�趨��Χ(-10000��10000) 
ע�⣺ͨѶ�趨ֵ����Եİٷ���
��-100.00��100.00%��������ΪƵ��Դ
�趨ʱ����Ե������Ƶ�ʵİٷ�����
����Ϊת�ظ���ʱ����Ե��������
ת�صİٷ���������Ϊ PID ��������
ʱ����Ե����������̵İٷ����� 
*/


/*�ȸߺ��*/

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
void KangWoRun(UINT16 rev_fwd, UINT16 rev_num)
{

    UINT8   i;
    UINT8   buff[20];
    UINT16  tmp_crc;
  
    Wait(60000); 
    Wait(60000);
	Wait(60000);
	Wait(60000);  
	Wait(60000);  
	
	
	Wait(60000); 
    Wait(60000);
	Wait(60000);
	Wait(60000);  
	Wait(60000);   
	
	//////OSTimeDly(100);
		
    //д��Ƶ��
	buff[0] = 0x01;
	buff[1] = 0x06;  //д�뵥������
	
	buff[2] = 0x00;  //��ַ
	buff[3] = 0x02;
	
	//buff[2] = 0x09;  //��ַ
	//buff[3] = 0x07;
	    
	rev_num *= 20;
	rev_num /= 3;
	
	        
	buff[4] = rev_num >> 8;
	buff[5] = rev_num & 0xFF;
	    
	tmp_crc = CRCCheck(buff, 6);
	    
	buff[6] = tmp_crc & 0xFF;   //��λ����
	buff[7] = tmp_crc >> 8;
	       
	for (i = 0; i < 8; i++)
	{
	    UARTSendByte(TRANSD_CHNNL, buff[i]);
	    Wait(100);
	    
	}   
	      
	
    Wait(60000); 
    Wait(60000);
	Wait(60000);
	Wait(60000);  
	Wait(60000); 
	Wait(60000); 
    Wait(60000); 
    
    
    Wait(60000); 
    Wait(60000);
	Wait(60000);
	Wait(60000);  
	Wait(60000); 
	Wait(60000); 
    Wait(60000); 
  	  
    buff[0] = 0x01;
    buff[1] = 0x06;  //д�뵥������
    
    buff[2] = 0x00;  //��ַ
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
        buff[5] = 0x02;
    
    }
    
    tmp_crc = CRCCheck(buff, 6);
    
    buff[6] = tmp_crc & 0xFF;   //��λ����
    buff[7] = tmp_crc >> 8;
    
    for (i = 0; i < 8; i++)
    {
        UARTSendByte(TRANSD_CHNNL, buff[i]);
        Wait(100);
    
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
void KangWoStop(UINT16 rev_fwd)
{
    UINT8   i;
    UINT8   buff[20];
    UINT16  tmp_crc;
    
    buff[0] = 0x01;
    buff[1] = 0x06;  //д�뵥������
    buff[2] = 0x00;  //��ַ
    buff[3] = 0x01;
    
    buff[4] = 0x00;
    
    buff[5] = 0x03;
    
    //if (rev_fwd == FWD_STOP)
    //{   
    //    buff[5] = 0x0;
    //}
    //else if (rev_fwd == REV_STOP)
    //{
    //    buff[5] = 0x02;
    
    //}
    
    tmp_crc = CRCCheck(buff, 6);
    
    buff[6] = tmp_crc & 0xFF;   //��λ����
    buff[7] = tmp_crc >> 8;
    
    
    for (i = 0; i < 8; i++)
    {
        UARTSendByte(TRANSD_CHNNL, buff[i]);
        Wait(100);
    
    }    
   
  
} 



#else

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
void KangWoRun(UINT16 rev_fwd, UINT16 rev_num)
{
   UINT8 check_sum;
   UINT8 buff[11];  
   UINT16 i;
   UINT16 tmp;
    
   Wait(60000); 
   Wait(60000);
   Wait(60000);
   Wait(60000);  
   Wait(60000); 
	                        
   check_sum = 0;     
   
   buff[0] = 0x5A;
   buff[1] = 0x01;
   buff[2] = 0;
   buff[3] = 0;
   buff[4] = 0;
   buff[5] = 0;
   
   if (rev_fwd == FWD_RUN)
   { 
      buff[6] = 0x12;
   }
   else if (rev_fwd == REV_RUN)
   {
      buff[6] = 0x14;
   
   } 
   
   buff[7] = 0;
   
   tmp =(rev_num * 10) / 3; 
   
   buff[8] = tmp & 0xFF;
   buff[9] = (tmp >> 8) & 0xFF;
   
   ENA_RS485_OUT();
    
   for (i = 0; i < 10; i++)
   {
       UARTSendByte(TRANSD_CHNNL, buff[i]);
       Wait(10);
       check_sum += buff[i];
   
   } 
   
   check_sum %= 256;
   
   UARTSendByte(TRANSD_CHNNL, check_sum); 
   
 

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
void  KangWoStop(UINT16 fr)
{
   
   //��У�� 
   UINT8 check_sum; 
   UINT16 i;
   UINT8 buff[11];  
   
	Wait(60000); 
	Wait(60000);
	Wait(60000);
	Wait(60000);  
	Wait(60000); 

                           
   check_sum = 0;     
   
   buff[0] = 0x5A;
   buff[1] = 0x01;
   buff[2] = 0;
   buff[3] = 0;
   buff[4] = 0;
   buff[5] = 0;
   
   buff[6] = 0x10;
   buff[7] = 0x1;
   
   buff[8] = 0;
   buff[9] = 0;
   
   ENA_RS485_OUT();
    
   for (i = 0; i < 10; i++)
   {
       UARTSendByte(TRANSD_CHNNL, buff[i]);
       Wait(10);
       check_sum += buff[i];
   
   } 
   
   check_sum %= 256;
   
   UARTSendByte(TRANSD_CHNNL, check_sum); 
   

}

#endif