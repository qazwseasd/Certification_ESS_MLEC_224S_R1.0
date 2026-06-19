/*
 * Drv_AdcEx.c
 *
 *  Created on: Sep 11, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_AdcEx.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define ADCEX_START			0x80u
#define ADCEX_SINGLE		0x04u
#define ADCEX_ALWAYS_PWR	0x03u

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
* @brief	Initialize External ADC
* @param	cs_port - Parameter for selecting the ADC chip to use
* @param	cs_pin - Parameter for selecting the ADC chip to use
* @return	BOOL, TRUE - Success, FALSE - X
*/
uint8 AdcEx_Init(GPIO_TypeDef* cs_port, uint16 cs_pin)
{
	uint8 TX_Buff = ADCEX_SINGLE | ADCEX_ALWAYS_PWR;
	
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
	delay_us(2u);

	(void)DRV_SPI_Transmit(&hspi2, &TX_Buff, 1u);
	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

	return DEF_True;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get Raw data of External ADC
* @param	cs_port - Parameter for selecting the ADC chip to use
* @param	cs_pin - Parameter for selecting the ADC chip to use
* @param	ch - Channel from which you want to read raw data
* @return	uint16 - ADC raw data value
*/
/*
uint16 AdcEx_GetRawData(GPIO_TypeDef* cs_port, uint16 cs_pin, uint8 ch)
{
	uint8 TX_Buff = ADCEX_START | ch | ADCEX_SINGLE | ADCEX_ALWAYS_PWR;
	uint8 buf[2];

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
	delay_us(2);

	DRV_SPI_Transmit(&hspi2, &TX_Buff, 1u);
	delay_us(2);

	DRV_SPI_Receive(&hspi2, buf, 2u);

	HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

	return( ((uint16)buf[0] << 5) | ((uint16)buf[1] >> 3) );
}
*/
uint16 AdcEx_GetRawData(GPIO_TypeDef* cs_port, uint16 cs_pin, uint8 ch)
{
    uint8_t tx[3];
	uint8_t rx[3];

    tx[0] = ADCEX_START | ch | ADCEX_SINGLE | ADCEX_ALWAYS_PWR;
	tx[1] = 0x00;
	tx[2] = 0x00;

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);

	delay_us(2u);
	
	(void)DRV_SPI_TransmitReceive(&hspi2, tx, rx, 3);

    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    
    return ( ((uint16_t)rx[1] << 5) | ((uint16_t)rx[2] >> 3) );
}


