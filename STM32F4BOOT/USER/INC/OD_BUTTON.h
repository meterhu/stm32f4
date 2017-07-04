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
*                作者: 
*
*************************************************************************************************/
//===============================================================================================

#ifndef __OWN_DRAW_ITEM_H
#define __OWN_DRAW_ITEM_H

//=================================================================================================
//*************************************************************************************************
//宏定义

//#define 
#define  OD_BTN_UP    0x00
#define  OD_BTN_DOWN  0x01



//=================================================================================================
//*************************************************************************************************
//数据类型定义



typedef  struct _OwnDrawItem
{
    uint16_t  x;
    uint16_t  y;
    uint16_t  w;
    uint16_t  h;
    uint16_t  state;
    GUI_CONST_STORAGE GUI_BITMAP  *up_bmp;
    GUI_CONST_STORAGE GUI_BITMAP  *down_bmp;
    
}OWN_DRAW_ITEM;

typedef  struct _OwnDrawItemBitMap
{
    GUI_CONST_STORAGE GUI_BITMAP  *up_bmp;
    GUI_CONST_STORAGE GUI_BITMAP  *down_bmp;
    
}OWN_DRAW_ITEM_BITMAP;



//=================================================================================================
//*************************************************************************************************
//变量定义

//========================================================================================
//


//=================================================================================================
//*************************************************************************************************
//外部函数声明
extern void   OD_BUTTON_SetBitmap(BUTTON_Handle hObj,unsigned int Index, const GUI_BITMAP* pBitmap);
extern BUTTON_Handle OD_BUTTON_Create(int x0, int y0, int xsize, int ysize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id);
extern BUTTON_Handle OD_BUTTON_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb);




#endif    //__INIT_HW_H
