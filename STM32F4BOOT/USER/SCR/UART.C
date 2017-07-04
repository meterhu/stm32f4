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
#include "stm32f4_std_libs.h"
//#include "ucos_ii.h"
#include "m_config.h"
#include "main.h"
#include "UART.h"
#include "Queue.h"

QUEUE    UART1RQBuff;
QUEUE    *pUART1RQBuff; 

QUEUE    UART2RQBuff;
QUEUE    *pUART2RQBuff;

#if (COMM_UART == 0) 
    uint8_t  UART1RecvBuff[UART1_RX_BUFF_SIZE];
#elif (COMM_UART == 1)  
    uint8_t  UART2RecvBuff[UART2_RX_BUFF_SIZE];
#endif
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
void InitUARTQBuff(void)
{
#if (COMM_UART == 0) 
    pUART1RQBuff = &UART1RQBuff;
    
    pUART1RQBuff->qb      = UART1RecvBuff;
    pUART1RQBuff->size    = UART1_RX_BUFF_SIZE;
    pUART1RQBuff->rear    = 0;
    pUART1RQBuff->front   = UART1RQBuff.rear;
    pUART1RQBuff->state   = Q_EMPTY;
    pUART1RQBuff->r_new   = 0; 
#elif (COMM_UART == 1) 
    pUART2RQBuff = &UART2RQBuff;
    
    pUART2RQBuff->qb      = UART2RecvBuff;
    pUART2RQBuff->size    = UART2_RX_BUFF_SIZE;
    pUART2RQBuff->rear    = 0;
    pUART2RQBuff->front   = UART2RQBuff.rear;
    pUART2RQBuff->state   = Q_EMPTY;
    pUART2RQBuff->r_new   = 0;
#endif
    

    
    
}
 
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
void  InitUART(USART_TypeDef *UARTChnnl, uint32_t baud)
{
    
    /* USART1 configuration ------------------------------------------------------*/
    /* USART1 configured as follow:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled
    */
    
	NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;   
    USART_InitTypeDef USART_InitStructure;

    //时钟使能要在配置UART口之前
    if (UARTChnnl == USART1)
    {
        ///RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

        /* Configure USART2 Tx (PA.9) as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;//GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
        
        /* Configure USART2 Rx (PA.10) as input floating */
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			
        //Enable  Interrupt
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART1_IRQ_PRIO;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);

        

    }
    else if (UARTChnnl == USART2)
    {
        /* Configure USART2 Tx (PA.2) as alternate function push-pull */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//         GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//         GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;//GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
        
        /* Configure USART2 Rx (PA.3) as input floating */
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
        /* Enable the USART2 Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART2_IRQ_PRIO;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);

        
    }
    else if (UARTChnnl == USART3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    }

       
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
   /// USART_DeInit(UARTChnnl);
    
    /* Configure the USART1 */
    USART_Init(UARTChnnl, &USART_InitStructure);
    
    /* Enable USART1 */
    
    USART_Cmd(UARTChnnl, ENABLE);
    USART_ClearFlag(UARTChnnl,USART_FLAG_TC);
    USART_ITConfig(UARTChnnl, USART_IT_RXNE ,ENABLE);
		

}

 
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
void  UsartPrintf(USART_TypeDef *UARTChnnl,char *str)
{
    while (*str != '\0')
    {
        //Delay(1000);
        while(USART_GetFlagStatus(UARTChnnl, USART_FLAG_TXE) == RESET)
        {
             ;
          
        }

        USART_SendData(UARTChnnl, *str++);
    }
    
}



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
void USART2_IRQHandler(void)
{
	volatile uint8_t tmp;
	//	
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		 USART_ClearITPendingBit(USART2,USART_IT_RXNE);
         
		 //将接收到数据发送出去，即自发自收
		 tmp = (USART2->DR & (uint16_t)0x01FF);
         Store2Q(pUART2RQBuff, tmp);		 
	}

}


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
void USART1_IRQHandler(void)
{
	volatile uint8_t tmp;
	//
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE);
         
		 LED_TOGGLE();
		 //将接收到数据发送出去，即自发自收
		 tmp = (USART1->DR & (uint16_t)0x01FF);
         //USART_SendData(USART1,tmp);
         Store2Q(pUART1RQBuff, tmp);
               
		 
	}

}


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
void UART_SendByte(USART_TypeDef* USARTx, uint8_t byte)
{

    //Delay(1000);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
    {
         ;
      
    }       

    USART_SendData(USARTx, byte);
       ///

}


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
void UART_SendBytes(USART_TypeDef* USARTx, uint8_t *pdata, int len)
{
    
    //Delay(1000);
    while(len--)
    {
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
        {
             ;
          
        }       

        USART_SendData(USARTx, *pdata++);
    }


}






