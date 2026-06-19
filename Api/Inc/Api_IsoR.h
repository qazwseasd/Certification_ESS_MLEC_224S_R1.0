/*
 * Api_IsoR.h
 *
 *  Created on: Sep 19, 2025
 *      Author: ETI
 */

#ifndef INC_API_ISOR_H_
#define INC_API_ISOR_H_


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
typedef enum
{
    ISO_NoOperation = 0,
    ISO_Notyet_Process,
    ISO_In_Process,
    ISO_Done,
    ISO_Fail,
    ISO_Meas_StartWait,
    ISO_EndWait,
} IsoR_MeasStatus;


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern bool 			IsoR_Init(void);
extern void 			IsoR_Measurement_Task(void);
extern void 			IsoR_SetMeasCommand(bool cmd);
extern IsoR_MeasStatus  IsoR_Get_MeasOperationStatus(void);
extern const uint16		IsoR_LookupTbl[29u];
extern const uint16   		IsoPV_Lookup[15u];
extern const uint16 		IsoR_RawPN_Table[15u][29u];
extern bool 			IsoR_IsMeasurement(void);
extern uint16 IsoR_Get_ISO_Resistor(void);
extern void IsoR_Set_ISO_Resistor(uint16 IsoR);


#endif /* INC_API_ISOR_H_ */
