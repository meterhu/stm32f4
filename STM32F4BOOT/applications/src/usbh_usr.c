/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file includes the usb host library user callbacks
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_usr.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"

#include "ff.h"       /* FATFS */

#include "string.h"

uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;

FATFS fatfs;
FIL fil;

uint8_t MscRxTxData[512]={0};
uint32_t wTimes=0;
uint8_t MscData[36];

/** @defgroup USBH_USR_Private_Macros
* @{
*/ 
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
/**
* @}
*/ 
static void HexToAssic(void);

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_cb =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
    
};

/**
* @brief  USBH_USR_Init 
*
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //open PA clock.
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOA, &GPIO_InitStructure); //button--for MSC.
}

/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
  USBH_USR_ApplicationState = USH_USR_FS_INIT;
}

/**
* @brief  USBH_USR_DeviceAttached 
*         
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{

}

/**
* @brief  USBH_USR_ResetUSBDevice 
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* callback for USB-Reset */
}

/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Staus
*/
void USBH_USR_DeviceDisconnected (void)
{

}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Over Current Detected on VBUS
* @param  None
* @retval Staus
*/
void USBH_USR_OverCurrentDetected (void)
{

}

/**
* @brief  USBH_USR_DeviceSpeedDetected 
*         
* @param  Device speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  	if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  	{

  	}
  	else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  	{

  	}
  	else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  	{

  	}
  	else
  	{

  	}
}

/**
* @brief  USBH_USR_Device_DescAvailable 
*         
* @param  device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 

}

/**
* @brief  USBH_USR_DeviceAddressAssigned 
*          
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{
  
}

/**
* @brief  USBH_USR_Conf_Desc 
*         
* @param  Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
    
}

/**
* @brief  USBH_USR_Manufacturer_String 
*          
* @param  Manufacturer String 
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{

}

/**
* @brief  USBH_USR_Product_String 
*         
* @param  Product String
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
  
}

/**
* @brief  USBH_USR_SerialNum_String 
*          
* @param  SerialNum_String 
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
    
}

/**
* @brief  EnumerationDone 
*         
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{ 
  
}

/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{
	USBH_USR_Status usbh_usr_status=USBH_USR_NO_RESP;
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)
	{
    	usbh_usr_status = USBH_USR_RESP_OK; 
	}
    return usbh_usr_status;
}

/**
* @brief  USBH_USR_MSC_Application 
*         Demo application for mass storage
* @param  None
* @retval Staus
*/
int USBH_USR_MSC_Application(void)
{
	FRESULT res;
	UINT br, bw;
	
	switch(USBH_USR_ApplicationState)
	{
		case USH_USR_FS_INIT:
			res = f_mount(0, &fatfs);
			if(res != FR_OK)
			{
				return (-1); //error.
			}
			USBH_USR_ApplicationState = USH_USR_FS_READLIST;
			break;
			
		case USH_USR_FS_READLIST:
			res = f_open(&fil, "0:read.txt", FA_OPEN_EXISTING | FA_READ);
			if(res != FR_OK)
			{
				return (-1); //error.
			}
			
			res = f_read(&fil, MscRxTxData, 512, &br);
			if(res != FR_OK)
			{
				return (-1); //error.
			}
				
			res = f_close(&fil);
			if(res != FR_OK)
			{
				return (-1); //error.
			}
			
			USBH_USR_ApplicationState = USH_USR_FS_WRITEFILE;
			break;
			
		case USH_USR_FS_WRITEFILE:
			res = f_open(&fil, "0:write.txt", FA_OPEN_ALWAYS | FA_WRITE);
			if(res != FR_OK)
			{
				return (-1); //error.
			}
			
			res = f_lseek(&fil, f_size(&fil)); //point to end of file.
			if(res != FR_OK)
			{
				return (-1); //error.
			}
			
			wTimes++;
			
			HexToAssic();
			
			res = f_write(&fil, MscData, 36, &bw);
			if(res != FR_OK)
			{
				return (-1); //error.
			}
			
			res = f_write(&fil, MscRxTxData, 512, &bw);
			if(res != FR_OK)
			{ 
				return (-1);
			}
			
			res = f_close(&fil);
			if(res != FR_OK)
			{
				return (-1); //error.
			}
			
			USBH_USR_ApplicationState = USH_USR_FS_READLIST;			
			break;
		default:
			return (-1);
	}
	return 0;
}

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
 
}

/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError(void)
{

}

/* hex to assic, for USB flash disk storage */
extern uint32_t LisData;
static void HexToAssic(void)
{
	uint8_t temp;
	
	temp = (uint8_t)(LisData>>24); //ID.
	MscData[0] = temp / 100 + 0x30; 
	MscData[1] = temp / 10 % 10 + 0x30; 
	MscData[2] = temp % 10 + 0x30;
		
	temp = (uint8_t)LisData; //X-axis.
	MscData[4] = temp / 100 + 0x30; 
	MscData[5] = temp / 10 % 10 + 0x30; 
	MscData[6] = temp % 10 + 0x30;	
	
	temp = (uint8_t)(LisData>>8); //Y-axis.
	MscData[8] = temp / 100 + 0x30; 
	MscData[9] = temp / 10 % 10 + 0x30; 
	MscData[10] = temp % 10 + 0x30;		
		
	temp = (uint8_t)(LisData>>16); //Z-axis.
	MscData[12] = temp / 100 + 0x30; 
	MscData[13] = temp / 10 % 10 + 0x30; 
	MscData[14] = temp % 10 + 0x30;
	
	MscData[17] = 0x30; 
	MscData[18] = 0x30;
	MscData[19] = 0x30;

	temp = (uint8_t)(wTimes>>24); //write times.
	MscData[22] = temp / 100 + 0x30; 
	MscData[23] = temp / 10 % 10 + 0x30; 
	MscData[24] = temp % 10 + 0x30;
		
	temp = (uint8_t)(wTimes>>16); 
	MscData[25] = temp / 100 + 0x30; 
	MscData[26] = temp / 10 % 10 + 0x30; 
	MscData[27] = temp % 10 + 0x30;	
	
	temp = (uint8_t)(wTimes>>8); 
	MscData[28] = temp / 100 + 0x30; 
	MscData[29] = temp / 10 % 10 + 0x30; 
	MscData[30] = temp % 10 + 0x30;		
		
	temp = (uint8_t)wTimes; 
	MscData[31] = temp / 100 + 0x30; 
	MscData[32] = temp / 10 % 10 + 0x30; 
	MscData[33] = temp % 10 + 0x30;	
}

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
