/*
 * App_CellBalance.h
 *
 *  Created on: Sep 10, 2025
 *      Author: ETI
 */

#ifndef INC_APP_CELLBALANCE_H_
#define INC_APP_CELLBALANCE_H_


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
typedef struct
{
    uint16 	Bal_StartV;
    uint16 	Bal_StopV;
    uint16 	Bal_SafetyV;

    uint8	Bal_Seq;
    uint16	Bal_Cnt;
    uint8 	Bal_State;
    uint16 	Bal_Output[DEF_MAX_NUM_OF_BMIC];
    uint16 	Bal_Status[DEF_MAX_NUM_OF_BMIC];
	uint16  Bal_Count;
}CellBalance_Ctrl_t;


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void 	CellBalance_Init(void);
extern void 	CellBalance_Control_Task(void);
extern uint16  	CellBalance_Count_NumberOfBalancingCell(void);
extern uint16  	CellBalance_Get_NumberOfBalancingCell(void);
extern uint8 	CellBalance_Get_BalancingState(void);
extern void 	CellBalance_Set_ForcedOff(bool fbc);
extern uint16 	CellBalance_Get_BalStatus(uint16 Bmic_No);



#endif /* INC_APP_CELLBALANCE_H_ */
