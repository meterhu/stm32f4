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
#include "UserLib.h"
#include "stm32f4xx.h"
#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "TEXT.h"
#include "MainDlg.h"
#include "SoftKBDlg.h"
#include "TimeSet.h"
#include "ID.h"
#include "LCD_RA8875.h"
#include "msg.h"





WM_HWIN hWindKB;
extern WM_HWIN hMain;

void  *ParentWndTransParam;

int  ParentParam;

int NumBtn09ID[]=
{
    GUI_ID_BUTTON_NUM0,GUI_ID_BUTTON_NUM1,GUI_ID_BUTTON_NUM2,GUI_ID_BUTTON_NUM3,
    GUI_ID_BUTTON_NUM4,GUI_ID_BUTTON_NUM5,GUI_ID_BUTTON_NUM6,GUI_ID_BUTTON_NUM7,
    GUI_ID_BUTTON_NUM8,GUI_ID_BUTTON_NUM9
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
GUI_WIDGET_CREATE_INFO _aTextDialogCreate[] = {
		{ FRAMEWIN_CreateIndirect,       "KeyBoard",                  GUI_ID_SFKB_DLG,             0,   0,  215,320,FRAMEWIN_CF_MOVEABLE|FRAMEWIN_CF_ACTIVE,0},
        
		{ EDIT_CreateIndirect,           "",                          GUI_ID_EDIT_KB_INPUT,        5,   10, 195,55, 0,0},
		{ BUTTON_CreateIndirect,         "1",                         GUI_ID_BUTTON_NUM1,          5,   75, 55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "2",                         GUI_ID_BUTTON_NUM2,          75,  75, 55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "3",						  GUI_ID_BUTTON_NUM3,          145, 75, 55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "4",                         GUI_ID_BUTTON_NUM4,          5,   120,55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "5",                         GUI_ID_BUTTON_NUM5,          75,  120,55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "6",                         GUI_ID_BUTTON_NUM6,          145, 120,55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "7",                         GUI_ID_BUTTON_NUM7,          5,   165,55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "8",                         GUI_ID_BUTTON_NUM8,          75,  165,55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "9",                         GUI_ID_BUTTON_NUM9,          145, 165,55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "0",                         GUI_ID_BUTTON_NUM0,          5,   210,125,35, 0,0},
    	{ BUTTON_CreateIndirect,         "BKSPC",                     GUI_ID_BUTTON_BK_SPC,        145, 210,55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "Clr",                       GUI_ID_BUTTON_CLR,           145, 210,55, 35, 0,0},
		{ BUTTON_CreateIndirect,         "OK",                        GUI_ID_BUTTON_OK,            5,   255,80, 35, 0,0},
		{ BUTTON_CreateIndirect,         "Cancel",                    GUI_ID_BUTTON_CANCEL,        120, 255,80, 35, 0,0},
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
void InitKBDlg(WM_MESSAGE * pMsg)
{
	int i;
    char  str_buff[10];
    
    
    WM_HWIN hWin = pMsg->hWin;

    FRAMEWIN_SetClientColor(hWin,RGB(185,205,232));

    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_NUM0),BUTTON_CI_UNPRESSED, RGB(70,77,106));
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_NUM0),BUTTON_CI_PRESSED, RGB(123,128,106));
    BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_NUM0),BUTTON_CI_UNPRESSED,  GUI_WHITE);
    BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_NUM0),BUTTON_CI_PRESSED,  GUI_WHITE);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_NUM0),&GUI_Font16_ASCII);


    for (i = 2; i < 2 + 14; i++) //共14个按钮，在数组中的索引从2开始
    {
        BUTTON_SetBkColor(WM_GetDialogItem(hWin,_aTextDialogCreate[i].Id),BUTTON_CI_UNPRESSED, RGB(70,77,106));
        BUTTON_SetBkColor(WM_GetDialogItem(hWin,_aTextDialogCreate[i].Id),BUTTON_CI_PRESSED, RGB(123,128,106));
        BUTTON_SetTextColor(WM_GetDialogItem(hWin,_aTextDialogCreate[i].Id),BUTTON_CI_UNPRESSED,  GUI_WHITE);
        BUTTON_SetTextColor(WM_GetDialogItem(hWin,_aTextDialogCreate[i].Id),BUTTON_CI_PRESSED,  GUI_WHITE);
        BUTTON_SetFont(WM_GetDialogItem(hWin,_aTextDialogCreate[i].Id),&GUI_Font16_ASCII);

    }

    //GUI_ID_EDIT_KEYBOART
    //
    EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT), EDIT_CF_RIGHT|EDIT_CF_BOTTOM);
    EDIT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT), EDIT_CI_ENABLED,0xe7eaeb );
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT),&GUI_FontD32);
    EDIT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT),0,GUI_RED);
    
    sprintf(str_buff,"%ld",ParentParam);
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT),str_buff);
    
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
void _cbKBDlgCallback(WM_MESSAGE * pMsg) 
{
    int NCode;
    int id;
    int i;
    ///static  U32 input_num ;
    WM_MESSAGE Msg = {0};
    
    char str_buff[10];
      
    WM_HWIN hWin = pMsg->hWin;
    
    switch (pMsg->MsgId) 
    {
        case WM_CREATE:
            break;
        case WM_PAINT:
            //TextNumPaintDialog(pMsg);
            break;        
        case WM_INIT_DIALOG:
            InitKBDlg(pMsg);
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
            
         case WM_NOTIFY_PARENT:
            id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (NCode) 
            {
				case WM_NOTIFICATION_RELEASED:
                    switch (id)
                    {                
                        case GUI_ID_OK:
                            GUI_EndDialog(hWin, 0);
                  
                            break;
                        case GUI_ID_CANCEL:
                            GUI_EndDialog(hWin, 0);
                            break;
                        case GUI_ID_BUTTON_OK:
                            GUI_EndDialog(hWin, ParentParam);
                            Msg.MsgId   = WM_USER_SFKB_END;
                            Msg.Data.v  = BCD2UINT16(ParentParam);
                            Msg.hWin    = hMain;      
                            Msg.hWinSrc = hWin;

                            WM_SendMessage(hMain, &Msg);
                            break;
                        case GUI_ID_BUTTON_CANCEL:
                            GUI_EndDialog(hWin, -1);
                       
                            break;
                        default:
                            break;
                    }  
                    break;                    
                case WM_NOTIFICATION_CLICKED:
                    switch(id)
                    {  
                        case GUI_ID_BUTTON_CLR:  //清除
                            ParentParam = 0;
                            goto update_edit;
                        case GUI_ID_BUTTON_BK_SPC: //退格
                            ParentParam = ParentParam >> 4;
                            goto update_edit;
                        case GUI_ID_BUTTON_OK:
                            //GUI_EndDialog(hWin, input_num);
                            break;
                        case GUI_ID_BUTTON_CANCEL:
                            //GUI_EndDialog(hWin, -1);
                            break;
                        case GUI_ID_BUTTON_NUM1:
                        case GUI_ID_BUTTON_NUM2:
                        case GUI_ID_BUTTON_NUM3:
                        case GUI_ID_BUTTON_NUM4:
                        case GUI_ID_BUTTON_NUM5:
                        case GUI_ID_BUTTON_NUM6:
                        case GUI_ID_BUTTON_NUM7:
                        case GUI_ID_BUTTON_NUM8:
                        case GUI_ID_BUTTON_NUM9:
                        case GUI_ID_BUTTON_NUM0:    
                            for (i = 0; i < 10; i++) //10个数字
                            {
                                if (NumBtn09ID[i] == id)
                                {
                                    ParentParam = (ParentParam << 4) + i; 
                                    break;             
                                }
                            } 
                            
                            ParentParam &= 0xFFFFF;
                            
                          update_edit:
                            sprintf(str_buff,"%x",ParentParam);
                            EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT),str_buff);
                            break;
                         default:
                             break;
                   } 
                   break;
               }
               
               break;
               
         default:
            WM_DefaultProc(pMsg);      
    }      
          
        
    
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
void _cbKBDlgCallback1(WM_MESSAGE * pMsg) 
{
    int NCode;
    int id;
    int i;
    
    char str_buff[10];
      
    WM_HWIN hWin = pMsg->hWin;
    
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            //TextNumPaintDialog(pMsg);
            break;        
        case WM_INIT_DIALOG:
            InitKBDlg(pMsg);
            break;        
        case WM_KEY:
            break;            
        case WM_NOTIFY_PARENT:
            id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (NCode) 
            {
				case WM_NOTIFICATION_RELEASED:
                    switch (id)
                    {                
                        case GUI_ID_OK:
                            GUI_EndDialog(hWindKB, 0);
                            break;
                        case GUI_ID_CANCEL:
                            GUI_EndDialog(hWindKB, 0);
                        
                            break;
                    }  
                    break;  
                                 
                case WM_NOTIFICATION_CLICKED:
                    switch(id)
                    {  
                        case GUI_ID_BUTTON_CLR:  //清除
                            EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT),"0");
                            break;
                        case GUI_ID_BUTTON_BK_SPC: //退格
                            WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT));
                            GUI_SendKeyMsg(GUI_KEY_BACKSPACE,1);
                            break;
                        case GUI_ID_BUTTON_OK:
                            EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT),'\0');
                            EDIT_GetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT),str_buff,sizeof(str_buff));
                            GUI_EndDialog(hWin, atoi(str_buff));
                            break;
                        case GUI_ID_BUTTON_CANCEL:
                            GUI_EndDialog(hWin, -1);
                            break;
                        default:
                            for (i = 0; i < 10; i++) //10个数字
                            {
                                if (NumBtn09ID[i] == id)
                                {
                                    EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT_KB_INPUT),'0' + i);
                                    break;             
                                }
                            }                             
                          
                   } 
                   break;
               }
               
               break;
               
         default:
            WM_DefaultProc(pMsg);      
    }      
          
        
    
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
int SoftKBDlg(WM_HWIN hParenWin,int x0,int y0, void *para)
{
    int x;
    int y;
    int j;
    int i;
    int k;
    int result;
    
    
    y = BTN_TOP_SPACE;
    k = 2; //按钮索引从2开始
    for (i = 0; i < 3; i++) //3行3列
    {
         for (x = BTN_LEFT_SPACE,j = 0; j < 3; j++)
         {
              _aTextDialogCreate[k].x0 = x;
              _aTextDialogCreate[k].y0 = y; 
              
             
              _aTextDialogCreate[k].xSize =  NUM_BUTTON_W;
              _aTextDialogCreate[k].ySize =  NUM_BUTTON_H;
             x += NUM_BUTTON_W + BTN_H_SPACE; 
             k++; 
         }
         
         y += NUM_BUTTON_H + BTN_V_SPACE;
         
    }
    
    //数字0
    x = BTN_LEFT_SPACE;
    _aTextDialogCreate[k].x0 = x;
    _aTextDialogCreate[k].y0 = y; 
    _aTextDialogCreate[k].xSize =  NUM_BUTTON_W * 2 + BTN_H_SPACE;
    _aTextDialogCreate[k].ySize =  NUM_BUTTON_H;
    
   
    k++;
    
    //退格
    x += NUM_BUTTON_W * 2 + BTN_H_SPACE + BTN_H_SPACE; 
    _aTextDialogCreate[k].x0 = x;
    _aTextDialogCreate[k].y0 = y; 
    _aTextDialogCreate[k].xSize =  NUM_BUTTON_W;
    _aTextDialogCreate[k].ySize =  NUM_BUTTON_H;
     
    k++;
    
    //最后1行
    x = BTN_LEFT_SPACE;
    y += NUM_BUTTON_H + BTN_V_SPACE;
    for (i = 0; i < 3; i++)
    {
        _aTextDialogCreate[k].x0 = x;
        _aTextDialogCreate[k].y0 = y; 
        _aTextDialogCreate[k].xSize =  NUM_BUTTON_W;
        _aTextDialogCreate[k].ySize =  NUM_BUTTON_H;  
        x += NUM_BUTTON_W + BTN_H_SPACE;         
        k++;
        
    }
    
    y += NUM_BUTTON_H + BTN_V_SPACE;
    
    //文本输入框
    _aTextDialogCreate[1].x0    = EDIT_LEFT_SPACE;
    _aTextDialogCreate[1].y0    = EDIT_TOP_SPACE;
    _aTextDialogCreate[1].xSize = EDIT_XSIZE;
    _aTextDialogCreate[1].ySize = EDIT_YSIZE;
   
    
    //窗体本身
    _aTextDialogCreate[0].xSize = NUM_BUTTON_W * 3 + BTN_H_SPACE * 2 + BTN_LEFT_SPACE * 2;
    _aTextDialogCreate[0].ySize = y + EDIT_TOP_SPACE;
    
    x0 -= _aTextDialogCreate[0].xSize; //
    if (y0 + _aTextDialogCreate[0].ySize > 479) //479 = LCD_H
    {
         y0 = 479 - _aTextDialogCreate[0].ySize - 50; //50--debug
    }
     
   
    ParentParam  = (int)para;  //注意这里保存的是要返回值的指针
    
    
  #if 1
    //WM_DisableWindow(hParenWin);
	hWindKB = GUI_CreateDialogBox(_aTextDialogCreate, GUI_COUNTOF(_aTextDialogCreate), &_cbKBDlgCallback,hParenWin, x0, y0);
    WM_MakeModal(hWindKB);
  #else    
    //WM_DisableWindow(hParenWin);
	hWindKB = GUI_CreateDialogBox(_aTextDialogCreate, GUI_COUNTOF(_aTextDialogCreate), &_cbKBDlgCallback,WM_HBKWIN, x0, y0);
    
    ///WM_SetCallback(hWindKB, &_cbKBDlgCallback);  
    WM_MakeModal(hWindKB);
    //WM_SetFocus(hWindKB);
    result =  GUI_ExecCreatedDialog(hWindKB);

	// 焦点返回给主对话框
   /// WM_EnableWindow(hParenWin);
	WM_MakeModal(hMain);
  #endif 
    
    //WM_SetFocus(hMain);
    //SetMainCallback();
	//GUI_ExecCreatedDialog(hMain);
    
    
    return result;
	
	
}






