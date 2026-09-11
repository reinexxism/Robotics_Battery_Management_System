#include "Mcal.h"
#include "S32K144_PCC.h"
#include "S32K144_LPI2C.h"
#include "Port_Ci_Port_Ip.h"
#include "Port_Ci_Port_Ip_Cfg.h"
#include "Clock_Ip.h"
#include "Clock_Ip_Cfg.h"

volatile Clock_Ip_StatusType clock_status = CLOCK_IP_ERROR;
#define CELL_COUNT 6U
/* Array index 0 is CELL1; index 5 is CELL6. */
volatile int16_t cell_mV[CELL_COUNT] = {-1, -1, -1, -1, -1, -1};
volatile uint32_t cell_result[CELL_COUNT] = {0U};
volatile uint32_t cells_read = 0U;

/* 0=pending, 1=success, 2=NACK, 3=timeout, 4=clock error, 5=bus/FIFO error */
volatile uint32_t i2c_result = 0U;
volatile uint32_t i2c_last_msr = 0U;
volatile uint32_t read_done = 0U;

/* Bounded polling: iteration limit, not a calibrated time in milliseconds. */
static int WaitFlag(uint32_t flag)
{
    for (uint32_t n = 0U; n < 1000000U; n++)
    {
        uint32_t s = IP_LPI2C0->MSR;
        i2c_last_msr = s;
        if ((s & LPI2C_MSR_NDF_MASK) != 0U)
        {
            i2c_result = 2U;
            return 0;
        }
        if ((s & (LPI2C_MSR_ALF_MASK | LPI2C_MSR_FEF_MASK |
                  LPI2C_MSR_PLTF_MASK)) != 0U)
        {
            i2c_result = 5U;
            return 0;
        }
        if ((s & flag) != 0U) { return 1; }
    }
    i2c_result = 3U;
    return 0;
}

static void ReadCell(uint8_t register_address, uint32_t cell_index)
{
    uint8_t rx[2];
    i2c_result = 0U;
    /* W1C: writing 1 clears old completion/error flags, not writing 0.
     * The previous transaction has completed; stop the scan on any failure.
     */
    IP_LPI2C0->MSR = LPI2C_MSR_EPF_MASK | LPI2C_MSR_SDF_MASK |
                     LPI2C_MSR_NDF_MASK | LPI2C_MSR_ALF_MASK |
                     LPI2C_MSR_FEF_MASK | LPI2C_MSR_PLTF_MASK;
    /* CMD 4=START, 0=transmit, 1=receive N+1 bytes, 2=STOP.
     * Default BQ address: 0x08 (7-bit), 0x10 write, 0x11 read.
     * CRC must be disabled. Cell addresses: 0x14,16,18,1A,1C,1E.
     */
    const uint32_t commands[] = {
        LPI2C_MTDR_CMD(4U) | LPI2C_MTDR_DATA(0x10U),
        LPI2C_MTDR_CMD(0U) | LPI2C_MTDR_DATA(register_address),
        LPI2C_MTDR_CMD(4U) | LPI2C_MTDR_DATA(0x11U),
        LPI2C_MTDR_CMD(1U) | LPI2C_MTDR_DATA(1U),
        LPI2C_MTDR_CMD(2U)
    };

    for (uint32_t i = 0U; i < 5U; i++)
    {
        if (!WaitFlag(LPI2C_MSR_TDF_MASK)) { return; }
        IP_LPI2C0->MTDR = commands[i];
    }
    for (uint32_t i = 0U; i < 2U; i++)
    {
        if (!WaitFlag(LPI2C_MSR_RDF_MASK)) { return; }
        rx[i] = (uint8_t)IP_LPI2C0->MRDR;
    }
    if (!WaitFlag(LPI2C_MSR_SDF_MASK)) { return; }

    cell_mV[cell_index] = (int16_t)((uint16_t)rx[0] | ((uint16_t)rx[1] << 8U));
    i2c_result = 1U;
}

int main(void)
{
    clock_status = Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    if (clock_status != CLOCK_IP_SUCCESS)
    {
        i2c_result = 4U;
        read_done = 1U;
        for (;;) { }
    }

    IP_PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK;
    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
    Port_Ci_Port_Ip_Init(
        NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
        g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals);

    /* Existing generated clock configuration: SIRCDIV2 = 8 MHz. */
    IP_PCC->PCCn[PCC_LPI2C0_INDEX] |= PCC_PCCn_CGC_MASK;
    IP_LPI2C0->MCR = LPI2C_MCR_RST_MASK;
    IP_LPI2C0->MCR = 0U;
    IP_LPI2C0->MIER = 0U;
    IP_LPI2C0->MDER = 0U;
    IP_LPI2C0->MCFGR0 = 0U;
    /* PRESCALE=0: divide by 1; PINCFG=0: two-pin open-drain. */
    IP_LPI2C0->MCFGR1 = 0U;
    IP_LPI2C0->MCFGR2 = LPI2C_MCFGR2_BUSIDLE(80U);
    IP_LPI2C0->MCFGR3 = 0U;
    /* Approximately 100 kHz from the 8 MHz peripheral clock. */
    IP_LPI2C0->MCCR0 = LPI2C_MCCR0_CLKLO(39U) |
                      LPI2C_MCCR0_CLKHI(39U) |
                      LPI2C_MCCR0_SETHOLD(19U) |
                      LPI2C_MCCR0_DATAVD(9U);
    IP_LPI2C0->MFCR = 0U;
    IP_LPI2C0->MCR = LPI2C_MCR_MEN_MASK;

    /* Six separate two-byte reads, once per program run. */
    for (uint32_t cell = 0U; cell < CELL_COUNT; cell++)
    {
        uint8_t address = (uint8_t)(0x14U + (2U * cell));
        ReadCell(address, cell);
        cell_result[cell] = i2c_result;
        if (i2c_result != 1U) { break; }
        cells_read++;
    }
    if (i2c_result != 1U)
    {
        /* Abort a failed transaction and release the controller outputs. */
        IP_LPI2C0->MCR = LPI2C_MCR_RST_MASK;
        IP_LPI2C0->MCR = 0U;
    }
    read_done = 1U;
    for (;;) { } /* Set a breakpoint here; do not single-step the transfer. */
}
