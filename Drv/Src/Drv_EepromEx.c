/*
 * Drv_EepromEx.c
 *
 *  Created on: Sep 15, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_EepromEx.h"


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
 * @brief	Initialize External EEPROM Driver
 * @param 	void
 * @return	BOOL, DEF_True - Success, Def_False - X
*/
uint8 EepromEx_Init(void)
{	
	(void)AdcEx_Init(Main_IEE_CS_GPIO_Port, Main_IEE_CS_Pin);
	return DEF_True;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Read data of External EEPROM
 * @param 	u16ByteIdx - Address
 * @param 	u16Len - Length of read data
 * @param 	eData -  Pointer variable of read data
 * @return	BOOL, DEF_True - Success, Def_False - X
*/
bool EepromEx_ReadData(uint32 u16ByteIdx, uint32 u16Len, uint8* eData)
{	
	uint8* str = eData;
	bool Result;
	uint8 TxData = EEPROMEX_CMD_READ;
	uint8 u16_ByteIdx[3];

	u16_ByteIdx[0] = ((uint8)(u16ByteIdx >> 16u));
	u16_ByteIdx[1] = ((uint8)(u16ByteIdx >> 8u));
	u16_ByteIdx[2] = ((uint8)(u16ByteIdx & 0xffu));
	
	if((u16ByteIdx + u16Len) > EEPROMEX_MAXSIZE)	// A reading address exceeds EEPROM Address size 
	{
		Result = DEF_False;				
	}
	else
	{
		HAL_GPIO_WritePin(Main_IEE_CS_GPIO_Port, Main_IEE_CS_Pin, GPIO_PIN_RESET);
		delay_us(1u);
		
		(void)DRV_SPI_Transmit(&hspi2, &TxData, 1u);
		(void)DRV_SPI_Transmit(&hspi2, u16_ByteIdx, 3u);

		(void)DRV_SPI_Receive(&hspi2, str, (uint16)u16Len);
		delay_us(1u);

		HAL_GPIO_WritePin(Main_IEE_CS_GPIO_Port, Main_IEE_CS_Pin, GPIO_PIN_SET);

		Result = DEF_True;
	}

	return Result;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Write data of External EEPROM
 * @param 	u16ByteIdx - Address
 * @param 	u16Len - Length of write data
 * @param 	eData -  Pointer variable of write data
 * @return	BOOL, DEF_True - Success, Def_False - X
*/
bool EepromEx_WriteData(uint32 u16ByteIdx, uint32 u16Len, uint8* eData)
{
	uint32 addr = u16ByteIdx;
	uint32 len = u16Len;
	uint8* u8Data = eData;
	bool Result;
	uint8 TxData1 = EEPROMEX_CMD_WREN;
	uint8 TxData2 = EEPROMEX_CMD_WRITE;
	uint8 u8_addr[3];

	u8_addr[0] = ((uint8)(addr >> 16u));
	u8_addr[1] = ((uint8)(addr >> 8u));
	u8_addr[2] = ((uint8)(addr & 0xffu));

	if((addr + len) > 0x3ffffu)		
	{
		Result = DEF_False;
	}
	else
	{
		HAL_GPIO_WritePin(Main_IEE_CS_GPIO_Port, Main_IEE_CS_Pin, GPIO_PIN_RESET);
		(void)DRV_SPI_Transmit(&hspi2, &TxData1, 1u);
		HAL_GPIO_WritePin(Main_IEE_CS_GPIO_Port, Main_IEE_CS_Pin, GPIO_PIN_SET);
		delay_us(1u);

		HAL_GPIO_WritePin(Main_IEE_CS_GPIO_Port, Main_IEE_CS_Pin, GPIO_PIN_RESET);
		delay_us(1u);
		(void)DRV_SPI_Transmit(&hspi2, &TxData2, 1u);
		(void)DRV_SPI_Transmit(&hspi2, u8_addr, 3u);

		for(uint16 loop = 0u; loop < len; loop++)
		{
			(void)DRV_SPI_Transmit(&hspi2, &u8Data[loop], 1u);
		}
		delay_us(1u);
		
		HAL_GPIO_WritePin(Main_IEE_CS_GPIO_Port, Main_IEE_CS_Pin, GPIO_PIN_SET);

		Result = DEF_True;
	}

	return Result;
}


/*---------------------------------------------------------------------*/


