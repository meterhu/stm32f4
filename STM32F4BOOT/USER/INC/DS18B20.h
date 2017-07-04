//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            文件名称:  DS18B20.h            
 *
 *                摘要: 
 *
 *            创建时间: 2006-00-00  
 *
 *            修改时间: 2006-00-00
 *            修改原因:
 *
 *            
 *
 *                作者: 杨卫华
 *
 *************************************************************************************************/
 //===============================================================================================
 
#ifndef _DS18B20_H
#define _DS18B20_H

//=================================================================================================
//*************************************************************************************************
#define DS18B20_READ_ROM           0x33
#define DS18B20_MATCH_ROM          0x55
#define DS18B20_SKIP_ROM           0xCC
#define DS18B20_SEARCH_ROM         0xf0
#define DS18B20_ALARM_SEARCH_ROM   0xEC
#define DS18B20_WRITE_RAM          0x4E
#define DS18B20_READ_RAM           0xBE
#define DS18B20_COPY_RAM           0x48
#define DS18B20_CONVERT_TEM        0x44
#define DS18B20_EECALL_EEPROM      0xB8
#define DS18B20_READ_POWER_SUPPLY  0xB4


#define  BIT12_RESOLU              0x11
#define  BIT11_RESOLU              0x10
#define  BIT10_RESOLU              0x01
#define  BIT09_RESOLU              0x00



#define  TMPT_RESOLU               BIT12_RESOLU

 

////#define  NOT(v, type)   ((type)(~((type)(v))))  

extern volatile uint8_t  NopTmp;
#define  _NOP()       NopTmp = 0; NopTmp = 1;NopTmp = 2;NopTmp = 3;NopTmp = 4
#define  NOP()        NopTmp = 0; NopTmp = 1



#define  DS18B_START_CONV  0xFF
#define  DS18B_START_GETV  0x01

//==========================================================================================
#define TEMPTR_CHNNL     (1)
//Temp0_Read   P0.15
//Temp0_Write  P0.16

//Temp1_Read     P0.11
//Temp1_Write    P0.10
//#define  DQ_IO_BIT     BIT17

#if  (TEMPTR_CHNNL == 0)
 #define  DQ_OUT_IO     BIT16
#else
 #define  DQ_OUT_IO     BIT10
#endif

//#define  DQ_OUT(bit)   IO1DIR |= DQ_OUT_IO; if (bit) IO1SET = DQ_OUT_IO; else IO1CLR = DQ_OUT_IO


#define  DS18B20_NUMS    2

#define  DS18B20_MIN_V              (-40)  //最低温度


//================================================================================================
#define  _D_DIS_INT()             //OS_ENTER_CRITICAL()          //DIS_REFRESH_LED()  //2008-03-16
#define  _D_ENA_INT()             //OS_EXIT_CRITICAL()           //2008-03-16

#define  __DS18B20_MATCH_ROM     (0)
//================================================================================================

#define  TMPTR_INIT_TMOUT           -50
#define  TMPTR_INVALID_V            -60
#define  TMPTR_INIT_OVER            0
#define  TMPTR_VALID_V              1

#define  TMPTR_AD_DELAY_MS          200  //采样转换时间
#define  TMPTR_AD_MAX_V             1000 //100度
#define  TMPTR_AD_MIN_V             (-10) //-10度

#define  TMPTR_DIFF_MAX_V           100  //两次温度采样值允许最大误差25度
#define  TMPTR_INVALID_V1           850  //



#define  TMPTR0_R_PORT             GPIOA          
#define  TMPTR0_R_LINE             GPIO_Pin_4

#define  TMPTR0_W_PORT             GPIOA 
#define  TMPTR0_W_LINE             GPIO_Pin_1



#define  TMPTR1_R_PORT             GPIOA         
#define  TMPTR1_R_LINE             GPIO_Pin_5

#define  TMPTR1_W_PORT             GPIOB 
#define  TMPTR1_W_LINE             GPIO_Pin_0

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
__inline  void  DQ_OUT(uint8_t bit)
{
     GPIO_InitTypeDef GPIO_InitStructure;
    

    
    //Temp1_read
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

 #if  (TEMPTR_CHNNL == 0)    //PA0
    GPIO_InitStructure.GPIO_Pin = TMPTR0_W_LINE;
    GPIO_Init(TMPTR0_W_PORT, &GPIO_InitStructure);
    if (bit)
    {
        GPIO_SetBits(TMPTR0_W_PORT, TMPTR0_W_LINE);
    }
    else
    {
        GPIO_ResetBits(TMPTR0_W_PORT, TMPTR0_W_LINE);
    }
 #else
    GPIO_InitStructure.GPIO_Pin = TMPTR1_W_LINE;
    GPIO_Init(TMPTR1_W_PORT, &GPIO_InitStructure);
    if (bit)
    {
        GPIO_SetBits(TMPTR1_W_PORT, TMPTR1_W_LINE);
    }
    else
    {
        GPIO_ResetBits(TMPTR1_W_PORT, TMPTR1_W_LINE);
    }
 #endif
	
   
     

}



//========================================================================================
//
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
__inline  uint32_t  DQ_IN(void)
{
    /////IO0DIR &= NOT(DQ_IO_BIT, UINT32);
    uint32_t pin_val;       
	GPIO_InitTypeDef GPIO_InitStructure;
   
    NOP();

   ///  
	//Temp1_Read
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

   #if  (TEMPTR_CHNNL == 0)    //PA0
    GPIO_InitStructure.GPIO_Pin = TMPTR0_R_LINE;
    GPIO_Init(TMPTR0_R_PORT, &GPIO_InitStructure);

    //pin_val = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
    pin_val = GPIO_ReadInputDataBit(TMPTR0_R_PORT, TMPTR0_R_LINE);
   
   #else   //PB1
    GPIO_InitStructure.GPIO_Pin = TMPTR1_R_LINE;
    GPIO_Init(TMPTR1_R_PORT, &GPIO_InitStructure);

    //pin_val = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
    pin_val = GPIO_ReadInputDataBit(TMPTR1_R_PORT, TMPTR1_R_LINE);
   #endif
    
    return pin_val;

}




extern uint8_t  TemptrIDBuff[DS18B20_NUMS][8];

//=================================================================================================
//*************************************************************************************************
extern void  DS18B20ReadID(uint8_t *id_buff);
extern int16_t GemTemptr(int16_t *tmptr);
extern void  DS18B20Conf(void);
//void  InitDS18B20IOConf(void);
//INT16 GemAllTemptr(INT16 *pd, UINT8 n);
extern void InitDS28B20IO(void);
extern uint8_t Init18B20(void);
extern void  DS18B20IOTest(void);

 
 
#endif //_DS18B20_H


