/*
 * myspi.h
 *
 *  Created on: Jul 6, 2024
 *      Author: PMY
 */

#ifndef INC_MYSPI_H_
#define INC_MYSPI_H_

#include "gpio.h"

typedef struct MYSPI_IO
{
    Site_io_t CS;
    Site_io_t DOUT;
    Site_io_t SCLK;
    Site_io_t LDAC;
    Site_io_t CLR;
    Site_io_t CLRSEL;
} MYSPI_IO_t;

void MySPI_SendByte2AD5726(MYSPI_IO_t VO, unsigned short dt);
void MySPI_SendByte2max5388(MYSPI_IO_t VO, unsigned short dt);

#endif /* INC_MYSPI_H_ */
