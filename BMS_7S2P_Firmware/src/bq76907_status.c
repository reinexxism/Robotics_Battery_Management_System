/*
 * bq76907_status.c
 *
 *  Created on: 2026. 11. 2.
 *      Author: nosta
 */

#include "bq76907_status.h"
#include "bq76907_i2c.h"

#define BQ76907_BATTERY_STATUS_REG 0x12U

Lpi2c_Ip_StatusType BmicStatus_Read(uint16 *batteryStatus)
{
    uint8 data[2];
    Lpi2c_Ip_StatusType result;

    if (batteryStatus == NULL_PTR)
    {
        return LPI2C_IP_ERROR_STATUS;
    }

    result = Bq76907_I2cRead(BQ76907_BATTERY_STATUS_REG, data, 2U);

    if (result == LPI2C_IP_SUCCESS_STATUS)
    {
        *batteryStatus = (uint16)(data[0] | ((uint16)data[1] << 8U));
    }

    return result;
}
