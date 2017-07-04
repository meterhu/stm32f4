/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GUI.h"

#include "WM.h"
#include "BUTTON.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "TEXT.h"
#include "MainDlg.h"
#include "rtc.h"
#include "SoftKBDlg.h"
#include "TimeSet.h"
#include "ID.h"

U16  Year;
U8   Month;
U8   Date;
U8   Hour;
U8   Min;
U8   Sec;
U8   WeekData = 0x06;
/***************************************************************************
*
*                   设置时间对话框
*
*****************************************************************************/
const GUI_WIDGET_CREATE_INFO _aTimeDialogCreate[] = {
		{ FRAMEWIN_CreateIndirect,       "Time Set",                    0,                        0, 0, 500,350,0,0},
		{ BUTTON_CreateIndirect,         "OK",                        GUI_ID_BUTTON_TIME_OK,           300, 295,80, 35, 0,0},
		{ BUTTON_CreateIndirect,         "Cancel",                    GUI_ID_BUTTON_TIME_CAVCEL,           400, 295,80, 35, 0,0},
		{ EDIT_CreateIndirect,           "",                          GUI_ID_EDIT_YEAR,              20, 20,60,30, 0,0},
		{ EDIT_CreateIndirect,           "",                          GUI_ID_EDIT_MONTH,              110, 20,60,30, 0,0},
		{ EDIT_CreateIndirect,           "",                          GUI_ID_EDIT_DATE,              200, 20,60,30, 0,0},
		{ EDIT_CreateIndirect,           "",                          GUI_ID_EDIT_HOUR,              20, 70,60,30, 0,0},
		{ EDIT_CreateIndirect,           "",                          GUI_ID_EDIT_MIN,              110, 70,60,30, 0,0},
		{ EDIT_CreateIndirect,           "",                          GUI_ID_EDIT_SEC,              200, 70,60,30, 0,0},
		{ TEXT_CreateIndirect,           "Y",                         GUI_ID_TEXT_YEAR,              85,  25, 30, 30, 0,0},
		{ TEXT_CreateIndirect,           "M",                         GUI_ID_TEXT_MONTH,              175,  25, 30, 30, 0,0},
		{ TEXT_CreateIndirect,           "D",                         GUI_ID_TEXT_DATE,              265,  25, 30, 30, 0,0},
		{ TEXT_CreateIndirect,           "h",                         GUI_ID_TEXT_HOUR,              85,  75, 30, 30, 0,0},
		{ TEXT_CreateIndirect,           "m",                         GUI_ID_TEXT_MIN,              175, 75, 30, 30, 0,0},
		{ TEXT_CreateIndirect,           "s",                         GUI_ID_TEXT_SEC,              265, 75, 30, 30, 0,0},
};

/*********************************************************************
*
*       设置时间初始化
*
**********************************************************************
*/
void InitTimeDialog(WM_MESSAGE * pMsg)
{
	char str[10] = "";
	WM_HWIN hWin = pMsg->hWin;
	
	RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	
	Year = RTC_DateStructure.RTC_Year;//设置年	
	Month = RTC_DateStructure.RTC_Month ; //设置月
	Date = RTC_DateStructure.RTC_Date; //设置日
	Hour = RTC_TimeStructure.RTC_Hours ; //设置小时
	Min = RTC_TimeStructure.RTC_Minutes; //设置分
	Sec = RTC_TimeStructure.RTC_Seconds ; //设置秒
	

	FRAMEWIN_SetClientColor(hWin,0xdfdfd5);
	 //
    //GUI_ID_BUTTON_TIME_OK
    //
	BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_OK),BUTTON_CI_UNPRESSED, 0xe7eaeb);
	BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_OK),BUTTON_CI_PRESSED, 0xaeaea1);
  BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_OK),&GUI_Font24_ASCII);
	//
  //GUI_ID_BUTTON_TIME_CAVCEL
  //
	BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_CAVCEL),BUTTON_CI_UNPRESSED, 0xe7eaeb);
	BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_CAVCEL),BUTTON_CI_PRESSED, 0xaeaea1);
  BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_TIME_CAVCEL),&GUI_Font24_ASCII);	

	//EXT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_TEXT4),0xb0c0d8);
  TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_YEAR),&GUI_Font24B_ASCII);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_MONTH),&GUI_Font24B_ASCII);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_DATE),&GUI_Font24B_ASCII);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_HOUR),&GUI_Font24B_ASCII);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_MIN),&GUI_Font24B_ASCII);
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_SEC),&GUI_Font24B_ASCII);
	//
  //GUI_ID_EDIT_YEAR
  //
	EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT_YEAR), EDIT_CF_HCENTER|EDIT_CF_VCENTER);
	EDIT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_YEAR), EDIT_CI_ENABLED,GUI_WHITE );
	EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT_YEAR),&GUI_Font24B_ASCII);
	EDIT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_YEAR),0,GUI_BLUE);
	sprintf(str,"20%d",RTC_DateStructure.RTC_Year);
  EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_YEAR),str);
	//
  //GUI_ID_EDIT_MONTH
  //
	EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT_MONTH), EDIT_CF_HCENTER|EDIT_CF_VCENTER);
	EDIT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_MONTH), EDIT_CI_ENABLED,GUI_WHITE );
	EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT_MONTH),&GUI_Font24B_ASCII);
	EDIT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_MONTH),0,GUI_BLUE);
	sprintf(str,"%d",RTC_DateStructure.RTC_Month);
  EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_MONTH),str);
	//
  //GUI_ID_EDIT_DATE
  //
	EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT_DATE), EDIT_CF_HCENTER|EDIT_CF_VCENTER);
	EDIT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_DATE), EDIT_CI_ENABLED,GUI_WHITE );
	EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT_DATE),&GUI_Font24B_ASCII);
	EDIT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_DATE),0,GUI_BLUE);
	sprintf(str,"%d",RTC_DateStructure.RTC_Date);
  EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_DATE),str);
	//
  //GUI_ID_EDIT_HOUR
  //
	EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT_HOUR), EDIT_CF_HCENTER|EDIT_CF_VCENTER);
	EDIT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_HOUR), EDIT_CI_ENABLED,GUI_WHITE );
	EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT_HOUR),&GUI_Font24B_ASCII);
	EDIT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_HOUR),0,GUI_BLUE);
	sprintf(str,"%d",RTC_TimeStructure.RTC_Hours);
  EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_HOUR),str);
	//
  //GUI_ID_EDIT_MIN
  //
	EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT_MIN), EDIT_CF_HCENTER|EDIT_CF_VCENTER);
	EDIT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_MIN), EDIT_CI_ENABLED,GUI_WHITE );
	EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT_MIN),&GUI_Font24B_ASCII);
	EDIT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_MIN),0,GUI_BLUE);
	sprintf(str,"%d",RTC_TimeStructure.RTC_Minutes);
  EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_MIN),str);
	//
  //GUI_ID_EDIT_SEC
  //
	EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT_SEC), EDIT_CF_HCENTER|EDIT_CF_VCENTER);
	EDIT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_SEC), EDIT_CI_ENABLED,GUI_WHITE );
	EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT_SEC),&GUI_Font24B_ASCII);
	EDIT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_SEC),0,GUI_BLUE);
	sprintf(str,"%d",RTC_TimeStructure.RTC_Seconds);
  EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_SEC),str);
}

/*********************************************************************
*
*       设置时间回调函数
*
**********************************************************************
*/
void _cbTimeCallback(WM_MESSAGE * pMsg) 
{
	  RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            //TextNumPaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitTimeDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
							  case GUI_ID_BUTTON_TIME_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
										{
                        GUI_EndDialog(hWin, 0);
											 
												RTC_DateStructure.RTC_Year = Year; //设置年
								
												RTC_DateStructure.RTC_Month = Month; //设置月
								
												RTC_DateStructure.RTC_Date = Date; //设置日
								
												RTC_TimeStructure.RTC_Hours = Hour; //设置小时
							
												RTC_TimeStructure.RTC_Minutes = Min; //设置分
						
												RTC_TimeStructure.RTC_Seconds = Sec; //设置秒
									
												RTC_DateStructure.RTC_WeekDay = WeekData;
										    
										    RTC_Set(&RTC_DateStructure,&RTC_TimeStructure);
										}
                    break;
                case GUI_ID_BUTTON_TIME_CAVCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;               				     
                case GUI_ID_EDIT_YEAR:							//EDIT3  处理过程
								case GUI_ID_EDIT_MONTH:	
								case GUI_ID_EDIT_DATE:	
								case GUI_ID_EDIT_HOUR:
								case GUI_ID_EDIT_MIN:
								case GUI_ID_EDIT_SEC:
									switch(NCode)
									{
										case WM_NOTIFICATION_GOT_FOCUS:
												////OnTimeEditClicked(pMsg);
										    break;																    								
									}
									break;	
								
				
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}





