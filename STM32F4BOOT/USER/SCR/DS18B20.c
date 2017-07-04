//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            文件名称:              
 *
 *                摘要: 18B20驱动程序, DQ为数据口，接于P2.1,   上拉4.7k电阻
 *
 *            创建时间: 2006-00-00  
 *
 *            修改时间: 2006-00-00
 *            修改原因:
 *
 *            
 *
 *                作者: 
 *
 *************************************************************************************************/
 //===============================================================================================
#include "stm32f4xx.h"
#include "stm32f4_std_libs.h"
#include "ucos_ii.h"
#include "mystd.h"
#include "m_config.h"
#include "InitHW.h"
#include "UART.h"
#include "UserLib.h"
#include "DS18B20.h"



//IRQ_Exception
//#define  _DS18B20_WITH_CRC
#define  _DEBUG_R_TMPTR_TM_DLY              75

volatile uint8_t  NopTmp;

#define  _USE_OS

volatile uint16_t   TmptrADDelayMS = 0;

#define  TIMER_TICK_CNT_OV(n)                 1
//================================================================================================
//extern UINT8  TmptrIDBuff[DS18B20_NUMS][8];
//================================================================================================


#ifdef  _DS18B20_WITH_CRC
uint8_t CrcTable [256]=
{
    0,  94, 188,  226,  97,  63,  221,  131,  194,  156,  126,  32,  163,  253,  31,  65,
    157,  195,  33,  127,  252,  162,  64,  30,  95,  1,  227,  189,  62,  96,  130,  220,
    35,  125,  159,  193,  66,  28,  254,  160,  225,  191,  93,  3,  128,  222,  60,  98,
    190,  224,  2,  92,  223,  129,  99,  61,  124,  34,  192,  158,  29,  67,  161,  255,
    70,  24,  250,  164,  39,  121,  155,  197,  132,  218,  56,  102,  229,  187,  89,  7,
    219,  133, 103,  57,  186,  228,  6,  88,  25,  71,  165,  251,  120,  38,  196,  154,
    101,  59, 217,  135,  4,  90,  184,  230,  167,  249,  27,  69,  198,  152,  122,  36,
    248,  166, 68,  26,  153,  199,  37,  123,  58,  100,  134,  216,  91,  5,  231,  185,
    140,  210, 48,  110,  237,  179,  81,  15,  78,  16,  242,  172,  47,  113,  147,  205,
    17,  79,  173,  243,  112,  46,  204,  146,  211,  141,  111,  49,  178,  236,  14,  80,
    175,  241, 19,  77,  206,  144,  114,  44,  109,  51,  209,  143,  12,  82,  176,  238,
    50,  108,  142,  208,  83,  13,  239,  177,  240,  174,  76,  18,  145,  207,  45,  115,
    202,  148, 118,  40,  171,  245,  23,  73,  8,  86,  180,  234,  105,  55,  213, 139,
    87,  9,  235,  181,  54,  104,  138,  212,  149,  203,  41,  119,  244,  170,  72,  22,
    233,  183,  85,  11,  136,  214,  52,  106,  43,  117,  151,  201,  74,  20,  246,  168,
    116,  42,  200,  150,  21,  75,  169,  247,  182,  232,  10,  84,  215,  137,  107,  53
    
};
#endif


uint8_t   TemptrConvTick[2];


//timer3未分频前时钟24M(即96M/4的外频),那么1us timer3的TC计数为:TC 为1s = 24M;1ms TC为 24M / 1000
//1us TC 为24M/1000/1000 = 24
#define  SYS_TM_TICK_PER_MICRO_SEC    24  



//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 18B20初始化
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void  DS18B20IOTest(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    //Temp0_Read
   /// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   /// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   /// GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //Temp0_Write
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    while(1)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);

    }
   
}

//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 18B20初始化
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void InitDS28B20IO(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

   #if  (TEMPTR_CHNNL == 0)    //PA0
    //Temp0_Read
    GPIO_InitStructure.GPIO_Pin = TMPTR0_R_LINE;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(TMPTR0_R_PORT, &GPIO_InitStructure);
    
    //Temp0_Write
    GPIO_InitStructure.GPIO_Pin = TMPTR0_W_LINE;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(TMPTR0_W_PORT, &GPIO_InitStructure);
   #else
    //Temp1_Read
    GPIO_InitStructure.GPIO_Pin = TMPTR1_R_LINE;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(TMPTR1_R_PORT, &GPIO_InitStructure);
    
    //Temp1_Write
    GPIO_InitStructure.GPIO_Pin = TMPTR1_W_LINE;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(TMPTR1_W_PORT, &GPIO_InitStructure);
   #endif


}





//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 18B20初始化
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
uint8_t Init18B20(void)
{
    
    //volatile  UINT32  in;
    ///volatile  UINT32  timer_us2;
    ///char  buff[20];
    volatile  uint32_t  TmptrIOTmOut = 0;
    

 
//      min-480us        |-------------------->min-480us<-------------------|
//      max-960us        |->15-60us<-|------->60-240us<--------|
//                        ___________
// |                     |           |                         |XXXXXXXXXXXX|      
// |_____________________|           |_________________________|XXXXXXXXXXXX|
    
    _D_DIS_INT();
    DQ_OUT(1);
    _NOP();
    _NOP(); 
      
    DQ_OUT(0);
    
    
    DelayUS(600);   //
    
    //切换到输入口, 外部上拉至高, 释放总线
    DQ_OUT(1);
    
    
    //以下操作由DS18B20引起
    ///BEGIN_TIMER_uS();
    
    TmptrIOTmOut = 0;
    
    while (DQ_IN())
    {
        TmptrIOTmOut++;
        if (TmptrIOTmOut++ > 4000)  //超时处理
        {
        	return FALSE_P;
        
        }        
        
    }
    

    ///timer_us1 = GET_TIMER_uS(); 
    
      
   /// BEGIN_TIMER_uS();
    TmptrIOTmOut = 0;       
    while (!DQ_IN())
    {
        if (TmptrIOTmOut++ > 4000)  //超时处理
        {
        	return FALSE_P;
        
        }    
        
    } 
    
    _D_ENA_INT();
   
    
    DelayUS(600);    //
    
    return 1;
    
          
    
}


//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 向18B20写入一个字节
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void DS18B20WriteByte (uint8_t wr)  
{
    uint8_t i;
    

    for (i = 0; i < 8; i++)
    {  

        _D_DIS_INT();////
      

        DQ_OUT(0);
        
        DelayUS(10);   // <= 15
                        
        DQ_OUT(wr & 0x01);
        
        _D_ENA_INT();////
     
        
        DelayUS(20);   //延时 >15 us(写周期不少于15us) 
                    
        
        DQ_OUT(1);    
        
        
        wr >>= 1;
        
        DelayUS(4);   //延时 2 us 
               
    }
    
    DelayUS(2);
    
    
    
}




//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 读18B20的一个字节
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
uint8_t DS18B20ReadByte (void)     
{
    
    uint8_t i;
    uint8_t u;
        

    for (u = 0, i = 0; i < 8; i++)
    {
    
         _D_DIS_INT(); ////
    
        DQ_OUT(0);
        DelayUS(10);   // <= 15
                    
        u >>= 1;
        
        DQ_OUT(1);    //////////////////// 
        
        DelayUS(5); 
              
        if (DQ_IN())
        {
            u |= 0x80;
        }
                
        _D_ENA_INT();  ////
        
        DelayUS(_DEBUG_R_TMPTR_TM_DLY);  /////40
        
        DQ_OUT(1);
        DelayUS(4);  
        
                
        
    }
    
    return(u);
    
}

//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 向18B20写入一个字节
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void DS18B20WriteData(uint8_t *pd, uint8_t n)  
{
    uint8_t i;
    
    for (i = 0; i < n; i++)
    {
        DS18B20WriteByte(*pd++);  
    
    }    
    
    
}


//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 从18B20读取数据(多个字节)
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void DS18B20ReadBytes (uint8_t *pb, uint8_t j)
{
    uint8_t i;
    
    for (i = 0; i < j; i++)
    {
        *pb++ = DS18B20ReadByte();
          
    }
    
    
}

#ifdef  _DS18B20_WITH_CRC
//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: CRC校验
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
uint8_t CRC (uint8_t *pd, uint8_t j)
{
    uint8_t i;
    uint8_t crc_data = 0;
    
    for (i = 0; i < j; i++)  //查表校验
    {
        crc_data = CrcTable[crc_data^pd[i]];
    }
    
    return (crc_data);
    
}

#endif



//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 读取温度
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
int16_t GemTemptr(int16_t *rt_tmptr)
{
    uint8_t  temptr_buff[9] = {0};
    int16_t  temptr;
    static uint8_t  step    = DS18B_START_CONV;

    
    if (step == DS18B_START_CONV) //启动温度转换
    {
        if (!Init18B20())
        {
        	return TMPTR_INIT_TMOUT;
        }
        
     #if __DS18B20_MATCH_ROM == 1
        DS18B20WriteByte(DS18B20_MATCH_ROM);
        DS18B20WriteData(TmptrIDBuff[id], 8); 
       
     #else
        DS18B20WriteByte(DS18B20_SKIP_ROM);      //跳过ROM
     #endif
        
        DS18B20WriteByte(DS18B20_CONVERT_TEM);   //温度转换
     
        step = DS18B_START_GETV;
        
        TmptrADDelayMS = 0;

        return  TMPTR_INIT_OVER;
        
        
    }
    else if (step == DS18B_START_GETV)
    {
        //================================================================
        //9-bit 93.75ms (tconv/8)
        // 10-bit 187.5ms (tconv/4)
        // 11-bit 375ms   (tconv/2)
        // 12-bit 750ms   (tconv)
        // 注意前面的配置

        //下面的延时是必要的,也可在外围延迟
        
        #ifdef _USE_OS
         //OSTimeDly(800 / (1000 / OS_TICKS_PER_SEC));
          OSTimeDlyHMSM(0, 0, 0, 1000);
        #else
         //DelayUS(8000);
         DelayUS(8000);  //9-bit 93.75ms (tconv/8) 
        #endif
       

        step = DS18B_START_CONV;
        if (!Init18B20())
        {
        	
            return TMPTR_INIT_TMOUT;
        }
            
       #if  __DS18B20_MATCH_ROM == 1
        DS18B20WriteByte(DS18B20_MATCH_ROM);
        DS18B20WriteData(TmptrIDBuff[id], 8);
       #else
        DS18B20WriteByte(DS18B20_SKIP_ROM);      //跳过ROM
       #endif
          
        DS18B20WriteByte(DS18B20_READ_RAM);      //读温度
        DS18B20ReadBytes(temptr_buff, 2);
        
       #if  __DS18B20_MATCH_ROM == 1
        if (CRC(temptr_buff, 2) == 0) //校验正确,读取9个字节时才能校验
        {
            temptr = ((((temptr_buff[1] << 8) | temptr_buff[0])) * 10) / 16;
            
        }
       #else
        temptr = ((((temptr_buff[1] << 8) | temptr_buff[0])) * 10) / 16;
       #endif
        
        
        if ((temptr > TMPTR_AD_MAX_V) || (temptr == TMPTR_INVALID_V1) || (temptr < TMPTR_AD_MIN_V))
        {
        	////TRACE("temptr value is  %d\r\n", temptr);
            return TMPTR_INVALID_V;
        
        }
        else
        {
             *rt_tmptr = temptr;
             
        }

    
    }
    
    return TMPTR_VALID_V;
    
}


//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 内部配置, 配置报警限定值和分辨率
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void DS18B20Conf(void)  
{
    Init18B20();
    DS18B20WriteByte(DS18B20_SKIP_ROM);       //跳过ROM
    DS18B20WriteByte(DS18B20_WRITE_RAM);      //读便笺寄存器
    DS18B20WriteByte(0x19);                   //上限
    DS18B20WriteByte(0x1A);                   //下限
    DS18B20WriteByte(0x1F);                   //设定精度,9位
    Init18B20();
    DS18B20WriteByte(DS18B20_SKIP_ROM);       //跳过ROM
    DS18B20WriteByte(DS18B20_COPY_RAM);       //保存设定值
    Init18B20();
    DS18B20WriteByte(DS18B20_SKIP_ROM);       //跳过ROM
    DS18B20WriteByte(DS18B20_EECALL_EEPROM);  //回调设定值
    
    
}

//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 读18B20ID
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void DS18B20ReadID(uint8_t *id_buff)
{
    ///////DIS_INT();   
    Init18B20();
    DS18B20WriteByte(DS18B20_READ_ROM);  //读ROM
    DS18B20ReadBytes(id_buff, 8);        //8个字节，见DS18B20说明
   // ENA_INT();
    
}

 
