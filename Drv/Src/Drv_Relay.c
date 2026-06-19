/*
 * Drv_Relay.c
 *
 *  Created on: Sep 12, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Relay.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/
CLASSB static uint8 Relay_PosNegPreCommand;


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
* @brief	Initialize Relay setting
* @param	void
* @return	BOOL, TRUE - Success
*/
uint8 Relay_Init(void)
{
	Relay_PosNegPreCommand = 0u;
	Relay_SetAllRelayLow(RELAY_OFF);
	Relay_SetAllRelayHigh(RELAY_OFF);
	return DEF_True;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set Positive Relay High state
* @param	RS - Relay High state
* @return	void
*/
void Relay_SetPosRelayHigh(Relay_State RS)
{
	if (RS == RELAY_ON)
	{
		HAL_GPIO_WritePin(MAIN_CON_H1_H_EN_GPIO_Port, MAIN_CON_H1_H_EN_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(MAIN_CON_H1_H_EN_GPIO_Port, MAIN_CON_H1_H_EN_Pin, GPIO_PIN_RESET);
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set Positive Relay Low state
* @param	RS - Relay Low state
* @return	void
*/
void Relay_SetPosRelayLow(Relay_State RS)
{
	if (RS == RELAY_ON)
	{
		HAL_GPIO_WritePin(MAIN_CON_H1_L_EN_GPIO_Port, MAIN_CON_H1_L_EN_Pin, GPIO_PIN_SET);
		Relay_PosNegPreCommand |= RELAY_POS_FLAG;
	}
	else
	{
        HAL_GPIO_WritePin(MAIN_CON_H1_L_EN_GPIO_Port, MAIN_CON_H1_L_EN_Pin, GPIO_PIN_RESET);
		Relay_PosNegPreCommand &= ~(RELAY_POS_FLAG);
	}
}

/*---------------------------------------------------------------------*/
/**
* @brief	Get Positive Relay state
* @param	void
* @return	Relay_State, 0 - OFF, 1 - ON
*/
Relay_State Relay_GetPosRelay(void)
{
	Relay_State result;

	if (HAL_GPIO_ReadPin(M1__Aux_ConA10_GPIO_Port, M1__Aux_ConA10_Pin) != GPIO_PIN_RESET)
	{
		result = RELAY_OFF; 
	}
	else															
	{
		result = RELAY_ON;
	}

	return result;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set Negative Relay High state
* @param	RS - Relay High state
* @return	void
*/
void Relay_SetNegRelayHigh(Relay_State RS)
{
	if (RS == RELAY_ON)
	{
		HAL_GPIO_WritePin(MAIN_CON_L1_H_EN_GPIO_Port, MAIN_CON_L1_H_EN_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(MAIN_CON_L1_H_EN_GPIO_Port, MAIN_CON_L1_H_EN_Pin, GPIO_PIN_RESET);
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set Negative Relay Low state
* @param	RS - Relay Low state
* @return	void
*/
void Relay_SetNegRelayLow(Relay_State RS)
{
	if (RS == RELAY_ON)
	{
		HAL_GPIO_WritePin(MAIN_CON_L1_L_EN_GPIO_Port, MAIN_CON_L1_L_EN_Pin, GPIO_PIN_SET);
		Relay_PosNegPreCommand |= RELAY_NEG_FLAG;
	}
	else
	{
		HAL_GPIO_WritePin(MAIN_CON_L1_L_EN_GPIO_Port, MAIN_CON_L1_L_EN_Pin, GPIO_PIN_RESET);
		Relay_PosNegPreCommand &= ~(RELAY_NEG_FLAG);
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get Negative Relay state
* @param	void
* @return	Relay_State, 0 - OFF, 1 - ON
*/
Relay_State Relay_GetNegRelay(void)
/*
	get String 1 Negative relay 
*/
{
	Relay_State result;
	
	if (HAL_GPIO_ReadPin(M1__Aux_Con_GPIO_Port, M1__Aux_Con_Pin) != GPIO_PIN_RESET)
	{
		result = RELAY_OFF; 
	}
	else															
	{
		result = RELAY_ON;
	}

	return result;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set Precharge Relay High state
* @param	RS - Relay High state
* @return	void
*/
void Relay_SetPreRelayHigh(Relay_State RS)
{
	if (RS == RELAY_ON)
	{
		HAL_GPIO_WritePin(PRE_CON_H_EN_GPIO_Port, PRE_CON_H_EN_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(PRE_CON_H_EN_GPIO_Port, PRE_CON_H_EN_Pin, GPIO_PIN_RESET);
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set Precharge Relay Low state
* @param	RS - Relay Low state
* @return	void
*/
void Relay_SetPreRelayLow(Relay_State RS)
{
	if (RS == RELAY_ON)
	{
		HAL_GPIO_WritePin(PRE_CON_L_EN_GPIO_Port, PRE_CON_L_EN_Pin, GPIO_PIN_SET);
		Relay_PosNegPreCommand |= RELAY_PRE_FLAG;
	}
	else
	{
		HAL_GPIO_WritePin(PRE_CON_L_EN_GPIO_Port, PRE_CON_L_EN_Pin, GPIO_PIN_RESET);
		Relay_PosNegPreCommand &= ~(RELAY_PRE_FLAG);
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set All Relay Low state
* @param	RS - Relay Low state
* @return	void
*/
void Relay_SetAllRelayLow(Relay_State RS)
{
	if (RS == RELAY_OFF)
	{		
		Relay_SetPosRelayLow(RELAY_OFF);
		Relay_SetNegRelayLow(RELAY_OFF);
		Relay_SetPreRelayLow(RELAY_OFF);
	}
	else
	{	
		Relay_SetPosRelayLow(RELAY_ON);
		Relay_SetNegRelayLow(RELAY_ON);
		Relay_SetPreRelayLow(RELAY_ON);	
	}	
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set All Relay High state
* @param	RS - Relay High state
* @return	void
*/
void Relay_SetAllRelayHigh(Relay_State RS)
{
	if (RS == RELAY_OFF)
	{		
		Relay_SetPosRelayHigh(RELAY_OFF);
		Relay_SetNegRelayHigh(RELAY_OFF);
		Relay_SetPreRelayHigh(RELAY_OFF);
	}
	else
	{	
		Relay_SetPosRelayHigh(RELAY_ON);
		Relay_SetNegRelayHigh(RELAY_ON);
		Relay_SetPreRelayHigh(RELAY_ON);		
	}	
}

/*---------------------------------------------------------------------*/
/**
* @brief	Get All Relay state
* @param	void
* @return	uint8 - All Relay state
*/
uint8 Relay_GetAllRelay(void)
{
	uint8 relay_aux_status = 0;

	if(Relay_GetPosRelay() == RELAY_ON)	
	{
		relay_aux_status |= RELAY_POS_FLAG;
	}
	if(Relay_GetNegRelay() == RELAY_ON)	
	{
		relay_aux_status |= RELAY_NEG_FLAG;
	}
	
	return relay_aux_status;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get main relay command
* @param	void
* @return	uint8 - main relay command
*/
uint8 Relay_GetMainRelayCommand(void)
{
	return Relay_PosNegPreCommand;
}
/*---------------------------------------------------------------------*/


