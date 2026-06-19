/*
 * Drv_Relay.h
 *
 *  Created on: Sep 12, 2025
 *      Author: ETI
 */

#ifndef INC_DRV_RELAY_H_
#define INC_DRV_RELAY_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define RELAY_PRE_FLAG		0x1u    // 0 
#define RELAY_NEG_FLAG		0x2u    // 1
#define RELAY_POS_FLAG		0x4u    // 2

#define RELAY_ALL_OFF       0x0u


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/
typedef enum
{
	RELAY_OFF,
	RELAY_ON
}Relay_State;


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern uint8		Relay_Init(void);
extern void 	   	Relay_SetPosRelayHigh(Relay_State RS);
extern void 	   	Relay_SetPosRelayLow(Relay_State RS);
extern Relay_State 	Relay_GetPosRelay(void);
extern void 	   	Relay_SetNegRelayHigh(Relay_State RS);
extern void 	   	Relay_SetNegRelayLow(Relay_State RS);
extern Relay_State 	Relay_GetNegRelay(void);
extern void 	   	Relay_SetPreRelayHigh(Relay_State RS);
extern void 	   	Relay_SetPreRelayLow(Relay_State RS);
extern void 	   	Relay_SetAllRelayLow(Relay_State RS);
extern void        	Relay_SetAllRelayHigh(Relay_State RS);
extern uint8		Relay_GetAllRelay(void);
extern uint8 		Relay_GetMainRelayCommand(void);



#endif /* INC_DRV_RELAY_H_ */
