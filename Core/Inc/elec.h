/*
 * elec.h
 *
 *  Created on: Jul 1, 2024
 *      Author: PMY
 */

#ifndef INC_ELEC_H_
#define INC_ELEC_H_

#include "gpio.h"

typedef struct Site
{
    Site_io_t Anode;
    Site_io_t Cathode;
    Site_io_t GND;
} Site_t;

void FloatState(Site_io_t Src_site, Site_io_t Dst_site);
void Current_steering(Site_io_t Src_site, Site_io_t Dst_site);
uint8_t elecstimulation(Site_t Src_site, Site_t Dst_site, uint8_t CathodType);
#endif /* INC_ELEC_H_ */
