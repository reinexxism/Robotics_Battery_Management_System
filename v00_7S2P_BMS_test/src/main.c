#include "Clock_Ip.h"
#include "Port_Ci_Port_Ip.h"
#include "Port_Ci_Port_Ip_Cfg.h"
#include "Gpio_Dio_Ip.h"

volatile uint8 g_led_test_step = 0U;

static void LED_Delay(void)
{
    volatile uint32 count;

    for (count = 0U; count < 1000000U; count++)
    {
        /* Busy wait */
    }
}

static void LED_SetMask(uint8 mask)
{
    Gpio_Dio_Ip_WritePin(
        IP_PTD, 1U, (uint8)((mask >> 0U) & 1U));

    Gpio_Dio_Ip_WritePin(
        IP_PTD, 0U, (uint8)((mask >> 1U) & 1U));

    Gpio_Dio_Ip_WritePin(
        IP_PTE, 11U, (uint8)((mask >> 2U) & 1U));

    Gpio_Dio_Ip_WritePin(
        IP_PTE, 10U, (uint8)((mask >> 3U) & 1U));
}

int main(void)
{
    Clock_Ip_Init(Clock_Ip_aClockConfig);

    Port_Ci_Port_Ip_Init(
        NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
        g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals
    );

    /* 3. 처음에는 LED 모두 OFF */
    LED_SetMask(0U);

    for (;;)
        {
            /*
             * 테스트 1: LED1~3 고정 ON, LED4 OFF
             *
             * 0x07 = 이진수 0111
             * LED4 LED3 LED2 LED1
             *   0    1    1    1
             */
//            LED_SetMask(0x07U);
//            continue;  /* 아래 테스트 2를 건너뛰고 반복 */

            /*
             * 테스트 2: LED1~3 고정 ON, LED4 반복 점멸
             *
             * 테스트 1의 위 두 줄을 주석 처리하면 실행됨.
             */

            /* LED1~3 ON 유지 + LED4 ON */
            LED_SetMask(0x0FU);  /* 1111 */
            LED_Delay();

            /* LED1~3 ON 유지 + LED4 OFF */
            LED_SetMask(0x07U);  /* 0111 */
            LED_Delay();
        }
}
