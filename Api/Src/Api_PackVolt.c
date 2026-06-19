/*
 * Api_PackVolt.c
 *
 *  Created on: Sep 16, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Api_PackVolt.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
/*
	(Vref * Rtotal) / (Rtarget * ADC resolution) * Place after decimal
	value
	= (5 * (1.5MOhm * 10 + 20KOhm)) / (20KOhm * (4096(12bit) - 1)) * 10
*/
#define PACKVOLT_RES_RATIO		(float)9.16971917

#define PACKVOLT_SAMPLING_TIME	9u
#define PACKVOLT_AVERAGE_SHIFT	3u

#define PACKVOLT_FILTED_COEF    (float)0.7

/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/
CLASSB static uint32 PackVolt_CalculatedValue;
CLASSB static uint32 PackVolt_FiltedValue;

CLASSB static uint32 BusVolt_CalculatedValue;
CLASSB static uint32 BusVolt_FiltedValue;

CLASSB static uint8  PackVolt_MeasSeq;

CLASSB static CALIB_DATA 	Pack_CalibData;

CLASSB static uint16	V_PackMeas;
CLASSB static uint16	V_BusMeas;



/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
uint32  PackVolt_Measurement(void);
uint32  BusVolt_Measurement(void);

/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/

/**
 * @brief	Pack Voltage Initialization
 * @param 	void
 * @return	BOOL, TRUE - Success
*/
bool PackVolt_Init(void)
{
	Dio_SetFET_ForPackVolt(OFF);
	Dio_SetFET_ForBusVolt(OFF);
	PackVolt_MeasSeq = 0;
	return (BOOL)TRUE;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Pack Voltage Measurement
 * @param 	void
 * @return	uint32 - Pack Voltage AD raw data
*/
uint32 PackVolt_Measurement(void)
{
	uint32 ad_raw = 0u;
	uint8  i;

	for (i = 0u; i < PACKVOLT_SAMPLING_TIME; i++)
	{
		if(i > 0u)
		{
			#ifdef Test_PackVolt_Measurement
			ad_raw += 1100u;
			#else
			ad_raw += AdcEx_GetRawData(Main_ADC1_CS_GPIO_Port, Main_ADC1_CS_Pin, ADCEX_CH_PACKVOLT);
			#endif
		}
	}

    ad_raw = ad_raw >> PACKVOLT_AVERAGE_SHIFT;

    return ad_raw;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Bus Voltage measurement
 * @param 	void
 * @return	uint32, Bus voltage AD raw data
*/
uint32 BusVolt_Measurement(void)
{
	uint32 ad_raw = 0u;
	uint8  i;

	for (i = 0u; i < PACKVOLT_SAMPLING_TIME; i++)
	{
		if(i > 0u)
		{
			#ifdef Test_BusVolt_Measurement
			ad_raw += 1100u;
			#else
			ad_raw += AdcEx_GetRawData(Main_ADC1_CS_GPIO_Port, Main_ADC1_CS_Pin, ADCEX_CH_BUSVOLT);
			#endif
		}
	}

    ad_raw = ad_raw >> PACKVOLT_AVERAGE_SHIFT;

    return ad_raw;
}

/**
 * @brief	Pack Voltage Read Task
 * @param 	void
 * @return	void
*/
void PackVolt_Read_Task(void)
{
    CLASSB static bool PackV_MeasStart;
	CLASSB static bool BusV_MeasStart;
	float PackVolt;
	float BusVolt;

	Calib_Get_Calibdata(&Pack_CalibData);

	#ifdef Test_Pack1V_Gain
	Pack_CalibData.Pack1V_Gain = -1.f;
	#endif

	#ifdef Test_BusV_Gain
	Pack_CalibData.BusV_Gain = -1.f;
	#endif

	#ifdef Test_PackVolt_MeasSeq
	PackVolt_MeasSeq = 99u;
	#endif

	switch (PackVolt_MeasSeq)
	{
		case 0:
			Dio_SetFET_ForPackVolt(ON);
			PackVolt_MeasSeq++;
			break;

		case 1:
			PackVolt = (float)PackVolt_Measurement() * PACKVOLT_RES_RATIO;
			
			PackVolt_CalculatedValue = (uint32)PackVolt;
			Dio_SetFET_ForPackVolt(OFF);

            if(PackV_MeasStart == FALSE)
            {
                PackV_MeasStart = TRUE;
                PackVolt_FiltedValue = PackVolt_CalculatedValue;
            }
            PackVolt = FILTER(PackVolt_FiltedValue, PackVolt_CalculatedValue, PACKVOLT_FILTED_COEF);
			PackVolt_FiltedValue = (uint32)PackVolt;

			#ifdef TestMode
				V_PackMeas = Adbms_Get_V_CellSum();
			#else
				if(((float)PackVolt_FiltedValue * Pack_CalibData.Pack1V_Gain) <= -1.0f*Pack_CalibData.Pack1V_Offset)
				{
					V_PackMeas = 0;
				}
				else
				{
					PackVolt = ((float)PackVolt_FiltedValue * Pack_CalibData.Pack1V_Gain) + Pack_CalibData.Pack1V_Offset;
					V_PackMeas = (uint16)PackVolt;
				}
			#endif

			#ifdef Isolation_Test
				if(((float)PackVolt_FiltedValue * Pack_CalibData.Pack1V_Gain) <= -1.0f*Pack_CalibData.Pack1V_Offset)
				{
					V_PackMeas = 0;
				}
				else
				{
					PackVolt = ((float)PackVolt_FiltedValue * Pack_CalibData.Pack1V_Gain) + Pack_CalibData.Pack1V_Offset;
					V_PackMeas = (uint16)PackVolt;
				}
			#endif

			#ifdef Dead_Fuse
			if(((float)PackVolt_FiltedValue * Pack_CalibData.Pack1V_Gain) <= -1.0f*Pack_CalibData.Pack1V_Offset)
				{
					V_PackMeas = 0;
				}
				else
				{
					PackVolt = ((float)PackVolt_FiltedValue * Pack_CalibData.Pack1V_Gain) + Pack_CalibData.Pack1V_Offset;
					V_PackMeas = (uint16)PackVolt;
				}
			#endif
			
			PackVolt_MeasSeq++;
			break;

		case 2: 
			Dio_SetFET_ForBusVolt(ON);
			PackVolt_MeasSeq++;
			break;

		case 3:
   			BusVolt = (float)BusVolt_Measurement() * PACKVOLT_RES_RATIO;
			
			BusVolt_CalculatedValue = (uint32)BusVolt;
			Dio_SetFET_ForBusVolt(OFF);

            if(BusV_MeasStart == FALSE)
            {
                BusV_MeasStart = TRUE;
                BusVolt_FiltedValue = BusVolt_CalculatedValue;
            }
			BusVolt = FILTER(BusVolt_FiltedValue, BusVolt_CalculatedValue, PACKVOLT_FILTED_COEF);
			BusVolt_FiltedValue = (uint32)BusVolt;

			#ifdef TestMode
				V_BusMeas = Adbms_Get_V_CellSum();
			#else
			if(((float)BusVolt_FiltedValue * Pack_CalibData.BusV_Gain) <= -1.0f*Pack_CalibData.BusV_Offset)
			{
				V_BusMeas = 0;
			}
			else
			{
				BusVolt = ((float)BusVolt_FiltedValue * Pack_CalibData.BusV_Gain) + Pack_CalibData.BusV_Offset;
				V_BusMeas = (uint16)BusVolt;
			}
			#endif
				
            PackVolt_MeasSeq = 0u;
			break;

		default :
			Dio_SetFET_ForPackVolt(OFF);
			Dio_SetFET_ForBusVolt(OFF);
			PackVolt_MeasSeq = 0u;
			break;
	}
}


/*---------------------------------------------------------------------*/
uint16 PackVolt_Get_V_PackMeas(void)
{
    return V_PackMeas;
}


/*---------------------------------------------------------------------*/
void PackVolt_Set_V_PackMeas(uint16 PackV)
{
    V_PackMeas = PackV;
}


/*---------------------------------------------------------------------*/
uint16 PackVolt_Get_V_BusMeas(void)
{
    return V_BusMeas;
}


/*---------------------------------------------------------------------*/
void PackVolt_Set_V_BusMeas(uint16 BusV)
{
    V_BusMeas = BusV;
}



/* End of Api_PackVolt.c */

