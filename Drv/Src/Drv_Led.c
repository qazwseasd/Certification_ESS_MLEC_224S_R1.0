/*
 * Drv_Led.c
 *
 *  Created on: Mar 25, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Led.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/

#define Led_Cmd_On					(GPIO_PinState)GPIO_PIN_SET
#define Led_Cmd_Off					(GPIO_PinState)GPIO_PIN_RESET


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/

typedef struct
{
	GPIO_TypeDef	*port;
	uint16			pin;
	uint8			period;
	uint8			peri_cnt;
} Led_t;


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static	Led_t	Led[DRV_Led_NumOf]	=
{
#if (DRV_Led_NumOf >= 1u)
	{ DRV_Led_1_Port, DRV_Led_1_Pin, 0u, 0u },
#endif
#if (DRV_Led_NumOf >= 2u)
	{ DRV_Led_2_Port, DRV_Led_2_Pin, 0u, 0u },
#endif
#if (DRV_Led_NumOf >= 3u)
	{ DRV_Led_3_Port, DRV_Led_3_Pin, 0u, 0u },
#endif
#if (DRV_Led_NumOf >= 4u)
	{ DRV_Led_4_Port, DRV_Led_4_Pin, 0u, 0u },
#endif
#if (DRV_Led_NumOf >= 5u)
	{ DRV_Led_5_Port, DRV_Led_5_Pin, 0u, 0u },
#endif
};


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/

/**
 * @brief   Initializes the LED pins and turns off both LEDs.
 *
 * @details This function initializes the LED hardware by turning off both LED1 and LED2
 *          using the `Led_L1_Off()` and `Led_L2_Off()` functions.
 *
 * @param   None
 *          No input parameters are required for this function.
 *
 * @retval  None
 *          This function does not return a value.
 */
void	Led_Init(void)
{
	uint8	ledIdx;

	for (ledIdx = 0u; ledIdx < DRV_Led_NumOf; ledIdx++)
	{
		Led_Off(ledIdx);
	}
//	Led_Set_Period(DRV_Led_Red,		DRV_Led_Peri_100ms);
}


/**
 * @brief  LED control task.
 *
 * This task updates the LED states based on their configured blinking periods.
 * - For LEDs not in continuous-on mode, it increments a period counter.
 * - If the counter reaches the set period, the LED toggles and the counter resets.
 */
void	Led_Task(void)
{
	uint8	ledIdx;

	for (ledIdx = 0u; ledIdx < DRV_Led_NumOf; ledIdx++)
	{
		if (Led[ledIdx].period != DRV_Led_Peri_0ms)
		{
			Led[ledIdx].peri_cnt++;

			if (Led[ledIdx].peri_cnt >= Led[ledIdx].period)
			{
				Led[ledIdx].peri_cnt = 0u;
				Led_Toggle(ledIdx);
			}
			else{}
		}
		else{}
	}
}


/**
 * @brief  Turns off the specified LED.
 *
 * This function clears the GPIO pin associated with the given LED index,
 * effectively turning the LED off.
 *
 * @param  ledIdx Index of the LED (0 to DRV_Led_NumOf-1).
 */
void	Led_Off(uint8 ledIdx)
{
	if (ledIdx < DRV_Led_NumOf)
	{
		Dio_Clr(Led[ledIdx].port, Led[ledIdx].pin);
	}
	else{}
}


/**
 * @brief  Turns on the specified LED.
 *
 * This function sets the GPIO pin associated with the given LED index,
 * effectively turning the LED on.
 *
 * @param  ledIdx Index of the LED (0 to DRV_Led_NumOf-1).
 */
void	Led_On(uint8 ledIdx)
{
	if (ledIdx < DRV_Led_NumOf)
	{
		Dio_Set(Led[ledIdx].port, Led[ledIdx].pin);
	}
	else{}
}


/**
 * @brief  Toggles the state of the specified LED.
 *
 * This function inverts the current state of the GPIO pin associated with
 * the given LED index. If the LED is on, it turns off; if off, it turns on.
 *
 * @param  ledIdx Index of the LED (0 to DRV_Led_NumOf-1).
 */
void	Led_Toggle(uint8 ledIdx)
{
	if (ledIdx < DRV_Led_NumOf)
	{
		Dio_Toggle(Led[ledIdx].port, Led[ledIdx].pin);
	}
	else{}
}


/**
 * @brief  Sets the blinking period of the specified LED.
 *
 * This function configures the blink period of the given LED. The input period
 * value is divided by DRV_Led_Peri_Task to calculate the effective period count
 * used in the LED task.
 *
 * @param  ledIdx    Index of the LED (0 to DRV_Led_NumOf-1).
 * @param  setPeriod Desired blink period in task ticks. [ms]
 */
void	Led_Set_Period(uint8 ledIdx, uint8 setPeriod)
{
	if (ledIdx < DRV_Led_NumOf)
	{
		Led[ledIdx].period = setPeriod;
	}
	else{}
}


void	Led_Set_PeriodCount_Reset(void)
{
	uint8 ledIdx;
	
	for (ledIdx = 0u; ledIdx < DRV_Led_NumOf; ledIdx++)
	{
		Led[ledIdx].peri_cnt = 0u;
	}
}




