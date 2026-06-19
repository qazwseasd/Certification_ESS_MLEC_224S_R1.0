/*
 * Drv_Can.c
 *
 *  Created on: Jul 2, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Can.h"


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

static	Can_RxData_t		Can_Fifo0RxData;
static	Can_RxData_t		Can_Fifo1RxData;

static	Can_ProcessMsg_Cb	Can_Fifo0ProcessMsg_Cb;
static	Can_ProcessMsg_Cb	Can_Fifo1ProcessMsg_Cb;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/

uint8	Can_Filter_Config(	CAN_HandleTypeDef *hcan, uint8 Filter_Idx,
							uint32 Filter, uint32 Filter_Mask);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/*
void	Can_Init(CAN_HandleTypeDef *hcan)
{
	uint8	Result_FilterConfig = 0u;
	uint8	Can1filterIdx		= 0u;
	uint8	Can2filterIdx		= DRV_Can_2_Filter_StartBank;

	if 		(hcan->Instance == CAN1)
	{
		Result_FilterConfig |= Can_Filter_Config(hcan, Can1filterIdx, DRV_Can_1_Filter1, DRV_Can_1_Mask1);
		Can1filterIdx++;
		Result_FilterConfig |= Can_Filter_Config(hcan, Can1filterIdx, DRV_Can_2_Filter1, DRV_Can_2_Mask1);

		Result_FilterConfig |= (uint8)HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	}
	else if (hcan->Instance == CAN2)
	{
		Result_FilterConfig |= Can_Filter_Config(hcan, Can2filterIdx, DRV_Can_2_Filter1, DRV_Can_2_Mask1);

		Result_FilterConfig |= (uint8)HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
	}
	else{}

	if (Result_FilterConfig == DEF_Error)
	{

	}
	else{}

	if (HAL_CAN_Start(hcan) != HAL_OK)
	{

	}
	else{}
}
*/
void Can_Init(CAN_HandleTypeDef *hcan)
{
    uint8 Result_FilterConfig = 0u;
    uint8 Can1filterIdx       = 0u;
    uint8 Can2filterIdx       = DRV_Can_2_Filter_StartBank;

    CAN_RxHeaderTypeDef RxHeader;
    uint8 RxData[8];
    uint32 lastMsgTick;
	uint32 currentTick;
	uint32 fillLevel0;
	uint32 fillLevel1;

    if (hcan->Instance == CAN1)
    {
        Result_FilterConfig |= Can_Filter_Config(hcan, Can1filterIdx, DRV_Can_1_Filter1, DRV_Can_1_Mask1);
        Can1filterIdx++;
        Result_FilterConfig |= Can_Filter_Config(hcan, Can1filterIdx, DRV_Can_2_Filter1, DRV_Can_2_Mask1);
    }
    else if (hcan->Instance == CAN2)
    {
        Result_FilterConfig |= Can_Filter_Config(hcan, Can2filterIdx, DRV_Can_2_Filter1, DRV_Can_2_Mask1);
        
    }
    else {}

    if (Result_FilterConfig == DEF_Error)
    {

    }


    if (HAL_CAN_Start(hcan) != HAL_OK)
    {

    }

    lastMsgTick = HAL_GetTick();

	Led_On(DRV_Led_Green);
	Led_On(DRV_Led_Red);

    for (;;)
    {
        currentTick = HAL_GetTick();

		fillLevel0 = HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0);
		fillLevel1 = HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO1);

         (void)HAL_IWDG_Refresh(&hiwdg); 

        if ((fillLevel0 > 0u) || (fillLevel1 > 0u))
        {
            uint32 fifo;
			
			if (fillLevel0 > 0u)
		    {
		        fifo = CAN_RX_FIFO0;
		    }
		    else
		    {
		        fifo = CAN_RX_FIFO1;
		    }
			
            (void)HAL_CAN_GetRxMessage(hcan, fifo, &RxHeader, RxData);
            
            lastMsgTick = currentTick;
        }
        else
        {
            if ((currentTick - lastMsgTick) > 20u)
            {
                break;
            }
        }
		
    }

	Led_Off(DRV_Led_Green);
	Led_Off(DRV_Led_Red);

    if (hcan->Instance == CAN1)
    {
		__HAL_CAN_DISABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_FULL | CAN_IT_RX_FIFO0_OVERRUN);
        (void)HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    }
    else if (hcan->Instance == CAN2)
    {
		__HAL_CAN_DISABLE_IT(hcan, CAN_IT_RX_FIFO1_MSG_PENDING | CAN_IT_RX_FIFO1_FULL | CAN_IT_RX_FIFO1_OVERRUN);
        (void)HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
    }
	else
	{
	}
}



void	Can_ProcessMsg_Cb_Register(CAN_HandleTypeDef *hcan, Can_ProcessMsg_Cb ProcessMsg_Cb)
{
	if 		(hcan->Instance == CAN1)
	{
		Can_Fifo0ProcessMsg_Cb = ProcessMsg_Cb;
	}
	else if (hcan->Instance == CAN2)
	{
		Can_Fifo1ProcessMsg_Cb = ProcessMsg_Cb;
	}
	else{}
}


uint8	Can_Filter_Config(CAN_HandleTypeDef *hcan, uint8 Filter_Idx, uint32 Filter, uint32 Filter_Mask)
{
	CAN_FilterTypeDef 	CanFilter;

#if (DRV_Can_1_IdType == CAN_ID_EXT)
	uint32	filterId	= Filter		<< 3u;
	uint32	filterMask	= Filter_Mask	<< 3u;
#else
	uint32	filterId	= Filter		<< 21u;
	uint32	filterMask	= Filter_Mask	<< 21u;
#endif

    CanFilter.FilterIdHigh          = (filterId & 0xFFFF0000u) >> 16u;
    CanFilter.FilterIdLow           =  filterId & 0x0000FFF8u;
    CanFilter.FilterMaskIdHigh      = (filterMask & 0xFFFF0000u) >> 16u;
    CanFilter.FilterMaskIdLow       =  filterMask & 0x0000FFF8u;


	CanFilter.FilterBank 			= Filter_Idx;
	CanFilter.FilterMode 			= CAN_FILTERMODE_IDMASK;
	CanFilter.FilterScale 			= CAN_FILTERSCALE_32BIT;
	CanFilter.FilterActivation 		= CAN_FILTER_ENABLE;
	CanFilter.SlaveStartFilterBank 	= DRV_Can_2_Filter_StartBank;

	if 		(hcan->Instance == CAN1)
	{
		CanFilter.FilterFIFOAssignment 	= CAN_FILTER_FIFO0;
	}
	else if (hcan->Instance == CAN2)
	{
		CanFilter.FilterFIFOAssignment 	= CAN_FILTER_FIFO1;
	}
	else{}

	if (HAL_CAN_ConfigFilter(hcan, &CanFilter) != HAL_OK)
	{
		return DEF_Error;
	}
	else{}

	return DEF_Ok;
}


bool	Can_Send_MsgFrame( CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *header, uint8 *TxData)
{
	if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0u)
	{
		return DEF_False;
	}

	uint32	TxMailBox = 0u;
	uint8	sendData[100u];

	(void)memcpy(sendData, TxData, header->DLC);

	if ( HAL_CAN_AddTxMessage(hcan, header, sendData, &TxMailBox ) == HAL_OK)
	{
		#ifdef Test_CanVcuTx_Task
		return DEF_False;
		#else
		return DEF_True;
		#endif
	}
	else
	{
		return DEF_False;
	}
}



void 	HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef	RxHeader;

	(void)HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Can_Fifo0RxData.Data);

	if 		(RxHeader.IDE == CAN_ID_STD)
	{
		Can_Fifo0RxData.MsgID = RxHeader.StdId;
	}
	else if	(RxHeader.IDE == CAN_ID_EXT)
	{
		Can_Fifo0RxData.MsgID = RxHeader.ExtId;
	}
	else{}

	Can_Fifo0RxData.Rtr = RxHeader.RTR;

	if (Can_Fifo0ProcessMsg_Cb != NULL)
	{
		Can_Fifo0ProcessMsg_Cb(&Can_Fifo0RxData);
	}
	else{}
}



void 	HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef	RxHeader;

	(void)HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, Can_Fifo1RxData.Data);

	if 		(RxHeader.IDE == CAN_ID_STD)
	{
		Can_Fifo1RxData.MsgID = RxHeader.StdId;
	}
	else if	(RxHeader.IDE == CAN_ID_EXT)
	{
		Can_Fifo1RxData.MsgID = RxHeader.ExtId;
	}
	else{}

	Can_Fifo1RxData.Rtr = RxHeader.RTR;

	if (Can_Fifo1ProcessMsg_Cb != NULL)
	{
		Can_Fifo1ProcessMsg_Cb(&Can_Fifo1RxData);
	}
	else{}
}


