//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            文件名称: KangWo.c              
 *
 *                摘要: 康沃变频器命令 
 *
 *            创建时间: 2006-09-20  
 *
 *            修改时间: 2006-09-20
 *            修改原因:
 *
 *            
 *                说明:
 *                      变频器应进行如下设定:
 *                      H-78: 本机地址设定为01
 *                      H-79:奇校验 - 2
 *                      H-80:波特率为4800 - 2
 *                      H-81:本变频器为从机 - 0
 *                      H-83:为1,RS-485通讯断线模式为维持现有状态
 *                      另外B-1(为7),B-3(为3或4)还要进行相应的设定
 *
 *                作者: 杨卫华
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
地址:0001H 
命令:
0001H：正转运行 
0002H：反转运行 
0003H：停机 
0004H：自由停车 
0005H：故障复位 
*/

/*
地址:0002H
设定范围(-10000～10000) 
注意：通讯设定值是相对的百分数
（-100.00～100.00%）。当作为频率源
设定时，相对的是最大频率的百分数；
当作为转矩给定时，相对的是两倍额定
转矩的百分数；当作为 PID 给定或反馈
时，相对的是物理量程的百分数。 
*/


/*先高后低*/

//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
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
		
    //写入频率
	buff[0] = 0x01;
	buff[1] = 0x06;  //写入单笔数据
	
	buff[2] = 0x00;  //地址
	buff[3] = 0x02;
	
	//buff[2] = 0x09;  //地址
	//buff[3] = 0x07;
	    
	rev_num *= 20;
	rev_num /= 3;
	
	        
	buff[4] = rev_num >> 8;
	buff[5] = rev_num & 0xFF;
	    
	tmp_crc = CRCCheck(buff, 6);
	    
	buff[6] = tmp_crc & 0xFF;   //低位在先
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
    buff[1] = 0x06;  //写入单笔数据
    
    buff[2] = 0x00;  //地址
    buff[3] = 0x01; 
    
    //buff[2] = 0x20;  //地址
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
    
    buff[6] = tmp_crc & 0xFF;   //低位在先
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
 *         函数名称: 
 *
 *         函数功能: 
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void KangWoStop(UINT16 rev_fwd)
{
    UINT8   i;
    UINT8   buff[20];
    UINT16  tmp_crc;
    
    buff[0] = 0x01;
    buff[1] = 0x06;  //写入单笔数据
    buff[2] = 0x00;  //地址
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
    
    buff[6] = tmp_crc & 0xFF;   //低位在先
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
 *         函数名称: 
 *
 *         函数功能: 
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
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
 *         函数名称: 
 *
 *         函数功能: 
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void  KangWoStop(UINT16 fr)
{
   
   //奇校验 
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
