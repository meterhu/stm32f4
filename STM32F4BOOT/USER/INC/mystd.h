//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            文件名称: Std.h             
 *
 *                摘要: 标准头文件
 *
 *            创建时间: 2006-09-20  
 *
 *            修改时间: 2006-09-20
 *            修改原因:
 *
 *            
 *
 *                作者: 杨卫华
 *
 *************************************************************************************************/
 //===============================================================================================
 

#ifndef _STD_H
#define _STD_H
 

//================================================================================================
//************************************************************************************************
//宏定义
#ifndef  NULL
  #define NULL 0
#endif       

#ifndef  TRUE_P
 #define  TRUE_P 1
#endif

#ifndef  FALSE_P
 #define  FALSE_P 0
#endif  

#ifndef  _TRUE
 #define  _TRUE 1
#endif

#ifndef  _FALSE
 #define  _FALSE 0
#endif 

#ifndef FALSE			
#define FALSE	0		
#endif
#ifndef TRUE
#define TRUE	1
#endif


//错误代码
#define  INVALID_DATA   -2
#define  PARAM_CMD_INVAILID  -6

#define BIT(n) (1 << (n))


#define BIT0   (1 << 0)
#define BIT1   (1 << 1)
#define BIT2   (1 << 2)
#define BIT3   (1 << 3)
#define BIT4   (1 << 4)
#define BIT5   (1 << 5)
#define BIT6   (1 << 6)
#define BIT7   (1 << 7)
#define BIT8   (1 << 8)
#define BIT9   (1 << 9)
#define BIT10  (1 << 10)
#define BIT11  (1 << 11)
#define BIT12  (1 << 12)
#define BIT13  (1 << 13)
#define BIT14  (1 << 14)
#define BIT15  (1 << 15)
#define BIT16  (1 << 16)
#define BIT17  (1 << 17)
#define BIT18  (1 << 18)
#define BIT19  (1 << 19)
#define BIT20  (1 << 20)
#define BIT21  (1 << 21)
#define BIT22  (1 << 22)
#define BIT23  (1 << 23)
#define BIT24  (1 << 24)
#define BIT25  (1 << 25)
#define BIT26  (1 << 26)
#define BIT27  (1 << 27)
#define BIT28  (1 << 28)
#define BIT29  (1 << 29)

#define BIT30  (1 << 30)
#define BIT31  ((UINT32)1 << 31)

    /* 7.18.1.1 */

    /* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __int64 int64_t;

    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

    /* 7.18.1.2 */

    /* smallest type of at least n bits */
    /* minimum-width signed integer types */
typedef   signed          char int_least8_t;
typedef   signed short     int int_least16_t;
typedef   signed           int int_least32_t;
typedef   signed       __int64 int_least64_t;

    /* minimum-width unsigned integer types */
typedef unsigned          char uint_least8_t;
typedef unsigned short     int uint_least16_t;
typedef unsigned           int uint_least32_t;
typedef unsigned       __int64 uint_least64_t;

    /* 7.18.1.3 */

    /* fastest minimum-width signed integer types */
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
typedef   signed       __int64 int_fast64_t;

    /* fastest minimum-width unsigned integer types */
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
typedef unsigned       __int64 uint_fast64_t;

    /* 7.18.1.4 integer types capable of holding object pointers */
typedef   signed           int intptr_t;
typedef unsigned           int uintptr_t;

    /* 7.18.1.5 greatest-width integer types */
typedef   signed       __int64 intmax_t;
typedef unsigned       __int64 uintmax_t;


#define  DIM(a)   (sizeof(a) / sizeof(a[0])) 
#define  MAX_I(a) ((sizeof(a) / sizeof(a[0])) - 1)

#define  UP_CHAR(ch)  (((ch) >= 'a') && ((ch) <= 'z')) ? (ch) -= ('a' - 'A'):(void)0 

#define  _GET_FILE_NAME(f)   #f
#define  GET_FILE_NAME(f)    _GET_FILE_NAME(f)


#define  NULL_P      ((UINT8 *)0)
#define  STR(a)      #a

#define  GOTO(addr)    ((void (*)())(addr))()


#define  CHG_STATE(s)   ((s) = !(s))  

#define  TIMER_CNT_MS       (4)

#define  TIMER_CNT_PER_S  (1000 / TIMER_CNT_MS)
 
//#define  WARNING(str);       pragma message(str)
//#define  ERROR(str)          error str

//#error("this variant is not use");

//#pragma  message

#define  _VAR_NAME(var0, var1)    (var0##var1) 

#define  _COMPILE_DATE  (__DATE__)
#define  _COMPILE_TIME  (__TIME__)  
#define  _FILE          (__FILE__)
#define  _LINE          (__LINE__)





#define  DIFF_V(a, b)  (((a) >= (b))?((a) - (b)):((b) - (a)))

#define  __BIGIN_BLOCK    {
#define  __END_BLOCK      }



//void  Trace(char *fmt, ...);
//void  Trace0(char *fmt, ...);
//
//#ifdef  __DEBUG
//#define  TRACE  Trace
////efine  TRACE0 
////extern void  TRACE(char *fmt, ...);
//#else
//
//#define  TRACE  1 ? (void)0 : Trace    //模仿VC, if (1)  (void)0  else Trace(...)
//
//#define  TRACE0  1 ? (void)0 : Trace0    //模仿VC, if (1)  (void)0  else Trace(...)
//
//#endif




#define ABS(v) (((v)>0) ? (v) : -(v))



//================================================================================================
//************************************************************************************************
//#define  G_DEF(H)   ##ifdef  G_#H ##define  EXT_LCD  ##else ##define  EXT_LCD   extern  ##endif
//#define  CONST const
#define  CONST  const


#define  ALIGN(n)   __align(n)
#define  PACKED     __packed

#define  AVOID_WARN(a)  (a) = (a)




//下面是无填充的结构体定义示例
//typedef __packed struct   _FramePacket  
//{
//    INT16  x;
//    INT16  y;
//
//}FRAME_PACKET;

                             
//================================================================================================
//************************************************************************************************
//数据类型定义

typedef	unsigned  char    UINT8;
typedef unsigned  short   UINT16;
typedef unsigned  int     UINT32;
typedef	signed    char    INT8;
typedef signed    short   INT16;
typedef signed    int     INT32;
typedef short     int     BOOL;
typedef	unsigned  char	  _BYTE;
typedef unsigned  short	  _WORD;
typedef unsigned  int	  _DWORD;
typedef unsigned  char    _BOOL;
typedef unsigned  char    UCHAR;
#ifdef __DEBUG

extern UINT8  Test8;
extern UINT32 Test32;
extern UINT16 Test16;
#else

#define  Test8    
#define  Test16
#define  Test32

#endif


typedef  struct _Size
{
    UINT16  w;
    UINT16  h;
}SIZE;

typedef struct _Point
{
    INT16  x;
    INT16  y;

}POINT;

typedef struct _Rect
{
    UINT16  x1;
    UINT16  y1;
    UINT16  x2;
    UINT16  y2;

}RECT;

typedef struct _WinRect
{
    int  x;
    int  y;
    int  w;
    int  h;

}WIN_RECT;


typedef struct  _Time
{
	UINT16  year;
	UINT8   month;
	UINT8   day;
	UINT8   weekday;
    UINT8   dayofyear;
	UINT8   hour;
	UINT8   min;
	UINT8   sec;
	
		
}TIME;


typedef struct  _DateTime
{
	INT16  year;
	INT8   mon;
	INT8   day;
	INT8   weekday;
    INT8   dayofyear;
	INT8   hour;
	INT8   min;
	INT8   sec;
	
		
}DATE_TIME;


//================================================================================================
//************************************************************************************************
//时钟计数器，可用于超时管理
/*
#define   TIMER_TICK_CNT_NUMS  10

typedef  struct  _TimerTickCnt
{
    volatile UINT32  cnt;
    UINT32   end_cnt;
    UINT8    used;
    UINT8    valid;
    UINT8    ov_flg;
    INT16    (*tm_out_prc)(void *pd);
    UINT32   param;

}TIMER_TICK_CNT;

extern  TIMER_TICK_CNT TimerTickCnt[];

#define   TIMER_TICK_CNT_OV(p)     (((p)->cnt > (p)->end_cnt) || ((p)->ov_flg))
#define   DEL_TIMER_TICK_CNT(p)    (p)->used  = 0; (p)->valid = 0
#define   RST_TIMER_TICK_CNT(p)    (p)->cnt   = 0; (p)->ov_flg = 0

#define   DIS_TIMER_TICK_CNT(p)    (p)->valid = 0
#define   ENA_TIMER_TICK_CNT(p)    (p)->valid = 1
#define   GET_TIMER_TICK_CNT(p)    (p)->cnt


#define   SET_TIMER_TICK_END_CNT(p, cnt)  (p)->end_cnt = (cnt)

//新建一个时间计数器
__inline   TIMER_TICK_CNT* NEW_TIMER_TICK_CNT(UINT32  end_cnt, INT16 (*tm_out_prc)(void *pd))
{
    UINT8  i;
    
    for (i = 0; i < TIMER_TICK_CNT_NUMS; i++)
    {
         if (!TimerTickCnt[i].used)
         {
             TimerTickCnt[i].used         = 1;
             TimerTickCnt[i].end_cnt      = end_cnt;
             TimerTickCnt[i].cnt          = 0; 
             TimerTickCnt[i].valid        = 0;
             TimerTickCnt[i].tm_out_prc   = tm_out_prc;
             return &TimerTickCnt[i];
             
         }
    
    }
    
    
    return NULL;


}

//时间计数器处理
__inline   void  TIMER_TICK_COUNT(void)
{
    UINT8  i;
    
    for (i = 0; i < TIMER_TICK_CNT_NUMS; i++)
    {
         if (TimerTickCnt[i].valid)
         {
             if (TimerTickCnt[i].cnt >= TimerTickCnt[i].end_cnt)  //TimerTickCnt[i].cnt > (UINT32)-2)
             {
                 TimerTickCnt[i].ov_flg = 1;
                 if (TimerTickCnt[i].tm_out_prc)  //调用超时处理函数
                 {
                     (*TimerTickCnt[i].tm_out_prc)((void *)&TimerTickCnt[i].param);
                    
                 }
                 
             }
             else
             {
                 TimerTickCnt[i].cnt++;
             }  
         }
    
    }


}

*/

extern void ResetHandler(void);


#define  RESET()   ResetHandler()
    

#define  NOT(datatype, data)            (~(datatype)(data))    

//屏蔽某一位:
//data:要屏蔽的数据
//databitwidth:数据类型
//bit_num:要屏蔽的位号
#define  MASK_BIT(data, datatype, bit_num)    (data) &= (~(datatype)((datatype)(0x01 << (bit_num)))) 
    
//================================================================================================
//************************************************************************************************
//宏函数定义
#define	OUT_PORTB(port, data)	 *((volatile UINT8 *)(port))  = (UINT8)(data)
#define	OUT_PORTW(port, data)	 *((volatile UINT16 *)(port)) = (UINT16)(data)
#define	OUT_PORT(port, data)	 *((volatile UINT32 *)(port)) = (UINT32)(data)
#define	IN_PORTB(port)			 *((volatile UINT8 *)(port))
#define	IN_PORTW(port)			 *((volatile UINT16 *)(port))
#define	IN_PORT(port)			 *((volatile UINT32 *)(port))

#define  SET_BIT_HI(r, bit)   (r) |= (1 << (bit))
#define  SET_BIT_LO(r, bit)   (r) &= ~(1 << (bit)) 
#define  GET_BIT(r, bit)      (((r) & (1 << (bit))) >> (bit)) 


#define  SWAP(a, b, type) do{type  tmp; tmp = a; a  = b; b  = tmp;}while(0)

extern  void  ARMDisINT(void);
extern  void  ARMEnaINT(void);

extern  UINT32    Bits[];

void   Message(char *msg);

#define  MAKE_WORD(u16_1, u16_2)   ((u16_1 << 16) | (UINT16)u16_2)
#define  MAKE_HWORD(u8_h, u8_l)    (((u8_h) << 8) | (UINT8)(u8_l))
#define  MAKE_4BYTE_WORD(u8_3, u8_2,u8_1, u8_0)    (((u8_3) << 24) | ((u8_2) << 16) |((u8_1) << 8) | (UINT8)(u8_0))


#define    RS32(data, bits)   ((UINT32)(data) >> (bits))  //32位操作数右移
#define    LS32(data, bits)   ((UINT32)(data) << (bits))  //32位操作数左移

#define    __MILLION     (1000 * 1000)

#endif  //_STD_H






