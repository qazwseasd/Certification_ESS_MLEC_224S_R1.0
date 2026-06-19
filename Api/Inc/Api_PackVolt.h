/*
 * Api_PackVolt.h
 *
 *  Created on: Sep 16, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/
#ifndef INC_API_PACKVOLT_H_
#define INC_API_PACKVOLT_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Api_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define PACKVOLT_RESOLUTION	(float)0.1		/* [V] */

/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern bool PackVolt_Init(void);
extern void PackVolt_Read_Task(void);
extern uint16 PackVolt_Get_V_PackMeas(void);
extern void PackVolt_Set_V_PackMeas(uint16 PackV);
extern uint16 PackVolt_Get_V_BusMeas(void);
extern void PackVolt_Set_V_BusMeas(uint16 BusV);

#endif /* INC_API_PACKVOLT_H_ */
