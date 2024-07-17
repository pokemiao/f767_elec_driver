/*
 * opto.c
 *
 *  Created on: Jul 6, 2024
 *      Author: PMY
 */
#include "main.h"
#include "opto.h"

volatile uint8_t Opto_PhaseFlag = 1;

extern uint8_t O_Stimulating, O_SPeriod_flag, LPeriod_flag;
extern uint32_t O_PTime1, O_PTime2;

void FixState()
{
    O_Fix_GPIO_Port->BSRR = O_Fix_Pin << 16;
    O_For_GPIO_Port->BSRR = O_For_Pin;
    O_Rev_GPIO_Port->BSRR = O_Rev_Pin;
}

void ForwardState()
{
    O_Fix_GPIO_Port->BSRR = O_Fix_Pin;
    O_For_GPIO_Port->BSRR = O_For_Pin << 16;
    O_Rev_GPIO_Port->BSRR = O_Rev_Pin;
}

void ReverseState()
{
    O_Fix_GPIO_Port->BSRR = O_Fix_Pin;
    O_For_GPIO_Port->BSRR = O_For_Pin;
    O_Rev_GPIO_Port->BSRR = O_Rev_Pin << 16;
}

/**
 * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
 */
void TIM_STP_Opto_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&hTIM_STP_Opto, TIM_FLAG_UPDATE) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(&hTIM_STP_Opto, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_SET_COUNTER(&hTIM_STP_Opto, 0);
            __HAL_TIM_CLEAR_FLAG(&hTIM_STP_Opto, TIM_IT_UPDATE);
            O_SPeriod_flag = 0;
        }
    }
}

/**
 * @brief This function handles TIM8 break interrupt and TIM12 global interrupt.
 */
void TIM_Pulse_Opto_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&hTIM_Pulse_Opto, TIM_FLAG_UPDATE) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(&hTIM_Pulse_Opto, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_DISABLE(&hTIM_Pulse_Opto);
            __HAL_TIM_SET_COUNTER(&hTIM_Pulse_Opto, 0);
            __HAL_TIM_CLEAR_FLAG(&hTIM_Pulse_Opto, TIM_IT_UPDATE);

            switch (Opto_PhaseFlag)
            {
            case 0:
                Opto_PhaseFlag++;
                break;
            case 1:
                Opto_PhaseFlag++;
                break;
            case 2:
                Opto_PhaseFlag++;
                break;
            case 3:
                Opto_PhaseFlag = 0;
                break;
            }

            O_Stimulating = 0;
        }
    }
}

uint8_t optostimulation()
{
    switch (Opto_PhaseFlag)
    {
    case 0:
        FixState();
        Opto_PhaseFlag = 1;
        O_Stimulating = 0;
        HAL_TIM_Base_Stop_IT(&hTIM_Pulse_Opto);
        return 0;
    case 1:
        __HAL_TIM_SET_AUTORELOAD(&hTIM_Pulse_Opto, O_PTime1);
        __HAL_TIM_SET_COUNTER(&hTIM_Pulse_Opto, 0);
        HAL_TIM_Base_Start_IT(&hTIM_Pulse_Opto);
        ForwardState();
        break;
    case 2:
        FixState();
        __HAL_TIM_SET_AUTORELOAD(&hTIM_Pulse_Opto, 1);
        __HAL_TIM_ENABLE(&hTIM_Pulse_Opto);
        break;
    case 3:
        __HAL_TIM_SET_AUTORELOAD(&hTIM_Pulse_Opto, O_PTime2);
        __HAL_TIM_ENABLE(&hTIM_Pulse_Opto);
        ReverseState();
        break;
    default:
        break;
    }

    return 1;
}
