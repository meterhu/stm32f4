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
*��������: InitFILTER
*������void
*����ֵ:void
*��������:�˲� 
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
*��������: InitF
*������void
*����ֵ:void
*��������: ��ʼ���ṹ��
*****************************************************************************/
void InitF(FILTER *pF)
{
   pF->total    = 0;
   pF->n        = 0;

}

/***************************************************************************
*��������: filter(u32 cmd)      
*������void
*����ֵ:void
*��������: �����˲�����Digital Filter���������ֳ˷������ӷ�������ʱ��Ԫ��ɵ�һ��װ�ã�
*                   �书���Ƕ�������ɢ�źŵ����ִ���������㴦���Դﵽ�ı��ź�Ƶ�׵�Ŀ�ġ�
*                   ȡN������ֵ��ƽ����RAM��Ҫ����N�����ݵ��ݴ�����ÿ�ɼ�һ�����ݶ�Ҫ�����ݴ�����
*                   ͬʱȥ��һ����������ݣ�������N������ʼ�������������[5-6].
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
*��������: filter_num
*������void
*����ֵ:void
*��������: �˲�
*****************************************************************************/
u32 filter_num(FILTER *pF,u32 data)
{
	u32 tmp_fil;
	pF->total -= pF->buff[pF->n];		 //�˲��������Ĵ�С
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




