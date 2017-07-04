/*
 * COPYRIGHT (c) 2010-2012 MACRONIX INTERNATIONAL CO., LTD
 * SPI Flash Low Level Driver (LLD) Sample Code
 *
 * SPI interface command hex code, type definition and function prototype.
 *
 * $Id: MX25_CMD.h,v 1.14 2012/02/05 22:27:09 mxclldb1 Exp $
 */
#ifndef    __MX25_CMD_H__
#define    __MX25_CMD_H__

/*
  Compiler Option
*/

/* Select your flash device type */

///#include "stm32f10x.h"

/* Note:
   Synchronous IO     : MCU will polling WIP bit after
                        sending prgram/erase command
   Non-synchronous IO : MCU can do other operation after
                        sending prgram/erase command
   Default is synchronous IO
*/
//#define    NON_SYNCHRONOUS_IO

/*
  Type and Constant Define
*/


//=================================================================================================
//*************************************************************************************************
//宏定义

// variable
#define    TRUE     1
#define    FALSE    0
#define    BYTE_LEN          8
#define    IO_MASK           0x80
#define    HALF_WORD_MASK    0x0000ffff

/*
  Flash Related Parameter Define
*/

#define    Block_Offset       0x10000     // 64K Block size
#define    Block32K_Offset    0x8000      // 32K Block size
#define    Sector_Offset      0x1000      // 4K Sector size
#define    Page_Offset        0x0100      // 256 Byte Page size
#define    Page32_Offset      0x0020      // 32 Byte Page size (some products have smaller page size)
#define    Block_Num          (FlashSize / Block_Offset)

// Flash control register mask define
// status register
#define    FLASH_WIP_MASK         0x01
#define    FLASH_LDSO_MASK        0x02
#define    FLASH_QE_MASK          0x40
// security register
#define    FLASH_OTPLOCK_MASK     0x03
#define    FLASH_4BYTE_MASK       0x04
#define    FLASH_WPSEL_MASK       0x80
// configuration reigster
#define    FLASH_DC_MASK          0x80
// other
#define    BLOCK_PROTECT_MASK     0xff
#define    BLOCK_LOCK_MASK        0x01

/* Timer Parameter */
//#define  TIMEOUT    1
//#define  TIMENOTOUT 0
//#define  TPERIOD    240             // ns, The time period of timer count one
//#define  COUNT_ONE_MICROSECOND  16  // us, The loop count value within one micro-second


/*
  System Information Define
*/

#define    CLK_PERIOD             72//20     // unit: ns
#define    Min_Cycle_Per_Inst     12     // use 12T 8051
#define    One_Loop_Inst          8      // instruction count of one loop (estimate)


/**************************************************
*   SPI1_NSS	    PB10	        
*   SPI1_SCK	    PD7	         
*   SPI1_MISO	    PC1         
*   SPI1_MOSI	    PC4     	    
*********************************************/ 
#define   MX25_SI_LO()    GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define   MX25_SI_HI()    GPIO_SetBits(GPIOC, GPIO_Pin_4)  

#define   MX25_CS_LO()    GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define   MX25_CS_HI()    GPIO_SetBits(GPIOB, GPIO_Pin_10)

#define   MX25_CLK_LO()   GPIO_ResetBits(GPIOD, GPIO_Pin_7)
#define   MX25_CLK_HI()   GPIO_SetBits(GPIOD, GPIO_Pin_7)

#define   MX25_SO_LO()    GPIO_ResetBits(GPIOC, GPIO_Pin_1)
#define   MX25_SO_HI()    GPIO_SetBits(GPIOC, GPIO_Pin_1)  

#define   MX25_SO()       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)                // (IO0PIN & BIT19)¶ÁÈ¡×´Ì¬Î»



/*
  Flash ID, Timing Information Define
  (The following information could get from device specification)
*/


#define    FlashID          0xc22017
#define    ElectronicID     0x16
#define    RESID0           0xc216
#define    RESID1           0x16c2
#define    FlashSize        0x200000       // 8 MB
#define    CE_period        41666667       // tCE /  ( CLK_PERIOD * Min_Cycle_Per_Inst *One_Loop_Inst)
#define    tW               100000000      // 100ms
#define    tDP              10000          // 10us
#define    tBP              300000         // 300us
#define    tPP              5000000        // 5ms
#define    tSE              300000000      // 300ms
#define    tBE              2000000000     // 2s
#define    tPUW             10000000       // 10ms
#define    tWSR             tBP
// Support I/O mode
#define    SIO              0
#define    DIO              1


// Flash information define
#define    WriteStatusRegCycleTime     tW / (CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#define    PageProgramCycleTime        tPP / (CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#define    SectorEraseCycleTime        tSE / (CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#define    BlockEraseCycleTime         tBE / (CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#define    ChipEraseCycleTime          CE_period
#define    FlashFullAccessTime         tPUW / (CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)

#ifdef tBP
#define    ByteProgramCycleTime        tBP / (CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#endif
#ifdef tWSR
#define    WriteSecuRegCycleTime       tWSR / (CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#endif
#ifdef tBE32
#define    BlockErase32KCycleTime      tBE32 / (CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#endif
#ifdef tWREAW
#define    WriteExtRegCycleTime        tWREAW / (CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#endif

/*** MX25 series command hex code definition ***/
//ID comands
#define    FLASH_CMD_RDID      0x9f    //RDID (Read Identification)
#define    FLASH_CMD_RES       0xAB    //RES (Read Electronic ID)
#define    FLASH_CMD_REMS      0x90    //REMS (Read Electronic & Device ID)

//Register comands
#define    FLASH_CMD_WRSR      0x01    //WRSR (Write Status Register)
#define    FLASH_CMD_RDSR      0x05    //RDSR (Read Status Register)
#define    FLASH_CMD_WRSCUR    0x2F    //WRSCUR (Write Security Register)
#define    FLASH_CMD_RDSCUR    0x2B    //RDSCUR (Read Security Register)

//READ comands
#define    FLASH_CMD_READ        0x03    //READ (1 x I/O)
#define    FLASH_CMD_FASTREAD    0x0B    //FAST READ (Fast read data)
#define    FLASH_CMD_DREAD       0x3B    //DREAD (1In/2 Out fast read)
#define    FLASH_CMD_RDSFDP      0x5A    //RDSFDP (Read SFDP)

//Program comands
#define    FLASH_CMD_WREN     0x06    //WREN (Write Enable)
#define    FLASH_CMD_WRDI     0x04    //WRDI (Write Disable)
#define    FLASH_CMD_PP       0x02    //PP (page program)

//Erase comands
#define    FLASH_CMD_SE       0x20    //SE (Sector Erase)
#define    FLASH_CMD_BE       0xD8    //BE (Block Erase)
#define    FLASH_CMD_CE       0x60    //CE (Chip Erase) hex code: 60 or C7

//Mode setting comands
#define    FLASH_CMD_DP       0xB9    //DP (Deep Power Down)
#define    FLASH_CMD_RDP      0xAB    //RDP (Release form Deep Power Down)
#define    FLASH_CMD_ENSO     0xB1    //ENSO (Enter Secured OTP)
#define    FLASH_CMD_EXSO     0xC1    //EXSO  (Exit Secured OTP)

//Reset comands

//Security comands

//Suspend/Resume comands

//=================================================================================================
//*************************************************************************************************
//数据结构定义

// define type
// typedef    unsigned long     uint32_t;
// typedef    unsigned int      uint16_t;
// typedef    unsigned char     uint8_t;
///typedef    unsigned char     BOOL;
 
// Return Message
typedef enum {
    FlashOperationSuccess,
    FlashWriteRegFailed,
    FlashTimeOut,
    FlashIsBusy,
    FlashQuadNotEnable,
    FlashAddressInvalid,
}ReturnMsg;

// Flash status structure define
struct sFlashStatus{
    /* Mode Register:
     * Bit  Description
     * -------------------------
     *  7   RYBY enable
     *  6   Reserved
     *  5   Reserved
     *  4   Reserved
     *  3   Reserved
     *  2   Reserved
     *  1   Parallel mode enable
     *  0   QPI mode enable
    */
    uint8_t    ModeReg;
    BOOL     ArrangeOpt;
};

typedef struct sFlashStatus FlashStatus;

//=================================================================================================
//*************************************************************************************************
//函数声明
/* Basic functions */
void CS_High(void);
void CS_Low(void);
void InsertDummyCycle( uint8_t dummy_cycle );
void SendByte( uint8_t byte_value );
uint8_t GetByte( void );

/* Utility functions */
void Wait_Flash_WarmUp(void);
void InitSimuSPI(void);
BOOL WaitFlashReady( uint32_t ExpectTime );
BOOL WaitRYBYReady( uint32_t ExpectTime );
BOOL IsFlashBusy( void );
BOOL IsFlashQIO( void );
BOOL IsFlash4Byte( void );
void SendFlashAddr( uint32_t flash_address, BOOL addr_mode );
void TestMX25LPin(void);
uint8_t FlashID_Test(void);
uint8_t FlashReadWrite_Test(void);
int Flash_Write(uint32_t flash_addr, uint8_t* pb, uint32_t buff_size);
int Flash_Read(uint32_t flash_addr, uint8_t* pb, uint32_t buff_size);
uint8_t ReadFlashID(uint32_t  *id);

/* Flash commands */
ReturnMsg CMD_RDID( uint32_t *Identification );
ReturnMsg CMD_RES( uint8_t *ElectricIdentification );
ReturnMsg CMD_REMS( uint16_t *REMS_Identification, FlashStatus *fsptr );

ReturnMsg CMD_RDSR( uint8_t *StatusReg );
ReturnMsg CMD_WRSR( uint8_t UpdateValue );

ReturnMsg CMD_RDSCUR( uint8_t *SecurityReg );
ReturnMsg CMD_WRSCUR( void );

ReturnMsg CMD_READ( uint32_t flash_address, uint8_t *target_address, uint32_t byte_length  );
ReturnMsg CMD_FASTREAD( uint32_t flash_address, uint8_t *target_address, uint32_t byte_length );
ReturnMsg CMD_RDSFDP( uint32_t flash_address, uint8_t *target_address, uint32_t byte_length );

ReturnMsg CMD_WREN( void );
ReturnMsg CMD_WRDI( void );
ReturnMsg CMD_PP( uint32_t flash_address, uint8_t *source_address, uint32_t byte_length );

ReturnMsg CMD_SE( uint32_t flash_address );
ReturnMsg CMD_BE( uint32_t flash_address );
ReturnMsg CMD_CE( void );

ReturnMsg CMD_DP( void );
ReturnMsg CMD_RDP( void );
ReturnMsg CMD_ENSO( void );
ReturnMsg CMD_EXSO( void );


//ReturnMsg CMD_PLLK( uint8 UpdateValue );
#endif    /* __MX25_CMD_H__ */
