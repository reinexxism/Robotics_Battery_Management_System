/*==================================================================================================
* Project : RTD AUTOSAR 4.7
* Platform : CORTEXM
* Peripheral : S32K14X
* Dependencies : none
*
* Autosar Version : 4.7.0
* Autosar Revision : ASR_REL_4_7_REV_0000
* Autosar Conf.Variant :
* SW Version : 3.0.0
* Build Version : S32K1_RTD_3_0_0_D2503_ASR_REL_4_7_REV_0000_20250307
*
* Copyright 2020-2025 NXP
*
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be 
*   used strictly in accordance with the applicable license terms.  By expressly 
*   accepting such terms or by downloading, installing, activating and/or otherwise 
*   using the software, you are agreeing that you have read, and that you agree to 
*   comply with and are bound by, such license terms.  If you do not agree to be 
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

/* Including necessary configuration files. */
#include "Mcal.h"

volatile int exit_code = 0;
/* User includes */
#include "bms_power_led.h"
#include "bq76907_i2c.h"
#include "bq76907_status.h"
#include "bq76907_config.h"

/*General includes*/
#include "Clock_Ip.h"
#include "Clock_Ip_Cfg.h"
#include "Port_Ci_Port_Ip.h"
#include "Port_Ci_Port_Ip_Cfg.h"

/*General variation*/
static volatile uint8 bmic_read_done = 0U;
static volatile Lpi2c_Ip_StatusType bmic_read_result;
static volatile uint16 bmic_battery_status;
static volatile Bq76907_7sCheck bmic_7s_check = BQ_7S_UNKNOWN;
static volatile uint8 bmic_read_request = 1U;
static volatile uint32 bmic_read_count = 0U;
static volatile uint32 bmic_retry_count = 0U;

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your code here */
	Clock_Ip_Init(Clock_Ip_aClockConfig);

	Port_Ci_Port_Ip_Init(
	    NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
	    g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals
	);

	BmsPowerLed_On();
	Bq76907_I2cInit();

    for (;;)
    {
        if (bmic_read_request != 0U)
        {
            uint16 status = 0U;

            bmic_read_request = 0U;
            bmic_read_done = 0U;

            bmic_read_result = BmicStatus_Read(&status);

            if (bmic_read_result == LPI2C_IP_RECEIVED_NACK_STATUS)
            {
                bmic_retry_count++;
                bmic_read_result = BmicStatus_Read(&status);
            }

            if (bmic_read_result == LPI2C_IP_SUCCESS_STATUS)
            {
                bmic_battery_status = status;
                Bq76907_ConfigReadPending();
                bmic_7s_check = Bq76907_ConfigCheck7s();
            }

            bmic_read_count++;
            bmic_read_done = 1U;
        }

        if (exit_code != 0)
        {
            break;
        }
    }

    return exit_code;
}

/** @} */
