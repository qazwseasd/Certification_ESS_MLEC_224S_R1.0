/*
 * App_CurrLimit.h
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */

#ifndef INC_APP_CURRLIMIT_H_
#define INC_APP_CURRLIMIT_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define CURRLIMIT_CALCULATION_PERIOD	200u

#define CURRLIMIT_SOC_STOP_CHG			950u		/* [0.1 %] */


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void CurrLimit_Init(void);
extern void CurrLimit_Calculation_Task(void);
extern BOOL CurrLimit_Get_FullyCHG_Flag(void);
extern BOOL CurrLimit_Get_FullyDCH_Flag(void);
extern sint16 CurrLimit_Get_ChgCurrLimit(void);
extern sint16 CurrLimit_Get_DchCurrLimit(void);


#endif /* INC_APP_CURRLIMIT_H_ */
