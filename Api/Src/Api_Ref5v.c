/*
 * Api_Ref5v.c
 *
 *  Created on: Sep 16, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Api_Ref5v.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
/*
	(Vref * Rtotal) / (Rtarget * ADC resolution) * Place after decimal
	value
	= (5 * (100KOhm * 2 + 200KOhm)) / (200KOhm * 4096(12bit)) * 1000
*/
#define REF5V_RES_RATIO			1.221f

#define REF5V_SAMPLING_TIME		9u
#define REF5V_AVERAGE_SHIFT		3u

/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/
CLASSB static uint16 Ref5v_CalculatedValue;
CLASSB static uint8 Ref5V_Fault;


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
uint16  Ref5v_AD_Measurement(void);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/

/**
 * @brief	Reference 5V Initialization
 * @param 	void
 * @return	void
*/
bool Ref5v_Init(void)
{
    
	return TRUE;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Reference 5V Measurement
 * @param 	void
 * @return	uint16 - AD Raw data
*/
uint16 Ref5v_AD_Measurement(void)
{
	uint32 AdcRawSum = 0;
    uint8 i;

	for(i = 0; i < REF5V_SAMPLING_TIME; i++)
	{
		if(i > 0u)
		{
			AdcRawSum += AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_REF5V);		
		}
	}

	AdcRawSum = AdcRawSum >> REF5V_AVERAGE_SHIFT;
	AdcRawSum = AdcRawSum * 2u;
	
    return (uint16)(AdcRawSum);
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Reference 5V Calculation
 * @param 	void
 * @return	void
*/
void Ref5v_Calculation(void)
{
	float fdata;
    float Ref5vFilt = (float)Ref5v_AD_Measurement();   
	CLASSB static uint8 Count_5v;

	float Ref5V_Gain;
	float Ref5V_Offset;

	Ref5V_Gain = Calib_Get_Ref5V_Gain();
	Ref5V_Offset = Calib_Get_Ref5V_Offset();

	#ifdef Test_Ref5v_Calculation
	Ref5vFilt = 3800u;
	#endif
	
    fdata = (Ref5vFilt * (float)REF5V_RES_RATIO * Ref5V_Gain) + Ref5V_Offset;
    Ref5v_CalculatedValue = (uint16)fdata;

	if((Ref5v_CalculatedValue < 4750u) || (Ref5v_CalculatedValue > 5250u))
	{
		Count_5v++;
	}
	else
	{
		Count_5v = 0u;
	}

	if(Count_5v > 50u)
	{
		Count_5v = 51u;
		Ref5V_Fault = TRUE;
	}
	else
	{
		Ref5V_Fault = FALSE;
	}
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Reference 5V AD Raw data
 * @param 	void
 * @return	uint32 - AD Raw data
*/
uint16 Ref5v_GetADRaw(void)
{
	uint16 rv;
	float fdata;
	
	float Ref5V_Gain;


	Ref5V_Gain = Calib_Get_Ref5V_Gain();

	fdata = (float)Ref5v_AD_Measurement() * Ref5V_Gain;

	rv = (uint16)fdata;
		
	return rv;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Reference 5V calculation value
 * @param 	void
 * @return	uint16 - Ref 5V Calculation value
*/
uint16 Ref5V_GetCalculatedValue(void)
{
	return Ref5v_CalculatedValue;
}

/*---------------------------------------------------------------------*/
/**
 * @brief	Check Reference 5V fault
 * @param 	void
 * @return  bool - Ref5V_Fault check
*/
bool Ref5V_IsFault(void)
{
	return (BOOL)Ref5V_Fault;
}

/*---------------------------------------------------------------------*/

/* End of L2Api_Ref5v.c */

