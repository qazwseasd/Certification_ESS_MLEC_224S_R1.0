/*
 * Api_Calibdata.h
 *
 *  Created on: Apr 22, 2026
 *      Author: ETI
 */

/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/
#ifndef INC_API_CALIBDATA_H_
#define INC_API_CALIBDATA_H_


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
typedef struct {
	float 		LowGain_Plus;
	sint32		LowOffset_Plus;
	float 		LowGain_Minus;
	sint32		LowOffset_Minus;
	float 		HighGain_Plus;
	sint32 		HighOffset_Plus;
	float 		HighGain_Minus;
	sint32 		HighOffset_Minus;
}CURRENT_CAL ; 

typedef struct{
	float 		Pack1V_Gain;
	float		Pack1V_Offset;
	float 		Pack2V_Gain;
	float		Pack2V_Offset;	
	float		BusV_Gain;
	float		BusV_Offset;
	float		Ref5V_Gain;
	float		Ref5V_Offset;	
	CURRENT_CAL	CurrentSensor_1;
	CURRENT_CAL CurrentSensor_2;
	uint16      dummy;
    uint16		crc;
}CALIB_DATA;


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void Calib_Get_Calibdata(CALIB_DATA* CalibData);
extern void Calib_Set_Calibdata(CALIB_DATA* CalibData);
extern float Calib_Get_Ref5V_Gain(void);
extern float Calib_Get_Ref5V_Offset(void);



#endif /* INC_API_CALIBDATA_H_ */
