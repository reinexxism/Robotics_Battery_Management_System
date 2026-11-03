/*
 * bq76907_i2c.h
 *
 *  Created on: 2026. 11. 2.
 *      Author: nosta
 */

#ifndef BQ76907_I2C_H_
#define BQ76907_I2C_H_

#include "Lpi2c_Ip_Types.h"

void Bq76907_I2cInit(void);
Lpi2c_Ip_StatusType Bq76907_I2cRead(uint8 reg, uint8 *data, uint32 length);

Lpi2c_Ip_StatusType Bq76907_SelectDataMemory(uint16 address);

#endif /* BQ76907_I2C_H_ */
