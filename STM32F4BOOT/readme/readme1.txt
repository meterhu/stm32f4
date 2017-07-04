

------------------------------------------------------------------------------------------------------------------------------------------------------
一.系统频率和外频
系统频率(SystemFrequency) = 96MHZ
外频 =  系统频率 / 4 = 24MHZ(见各外设模块的初始化频率设定代码，如usFdiv = (SystemFrequency/4/16) / UART_BPS; 先将系统频率除4，即外频) 
外频设定代码在文件system_LPC17xx.c:
LPC_SC->CCLKCFG   = CCLKCFG_Val;                                    /* Setup Clock Divider            */
其中 CCLKCFG_Val的值定义为3,即为3 + 1 = 4分频，更具体的参考器件文档第四章中关于时钟的描述

各外频模块经过PCLKSEL0，PCLKSEL1的设定后--在system_LPC17xx.c文件中将其均设为0---即各模块均使用PCLK，不再次分频

------------------------------------------------------------------------------------------------------------------------------------------------------


二.优先级
1.systick 优先级为 : #define __NVIC_PRIO_BITS          5 


------------------------------------------------------------------------------------------------------------------------------------------------------
三.中断服务程序挂接
在system_LPC17xx.s中，包含各中断服务程序的入口地址，包含了服务例程的通用名，用户的程序中只要按此名称来实现服务例程即可。
如第70行:
                DCD     SysTick_Handler           ; SysTick Handler

而用户只要完成SysTick_Handler函数的实现。



------------------------------------------------------------------------------------------------------------------------------------------------------
四.UCOS使用编译选项
为了保持程序在无OS的情况下运行的兼容性，自定义了_OS_USE编译开关，该宏定义在m_config.h中可以看到

------------------------------------------------------------------------------------------------------------------------------------------------------

五.OS中断中的内联函数OS_ENTRY_INT(),OS_EXIT_INT()
因为无法使在宏定义中使用条件编译，且为了保持设计的简洁化和中断程序运行的高效，使用了强制内联函数(关键字__forceinline),强制内联

------------------------------------------------------------------------------------------------------------------------------------------------------
六.关于OSxxxTick

1.tasks.c中函数MainTask初始化os Tick

  SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC - 1); //1ms  

2. core_cm3.h中SysTick_Config的实现

    static __INLINE uint32_t SysTick_Config(uint32_t ticks)
    {
      if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
    
      SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
      NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
      SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
      SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                       SysTick_CTRL_TICKINT_Msk   |
                       SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
      return (0);                                                  /* Function successful */
    } 

3.startup_LPC17xx.s中(79行) DCD     OS_CPU_SysTickHandler;SysTick_Handler           ; SysTick Handler


4. 文件os_cpu_c.c中OS_CPU_SysTickHandler实现
    void  OS_CPU_SysTickHandler (void)
    {
        OS_CPU_SR  cpu_sr;
    
    
        OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
        OSIntNesting++;
        OS_EXIT_CRITICAL();
    
        OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */
    
        OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
    }
