//======================================================================================================
/******************************************************************************************************
 *                                                                                                     
 *            文件名称:              
 *
 *                摘要: 
 *
 *            创建时间: 2012-05-xx  
 *
 *            修改时间: 2012-05-xx
 *            修改原因:
 *
 *            
 *
 *                作者: 杨卫华
 *
 *******************************************************************************************************/
//======================================================================================================

#include "stm32f4xx.h"
#include <stdarg.h>
#include "ucos_ii.h"
#include "mystd.h"  
#include "m_config.h"
#include "SysAppConfig.h"
#include "UserLib.h"
#include "OS_Trace.h"


extern void  UsartPrintf(USART_TypeDef *UARTChnnl,char *str);
extern OS_EVENT    *PRINTF_SEM; 


//======================================================================================================
/*******************************************************************************************************
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
 *******************************************************************************************************/
void  OS_Trace(char *fmt, ...)
{
    ///INT8U  err;


//     va_list ap;
//     char string[256];    
//     
//     //OSSemPend(PRINTF_SEM, 100,&err);
//     
//     va_start(ap, fmt);
//     vsprintf(string, fmt, ap);
//     UsartPrintf(DEBUG_UART_CHNNL, string);
//     va_end(ap);
//     
//     //OSSemPost(PRINTF_SEM);


}



