/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>
#include "stm32f7xx_it.h"
#include "elec.h"
#include "opto.h"
#include "myspi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// UART_HandleTypeDef huart3;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char msg[] = "Hello, UART!\n";
const uint8_t startconfirm[] = "started\n";

uint8_t program_stage = 0;
volatile uint8_t startprogram = 0;
volatile uint8_t ParReceive_enable = 1;

extern ADC_HandleTypeDef hadc1;
extern uint8_t ADC_sample_complete;
extern uint16_t adc_buf[ADC_BUF_LEN];

extern Site_t Site_group[3];
extern MYSPI_IO_t EVO;
// extern uint8_t VO_Group[4];
char E_DATARx[ECHO_BUFFER_LENGTH], E_PTime1_s[ECHO_BUFFER_LENGTH], E_PTime2_s[ECHO_BUFFER_LENGTH], E_Cur1_s[ECHO_BUFFER_LENGTH], E_Cur2_s[ECHO_BUFFER_LENGTH],
    E_PulsePeriod_s[ECHO_BUFFER_LENGTH], E_Repeat_s[ECHO_BUFFER_LENGTH], E_Delay_s[ECHO_BUFFER_LENGTH],
    O_DATARx[ECHO_BUFFER_LENGTH], O_PTime1_s[ECHO_BUFFER_LENGTH], O_PTime2_s[ECHO_BUFFER_LENGTH], O_Cur1_s[ECHO_BUFFER_LENGTH], O_Cur2_s[ECHO_BUFFER_LENGTH],
    O_PulsePeriod_s[ECHO_BUFFER_LENGTH], O_Repeat_s[ECHO_BUFFER_LENGTH], O_Delay_s[ECHO_BUFFER_LENGTH],
    Src_Site_s[ECHO_BUFFER_LENGTH], CathodType_s[ECHO_BUFFER_LENGTH], DstSite_s[ECHO_BUFFER_LENGTH], Mode_s[ECHO_BUFFER_LENGTH], StimPeriod_s[ECHO_BUFFER_LENGTH] = "";
volatile uint16_t E_PTime1, E_PTime2, E_CurA, E_CurB, E_CurC, E_CurD,
    E_PulsePeriod, E_Repeat, E_Delay,
    O_PTime1, O_PTime2, O_Cur1, O_Cur2,
    O_PulsePeriod, O_Repeat, O_Delay,
    Src_Site, CathodType, DstSite, Mode, StimPeriod = 0;

volatile uint8_t E_Stimulating = 0;
volatile uint8_t E_Sti_Count = 0;
volatile uint8_t E_SPeriod_flag = 0;
volatile uint8_t LPeriod_flag = 0;

volatile uint8_t O_Stimulating = 0;
volatile uint8_t O_Sti_Count = 0;
volatile uint8_t O_SPeriod_flag = 0;

volatile uint8_t Esticompleted = 0;
volatile uint8_t Ostimcompleted = 0;
volatile uint8_t O_stimallowed = 0;
volatile uint8_t E_stimallowed = 0;

uint8_t LED_first_flag = 1;
extern volatile uint8_t Opto_PhaseFlag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief This function handles TIM2 global interrupt.
 */
void TIM_Delay_IRQHandler(void)
{
  if (__HAL_TIM_GET_FLAG(&hTIM_Delay, TIM_FLAG_UPDATE) != RESET)
  {
    if (__HAL_TIM_GET_IT_SOURCE(&hTIM_Delay, TIM_IT_UPDATE) != RESET)
    {
    	HAL_TIM_Base_Stop_IT(&hTIM_Delay);
      __HAL_TIM_SET_COUNTER(&hTIM_Delay, 0);
      __HAL_TIM_CLEAR_FLAG(&hTIM_Delay, TIM_IT_UPDATE);
      if (LED_first_flag)
      {
        E_stimallowed = 1;
        __HAL_TIM_SET_AUTORELOAD(&hTIM_STP_Elec, E_PulsePeriod);
        __HAL_TIM_SET_COUNTER(&hTIM_STP_Elec, 0);
        HAL_TIM_Base_Start_IT(&hTIM_STP_Elec);
      }
      else
      {
        O_stimallowed = 1;
        __HAL_TIM_SET_AUTORELOAD(&hTIM_STP_Opto, O_PulsePeriod);
        __HAL_TIM_SET_COUNTER(&hTIM_STP_Opto, 0);
        HAL_TIM_Base_Start_IT(&hTIM_STP_Opto);
      }
    }
  }
}

/**
 * @brief This function handles TIM1 trigger and commutation interrupts and TIM11 global interrupt.
 */
void TIM_LTP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 0 */
  if (__HAL_TIM_GET_FLAG(&hTIM_LTP, TIM_FLAG_UPDATE) != RESET)
  {
    if (__HAL_TIM_GET_IT_SOURCE(&hTIM_LTP, TIM_IT_UPDATE) != RESET)
    {
      __HAL_TIM_SET_COUNTER(&hTIM_LTP, 0);
      __HAL_TIM_CLEAR_FLAG(&hTIM_LTP, TIM_IT_UPDATE);
      LPeriod_flag = 1;
    }
  }
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  MX_DAC_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_TIM7_Init();
  MX_TIM11_Init();
  MX_TIM12_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // HAL_ADC_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buf, ADC_BUF_LEN);

  HAL_GPIO_WritePin(EDAC_CLRSEL_GPIO_Port, EDAC_CLRSEL_Pin, GPIO_PIN_SET); // Set EDAC to middle voltage
  HAL_GPIO_WritePin(EDAC_CLR_GPIO_Port, EDAC_CLR_Pin, GPIO_PIN_RESET);     // Set EDAC to middle voltage
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (program_stage == 1)
    {
      startprogram = 1;
      ParReceive_enable = 1;
      HAL_TIM_Base_Start_IT(&hADC_Trigger); // ADC timer

      HAL_UART_Transmit(&huart3, (uint8_t *)&startconfirm, sizeof(startconfirm), HAL_MAX_DELAY);
    }

    while (startprogram)
    {
      switch (program_stage)
      {
      case 2:
        program_stage = 0;
        startprogram = 0;
        Opto_PhaseFlag = 1;
        O_Stimulating = 0;
        E_Stimulating = 0;

        HAL_TIM_Base_Stop_IT(&hADC_Trigger); // ADC timer

        HAL_TIM_Base_Stop_IT(&hTIM_STP_Opto);
        HAL_TIM_Base_Stop_IT(&hTIM_Pulse_Opto);

        HAL_TIM_Base_Stop_IT(&hTIM_STP_Elec);
        HAL_TIM_Base_Stop_IT(&hTIM_Pulse_Elec);

        HAL_TIM_Base_Stop_IT(&hTIM_Delay);
        HAL_TIM_Base_Stop_IT(&hTIM_LTP);

        HAL_GPIO_WritePin(EDAC_CLR_GPIO_Port, EDAC_CLR_Pin, GPIO_PIN_RESET); // Set EDAC to middle voltage
        break;
      case 3:
        if (ParReceive_enable)
        {
          ParReceive_enable = 0;
          while (!getCmd(O_DATARx))
            ;
          parseCmd_O(O_DATARx, O_PTime1_s, O_PTime2_s, O_Cur1_s, O_Cur2_s, Mode_s, O_PulsePeriod_s, O_Repeat_s, StimPeriod_s, O_Delay_s);
          O_PTime1 = atoi(O_PTime1_s) / 250 - 1;
          O_PTime2 = atoi(O_PTime2_s) / 250 - 1;
          O_Cur1 = 0x100 + atoi(O_Cur1_s);
          O_Cur2 = 0x100 + atoi(O_Cur2_s);
          Mode = atoi(Mode_s);
          O_PulsePeriod = atoi(O_PulsePeriod_s) / 250 - 1;

          O_Repeat = atoi(O_Repeat_s);
          StimPeriod = atoi(StimPeriod_s) - 1;

          if (Mode)
          {
            __HAL_TIM_SET_AUTORELOAD(&hTIM_LTP, StimPeriod);
            __HAL_TIM_SET_COUNTER(&hTIM_LTP, 0);
            HAL_TIM_Base_Start_IT(&hTIM_LTP);
          }

          __HAL_TIM_SET_AUTORELOAD(&hTIM_STP_Opto, O_PulsePeriod);
          __HAL_TIM_SET_COUNTER(&hTIM_STP_Opto, 0);
          HAL_TIM_Base_Start_IT(&hTIM_STP_Opto);
        }
        if (Mode)
        {
          if (!LPeriod_flag)
          {
            if (!O_SPeriod_flag)
            {
              if (O_Sti_Count < O_Repeat)
              {
                if (!O_Stimulating)
                {
                  O_Stimulating = 1;
                  if (!optostimulation())
                  {
                    O_Sti_Count++;
                    O_SPeriod_flag = 1;
                  }
                }
              }
              else
              {
                __HAL_TIM_DISABLE(&hTIM_STP_Opto);
              }
            }
          }
          else
          {
            LPeriod_flag = 0;
            O_SPeriod_flag = 0;
            O_Sti_Count = 0;
            __HAL_TIM_ENABLE(&hTIM_STP_Opto);
          }
        }
        else
        {
          if (!O_SPeriod_flag)
          {
            if (O_Sti_Count < O_Repeat)
            {
              if (!O_Stimulating)
              {
                O_Stimulating = 1;
                if (!optostimulation())
                {
                  O_Sti_Count++;
                  O_SPeriod_flag = 1;
                }
              }
            }
            else
            {
              program_stage = 1;
              ParReceive_enable = 1;
              O_SPeriod_flag = 0;
              O_Sti_Count = 0;
              HAL_TIM_Base_Stop_IT(&hTIM_STP_Opto);
            }
          }
        }
        break;
      case 4:
        if (ParReceive_enable)
        {
          ParReceive_enable = 0;
          while (!getCmd(E_DATARx))
            ;
          parseCmd_E(E_DATARx, E_PTime1_s, E_PTime2_s, E_Cur1_s, E_Cur2_s, Src_Site_s, CathodType_s, DstSite_s,
                     Mode_s, E_PulsePeriod_s, E_Repeat_s, StimPeriod_s, E_Delay_s);

          E_PTime1 = (uint16_t)atoi(E_PTime1_s) * 4 - 1 - 3;
          E_PTime2 = (uint16_t)atoi(E_PTime2_s) * 4 +1;
          E_CurD = (0b11 << 14) + (uint16_t)atoi(E_Cur1_s);        // Negtive High Current
          E_CurC = (0b10 << 14) + (uint16_t)atoi(E_Cur2_s);        // Negtive Low Current
          E_CurB = (0b01 << 14) + 4095 - (uint16_t)atoi(E_Cur1_s); // Positive High Current
          E_CurA = (0b00 << 14) + 4095 - (uint16_t)atoi(E_Cur2_s); // Positive Low Current
          Src_Site = (uint16_t)atoi(Src_Site_s);
          CathodType = (uint16_t)atoi(CathodType_s);
          DstSite = (uint16_t)atoi(DstSite_s);
          Mode = (uint16_t)atoi(Mode_s);
          E_PulsePeriod = (uint16_t)atoi(E_PulsePeriod_s) - 1;

          E_Repeat = (uint16_t)atoi(E_Repeat_s);
          StimPeriod = (uint16_t)atoi(StimPeriod_s) - 1;

          HAL_GPIO_WritePin(EDAC_CLR_GPIO_Port, EDAC_CLR_Pin, GPIO_PIN_SET);

          MySPI_SendByte2AD5726(EVO, E_CurA); // VO+ when switch PG14 to 1
          MySPI_SendByte2AD5726(EVO, E_CurB); // VO+ when switch PG14 to 0 (default)
          MySPI_SendByte2AD5726(EVO, E_CurC); // VO- when switch PG9 to 1
          MySPI_SendByte2AD5726(EVO, E_CurD); // VO- when switch PG9 to 0 (default)
          if (Mode)
          {
            __HAL_TIM_SET_AUTORELOAD(&hTIM_LTP, StimPeriod);
            __HAL_TIM_SET_COUNTER(&hTIM_LTP, 0);
            HAL_TIM_Base_Start_IT(&hTIM_LTP);
          }

          __HAL_TIM_SET_AUTORELOAD(&hTIM_STP_Elec, E_PulsePeriod);
          __HAL_TIM_SET_COUNTER(&hTIM_STP_Elec, 0);
          HAL_TIM_Base_Start_IT(&hTIM_STP_Elec);
        }
        if (Mode)
        {
          if (!LPeriod_flag)
          {
            if (!E_SPeriod_flag)
            {
              if (E_Sti_Count < E_Repeat)
              {
                if (!E_Stimulating)
                {
                  E_Stimulating = 1;
                  if (!elecstimulation(Site_group[Src_Site], Site_group[DstSite], CathodType))
                  {
                    E_Sti_Count++;
                    E_SPeriod_flag = 1;
                  }
                }
              }
              else
              {
                __HAL_TIM_DISABLE(&hTIM_STP_Elec);
              }
            }
          }
          else
          {
            LPeriod_flag = 0;
            E_SPeriod_flag = 0;
            E_Sti_Count = 0;
            __HAL_TIM_ENABLE(&hTIM_STP_Elec);
          }
        }
        else
        {
          if (!E_SPeriod_flag)
          {
            if (E_Sti_Count < E_Repeat)
            {
              if (!E_Stimulating)
              {
                E_Stimulating = 1;
                if (!elecstimulation(Site_group[Src_Site], Site_group[DstSite], CathodType))
                {
                  E_Sti_Count++;
                  E_SPeriod_flag = 1;
                }
              }
            }
            else
            {
              program_stage = 1;
              ParReceive_enable = 1;
              E_SPeriod_flag = 0;
              E_Sti_Count = 0;
              HAL_TIM_Base_Stop_IT(&hTIM_STP_Elec);
            }
          }
        }
        break;
      case 5:
        if (ParReceive_enable)
        {
          ParReceive_enable = 0;
          while (!getCmd(O_DATARx))
            ;
          while (!getCmd(E_DATARx))
            ;
          parseCmd_O(O_DATARx, O_PTime1_s, O_PTime2_s, O_Cur1_s, O_Cur2_s, Mode_s, O_PulsePeriod_s, O_Repeat_s, StimPeriod_s, O_Delay_s);
          O_PTime1 = atoi(O_PTime1_s) / 250 - 1;
          O_PTime2 = atoi(O_PTime2_s) / 250 - 1;
          O_Cur1 = 0x100 + atoi(O_Cur1_s);
          O_Cur2 = 0x100 + atoi(O_Cur2_s);
          O_PulsePeriod = atoi(O_PulsePeriod_s) / 250 - 1;
          O_Repeat = atoi(O_Repeat_s);
          O_Delay = atoi(O_Delay_s);

          parseCmd_E(E_DATARx, E_PTime1_s, E_PTime2_s, E_Cur1_s, E_Cur2_s, Src_Site_s, CathodType_s, DstSite_s,
                     Mode_s, E_PulsePeriod_s, E_Repeat_s, StimPeriod_s, E_Delay_s);
          E_PTime1 = (uint32_t)atoi(E_PTime1_s) * 4 - 1 - 7;
          E_PTime2 = (uint32_t)atoi(E_PTime2_s) * 4 - 1 - 10;
          E_CurD = (0b11 << 14) + (uint32_t)atoi(E_Cur1_s);        // Negtive High Current
          E_CurC = (0b10 << 14) + (uint32_t)atoi(E_Cur2_s);        // Negtive Low Current
          E_CurB = (0b01 << 14) + 4095 - (uint32_t)atoi(E_Cur1_s); // Positive High Current
          E_CurA = (0b00 << 14) + 4095 - (uint32_t)atoi(E_Cur2_s); // Positive Low Current
          Src_Site = (uint32_t)atoi(Src_Site_s);
          CathodType = (uint32_t)atoi(CathodType_s);
          DstSite = (uint32_t)atoi(DstSite_s);
          Mode = (uint32_t)atoi(Mode_s);
          E_PulsePeriod = (uint32_t)atoi(E_PulsePeriod_s) - 1;
          E_Repeat = (uint32_t)atoi(E_Repeat_s);
          StimPeriod = (uint32_t)atoi(StimPeriod_s) - 1;
          E_Delay = (uint32_t)atoi(E_Delay_s);

          HAL_GPIO_WritePin(EDAC_CLR_GPIO_Port, EDAC_CLR_Pin, GPIO_PIN_SET);
          MySPI_SendByte2AD5726(EVO, E_CurA); // VO+ when switch PG14 to 1
          MySPI_SendByte2AD5726(EVO, E_CurB); // VO+ when switch PG14 to 0 (default)
          MySPI_SendByte2AD5726(EVO, E_CurC); // VO- when switch PG9 to 1
          MySPI_SendByte2AD5726(EVO, E_CurD); // VO- when switch PG9 to 0 (default)

          if (Mode)
          {
            __HAL_TIM_SET_AUTORELOAD(&hTIM_LTP, StimPeriod);
            __HAL_TIM_SET_COUNTER(&hTIM_LTP, 0);
            HAL_TIM_Base_Start_IT(&hTIM_LTP);
          }

          if (O_Delay < E_Delay)
          {
            LED_first_flag = 1;
            O_stimallowed = 1;
            __HAL_TIM_SET_AUTORELOAD(&hTIM_STP_Opto, O_PulsePeriod);
            __HAL_TIM_SET_COUNTER(&hTIM_STP_Opto, 0);
            HAL_TIM_Base_Start_IT(&hTIM_STP_Opto);

            __HAL_TIM_SET_AUTORELOAD(&hTIM_Delay, E_Delay - 1);
            __HAL_TIM_SET_COUNTER(&hTIM_Delay, 0);
            HAL_TIM_Base_Start_IT(&hTIM_Delay);
          }
          else if (O_Delay > E_Delay)
          {
            LED_first_flag = 0;
            E_stimallowed = 1;
            __HAL_TIM_SET_AUTORELOAD(&hTIM_STP_Elec, E_PulsePeriod);
            __HAL_TIM_SET_COUNTER(&hTIM_STP_Elec, 0);
            HAL_TIM_Base_Start_IT(&hTIM_STP_Elec);

            __HAL_TIM_SET_AUTORELOAD(&hTIM_Delay, O_Delay - 1);
            __HAL_TIM_SET_COUNTER(&hTIM_Delay, 0);
            HAL_TIM_Base_Start_IT(&hTIM_Delay);
          }
          else
          {
            O_stimallowed = 1;
            E_stimallowed = 1;
            __HAL_TIM_SET_AUTORELOAD(&hTIM_STP_Opto, O_PulsePeriod);
            __HAL_TIM_SET_COUNTER(&hTIM_STP_Opto, 0);
            HAL_TIM_Base_Start_IT(&hTIM_STP_Opto);

            __HAL_TIM_SET_AUTORELOAD(&hTIM_STP_Elec, E_PulsePeriod);
            __HAL_TIM_SET_COUNTER(&hTIM_STP_Elec, 0);
            HAL_TIM_Base_Start_IT(&hTIM_STP_Elec);
          }
        }
        if (Mode)
        {
          if (!LPeriod_flag)
          {
            if (O_stimallowed)
            {
              if (!O_SPeriod_flag)
              {
                if (O_Sti_Count < O_Repeat)
                {
                  if (!O_Stimulating)
                  {
                    O_Stimulating = 1;
                    if (!optostimulation())
                    {
                      O_Sti_Count++;
                      O_SPeriod_flag = 1;
                    }
                  }
                }
                else
                {
                  O_stimallowed = 0;
                  HAL_TIM_Base_Stop_IT(&hTIM_STP_Opto);
                }
              }
            }

            if (E_stimallowed)
            {
              if (!E_SPeriod_flag)
              {
                if (E_Sti_Count < E_Repeat)
                {
                  if (!E_Stimulating)
                  {
                    E_Stimulating = 1;
                    if (!elecstimulation(Site_group[Src_Site], Site_group[DstSite], CathodType))
                    {
                      E_Sti_Count++;
                      E_SPeriod_flag = 1;
                    }
                  }
                }
                else
                {
                  E_stimallowed = 0;
                  HAL_TIM_Base_Stop_IT(&hTIM_STP_Elec);
                }
              }
            }
          }
          else
          {
            E_Sti_Count = 0;
            O_Sti_Count = 0;
            LPeriod_flag = 0;

            if (O_Delay < E_Delay)
            {
              LED_first_flag = 1;
              O_stimallowed = 1;
              __HAL_TIM_SET_COUNTER(&hTIM_STP_Opto, 0);
              HAL_TIM_Base_Start_IT(&hTIM_STP_Opto);

              __HAL_TIM_SET_COUNTER(&hTIM_Delay, 0);
              HAL_TIM_Base_Start_IT(&hTIM_Delay);
            }
            else if (O_Delay > E_Delay)
            {
              LED_first_flag = 0;
              E_stimallowed = 1;
              __HAL_TIM_SET_COUNTER(&hTIM_STP_Elec, 0);
              HAL_TIM_Base_Start_IT(&hTIM_STP_Elec);

              __HAL_TIM_SET_COUNTER(&hTIM_Delay, 0);
              HAL_TIM_Base_Start_IT(&hTIM_Delay);
            }
            else
            {
              O_stimallowed = 1;
              E_stimallowed = 1;
              __HAL_TIM_SET_COUNTER(&hTIM_STP_Opto, 0);
              HAL_TIM_Base_Start_IT(&hTIM_STP_Opto);

              __HAL_TIM_SET_COUNTER(&hTIM_STP_Elec, 0);
              HAL_TIM_Base_Start_IT(&hTIM_STP_Elec);
            }
          }
        }
        else
        {
          if (O_stimallowed)
          {
            if (!O_SPeriod_flag)
            {
              if (O_Sti_Count < O_Repeat)
              {
                if (!O_Stimulating)
                {
                  O_Stimulating = 1;
                  if (!optostimulation())
                  {
                    O_Sti_Count++;
                    O_SPeriod_flag = 1;
                  }
                }
              }
              else
              {
                Ostimcompleted = 1;
                O_stimallowed = 0;
                HAL_TIM_Base_Stop_IT(&hTIM_STP_Opto);
              }
            }
          }

          if (E_stimallowed)
          {
            if (!E_SPeriod_flag)
            {
              if (E_Sti_Count < E_Repeat)
              {
                if (!E_Stimulating)
                {
                  E_Stimulating = 1;
                  if (!elecstimulation(Site_group[Src_Site], Site_group[DstSite], CathodType))
                  {
                    E_Sti_Count++;
                    E_SPeriod_flag = 1;
                  }
                }
              }
              else
              {
                Esticompleted = 1;
                E_stimallowed = 0;
                HAL_TIM_Base_Stop_IT(&hTIM_STP_Elec);
              }
            }
          }

          if (Esticompleted && Ostimcompleted)
          {
            program_stage = 1;
            ParReceive_enable = 1;
            Esticompleted = 0;
            Ostimcompleted = 0;

            O_SPeriod_flag = 0;
            O_Sti_Count = 0;
            E_SPeriod_flag = 0;
            E_Sti_Count = 0;
            HAL_TIM_Base_Stop_IT(&hTIM_Delay);
          }
        }
      }
      if (program_stage != 2)
      {
        if (ADC_sample_complete)
        {
          ADC_sample_complete = 0;
          //          sendValueThroughUART(g_ADC_TransferArray, ADC_TransferLength);
        }
      }
    }
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
   */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
   */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
