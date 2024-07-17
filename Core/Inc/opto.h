/*
 * opto.h
 *
 *  Created on: Jul 6, 2024
 *      Author: PMY
 */

#ifndef INC_OPTO_H_
#define INC_OPTO_H_
#include "gpio.h"

void FixState();
void ForwardState();
void ReverseState();

uint8_t optostimulation();

#endif /* INC_OPTO_H_ */
