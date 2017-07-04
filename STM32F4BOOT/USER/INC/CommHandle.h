//===============================================================================================
/************************************************************************************************
*                                                                                                     
*            �ļ�����:              
*
*                ժҪ: 
*
*            ����ʱ��: 2012-05-20  
*
*            �޸�ʱ��: 2012-05-20 
*            �޸�ԭ��:
*
*            
*
*                ����: ������
*
*************************************************************************************************/
//===============================================================================================

#ifndef __COMM_HANDLE_H
#define __COMM_HANDLE_H


//=================================================================================================
//*************************************************************************************************
//�궨��
#define   MAX_FRAME_SIZE      2500
#define   FRAME_DATA_SIZE     (MAX_FRAME_SIZE - 6 - 1)   //6


#define   SEND_DATA_FUNC      0x01
#define   SEND_PWD_FUNC       0x02

//=================================================================================================
#define   FRM_NO_STATE        0x01
#define   FRM_HEAD1_STATE     0x02
#define   FRM_HEAD2_STATE     0x04
#define   FRM_FUNC_STATE      0x08
#define   FRM_RESV_STATE      0x10
#define   FRM_LENL_STATE      0x20
#define   FRM_LENH_STATE      0x40
#define   FRM_DATA_STATE      0x80
#define   FRM_CRC_STATE       0x22


#define   FRM_OCCURHEAD1_STATE      0x01
#define   FRM_OCCURHEAD2_STATE      0x02 
#define   FRM_SEC_OCCU_STATE        0x04 

#define   FRM_HEAD_FIRST_OCCUR    (FRM_NO_STATE | FRM_HEAD1_FLG)

#define   FRM_NO_ERR              0
#define   FRM_MAX_LEN_ERR        0x0100
#define   FRM_CRC_ERR            0x0200
#define   FRM_ERR_AND_NO_HEAD    0x0400





#define  MAX_UART_RX_Q_SIZE    0xFF

//=================================================================================================
//������
#define  START_CNT    0x01
#define  OVER_CNT     0x02
#define  RUN          0x03
#define  PREPARE      0x04
#define  END_CNT      0x08

////������

//--------------------------------�ϴ�------------------------------------------------
#define  UNIVER_ACK_CW            0x01   //ͨ��Ӧ��֡
#define  UNIPUE_ID_CW             0x02   //MCUΨһ��ʶ������֡
#define  MCU_INFO_CW              0x03   //MCU��Ϣ����֡
#define  EVENT_REPOT_CW           0x04   //�¼���������֡

//-------------------------------�´�------------------------------------------------
#define  START_FIRE_WARE_UPDATE_CW   0x40   //�����̼���������֡
#define  FIRMW_DATA_CW               0x41   //�̼�����֡
#define  STA_DES_CW                  0x42   //������������֡
#define  DES_KEY_CW                  0x43   //������������֡
#define  FLASH_WRP_LOCK_CW           0x64   //Flash ��д����
#define  FLASH_WRP_UNLOCK_CW         0x65   //Flash ��д����
#define  SYS_RESET_CW                0x66   //MCU��λ����֡
#define  BAUD_RATE_CW                0x67   //���ڲ���������

//-------------------------------������------------------------------------------------
#define  ERASE_SECTOR_ERR         0x01   //������������
#define  WRITE_SECTOR_ERR         0x02   //����д�����
#define  FIRMW_DATA_CRC_ERR       0x03   //�̼�����У�������
#define  FIRMW_DATA_CRC_MATCH     0x04   //�̼�����У�������
#define  FIRMW_DATA_WRITE_SUCCES  0x05   //�̼�����У�������
#define  OVER_FLASH_SIZE          0x06   //����flash��Χ
#define  ERASE_SECTOR_SUCCESS     0x07   //���������ɹ�
#define  KEY_CODE_MATCH_ERR       0x08   //����ƥ�����
#define  UART_BAUD_RATE_SUCCESS   0x09   //���ڲ��������óɹ�
#define  APP_MATCH_BOOT_ERR       0x10   //APP.bin�ļ���Ч

//================================================================================================
#define  OPEN_OIL_BOARD()     OUT(1, 0x00) 
#define  CLOSE_OIL_BOARD()    OUT(1, 0xff) 


#define  MCU_ID              0
#define  FLASH_SIZE          0xD8000    // 16K + 16K + 64K + 128K * 6
#define  SECTOR_NUM          9

#define  STORE_KEY_ADDR     0x08008000 
#define  STORE_KEY_BYTES     2048

#define  APP1_START_ADDR    0x0800C000  
#define  APP_RESV_BYTES     512

#define  HEADER_FLAG_LEN    16


#define  APP_VALID_ADDR     APP1_START_ADDR + APP_RESV_BYTES 

#define APP_VALID_FLAG_BYTE1  0xAA
#define APP_VALID_FLAG_BYTE2  0x55
#define APP_VALID_FLAG_BYTE3  0xA5
#define APP_VALID_FLAG_BYTE4  0x5A

#define KEY_LEN        16


//=================================================================================================
//*************************************************************************************************
//�������Ͷ���

typedef  struct  __CommFrame
{

    uint16_t  len;
    uint16_t  cur_i;  
    uint8_t   *phd_occur;
    uint8_t   state;
    uint8_t   hd_occur_cnt;
    uint8_t   hd_occur_flg;
    uint8_t   crc;
    uint8_t   func;   
    int8_t    err; 
    
    uint8_t   param[FRAME_DATA_SIZE];
    uint8_t   *p;

   

}COMM_FRAME;

typedef struct _COMM_PROC
{
    uint8_t  comm_c_w;
    int16_t  (*proc)(COMM_FRAME *pcf);
   
}COMM_PROC;


typedef __packed struct _FUEL_INJE_COMM_RAIL_CTRL       
{
     uint16_t  ctrl_w;
     uint16_t  fre;
     uint16_t  duty_cycle;
     uint16_t  pulse_w;
     uint16_t  pressure;
     uint16_t  test_tm_s;

}FUEL_INJE_COMM_RAIL_CTRL;




//=================================================================================================
//*************************************************************************************************
//��������

//========================================================================================
//


//=================================================================================================
//*************************************************************************************************
//�ⲿ��������

extern    void InitComm(void);
//extern    void  FrameParseTask(void *p_arg);
extern void  FrameParseExec(void);
extern __asm void SystemReset(void);
extern __asm void RunApp(void);

#endif    //__COMM_HANDLE_H
