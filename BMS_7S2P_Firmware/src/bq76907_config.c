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
    { .name = "Vcell Mode", .address = 0x901BU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Power Config", .address = 0x9014U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "I2C Address", .address = 0x9016U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell 1 Gain", .address = 0x9000U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Stack Gain", .address = 0x9002U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell 2 Gain Delta", .address = 0x9004U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell 3 Gain Delta", .address = 0x9005U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell 4 and 5 Gain Delta", .address = 0x9071U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell 6 and 7 Gain Delta", .address = 0x9072U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Curr Gain", .address = 0x9006U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Curr Offset", .address = 0x9008U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "CC1 Gain", .address = 0x900AU, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "CC1 Offset", .address = 0x900CU, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "TS Offset", .address = 0x900EU, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Int Temp Gain", .address = 0x9010U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Int Temp Offset", .address = 0x9012U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "REGOUT Config", .address = 0x9015U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "I2C Config", .address = 0x9017U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "DA Config", .address = 0x9019U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Default Alarm Mask", .address = 0x901CU, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "FET Options", .address = 0x901EU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Charge Detector Time", .address = 0x901FU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Balancing Configuration", .address = 0x9020U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Min Temp Threshold", .address = 0x9021U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Max Temp Threshold", .address = 0x9022U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Max Internal Temp", .address = 0x9023U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Enabled Protections A", .address = 0x9024U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Enabled Protections B", .address = 0x9025U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "DSG FET Protections A", .address = 0x9026U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "CHG FET Protections A", .address = 0x9027U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Both FET Protections B", .address = 0x9028U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Body Diode Threshold", .address = 0x9029U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell Open Wire NORMAL Check Time", .address = 0x902BU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell Open Wire SLEEP Check Time", .address = 0x902CU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Host Watchdog Timeout", .address = 0x902DU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell Undervoltage Protection Threshold", .address = 0x902EU, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell Undervoltage Protection Delay", .address = 0x9030U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell Undervoltage Protection Recovery Hysteresis", .address = 0x9031U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell Overvoltage Protection Threshold", .address = 0x9032U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell Overvoltage Protection Delay", .address = 0x9034U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Cell Overvoltage Protection Recovery Hysteresis", .address = 0x9035U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overcurrent in Charge Protection Threshold", .address = 0x9036U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overcurrent in Charge Protection Delay", .address = 0x9037U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overcurrent in Discharge 1 Protection Threshold", .address = 0x9038U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overcurrent in Discharge 1 Protection Delay", .address = 0x9039U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overcurrent in Discharge 2 Protection Threshold", .address = 0x903AU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overcurrent in Discharge 2 Protection Delay", .address = 0x903BU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Short Circuit in Discharge Protection Threshold", .address = 0x903CU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Short Circuit in Discharge Protection Delay", .address = 0x903DU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Latch Limit", .address = 0x903EU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Recovery Time", .address = 0x903FU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overtemperature in Charge Protection Threshold", .address = 0x9040U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overtemperature in Charge Protection Delay", .address = 0x9041U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overtemperature in Charge Protection Recovery", .address = 0x9042U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Undertemperature in Charge Protection Threshold", .address = 0x9043U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Undertemperature in Charge Protection Delay", .address = 0x9044U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Undertemperature in Charge Protection Recovery", .address = 0x9045U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overtemperature in Discharge Protection Threshold", .address = 0x9046U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overtemperature in Discharge Protection Delay", .address = 0x9047U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Overtemperature in Discharge Protection Recovery", .address = 0x9048U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Undertemperature in Discharge Protection Threshold", .address = 0x9049U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Undertemperature in Discharge Protection Delay", .address = 0x904AU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Undertemperature in Discharge Protection Recovery", .address = 0x904BU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Internal Overtemperature Protection Threshold", .address = 0x904CU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Internal Overtemperature Protection Delay", .address = 0x904DU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Internal Overtemperature Protection Recovery", .address = 0x904EU, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Sleep Current", .address = 0x904FU, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Voltage Time", .address = 0x9051U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Wake Comparator Current", .address = 0x9052U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Shutdown Cell Voltage", .address = 0x9053U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Shutdown Stack Voltage", .address = 0x9055U, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Shutdown Temperature", .address = 0x9057U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Auto Shutdown Time", .address = 0x9058U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Security Settings", .address = 0x9059U, .width = 1U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Full Access Key Step 1", .address = 0x905AU, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
    { .name = "Full Access Key Step 2", .address = 0x905CU, .width = 2U, .applyMask = 0U, .result = BQ_CONFIG_NOT_READ },
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

Bq76907_7sCheck Bq76907_ConfigCheck7s(void)
{
    const size_t count = sizeof(bq76907_config_table)
                       / sizeof(bq76907_config_table[0]);

    for (size_t i = 0U; i < count; i++)
    {
        const Bq76907_ConfigEntry *entry = &bq76907_config_table[i];

        if (entry->address == 0x901BU)
        {
            if (entry->result != BQ_CONFIG_READ_OK)
            {
                return BQ_7S_UNKNOWN;
            }

            const uint32_t mode = entry->before & 0x07U;

            if ((mode == 0U) || (mode == 1U) || (mode == 7U))
            {
                return BQ_7S_MATCH;
            }

            return BQ_7S_MISMATCH;
        }
    }

    return BQ_7S_UNKNOWN;
}
