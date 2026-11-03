/*
 * bq76907_status.h
 *
 *  Created on: 2026. 11. 2.
 *      Author: nosta
 */

#ifndef BQ76907_STATUS_H_
#define BQ76907_STATUS_H_

#include "Lpi2c_Ip_Types.h"

Lpi2c_Ip_StatusType BmicStatus_Read(uint16 *batteryStatus);


#endif /* BQ76907_STATUS_H_ */
