/*
 * App_CanUiRx.h
 *
 *  Created on: Mar 19, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/

#ifndef INC_APP_CANUIRX_H_
#define INC_APP_CANUIRX_H_

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_Def.h"

/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/

#define CanUiRx_Cmd_Type_Led	1u			/* LED Control	*/

#define CanUiRx_Cmd_Led_Off		0u
#define CanUiRx_Cmd_Led_On		1u

/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/
// extern PARAMETER Can_temp_Parameter;

extern uint8		Config_Data[8];
extern uint8		Calibrate_Data[8u];
extern uint8		Event_Data[8u];
extern uint8		DetectFCC_Data[8u];

extern uint16  	 CAN_BmicNo;

extern uint16     Can_ConfigSave_DownCount;


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/

void 		CanUiRx_Init(void);
void 		CanUiRx_Callback(Can_RxData_t *RxData);
uint32		CanVcuRx_Get_OfflineCnt(void);
extern void	Can_Process_Task(void);
extern bool Can_GetContactorCmd(void);
extern BOOL Get_IsSaveConfigData(void);
extern void Set_SaveConfigData(BOOL SetV);
// extern void tempParameterUpdate(void);
extern void Can_CountingTimeOut(void);
extern uint8 CanUiRx_Get_BalanceTest(void);



/* Modif By SKAHN _2500903_SAT */
//Mbms_ModeCmd_t	CanUiRx_Get_CmdMbmsMode(void);
/* Modif By SKAHN _250903_END */
#endif /* INC_APP_CANUIRX_H_ */
