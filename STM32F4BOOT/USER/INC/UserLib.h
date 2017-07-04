//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            文件名称:              
 *
 *                摘要: 
 *
 *            创建时间: 2008-10-00  
 *
 *            修改时间: 2008-10-00
 *            修改原因:
 *
 *            
 *
 *                作者: 杨卫华
 *
 *************************************************************************************************/
 //===============================================================================================
 
#ifndef _USER_LIB_H
#define _USER_LIB_H

//=================================================================================================
//*************************************************************************************************
//数据类型定义
/*typedef  struct  _PORT_REG
{
    volatile unsigned *con;
    volatile unsigned *data;
    volatile unsigned *rup;
  

}PORT_REG;
*/

//extern PORT_REG  PortA_J[];



//=================================================================================================
//*************************************************************************************************
//宏定义
#define  INT_CLOSE   0x01
#define  INT_OPEN    0x02


#define  PORT_BASE 


#define  PORTA        0x0A
#define  PORTB        0x0B
#define  PORTC        0x0C
#define  PORTD        0x0D
#define  PORTE        0x0E
#define  PORTF        0x0F
#define  PORTG        0x10
#define  PORTH        0x11
#define  PORTJ        0x13

#define  GIO_IN_FUNC  0x00
#define  GIO_OUT_FUNC 0x01

#define  GIO_3_FUNC   0x02
#define  GIO_4_FUNC   0x03

 
//#define  SET_GIO_IN_FUN(r_hnm,bit)   do{r &= (~((UINT32)(0x3 << (bit << 1)))); r |= ((GIO_IN_FUNC) << (bit << 1))}while(0)
//#define  SET_GIO_3_FUN(r_hnm,bit)    do{r &= (~((UINT32)(0x3 << (bit << 1)))); r |= ((GIO_3_FUNC) << (bit << 1))}while(0)
//#define  SET_GIO_4_FUN(r_hnm,bit)    do{r &= (~((UINT32)(0x3 << (bit << 1)))); r |= ((GIO_4_FUNC) << (bit << 1))}while(0)

//#define  SET_GIO_OUT_FUN(r,bit)  do{r &= (~((UINT32)(0x3 << (bit << 1)))); r |= ((GIO_OUT_FUNC) << (bit << 1))}while(0)
//#define  SET_GIO_IN_FUN(r,bit)   do{r &= (~((UINT32)(0x3 << (bit << 1)))); r |= ((GIO_IN_FUNC) << (bit << 1))}while(0)
//#define  SET_GIO_3_FUN(r,bit)    do{r &= (~((UINT32)(0x3 << (bit << 1)))); r |= ((GIO_3_FUNC) << (bit << 1))}while(0)
//#define  SET_GIO_4_FUN(r,bit)    do{r &= (~((UINT32)(0x3 << (bit << 1)))); r |= ((GIO_4_FUNC) << (bit << 1))}while(0)

//#define  SET_GIO_FUN(port,bit,func)   do{*(PortA_J[port - 0xA].con) &= (~((UINT32)(0x3 << (bit << 1)))); *(PortA_J[port - 0xA].con) |= ((func) << (bit << 1));}while(0)

#define  SET_GIO_FUN(port,bit,func)   do{(port) &= (~((UINT32)((UINT32)0x3 << ((bit) << 1)))); (port) |= ((UINT32)(func) << ((bit) << 1));}while(0) //(UINT32)0x3如果没有UINT32,则会报警

//#define  DIS_GIO_UPR(port,bit)     (*(PortA_J[port - 0xA].rup) |= ((GIO_4_FUNC) << bit))
//#define  ENA_GIO_UPR(port,bit)     (*(PortA_J[port - 0xA].rup) &= (~((UINT32)(0x1 << bit))))

//#define  SET_PORT_HI(port, bit)    (*(PortA_J[port - 0xA].data) |= (1 << (bit)))
//#define  SET_PORT_LO(port, bit)    (*(PortA_J[port - 0xA].data) &= ~((UINT32)(1 << (bit))))

//========================================================================================
//
//上拉电阻设定
#define  DIS_PB_UPR(bit)     rGPBUP |= ((0x01) << bit)
#define  ENA_PB_UPR(bit)     rGPBUP &= (~((UINT32)(0x1 << bit)))

#define  DIS_PC_UPR(bit)     rGPCUP |= ((0x01) << bit)
#define  ENA_PC_UPR(bit)     rGPCUP &= (~((UINT32)(0x1 << bit)))

#define  DIS_PD_UPR(bit)     rGPDUP |= ((0x01) << bit)
#define  ENA_PD_UPR(bit)     rGPDUP &= (~((UINT32)(0x1 << bit)))

#define  DIS_PE_UPR(bit)     rGPEUP |= ((0x01) << bit)
#define  ENA_PE_UPR(bit)     rGPEUP &= (~((UINT32)(0x1 << bit)))

#define  DIS_PF_UPR(bit)     rGPFUP |= ((0x01) << bit)
#define  ENA_PF_UPR(bit)     rGPFUP &= (~((UINT32)(0x1 << bit)))

#define  DIS_PG_UPR(bit)     rGPGUP |= ((0x01) << bit)
#define  ENA_PG_UPR(bit)     rGPGUP &= (~((UINT32)(0x1 << bit)))

#define  DIS_PH_UPR(bit)     rGPHUP |= ((0x01) << bit)
#define  ENA_PH_UPR(bit)     rGPHUP &= (~((UINT32)(0x1 << bit)))

#define  DIS_PJ_UPR(bit)     rGPJUP |= ((0x01) << bit)
#define  ENA_PJ_UPR(bit)     rGPJUP &= (~((UINT32)(0x1 << bit)))



//========================================================================================
//rGPBUP
//IO功能口设定
#define  SET_PA_FUN(bit,func)    SET_GIO_FUN(rGPACON,bit,func)
#define  SET_PB_FUN(bit,func)    SET_GIO_FUN(rGPBCON,bit,func)
#define  SET_PC_FUN(bit,func)    SET_GIO_FUN(rGPCCON,bit,func)
#define  SET_PD_FUN(bit,func)    SET_GIO_FUN(rGPDCON,bit,func)
#define  SET_PE_FUN(bit,func)    SET_GIO_FUN(rGPECON,bit,func)
#define  SET_PF_FUN(bit,func)    SET_GIO_FUN(rGPFCON,bit,func)
#define  SET_PG_FUN(bit,func)    SET_GIO_FUN(rGPGCON,bit,func)
#define  SET_PH_FUN(bit,func)    SET_GIO_FUN(rGPHCON,bit,func)
#define  SET_PJ_FUN(bit,func)    SET_GIO_FUN(rGPJCON,bit,func)

//#define  SET_PA_FUN(bit,func)    SET_GIO_FUN(rGPACON,bit,func)
//#define  SET_PB_FUN(bit,func)    SET_GIO_FUN(rGPBCON,bit,func)
//#define  SET_PC_FUN(bit,func)    SET_GIO_FUN(rGPCCON,bit,func)
//#define  SET_PD_FUN(bit,func)    SET_GIO_FUN(rGPDCON,bit,func)
//#define  SET_PE_FUN(bit,func)    SET_GIO_FUN(rGPECON,bit,func)
//#define  SET_PF_FUN(bit,func)    SET_GIO_FUN(rGPFCON,bit,func)
//#define  SET_PG_FUN(bit,func)    SET_GIO_FUN(rGPGCON,bit,func)
//#define  SET_PH_FUN(bit,func)    SET_GIO_FUN(rGPHCON,bit,func)
//#define  SET_PJ_FUN(bit,func)    SET_GIO_FUN(rGPJCON,bit,func)


//========================================================================================
//
//IO高低电平设置
#define  SET_PA_HI(bit)    rGPADAT |= (1 << (bit))
#define  SET_PA_LO(bit)    rGPADAT &= ~((UINT32)(1 << (bit)))

#define  SET_PB_HI(bit)    rGPBDAT |= (1 << (bit))
#define  SET_PB_LO(bit)    rGPBDAT &= ~((UINT32)(1 << (bit)))


#define  SET_PC_HI(bit)    rGPCDAT |= (1 << (bit))
#define  SET_PC_LO(bit)    rGPCDAT &= ~((UINT32)(1 << (bit)))

#define  SET_PD_HI(bit)    rGPDDAT |= (1 << (bit))
#define  SET_PD_LO(bit)    rGPDDAT &= ~((UINT32)(1 << (bit)))

#define  SET_PE_HI(bit)    rGPEDAT |= (1 << (bit))
#define  SET_PE_LO(bit)    rGPEDAT &= ~((UINT32)(1 << (bit)))

#define  SET_PF_HI(bit)    rGPEDAT |= (1 << (bit))
#define  SET_PF_LO(bit)    rGPEDAT &= ~((UINT32)(1 << (bit)))

#define  SET_PG_HI(bit)    rGPGDAT |= (1 << (bit))
#define  SET_PG_LO(bit)    rGPGDAT &= ~((UINT32)(1 << (bit)))

#define  SET_PH_HI(bit)    rGPHDAT |= (1 << (bit))
#define  SET_PH_LO(bit)    rGPHDAT &= ~((UINT32)(1 << (bit)))

#define  SET_PJ_HI(bit)    rGPJDAT |= (1 << (bit))
#define  SET_PJ_LO(bit)    rGPJDAT &= ~((UINT32)(1 << (bit)))

//========================================================================================
//
//读取IO
#define  GET_PA(bit)    (((rGPADAT) & (1 << (bit))) >> (bit)) 
#define  GET_PB(bit)    (((rGPBDAT) & (1 << (bit))) >> (bit)) 
#define  GET_PC(bit)    (((rGPCDAT) & (1 << (bit))) >> (bit)) 
#define  GET_PD(bit)    (((rGPDDAT) & (1 << (bit))) >> (bit)) 
#define  GET_PE(bit)    (((rGPEDAT) & (1 << (bit))) >> (bit)) 
#define  GET_PF(bit)    (((rGPFDAT) & (1 << (bit))) >> (bit)) 
#define  GET_PG(bit)    (((rGPGDAT) & (1 << (bit))) >> (bit)) 
#define  GET_PH(bit)    (((rGPHDAT) & (1 << (bit))) >> (bit)) 
#define  GET_PJ(bit)    (((rGPJDAT) & (1 << (bit))) >> (bit)) 



//1--masked,0--enable interrupt,R/W
#define  OPEN_INT_BIT(bit)      rINTMSK    &= ~(UINT32)(bit)   
#define  CLOSE_INT_BIT(bit)     rINTMSK    |= (bit) 
#define  OPEN_SUB_INT_BIT(bit)  rINTSUBMSK &= ~(UINT32)(bit)
#define  CLOSE_SUB_INT_BIT(bit)  rINTSUBMSK |= (UINT32)(bit)
#define  CLEAR_SUB_INT_BIT(bit) rSUBSRCPND = (bit)
//=================================================================================================
//*************************************************************************************************
//变量定义

//========================================================================================
//
extern uint32_t SysTimeTickCnt;
#define  GET_SYS_TIMER_TICK()   (SysTimeTickCnt)

//=================================================================================================
//*************************************************************************************************
//外部函数声明
extern void   Delay(uint32_t  n); 
extern uint8_t  DisINT(void);
extern void   EnaINT(uint8_t old);
extern int MemoryCmp(void *s1, const void *s2, int n);
extern void MemoryCopy(void *s1, const void *s2, int n);
extern void MemorySet(void *s, const char ch, int n);
extern void MemoryHWordCopy(uint16_t *s1, const uint16_t *s2, int n);
extern UINT16 BCD2UINT16(uint32_t bcd);
extern void  Trace(char *fmt, ...);
extern _BOOL _memcmp(void *s1, const void *s2, int n);
extern void __memset(void *s, const char ch, int n);
extern void _memcpy(void *s1, const void *s2, int n);

extern void  DelayUS(int32_t us);
extern void  DelayMS(int32_t ms);
extern void  Trace1(char *fmt, ...);

#endif //_USER_LIB_H



