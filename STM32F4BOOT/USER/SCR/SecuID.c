//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            �ļ�����:              
 *
 *                ժҪ: 
 *
 *            ����ʱ��: 2008-01-17  
 *
 *            �޸�ʱ��: 2008-01-17
 *            �޸�ԭ��:
 *
 *            
 *
 *                ����: ������
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
 *         ��������: GetFactSecuID
 *
 *         ��������: ��ȡ���̰�ȫ��
 *
 *         ��ڲ���: 
 *                  pfact_secu_id_buff :���̰�ȫ�Ż����� 
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
 *
 *************************************************************************************************/
void GetFactSecuID(UINT16 *pfact_secu_id_buff)
{
    CPU_INFO  cpu_id;
    
    //96λ
    //12�ֽ�
    //6������
    ReadCPUID(&cpu_id);
    
    _memcpy(pfact_secu_id_buff, &cpu_id,12);
    
    ///SecIDQuery(pfact_secu_id_buff, puser_secu_id_buff);

}


//================================================================================================
/*************************************************************************************************
 *
 *         ��������: EncryptFactSecuID
 *
 *         ��������: ���ܳ��̰�ȫ��
 *
 *         ��ڲ���: 
 *                  pencrypt_secu_id:Ԥ���ܵİ�ȫ������ ������
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
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
 *         ��������: DecryptFactSecuID
 *
 *         ��������: ���ܳ��̰�ȫ��
 *
 *         ��ڲ���: 
 *                  pencrypt_fact_secu_id_buff:�������ݻ�����
 *                  pdecrypt_fact_secu_id     :�������ݻ�����
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
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
 *         ��������: DecryptFactSecuID
 *
 *         ��������: ���ܳ��̰�ȫ��
 *
 *         ��ڲ���: 
 *                  pfact_secu_id_buff:����ID
 *
 *         ���ڲ���: 
 *                  0  : У����ȷ
 *                  -1 : У�����
 *
 *             ˵��:  ���ݾֲ�����������������ȫ�Ը��ߣ���Ϊ�ֲ���������ʱ���ͷŵ���
 *
 *************************************************************************************************/
INT16  CheckFactSecuIDValid(UINT16 *pfact_secu_id_buff)
{
     UINT8  i;
     UINT16 sum;
     
     
     UINT16 saved_secu_id_buff[SECU_ID_HWORD_SIZE];
     UINT16 decrypt_fact_secu_id_buff[SECU_ID_HWORD_SIZE];
     
     //��flash�ж�ȡ��ȫid
   ////  FlashReadHWord(SECU_ID1_ADDR,  saved_secu_id_buff, SECU_ID_HWORD_SIZE / 2);
   ////  FlashReadHWord(SECU_ID2_ADDR,  saved_secu_id_buff + SECU_ID_HWORD_SIZE / 2, SECU_ID_HWORD_SIZE / 2);
    
   ////  FlashReadHWord(SECU_ID_SUM_ADDR,  &sum, 1);

     //��֤id��У���
     for (i = 0; i < SECU_ID_HWORD_SIZE; i++)
     {
         sum -= saved_secu_id_buff[i];
     }
     
     if (sum != 0)
     {
         return -1;
     
     }
     
     //����Ԥ��İ�ȫid
     Des_Go((_BYTE *)decrypt_fact_secu_id_buff, (_BYTE *)saved_secu_id_buff, SECU_ID_BYTE_SIZE,
            SeucIDEncryptKey, sizeof(SeucIDEncryptKey), DECRYPT);
            
          
     //���ܺ��Ԥ���id��ʵ��id�Ƚ���֤
     for (i = 0; i < SECU_ID_HWORD_SIZE; i++)
     {
        if (decrypt_fact_secu_id_buff[i] != *pfact_secu_id_buff++)
        {
            return -1;
        }
     
     }
     
     return 0;
     
} 
