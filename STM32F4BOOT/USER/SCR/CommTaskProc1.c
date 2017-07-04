//======================================================================================================
/******************************************************************************************************
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
 *******************************************************************************************************/
//======================================================================================================
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "mystd.h"  
#include "m_config.h"
#include "UserLib.h"
#include "SysAppConfig.h"
#include "Queue.h"
#include "Buff.h"
#include "UART.h"
#include "FramePack.h"
#include "CommHandle.h"
#include "main.h"
#include "tasks.h"
#include "CommTaskProc.h"
#include "RevAdvAngle.h"
#include "PWM.h"
#include "OS_Trace.h"
#include "SimuI2C.h"
#include "ADS123x.h"
#include "CPUID.h"


MCU_INFO  muc_info;
#define  CPU_ID_BASE_ADDR   0x1FFF7A10
#define  FLASH_START_ADDR   0x08008000
#define  FLASH_SIZE         0xD8000    // 16K + 16K + 64K + 128K * 6
#define  SECTOR_NUM         9

uint16_t  TotalFileLen;
uint16_t  DataPackAll;
int16_t   DataPackLen;
int8_t    CHECK_A;
int8_t    CHECK_B;   

static   uint16_t  EraseSectorSize;
const uint8_t    mcu_key_buff[16] = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};    


extern     SAVE_PARAM_ONE  save_para_one;

extern TASK_INFO   TaskInfo[];
u16 Bit_Buff[12] = {BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,BIT8,BIT9,BIT10,BIT11};	   //���׺�

//======================================================================================================
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
int16_t  BootFirmwUpdProc(COMM_FRAME *pcf)
{      
    uint32_t   date_num;
    uint8_t    buff[4];
    //FLASH_Status  flash_state;
    
    TotalFileLen  = MAKE_HWORD((pcf)->param[2], (pcf)->param[1]);    
    DataPackAll   = GET_DATA0_HWORD(pcf);
    DataPackLen   = GET_DATA1_HWORD(pcf);
    CHECK_A       = pcf->param[7];
    CHECK_B       = pcf->param[8];
    
    date_num = DataPackAll * DataPackLen;
    //������Ҫ����������
    EraseSectorSize = TotalFileLen / 1024;
    
    //0-3    MCU ID  
    //4-7    flash����
    //8-9    ��������
    //10-13  Flash��ʼ��ַ
    
    //muc_info.id               = *(volatile uint32_t *)(CPU_ID_BASE_ADDR);
    muc_info.id               = DBGMCU_IDCODE_DEV_ID | DBGMCU_IDCODE_REV_ID;
    muc_info.flash_size       = FLASH_SIZE;
    muc_info.sector_num       = SECTOR_NUM;
    muc_info.flash_start_addr = FLASH_START_ADDR;
    
    //0      Ӧ��������
    //1      Ӧ����
    //2-3    Ӧ�����
    buff[0]  = BOOT_FIRM_UPD_CW;
    buff[1]  = 0x01;  //����Ӧ��
    buff[2]  = 0x00;
    buff[3]  = 0x00;
#if 0    
    if(EraseSectorSize < 16)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
    }
    else if(EraseSectorSize < 32)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //���� 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }        
    }
    else if(EraseSectorSize < 96)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3); //����   
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }        
    }
    else if(EraseSectorSize < 224)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3); //���� 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_5,VoltageRange_3); //���� 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }        
    }

#endif
    
    if(date_num > muc_info.flash_size)
    {
        SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4);   //2��2���ֽ�,16bit    
    }
    else
    {
        SendFrame(MCU_INFO_CW, (uint8_t *)&muc_info, 14);   //2��2���ֽ�,16bit   
    }
    
          
    return 1;
}


//======================================================================================================
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
int16_t  FirmwDateProc(COMM_FRAME *pcf)
{    
    uint8_t    buff[4];
    uint8_t    event_report[1];
    uint8_t    i;
    volatile uint16_t    data_pack_cur_num;
    volatile uint16_t    data_len;
    
    //FLASH_Status  flash_state;
    
    static uint8_t check_a;
    static uint8_t check_b;
    
    static uint32_t *paddr;   //����һ��ָ��ʼ��ַ��ָ��
    paddr = (uint32_t *)FLASH_START_ADDR;
    
    data_pack_cur_num = MAKE_HWORD((pcf)->param[2], (pcf)->param[1]);
    data_len          = GET_DATA0_HWORD(pcf);;
    
    //0      Ӧ��������
    //1      Ӧ����
    //2-3    Ӧ�����
    buff[0]  = FIRMW_DATA_CW;
    buff[1]  = 0x00;  //����Ӧ��
    buff[2]  = data_pack_cur_num & 0xFF;
    buff[3]  = (data_pack_cur_num >> 8) & 0xFF;

    FLASH_Unlock();	//����
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    
    for(i = 0; i < data_len - 2; i++)
    {
        check_a = check_a + pcf->param[i + 3];
        check_b = check_b + check_a;

        #if 0
        flash_state = FLASH_ProgramWord(*paddr++, (pcf)->param[i + 3]);//д������
                                                                              
        if(flash_state != FLASH_COMPLETE)
        {
            buff[1]  = 0x01;
            event_report[0] = WRITE_SECTOR_ERR;
            SendFrame(EVENT_REPOT_CW, (uint8_t *)event_report, 4); //2��2���ֽ�,16bit
            //SendFrame(EVENT_REPOT_CW, (uint8_t *)&WRITE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit    
        }
        #endif
        
    }

    FLASH_Lock() ;  //����

    if(data_pack_cur_num == DataPackAll - 1)  //�������
    {
        if((check_a == CHECK_A) && (check_b == CHECK_B))   //�������У������PC�˸�����У�������
        {
            event_report[0] = FIRMW_DATA_CRC_MATCH;
            SendFrame(EVENT_REPOT_CW, (uint8_t *)event_report, 4);   //2��2���ֽ�,16bit
            //SendFrame(EVENT_REPOT_CW, (uint8_t *)&FIRMW_DATA_CRC_MATCH, 4);   //2��2���ֽ�,16bit 
            //����λ
            
        }
        else
        {
            event_report[0] = FIRMW_DATA_CRC_ERR;
            SendFrame(EVENT_REPOT_CW, (uint8_t *)event_report, 4);   //2��2���ֽ�,16bit
            //SendFrame(EVENT_REPOT_CW, (uint8_t *)&FIRMW_DATA_CRC_ERR, 4);   //2��2���ֽ�,16bit
            //���Ѿ�д��Ĺ̼�����Ч������ֹ����Ĺ̼�����
            
        }

    }
    
    SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4);   //2��2���ֽ�,16bit
    
    return 1;
}
//======================================================================================================
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
int16_t  StartDesProc(COMM_FRAME *pcf)
{
    uint8_t    key_buff[20];
    uint8_t    i;
    uint8_t    match_key_state;
    
    CPU_INFO  CPUInfo;
    
    for(i = 0; i < pcf->len - 1; i++)
    {
         key_buff[i] = pcf->param[i + 1];      
    }
    
    //ƥ������
    match_key_state = MemoryCmp((uint8_t *)key_buff, (uint8_t *)mcu_key_buff, 16);
    if(match_key_state == TRUE)
    {
        //����Ӧ��֡MCUΨһ��ʶ��
        ReadCPUID(&CPUInfo);
        SendFrame(UNIPUE_ID_CW, (uint8_t *)&CPUInfo, 16);   //2��2���ֽ�,16bit
    }
    
    return 1;   
}
//======================================================================================================
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
int16_t  DesKeyProc(COMM_FRAME *pcf)
{
    uint8_t  encrypt_key[16];
    uint8_t  random_num[1008];
    uint16_t i;
    uint8_t  buff[4];
    
    //128bit�ļ�������
    for(i = 0; i < 16; i++)
    {
         encrypt_key[i] = pcf->param[i + 1];      
    }
    
    //1008bytes�������
    for(i = 0; i < 1008; i++)
    {
         random_num[i] = pcf->param[i + 17];      
    }
    
    //MCU ���ܲ���
    //.....
    
    
    
    //0      Ӧ��������
    //1      Ӧ����
    //2-3    Ӧ�����
    buff[0]  = DES_KEY_CW;
    buff[0]  = 0x00;  //��ȷӦ��
    buff[2]  = 0x00;
    buff[3]  = 0x00;
    SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4);   //2��2���ֽ�,16bit
    
    return 1;    
}




