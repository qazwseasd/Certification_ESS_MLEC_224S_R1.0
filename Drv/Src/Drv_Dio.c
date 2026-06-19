/*
 * Drv_Dio.c
 *
 *  Created on: Sep 11, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Dio.h"


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
uint16	static BoardID;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
uint8   Dio_GetDipSwitch(void);



/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
 * @brief	Get board dip switch number
 * @param 	void
 * @return	uint8 - board dip switch number
*/
uint8 Dio_GetDipSwitch(void)
{
	uint32 rv = ~IDCOM_GPIO_Port->IDR;

	CLR_PORT_PIN(IDCOM_GPIO_Port, IDCOM_Pin);

	return (uint8)(rv >> 2);
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get board ID
 * @param 	void
 * @return	void
*/
void Dio_Earn_BoardID(void)
{ 
	BoardID = (uint8)(Dio_GetDipSwitch() & 0x0Fu);
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get HVIL EPO state
 * @param 	void
 * @return	BOOL, TRUE - On, FALES - X
*/
uint8 Dio_GetHVILEPO(void)
{
	uint8 Result;



	if(HAL_GPIO_ReadPin(Sig_HW_EPO_CTL_GPIO_Port, Sig_HW_EPO_CTL_Pin) != GPIO_PIN_RESET)
	{
		Result = TRUE;
	}
	else
	{
		Result = FALSE;
	}

	return Result;
}


/*---------------------------------------------------------------------*/
/**
* @brief 	Set the Pack Voltage Measurement Switch
* @param 	OnOff - ON:Switch on, OFF:Switch off
* @return 	void
*/
void Dio_SetFET_ForPackVolt(bool OnOff)
{
	if (OnOff == ON)
	{
		SET_PORT_PIN(Iso_PACK_FET_EN_GPIO_Port, Iso_PACK_FET_EN_Pin);
	}
	else
	{
		CLR_PORT_PIN(Iso_PACK_FET_EN_GPIO_Port, Iso_PACK_FET_EN_Pin);
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief 	Set the Bus Voltage Measurement Switch
* @param 	OnOff - ON:Switch on, OFF:Switch off
* @return	void
*/
void Dio_SetFET_ForBusVolt(bool OnOff)
{
	if (OnOff == ON)
	{
		SET_PORT_PIN(Iso_BUS_FET_EN_GPIO_Port, Iso_BUS_FET_EN_Pin);
	}
	else
	{
		CLR_PORT_PIN(Iso_BUS_FET_EN_GPIO_Port, Iso_BUS_FET_EN_Pin);
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief 	Set the Isolation Positive Switch
* @param 	OnOff - ON:Switch on, OFF:Switch off
* @return	void
*/
void Dio_SetFET_ForIsoR_High(bool OnOff)
{
	if (OnOff == ON)
	{
		SET_PORT_PIN(Iso_R_H_FET_EN_GPIO_Port, Iso_R_H_FET_EN_Pin);
	}
	else
	{
		CLR_PORT_PIN(Iso_R_L_FET_EN_GPIO_Port, Iso_R_H_FET_EN_Pin);
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief 	Set the Isolation Negative Switch
* @param 	OnOff - ON:Switch on, OFF:Switch off
* @return	void
*/
void Dio_SetFET_ForIsoR_Low(bool OnOff)
{
	if (OnOff == ON)
	{
		SET_PORT_PIN(Iso_R_L_FET_EN_GPIO_Port, Iso_R_L_FET_EN_Pin);
	}
	else
	{
		CLR_PORT_PIN(Iso_R_L_FET_EN_GPIO_Port, Iso_R_L_FET_EN_Pin);
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief 	Get the Isolation Positive Switch State
* @param 	void
* @return 	BOOL-Positive switch control pin state(TRUE - Close, FALSE - Open)
*/
uint8 Dio_GetFET_ForIsoR_High(void)
{
	uint8 Result;

	if(HAL_GPIO_ReadPin(Iso_R_H_FET_EN_GPIO_Port, Iso_R_H_FET_EN_Pin) != GPIO_PIN_RESET)
	{
		Result = FALSE;
	}
	else
	{
		Result = TRUE;
	}

	return Result;
}


/*---------------------------------------------------------------------*/
/**
* @brief 	Get the Isolation Negative Switch State
* @param 	void
* @return 	BOOL-Negative switch control pin state(TRUE - Close, FALSE - Open)
*/
uint8 Dio_GetFET_ForIsoR_Low(void)
{
	uint8 Result;

	if(HAL_GPIO_ReadPin(Iso_R_L_FET_EN_GPIO_Port, Iso_R_L_FET_EN_Pin) != GPIO_PIN_RESET)
	{
		Result = FALSE;
	}
	else
	{
		Result = TRUE;
	}

	return Result;
}


/*---------------------------------------------------------------------*/
/**
* @brief 	Set the Self HVIL Control State
* @param 	void
* @return 	BOOL-Self HVIL Control pin state(TRUE - Close, FALSE - Open)
*/
void Dio_Set_Self_HVIL_Ctrl(bool OnOff)
{
	if (OnOff == ON)
	{
		SET_PORT_PIN(HW_EPO_EN_GPIO_Port, HW_EPO_EN_Pin);
	}
	else
	{
		CLR_PORT_PIN(HW_EPO_EN_GPIO_Port, HW_EPO_EN_Pin);
	}
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Clear ADC Chip select pin
 * @param 	port - port select
 * @param 	pin - pin select
 * @return	void
*/
void Dio_Clear_ADCChipSelectPin(uint8 port, uint8 pin)
{
	if (port == 0u)
	{
		CLR_PORT_PIN(GPIOA, pin);
	}
	if (port == 1u)
	{
        CLR_PORT_PIN(GPIOB, pin);
	}
	if (port == 2u)
	{
		CLR_PORT_PIN(GPIOC, pin);
	}
	if (port == 3u)
	{
		CLR_PORT_PIN(GPIOD, pin);
	}
	if (port == 4u)
	{
		CLR_PORT_PIN(GPIOE, pin);
	}
	if (port == 5u)
	{
		CLR_PORT_PIN(GPIOF, pin);
	}
	if (port == 6u)
	{
		CLR_PORT_PIN(GPIOG, pin);
	}
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Set ADC Chip select pin
 * @param 	port - port select
 * @param 	pin - pin select
 * @return	void
*/
void Dio_Set_ADCChipSelectPin(uint8 port, uint8 pin)
{
	if (port == 0u)
	{
		SET_PORT_PIN(GPIOA, pin);
	}
	if (port == 1u)
	{
		SET_PORT_PIN(GPIOB, pin);
	}
	if (port == 2u)
	{
		SET_PORT_PIN(GPIOC, pin);
	}
	if (port == 3u)
	{
		SET_PORT_PIN(GPIOD, pin);
	}
	if (port == 4u)
	{
		SET_PORT_PIN(GPIOE, pin);
	}
	if (port == 5u)
	{
		SET_PORT_PIN(GPIOF, pin);
	}
	if (port == 6u)
	{
		SET_PORT_PIN(GPIOG, pin);
	}
}


/*---------------------------------------------------------------------*/
/**
 * @brief Toggles the state of a specified GPIO pin.
 *
 * This function inverts the current logic level of the given GPIO pin.
 * If the pin is HIGH, it will be set to LOW.
 * If the pin is LOW, it will be set to HIGH.
 *
 * @param port GPIO port to which the pin belongs (e.g., GPIOA, GPIOB).
 * @param pin  GPIO pin number (e.g., GPIO_PIN_0, GPIO_PIN_1).
 */
void	Dio_Toggle(GPIO_TypeDef *port, uint16_t pin)
{
	HAL_GPIO_TogglePin(port, pin);
}


/**
 * @brief Sets the specified GPIO pin to HIGH.
 *
 * This function drives the given GPIO pin to a HIGH logic level.
 *
 * @param port GPIO port to which the pin belongs (e.g., GPIOA, GPIOB).
 * @param pin  GPIO pin number (e.g., GPIO_PIN_0, GPIO_PIN_1).
 */
void	Dio_Set(GPIO_TypeDef *port, uint16_t pin)
{
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}


/**
 * @brief Clears the specified GPIO pin to LOW.
 *
 * This function drives the given GPIO pin to a LOW logic level.
 *
 * @param port GPIO port to which the pin belongs (e.g., GPIOA, GPIOB).
 * @param pin  GPIO pin number (e.g., GPIO_PIN_0, GPIO_PIN_1).
 */
void	Dio_Clr(GPIO_TypeDef *port, uint16_t pin)
{
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}


/*---------------------------------------------------------------------*/
uint16 Dio_Get_BoardID(void)
{
	return BoardID;
}

