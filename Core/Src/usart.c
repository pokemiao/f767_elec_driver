/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <string.h>

#define Getstartcmd() (strcmp((char *)g_rxBuffer, startcmd) == 0)
#define Getstopcmd() (strcmp((char *)g_rxBuffer, stopcmd) == 0)
#define Getoptocmd() (strcmp((char *)g_rxBuffer, optocmd) == 0)
#define Geteleccmd() (strcmp((char *)g_rxBuffer, eleccmd) == 0)
#define Getmixcmd() (strcmp((char *)g_rxBuffer, mix) == 0)

uint8_t uart_data = 0;
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile unsigned short rxIndex = 0; /* Index of the memory to save new arrived data. */

uint8_t rxComplete = 0;
char startcmd[] = "start";
char stopcmd[] = "stop";
char optocmd[] = "opto";
char eleccmd[] = "elec";
char mix[] = "mix";

extern uint8_t program_stage;

void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
  /* Check if data is received */
  if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE))
  {
    /* Receive one byte of data */
    uart_data = huart3.Instance->RDR;
    if (uart_data != '\n' && ((rxIndex + 1) % ECHO_BUFFER_LENGTH) != 0)
    {
      g_rxBuffer[rxIndex] = uart_data;
      rxIndex++;
      rxIndex %= ECHO_BUFFER_LENGTH;
    }
    else
    {
      g_rxBuffer[rxIndex] = '\0';
      rxComplete = 1;
      rxIndex = 0;
      // Process commands
      if (Getstartcmd())
      {
        program_stage = 1;
        rxComplete = 0;
        memset(g_rxBuffer, 0, sizeof(g_rxBuffer));
      }
      else if (Getstopcmd())
      {
        program_stage = 2;
        rxComplete = 0;
        memset(g_rxBuffer, 0, sizeof(g_rxBuffer));
      }
      else if (Getoptocmd())
      {
        program_stage = 3;
        rxComplete = 0;
        memset(g_rxBuffer, 0, sizeof(g_rxBuffer));
      }
      else if (Geteleccmd())
      {
        program_stage = 4;
        rxComplete = 0;
        memset(g_rxBuffer, 0, sizeof(g_rxBuffer));
      }
      else if (Getmixcmd())
      {
        program_stage = 5;
        rxComplete = 0;
        memset(g_rxBuffer, 0, sizeof(g_rxBuffer));
      }
    }
  }

  //  HAL_UART_Receive_IT(&huart3, &uart_data, 1);
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/* USER CODE END 0 */

UART_HandleTypeDef huart3;

/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
  __HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);
  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */
  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, STLK_RX_Pin|STLK_TX_Pin);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

uint8_t getCmd(char *ed)
{
  if (rxComplete)
  {
    __disable_irq();
    strcpy(ed, (char *)g_rxBuffer);
    rxComplete = 0;
    memset(g_rxBuffer, 0, sizeof(g_rxBuffer));
    __enable_irq();
    return 1;
  }
  else
    return 0;
}

void sendValueThroughUART(uint16_t *value, uint8_t length)
{
  uint8_t data[2 * length];
  for (int i = 0; i < length; i++)
  {
    data[2 * i] = (value[i] >> 8) & 0xFF; // high byte
    data[2 * i + 1] = value[i] & 0xFF;    // low byte
  }
  // UART_WriteBlocking(UART_PC, data, 2 * length);
  HAL_UART_Transmit(&huart3, data, 2 * length, 100);
}

void parseCmd_O(char *ed, char *O_Prd_s, char *O_PTime_s, char *O_Cur1, char *O_Cur2,
 char *Mode_s, char *O_PulsePeriod_s, char *O_Repeat_s, char *O_StimPeriod_s, char *O_Delay_s)
{
    char *token;
    token = strtok(ed, ",");
    if (token != NULL)
    {
        strcpy(O_Prd_s, token);
    }

    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(O_PTime_s, token);
    }

    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(O_Cur1, token);
    }

    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(O_Cur2, token);
    }

    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(Mode_s, token);
    }
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(O_PulsePeriod_s, token);
    }
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(O_Repeat_s, token);
    }
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(O_StimPeriod_s, token);
    }
    
}

void parseCmd_E(char *ed, char *E_Prd_s, char *E_PTime_s, char *E_Cur1, char *E_Cur2,
 char *Src_Site_s, char *CathodType_s, char *DstSite_s, char *Mode_s, 
 char *E_PulsePeriod_s, char *E_Repeat_s, char *E_StimPeriod_s, char *E_Delay_s)
{
    char *token;
    token = strtok(ed, ",");
    if (token != NULL)
    {
        strcpy(E_Prd_s, token);
    }

    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(E_PTime_s, token);
    }

    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(E_Cur1, token);
    }

    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(E_Cur2, token);
    }
    
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(Src_Site_s, token);
    }
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(CathodType_s, token);
    }
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(DstSite_s, token);
    }
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(Mode_s, token);
    }
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(E_PulsePeriod_s, token);
    }
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(E_Repeat_s, token);
    }
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(E_StimPeriod_s, token);
    }
    token = strtok(NULL, ",");
    if (token != NULL)
    {
        strcpy(E_Delay_s, token);
    }

}

/* USER CODE END 1 */
