/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */
#define ECHO_BUFFER_LENGTH 64
/* USER CODE END Private defines */

void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
uint8_t getCmd(char *ed);
void sendValueThroughUART(uint16_t *value, uint8_t length);
void parseCmd_O(char *ed, char *O_Prd_s, char *O_PTime_s, char *O_Cur1, char *O_Cur2,
                char *Mode_s, char *O_PulsePeriod_s, char *O_Repeat_s, char *O_StimPeriod_s, char *O_Delay_s);
void parseCmd_E(char *ed, char *E_Prd_s, char *E_PTime_s, char *E_Cur1, char *E_Cur2, char *Src_Site_s, char *CathodType_s, char *DstSite_s,
                char *Mode_s, char *E_PulsePeriod_s, char *E_Repeat_s, char *E_StimPeriod_s, char *E_Delay_s);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

