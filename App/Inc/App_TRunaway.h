/*
 * App_TRunaway.h
 *
 *  Created on: Oct 23, 2025
 *      Author: ETI
 */

#ifndef INC_APP_TRUNAWAY_H_
#define INC_APP_TRUNAWAY_H_

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


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/
extern bool	DetectTR_Condition_1;	/* (TCellmax - Taverage) >= 35℃ */
extern bool	DetectTR_Condition_2;	/* (TCellMax) ≥ 60 [℃] */
extern bool	DetectTR_Condition_3;	/* (TCellRiseRate) > 1 [℃/s], lasting 3s */
extern bool	DetectTR_Condition_4;	/* (VCellDropMax) > 1.213 [V] in 5s */
extern bool	DetectTR_Condition_5;	/* (RIso) ≤ (100 [Ω/V] + margin 50kΩ */
extern bool	DetectTR_Condition_6;	/* (VCellmin) <= 1.7V */
extern bool	DetectTR_Condition_7;	/* (VCellSensing OFF) */
extern bool	DetectTR_Condition_8;	/* (TCellSensing OFF) */
extern bool	DetectTR_Condition_9;	/* (BMIC ERROR) */
extern bool	DetectTR_ThermalRunaway;


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern	void	DetectTR_Task(void);
extern uint8 TRunaway_Get_Trunaway_V_BMICNum(void);
extern uint8 TRunaway_Get_Trunaway_V_CellNum(void);
extern uint8 TRunaway_Get_Trunaway_T_BMICNum(void);
extern uint8 TRunaway_Get_Trunaway_T_CellNum(void);


#endif /* INC_APP_TRUNAWAY_H_ */
