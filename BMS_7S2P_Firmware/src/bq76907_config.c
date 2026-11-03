/*
 * bq76907_config.c
 *
 *  Created on: 2026. 11. 3.
 *      Author: nosta
 */

#include "bq76907_config.h"
#include "bq76907_i2c.h"

static Bq76907_ConfigEntry bq76907_config_table[] =
{
    {
        .name      = "Vcell Mode",
        .address   = 0x901BU,
        .width     = 1U,
        .applyMask = 0U,
        .result    = BQ_CONFIG_NOT_READ
    },
    {
        .name      = "Power Config",
        .address   = 0x9014U,
        .width     = 1U,
        .applyMask = 0U,
        .result    = BQ_CONFIG_NOT_READ
    }
};

static Lpi2c_Ip_StatusType Bq76907_ConfigSelectAndCheck(uint16 address)
{
    uint8 echo[2];
    uint16 echoedAddress;
    Lpi2c_Ip_StatusType status;

    status = Bq76907_SelectDataMemory(address);
    if (status != LPI2C_IP_SUCCESS_STATUS)
    {
        return status;
    }

    status = Bq76907_I2cRead(0x3EU, echo, 2U);
    if (status != LPI2C_IP_SUCCESS_STATUS)
    {
        return status;
    }

    echoedAddress = (uint16)(echo[0] | ((uint16)echo[1] << 8U));
    if (echoedAddress != address)
    {
        return LPI2C_IP_ERROR_STATUS;
    }

    return LPI2C_IP_SUCCESS_STATUS;
}

static boolean Bq76907_ConfigResponseValid(uint16 address, const uint8 *data,
                                          uint8 width, uint8 checksum, uint8 length)
{
    uint8 sum;
    uint8 i;

    if ((length > 36U) || (length < (4U + width)))
    {
        return FALSE;
    }

    sum = (uint8)((address & 0xFFU) + (address >> 8U));
    for (i = 0U; i < (length - 4U); i++)
    {
        sum = (uint8)(sum + data[i]);
    }

    const uint8 expectedChecksum = (uint8)(~sum);
    return expectedChecksum == checksum;
}

static void Bq76907_ConfigReadEntry(Bq76907_ConfigEntry *entry)
{
    uint8 data[32];
    uint8 trailer[2];
    uint32 value = 0U;
    uint8 i;

    entry->result = BQ_CONFIG_READ_FAILED;

    if ((entry->width == 0U) || (entry->width > sizeof(value)))
    {
        return;
    }

    if (Bq76907_ConfigSelectAndCheck(entry->address) != LPI2C_IP_SUCCESS_STATUS)
    {
        return;
    }

    if (Bq76907_I2cRead(0x40U, data, sizeof(data)) != LPI2C_IP_SUCCESS_STATUS)
    {
        return;
    }

    /* 0x60: checksum, 0x61: length */
    if (Bq76907_I2cRead(0x60U, trailer, sizeof(trailer)) != LPI2C_IP_SUCCESS_STATUS)
    {
        return;
    }

    if (!Bq76907_ConfigResponseValid(entry->address, data, entry->width,
                                     trailer[0], trailer[1]))
    {
        return;
    }

    for (i = 0U; i < entry->width; i++)
    {
        value |= (uint32)data[i] << (8U * i);
    }

    entry->before = value;
    entry->result = BQ_CONFIG_READ_OK;
}

void Bq76907_ConfigReadPending(void)
{
    const size_t count = sizeof(bq76907_config_table)
                       / sizeof(bq76907_config_table[0]);

    for (size_t i = 0U; i < count; i++)
    {
        Bq76907_ConfigEntry *entry = &bq76907_config_table[i];

        if ((entry->result == BQ_CONFIG_NOT_READ) ||
            (entry->result == BQ_CONFIG_READ_FAILED))
        {
            Bq76907_ConfigReadEntry(entry);
        }
    }
}
