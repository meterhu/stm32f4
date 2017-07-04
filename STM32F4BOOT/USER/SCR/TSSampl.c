//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            �ļ�����:              
 *
 *                ժҪ: 
 *
 *            ����ʱ��: 2008-10-10  
 *
 *            �޸�ʱ��: 2008-10-10
 *            �޸�ԭ��:
 *
 *            
 *
 *                ����: ������
 *
 *************************************************************************************************/
//===============================================================================================
#include "stm32f4xx.h"
#include "mystd.h"
#include "m_config.h"
#include "UserLib.h"
#include "m_config.h"
#include "LCD_RA8875.h"

#define TS_SAMP_CNT      9
#define SAMP_CNT_DIV2 2

extern __IO uint16_t s_usTouchX, s_usTouchY;	/* ������ADCֵ */

volatile uint16_t  ValidTS_X;
volatile uint16_t  ValidTS_Y;
extern void GUI_TOUCH_Exec1(unsigned int short  ts_x,unsigned int short ts_y);
extern uint16_t RA8875_TouchReadXY(void);

#define  TS_VALID_VALUE_DIFF   20
     
//================================================================================================
/*************************************************************************************************
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
 *************************************************************************************************/
int16_t TS_GetXY(void)
{
    uint8_t   cnt=0;
    uint8_t   i, j, k, min;
    uint16_t  temp;
    uint16_t  tempXY[2][9], XY[2];

    ValidTS_X = 0;
    ValidTS_Y = 0;    
    do
    {
        if(RA8875_TouchReadXY())
        {
            tempXY[0][cnt] = s_usTouchX;
            tempXY[1][cnt] = s_usTouchY;
            cnt++;
        }
        else
        {
            break;   
        }
        
    }while(cnt < 9);

    if(cnt == 9)
    {
        for(k = 0; k < 2; k++)
        { // ��������
            for(i = 0; i< cnt - 1; i++)
            {
                min = i;
                for (j = i + 1; j < cnt; j++)
                {
                    if (tempXY[k][min] > tempXY[k][j]) min=j;
                }
                
                temp = tempXY[k][i];
                tempXY[k][i] = tempXY[k][min];
                tempXY[k][min] = temp;
            }
            
            if ((tempXY[k][5] - tempXY[k][4])> TS_VALID_VALUE_DIFF)
            {
                return 0;
            }
            else
            {
            
                // ���м�ֵ�ľ�ֵ
                XY[k] = (tempXY[k][3] + tempXY[k][4] + tempXY[k][5] + tempXY[k][6]) / 4;
            }
        }

        // ��������
        ValidTS_X = XY[0];//((XY[0]-350)/11);
        ValidTS_Y = XY[1];//((XY[1]-400)/14);

        return 1;
    }

    return 0;
  
}

//================================================================================================
/*************************************************************************************************
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
 *************************************************************************************************/
void  TS_Task(void)
{
    TS_GetXY();

    GUI_TOUCH_Exec1(ValidTS_X,ValidTS_Y);
       
}   



