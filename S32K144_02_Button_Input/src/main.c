/*
*   Copyright 2020 NXP
*
*   NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly
*   in accordance with the applicable license terms.  By expressly accepting
*   such terms or by downloading, installing, activating and/or otherwise using
*   the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms.  If you do not agree to
*   be bound by the applicable license terms, then you may not retain,
*   install, activate or otherwise use the software.
*
*   This file contains sample code only. It is not part of the production code deliverables.
*/

#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Port_Ci_Port_Ip.h"
#include "Gpio_Dio_Ip.h"
#include "Clock_Ip.h"

#include "check_example.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/


//void TestDelay(uint32 delay);
//void TestDelay(uint32 delay)
//{
//   static volatile uint32 DelayTimer = 0;
//   while(DelayTimer<delay)
//   {
//       DelayTimer++;
//   }
//   DelayTimer=0;
//}

/**
* @brief        Main function of the example
* @details      Initialize the used drivers and uses the Dio
*               driver to toggle a LED on a push button
*/
//int main(void)
//{
//    uint8 i = 0U;
//    /* Initialize the clock driver */
//    Clock_Ip_Init(Clock_Ip_aClockConfig);
//
//#if defined (FEATURE_CLOCK_IP_HAS_SPLL_CLK)
//    while ( CLOCK_IP_PLL_LOCKED != Clock_Ip_GetPllStatus() )
//    {
//        /* Busy wait until the System PLL is locked */
//    }
//
//    Clock_Ip_DistributePll();
//#endif
//    /* Initialize all pins using the Port driver */
//    Port_Ci_Port_Ip_Init(NUM_OF_CONFIGURED_PINS_PortContainer_0_VS_0, g_pin_mux_InitConfigArr_PortContainer_0_VS_0);
//
//    while (i++ < 10)
//    {
//        /* Dio_WriteChannel(DioConf_DioChannel_Digital_Output_LED_D78, STD_HIGH); */
//        Gpio_Dio_Ip_WritePin(LED_PORT, LED_PIN, 1U);
//        TestDelay(4800000);
//        /* Dio_WriteChannel(DioConf_DioChannel_Digital_Output_LED_D78, STD_LOW); */
//        Gpio_Dio_Ip_WritePin(LED_PORT, LED_PIN, 0U);
//        TestDelay(4800000);
//    }
//
//    Exit_Example(TRUE);
//
//    return (0U);
//}

int main(void)
{
    uint8 sw2State = 0U;
    uint8 sw3State = 0u;

    /* 시스템 클록 초기화 */
    Clock_Ip_Init(Clock_Ip_aClockConfig);

#if defined(FEATURE_CLOCK_IP_HAS_SPLL_CLK)
    while (CLOCK_IP_PLL_LOCKED != Clock_Ip_GetPllStatus())
    {
        /* PLL이 안정화될 때까지 대기 */
    }

    Clock_Ip_DistributePll();
#endif

    /* LED와 SW2 핀 설정 적용 */
    Port_Ci_Port_Ip_Init(
        NUM_OF_CONFIGURED_PINS_PortContainer_0_VS_0,
        g_pin_mux_InitConfigArr_PortContainer_0_VS_0
    );

    /*
     * RGB LED는 Active-Low
     * 1U = OFF
     * 0U = ON
     */
    Gpio_Dio_Ip_WritePin(BLUE_LED_PORT, BLUE_LED_PIN, 1U);
    Gpio_Dio_Ip_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, 1U);
    Gpio_Dio_Ip_WritePin(RED_LED_PORT, RED_LED_PIN, 1U);

    while (1)
    {
        /* SW2 버튼 상태 읽기 */
        sw2State = Gpio_Dio_Ip_ReadPin(
            SW2_BUTTON_PORT,
            SW2_BUTTON_PIN
        );

        sw3State = Gpio_Dio_Ip_ReadPin(
        	SW3_BUTTON_PORT,
        	SW3_BUTTON_PIN
        );

        if (sw2State == 1U)
        {
            /* SW2를 누르면 초록 LED ON */
            Gpio_Dio_Ip_WritePin(
                GREEN_LED_PORT,
                GREEN_LED_PIN,
                0U
            );
        }
        else
        {
            /* SW2에서 손을 떼면 초록 LED OFF */
            Gpio_Dio_Ip_WritePin(
                GREEN_LED_PORT,
                GREEN_LED_PIN,
                1U
            );
        }

        if (sw3State == 1U)
                {
                    /* SW2를 누르면 초록 LED ON */
                    Gpio_Dio_Ip_WritePin(
                        RED_LED_PORT,
                        RED_LED_PIN,
                        0U
                    );
                }
                else
                {
                    /* SW2에서 손을 떼면 초록 LED OFF */
                    Gpio_Dio_Ip_WritePin(
                        RED_LED_PORT,
                        RED_LED_PIN,
                        1U
                    );
                }
    }

    return 0;
}

#ifdef __cplusplus
}
#endif

/** @} */
