/*
 * Api_AuxVolt.c
 *
 *  Created on: Sep 16, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Api_AuxVolt.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define AUXV_RATIO	13.4353f

#define AUXVOLT_FILTED_COEF    0.7f


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/
static uint16 AuxVolt_ADRaw;
static float AuxVolt_FiltedValue;


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static uint16 V_Aux;

/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
uint16  AuxVolt_Measurement(void);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
 * @brief	Measure Aux Voltage
 * @param 	void
 * @return	uint16 - Aux voltage AD raw data
*/
uint16 AuxVolt_Measurement(void)
/*
	Call priodic : 100ms
	return value : voltage , factor : mV

	HW Scale : 6:1
*/
{
	float ad_raw;

	ad_raw = ((float)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_ADS7844_CH7) * AUXV_RATIO);
	
	return (uint16)ad_raw;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Aux Voltage Read TASK
 * @param 	void
 * @return	void
*/
void AuxVolt_Read_Task(void)
{
    static bool	AuxV_MeasStart = FALSE;


	AuxVolt_ADRaw = AuxVolt_Measurement();
	
	if(AuxV_MeasStart == FALSE)
	{
	    AuxV_MeasStart = TRUE;  
	    AuxVolt_FiltedValue = (float)AuxVolt_ADRaw;	      
	}
	AuxVolt_FiltedValue = FILTER(AuxVolt_FiltedValue, AuxVolt_ADRaw, AUXVOLT_FILTED_COEF);
	V_Aux = (uint16)AuxVolt_FiltedValue;;

}


/*---------------------------------------------------------------------*/
/**
 * @brief	Aux Voltage Calculation TASK
 * @param 	void
 * @return	void
*/
void AuxVolt_Calculation_Task(void)
{
	uint8  i;
	uint32 AuxV_sum = 0u;

	static bool	AuxV_MeasStart = FALSE;
	static uint16  AuxV_arr[DEF_NUMBER_OF_MOVING];
	static uint8   AuxV_cnt = 0u;
	
	if (AuxV_MeasStart == FALSE)
	{
		for(i = 0u; i < DEF_NUMBER_OF_MOVING; i++)
		{
			AuxV_arr[i] = AuxVolt_ADRaw;
		}
	}
	else
	{		
		AuxV_arr[AuxV_cnt] = AuxVolt_ADRaw;
		if (AuxV_cnt++ >= (DEF_NUMBER_OF_MOVING - 1u))
		{
			AuxV_cnt = 0u;
		}
	}

	for(i = 0u; i < DEF_NUMBER_OF_MOVING; i++)
	{	
		AuxV_sum += AuxV_arr[i];
	}
	V_Aux = (uint16)(AuxV_sum >> DEF_MOVING_AVERAGE_SHIFT);

	AuxV_MeasStart = TRUE;
}

/*---------------------------------------------------------------------*/
uint16 AuxVolt_Get_V_Aux(void)
{
    return V_Aux;
}



