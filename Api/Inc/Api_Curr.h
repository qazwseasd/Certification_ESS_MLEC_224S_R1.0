/*
 * Api_Curr.h
 *
 *  Created on: Sep 19, 2025
 *      Author: ETI
 */

#ifndef INC_API_CURR_H_
#define INC_API_CURR_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Api_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern bool Curr_Init(void);
extern void Curr_Read_Task(void);
extern void Curr_Calculation_Task(void);
extern uint16  Curr_GetADRawLow(void);
extern uint16  Curr_GetADRawHigh(void);
extern uint16  Curr_GetADRawLow_Sub(void);
extern uint16  Curr_GetADRawHigh_Sub(void);
extern sint32  Curr_GetCalibValueLow(void);
extern sint32  Curr_GetCalibValueHigh(void);
extern sint32  Curr_GetCalibValueLow_Sub(void);
extern sint32  Curr_GetCalibValueHigh_Sub(void);
extern sint32  Curr_GetCurrUnfiltered(void);
extern sint32  Curr_GetCurrUnfiltered_Sub(void);
extern sint32  Curr_GetCurr1secSum(void);
extern uint16  Curr_GetCurr1secCnt(void);
extern void Curr_SetCurr1secSum(sint32 curr_1sec_sum);
extern void Curr_SetCurr1secCnt(uint16 curr_1sec_cnt);
extern bool Curr_IsSensorOffline(void);
extern sint16 Curr_GetChargeMax(void);
extern sint16 Curr_GetDischargeMax(void);
extern sint16 Curr_Get_I_PackMeas(void);
extern sint16 Curr_Get_I_PackMeas_Sub(void);



#endif /* INC_API_CURR_H_ */
