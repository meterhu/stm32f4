#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "stm32f4xx_fsmc.h"
#include "mystd.h"
#include "m_config.h"
#include "SysAppConfig.h"
#include "AppCRC.h"
#include "FramePack.h"
#include "CommHandle.h"
#include "FlashOpt.h"
#include "CPUID.h"
#include "UserLib.h"
#include "WRProtect.h"



uint8_t LockBuff[10]={0x50, 0xA0, 0x5A, 0xA5, 0x1E, 0x29, 0x3F, 0x46, 0x5B, 0xC7};
uint8_t UnlockBuff[16] = {0x91, 0x5A, 0x80, 0xE2, 0x17, 0x2B, 0x3C, 0x92, 0x56, 0x8A, 0x39, 0xD2, 0xE3, 0x1F, 0x01, 0xAA};
//================================================================================================
/*******************************************************************************************************
 *
 *         ��������: 
 *
 *         ��������: 
 *
 *         ��ڲ���: 
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
 *
 *******************************************************************************************************/
int16_t  DataAreaWRPLock(COMM_FRAME *pcf)
{
    uint8_t rv_comm_buff[20];
    uint16_t wrp_lock_param[2];
    uint8_t i;
    uint8_t rt;
    uint8_t buff[20];  
    
    FlagStatus   RDP_state;
    
    wrp_lock_param[0] = MAKE_HWORD((pcf)->param[2], (pcf)->param[1]);
    wrp_lock_param[1] = MAKE_HWORD((pcf)->param[4], (pcf)->param[3]);
    
    //������������
    for(i = 0; i < pcf->len - 5; i++)
    {
        rv_comm_buff[i] = pcf->param[5 + i];   
    }
    
    //ƥ�������������
    rt = MemoryCmp(rv_comm_buff, LockBuff, sizeof(LockBuff));
    if(rt)
    {
        //���ƥ�䣬����Flash�Ķ�д����
        FLASH_Unlock();
        FLASH_OB_Unlock();
        FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

         //����������������
        buff[0] = FLASH_WRP_LOCK_CW;
        buff[1] = 0;
        SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2��2���ֽ�,16bit         
        if(FLASH_OB_GetRDP() != SET)
        {  
           do
            {
                FLASH_OB_RDPConfig(OB_RDP_Level_1);//����������
            }while(FLASH_OB_Launch() != FLASH_COMPLETE);
             
        }  

        RDP_state = FLASH_OB_GetRDP();
        FLASH_OB_Lock();
        FLASH_Lock();
    }
        
    return 1;
}
//================================================================================================
/*******************************************************************************************************
 *
 *         ��������: 
 *
 *         ��������: 
 *
 *         ��ڲ���: 
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
 *
 *******************************************************************************************************/
int16_t  DataAreaWRPUnlock(COMM_FRAME *pcf)
{
    uint32_t wrp_state;
    uint8_t rv_comm_buff[20];
    uint16_t wrp_unlock_param[2];
    uint8_t i;
    uint8_t rt;
    uint8_t buff[20];    
    FlagStatus   RDP_state;
    
    wrp_unlock_param[0] = MAKE_HWORD((pcf)->param[2], (pcf)->param[1]);
    wrp_unlock_param[1] = MAKE_HWORD((pcf)->param[4], (pcf)->param[3]);
    
    //������������
    for(i = 0; i < pcf->len - 5; i++)
    {
        rv_comm_buff[i] = pcf->param[5 + i];   
    }
    
    //ƥ�������������
    rt = MemoryCmp(rv_comm_buff, UnlockBuff, sizeof(UnlockBuff));
    if(rt)
    {
        //���ƥ�䣬����Flash�Ķ�д����
        FLASH_Unlock();
        FLASH_OB_Unlock();
        FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
        

        wrp_state = FLASH_OB_GetWRP();
        if(!wrp_state)
        {  
           do
            {
                FLASH_OB_WRPConfig(WRP_Sector_All, DISABLE); //�ر�д����
            }while(FLASH_OB_Launch() != FLASH_COMPLETE);
            buff[1] = 0; 
        }
        else
        {
            buff[1] = 1;    
        }

         //����������������
        buff[0] = FLASH_WRP_UNLOCK_CW;
        SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2��2���ֽ�,16bit 
        
        __disable_irq();
        if(FLASH_OB_GetRDP() == SET)
        {  
           do
            {
                FLASH_OB_RDPConfig(OB_RDP_Level_0);//�رն�����
            }while(FLASH_OB_Launch() != FLASH_COMPLETE);
             
        }
        else
        {             
            buff[0] = FLASH_WRP_UNLOCK_CW;
            buff[1] = 1;
            SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2��2���ֽ�,16bit 
        }
        
       
        //����֮��Flashȫ������,RAM���Ի�ִ�е�����ĳ���
        __enable_irq();
        
        RDP_state = FLASH_OB_GetRDP();
        FLASH_OB_Lock();
        FLASH_Lock();
    }
    
    return 1;    
}


/*******************************************************************************************************
 *
 *         ��������: 
 *
 *         ��������: 
 *
 *         ��ڲ���: 
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
 *
 *******************************************************************************************************/
void FlashWRPLock(uint8_t sector_num)
{
    //FLASH_Unlock();
    FLASH_OB_Unlock(); 
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);    
    
    if(FLASH_OB_GetWRP() & (0x1<<sector_num))
    {
             FLASH_OB_WRPConfig((0x1<<sector_num), ENABLE); //����д����           
    }

    FLASH_OB_Lock();
    //FLASH_Lock();    
    
}
/*******************************************************************************************************
 *
 *         ��������: 
 *
 *         ��������: 
 *
 *         ��ڲ���: 
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
 *
 *******************************************************************************************************/
void FlashWRPUnlock(uint8_t sector_num)
{
    //FLASH_Unlock();
    FLASH_OB_Unlock(); 
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);    
    
    if(!(FLASH_OB_GetWRP() & (0x1<<sector_num)))
    {
             FLASH_OB_WRPConfig((0x1<<sector_num), DISABLE); //����д����           
    }

    FLASH_OB_Lock();
    //FLASH_Lock();    
    
}
/*******************************************************************************************************
 *
 *         ��������: 
 *
 *         ��������: 
 *
 *         ��ڲ���: 
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
 *
 *******************************************************************************************************/
void FlashWRDPUnlock(void)
{
    uint32_t rt;
    FlagStatus   RDP_state;
    FLASH_Status Flash_state;
    uint32_t     optcr_byte;
    //���ƥ�䣬����Flash�Ķ�д����
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
    FLASH_Unlock();
    FLASH_OB_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

    rt = FLASH_OB_GetWRP();
    
    do
    {
        //FlashWRPLock(WRITE, WRP_Sector_All);
        FLASH_OB_WRPConfig(WRP_Sector_All, DISABLE); //����д����
    }while(FLASH_OB_Launch() != FLASH_COMPLETE);
    
    rt = FLASH_OB_GetWRP();
    
    RDP_state = FLASH_OB_GetRDP();
    do
    {
        FLASH_OB_RDPConfig(OB_RDP_Level_0);//�رն�����
    }while(FLASH_OB_Launch() != FLASH_COMPLETE);
    
    RDP_state = FLASH_OB_GetRDP();    
    FLASH_OB_Lock();
    FLASH_Lock();
    
}
uint32_t  rt;

void FlashWRP(void)
{
    uint32_t rt;
    FlagStatus   RDP_state;
    
    FLASH_Unlock();
    FLASH_OB_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

    RDP_state = FLASH_OB_GetRDP(); 
    rt = FLASH_OB_GetWRP();
    
    do
    {
        FLASH_OB_WRPConfig(WRP_Sector_All, ENABLE); //����д����
    }while(FLASH_OB_Launch() != FLASH_COMPLETE);   

    rt = FLASH_OB_GetWRP();
    
    FLASH_OB_Lock();
    FLASH_Lock();    
    
}



