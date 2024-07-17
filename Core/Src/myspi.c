/*
 * myspi.c
 *
 *  Created on: Jul 6, 2024
 *      Author: PMY
 */
#include "main.h"
#include "myspi.h"

const Site_io_t EVO_CS = {EDAC_CS_GPIO_Port, EDAC_CS_Pin};
const Site_io_t EVO_DA = {EDAC_SDIN_GPIO_Port, EDAC_SDIN_Pin};
const Site_io_t EVO_SCLK = {EDAC_SCLK_GPIO_Port, EDAC_SCLK_Pin};
const Site_io_t EVO_LDAC = {EDAC_LDAC_GPIO_Port, EDAC_LDAC_Pin};
const Site_io_t EVO_CLR = {EDAC_CLR_GPIO_Port, EDAC_CLR_Pin};
const Site_io_t EVO_CLRSEL = {EDAC_CLRSEL_GPIO_Port, EDAC_CLRSEL_Pin};

const MYSPI_IO_t EVO = {EVO_CS, EVO_DA, EVO_SCLK, EVO_LDAC, EVO_CLR, EVO_CLRSEL};

void MySPI_SendByte2AD5726(MYSPI_IO_t VO, unsigned short dt)
{
    uint8_t i, j;
    VO.CS.io_port->BSRR = VO.CS.pin << 16;     // CS RESET
    VO.SCLK.io_port->BSRR = VO.SCLK.pin << 16; // SCLK RESET

    for (i = 16; i > 0; i--)
    {
        if (dt & 0x8000)
            VO.DOUT.io_port->BSRR = VO.DOUT.pin; // DOUT SET
        else
            VO.DOUT.io_port->BSRR = VO.DOUT.pin << 16; // DOUT RESET

        for (j = 0; j < 5; j++)
        {
            __asm("NOP");
        }

        VO.SCLK.io_port->BSRR = VO.SCLK.pin; // SCLK SET
        dt <<= 1;

        for (j = 0; j < 5; j++)
        {
            __asm("NOP");
        }

        VO.SCLK.io_port->BSRR = VO.SCLK.pin << 16; // SCLK RESET
    }
    VO.CS.io_port->BSRR = VO.CS.pin; // CS SET

    VO.LDAC.io_port->BSRR = VO.LDAC.pin << 16; // LDAC RESET
    for (j = 0; j < 5; j++)
    {
        __asm("NOP");
    }
    VO.LDAC.io_port->BSRR = VO.LDAC.pin; // LDAC SET
}

void MySPI_SendByte2max5388(MYSPI_IO_t VO, unsigned short dt)
{
    uint8_t i, j;
    VO.CS.io_port->BSRR = VO.CS.pin << 16;     // CS RESET
    VO.SCLK.io_port->BSRR = VO.SCLK.pin << 16; // SCLK RESET

    for (i = 9; i > 0; i--)
    {
        if (dt & 0x100)
            VO.DOUT.io_port->BSRR = VO.DOUT.pin; // DOUT SET
        else
            VO.DOUT.io_port->BSRR = VO.DOUT.pin << 16; // DOUT RESET

        for (j = 0; j < 5; j++)
        {
            __asm("NOP");
        }

        VO.SCLK.io_port->BSRR = VO.SCLK.pin; // SCLK SET
        dt <<= 1;

        for (j = 0; j < 5; j++)
        {
            __asm("NOP");
        }

        VO.SCLK.io_port->BSRR = VO.SCLK.pin << 16; // SCLK RESET
    }
    VO.CS.io_port->BSRR = VO.CS.pin; // CS SET
}
