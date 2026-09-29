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
    uint8 led;

    Clock_Ip_Init(Clock_Ip_aClockConfig);

    Port_Ci_Port_Ip_Init(
        NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
        g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals
    );

    /* 3. 처음에는 LED 모두 OFF */
    LED_SetMask(0U);

    /* 4. LED1 -> LED2 -> LED3 -> LED4 반복 */
    for (;;)
    {
        for (led = 0U; led < 4U; led++)
        {
            g_led_test_step = led + 1U;

            LED_SetMask((uint8)(1U << led));
            LED_Delay();

            LED_SetMask(0U);
            g_led_test_step = 0U;
            LED_Delay();
        }
    }
}
