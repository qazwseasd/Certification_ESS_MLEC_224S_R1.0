/*
 * Api_Curr.c
 *
 *  Created on: Sep 19, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Api_Curr.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define CURR_SAMP_TIME          17u
#define CURR_AVG_SHIFT			4u
#define CURR_FILTER_COEF        (float)0.75 //(float)0.5 //(float)0.25

#define CURR_LOW_MEAS_NEG_RANGE	380u		// low channel negative measurement range
#define CURR_LOW_MEAS_POS_RANGE	3715u		// low channel positive measurement range
#define CURR_NO_PROCESS_RANGE	100

#define CURR_LOW_SENSITIVITY	0.0667		
#define CURR_HIGH_SENSITIVITY	0.0057

/* for debug */
//	#define NoCheck_CURR_SensorOFFLine 

/* Sub Current Sensor On */
 #define Sub_Curr_Sensor 


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
CLASSB static uint16  Curr_ADRawLow;
CLASSB static uint16  Curr_ADRawHigh;
CLASSB static uint16  Curr_ADRawLow_Sub;
CLASSB static uint16  Curr_ADRawHigh_Sub;
CLASSB static sint32  Curr_CalibValueLow;
CLASSB static sint32  Curr_CalibValueHigh;
CLASSB static sint32  Curr_CalibValueLow_Sub;
CLASSB static sint32  Curr_CalibValueHigh_Sub;
CLASSB static sint32  Curr_ADRawUnfiltered;
CLASSB static sint32  Curr_ADRawUnfiltered_Sub;
CLASSB static uint8	Curr_Count1;
CLASSB static uint8	Curr_Count2;
CLASSB static sint32  Curr_1secSum; 			// saved capacity for 1 second
CLASSB static uint16  Curr_1secCount;			// saved capacity count number for 1 second
CLASSB static uint8   Curr_SensorOffline;

CLASSB static sint16  CurrChgMax;
CLASSB static sint16  CurrDchMax;

CLASSB static CALIB_DATA 	Curr_CalibData;

CLASSB static sint16  I_PackMeas;
CLASSB static sint16  I_PackMeas_Sub;




/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/

sint32  Curr_Measurement(uint16 ref5v);
sint32  Curr_Measurement_Sub(uint16 ref5v);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
 * @brief	Current Initialization
 * @param 	void
 * @return	BOOL, TRUE - Success, FALSE - X
*/
bool Curr_Init(void)
{
	#ifdef Test_Curr_Init
	CurrChgMax = 900;
	CurrDchMax = 900;
	#endif

    CurrChgMax = (sint16)0;
    CurrDchMax = (sint16)0;
    
	return (BOOL)TRUE;
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/**
 * @brief	Current Measurement
 * @param 	ref5v - reference 5v value
 * @return	sint32  - Current AD raw data
*/
sint32 Curr_Measurement(uint16 ref5v)
{
	sint32 curr_calibrated = 0;	
	uint32 low_ch_value = 0u;
	uint32 high_ch_value = 0u;
	uint8 	i;
	float fdata;
	sint32 sdata;
	uint16 udata;

//	Curr_SensorOffline = FALSE;
	if(ManaMsm_Get_System_State() == SYS_CALIBRATION)
	{
		for (i = 0u; i < 12u; i++)
		{
			if ((i >= 2u) && (i <= 9u))
			{
				low_ch_value  += (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_LOW);
				high_ch_value += (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_HIGH);
			}				
		}				

		Curr_ADRawLow  = (uint16)(low_ch_value  >> 3u);
		Curr_ADRawHigh = (uint16)(high_ch_value >> 3u);

		/* (+) current */
		if(Curr_ADRawLow >= (ref5v >> 1u))
		{
			udata = ref5v >> 1u;
			sdata = ((sint32)Curr_ADRawLow - (sint32)udata) * 5000 / (sint32)ref5v;
			fdata = (float)sdata  / (float)CURR_LOW_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_1.LowGain_Plus;
			Curr_CalibValueLow = (sint32)fdata + Curr_CalibData.CurrentSensor_1.LowOffset_Plus;
		}
		/* (-) current */
		else
		{
			udata = ref5v >> 1u;
			sdata = ((sint32)Curr_ADRawLow - (sint32)udata) * 5000 / (sint32)ref5v;
			fdata = (float)sdata / (float)CURR_LOW_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_1.LowGain_Minus;
			Curr_CalibValueLow = (sint32)fdata + Curr_CalibData.CurrentSensor_1.LowOffset_Minus;
		}
		/* (+) current */
		if(Curr_ADRawHigh >= (ref5v >> 1u))
		{
			udata = ref5v >> 1u;
			sdata = ((sint32)Curr_ADRawHigh - (sint32)udata) * 5000 / (sint32)ref5v;
			fdata = (float)sdata / (float)CURR_HIGH_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_1.HighGain_Plus;
			Curr_CalibValueHigh = (sint32)fdata + Curr_CalibData.CurrentSensor_1.HighOffset_Plus;
		}
		/* (-) current */
		else
		{
			udata = ref5v >> 1u;
			sdata = ((sint32)Curr_ADRawHigh - (sint32)udata) * 5000 / (sint32)ref5v;
			fdata = (float)sdata / (float)CURR_HIGH_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_1.HighGain_Minus;
			Curr_CalibValueHigh = (sint32)fdata + Curr_CalibData.CurrentSensor_1.HighOffset_Minus;
		}			
	}
	else
	{
		#ifdef Test_Curr_Measurement_1
		low_ch_value = 80u;
		high_ch_value = 80u;
		#elif defined(Test_Curr_Measurement_low)
		ref5v = 4000u;
		low_ch_value = 3601u;
		high_ch_value = 2137u;
		#elif defined(Test_Curr_Measurement_high)
		ref5v = 4000u;
		low_ch_value = 3999u;
		high_ch_value = 2411u;
		#elif defined(Test_Curr_Measurement_zero)
		ref5v = 4000u;
		low_ch_value = 2003u;
		high_ch_value = 2000u;
		#else
		low_ch_value = (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_LOW);
		high_ch_value = (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_HIGH);
		#endif

		if ((low_ch_value < 82u) || (high_ch_value < 82u))
		{
			Curr_SensorOffline = TRUE;
			curr_calibrated = 0;
		}

		/* Low channel of current sensor is normal */
		else if((low_ch_value >= CURR_LOW_MEAS_NEG_RANGE) && (low_ch_value <= CURR_LOW_MEAS_POS_RANGE))		//using low channel when within +- 25A 
		{
			low_ch_value = 0u;
			for (i = 0u; i < CURR_SAMP_TIME; i++)
			{
				if (i >= 1u)
				{
					#ifdef Test_Curr_Measurement_low
					low_ch_value += 3601u;
					#elif defined(Test_Curr_Measurement_zero)
					low_ch_value += 2003u;
					#else
					low_ch_value += (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_LOW);
					#endif
				}			
			}
			low_ch_value = low_ch_value >> CURR_AVG_SHIFT;

			if(low_ch_value >= ((uint32)ref5v >> 1u))
			{
				udata = ref5v >> 1u;
				sdata = ((sint32)low_ch_value - (sint32)udata) * 5000 / (sint32)ref5v;
				fdata = (float)sdata / (float)CURR_LOW_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_1.LowGain_Plus;
				curr_calibrated = (sint32)fdata + Curr_CalibData.CurrentSensor_1.LowOffset_Plus;
			}
			else
			{
				udata = ref5v >> 1u;
				sdata = ((sint32)low_ch_value - (sint32)udata) * 5000 / (sint32)ref5v;
				fdata = (float)sdata / (float)CURR_LOW_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_1.LowGain_Minus;
				curr_calibrated = (sint32)fdata + Curr_CalibData.CurrentSensor_1.LowOffset_Minus;
			}
		}

		else
		{
			high_ch_value = 0u;
			for (i = 0u; i < CURR_SAMP_TIME; i++)
			{
				if (i >= 1u)
				{
					#ifdef Test_Curr_Measurement_high
					high_ch_value += 2411u;
					#else
					high_ch_value += (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_HIGH);	
					#endif
				}
			}
			high_ch_value = high_ch_value >> CURR_AVG_SHIFT;							

			/* (+) current */
			if(high_ch_value >= ((uint32)ref5v >> 1u))
			{
				udata = ref5v >> 1u;
				sdata = ((sint32)high_ch_value - (sint32)udata) * 5000 / (sint32)ref5v;
				fdata = (float)sdata / (float)CURR_HIGH_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_1.HighGain_Plus;
				curr_calibrated = (sint32)fdata + Curr_CalibData.CurrentSensor_1.HighOffset_Plus;
			}
			/* (-) current */
			else
			{
				udata = ref5v >> 1u;
				sdata = ((sint32)high_ch_value - (sint32)udata) * 5000 / (sint32)ref5v;
				fdata = (float)sdata / (float)CURR_HIGH_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_1.HighGain_Minus;
				curr_calibrated = (sint32)fdata + Curr_CalibData.CurrentSensor_1.HighOffset_Minus;
			}				
		}		

		if((curr_calibrated <= CURR_NO_PROCESS_RANGE) && (curr_calibrated >= (CURR_NO_PROCESS_RANGE * (-1))))
		{
			curr_calibrated = 0;
		}
	}		

	return curr_calibrated; 
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Sub Current Measurement
 * @param 	ref5v - reference 5v value
 * @return	sint32  - Sub Current AD raw data
*/
sint32 Curr_Measurement_Sub(uint16 ref5v)
{
	sint32 curr_calibrated = 0;	
	uint32 low_ch_value = 0u;
	uint32 high_ch_value = 0u;
	uint8 	i;
	float fdata;
	sint32 sdata;
	uint16 udata;

//	Curr_SensorOffline = FALSE;
	if(ManaMsm_Get_System_State() == SYS_CALIBRATION)
	{
		for (i = 0u; i < 12u; i++)
		{
			if ((i >= 2u) && (i <= 9u))
			{
				low_ch_value  += (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_LOW_Sub);
				high_ch_value += (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_HIGH_Sub);
			}				
		}				

		Curr_ADRawLow_Sub  = (uint16)(low_ch_value  >> 3u);
		Curr_ADRawHigh_Sub = (uint16)(high_ch_value >> 3u);

		/* (+) current */
		if(Curr_ADRawLow_Sub >= ((uint32)ref5v >> 1u))
		{
			udata = ref5v >> 1u;
			sdata = ((sint32)Curr_ADRawLow_Sub - (sint32)udata) * 5000 / (sint32)ref5v;
			fdata = (float)sdata  / (float)CURR_LOW_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_2.LowGain_Plus;
			Curr_CalibValueLow_Sub = (sint32)fdata + Curr_CalibData.CurrentSensor_2.LowOffset_Plus;
		}
		/* (-) current */
		else
		{
			udata = ref5v >> 1u;
			sdata = ((sint32)Curr_ADRawLow_Sub - (sint32)udata) * 5000 / (sint32)ref5v;
			fdata = (float)sdata / (float)CURR_LOW_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_2.LowGain_Minus;
			Curr_CalibValueLow_Sub = (sint32)fdata + Curr_CalibData.CurrentSensor_2.LowOffset_Minus;
		}
		/* (+) current */
		if(Curr_ADRawHigh_Sub >= (ref5v >> 1u))
		{
			udata = ref5v >> 1u;
			sdata = ((sint32)Curr_ADRawHigh_Sub - (sint32)udata) * 5000 / (sint32)ref5v;
			fdata = (float)sdata / (float)CURR_HIGH_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_2.HighGain_Plus;
			Curr_CalibValueHigh_Sub = (sint32)fdata + Curr_CalibData.CurrentSensor_2.HighOffset_Plus;
		}
		/* (-) current */
		else
		{
			udata = ref5v >> 1u;
			sdata = ((sint32)Curr_ADRawHigh_Sub - (sint32)udata) * 5000 / (sint32)ref5v;
			fdata = (float)sdata / (float)CURR_HIGH_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_2.HighGain_Minus;
			Curr_CalibValueHigh_Sub = (sint32)fdata + Curr_CalibData.CurrentSensor_2.HighOffset_Minus;
		}			
	}
	else
	{
		#ifdef Test_Curr_Measurement_low
		ref5v = 4000u;
		low_ch_value = 3601u;
		high_ch_value = 2137u;
		#else
		low_ch_value = (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_LOW_Sub);
		high_ch_value = (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_HIGH_Sub);
		#endif

		if ((low_ch_value < 82u) || (high_ch_value < 82u))
		{
			Curr_SensorOffline = TRUE;
			curr_calibrated = 0;
		}
		
		/* Low channel of current sensor is normal */
		else if((low_ch_value >= CURR_LOW_MEAS_NEG_RANGE) && (low_ch_value <= CURR_LOW_MEAS_POS_RANGE))		//using low channel when within +- 25A 
		{
			low_ch_value = 0u;
			for (i = 0u; i < CURR_SAMP_TIME; i++)
			{
				if (i >= 1u)
				{
					#ifdef Test_Curr_Measurement_low
					low_ch_value += 3601u;
					#else
					low_ch_value += (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_LOW_Sub);
					#endif
				}			
			}
			low_ch_value = low_ch_value >> CURR_AVG_SHIFT;

			if(low_ch_value >= ((uint32)ref5v >> 1u))
			{
				udata = ref5v >> 1u;
				sdata = ((sint32)low_ch_value - (sint32)udata) * 5000 / (sint32)ref5v;
				fdata = (float)sdata / (float)CURR_LOW_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_2.LowGain_Plus;
				curr_calibrated = (sint32)fdata + Curr_CalibData.CurrentSensor_2.LowOffset_Plus;
			}
			else
			{
				udata = ref5v >> 1u;
				sdata = ((sint32)low_ch_value - (sint32)udata) * 5000 / (sint32)ref5v;
				fdata = (float)sdata / (float)CURR_LOW_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_2.LowGain_Minus;
				curr_calibrated = (sint32)fdata + Curr_CalibData.CurrentSensor_2.LowOffset_Minus;
			}
		}

		else
		{
			high_ch_value = 0u;
			for (i = 0u; i < CURR_SAMP_TIME; i++)
			{
				if (i >= 1u)
				{
					high_ch_value += (uint32)AdcEx_GetRawData(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin, ADCEX_CH_CURR_HIGH_Sub);	
				}
			}
			high_ch_value = high_ch_value >> CURR_AVG_SHIFT;							

			/* (+) current */
			if(high_ch_value >= ((uint32)ref5v >> 1u))
			{
				udata = ref5v >> 1u;
				sdata = ((sint32)high_ch_value - (sint32)udata) * 5000 / (sint32)ref5v;
				fdata = (float)sdata / (float)CURR_HIGH_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_2.HighGain_Plus;
				curr_calibrated = (sint32)fdata + Curr_CalibData.CurrentSensor_2.HighOffset_Plus;
			}
			/* (-) current */
			else
			{
				udata = ref5v >> 1u;
				sdata = ((sint32)high_ch_value - (sint32)udata) * 5000 / (sint32)ref5v;
				fdata = (float)sdata / (float)CURR_HIGH_SENSITIVITY * (float)Curr_CalibData.CurrentSensor_2.HighGain_Minus;
				curr_calibrated = (sint32)fdata + Curr_CalibData.CurrentSensor_2.HighOffset_Minus;
			}				
		}		

		if((curr_calibrated <= CURR_NO_PROCESS_RANGE) && (curr_calibrated >= (CURR_NO_PROCESS_RANGE * (-1))))
		{
			curr_calibrated = 0;
		}
	}		

	return curr_calibrated; 
}
/*---------------------------------------------------------------------*/
/**
 * @brief	Current Read Task
 * @param 	void
 * @return	void
*/
void Curr_Read_Task(void)
{
	CLASSB static uint8  m_loop;
	
	switch(m_loop)
	{
		case 0u:
			Ref5v_Calculation();
			m_loop++;
			break;
			
		case 1u:
			Calib_Get_Calibdata(&Curr_CalibData);
			Curr_ADRawUnfiltered += Curr_Measurement(Ref5v_GetADRaw());
			Curr_ADRawUnfiltered_Sub += Curr_Measurement_Sub(Ref5v_GetADRaw());
			Curr_Count1++;
			Curr_Count2++;
		
			m_loop = 0u;
			break;

		default :
			break;
	}
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Current calculation TASK
 * @param 	void
 * @return	void
*/
void Curr_Calculation_Task(void)
{
	CLASSB static bool	Curr_MeasStart;
	CLASSB static sint32  current;
	CLASSB static sint32  current2;
	float		fdata;
	float		fdata2;
	sint32			sdata = 0;
	sint32			sdata2 = 0;
	sint16	s16data;
	sint16	s16data2;

	#ifdef Test_Curr_Calculation_Task
	    Curr_ADRawUnfiltered = 450000;
		Curr_Count1 = 5u;
		Curr_ADRawUnfiltered_Sub = 350000;
		Curr_Count2 = 5u;
	#elif defined(Test_Curr_Calculation_Task_2)
		if(Curr_MeasStart == FALSE)
		{
			Curr_ADRawUnfiltered = -350000;
			Curr_Count1 = 5u;
			Curr_ADRawUnfiltered_Sub = -350000;
			Curr_Count2 = 5u;
		}
		else
		{
			Curr_ADRawUnfiltered = 350000;
			Curr_Count1 = 5u;
			Curr_ADRawUnfiltered_Sub = 350000;
			Curr_Count2 = 5u;
		}
	#endif

	if(Curr_Count1 != 0u)
	{
		sdata = Curr_ADRawUnfiltered / (sint32)Curr_Count1;
		Curr_ADRawUnfiltered = 0;
	}

	if(Curr_Count2 != 0u)
	{
		sdata2 = Curr_ADRawUnfiltered_Sub / (sint32)Curr_Count2;
		Curr_ADRawUnfiltered_Sub = 0;
	}
	
	Curr_Count1 = 0u;
	Curr_Count2 = 0u;

	if (Curr_MeasStart == FALSE)
	{
		current = sdata;
		
		current2 = sdata2;
	}
	else
	{
	    fdata = FILTER(current, sdata, CURR_FILTER_COEF);
		current = (sint32)fdata;

		fdata2 = FILTER(current2, sdata2, CURR_FILTER_COEF);
		current2 = (sint32)fdata2;
	}
	
	Curr_1secSum   += -1 * sdata;
	Curr_1secCount ++;

	fdata = (float)current * (float)0.01;
	
	#ifdef Pack_OverVoltage_Test
	I_PackMeas = 20;
	#endif
	#ifdef Cell_OverVoltage_Test
	I_PackMeas = 20;
	#endif
	#ifdef Pack_UnderVoltage_Test
	I_PackMeas = -20;
	#endif
	#ifdef Cell_UnderVoltage_Test
	I_PackMeas = -20;
	#endif
	#ifdef Temp_Over_Test
	I_PackMeas = 20;
	#endif
	#ifdef Temp_Under_Test
	I_PackMeas = -20;
	#endif
	#ifdef Current_Test
	I_PackMeas = -1 * (sint16)fdata;
	#endif
	
	#ifndef TestMode
	I_PackMeas = -1 * (sint16)fdata;
	#endif

	fdata2 = (float)current2 * (float)0.01;
	I_PackMeas_Sub = -1 * (sint16)fdata2;

	if((I_PackMeas > 100) || (I_PackMeas < -100))
	{
		s16data = ABS(I_PackMeas - I_PackMeas_Sub);
		s16data2 = ABS(I_PackMeas / 10);
		if(s16data > s16data2)
		{
			Curr_SensorOffline = TRUE;
		}
		else
		{
			Curr_SensorOffline = FALSE;
		}
	}
	else
	{
		Curr_SensorOffline = FALSE;
	}

    if(I_PackMeas >= (sint16)0)
    {
        if(I_PackMeas > CurrChgMax)
        {
            CurrChgMax = I_PackMeas;     
        }
    }
    else
    {
        if(I_PackMeas < (-1 * CurrDchMax))
        {
            CurrDchMax = (-1 * I_PackMeas);    
        }
    }

	Curr_MeasStart = TRUE;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Current Low AD raw data
 * @param 	void
 * @return	uint16 - Low AD raw data
*/
uint16 Curr_GetADRawLow(void)
{
	return Curr_ADRawLow;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Current High AD raw data
 * @param 	void
 * @return	uint16 - High AD raw data
*/
uint16 Curr_GetADRawHigh(void)
{
	return Curr_ADRawHigh;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Sub Current Low AD raw data
 * @param 	void
 * @return	uint16 - Low AD raw data
*/
uint16 Curr_GetADRawLow_Sub(void)
{
	return Curr_ADRawLow_Sub;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Sub Current High AD raw data
 * @param 	void
 * @return	uint16 - High AD raw data
*/
uint16 Curr_GetADRawHigh_Sub(void)
{
	return Curr_ADRawHigh_Sub;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Current Low Calibration value
 * @param 	void
 * @return	sint32 - Low Calibration value
*/
sint32 Curr_GetCalibValueLow(void)
{
	return Curr_CalibValueLow;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Current High Calibration value
 * @param 	void
 * @return	sint32 - High Calibration value
*/
sint32 Curr_GetCalibValueHigh(void)
{
	return Curr_CalibValueHigh;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Sub Current Low Calibration value
 * @param 	void
 * @return	sint32 - Low Calibration value
*/
sint32 Curr_GetCalibValueLow_Sub(void)
{
	return Curr_CalibValueLow_Sub;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Sub Current High Calibration value
 * @param 	void
 * @return	sint32 - High Calibration value
*/
sint32 Curr_GetCalibValueHigh_Sub(void)
{
	return Curr_CalibValueHigh_Sub;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Unfilterd Current value
 * @param 	void
 * @return	sint32 - Unfilterd Current value
*/
sint32 Curr_GetCurrUnfiltered(void)
{
	return Curr_ADRawUnfiltered;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Sub Unfilterd Current value
 * @param 	void
 * @return	sint32 - Unfilterd Current value
*/
sint32 Curr_GetCurrUnfiltered_Sub(void)
{
	return Curr_ADRawUnfiltered_Sub;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get sum of curret value (1second)
 * @param 	void
 * @return	sint32 - 1second curret value
*/
sint32 Curr_GetCurr1secSum(void)
{
	return Curr_1secSum;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get curret count value
 * @param 	void
 * @return	uint16 - vcurret count value
*/
uint16 Curr_GetCurr1secCnt(void)
{
	return Curr_1secCount;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Set sum of curret value (1second)
 * @param 	curr_1sec_sum - 1second curret value
 * @return	void
*/
void Curr_SetCurr1secSum(sint32 curr_1sec_sum)
{
	Curr_1secSum = curr_1sec_sum;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Set curret count value
 * @param 	curr_1sec_cnt - curret count value
 * @return	void
*/
void Curr_SetCurr1secCnt(uint16 curr_1sec_cnt)
{
	Curr_1secCount = curr_1sec_cnt;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Check Current sensor offline
 * @param 	void
 * @return	BOOL, TRUE - offline, FALSE - X
*/
bool Curr_IsSensorOffline(void)
{
#ifdef NoCheck_CURR_SensorOFFLine   
    Curr_SensorOffline = FALSE;
#endif


	return (BOOL)Curr_SensorOffline;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get max charge current
 * @param 	void
 * @return	sint16 - Max charge current value
*/
sint16 Curr_GetChargeMax(void)
{
    return CurrChgMax;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get max discharge current
 * @param 	void
 * @return	sint16 - Max discharge current value
*/
sint16 Curr_GetDischargeMax(void)
{
    return CurrDchMax;
}

/*---------------------------------------------------------------------*/
sint16 Curr_Get_I_PackMeas(void)
{
    return I_PackMeas;
}


/*---------------------------------------------------------------------*/
sint16 Curr_Get_I_PackMeas_Sub(void)
{
    return I_PackMeas_Sub;
}






