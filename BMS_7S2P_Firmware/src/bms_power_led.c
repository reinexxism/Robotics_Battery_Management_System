/*
 * bms_power_led.c
 *
 *  Created on: 2026. 11. 2.
 *      Author: nosta
 */

#include "bms_power_led.h"
#include "S32K144_GPIO.h"

void BmsPowerLed_On(void)
{
    IP_PTD->PSOR = (1U << 1U);
}
