/*
 * App_Relay.h
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */

#ifndef INC_APP_RELAY_H_
#define INC_APP_RELAY_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/
typedef enum
{
	RC_READY,
	RC_ON,
	RC_OFF,
	RC_CHECK,
	RC_TEST
}Relay_CtrlCmd;


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void Relay_App_Init(void);
extern void Relay_Control_Task(void);
extern void Relay_SetControlCommand(Relay_CtrlCmd cmd);
extern Relay_CtrlCmd Relay_GetControlCommand(void);
extern void Relay_SetTestCommand(uint8 cmd);
extern void Relay_Set_CriticalOpen(BOOL cmd);
extern RELAY_STATUS Relay_Get_Relay_Status(void);
extern void Relay_Set_Relay_Status(RELAY_STATUS status);
extern uint8 Relay_Get_Relay_OP_ErrCode(void);
extern uint8 Relay_Get_Relay_Input_V(void);

#endif /* INC_APP_RELAY_H_ */
