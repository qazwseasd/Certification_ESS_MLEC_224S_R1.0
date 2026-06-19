/*
 * Drv_Fan.c
 *
 *  Created on: Sep 12, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Fan.h"


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
 * @brief	Initialize FAN Driver
 * @param 	void
 * @return	BOOL, TRUE - Success, FALSE - X
*/
uint8 Fan_Init(void)
{
	(void)Fan_SetStateHigh(FAN_SW_OFF); 
	(void)Fan_SetStateLow(FAN_SW_OFF); 

	return DEF_True;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Set FAN high state
 * @param 	FS - FAN high state
 * @return	BOOL, TRUE - Success, FALSE - X
*/
uint8 Fan_SetStateHigh(Fan_State FS)
{	
	if (FS == FAN_SW_ON)
	{
		HAL_GPIO_WritePin(FAN_CON_1_H_EN_GPIO_Port, FAN_CON_1_H_EN_Pin, GPIO_PIN_SET);
	}
	else	
	{
		HAL_GPIO_WritePin(FAN_CON_1_H_EN_GPIO_Port, FAN_CON_1_H_EN_Pin, GPIO_PIN_RESET);
	}
	return DEF_True;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Set FAN Low state
 * @param 	FS - FAN Low state
 * @return	BOOL, TRUE - Success, FALSE - X
*/
uint8 Fan_SetStateLow(Fan_State FS)
{	
	if (FS == FAN_SW_ON)
	{
		HAL_GPIO_WritePin(FAN_CON_1_PWM_GPIO_Port, FAN_CON_1_PWM_Pin, GPIO_PIN_SET);
	}
	else	
	{
		HAL_GPIO_WritePin(FAN_CON_1_PWM_GPIO_Port, FAN_CON_1_PWM_Pin, GPIO_PIN_RESET);
	}
	return DEF_True;
}

/*---------------------------------------------------------------------*/
/**
* @brief	Get FAN state
* @param	void
* @return	Relay_State, 0 - OFF, 1 - ON
*/
Fan_State Fan_GetState(void)
{
	Fan_State Result;
	
	GPIO_PinState en  = HAL_GPIO_ReadPin(FAN_CON_1_H_EN_GPIO_Port, FAN_CON_1_H_EN_Pin);
	GPIO_PinState pwm = HAL_GPIO_ReadPin(FAN_CON_1_PWM_GPIO_Port, FAN_CON_1_PWM_Pin);

	Result = ((en == GPIO_PIN_SET) && (pwm == GPIO_PIN_SET)) ? FAN_SW_ON : FAN_SW_OFF;

	return Result;
}

