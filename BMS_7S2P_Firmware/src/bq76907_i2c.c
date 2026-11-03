/*
 * bq76907_i2c.c
 *
 *  Created on: 2026. 11. 2.
 *      Author: nosta
 */

#include "bq76907_i2c.h"
#include "Lpi2c_Ip.h"
#include "Lpi2c_Ip_PBcfg.h"

#define BQ76907_I2C_WAIT_BUDGET 100000U

volatile Lpi2c_Ip_StatusType bmic_tx_result;

void Bq76907_I2cInit(void)
{
    Lpi2c_Ip_MasterInit(0U, &I2c_Lpi2cMasterChannel0);
}

Lpi2c_Ip_StatusType Bq76907_I2cRead(uint8 reg, uint8 *data, uint32 length)
{
    Lpi2c_Ip_StatusType status;

    if ((data == NULL_PTR) || (length == 0U))
    {
        return LPI2C_IP_ERROR_STATUS;
    }

    status = Lpi2c_Ip_MasterSendDataBlocking(
        0U, &reg, 1U, FALSE, BQ76907_I2C_WAIT_BUDGET);

    bmic_tx_result = status;

    if (status != LPI2C_IP_SUCCESS_STATUS)
    {
        return status;
    }

    return Lpi2c_Ip_MasterReceiveDataBlocking(
        0U, data, length, TRUE, BQ76907_I2C_WAIT_BUDGET);
}

