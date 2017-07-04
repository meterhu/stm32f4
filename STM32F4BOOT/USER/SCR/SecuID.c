//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            文件名称:              
 *
 *                摘要: 
 *
 *            创建时间: 2008-01-17  
 *
 *            修改时间: 2008-01-17
 *            修改原因:
 *
 *            
 *
 *                作者: 杨卫华
 *
 *************************************************************************************************/
 //===============================================================================================
#include <stdio.h>
#include <stdarg.h>
#include <absacc.h>
#include "mystd.h"
#include "m_config.h"
#include "UserLib.h"
#include "Des.h"
#include "SecuID.h"
#include "CPUID.h"
//================================================================================================


_BYTE SeucIDEncryptKey[]={133,73,86,94,83,24,74,21,163,82,163,82,64,134,222};
UINT16  FactSecuIDBuff[SECU_ID_HWORD_SIZE];


#define  RefreshFactSecuIDBuff()     GetFactSecuID(FactSecuIDBuff)   
//================================================================================================
 
 

//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: GetFactSecuID
 *
 *         函数功能: 获取厂商安全号
 *
 *         入口参数: 
 *                  pfact_secu_id_buff :厂商安全号缓冲区 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void GetFactSecuID(UINT16 *pfact_secu_id_buff)
{
    CPU_INFO  cpu_id;
    
    //96位
    //12字节
    //6个半字
    ReadCPUID(&cpu_id);
    
    _memcpy(pfact_secu_id_buff, &cpu_id,12);
    
    ///SecIDQuery(pfact_secu_id_buff, puser_secu_id_buff);

}


//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: EncryptFactSecuID
 *
 *         函数功能: 加密厂商安全号
 *
 *         入口参数: 
 *                  pencrypt_secu_id:预加密的安全号数据 缓冲区
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void  EncryptFactSecuID(UINT16 *pencrypt_secu_id)
{
     UINT16 fact_secu_id_buff[SECU_ID_HWORD_SIZE];
     
     GetFactSecuID(fact_secu_id_buff);
     
     Des_Go((_BYTE *)pencrypt_secu_id, (_BYTE *)fact_secu_id_buff, SECU_ID_BYTE_SIZE, \
           SeucIDEncryptKey, sizeof(SeucIDEncryptKey), ENCRYPT);

}


//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: DecryptFactSecuID
 *
 *         函数功能: 解密厂商安全号
 *
 *         入口参数: 
 *                  pencrypt_fact_secu_id_buff:加密数据缓冲区
 *                  pdecrypt_fact_secu_id     :解密数据缓冲区
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void  DecryptFactSecuID(UINT16 *pencrypt_fact_secu_id_buff, UINT16 *pdecrypt_fact_secu_id)
{
     Des_Go((_BYTE *)pdecrypt_fact_secu_id, (_BYTE *)pencrypt_fact_secu_id_buff, SECU_ID_BYTE_SIZE, \
            SeucIDEncryptKey, sizeof(SeucIDEncryptKey), DECRYPT);

}



//================================================================================================
/*************************************************************************************************
 *
 *         函数名称: DecryptFactSecuID
 *
 *         函数功能: 解密厂商安全号
 *
 *         入口参数: 
 *                  pfact_secu_id_buff:厂商ID
 *
 *         出口参数: 
 *                  0  : 校验正确
 *                  -1 : 校验错误
 *
 *             说明:  兼容局部变量参数，这样安全性更高，因为局部变量不用时就释放掉了
 *
 *************************************************************************************************/
INT16  CheckFactSecuIDValid(UINT16 *pfact_secu_id_buff)
{
     UINT8  i;
     UINT16 sum;
     
     
     UINT16 saved_secu_id_buff[SECU_ID_HWORD_SIZE];
     UINT16 decrypt_fact_secu_id_buff[SECU_ID_HWORD_SIZE];
     
     //从flash中读取安全id
   ////  FlashReadHWord(SECU_ID1_ADDR,  saved_secu_id_buff, SECU_ID_HWORD_SIZE / 2);
   ////  FlashReadHWord(SECU_ID2_ADDR,  saved_secu_id_buff + SECU_ID_HWORD_SIZE / 2, SECU_ID_HWORD_SIZE / 2);
    
   ////  FlashReadHWord(SECU_ID_SUM_ADDR,  &sum, 1);

     //验证id的校验和
     for (i = 0; i < SECU_ID_HWORD_SIZE; i++)
     {
         sum -= saved_secu_id_buff[i];
     }
     
     if (sum != 0)
     {
         return -1;
     
     }
     
     //解密预存的安全id
     Des_Go((_BYTE *)decrypt_fact_secu_id_buff, (_BYTE *)saved_secu_id_buff, SECU_ID_BYTE_SIZE,
            SeucIDEncryptKey, sizeof(SeucIDEncryptKey), DECRYPT);
            
          
     //解密后的预存的id和实际id比较验证
     for (i = 0; i < SECU_ID_HWORD_SIZE; i++)
     {
        if (decrypt_fact_secu_id_buff[i] != *pfact_secu_id_buff++)
        {
            return -1;
        }
     
     }
     
     return 0;
     
} 
