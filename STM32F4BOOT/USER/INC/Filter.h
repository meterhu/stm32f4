#ifndef __FILTER_H
#define __FILTER_H

//=================================================================================================
//*************************************************************************************************
//宏定义
 #define FIL_LEN     5
//=================================================================================================
//*************************************************************************************************
//数据类型定义
typedef struct __Filter
{
	u32 total;
	u32 buff[FIL_LEN];
	u8  n;

}FILTER;
//=================================================================================================
//*************************************************************************************************
//宏定义

//=================================================================================================
//*************************************************************************************************
//外部函数声明

extern s32 Filter(u8 num,s32 data);
s32 Filter_num(FILTER *pF,s32 data);

void InitF(FILTER *pF);
extern void InitFILTER(void);

#endif












