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

/* LPUART 드라이버 및 생성된 설정 */
#include "Lpuart_Uart_Ip.h"
#include "Lpuart_Uart_Ip_Sa_VS_0_PBcfg.h"

#include "freemaster.h"

/*LPIT 드라이버 및 생성된 설정*/
#include "LPit_Gpt_Ip.h"
#include "LPit_Gpt_Ip_VS_0_PBcfg.h"

#include "IntCtrl_Ip.h"
#include "IntCtrl_Ip_Cfg.h"

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

volatile uint8 g_led_flag = 0u;
static uint8 g_last_led_flag = 0u;


volatile Lpuart_Uart_Ip_StatusType g_uart_tx_status;

void Lpit0_Ch0_Notification(uint8 channel)
{
	(void)channel;

	g_led_flag ^= 1u;
}

/* NVIC에서 직접 진입하는 실제 Interrupt Handler */
void LPIT0_Ch0_IRQHandler(void)
{
    if (TRUE == Lpit_Gpt_Ip_GetChannelInterruptFlagStatus(0U, 0U))
    {
        /* LPIT 채널 0의 인터럽트 Flag 제거 */
        Lpit_Gpt_Ip_ClearChannelInterruptFlag(0U, 0U);

        /* 사용자 Callback 실행 */
        Lpit0_Ch0_Notification(0U);
    }
}

int main(void)
{
    /* 시스템 클록 초기화 */
    Clock_Ip_Init(Clock_Ip_aClockConfig);

#if defined(FEATURE_CLOCK_IP_HAS_SPLL_CLK)
    while (CLOCK_IP_PLL_LOCKED != Clock_Ip_GetPllStatus())
    {
        /* PLL이 안정화될 때까지 대기 */
    }

    Clock_Ip_DistributePll();
#endif

    /* PTD0, PTD16, PTD15 핀 설정 적용 */
    Port_Ci_Port_Ip_Init(
        NUM_OF_CONFIGURED_PINS_PortContainer_0_VS_0,
        g_pin_mux_InitConfigArr_PortContainer_0_VS_0
    );

    IntCtrl_Ip_Init(&IntCtrlConfig_0);

    /* LPUART1 초기화 - FreeMASTER 통신용 */
    Lpuart_Uart_Ip_Init(
        LPUART_UART_IP_INSTANCE_USING_1,
        &Lpuart_Uart_Ip_xHwConfigPB_1_VS_0
    );

    FMSTR_Init();

//   LPIT 0번 모듈 초기화
    Lpit_Gpt_Ip_Init(
    	0U,
		&LPIT_0_InitConfig_PB_VS_0
//	LPIT 0번 모듈의 채널 0 초기화
    );
    Lpit_Gpt_Ip_InitChannel(
    	0U,
		&LPIT_0_ChannelConfig_PB_VS_0[0U]
    );
//    LPIT 채널 0의 인터럽트 활성화
    Lpit_Gpt_Ip_EnableChInterrupt(0U, 0U);
//   LPIT 채널 0을 1초 주기로 시작
    Lpit_Gpt_Ip_StartTimer(0U, 0U, 1000000U);

    /* Active-Low LED이므로 1U는 LED OFF */
    Gpio_Dio_Ip_WritePin(BLUE_LED_PORT, BLUE_LED_PIN, 1U);
    Gpio_Dio_Ip_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, 1U);
    Gpio_Dio_Ip_WritePin(RED_LED_PORT, RED_LED_PIN, 1U);

    while (1)
    {

    	 FMSTR_Poll();

//    	if (g_led_flag != g_last_led_flag)
//    	    {
//    	        g_last_led_flag = g_led_flag;
//
//    	        g_uart_tx_status = Lpuart_Uart_Ip_SyncSend(
//    	            LPUART_UART_IP_INSTANCE_USING_1,
//    	            g_uart_test_msg,
//    	            sizeof(g_uart_test_msg) - 1U,
//    	            100000U
//    	        );
//    	    }

    	 if (g_led_flag == 1U)
    	    {
    	        /* Active-Low: 0U = LED ON */
    	        Gpio_Dio_Ip_WritePin(
    	            GREEN_LED_PORT,
    	            GREEN_LED_PIN,
    	            0U
    	        );
    	    }
    	    else
    	    {
    	        /* Active-Low: 1U = LED OFF */
    	        Gpio_Dio_Ip_WritePin(
    	            GREEN_LED_PORT,
    	            GREEN_LED_PIN,
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
