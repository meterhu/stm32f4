//===============================================================================================
/************************************************************************************************
*                                                                                                     
*            文件名称:              
*
*                摘要: 
*
*            创建时间: 2012-05-20  
*
*            修改时间: 2012-05-20 
*            修改原因:
*
*            
*
*                作者: 杨卫华
*
*************************************************************************************************/
//===============================================================================================

#ifndef __FRAME_PACK_H
#define __FRAME_PACK_H

//=================================================================================================
//*************************************************************************************************
//宏定义

#define   FRM_HEAD1_FLG       0xAA
#define   FRM_HEAD2_FLG       0x75

#define  FRM_MAX_LEN          (0xFFFF)
#define  FRM_HEAD_SIZE         6   //包含帧头,命令字,长度字节,保留字节



#define FRM_CMD_REV            0x01
#define FRM_CMD_AD_V           0x02
#define FRM_CMD_STATE_WORD     0x03
#define FRM_CMD_TMPTR_V        0x08


#define FRM_CMD_OIL            0x03			  //测油量
#define FRM_CMD_POLLING        0x35			  //巡检
#define FRM_CMD_CYLINDER_SHELL 0x36			  //量筒去皮
#define FRM_CMD_CRC            0x37			  //校验

#define FRM_CMD_RUN            0x10
#define FRM_CMD_TMPTR_CTRL     0x11
#define FRM_CMD_FC_CTRL        0x12  //fan cover

#define FRM_CMD_RUN_SUB_CTRLW_STP  0x00
#define FRM_CMD_RUN_SUB_CTRLW_RUN  0x01
#define FRM_CMD_RUN_SUB_CTRLW_RST  0x02
#define FRM_CMD_RUN_SUB_PAUSE      0x03
#define FRM_CMD_RUN_SUB_PAUSE_RE   0x04
//#define FRM_CMD_RST_TEST       0x13  //

#define FRM_CMD_ACK_FRM        0x20  //应答帧 


#define FRM_ACK_FRM_STATE_OK    0x01
#define FRM_ACK_FRM_STATE_ERR   0x01


////#define COMM_UART   1
//=================================================================================================
//*************************************************************************************************
//数据类型定义
typedef __packed struct _FramePacket  
{
    uint8_t  hd1;		 //	帧头1
    uint8_t  hd2;        //帧头2
    uint8_t  cw;         //命令字
    //uint8_t  rsv;        //为了半字对齐

    uint8_t  frm_len_l;  //数据长度,即下面pb的长度,先低后高
    uint8_t  frm_len_h;
    uint8_t  rev_w;		 //保留字
    uint8_t  *pb;		 //数据缓冲区
    uint8_t  crc;		 //校验

}FRAME_PACKET;





//=================================================================================================
//*************************************************************************************************
//变量定义

//========================================================================================
//


//=================================================================================================
//*************************************************************************************************
//外部函数声明

extern  int16_t SendFrame(uint8_t cw, uint8_t *pb, uint16_t data_len);  
extern  int16_t SendAckFrame(uint8_t cw, uint8_t ack_state);
void 	uart3SendBytes(uint8_t const *pb,uint32_t num);
void 	USART3_SendByte(uint8_t dat);
void 	uart1SendBytes(uint8_t const *pb,uint32_t num);
void 	USART1_SendByte(uint8_t dat);
uint16_t MAKE_HWORD1(uint16_t frm_len_h,uint16_t frm_len_l);

#endif    //__FRAME_PACK_H
