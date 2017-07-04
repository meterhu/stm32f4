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
 *
 *******************************************************************************************************/
//======================================================================================================
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mystd.h"
#include "includes.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"
#include "LCD_RA8875.h"
#include "LCDConf.h"
#include "tasks.h"
#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "TEXT.h"
#include "ID.h"
#include "OD_BUTTON.h"
#include "RTC.h"
#include "UserLib.h"
#include "TSCalibrate.h"
#include "TSOriginCor.h"


WM_HWIN hTSCalibDlg = NULL;
extern WM_HWIN hMain;
POINT  TsDrawFocusPoint[4] =  //z������
{
    {TP_X1, TP_Y1},
    {TP_X2, TP_Y2},
    {TP_X3, TP_Y3},
    {TP_X4, TP_Y4},
    
};

int TOUCH_Calibration(void);
extern OS_EVENT  *TS_SYN;


//uint8_t  EEPROM_DMA_Buff[16]  __attribute__ ((at(0x20001000)));


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
GUI_WIDGET_CREATE_INFO _aTSCalibDialogCreate[] = 
{
    {FRAMEWIN_CreateIndirect,       "KeyBoard",   GUI_ID_TS_CLB_DLG,             0, 0, LCD_XSIZE,LCD_YSIZE,FRAMEWIN_CF_MOVEABLE|FRAMEWIN_CF_ACTIVE,0},
		
};


extern __IO uint16_t s_usTouchX, s_usTouchY;



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
static void _TSCalibCallBkWindow(WM_MESSAGE* pMsg)
{
	switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_BLUE);
            GUI_Clear();
            
            break;
        default:
            WM_DefaultProc(pMsg);
            break;
    }
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
void TSCalibDlg(void)
{   
    ///WM_SetDesktopColor(GUI_WHITE);      // Automacally update desktop window 
    WM_SetCreateFlags(WM_CF_MEMDEV);    //Use memory devices on all windows to avoid flicker 
    ///GUI_Clear();
    
    hTSCalibDlg = GUI_CreateDialogBox(_aTSCalibDialogCreate, GUI_COUNTOF(_aTSCalibDialogCreate), &_TSCalibCallBkWindow, WM_HBKWIN, 0, 0);
    FRAMEWIN_SetTitleVis(hTSCalibDlg,0);
    FRAMEWIN_SetTitleHeight(hTSCalibDlg,0);
    
    OSTaskSuspend(APP_TASK_START_PRIO_1);
      
    WM_Exec();
    TOUCH_Calibration();
    GUI_EndDialog(hTSCalibDlg,0);
    
    OSTaskResume(APP_TASK_START_PRIO_1);
 

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
void DrawTSCalibrFocus(POINT *pp, uint32_t color)
{
    //GUI_SetColor(color);
    
//     GUI_DrawLine(pp->x - 12,pp->y, pp->x + 13, pp->y); //����
//     GUI_DrawLine(pp->x, pp->y - 12, pp->x,pp->y + 13); //����
//     GUI_DrawCircle(pp->x, pp->y, 8);           //������Ȧ
    RA8875_ClrScr(BLUE);  
    RA8875_DrawLine(pp->x - 12,pp->y, pp->x + 13, pp->y,color); //����
    RA8875_DrawLine(pp->x, pp->y - 12, pp->x,pp->y + 13,color); //����
    RA8875_DrawCircle(pp->x, pp->y, 8,color);           //������?
    
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
int16_t TS_CalibGetXY(POINT *pp)
{
    uint32_t   cnt=0;
    uint32_t   ad_cnt = 0;
    uint8_t   i, j, k, min;
    uint16_t  temp;
    uint16_t  tempXY[2][30];
    uint16_t  XY[2];
   
 
    while(RA8875_TouchReadXY())  //���һֱ������һֱ����
    {
        tempXY[0][cnt] = s_usTouchX;
        tempXY[1][cnt] = s_usTouchY;
        cnt++;
        if (cnt >= 20)
        {
           cnt = 0;
        }
        printf("x = %d,y=%d\n",s_usTouchX,s_usTouchY);   
        ad_cnt++;
        OSTimeDlyHMSM(0, 0, 0, 80); //20���� * 100ms=2������
            
    }        
        
    
    //ƽ���˲�
     if(ad_cnt >= 20)
     {
        ad_cnt  = 20; 
        for(k = 0; k < 2; k++)
        { // ��������
            for(i = 0; i< ad_cnt - 1; i++)
            {
                min = i;
                for (j = i + 1; j < ad_cnt; j++)
                {
                    if (tempXY[k][min] > tempXY[k][j]) min=j;
                }  
                
                temp = tempXY[k][i];
                tempXY[k][i] = tempXY[k][min];
                tempXY[k][min] = temp;
            }
            
            if ((tempXY[k][10] - tempXY[k][11])> TS_CALIB_VALID_VALUE_DIFF)
            {
                return 0;
            }
            else
            {
                // ���м�ֵ�ľ�ֵ
                XY[k] = (tempXY[k][9] + tempXY[k][10] + tempXY[k][11] + tempXY[k][12]) / 4;
            }
            
        }

        // ��������
        pp->x = XY[0];  //((XY[0]-350)/11);
        pp->y = XY[1];  //((XY[1]-400)/14);

        return 1;
        
    }

    return 0;
  
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
int TOUCH_Calibration(void)
{
	int      i;
    int      rv;
    int      flg;
    POINT    TSADValue[4];
    TS_COR   ts_ad;
    TS_COR   tscor;
    float    xfac;
    float    yfac;
    ////INT8U  err;

   
    GUI_Clear();
    
    do
    {
        DrawTSCalibrFocus(&TsDrawFocusPoint[0],GUI_WHITE);
        
        for (i = 0; i < 4;) //4��У��
        {
            //OSSemPend(TS_SYN, 0, &err);
            rv = TS_CalibGetXY(&TSADValue[i]);
            
            if(rv) 
            {             
                DrawTSCalibrFocus(&TsDrawFocusPoint[i],BLUE); //������һ�����
                
                if (i < 3)//ע�����һ������У����ʶ
                {
                    DrawTSCalibrFocus(&TsDrawFocusPoint[i + 1],WHITE); //��������
                }
                i++;
            }
            else
            {
                //��Ч����ʾ
                //printf("invalid ts point\n");   
            }       
            
        }
        
        flg = 0;
        if (ABS(TSADValue[0].x - TSADValue[2].x) > 30) //��1����͵�3������ͬx��Ļ��������ȡ����ADֵ
        {  
            flg = 1;
        }
        
        if (ABS(TSADValue[0].y - TSADValue[1].y) > 30) //��1����͵�2������ͬy��Ļ��������ȡ����ADֵ
        {  
            flg = 1;
        }
        
        if (ABS(TSADValue[1].x - TSADValue[3].x) > 30) //��2����͵�4������ͬx��Ļ��������ȡ����ADֵ
        {  
            flg = 1;
        }
        
        if (ABS(TSADValue[2].y - TSADValue[3].y) > 30) //��3����͵�4������ͬy��Ļ��������ȡ����ADֵ
        {  
            flg = 1;
        }
                
                
    
    }while(flg);
    
    //��ͬ������ƽ��
    ts_ad.x0 = (TSADValue[0].x + TSADValue[2].x) / 2;  //x0
    ts_ad.x1 = (TSADValue[1].x + TSADValue[3].x) / 2;  //x1
    ts_ad.y0 = (TSADValue[0].y + TSADValue[1].y) / 2;  //y0
    ts_ad.y1 = (TSADValue[2].y + TSADValue[3].y) / 2;  //y1
    
    //AD����Ļ��Ӧ�ı�������
    xfac = (float)(ABS(ts_ad.x1 -  ts_ad.x0)) / (float)(ABS(TP_X2 - TP_X1));
	yfac = (float)(ABS(ts_ad.y1 -  ts_ad.y0)) / (float)(ABS(TP_Y3 - TP_Y1));
	
    //У���㵹����Ļ4����������ֵ(xMin,xMax,yMin,yMax)
    tscor.x0 = ts_ad.x0 - (int16_t)(TP_X1 * xfac);
    tscor.x1 = ts_ad.x1 + (int16_t)(TP_X1 * xfac);
    tscor.y0 = ts_ad.y0 - (int16_t)(TP_Y1 * yfac);
    tscor.y1 = ts_ad.y1 + (int16_t)(TP_Y1 * yfac);
     
    //����ֵ
    SaveTSCalibrateValue(&tscor);
    
    
    //���¼�������ֵ
    GUI_TOUCH_LoadCalibration(tscor.x0, tscor.x1,tscor.y0, tscor.y1);
       
    
    return 1;  
    
    
}










