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
#include "ucos_ii.h"
#include "m_config.h"
#include "mystd.h"
#include "DigitRP.h"
#include "userlib.h"
#include "SysAppConfig.h"
#include "ExINT.h"

//=======================================================================================
int32_t    DigitRPCnt  = 0;
u32 EXTITempNum0;
u32 Exti_0_Num=0;


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
void  InitDigitRP(void)
{
	DigitRP_Configuration();
	DigitRP_EXTI_Configuration();
	DigitRP_NVIC_Config();
    //InitExINT0_NVIC();
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
void DigitRP_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //使能GPIOA的AHB1时钟 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE ,ENABLE);                            

    /* Configure RIGHTB_IN1_NUM/RIGHTB_IN0_NUM (PE.01/PE.2) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* Configure LEFTB_IN1_NUM/LEFTB_IN0_NUM (PB.3/PE.0) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    
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
void DigitRP_EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    EXTI_ClearITPendingBit(EXTI_Line0);
    EXTI_ClearITPendingBit(EXTI_Line2);

    /* Connect EXTI Line to PB pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);


    //LEFT_DRP
    /* Enable the EXTI Line3 Interrupt */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    //RIGHT_DRP
    /* Enable the EXTI Line1 Interrupt */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


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
void DigitRP_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the EXTI9_5_IRQHandler Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable the EXTI9_5_IRQHandler Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);


 
}

//=======================================================================================
//INT32    LeftDWQCnt  = 0;
//INT32    RightDWQCnt = 0;



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
 *             说明: 调用:if (RPState)
 *                       {
 *                           DigitRP(LEFT_DRP);
 *                       }
 *
 *******************************************************************************************************/
void LeftDigitRP(void)
{
	static   uint32_t pre_tm_tick = 0;
	
	static   uint8_t  pre_dwq_dir = 0;
	static   uint8_t  dwq_dir = 0;
	uint32_t   tmp;
	//_BOOL    flg;
	
    

	//加速计数器
	static   uint32_t accel_cnt   = 0;
	
	uint32_t   tmp_tm_tick;
	volatile uint32_t m_int_state;
	
   
   //// rI_ISPC   = BIT_EINT0; //clear pending_bit  
    
    AVOID_WARN(pre_dwq_dir); //防止编译警告
     
    tmp_tm_tick = GET_SYS_TIMER_TICK();  //4ms的中断计数
       
    tmp = (tmp_tm_tick - pre_tm_tick);
    if (tmp < 8) //注意32位的计数器，在此一次开机使用时不会溢出
    {
         accel_cnt += (8 - tmp); 
    
    
    }	    
    else if (tmp >= 8)
    {
    	accel_cnt = 0;
    
    }
    
    /////TRACE("tick :%d\r\n", tmp); 
    
    //读取引脚状态 
    m_int_state    = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);//LPC_GPIO0->FIOPIN & BIT25;  //P0.25

    //左旋或右旋判断    
    if (m_int_state)  
    {
        dwq_dir = 0x01;
        
        //if(!((pre_dwq_dir != dwq_dir) && ((tmp_tm_tick - pre_tm_tick) < 50)))   //假定用户两次方向转换的时间不少于4 * 50 = 200ms
        {
              
	        DigitRPCnt++;
	        DigitRPCnt += accel_cnt;
	        if (DigitRPCnt >= DRP_MAX_V)
	        {
	        
	        	DigitRPCnt = DRP_MAX_V;
	        }
	        
	        pre_dwq_dir = dwq_dir;
        
        }
                
    }
    else
    {
        dwq_dir = 0x02;
        //if (!((pre_dwq_dir != dwq_dir) && ((tmp_tm_tick - pre_tm_tick) < 50))) 
        {
	        DigitRPCnt--;
	        DigitRPCnt -= accel_cnt;
	        if (DigitRPCnt < 0)
	        {
	            DigitRPCnt = 0;
	        }
	        
	        pre_dwq_dir = dwq_dir;
        }
      
    }
    
    pre_tm_tick = tmp_tm_tick;
    
    ///TRACE("DigitRPCnt:%d\r\n", DigitRPCnt);       

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
void RightDigitRP(void)
{
	static   uint32_t pre_tm_tick = 0;
	
	static   uint8_t  pre_dwq_dir = 0;
	static   uint8_t  dwq_dir = 0;
	uint32_t   tmp;
	//_BOOL    flg;
	
	//加速计数器
	static   uint32_t accel_cnt   = 0;
	
	uint32_t   tmp_tm_tick;
	volatile uint32_t m_int_state;
    volatile uint32_t m_int_state1;
	
 
   //// rI_ISPC   = BIT_EINT0; //clear pending_bit  
    
    AVOID_WARN(pre_dwq_dir); //防止编译警告
     
    tmp_tm_tick = GET_SYS_TIMER_TICK();  //4ms的中断计数
       
    tmp = (tmp_tm_tick - pre_tm_tick);
    if (tmp < 8) //注意32位的计数器，在此一次开机使用时不会溢出
    {
         accel_cnt += (8 - tmp); 
    
    
    }	    
    else if (tmp >= 8)
    {
    	accel_cnt = 0;
    
    }
    
    ///TRACE("tick :%d\r\n", tmp); 
    
    //读取引脚状态 
    m_int_state = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1);
    //左旋或右旋判断    
    if (m_int_state)  
    {
        dwq_dir = 0x01;
        
        //if(!((pre_dwq_dir != dwq_dir) && ((tmp_tm_tick - pre_tm_tick) < 50)))   //假定用户两次方向转换的时间不少于4 * 50 = 200ms
        {
              
	        DigitRPCnt++;
	        DigitRPCnt += accel_cnt;
	        if (DigitRPCnt >= DRP_MAX_V)
	        {
	        
	        	DigitRPCnt = DRP_MAX_V;
	        }
	        
	        pre_dwq_dir = dwq_dir;
        
        }
                
    }
    else
    {
        dwq_dir = 0x02;
        //if (!((pre_dwq_dir != dwq_dir) && ((tmp_tm_tick - pre_tm_tick) < 50))) 
        {
	        DigitRPCnt--;
	        DigitRPCnt -= accel_cnt;
	        if (DigitRPCnt < 0)
	        {
	            DigitRPCnt = 0;
	        }
	        
	        pre_dwq_dir = dwq_dir;
        }
      
    }
    
    pre_tm_tick = tmp_tm_tick;
    
    ///TRACE("DigitRPCnt:%d\r\n", DigitRPCnt);       

}









