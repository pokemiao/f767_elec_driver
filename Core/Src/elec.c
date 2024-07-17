/*
 * elec.c
 *
 *  Created on: Jul 1, 2024
 *      Author: PMY
 */
#include "main.h"
#include "elec.h"
#include "gpio.h"

const Site_io_t Site1_Anode = {Site1_Anode_GPIO_Port, Site1_Anode_Pin};
const Site_io_t Site1_Cathode = {Site1_Cathode_GPIO_Port, Site1_Cathode_Pin};
const Site_io_t Site1_GND = {Site1_GND_GPIO_Port, Site1_GND_Pin};
const Site_io_t Site2_Anode = {Site2_Anode_GPIO_Port, Site2_Anode_Pin};
const Site_io_t Site2_Cathode = {Site2_Cathode_GPIO_Port, Site2_Cathode_Pin};
const Site_io_t Site2_GND = {Site2_GND_GPIO_Port, Site2_GND_Pin};
const Site_io_t Site3_Anode = {Site3_Anode_GPIO_Port, Site3_Anode_Pin};
const Site_io_t Site3_Cathode = {Site3_Cathode_GPIO_Port, Site3_Cathode_Pin};
const Site_io_t Site3_GND = {Site3_GND_GPIO_Port, Site3_GND_Pin};

const Site_t Site1 = {Site1_Anode, Site1_Cathode, Site1_GND};
const Site_t Site2 = {Site2_Anode, Site2_Cathode, Site2_GND};
const Site_t Site3 = {Site3_Anode, Site3_Cathode, Site3_GND};

extern uint32_t E_PTime1, E_PTime2;
extern uint8_t E_Stimulating, E_SPeriod_flag, LPeriod_flag;

// extern MYSPI_IO_t VO_Group[2];
Site_t Site_group[3] = {Site1, Site2, Site3};

/**
 * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
 */
void TIM_STP_Elec_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&hTIM_STP_Elec, TIM_FLAG_UPDATE) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(&hTIM_STP_Elec, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_SET_COUNTER(&hTIM_STP_Elec, 0);
            __HAL_TIM_CLEAR_FLAG(&hTIM_STP_Elec, TIM_IT_UPDATE);
            E_SPeriod_flag = 0;
        }
    }
}

/**
 * @brief This function handles TIM1 break interrupt and TIM9 global interrupt.
 */
void TIM_Pulse_Elec_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&hTIM_Pulse_Elec, TIM_FLAG_UPDATE) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(&hTIM_Pulse_Elec, TIM_IT_UPDATE) != RESET)
        {
//            __HAL_TIM_DISABLE(&hTIM_Pulse_Elec);
            __HAL_TIM_SET_COUNTER(&hTIM_Pulse_Elec, 0);
            __HAL_TIM_CLEAR_FLAG(&hTIM_Pulse_Elec, TIM_IT_UPDATE);
            E_Stimulating = 0;
        }
    }
}

void FloatState(Site_io_t Src_site, Site_io_t Dst_site)
{
    Src_site.io_port->BSRR = Src_site.pin << 16;
    Dst_site.io_port->BSRR = Dst_site.pin << 16;
}

void Current_steering(Site_io_t Src_site, Site_io_t Dst_site)
{
    Src_site.io_port->BSRR = Src_site.pin;
    Dst_site.io_port->BSRR = Dst_site.pin;
}

uint8_t elecstimulation(Site_t Src_site, Site_t Dst_site, uint8_t CathodType)
{
    HAL_NVIC_DisableIRQ(ADC_Trigger_IRQn);
    
    __HAL_TIM_SET_AUTORELOAD(&hTIM_Pulse_Elec, E_PTime1);
    __HAL_TIM_SET_COUNTER(&hTIM_Pulse_Elec, 0);
    HAL_TIM_Base_Start_IT(&hTIM_Pulse_Elec);

    if (CathodType)
        Current_steering(Src_site.Cathode, Dst_site.GND);
    else
        Current_steering(Src_site.Cathode, Dst_site.Anode);
    while (E_Stimulating)
        ;

    if (CathodType)
        FloatState(Src_site.Cathode, Dst_site.GND);
    else
        FloatState(Src_site.Cathode, Dst_site.Anode);

    EDAC_NSW_GPIO_Port->BSRR = EDAC_NSW_Pin; // DACC
    EDAC_PSW_GPIO_Port->BSRR = EDAC_PSW_Pin; // DACA
    //    EStimulating = 1;
    //    __HAL_TIM_SET_AUTORELOAD(&hTIM_Pulse_Elec, 1);
    //    __HAL_TIM_ENABLE(&hTIM_Pulse_Elec);
    //    //    HAL_TIM_Base_Start_IT(&hTIM_Pulse_Elec);
    //    while (EStimulating)
    //        ;

    E_Stimulating = 1;
    __HAL_TIM_SET_AUTORELOAD(&hTIM_Pulse_Elec, E_PTime2);
    __HAL_TIM_ENABLE(&hTIM_Pulse_Elec);

    if (CathodType)
        Current_steering(Dst_site.Cathode, Src_site.GND);
    else
        Current_steering(Dst_site.Cathode, Src_site.Anode);
    while (E_Stimulating)
        ;

    if (CathodType)
        FloatState(Dst_site.Cathode, Src_site.GND);
    else
        FloatState(Dst_site.Cathode, Src_site.Anode);

    HAL_TIM_Base_Stop_IT(&hTIM_Pulse_Elec);

    EDAC_NSW_GPIO_Port->BSRR = EDAC_NSW_Pin << 16; // DACD
    EDAC_PSW_GPIO_Port->BSRR = EDAC_PSW_Pin << 16; // DACB

    HAL_NVIC_EnableIRQ(ADC_Trigger_IRQn);
    return 0;

    // switch (Elec_PhaseFlag)
    // {
    // case 0:
    //     if (CathodType)
    //         FloatState(Dst_site.Cathode, Src_site.GND);
    //     else
    //         FloatState(Dst_site.Cathode, Src_site.Anode);
    //     Elec_PhaseFlag = 1;
    //     E_Stimulating = 0;
    //     HAL_TIM_Base_Stop_IT(&hTIM_Pulse_Elec);
    //     return 0;
    // case 1:
    //     __HAL_TIM_SET_AUTORELOAD(&hTIM_Pulse_Elec, E_PTime1);
    //     __HAL_TIM_SET_COUNTER(&hTIM_Pulse_Elec, 0);
    //     HAL_TIM_Base_Start_IT(&hTIM_Pulse_Elec);
    //     if (CathodType)
    //         Current_steering(Src_site.Cathode, Dst_site.GND);
    //     else
    //         Current_steering(Src_site.Cathode, Dst_site.Anode);
    //     break;
    // case 2:
    //     if (CathodType)
    //         FloatState(Src_site.Cathode, Dst_site.GND);
    //     else
    //         FloatState(Src_site.Cathode, Dst_site.Anode);
    //     Elec_PhaseFlag = 3;
    //     E_Stimulating = 0;
    //     break;
    // case 3:
    //     __HAL_TIM_SET_AUTORELOAD(&hTIM_Pulse_Elec, E_PTime2);
    //     // __HAL_TIM_ENABLE(&hTIM_Pulse_Elec);
    //     if (CathodType)
    //         Current_steering(Dst_site.Cathode, Src_site.GND);
    //     else
    //         Current_steering(Dst_site.Cathode, Src_site.Anode);
    //     break;
    // default:
    //     break;
    // }

    // return 1;
}
