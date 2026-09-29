#include "Clock_Ip.h"
#include "Port_Ci_Port_Ip.h"
#include "Port_Ci_Port_Ip_Cfg.h"
#include "Lpi2c_Ip.h"
#include "Lpi2c_Ip_PBcfg.h"
#include "OsIf.h"

/* One-shot Battery Status read: default 7-bit address 0x08, CRC disabled.
 * Register selection is written; no configuration/FET/OTP data is written.
 * OSIF_COUNTER_DUMMY is configured: budget is NOT an accurate time in ms.
 */
#define BQ_I2C_INSTANCE  0U
#define BQ_WAIT_BUDGET   100000U

/* stage: 0=start, 1=clock ready, 2=pins ready, 3=I2C ready,
 *        4=select register, 5=receive, 6=success, 7=TX failed, 8=RX failed.
 * Status 0xFF means not attempted. Decode flags only if read_ok == 1.
 */
volatile uint8 bmic_stage = 0U;
volatile uint8 bmic_done = 0U;
volatile uint8 bmic_read_ok = 0U;
volatile uint32 bmic_tx_status = 0xFFU;
volatile uint32 bmic_rx_status = 0xFFU;
volatile uint8 bmic_rx_low = 0U;
volatile uint8 bmic_rx_high = 0U;
volatile uint16 bmic_battery_status = 0U;
volatile uint8 bmic_sleep = 0U;
volatile uint8 bmic_deepsleep = 0U;
volatile uint8 bmic_cfgupdate = 0U;

static void Bmic_ReadStatusOnce(void)
{
    uint8 reg = 0x12U;
    uint8 rx[2] = {0U, 0U};
    Lpi2c_Ip_StatusType result;

    bmic_stage = 4U;
    /* No STOP: receive starts with a repeated START. */
    result = Lpi2c_Ip_MasterSendDataBlocking(
        BQ_I2C_INSTANCE, &reg, 1U, FALSE, BQ_WAIT_BUDGET);
    bmic_tx_status = (uint32)result;
    if (result != LPI2C_IP_SUCCESS_STATUS)
    {
        bmic_stage = 7U;
        return;
    }

    bmic_stage = 5U;
    result = Lpi2c_Ip_MasterReceiveDataBlocking(
        BQ_I2C_INSTANCE, rx, 2U, TRUE, BQ_WAIT_BUDGET);
    bmic_rx_status = (uint32)result;
    if (result != LPI2C_IP_SUCCESS_STATUS)
    {
        bmic_stage = 8U;
        return;
    }

    bmic_rx_low = rx[0];
    bmic_rx_high = rx[1];
    bmic_battery_status = (uint16)((uint16)rx[0] | ((uint16)rx[1] << 8U));
    bmic_sleep = (uint8)((bmic_battery_status >> 15U) & 1U);
    bmic_deepsleep = (uint8)((bmic_battery_status >> 14U) & 1U);
    bmic_cfgupdate = (uint8)((bmic_battery_status >> 5U) & 1U);
    bmic_read_ok = 1U;
    bmic_stage = 6U;
}

int main(void)
{
    Clock_Ip_Init(Clock_Ip_aClockConfig);
    bmic_stage = 1U;
    OsIf_Init(NULL_PTR);
    Port_Ci_Port_Ip_Init(
        NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
        g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals);
    bmic_stage = 2U;
    Lpi2c_Ip_MasterInit(BQ_I2C_INSTANCE, &I2c_Lpi2cMasterChannel0);
    bmic_stage = 3U;

    Bmic_ReadStatusOnce();
    bmic_done = 1U;
    for (;;)
    {
        /* Suspend here to inspect the one-shot result. Reset to retry. */
    }
}
