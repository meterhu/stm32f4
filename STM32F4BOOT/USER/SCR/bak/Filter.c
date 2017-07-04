#include "stm32f4xx.h"
#include "Filter.h"


//============================================================================
FILTER F1;
FILTER *pF1;
FILTER F2;
FILTER *pF2;
FILTER F3;
FILTER *pF3;
FILTER F4;
FILTER *pF4;
//FILTER F5;
//FILTER *pF5;
//FILTER F6;
//FILTER *pF6;
//FILTER F7;
//FILTER *pF7;
//FILTER F8;
//FILTER *pF8;
//FILTER F9;
//FILTER *pF9;
//FILTER F10;
//FILTER *pF10;
//FILTER F11;
//FILTER *pF11;
//FILTER F12;
//FILTER *pF12;

/******************************************************************************
*函数名称: InitFILTER
*参数：void
*返回值:void
*函数功能:滤波 
*****************************************************************************/
void InitFILTER(void)
{
	pF1 = &F1;
	pF2 = &F2;
	pF3 = &F3;
	pF4 = &F4;
//	pF5 = &F5;
//	pF6 = &F6;
//	pF7 = &F7;
//	pF8 = &F8;
//	pF9 = &F9;
//	pF10 = &F10;
//	pF11 = &F11;
//	pF12 = &F12;

	InitF(pF1);
	InitF(pF2);
	InitF(pF3);
	InitF(pF4);
//	InitF(pF5);
//	InitF(pF6);
//	InitF(pF7);
//	InitF(pF8);
//	InitF(pF9);
//	InitF(pF10);
//	InitF(pF11);
//	InitF(pF12);	
}
/*******************************************************************************
*函数名称: InitF
*参数：void
*返回值:void
*函数功能: 初始化结构体
*****************************************************************************/
void InitF(FILTER *pF)
{
   pF->total    = 0;
   pF->n        = 0;

}

/***************************************************************************
*函数名称: filter(u32 cmd)      
*参数：void
*返回值:void
*函数功能: 数字滤波器（Digital Filter）是由数字乘法器、加法器和延时单元组成的一种装置，
*                   其功能是对输入离散信号的数字代码进行运算处理，以达到改变信号频谱的目的。
*                   取N个采样值求平均，RAM中要开辟N个数据的暂存区。每采集一个数据都要存入暂存区，
*                   同时去掉一个最早的数据，保持这N个数据始终是最近的数据[5-6].
*****************************************************************************/
u32 filter(u8 num,u32 data)
{
	u32 tmp_filter;

	switch(num)
	{
		case 0:
			 tmp_filter = filter_num(pF1,data);
			 break;
		case 1:
			 tmp_filter = filter_num(pF2,data);
			 break;
		case 2:
			 tmp_filter = filter_num(pF3,data);
			 break;
		case 3:
			 tmp_filter = filter_num(pF4,data);
			 break;
//		case 4:
//			 tmp_filter = filter_num(pF5,data);
//			 break;
//		case 5:
//			 tmp_filter = filter_num(pF6,data);
//			 break;
//		case 6:
//			 tmp_filter = filter_num(pF7,data);
//			 break;
//		case 7:
//			 tmp_filter = filter_num(pF8,data);
//			 break;
//		case 8:
//			 tmp_filter = filter_num(pF9,data);
//			 break;
//		case 9:
//			 tmp_filter = filter_num(pF10,data);
//			 break;
//		case 10:
//			 tmp_filter = filter_num(pF11,data);
//			 break;
//		case 11:
//			 tmp_filter = filter_num(pF12,data);
//			 break;	
	}

	return tmp_filter;
	 	   
}
/*******************************************************************************
*函数名称: filter_num
*参数：void
*返回值:void
*函数功能: 滤波
*****************************************************************************/
u32 filter_num(FILTER *pF,u32 data)
{
	u32 tmp_fil;
	pF->total -= pF->buff[pF->n];		 //滤波缓冲区的大小
  pF->buff[pF->n] = data;
  pF->n++;
  if( pF->n == FIL_LEN)
  {
	   pF->n = 0;
	}
	pF->total += data;
	
	tmp_fil = pF->total/FIL_LEN;

	return tmp_fil;
	 	   
}




