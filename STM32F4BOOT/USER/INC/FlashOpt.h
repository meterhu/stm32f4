#ifndef  __FLASH_OPT_H
#define  __FLASH_OPT_H


//=================================================================================================
//*************************************************************************************************
//宏定义
#define ADDR_FLASH_SECTOR_0    0x08000000
#define ADDR_FLASH_SECTOR_1    0x08004000
#define ADDR_FLASH_SECTOR_2    0x08008000
#define ADDR_FLASH_SECTOR_3    0x0800C000
#define ADDR_FLASH_SECTOR_4    0x08010000
#define ADDR_FLASH_SECTOR_5    0x08020000
#define ADDR_FLASH_SECTOR_6    0x08040000
#define ADDR_FLASH_SECTOR_7    0x08060000
#define ADDR_FLASH_SECTOR_8    0x08080000
#define ADDR_FLASH_SECTOR_9    0x080A0000
#define ADDR_FLASH_SECTOR_10   0x080C0000
#define ADDR_FLASH_SECTOR_11   0x080E0000


#define BOOT           1
#define APP            2
#define RESERVE        3
#define UNUSED         4

//=================================================================================================
//*************************************************************************************************
//数据类型定义
typedef struct _FLASH_INFO
{
    uint32_t  flash_sector;
    uint32_t  flash_addr;
    uint32_t  flash_sector_size;
    uint8_t   flash_sector_flag;
   
}FLASH_INFO;

typedef struct _FLASH_SECTOR_SIZE
{
    uint8_t front;
    uint8_t near;
   
}FLASH_SECTOR_SIZE;

//=================================================================================================
//*************************************************************************************************
//外部函数声明
extern uint16_t ProgramFlashErase(uint32_t addr, uint32_t size_t);
extern uint16_t CheckProgramData(uint32_t addr, uint32_t len, uint8_t ca, uint8_t cb);
extern uint16_t  Data2Flash(uint32_t addr, uint8_t *buff, uint32_t size_t);

uint32_t GetSector(uint32_t Address);
uint16_t GetSectorSize(uint32_t addr, uint32_t size_t, FLASH_SECTOR_SIZE *pz);

#endif  //__FLASH_OPT_H


