

------------------------------------------------------------------------------------------------------------------------------------------------------
һ.ϵͳƵ�ʺ���Ƶ
ϵͳƵ��(SystemFrequency) = 96MHZ
��Ƶ =  ϵͳƵ�� / 4 = 24MHZ(��������ģ��ĳ�ʼ��Ƶ���趨���룬��usFdiv = (SystemFrequency/4/16) / UART_BPS; �Ƚ�ϵͳƵ�ʳ�4������Ƶ) 
��Ƶ�趨�������ļ�system_LPC17xx.c:
LPC_SC->CCLKCFG   = CCLKCFG_Val;                                    /* Setup Clock Divider            */
���� CCLKCFG_Val��ֵ����Ϊ3,��Ϊ3 + 1 = 4��Ƶ��������Ĳο������ĵ��������й���ʱ�ӵ�����

����Ƶģ�龭��PCLKSEL0��PCLKSEL1���趨��--��system_LPC17xx.c�ļ��н������Ϊ0---����ģ���ʹ��PCLK�����ٴη�Ƶ

------------------------------------------------------------------------------------------------------------------------------------------------------


��.���ȼ�
1.systick ���ȼ�Ϊ : #define __NVIC_PRIO_BITS          5 


------------------------------------------------------------------------------------------------------------------------------------------------------
��.�жϷ������ҽ�
��system_LPC17xx.s�У��������жϷ���������ڵ�ַ�������˷������̵�ͨ�������û��ĳ�����ֻҪ����������ʵ�ַ������̼��ɡ�
���70��:
                DCD     SysTick_Handler           ; SysTick Handler

���û�ֻҪ���SysTick_Handler������ʵ�֡�



------------------------------------------------------------------------------------------------------------------------------------------------------
��.UCOSʹ�ñ���ѡ��
Ϊ�˱��ֳ�������OS����������еļ����ԣ��Զ�����_OS_USE���뿪�أ��ú궨����m_config.h�п��Կ���

------------------------------------------------------------------------------------------------------------------------------------------------------

��.OS�ж��е���������OS_ENTRY_INT(),OS_EXIT_INT()
��Ϊ�޷�ʹ�ں궨����ʹ���������룬��Ϊ�˱�����Ƶļ�໯���жϳ������еĸ�Ч��ʹ����ǿ����������(�ؼ���__forceinline),ǿ������

------------------------------------------------------------------------------------------------------------------------------------------------------
��.����OSxxxTick

1.tasks.c�к���MainTask��ʼ��os Tick

  SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC - 1); //1ms  

2. core_cm3.h��SysTick_Config��ʵ��

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

3.startup_LPC17xx.s��(79��) DCD     OS_CPU_SysTickHandler;SysTick_Handler           ; SysTick Handler


4. �ļ�os_cpu_c.c��OS_CPU_SysTickHandlerʵ��
    void  OS_CPU_SysTickHandler (void)
    {
        OS_CPU_SR  cpu_sr;
    
    
        OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
        OSIntNesting++;
        OS_EXIT_CRITICAL();
    
        OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */
    
        OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
    }
