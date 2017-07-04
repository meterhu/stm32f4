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

#ifndef __FRAME_PACK_H
#define __FRAME_PACK_H

//=================================================================================================
//*************************************************************************************************
//�궨��

#define   FRM_HEAD1_FLG       0xAA
#define   FRM_HEAD2_FLG       0x75

#define  FRM_MAX_LEN          (0xFFFF)
#define  FRM_HEAD_SIZE         6   //����֡ͷ,������,�����ֽ�,�����ֽ�



#define FRM_CMD_REV            0x01
#define FRM_CMD_AD_V           0x02
#define FRM_CMD_STATE_WORD     0x03
#define FRM_CMD_TMPTR_V        0x08


#define FRM_CMD_OIL            0x03			  //������
#define FRM_CMD_POLLING        0x35			  //Ѳ��
#define FRM_CMD_CYLINDER_SHELL 0x36			  //��ͲȥƤ
#define FRM_CMD_CRC            0x37			  //У��

#define FRM_CMD_RUN            0x10
#define FRM_CMD_TMPTR_CTRL     0x11
#define FRM_CMD_FC_CTRL        0x12  //fan cover

#define FRM_CMD_RUN_SUB_CTRLW_STP  0x00
#define FRM_CMD_RUN_SUB_CTRLW_RUN  0x01
#define FRM_CMD_RUN_SUB_CTRLW_RST  0x02
#define FRM_CMD_RUN_SUB_PAUSE      0x03
#define FRM_CMD_RUN_SUB_PAUSE_RE   0x04
//#define FRM_CMD_RST_TEST       0x13  //

#define FRM_CMD_ACK_FRM        0x20  //Ӧ��֡ 


#define FRM_ACK_FRM_STATE_OK    0x01
#define FRM_ACK_FRM_STATE_ERR   0x01


////#define COMM_UART   1
//=================================================================================================
//*************************************************************************************************
//�������Ͷ���
typedef __packed struct _FramePacket  
{
    uint8_t  hd1;		 //	֡ͷ1
    uint8_t  hd2;        //֡ͷ2
    uint8_t  cw;         //������
    //uint8_t  rsv;        //Ϊ�˰��ֶ���

    uint8_t  frm_len_l;  //���ݳ���,������pb�ĳ���,�ȵͺ��
    uint8_t  frm_len_h;
    uint8_t  rev_w;		 //������
    uint8_t  *pb;		 //���ݻ�����
    uint8_t  crc;		 //У��

}FRAME_PACKET;





//=================================================================================================
//*************************************************************************************************
//��������

//========================================================================================
//


//=================================================================================================
//*************************************************************************************************
//�ⲿ��������

extern  int16_t SendFrame(uint8_t cw, uint8_t *pb, uint16_t data_len);  
extern  int16_t SendAckFrame(uint8_t cw, uint8_t ack_state);
void 	uart3SendBytes(uint8_t const *pb,uint32_t num);
void 	USART3_SendByte(uint8_t dat);
void 	uart1SendBytes(uint8_t const *pb,uint32_t num);
void 	USART1_SendByte(uint8_t dat);
uint16_t MAKE_HWORD1(uint16_t frm_len_h,uint16_t frm_len_l);

#endif    //__FRAME_PACK_H
