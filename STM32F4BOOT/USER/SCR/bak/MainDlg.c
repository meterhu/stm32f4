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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GUI.h"
#include "mystd.h"
#include "AppCfg.h"
#include "LCD_RA8875.h"
#include "UserLib.h"
#include "WM.h" 
#include "BUTTON.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "TEXT.h"
#include "MainDlg.h"
#include "rtc.h"
#include "SoftKBDlg.h"
#include "ID.h"
#include "OD_BUTTON.h"
#include "msg.h"
#include "dialog.h"
#include "PWM.h"
///#include "SetDlg.h"

//===============================================================================================
//extern GUI_CONST_STORAGE GUI_BITMAP bmnum0_down;
//extern GUI_CONST_STORAGE GUI_BITMAP bmnum0_up;
// extern GUI_CONST_STORAGE GUI_BITMAP bmBitMap1;
// extern GUI_CONST_STORAGE GUI_BITMAP bmBitMap2;
// extern GUI_CONST_STORAGE GUI_BITMAP bmBitMap3;
// extern GUI_CONST_STORAGE GUI_BITMAP bmadd01;
// extern GUI_CONST_STORAGE GUI_BITMAP bmadd02;
// extern GUI_CONST_STORAGE GUI_BITMAP bmsub01;
// extern GUI_CONST_STORAGE GUI_BITMAP bmsub02;
// extern GUI_CONST_STORAGE GUI_BITMAP bmAngle01;
// extern GUI_CONST_STORAGE GUI_BITMAP bmAngle02;

extern void SetDlg(void);

//=================================================================================================================
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
		{ FRAMEWIN_CreateIndirect,       "",                      GUI_ID_MAIN_DLG,        0,   0,   800, 480, 0,0},
		{ BUTTON_CreateIndirect,         "",					  GUI_ID_BUTTON_ANGLE,    440,355,275,60, 0,0},
        { BUTTON_CreateIndirect,         "",					  GUI_ID_BUTTON_ADD,      440,275,75, 40, 0,0},
		{ BUTTON_CreateIndirect,         "",                      GUI_ID_BUTTON_SUB,      640,275,75, 40, 0,0},
    	{ TEXT_CreateIndirect,           "Speed(RPM)",            GUI_ID_TEXT_SPEED,      120, 86,  130,  24,  0,0},
		{ TEXT_CreateIndirect,           "Angle(deg)",            GUI_ID_TEXT_ANGLE,      510, 86,  130,  24,  0,0}, 
		{ TEXT_CreateIndirect,           "EUP/EUI  TESTER",       GUI_ID_TEXT_TITLE,      270, 22,   300, 30,  0,0},
		{ TEXT_CreateIndirect,           "2012-11-11   18:35:45", GUI_ID_TEXT_TIME,       3,   2,  250, 24,  0,0},
		{ EDIT_CreateIndirect,           "",                      GUI_ID_EDIT_SPEED,      45,  115, 285, 135, 0,0},
		{ EDIT_CreateIndirect,           "",                      GUI_ID_EDIT_ANGLE,      435, 115, 285, 135, 0,0},
       // { BUTTON_CreateIndirect,           "--",                 GUI_ID_EDIT_TEST_PWM,   700, 15, 90, 30, 0,0},
       // { BUTTON_CreateIndirect,           "++",                 GUI_ID_EDIT_TEST_PWM1,  600, 15, 90, 30, 0,0},
        
      //{ OD_BUTTON_CreateIndirect,         "",                   GUI_ID_BUTTON_WG,       80, 400, 50, 39,  0,0},
                
	
};

//===========================================================================================
uint32_t  AngleSet = 60;
WM_HWIN  hMain;
extern  WM_HWIN hSetDlg;
static void PaintDialog(WM_MESSAGE * pMsg);
static void InitDialog(WM_MESSAGE * pMsg);
static void Add(WM_MESSAGE * pMsg);
static void Sub(WM_MESSAGE * pMsg);
static int32_t  GetItemBKColor(int id);

int  AngleRun = 0;

//===========================================================================================
//*******************************************************************************************


DECLARE_MESSAGE_MAP()

BEGIN_MESSAGE_MAP()
ON_COMMAND(GUI_ID_BUTTON_ADD, Add)
ON_COMMAND(GUI_ID_BUTTON_SUB, Sub)
ON_WM_PAINT(PaintDialog)
ON_WM_INIT_DIALOG(InitDialog)
END_MESSAGE_MAP()

//===========================================================================================


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
static void Add(WM_MESSAGE * pMsg)
{
    char str_buff[20];

    AngleSet++;
    sprintf(str_buff,"%ld",AngleSet);
    EDIT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_EDIT_ANGLE),str_buff);
     
    
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
static void Sub(WM_MESSAGE * pMsg)
{
    char str_buff[20];

    AngleSet--;
    sprintf(str_buff,"%ld",AngleSet);
    EDIT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_EDIT_ANGLE),str_buff);
       
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
static void PaintDialog(WM_MESSAGE * pMsg)
{  
	U16 i = 0;
    float r,g,b;
    float ri,gi,bi;
    U16  y;
    
    //WM_HWIN hWin = pMsg->hWin;
//     GUI_RECT speed_r0={30,82,350,430};
//     GUI_RECT speed_r1={31,83,349,429};
//     GUI_RECT speed_r2={32,84,348,428};
	GUI_RECT speed_r0={30,82,350,430};
	GUI_RECT speed_r1={31,83,349,429};
	GUI_RECT speed_r2={32,84,348,428};
	GUI_RECT angle_r0={420,82,740,430};
	GUI_RECT angle_r1={421,83,739,429};
	GUI_RECT angle_r2={422,84,738,428};

    ri = (float)(217 - 151) / (float)480;
    gi = (float)(208 - 184) / (float)480;
    bi = (float)(226 - 198) / (float)480;
    r = 151;
    g = 184;
    b = 226;
    y = 0;
    for (i = 0; i < 480;i++,y++)
    {
        GUI_SetColor(RGB((U16)r,(U16)g,(U16)b));
        ///Delay(1000);
        GUI_DrawHLine(y,0,799);	        
        
        r +=  ri;
        g +=  gi;
        b -=  bi;
	
    }
    
    //GUI_SetColor(GUI_LIGHTCYAN);
    GUI_SetColor(RGB((U16)156,(U16)157,(U16)145));;
	GUI_DrawRectEx(&speed_r0);
	GUI_DrawRectEx(&speed_r1);
	GUI_DrawRectEx(&speed_r2);
	GUI_DrawRectEx(&angle_r0);
	GUI_DrawRectEx(&angle_r1);
	GUI_DrawRectEx(&angle_r2);
	
    ////GUI_DrawBitmap(&bmBitMap1, 120, 270);
	//GUI_DrawBitmap(&bmBitMap1, 120, 270);
    //GUI_DrawBitmap(&bmnum0_up,80,250);
    //GUI_DrawBitmap(&bmBitMap1, 120, 270);

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
static void InitDialog(WM_MESSAGE * pMsg)
{
  
    char               str_buff[35];
    RTC_TimeTypeDef    time;
    RTC_DateTypeDef    date;
    uint32_t           angle_backup;
    
    WM_HWIN hWin = pMsg->hWin;
    
    //WM_CreateTimer
    //
    //FRAMEWIN
    FRAMEWIN_SetClientColor(hWin,RGB(70,77,106));//0xb0c0d8RGB(151,184,224)
    FRAMEWIN_SetTitleVis(hWin,0);
    FRAMEWIN_SetBarColor(hWin,1,GUI_WHITE);
    FRAMEWIN_SetTitleHeight(hWin,0);
    //BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_OUTPUT),BUTTON_CI_UNPRESSED, RGB(39,183,246));
    //BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_OUTPUT),BUTTON_CI_PRESSED, RGB(13,143,221));
    ///BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_OUTPUT),BUTTON_CI_UNPRESSED,  GUI_WHITE);
    //BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_OUTPUT),BUTTON_CI_PRESSED,  GUI_WHITE);
    //BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_OUTPUT),&GUI_Font24_ASCII);

//     //GUI_ID_BUTTON_ANGLE
//     //�Ƕȿ���
//     BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ANGLE),BUTTON_CI_UNPRESSED, RGB(39,183,246));
//     BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ANGLE),BUTTON_CI_PRESSED, RGB(13,143,221));
//     BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ANGLE),BUTTON_CI_UNPRESSED,  GUI_WHITE);
//     BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ANGLE),BUTTON_CI_PRESSED,  GUI_WHITE);
//     BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ANGLE),&GUI_Font24_ASCII);
  
   #if 0 
    //GUI_ID_BUTTON_ADD
    //������ť
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ADD),BUTTON_CI_UNPRESSED, RGB(39,183,246));
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ADD),BUTTON_CI_PRESSED,  RGB(13,143,221));
    BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ADD),BUTTON_CI_UNPRESSED,  GUI_WHITE);
    BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ADD),BUTTON_CI_PRESSED,  GUI_WHITE);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ADD),&GUI_FontD32);
   
    
    //GUI_ID_BUTTON3
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SUB),BUTTON_CI_UNPRESSED, RGB(39,183,246));
    BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SUB),BUTTON_CI_PRESSED,  RGB(13,143,221));
    BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SUB),BUTTON_CI_UNPRESSED,  GUI_WHITE);
    BUTTON_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SUB),BUTTON_CI_PRESSED,  GUI_WHITE);
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SUB),&GUI_FontD32);
  #else
    //GUI_ID_BUTTON_ADD
    //������ť
   // OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ANGLE),0,&bmAngle01);///
   // OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ANGLE),1,&bmAngle01);
    //GUI_ID_BUTTON_ADD
    //������ť
   // OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ADD),0,&bmadd01);
   // OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_ADD),1,&bmadd02);
 

   // OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SUB),0,&bmsub01);
   // OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SUB),1,&bmsub02);
  #endif
    //
    //BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SAVE),BUTTON_CI_UNPRESSED, 0xe7eaeb);
    //BUTTON_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SAVE),BUTTON_CI_PRESSED, 0xb0c0d8);
    //BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SAVE),&GUI_Font24_ASCII);
    //BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SAVE),0,&bmsave_up);
    //BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_SAVE),1,&bmsave_down);
 
    //GUI_ID_TEXT_SPEED
    TEXT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_SPEED),GetItemBKColor(GUI_ID_TEXT_SPEED));
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_SPEED),RGB(235,220,197));
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_SPEED),&GUI_Font24B_ASCII);

    //GUI_ID_TEXT_ANGLE
    TEXT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_ANGLE),GetItemBKColor(GUI_ID_TEXT_ANGLE));
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_ANGLE),RGB(235,220,197));
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_ANGLE),&GUI_Font24B_ASCII);//GUI_FontD60x80
    
    //GUI_ID_TEXT_TITLE
    TEXT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_TITLE),GetItemBKColor(GUI_ID_TEXT_TITLE));
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_TITLE),GUI_WHITE);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_TITLE),&GUI_Font32B_ASCII);

    //GUI_ID_TEXT_TIME
    TEXT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_TIME),RGB(151,184,226));
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT_TIME),&GUI_Font24_ASCII);
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT_TIME),GUI_WHITE);
    
    //GUI_ID_EDIT_SPEED
    EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT_SPEED), EDIT_CF_HCENTER|EDIT_CF_VCENTER);
    EDIT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_SPEED), EDIT_CI_ENABLED,RGB(240,241,232));
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT_SPEED),&GUI_FontD80);
    EDIT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_SPEED),0,RGB(21,175,239));//RGB(18,120,188)
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_SPEED),"0000");
    
    //�Ի水ť����
    //OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_OD1),0,&bmnum0_up);
    //OD_BUTTON_SetBitmap(WM_GetDialogItem(hWin,GUI_ID_BUTTON_OD1),1,&bmnum0_down);

    //GUI_ID_EDIT_ANGLE
    EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT_ANGLE),EDIT_CF_HCENTER|EDIT_CF_VCENTER);
    EDIT_SetBkColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_ANGLE), EDIT_CI_ENABLED,RGB(240,241,232));
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT_ANGLE),&GUI_FontD80);
    EDIT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_EDIT_ANGLE),0,RGB(238,12,12));
    
    ///EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT_TEST_PWM),&GUI_Font24_ASCII);
    
    
    
    //�Ӻ�RTC RAM�ж�ȡ���һ�ε��趨ֵ
    angle_backup = RTC_ReadBackupRegister(RTC_BKP_DR3);
    
    if (angle_backup > ANGLE_MAX)
    {
        AngleSet = 1;
        RTC_WriteBackupRegister(RTC_BKP_DR3, AngleSet);
    }
    else 
    {
        AngleSet = angle_backup;
    }
    
    
    sprintf(str_buff,"%d",AngleSet);
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT_ANGLE),str_buff);
    
    //ʱ����ʾ
    RTC_GetTime(RTC_Format_BIN,&time);
    RTC_GetDate(RTC_Format_BIN,&date);
    sprintf(str_buff, "%02d-%02d-20%02d   %02d:%02d:%02d", 
            date.RTC_Month,date.RTC_Date,date.RTC_Year,
            time.RTC_Hours,time.RTC_Minutes,time.RTC_Seconds
           );
    TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT_TIME), str_buff);
                 
    
//     rect.x0 = 80;
//     rect.y0 = 250;
//     rect.x1 = bmnum0_up.XSize + rect.x0;
//     rect.y1 = bmnum0_up.YSize + rect.y0;
//     WM_SetUserClipRect(&rect);
    
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
#ifdef  _USE_MFC_MSG_STYLE
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    
    if (!WinMsgProc(pMsg, GetMessageMap())) //�����Ϣû�д��������ϵͳĬ�ϵ���Ϣ�������
    {
        WM_DefaultProc(pMsg);
    }
   

}
#endif

volatile int KK;

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
#ifndef  _USE_MFC_MSG_STYLE
static void _cbCallback(WM_MESSAGE *pMsg) 
{
    int                NCode;
    int                id;
    GUI_RECT           Rect;
    char               str_buff[35];
    GUI_PID_STATE*     pState;
    //int                result;
    
    RTC_TimeTypeDef    time;
    RTC_DateTypeDef    date;
    ///static int state = 0;
    static int jj = 10000;
    
    //WM_HWIN  hItem;
    
    static int     edit_press = 0;
    static int     ts_y;
    WM_PID_STATE_CHANGED_INFO  *pTSState;
    
    ///GUI_RECT rect;
    switch (pMsg->MsgId) 
    {
        case WM_USER_SHOW_PWM:
            sprintf(str_buff,"%ld",pMsg->Data.v); 
            //BUTTON_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_EDIT_TEST_PWM),str_buff);
            break;
        case WM_USER_SFKB_END:
             //ע�⣬������趨���Ժ󣬻��ȡ����Ϣ
            if(pMsg->Data.v > 360)
            {
                AngleSet = 360;
            }
            else
            {
                AngleSet = pMsg->Data.v;                
            }

            sprintf(str_buff,"%ld",AngleSet); 
            EDIT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_EDIT_ANGLE),str_buff);
        
            RTC_WriteBackupRegister(RTC_BKP_DR3, AngleSet);
            break;
        case WM_USER_MSG1:
            sprintf(str_buff,"%d",pMsg->Data.v);
            EDIT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_EDIT_SPEED), str_buff);
            KK = 0;       
            break;
        case WM_USER_MSG2:
            //WM_SelectWindow(WM_GetFirstChild(pMsg->hWin));
            
            //if(pMsg->Data.v == 0)
            //{
            //    GUI_DrawBitmap(&bmBitMap1, 120, 270);
            //}
            //else if(pMsg->Data.v == 1)
            //{
             //   GUI_DrawBitmap(&bmBitMap2, 120, 270);
            //}
            //else
            //{
            //    GUI_DrawBitmap(&bmBitMap3, 120, 270);
            //}	
            break;
        case WM_USER_SHOW_TIME:
            RTC_GetTime(RTC_Format_BIN,&time);
            RTC_GetDate(RTC_Format_BIN,&date);
            sprintf(str_buff, "%2d-%02d-20%02d   %02d:%02d:%02d", 
                    date.RTC_Month,date.RTC_Date,date.RTC_Year,
                    time.RTC_Hours,time.RTC_Minutes,time.RTC_Seconds
                   );
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT_TIME), str_buff);
            break;
        case WM_PAINT:                                 
            PaintDialog(NULL);
            break;
        case WM_PID_STATE_CHANGED:
            pTSState = (WM_PID_STATE_CHANGED_INFO*)(pMsg->Data.p);
            if (pTSState->State) //����
            {
                ts_y =   pTSState->y;
            }
            else  if ((ABS(pTSState->y - ts_y) > 150))// && (ts_y != -1))
            {
                  ////SetDlg();                
    
            }
            break;
        case WM_TOUCH_CHILD:            
            
            // ��ȡ�ؼ�ID
            id     = WM_GetId(pMsg->hWinSrc); 
  		    pState = (GUI_PID_STATE *)((WM_MESSAGE *)pMsg->Data.p)->Data.p;
            
        
			// ���TOUCH��״̬
            if (pState == 0) 
			{
				break;
			}
            if (pState->Pressed) 
			{
				switch(id)
                {   
                    //case GUI_ID_BUTTON_ADD: //��ť����
                    //{
                    //     AngleSet++;
                    //     sprintf(str_buff,"%ld",AngleSet);
                    //    EDIT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_EDIT_ANGLE),str_buff);
                    //     break;
                    //}
                    case GUI_ID_EDIT_ANGLE:
                    {
                        //�༭��--��Ϊ�༭���ڰ�ţ̌��Ҫ�����Ի���
                        //���Ի������ʱ���᷵�ص��ô��壬�һ�����ԭ���Ľ���Ϊ�˶Ի���
                        //��Ϊ��ʱ�������ѵ��𣬾ͻ����WM_TOUCH_CHILD�¼���pState->Pressed
                        //�ᱻ����Ϊ���𣬼������´�������Ĵ���������ʱ��(else���벿��)
                        //�����ֻ����²����Ƕȱ༭�����������¼����ظ������趨С����
                        //�˴��������ʶ�������Ƿ��ϴα�����,ע��˱��������ĵ�����
                        //�б�����                        
                        edit_press = 1;
                        break;
                    }
                    default:
                       ;
                }
			}
            else
            {
                if ((id == GUI_ID_EDIT_ANGLE) && edit_press)//ע����̧��ŵ���ģ̬�Ի���
                {
                    WM_GetWindowRectEx(pMsg->hWinSrc,&Rect);
                    //hItem = WM_GetDialogItem(pMsg->hWin,GUI_ID_EDIT_ANGLE);
                    //ts_y = -1;////////!!!!!!!!debug
                    //SoftKBDlg(pMsg->hWin,Rect.x0,Rect.y0, (void *)AngleSet); 
                    ////SoftKBDlg(pMsg->hWin,Rect.x0,Rect.y0, (void *)0); 
                    edit_press = 0;
//                     if (result >= 0) //-1Ϊ������cancel
//                     {
//                          AngleSet = BCD2UINT16(result);
//                          sprintf(str_buff,"%d",AngleSet);
//                          EDIT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_EDIT_ANGLE),str_buff);
//                     }
                    
                }
            }
            break;
        case WM_INIT_DIALOG:															
            InitDialog(pMsg);
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
        case WM_NOTIFY_PARENT:		//�ؼ�֪ͨ										
            id    = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (NCode) 
            {
                case WM_NOTIFICATION_RELEASED: //�����ͷ�
                {
                    switch(id)
                    {
                        case GUI_ID_OK:
                            GUI_EndDialog(pMsg->hWin, 0);
                            break;
                        case GUI_ID_CANCEL:
                            GUI_EndDialog(pMsg->hWin, 0);
                            break;
                         default:
                            ;
                            
                    }
                    break;
                }
                case WM_NOTIFICATION_CLICKED: //����
                {
                    switch(id)
                    {
                        case GUI_ID_BUTTON_ADD:
                            //WM_SelectWindow(WM_GetFirstChild(pMsg->hWin));
                            //GUI_DrawBitmap(&bmnum0_up,80,260);
                            AngleSet++;
                            if(AngleSet > 360)
                            {
                               AngleSet = 0;
                            }
                        
                            sprintf(str_buff,"%ld",AngleSet);
                            EDIT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_EDIT_ANGLE),str_buff);
                            break;
                        case GUI_ID_BUTTON_SUB:   
                            //WM_SelectWindow(WM_GetFirstChild(pMsg->hWin));
                            //GUI_DrawBitmap(&bmnum0_down,80,260);   
                            if (AngleSet == 0) 
                            {                                
                                AngleSet = 360;
                                
                            }
                            else
                            {
                                AngleSet--;                               
                            }
                            sprintf(str_buff,"%ld",AngleSet);
                            EDIT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_EDIT_ANGLE),str_buff);                          
                            break;
                        case   GUI_ID_BUTTON_ANGLE:
                            if (!AngleRun)
                            {                                
                               // OD_BUTTON_SetBitmap(WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON_ANGLE),0,&bmAngle02);
                               // OD_BUTTON_SetBitmap(WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON_ANGLE),1,&bmAngle02);
                                RestorePWMPin();
                                //TIM_ForcedOC2Config(TIM2, TIM_ForcedAction_Active); //PWM�رպ�ǿ������͵�ƽ
                                TIM_Cmd(TIM2, ENABLE);
                            }
                            else
                            {
                               /// OD_BUTTON_SetBitmap(WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON_ANGLE),0,&bmAngle01);
                               /// OD_BUTTON_SetBitmap(WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON_ANGLE),1,&bmAngle01);
                                //TIM_ForcedOC2Config(TIM2, TIM_ForcedAction_InActive); //PWM�رպ�ǿ������͵�ƽ
                                TIM_Cmd(TIM2, DISABLE);
                                ForcePWMOutLow();
                                
                                //ע��IM_CtrlPWMOutputs(TIM2,DISABLE);ֻ��TIM1��8��Ч
                               
                            }
                            AngleRun =!AngleRun;
                            break;
                      case GUI_ID_EDIT_TEST_PWM:
                         jj -= 1000;
                         if (jj <= 300)
                         {
                             jj = 400;   
                         }
                         TIM_SetAutoreload(TIM3, jj);
                          
                         break;
                      case GUI_ID_EDIT_TEST_PWM1:
                          TIM_SetAutoreload(TIM3, jj);
                          jj += 1000;
                          break;
                            
                        default:
                             ;
                         
                    }
                    break;
                    
                }
                

             } //END (Id) 
             
			 break;

        default:
            WM_DefaultProc(pMsg);
        
    } //END (pMsg->MsgId) 
    
}

#endif



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
void MainDlg(void)
{   
    ///WM_SetDesktopColor(GUI_WHITE);      // Automacally update desktop window 
    //WM_SetCreateFlags(WM_CF_MEMDEV);    //Use memory devices on all windows to avoid flicker 
    ///GUI_Clear();

    hMain = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, WM_HBKWIN, 0, 0);
    //WM_MakeModal(hMain);
    //WM_SetFocus(hWindKB);
    //GUI_ExecCreatedDialog(hMain);
    WM_SetCallback(hMain, &_cbCallback);  

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
void  SetMainCallback(void)
{
    WM_SetCallback(hMain, &_cbCallback);  
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
static int32_t  GetItemBKColor(int id)
{
  	float r,g,b;
    float ri,gi,bi;
    uint8_t rgb[480][3];
    int i;
    
    ri = (float)(217 - 151) / (float)480;
    gi = (float)(208 - 184) / (float)480;
    bi = (float)(226 - 198) / (float)480;
    r = 151;
    g = 184;
    b = 226;
    
    for (i = 0; i < 480;i++)
    {
        rgb[i][0]= (uint8_t)r;
        rgb[i][1]= (uint8_t)g;
        rgb[i][2]= (uint8_t)b;
        r +=  ri;
        g +=  gi;
        b -=  bi;
	
    }
      
   
    for (i = 0; i < sizeof(_aDialogCreate) / sizeof(_aDialogCreate[0]);i++)
    {
        if (_aDialogCreate[i].Id == id)
        {
             break;   
        }
    }
    
    //_aDialogCreate[i].ySize / 2----�ؼ��м�ı���ɫ
    if (i < sizeof(_aDialogCreate) / sizeof(_aDialogCreate[0]))
    {
         return RGB(rgb[_aDialogCreate[i].y0 + _aDialogCreate[i].ySize / 2][0],\
                    rgb[_aDialogCreate[i].y0 + _aDialogCreate[i].ySize / 2][1],\
                    rgb[_aDialogCreate[i].y0 + _aDialogCreate[i].ySize / 2][2]);             
    }
    
    return -1;
    
    
}


