//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            �ļ�����:              
 *
 *                ժҪ: 
 *
 *            ����ʱ��: 2012-05-xx  
 *
 *            �޸�ʱ��: 2012-05-xx
 *            �޸�ԭ��:
 *
 *            
 *
 *                ����: ������
 *
 *************************************************************************************************/
 //===============================================================================================
#include <stdarg.h>
#include "stm32f4xx.h"
#include "mystd.h"
#include "m_config.h"


#ifdef  __MY_DEBUG

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


#ifndef __UART_INTERFACE_H
#define __UART_INTEFFACE_H


int fputc(int ch, FILE *f);     //fputc�ض���
int fgetc(FILE *f); //fgetc�ض���


#endif


//======================================================================================================
/*******************************************************************************************************
 *
 *         ��������: int fputc(int ch, FILE *f)
 *
 *         ��������: Retargets the C library printf function to the USART.printf�ض���
 *
 *         ��ڲ���: 
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
 *
 *******************************************************************************************************/
int fputc(int ch, FILE *f)
{
          
    //Delay(1000);
    while(USART_GetFlagStatus(DEBUG_UART_CHNNL, USART_FLAG_TXE) == RESET)
    {
         ;
      
    }       

    USART_SendData(DEBUG_UART_CHNNL, ch);
   

   return ch;
}



/*******************************************************************************
* Function Name   : int fputc(int ch, FILE *f)
* Description     : Retargets the C library printf function to the USART.printf�ض���
* Input           : None
* Output          : None
* Return          : None
*******************************************************************************/
//int fputc(int ch, FILE *f) {
//  if (DEMCR & TRCENA) {
//    while (ITM_Port32(0) == 0);
//    ITM_Port8(0) = ch;
//  }
//  return(ch);
//}



//int fputc(int ch, FILE *f) 
//{
//    ITM_SendChar (ch);
//    return (ch);
//}



/*******************************************************************************
* Function Name   : int fgetc(FILE *f)
* Description     : Retargets the C library printf function to the USART.fgetc�ض���
* Input           : None
* Output          : None
* Return          : ��ȡ�����ַ�
*******************************************************************************/
int fgetc(FILE *f)
{
    return (0);
}

#endif
