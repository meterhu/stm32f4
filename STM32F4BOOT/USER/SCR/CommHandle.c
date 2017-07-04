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
//#include "ucos_ii.h"
#include "mystd.h"  
#include "m_config.h"
#include "UserLib.h"
#include "Queue.h"
#include "Buff.h"
#include "main.h"
#include "UART.h"
#include "FramePack.h"
#include "CommHandle.h"
#include "CommTaskProc.h"
#include "main.h"
#include "tasks.h"
#include "SysAppConfig.h"
#include "CPUID.h"
#include "FlashOpt.h"
#include "Capher.h"
#include "FirmwareUpdate.h"
#include "WRProtect.h"





//========================================================================================
static COMM_FRAME  CommFrame;
static COMM_FRAME  *pCF;

static void ExecFrame(COMM_FRAME *pcf);
//========================================================================================
#if (COMM_UART == 0) 
    extern QUEUE   *pUART1RQBuff; 
#elif (COMM_UART == 1) 
    extern QUEUE   *pUART2RQBuff; 
#endif
//========================================================================================

#define     DATA_BUFF2_SIZE 0x1FFF
uint8_t     DataBuff2[DATA_BUFF2_SIZE];
QUEUE       FrameQ2;
QUEUE       *pFrameQ2;


extern  void Cipher(unsigned char *intext, unsigned char * key, unsigned char * outtext);

__asm void SystemReset(void);
int16_t  SystemResetProc(COMM_FRAME *pcf);
int16_t  BaudRateProc(COMM_FRAME *pcf);
//下面的错误测试帧有如下特点
//uint8_t TestFrame[]= {0xAA, 0xAA, 0x77, 0xAA, 0x77, 0x01, 0xAF, 0x02, 0x00, 0xAA, 0x77, 0x01, 
//                      0xAF, 0x02, 0x00, 0x01, 0x02, 0x70};


extern uint8_t  FirmwareUpdateFlag;
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
void InitComm(void)
{
  
    pCF        = &CommFrame;
    pCF->state = FRM_NO_STATE; 
    
    pFrameQ2 = &FrameQ2;

    InitQueue(pFrameQ2, DataBuff2, DATA_BUFF2_SIZE, Q_EMPTY);


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
uint16_t  ParseFrame(uint8_t m_data)
{
    
    //=================================================================================================
    //帧格式:
    //0xAA 0x70 命令字(1 byte)  保留(1 byte)  数据长度低字节    数据长度高字节   数据 ...  CRC(1byte)
    //其中 数据长度为16位,其后用户数据的长度
    //CRC为其前所有字节的异或
    //=================================================================================================
    if ((m_data == FRM_HEAD1_FLG) && (pCF->state == FRM_NO_STATE))    //帧头判断
    {
        pCF->state = FRM_HEAD1_STATE;
                                  
    }           
    else if ((m_data == FRM_HEAD2_FLG) && (pCF->state == FRM_HEAD1_STATE))
    {
         pCF->state = FRM_HEAD2_STATE;
    }
    else if (pCF->state == FRM_HEAD2_STATE)
    {
        pCF->crc   = (m_data ^ FRM_HEAD1_FLG ^ FRM_HEAD2_FLG);  
        pCF->func  = m_data;
        pCF->state = FRM_FUNC_STATE;
    }
    else if (pCF->state == FRM_FUNC_STATE)  //长度低字节
    {
         pCF->state = FRM_LENL_STATE;
         pCF->len   = m_data + 1;
         pCF->crc  ^= m_data; 

    }
    else if (pCF->state == FRM_LENL_STATE)   //长度高字节
    {

        pCF->len  |= (m_data << 8);
        pCF->crc  ^= m_data;  
        if (pCF->len > MAX_FRAME_SIZE)   //长度超标
        {
             ;                   
        }
        else
        {
            pCF->state  = FRM_LENH_STATE;
            //pCF->p      = pCF->param;
        }
        
        pCF->cur_i = 0;            //cur_i当前数据长度索引,它在后续程序中递增变化
        pCF->state = FRM_LENH_STATE;

    }
    else if (pCF->state ==FRM_LENH_STATE)  //有效数据
    {   
        if  (pCF->cur_i < pCF->len)
        {
            pCF->param[pCF->cur_i++] = m_data;
            pCF->crc ^= m_data; 
        }
        else
        {
                        
            if (pCF->crc == m_data)  //校验通过
            {
                pCF->err = FRM_NO_ERR;

                //解析执行帧
                
                ExecFrame(pCF);
                    
            }
            else
            {
                ;                        
               
            } 

            pCF->state = FRM_NO_STATE;

        }
        
    }
    else 
    {
        pCF->state  = FRM_NO_STATE;
    }
       
    return 1;    

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
void  FrameParseExec(void)
{
  
    uint8_t   tmp;
    volatile uint8_t state; 
    ///OS_CPU_SR  cpu_sr = 0;
        
    //把UART缓冲队列中的数据保存到2级缓存队列FrameQ2中
   
    while(1)
    {

        __disable_irq();   //此处关中断是必要的
#if (COMM_UART == 0) 
        tmp = FetchFromQ(pUART1RQBuff);
        state = pUART1RQBuff->state;  
#elif (COMM_UART == 1) 
        tmp = FetchFromQ(pUART2RQBuff);
        state = pUART2RQBuff->state;  
#endif
        __enable_irq();
         
        if (state != Q_EMPTY)
        {
            ParseFrame(tmp);
        }
        else
        {
            break;
        }            
        
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
void ExecFrame(COMM_FRAME *pcf)
{
    
    FirmwareUpdateFlag = 1;
    switch(pcf->func)
    {
        case START_FIRE_WARE_UPDATE_CW:              //启动固件升级数据帧          
            StartFireWareUpdate(pcf);            
            break;
        
        case FIRMW_DATA_CW:                          //固件数据帧
            FirmwDateUpdate(pcf);
            break;
         
        case STA_DES_CW:                             //启动加密数据帧
            StartDesProc(pcf);
            break;
        
        case DES_KEY_CW:                             //加密密码数据帧
            DesKeyProc(pcf);
            break;
        
        case FLASH_WRP_LOCK_CW:                      //Flash 读写加锁
            DataAreaWRPLock(pcf);
            break;
        
        case FLASH_WRP_UNLOCK_CW:                    //Flash 读写解锁
            DataAreaWRPUnlock(pcf);
            break;   
        
        case SYS_RESET_CW:                           //MCU复位数据帧
            SystemResetProc(pcf);
            break; 

        case BAUD_RATE_CW:                           //波特率设置数据帧
            BaudRateProc(pcf);
            break;        
        default:
            ;
        
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
int16_t  SystemResetProc(COMM_FRAME *pcf)
{
    uint8_t  sys_reset_comm[20];
    volatile uint16_t sys_reset_param;
    uint8_t i;
    ///uint8_t *paddr;   
    //paddr = (uint8_t *)APP1_START_ADDR;
    
    sys_reset_param = MAKE_HWORD((pcf)->param[2], (pcf)->param[1]);
    
    for(i = 0; i < pcf->len - 3; i++)
    {
        sys_reset_comm[i] = pcf->param[3 + i];  
        
    }
    

//     if(*(paddr + 6) == sys_reset_comm[0])
//     if(*(paddr + 7) == sys_reset_comm[1]) 
//     if(*(paddr + 8) == sys_reset_comm[2])
//     if(*(paddr + 9) == sys_reset_comm[3])
        
    if(0xAA == sys_reset_comm[0])
    if(0x55 == sys_reset_comm[1]) 
    if(0xA5 == sys_reset_comm[2])
    if(0x5A == sys_reset_comm[3])
    {     
        SystemReset();

    }    

    return 1;
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
int16_t  BaudRateProc(COMM_FRAME *pcf)
{
    uint32_t uart_baud_rate;
    uint8_t  buff[20];

    buff[0] = BAUD_RATE_CW;
    buff[1] = UART_BAUD_RATE_SUCCESS;
    SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2即2个字节,16bit 
    
    uart_baud_rate = MAKE_4BYTE_WORD((pcf)->param[4], (pcf)->param[3], (pcf)->param[2], (pcf)->param[1]);

#if (COMM_UART == 0) 
    InitUART(USART1,uart_baud_rate);
#elif (COMM_UART == 1) 
    InitUART(USART2,uart_baud_rate);
#endif   
    
    

    
    return 1;
    
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
__asm void SystemReset(void)
{
 MOV R0, #1           //; 
 MSR FAULTMASK, R0    //; 清除FAULTMASK 禁止一切中断产生
 LDR R0, =0xE000ED0C  //;
 LDR R1, =0x05FA0004  //; 
 STR R1, [R0]         //; 系统软件复位   

deadloop
    B deadloop        //; 死循环使程序运行不到下面的代码
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
// __asm void RunApp1(void)
// {
//  LDR R0, =APP_VALID_ADDR  
//  MSR MSP,R0
//  LDR R1, =APP_VALID_ADDR + 0x189
//  BX  R1
//    
// }

// __asm void RunApp(void)
// {
//  LDR R0, =APP_VALID_ADDR;0x20010878 
//  MSR MSP,R0
//  LDR R1, =0x0800C39c
//  BX  R1
//    
// }




