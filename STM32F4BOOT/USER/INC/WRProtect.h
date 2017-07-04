#ifndef __WR_PROTECT_H
#define __WR_PROTECT_H


//=================================================================================================
//*************************************************************************************************
//宏定义
#define READ     0
#define WRITE    1

#define WRP_Sector_0         OB_WRP_Sector_0
#define WRP_Sector_1         OB_WRP_Sector_1
#define WRP_Sector_2         OB_WRP_Sector_2
#define WRP_Sector_3         OB_WRP_Sector_3
#define WRP_Sector_4         OB_WRP_Sector_4
#define WRP_Sector_5         OB_WRP_Sector_5
#define WRP_Sector_6         OB_WRP_Sector_6
#define WRP_Sector_7         OB_WRP_Sector_7
#define WRP_Sector_8         OB_WRP_Sector_8
#define WRP_Sector_9         OB_WRP_Sector_9
#define WRP_Sector_10        OB_WRP_Sector_10
#define WRP_Sector_11        OB_WRP_Sector_11
#define WRP_Sector_All       OB_WRP_Sector_All

//=================================================================================================
//*************************************************************************************************
//数据类型定义


//=================================================================================================
//*************************************************************************************************
//变量定义

//=================================================================================================
//*************************************************************************************************
//外部函数声明
extern int16_t  DataAreaWRPLock(COMM_FRAME *pcf);
extern int16_t  DataAreaWRPUnlock(COMM_FRAME *pcf);

extern void FlashWRPLock(uint8_t sector_num);
extern void FlashWRPUnlock(uint8_t sector_num);




#endif    //__WR_PROTECT_H


