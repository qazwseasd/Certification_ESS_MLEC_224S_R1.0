/*
 * Api_CtrlLed.c
 *
 *  Created on: May 21, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Api_CtrlLed.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static uint8	CtrlLed_State;


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
 * @brief Initializes the control LED module.
 *
 * This function sets the initial state of the control LED system
 * to all LEDs off.
 */
void	CtrlLed_Init(void)
{
	CtrlLed_State = CtrlLed_St_AllOff;
}


/**
 * @brief Executes the control LED state machine.
 *
 * This function checks the current LED control state and updates
 * the LEDs (Yellow, Red, Green) accordingly.
 * If the state has not changed since the last call, the function returns immediately.
 *
 * @note Each state defines which LEDs are ON/OFF and their blinking period.
 */
void	CtrlLed_Task(void)
{
	static	uint8	preState = CtrlLed_St_AllOff;

	if (CtrlLed_State == preState)
	{
		return;
	}
#ifndef TestMode
	switch (CtrlLed_State)
	{
		case CtrlLed_St_AllOff:
			Led_Off(DRV_Led_Yellow);
			Led_Off(DRV_Led_Red);
			Led_Off(DRV_Led_Green);
			Led_Set_Period(DRV_Led_Yellow,	DRV_Led_Peri_0ms);
			Led_Set_Period(DRV_Led_Red,		DRV_Led_Peri_0ms);
			Led_Set_Period(DRV_Led_Green,	DRV_Led_Peri_0ms);
			break;

		case CtrlLed_St_Init:
			Led_Off(DRV_Led_Yellow);
			Led_Off(DRV_Led_Red);
			Led_Set_Period(DRV_Led_Yellow,	DRV_Led_Peri_0ms);
			Led_Set_Period(DRV_Led_Red,		DRV_Led_Peri_0ms);
			Led_Set_Period(DRV_Led_Green,	DRV_Led_Peri_100ms);
			break;

		case CtrlLed_St_Normal:
			Led_Off(DRV_Led_Yellow);
			Led_Off(DRV_Led_Red);
			Led_Set_Period(DRV_Led_Yellow,	DRV_Led_Peri_0ms);
			Led_Set_Period(DRV_Led_Red,		DRV_Led_Peri_0ms);
			Led_Set_Period(DRV_Led_Green,	DRV_Led_Peri_100ms);
			break;

		case CtrlLed_St_Warning:
			Led_Off(DRV_Led_Green);
			Led_Off(DRV_Led_Red);
			Led_Set_Period(DRV_Led_Yellow,	DRV_Led_Peri_100ms);
			Led_Set_Period(DRV_Led_Red,		DRV_Led_Peri_0ms);
			Led_Set_Period(DRV_Led_Green,	DRV_Led_Peri_0ms);
			break;

		case CtrlLed_St_Fault:
			Led_Off(DRV_Led_Yellow);
			Led_Off(DRV_Led_Green);
			Led_Set_Period(DRV_Led_Yellow,	DRV_Led_Peri_0ms);
			Led_Set_Period(DRV_Led_Red,		DRV_Led_Peri_100ms);
			Led_Set_Period(DRV_Led_Green,	DRV_Led_Peri_0ms);
			break;
		case CtrlLed_St_CFG:
			Led_Off(DRV_Led_Yellow);
			Led_Off(DRV_Led_Red);
			Led_Off(DRV_Led_Green);
			Led_Set_PeriodCount_Reset();
			Led_Set_Period(DRV_Led_Yellow,	DRV_Led_Peri_500ms);
			Led_Set_Period(DRV_Led_Red,		DRV_Led_Peri_500ms);
			Led_Set_Period(DRV_Led_Green,	DRV_Led_Peri_500ms);
			break;
		default:
			Led_On(DRV_Led_Yellow);
			Led_On(DRV_Led_Red);
			Led_On(DRV_Led_Green);
			Led_Set_Period(DRV_Led_Yellow,	DRV_Led_Peri_0ms);
			Led_Set_Period(DRV_Led_Red,		DRV_Led_Peri_0ms);
			Led_Set_Period(DRV_Led_Green,	DRV_Led_Peri_0ms);
			break;
	}
#endif
	preState = CtrlLed_State;
}


/**
 * @brief Sets the control LED state.
 *
 * This function updates the internal state variable that determines
 * how the LEDs are controlled.
 *
 * @param state New state for the control LED system.
 */
void	CtrlLed_Set_State(uint8 state)
{
	CtrlLed_State = state;
}


