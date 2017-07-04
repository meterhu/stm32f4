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
 *
 *******************************************************************************************************/
//======================================================================================================
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mystd.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rtc.h"
#include "LCD_RA8875.h"
#include "LCDConf.h"
#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "TEXT.h"
#include "ID.h"
#include "OD_BUTTON.h"
#include "RTC.h"
#include  "TSCalibrate.h"


//========================================================================
#define  DATE_LEFT_SPACE  40
#define  YMD_HMS_SPACE    5
#define  DATE_TIME_SPACE  40
#define  DATA_TIME_TOP    140


//========================================================================
// extern GUI_CONST_STORAGE GUI_BITMAP bmadd_d;
// extern GUI_CONST_STORAGE GUI_BITMAP bmadd_u;
// extern GUI_CONST_STORAGE GUI_BITMAP bmsub_d;
// extern GUI_CONST_STORAGE GUI_BITMAP bmsub_u;
// extern GUI_CONST_STORAGE GUI_BITMAP bmset_u;
// extern GUI_CONST_STORAGE GUI_BITMAP bmset_d;
// extern GUI_CONST_STORAGE GUI_BITMAP bmtm_label;
// extern GUI_CONST_STORAGE GUI_BITMAP bmtextTD;
// extern GUI_CONST_STORAGE GUI_BITMAP bmTS_d;
// extern GUI_CONST_STORAGE GUI_BITMAP bmTS_u;

WM_HWIN hSetDlg = NULL;
extern WM_HWIN hMain;

extern GUI_CONST_STORAGE GUI_BITMAP bmnum0_down;
extern GUI_CONST_STORAGE GUI_BITMAP bmnum0_up;

///GUI_CONST_STORAGE GUI_BITMAP  *SetTimeBmp[]={&bmadd_u, &bmadd_d, &bmtextTD, &bmtextTD, &bmsub_u, &bmsub_d};

int DataTimeBtnID[]=
{
     //年月日(YMD)
    GUI_ID_BUTTON_DATA_SET_MA, GUI_ID_BUTTON_DATA_SET_MTEXT, GUI_ID_BUTTON_DATA_SET_MS, 
    GUI_ID_BUTTON_DATA_SET_DA, GUI_ID_BUTTON_DATA_SET_DTEXT, GUI_ID_BUTTON_DATA_SET_DS, 
    GUI_ID_BUTTON_DATA_SET_YA, GUI_ID_BUTTON_DATA_SET_YTEXT, GUI_ID_BUTTON_DATA_SET_YS, 
    
    //时分秒(HMS)
    GUI_ID_BUTTON_TIME_SET_HA, GUI_ID_BUTTON_TIME_SET_HTEXT, GUI_ID_BUTTON_TIME_SET_HS,
    GUI_ID_BUTTON_TIME_SET_MA, GUI_ID_BUTTON_TIME_SET_MTEXT, GUI_ID_BUTTON_TIME_SET_MS,
    GUI_ID_BUTTON_TIME_SET_SA, GUI_ID_BUTTON_TIME_SET_STEXT, GUI_ID_BUTTON_TIME_SET_SS,
    
    
};


DATE_TIME CurDT;
extern void  SetRTCTime(DATE_TIME *ptm);

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
GUI_WIDGET_CREATE_INFO _aSetDialogCreate[] = {
		{ FRAMEWIN_CreateIndirect,       "",   GUI_ID_SET_DLG,             0, 0, LCD_XSIZE,LCD_YSIZE,FRAMEWIN_CF_MOVEABLE|FRAMEWIN_CF_ACTIVE,0},
		
    	//年月日:
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_DATA_SET_MA,        0,0,0,0, 0,0},
		{OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_DATA_SET_MTEXT,     0,0,0,0, 0,0},
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_DATA_SET_MS,        0,0,0,0, 0,0},
        
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_DATA_SET_DA,        0,0,0,0, 0,0},
		{OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_DATA_SET_DTEXT,     0,0,0,0, 0,0},
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_DATA_SET_DS,        0,0,0,0, 0,0},
        
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_DATA_SET_YA,        0,0,0,0, 0,0},
		{OD_BUTTON_CreateIndirect,      "2012",       GUI_ID_BUTTON_DATA_SET_YTEXT,     0,0,0,0, 0,0},
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_DATA_SET_YS,        0,0,0,0, 0,0},
        
        //时分秒
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_HA,        0,0,0,0, 0,0},
		{OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_HTEXT,     0,0,0,0, 0,0},
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_HS,        0,0,0,0, 0,0},
        
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_MA,        0,0,0,0, 0,0},
		{OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_MTEXT,     0,0,0,0, 0,0},
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_MS,        0,0,0,0, 0,0},
        
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_SA,        0,0,0,0, 0,0},
		{OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_STEXT,     0,0,0,0, 0,0},
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_SS,        0,0,0,0, 0,0},
         
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_OK,        0,0,0,0, 0,0},
        {OD_BUTTON_CreateIndirect,      "",           GUI_ID_BUTTON_TIME_SET_CANCEL,    0,0,0,0, 0,0},
        
        
        {TEXT_CreateIndirect,           "0000",       GUI_ID_TEXT_TIME_SET_DATE,        0,0,0,0, 0,0},
        {TEXT_CreateIndirect,           "0000",       GUI_ID_TEXT_TIME_SET_TIME,        0,0,0,0, 0,0},    
};


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
void InitSetDlg(WM_HWIN hWin)
{
    
    int i;
    GUI_WIDGET_CREATE_INFO *pC; 
    char  str_buff[20];
    RTC_TimeTypeDef    time;
    RTC_DateTypeDef    date;
    
    //FRAMEWIN_SetClientColor(hWin,RGB(185,205,232));
    FRAMEWIN_SetClientColor(hWin,RGB(70,77,106));
    FRAMEWIN_SetTitleVis(hWin,0);
    FRAMEWIN_SetTitleHeight(hWin,0);
    
    //YMD
    pC = &_aSetDialogCreate[1];
    for (i = 0; i < 3; i++)
    {
      
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),0,&bmadd_u);
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),1,&bmadd_d);
        pC++;
             
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),0,&bmtextTD);
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),1,&bmtextTD);  
        BUTTON_SetTextColor(WM_GetDialogItem(hWin,pC->Id),BUTTON_CI_PRESSED,  GUI_BLACK);
        BUTTON_SetFont(WM_GetDialogItem(hWin,pC->Id),&GUI_FontD32);        
        pC++;

        
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),0,&bmsub_u);
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),1,&bmsub_d);
        pC++;
             
    }
    
        
    for (i = 0; i < 3; i++)
    {
    
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),0,&bmadd_u);
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),1,&bmadd_d);
        pC++;
        
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),0,&bmtextTD);
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),1,&bmtextTD);
        BUTTON_SetTextColor(WM_GetDialogItem(hWin,pC->Id),BUTTON_CI_PRESSED,  GUI_BLACK);
        BUTTON_SetFont(WM_GetDialogItem(hWin,pC->Id),&GUI_FontD32);   
        pC++;
      
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),0,&bmsub_u);
        OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),1,&bmsub_d);
        pC++;             
             
    }
  
    //OK
    OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),0,&bmset_u);
    OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),1,&bmset_d);
    pC++;   
    
    //Touch Ajust
    OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),0,&bmTS_u);
    OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,pC->Id),1,&bmTS_d);
    
    //YMD
    sprintf(str_buff, "20%02d", CurDT.year);
    BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON_DATA_SET_YTEXT),str_buff);

    sprintf(str_buff, "%02d", CurDT.mon);
    BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON_DATA_SET_MTEXT),str_buff);

    sprintf(str_buff, "%02d", CurDT.day);
    BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON_DATA_SET_DTEXT),str_buff);

    //HMS
    sprintf(str_buff, "%02d", CurDT.hour);
    BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_HTEXT),str_buff);
    
    sprintf(str_buff, "%02d", CurDT.min);
    BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_MTEXT),str_buff);
    
    sprintf(str_buff, "%02d", CurDT.sec);
    BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_STEXT),str_buff);
    
    //OK,cancel按钮
//     BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_CANCEL),BUTTON_CI_PRESSED,  GUI_BLACK);
//     BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_CANCEL),BUTTON_CI_UNPRESSED,  GUI_BLACK);
//     BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_CANCEL),&GUI_Font32_ASCII); 
//     BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_CANCEL),"TS Calibrate");      
      
//     BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_OK),BUTTON_CI_PRESSED,  GUI_BLACK);
//     BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_OK),BUTTON_CI_UNPRESSED,  GUI_BLACK);
//     BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_OK),&GUI_Font32_ASCII); 
//     BUTTON_SetText(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_SET_OK),"Set");   
    
    //日期显示
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_TIME_SET_DATE), GUI_WHITE);    
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_TIME_SET_DATE),&GUI_Font32_ASCII); 
   
    RTC_GetDate(RTC_Format_BIN,&date);
    sprintf(str_buff, "Date  %02d-%02d-20%02d", date.RTC_Month,date.RTC_Date,date.RTC_Year);
    TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT_TIME_SET_DATE), str_buff);
    
     //时间显示
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_TIME_SET_TIME), GUI_WHITE);    
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_TIME_SET_TIME),&GUI_Font32_ASCII);

    RTC_GetTime(RTC_Format_BIN,&time);
    sprintf(str_buff, "Time  %02d:%02d:%02d", time.RTC_Hours,time.RTC_Minutes,time.RTC_Seconds);
    TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT_TIME_SET_TIME), str_buff);    
    
    
    //OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_OUTPUT),0,&bmnum0_up);
    //OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_OUTPUT),1,&bmnum0_down);
    
    //OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ANGLE),0,&bmnum0_up);
    //OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ANGLE),1,&bmnum0_down);
    
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
void InitSetDlgCor(void)
{
    
    int i;
    int j;
    int x;
    int y;
    GUI_WIDGET_CREATE_INFO *pC; 
    GUI_CONST_STORAGE GUI_BITMAP  *pBmp;
  
      
    //YMD
    pC = &_aSetDialogCreate[1];
    x  = DATE_LEFT_SPACE;
   
    for (i = 0; i < 3; i++)
    {
        y  = DATA_TIME_TOP;
        
        for (j = 0; j < 3; j++)
        {
            pC->x0 = x;
            pC->y0 = y;
            
            pBmp =  SetTimeBmp[j * 2];
            pC->xSize =  pBmp->XSize;
            pC->ySize =  pBmp->YSize;
            
            y+= pC->ySize;
            pC++;

            
        }         

        x += bmadd_u.XSize + YMD_HMS_SPACE; 
                     
             
    }
    
      
    x  += DATE_TIME_SPACE;
    
    for (i = 0; i < 3; i++)
    {
        y  = DATA_TIME_TOP;
        pBmp =  *SetTimeBmp;
        for (j = 0; j < 3; j++)
        {
            pC->x0 = x;
            pC->y0 = y;
            pBmp =  SetTimeBmp[j * 2];
            pC->xSize =  pBmp->XSize;
            pC->ySize =  pBmp->YSize;
            
            y+= pC->ySize;
            pC++;

        }         

        x += bmadd_u.XSize + YMD_HMS_SPACE; 
               
    }
    
    y+= 50;
    //OK
    pC->x0 = x - 270;
    pC->y0 = y;
    pC->xSize =  bmset_u.XSize;
    pC->ySize =  bmset_u.YSize;
    pC++;
    
    //CANCEL
    //Touch Ajust
    pC->x0 = x - 655;
    pC->y0 = y;
    pC->xSize =  bmset_u.XSize;
    pC->ySize =  bmset_u.YSize;
    
    pC++;
    
    //时间显示抬头
    pC->x0    = DATE_LEFT_SPACE + 60;
    pC->y0    = 85;
    pC->xSize =  240;
    pC->ySize =  50;
    
    pC++;
    
    //时间显示抬头
    pC->x0    = 0x1A6;
    pC->y0    = 85;
    pC->xSize =  240;
    pC->ySize =  40;
    
    //日期显示抬头
    
    
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
void PaintSetDlg(WM_HWIN hWin)
{
	/*GUI_RECT speed_r0={20,62,750,450};
	GUI_RECT speed_r1={21,63,749,449};
	GUI_RECT speed_r2={22,64,748,448};


 
    GUI_SetColor(RGB(117,117,117));

	GUI_DrawRectEx(&speed_r0);
	GUI_DrawRectEx(&speed_r1);
	GUI_DrawRectEx(&speed_r2);
     */
    GUI_SetColor(RGB(38,38,38));
    GUI_DrawHLine(120,DATE_LEFT_SPACE,770);	
    GUI_DrawHLine(121,DATE_LEFT_SPACE,770);
    GUI_DrawHLine(122,DATE_LEFT_SPACE,770);     
   
    
    GUI_DrawBitmap(&bmtm_label,DATE_LEFT_SPACE, DATA_TIME_TOP - bmtm_label.YSize- 30);


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
void UpdataDlgItem(WM_HWIN hwnd, int id, int8_t *v,char *fmt,int min,int max)
{
    char   str_buff[10];

    
   
    if (*v < min)
    {
        *v = max;   
    }
    else if (*v > max)
    {
        *v = min;   
        
    }        
    
    sprintf(str_buff, fmt, *v);
    BUTTON_SetText(WM_GetDialogItem(hwnd,id),str_buff);
    
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
static void _cbSetDlgCallback(WM_MESSAGE *pMsg) 
{
    int    NCode;
    int    id;
    static int     ts_y = 0;
    static int     ts_pressed = 0;
    
    WM_PID_STATE_CHANGED_INFO  *pTSState;
    
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:                                 
            PaintSetDlg(pMsg->hWin);
            break;
        case WM_TOUCH_CHILD:           
   
            break;
        case WM_INIT_DIALOG:															
            InitSetDlg(pMsg->hWin);
            break;
        case WM_PID_STATE_CHANGED:
            pTSState = (WM_PID_STATE_CHANGED_INFO*)(pMsg->Data.p);
            if (pTSState->State) //按下
            {
                ts_y         =   pTSState->y;
                ts_pressed   = 1;
            }
            else  if ((ABS(pTSState->y - ts_y) > 150) && ts_pressed)  
            {
                //如果从其他界面划过来，且按钮又抬起，会直接跳到这里，所以要判断是否按下过:ts_pressed
                //WM_DisableWindow(hSetDlg);
                //WM_HideWindow(hSetDlg);
                               
                //WM_ShowWindow(hMain);
                //WM_EnableWindow(hMain);
                GUI_EndDialog(pMsg->hWin, 1);
                WM_MakeModal(hMain);
                ts_pressed = 0;
                //WM_SetFocus(hMain);
                
            }
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(pMsg->hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(pMsg->hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:		//控件通知										
            id    = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (NCode) 
            {
                case WM_NOTIFICATION_RELEASED: //按键释放
                {
                    switch(id)
                    {
                        case GUI_ID_OK:
                            GUI_EndDialog(pMsg->hWin, 0);
                            break;
                        case GUI_ID_CANCEL:
                            GUI_EndDialog(pMsg->hWin, 0);
                            break;
                        case GUI_ID_BUTTON_TIME_SET_OK:
                            SetRTCTime(&CurDT);
                            GUI_EndDialog(pMsg->hWin, 1);
                            WM_MakeModal(hMain);
                            break;
                        case GUI_ID_BUTTON_TIME_SET_CANCEL:
                            TSCalibDlg();
                            break; 
                        default:
                            ;
                            
                    }
                    break;
                }
                case WM_NOTIFICATION_CLICKED: //单击
                {
                    switch(id)
                    {
                        case GUI_ID_BUTTON_DATA_SET_YA://year++
                            CurDT.year++;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_DATA_SET_YTEXT,(int8_t*)&CurDT.year,"20%02d",0,99);
                            break;
                        case GUI_ID_BUTTON_DATA_SET_YS://year--
                            CurDT.year--;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_DATA_SET_YTEXT,(int8_t*)&CurDT.year,"20%02d",0,99);
                            break;
                        case GUI_ID_BUTTON_DATA_SET_MA: //month++
                            CurDT.mon++;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_DATA_SET_MTEXT,&CurDT.mon,"%02d",1,12);
                            break;
                        case GUI_ID_BUTTON_DATA_SET_MS: //month--
                            CurDT.mon--;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_DATA_SET_MTEXT,&CurDT.mon,"%02d",1,12);
                            break;
                        case GUI_ID_BUTTON_DATA_SET_DA: //date++
                            CurDT.day++;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_DATA_SET_DTEXT,&CurDT.day,"%02d",1,31);
                            break;
                        case GUI_ID_BUTTON_DATA_SET_DS: //date--
                            CurDT.day--;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_DATA_SET_DTEXT,&CurDT.day,"%02d",1,31);  
                            break;
                        case GUI_ID_BUTTON_TIME_SET_HA://hour++
                            CurDT.hour++;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_TIME_SET_HTEXT,&CurDT.hour,"%02d",0,23);
                            break;
                        case GUI_ID_BUTTON_TIME_SET_HS://hour--
                            CurDT.hour--;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_TIME_SET_HTEXT,&CurDT.hour,"%02d",0,23);
                            break;
                        case GUI_ID_BUTTON_TIME_SET_MA: //minute++
                            CurDT.min++;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_TIME_SET_MTEXT,&CurDT.min,"%02d",0,59);
                            break;
                        case GUI_ID_BUTTON_TIME_SET_MS: //minute--
                            CurDT.min--;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_TIME_SET_MTEXT,&CurDT.min,"%02d",0,59);      
                            break;
                        case GUI_ID_BUTTON_TIME_SET_SA: //second++
                            CurDT.sec++;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_TIME_SET_STEXT,&CurDT.sec,"%02d",0,59);
                            break;
                        case GUI_ID_BUTTON_TIME_SET_SS: //second--
                            CurDT.sec--;
                            UpdataDlgItem(pMsg->hWin,GUI_ID_BUTTON_TIME_SET_STEXT,&CurDT.sec,"%02d",0,59);      
                            break;  
                      
                        default:
                           ;
                         
                     }
                     break;
                    
                }  
                //break;                

             } //END (Id) 
             
			 break;

        default:
            WM_DefaultProc(pMsg);
        
    } //END (pMsg->MsgId) 
    
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
void SetDlg(void)
{   
    RTC_TimeTypeDef  CurTime;
    RTC_DateTypeDef  CurDate;
    ///int result;
    
    
    //WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
    //WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    //GUI_Clear();


    RTC_GetTime(RTC_Format_BIN, &CurTime);
	RTC_GetDate(RTC_Format_BIN, &CurDate);
    
    CurDT.hour = CurTime.RTC_Hours;
    CurDT.min  = CurTime.RTC_Minutes;
    CurDT.sec  = CurTime.RTC_Seconds;
    CurDT.year = CurDate.RTC_Year;
    CurDT.mon  = CurDate.RTC_Month;
    CurDT.day  = CurDate.RTC_Date;
    
    
    //注意坐标必须在窗体创建之前初始化,不能在WM_INIT_DLG中初始化
    InitSetDlgCor();
    ///WM_DisableWindow(hMain);
    
   #if 1
    hSetDlg = GUI_CreateDialogBox(_aSetDialogCreate, GUI_COUNTOF(_aSetDialogCreate), &_cbSetDlgCallback, hMain, 0, 0);

   #else
     hSetDlg = GUI_CreateDialogBox(_aSetDialogCreate, GUI_COUNTOF(_aSetDialogCreate), &_cbSetDlgCallback, WM_HBKWIN, 0, 0);
     
         
    //WM_MakeModal(hSetDlg);
    ///WM_SetFocus(hSetDlg);
    
     WM_MakeModal(hSetDlg);
     GUI_ExecCreatedDialog(hSetDlg);
   #endif   
    //WM_SetFocus(hWindKB);
    //result =  GUI_ExecCreatedDialog(hSetDlg);
	// 焦点返回给主对话框
    //WM_EnableWindow(hMain);
	//WM_MakeModal(hMain);
	//WM_SetFocus(hMain);
 
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
void  SetRTCTime(DATE_TIME *ptm)
{
    RTC_TimeTypeDef  CurTime;
    RTC_DateTypeDef  CurDate;
    
    CurTime.RTC_Hours   = ptm->hour;
    CurTime.RTC_Minutes = ptm->min;
    CurTime.RTC_Seconds = ptm->sec;
    CurDate.RTC_Year    = ptm->year % 2000;
    CurDate.RTC_Month   = ptm->mon;
    CurDate.RTC_Date    = ptm->day;  
    
    CurDate.RTC_WeekDay = 1;//不能为0,否则设置失败
    
    RTC_Set(&CurDate, &CurTime);
	  
    
}







