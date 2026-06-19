/*
 * Drv_Can.h
 *
 *  Created on: Jul 2, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/

#ifndef INC_DRV_CAN_H_
#define INC_DRV_CAN_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/

#define Can_RxBuffSize		8u			/* [byte] */


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/

typedef struct
{
	uint32				MsgID;
	uint32				Rtr;
	uint8				DataSize;	/* [byte] */
	uint8				Data[Can_RxBuffSize];
}Can_RxData_t;	/* Data for Rx */

typedef void (*Can_ProcessMsg_Cb)(Can_RxData_t* RxData);


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/

void	Can_Init(CAN_HandleTypeDef *hcan);
void	Can_ProcessMsg_Cb_Register(CAN_HandleTypeDef *hcan, Can_ProcessMsg_Cb ProcessMsg_Cb);
bool	Can_Send_MsgFrame( CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8 *TxData);


#endif /* INC_DRV_CAN_H_ */
