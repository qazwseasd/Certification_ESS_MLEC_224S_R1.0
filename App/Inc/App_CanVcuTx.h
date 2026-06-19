/*
 * App_CanVcuTx.h
 *
 *  Created on: Aug 11, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/

#ifndef INC_APP_CANVCUTX_H_
#define INC_APP_CANVCUTX_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define CAN_TXTOMASTER_PERIOD 	10u

/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/

void	CanVcuTx_Init(void);
void	CanVcuTx_Task(void);

void	CanVcuTx_Set_ReportData1( uint8 *TxData );
void	CanVcuTx_Set_PackData1(uint8 *TxData);
void	CanVcuTx_Set_PackData2(uint8 *TxData);
void	CanVcuTx_Set_PackData3(uint8 *TxData);
void	CanVcuTx_Set_PackData4(uint8 *TxData);
void	CanVcuTx_Set_PackData5(uint8 *TxData);
void	CanVcuTx_Set_PackData6(uint8 *TxData);
void	CanVcuTx_Set_PackData7(uint8 *TxData);
void	CanVcuTx_Set_PackData8(uint8 *TxData);
void	CanVcuTx_Set_PackData9(uint8 *TxData);
void	CanVcuTx_Set_PackData10(uint8 *TxData);
void	CanVcuTx_Set_PackData11(uint8 *TxData);
void	CanVcuTx_Set_PackData12(uint8 *TxData);
void	CanVcuTx_Set_PackData13(uint8 *TxData);
void	CanVcuTx_Set_PackData14(uint8 *TxData);
void	CanVcuTx_Set_PackData15(uint8 *TxData);

void	CanVcuTx_Set_ModeChange(uint8 *TxData);
void	CanVcuTx_Set_RelayData(uint8 *TxData);

void	CanVcuTx_Set_ModuleData1(uint8 *TxData);
void	CanVcuTx_Set_ModuleData2(uint8 *TxData);
void	CanVcuTx_Set_ModuleData3(uint8 *TxData);
void	CanVcuTx_Set_ModuleData4(uint8 *TxData);
void	CanVcuTx_Set_ModuleData5(uint8 *TxData);
void	CanVcuTx_Set_ModuleData6(uint8 *TxData);
void	CanVcuTx_Set_ModuleData7(uint8 *TxData);
void	CanVcuTx_Set_ModuleData8(uint8 *TxData);
void	CanVcuTx_Set_ModuleData9(uint8 *TxData);

void	CanVcuTx_Set_FaultRelease(uint8 *TxData);
void	CanVcuTx_Set_ConfigrationData(uint8 *TxData);
void	CanVcuTx_Set_CalibrateData(uint8 *TxData);
void	CanVcuTx_Set_EventData1(uint8 *TxData);
void	CanVcuTx_Set_EventData2(uint8 *TxData);
void	CanVcuTx_Set_EventData3(uint8 *TxData);
void	CanVcuTx_Set_DetectFCCData(uint8 *TxData);


void	CanVcuTx_Set_Debug_En(uint8 msgIdx);
bool	CanVcuTx_Get_Debug_En(uint8 msgIdx);

void CanVcuTx_Send_Test_Message(void);



#endif /* INC_APP_CANVCUTX_H_ */
