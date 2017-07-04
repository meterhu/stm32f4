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
#include "UART.h"
#include "FramePack.h"


#if (COMM_UART == 0)
#define  UARTS_BYTES(pb, num)   UART_SendBytes(USART1,pb,num)
#elif (COMM_UART == 1)
#define  UARTS_BYTES(pb, num)   UART_SendBytes(USART2,pb,num)
#endif



//=================================================================================================
//帧格式:
//0xAA 0x70 命令字(1 byte)  保留(1 byte)  数据长度低字节    数据长度高字节   数据 ...  CRC(1byte)
//其中 数据长度为16位,其后用户数据的长度
//CRC为其前所有字节的异或
//=================================================================================================


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
uint8_t CalculateCRC(FRAME_PACKET *p_fp, uint16_t date_len)
{
    uint16_t i;
    uint8_t *pb;
    uint8_t crc;

    pb = (uint8_t *)p_fp;  //注意p_fp必须字节对齐,即要注意结构体的填充

    crc = 0;

    for (i = 0; i < FRM_HEAD_SIZE; i++) {
        crc  ^= *pb++;

    }

    pb = p_fp->pb;

    for (i = 0; i < date_len; i++) {
        crc  ^= *pb++;

    }

    return crc;


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
void PackFrame(FRAME_PACKET *fp, uint8_t cw, uint8_t *pb, uint16_t data_len)
{

    //FRAME_PACKET  fp; //最好是动态分配,然后返回动态分配的内存地址

    fp->hd1 =  FRM_HEAD1_FLG;
    fp->hd2 =  FRM_HEAD2_FLG;

    fp->cw  = cw;
    //fp->rsv = 0;
    fp->frm_len_l = data_len & 0xFF;
    fp->frm_len_h = (data_len >> 8)& 0xFF;

    fp->pb        = pb;

    fp->crc =  CalculateCRC(fp, data_len);

    //return 1;

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
int16_t UARTSendFrame(FRAME_PACKET *p_fp)
{


    UARTS_BYTES((uint8_t *)p_fp, FRM_HEAD_SIZE);
    UARTS_BYTES(p_fp->pb, MAKE_HWORD(p_fp->frm_len_h,p_fp->frm_len_l));
    UARTS_BYTES(&(p_fp->crc), 1);


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
int16_t SendFrame(uint8_t cw, uint8_t *pb, uint16_t data_len)
{
    FRAME_PACKET  fp;

    //包装帧,注意这里无须进行互斥,其实不同任务同时使用该函数还不存在重入为问题,
    //因为任务堆栈不同(注意临时变量分配在任务堆栈中).
    fp.hd1 =  FRM_HEAD1_FLG;
    fp.hd2 =  FRM_HEAD2_FLG;

    fp.cw  = cw;
    //fp.rsv = 0xAF;
    fp.frm_len_l = data_len & 0xFF;
    fp.frm_len_h = (data_len >> 8)& 0xFF;

    fp.pb        = pb;

    fp.crc =  CalculateCRC(&fp, data_len);

    //PackFrame(&fp,cw,pb,data_len);

    //发送帧
    return (UARTSendFrame(&fp));

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
int16_t SendAckFrame(uint8_t cw, uint8_t ack_state)
{
    uint8_t ack_buff[2];

    ack_buff[0] = cw;
    ack_buff[1] = ack_state;


    return (SendFrame(FRM_CMD_ACK_FRM, ack_buff, 2));
    


}

