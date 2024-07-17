/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define O_For_Pin GPIO_PIN_3
#define O_For_GPIO_Port GPIOF
#define O_Rev_Pin GPIO_PIN_5
#define O_Rev_GPIO_Port GPIOF
#define O_Trigger_Pin GPIO_PIN_10
#define O_Trigger_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define ODAC_CS_Pin GPIO_PIN_0
#define ODAC_CS_GPIO_Port GPIOC
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define O_Fix_Pin GPIO_PIN_3
#define O_Fix_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define ODAC_SDIN_Pin GPIO_PIN_3
#define ODAC_SDIN_GPIO_Port GPIOA
#define Site2_Cathode_Pin GPIO_PIN_6
#define Site2_Cathode_GPIO_Port GPIOA
#define Site2_Anode_Pin GPIO_PIN_7
#define Site2_Anode_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define Site1_Anode_Pin GPIO_PIN_12
#define Site1_Anode_GPIO_Port GPIOF
#define EDAC_SDIN_Pin GPIO_PIN_13
#define EDAC_SDIN_GPIO_Port GPIOF
#define EDAC_LDAC_Pin GPIO_PIN_14
#define EDAC_LDAC_GPIO_Port GPIOF
#define EDAC_CLRSEL_Pin GPIO_PIN_15
#define EDAC_CLRSEL_GPIO_Port GPIOF
#define EDAC_SCLK_Pin GPIO_PIN_9
#define EDAC_SCLK_GPIO_Port GPIOE
#define EDAC_CS_Pin GPIO_PIN_11
#define EDAC_CS_GPIO_Port GPIOE
#define EDAC_CLR_Pin GPIO_PIN_13
#define EDAC_CLR_GPIO_Port GPIOE
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define Site3_GND_Pin GPIO_PIN_15
#define Site3_GND_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define Site1_GND_Pin GPIO_PIN_14
#define Site1_GND_GPIO_Port GPIOD
#define Site1_Cathode_Pin GPIO_PIN_15
#define Site1_Cathode_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define Site3_Cathode_Pin GPIO_PIN_6
#define Site3_Cathode_GPIO_Port GPIOC
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define ODAC_SCLK_Pin GPIO_PIN_3
#define ODAC_SCLK_GPIO_Port GPIOD
#define EDAC_NSW_Pin GPIO_PIN_9
#define EDAC_NSW_GPIO_Port GPIOG
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define EDAC_PSW_Pin GPIO_PIN_14
#define EDAC_PSW_GPIO_Port GPIOG
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#define Site3_Anode_Pin GPIO_PIN_8
#define Site3_Anode_GPIO_Port GPIOB
#define Site2_GND_Pin GPIO_PIN_9
#define Site2_GND_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define hADC_Trigger htim7
#define ADC_Trigger_IRQn TIM7_IRQn
#define ADC_Trigger_IRQHandler TIM7_IRQHandler

#define hTIM_Pulse_Elec htim9
#define TIM_Pulse_Elec_IRQn TIM1_BRK_TIM9_IRQn
#define TIM_Pulse_Elec_IRQHandler TIM1_BRK_TIM9_IRQHandler

#define hTIM_STP_Elec htim10
#define TIM_STP_Elec_IRQn TIM1_UP_TIM10_IRQn
#define TIM_STP_Elec_IRQHandler TIM1_UP_TIM10_IRQHandler

#define hTIM_LTP htim11
#define TIM_LTP_IRQn TIM1_TRG_COM_TIM11_IRQn
#define TIM_LTP_IRQHandler TIM1_TRG_COM_TIM11_IRQHandler

#define hTIM_Pulse_Opto htim12
#define TIM_Pulse_Opto_IRQn TIM8_BRK_TIM12_IRQn
#define TIM_Pulse_Opto_IRQHandler TIM8_BRK_TIM12_IRQHandler

#define hTIM_STP_Opto htim13
#define TIM_STP_Opto_IRQn TIM8_UP_TIM13_IRQn
#define TIM_STP_Opto_IRQHandler TIM8_UP_TIM13_IRQHandler

#define hTIM_Delay htim2
#define TIM_Delay_IRQn TIM2_IRQn
#define TIM_Delay_IRQHandler TIM2_IRQHandler

extern TIM_HandleTypeDef htim7, htim9, htim10, htim11, htim12, htim13, htim14;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
