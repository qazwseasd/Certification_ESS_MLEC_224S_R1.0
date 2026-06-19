/*
 * Drv_Spi.h
 *
 *  Created on: Sep 12, 2025
 *      Author: ETI
 */

#ifndef INC_DRV_SPI_H_
#define INC_DRV_SPI_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
HAL_StatusTypeDef DRV_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8 *pData, uint16 Size);
HAL_StatusTypeDef DRV_SPI_Receive(SPI_HandleTypeDef *hspi, uint8 *pData, uint16 Size);
HAL_StatusTypeDef DRV_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8 *pTxData, uint8 *pRxData, uint16 Size);


#endif /* INC_DRV_SPI_H_ */
