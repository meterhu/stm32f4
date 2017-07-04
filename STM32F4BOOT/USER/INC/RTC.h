/**
  ******************************************************************************
  * @file    RTC/TimeStamp/main.h 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    13-April-2012
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
//#include "stm324xg_eval.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void InitRTC(void);
void RTC_Config(void);
void RTC_TimeRegulate(void);
void RTC_TimeShow(void);
void RTC_DateShow(void);
void RTC_TimeStampShow(void);
uint8_t USART_Scanf(uint32_t MinValue, uint32_t MaxValue);
void RTC_TimeStampData(void);
u8 RTC_Set(RTC_DateTypeDef *RTC_Date,RTC_TimeTypeDef *RTC_Time);
void  ReadBackRAM(u8 off_addr,u32 *buff, u8 size);
void  WriteBackRAM(u8 off_addr,u32 *buff, u8 size);

void RTC_AlarmConfig(void);


 #endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
