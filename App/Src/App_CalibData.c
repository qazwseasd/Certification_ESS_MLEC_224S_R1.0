/*
 * App_CalibData.c
 *
 *  Created on: Sep 29, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_CalibData.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define	CALIBDATA_LENGTH			52u

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
static bool        			bCalibDataSaveFlag; 
CLASSB static CALIB_DATA	App_Calibdata;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
 * @brief	Initialize Calibration data
 * @param 	void
 * @return	void
*/
void CalibData_Init(void)
{
	uint8 cal_buf[CALIBDATA_LENGTH];
	bool UpFlag = FALSE;
	uint8 crc[2];
	uint16 udata;
	sint16 sdata;

	bCalibDataSaveFlag = FALSE;
	
	
	if(EepromEx_IsSavedCalibData() == TRUE)
	{
		(void)EepromEx_ReadCalibData(cal_buf);		
	
		if(Crc_Check(cal_buf, 52u) == TRUE)
	    {
			UpFlag = TRUE;
		}
	}

	if(UpFlag == TRUE)
	{
		udata = ((uint16)cal_buf[0u] << 8u) + (uint16)cal_buf[1u];
		App_Calibdata.Pack1V_Gain   = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[2u] << 8u) + (uint16)cal_buf[3u];
		sdata = (sint16)udata;
		App_Calibdata.Pack1V_Offset = (float)sdata / PACKVOLT_RESOLUTION / (float)100.0;

		udata = ((uint16)cal_buf[4u] << 8u) + (uint16)cal_buf[5u];
		App_Calibdata.Pack2V_Gain   = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[6u] << 8u) + (uint16)cal_buf[7u];
		sdata = (sint16)udata;
		App_Calibdata.Pack2V_Offset = (float)sdata / PACKVOLT_RESOLUTION / (float)100.0;

		udata = ((uint16)cal_buf[8u] << 8u) + (uint16)cal_buf[9u];
		App_Calibdata.BusV_Gain   = (float)udata   / (float)1000.;

		udata = ((uint16)cal_buf[10u] << 8u) + (uint16)cal_buf[11u];
		sdata = (sint16)udata;
		App_Calibdata.BusV_Offset = (float)sdata / PACKVOLT_RESOLUTION / (float)100.0;

		udata = ((uint16)cal_buf[12u] << 8u) + (uint16)cal_buf[13u];
		App_Calibdata.Ref5V_Gain   = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[14u] << 8u) + (uint16)cal_buf[15u];
		sdata = (sint16)udata;
		App_Calibdata.Ref5V_Offset = (float)sdata / REF5V_RESOLUTION / (float)100.0;


		udata = ((uint16)cal_buf[16u] << 8u) + (uint16)cal_buf[17u];
		App_Calibdata.CurrentSensor_1.LowGain_Plus = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[18u] << 8u) + (uint16)cal_buf[19u];
		sdata = (sint16)udata;
		App_Calibdata.CurrentSensor_1.LowOffset_Plus = (sint32)sdata;

		udata = ((uint16)cal_buf[20u] << 8u) + (uint16)cal_buf[21u];
		App_Calibdata.CurrentSensor_1.LowGain_Minus = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[22u] << 8u) + (uint16)cal_buf[23u];
		sdata = (sint16)udata;
		App_Calibdata.CurrentSensor_1.LowOffset_Minus = (sint32)sdata;

		udata = ((uint16)cal_buf[24u] << 8u) + (uint16)cal_buf[25u];
		App_Calibdata.CurrentSensor_1.HighGain_Plus = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[26u] << 8u) + (uint16)cal_buf[27u];
		sdata = (sint16)udata;
		App_Calibdata.CurrentSensor_1.HighOffset_Plus = (sint32)sdata;

		udata = ((uint16)cal_buf[28u] << 8u) + (uint16)cal_buf[29u];
		App_Calibdata.CurrentSensor_1.HighGain_Minus = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[30u] << 8u) + (uint16)cal_buf[31u];
		sdata = (sint16)udata;
		App_Calibdata.CurrentSensor_1.HighOffset_Minus = (sint32)sdata;

		udata = ((uint16)cal_buf[32u] << 8u) + (uint16)cal_buf[33u];
		App_Calibdata.CurrentSensor_2.LowGain_Plus = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[34u] << 8u) + (uint16)cal_buf[35u];
		sdata = (sint16)udata;
		App_Calibdata.CurrentSensor_2.LowOffset_Plus = (sint32)sdata;

		udata = ((uint16)cal_buf[36u] << 8u) + (uint16)cal_buf[37u];
		App_Calibdata.CurrentSensor_2.LowGain_Minus = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[38u] << 8u) + (uint16)cal_buf[39u];
		sdata = (sint16)udata;
		App_Calibdata.CurrentSensor_2.LowOffset_Minus = (sint32)sdata;

		udata = ((uint16)cal_buf[40u] << 8u) + (uint16)cal_buf[41u];
		App_Calibdata.CurrentSensor_2.HighGain_Plus = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[42u] << 8u) + (uint16)cal_buf[43u];
		sdata = (sint16)udata;
		App_Calibdata.CurrentSensor_2.HighOffset_Plus = (sint32)sdata;

		udata = ((uint16)cal_buf[44u] << 8u) + (uint16)cal_buf[45u];
		App_Calibdata.CurrentSensor_2.HighGain_Minus = (float)udata / (float)1000.;

		udata = ((uint16)cal_buf[46u] << 8u) + (uint16)cal_buf[47u];
		sdata = (sint16)udata;
		App_Calibdata.CurrentSensor_2.HighOffset_Minus = (sint32)sdata;		

	}
	else
	{
		App_Calibdata.Pack1V_Gain = 1000.f;
		App_Calibdata.Pack1V_Offset = 0.f;
		App_Calibdata.Pack2V_Gain = 1000.f;
		App_Calibdata.Pack2V_Offset = 0.f;
		App_Calibdata.BusV_Gain = 1000.f;
		App_Calibdata.BusV_Offset = 0.f;
		App_Calibdata.Ref5V_Gain = 1000.f;
		App_Calibdata.Ref5V_Offset = 0.f;

		App_Calibdata.CurrentSensor_1.LowGain_Plus = 1000.f;
		App_Calibdata.CurrentSensor_1.LowOffset_Plus = 0;
		App_Calibdata.CurrentSensor_1.LowGain_Minus = 1000.f;
		App_Calibdata.CurrentSensor_1.LowOffset_Minus = 0;
		App_Calibdata.CurrentSensor_1.HighGain_Plus = 1000.f;
		App_Calibdata.CurrentSensor_1.HighOffset_Plus = 0;
		App_Calibdata.CurrentSensor_1.HighGain_Minus = 1000.f;
		App_Calibdata.CurrentSensor_1.HighOffset_Minus = 0;
		App_Calibdata.CurrentSensor_2.LowGain_Plus = 1000.f;
		App_Calibdata.CurrentSensor_2.LowOffset_Plus = 0;
		App_Calibdata.CurrentSensor_2.LowGain_Minus = 1000.f;
		App_Calibdata.CurrentSensor_2.LowOffset_Minus = 0;
		App_Calibdata.CurrentSensor_2.HighGain_Plus = 1000.f;
		App_Calibdata.CurrentSensor_2.HighOffset_Plus = 0;
		App_Calibdata.CurrentSensor_2.HighGain_Minus = 1000.f;
		App_Calibdata.CurrentSensor_2.HighOffset_Minus = 0;
		
		cal_buf[0u] = (uint8)((uint16)App_Calibdata.Pack1V_Gain >> 8u) ;
		cal_buf[1u] = (uint8)((uint16)App_Calibdata.Pack1V_Gain & 0xFFu) ;
		cal_buf[2u] = (uint8)((uint16)App_Calibdata.Pack1V_Offset >> 8u) ;
		cal_buf[3u] = (uint8)((uint16)App_Calibdata.Pack1V_Offset & 0xFFu) ;

		cal_buf[4u] = (uint8)((uint16)App_Calibdata.Pack2V_Gain >> 8u) ;
		cal_buf[5u] = (uint8)((uint16)App_Calibdata.Pack2V_Gain & 0xFFu) ;
		cal_buf[6u] = (uint8)((uint16)App_Calibdata.Pack2V_Offset >> 8u) ;
		cal_buf[7u] = (uint8)((uint16)App_Calibdata.Pack2V_Offset & 0xFFu) ;

		cal_buf[8u] = (uint8)((uint16)App_Calibdata.BusV_Gain >> 8u) ;
		cal_buf[9u] = (uint8)((uint16)App_Calibdata.BusV_Gain & 0xFFu) ;
		cal_buf[10u] = (uint8)((uint16)App_Calibdata.BusV_Offset >> 8u) ;
		cal_buf[11u] = (uint8)((uint16)App_Calibdata.BusV_Offset & 0xFFu) ;

		cal_buf[12u] = (uint8)((uint16)App_Calibdata.Ref5V_Gain>> 8u) ;
		cal_buf[13u] = (uint8)((uint16)App_Calibdata.Ref5V_Gain & 0xFFu) ;
		cal_buf[14u] = (uint8)((uint16)App_Calibdata.Ref5V_Offset>> 8u) ;
		cal_buf[15u] = (uint8)((uint16)App_Calibdata.Ref5V_Offset & 0xFFu) ;
		
		cal_buf[16u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.LowGain_Plus >> 8u) ;
		cal_buf[17u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.LowGain_Plus & 0xFFu) ;
		cal_buf[18u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.LowOffset_Plus >> 8u) ;
		cal_buf[19u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.LowOffset_Plus & 0xFFu );		
		cal_buf[20u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.LowGain_Minus >> 8u) ;
		cal_buf[21u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.LowGain_Minus & 0xFFu) ;
		cal_buf[22u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.LowOffset_Minus >> 8u) ;
		cal_buf[23u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.LowOffset_Minus & 0xFFu) ;
		
		cal_buf[24u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.HighGain_Plus >> 8u) ;
		cal_buf[25u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.HighGain_Plus & 0xFFu) ;
		cal_buf[26u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.HighOffset_Plus >> 8u) ;
		cal_buf[27u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.HighOffset_Plus & 0xFFu) ;
		cal_buf[28u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.HighGain_Minus >> 8u) ;
		cal_buf[29u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.HighGain_Minus & 0xFFu) ;
		cal_buf[30u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.HighOffset_Minus >> 8u) ;
		cal_buf[31u] = (uint8)((uint16)App_Calibdata.CurrentSensor_1.HighOffset_Minus & 0xFFu) ;

		cal_buf[32u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.LowGain_Plus >> 8u) ;
		cal_buf[33u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.LowGain_Plus & 0xFFu) ;
		cal_buf[34u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.LowOffset_Plus >> 8u) ;
		cal_buf[35u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.LowOffset_Plus & 0xFFu) ;		
		cal_buf[36u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.LowGain_Minus >> 8u) ;
		cal_buf[37u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.LowGain_Minus & 0xFFu) ;
		cal_buf[38u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.LowOffset_Minus >> 8u) ;
		cal_buf[39u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.LowOffset_Minus & 0xFFu) ;
		
		cal_buf[40u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.HighGain_Plus >> 8u) ;
		cal_buf[41u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.HighGain_Plus & 0xFFu) ;
		cal_buf[42u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.HighOffset_Plus >> 8u) ;
		cal_buf[43u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.HighOffset_Plus & 0xFFu) ;
		cal_buf[44u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.HighGain_Minus >> 8u) ;
		cal_buf[45u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.HighGain_Minus & 0xFFu) ;
		cal_buf[46u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.HighOffset_Minus >> 8u) ;
		cal_buf[47u] = (uint8)((uint16)App_Calibdata.CurrentSensor_2.HighOffset_Minus & 0xFFu) ;
		cal_buf[48u] = 0u ;
		cal_buf[49u] = 0u ;

		Crc_Calculation(crc, cal_buf, CALIBDATA_LENGTH-2u);
    	memnadd(cal_buf, crc, CALIBDATA_LENGTH-2u, 2u);

		(void)EepromEx_SetSavedCalibDataFlag(TRUE);
		HAL_Delay(5u);
		(void)EepromEx_WriteCalibData(cal_buf);	
		HAL_Delay(5u);

		App_Calibdata.Pack1V_Gain = 1.f;
        App_Calibdata.Pack2V_Gain = 1.f;
		App_Calibdata.BusV_Gain = 1.f;
		App_Calibdata.Ref5V_Gain = 1.f;
		
		App_Calibdata.CurrentSensor_1.LowGain_Plus = 1.f;
		App_Calibdata.CurrentSensor_1.LowGain_Minus = 1.f;
		App_Calibdata.CurrentSensor_1.HighGain_Plus = 1.f;
		App_Calibdata.CurrentSensor_1.HighGain_Minus = 1.f;
		App_Calibdata.CurrentSensor_2.LowGain_Plus = 1.f;
		App_Calibdata.CurrentSensor_2.LowGain_Minus = 1.f;
		App_Calibdata.CurrentSensor_2.HighGain_Plus = 1.f;
		App_Calibdata.CurrentSensor_2.HighGain_Minus = 1.f;
	}

	Calib_Set_Calibdata(&App_Calibdata);
}

/*---------------------------------------------------------------------*/
void CalibData_Wrtie(void)
{

	uint8 cal_no = Calibrate_Data[0u] ; 		
	float gain_buf; 
	sint32  offset_buf;				
	uint16 u16data;
	uint8 cal_buf[CALIBDATA_LENGTH];
 	uint8 crc[2];
	float fdata;
	sint16	s16data;


	switch(cal_no)
	{
		case 0x02u: 		// write 
			u16data = (((uint16)Calibrate_Data[2u] << 8u) + (uint16)Calibrate_Data[3u]);
			gain_buf = (float)u16data / (float)1000.;
			u16data = (((uint16)Calibrate_Data[4u] << 8u) + (uint16)Calibrate_Data[5u]);
			offset_buf = (sint16)u16data;
	
			if(Calibrate_Data[1u] == 0x00u)
			{
				App_Calibdata.CurrentSensor_1.LowGain_Plus = gain_buf;
				App_Calibdata.CurrentSensor_1.LowOffset_Plus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x01u)
			{
				App_Calibdata.CurrentSensor_1.LowGain_Minus = gain_buf;
				App_Calibdata.CurrentSensor_1.LowOffset_Minus= offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x02u)
			{
				App_Calibdata.CurrentSensor_1.HighGain_Plus = gain_buf;
				App_Calibdata.CurrentSensor_1.HighOffset_Plus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x03u)
			{
				App_Calibdata.CurrentSensor_1.HighGain_Minus = gain_buf;
				App_Calibdata.CurrentSensor_1.HighOffset_Minus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x04u)
			{
				App_Calibdata.Pack1V_Gain = gain_buf;
				App_Calibdata.Pack1V_Offset = (float)offset_buf / PACKVOLT_RESOLUTION / (float)100.0;
			}
			else if(Calibrate_Data[1u] == 0x05u)
			{
				App_Calibdata.Pack2V_Gain = gain_buf;
				App_Calibdata.Pack2V_Offset = (float)offset_buf / PACKVOLT_RESOLUTION / (float)100.0;
			}
			else if(Calibrate_Data[1u] == 0x06u)
			{
				App_Calibdata.BusV_Gain = gain_buf;
				App_Calibdata.BusV_Offset = (float)offset_buf / PACKVOLT_RESOLUTION / (float)100.0;
			}
			else if(Calibrate_Data[1u] == 0x07u)
			{
				App_Calibdata.Ref5V_Gain= gain_buf;
				App_Calibdata.Ref5V_Offset = (float)offset_buf / REF5V_RESOLUTION / (float)100.0;
			}			
			else if(Calibrate_Data[1u] == 0x08u)
			{
				App_Calibdata.CurrentSensor_2.LowGain_Plus = gain_buf;
				App_Calibdata.CurrentSensor_2.LowOffset_Plus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x09u)
			{
				App_Calibdata.CurrentSensor_2.LowGain_Minus = gain_buf;
				App_Calibdata.CurrentSensor_2.LowOffset_Minus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x0Au)
			{
				App_Calibdata.CurrentSensor_2.HighGain_Plus = gain_buf;
				App_Calibdata.CurrentSensor_2.HighOffset_Plus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x0Bu)
			{
				App_Calibdata.CurrentSensor_2.HighGain_Minus = gain_buf;
				App_Calibdata.CurrentSensor_2.HighOffset_Minus = offset_buf;
			}				
			else
			{

			}
			Calib_Set_Calibdata(&App_Calibdata);
			
			break;
			
		case 0x03u: 				
			gain_buf = 1.f;
			offset_buf = 0;
	
			if(Calibrate_Data[1u] == 0x00u)
			{
				App_Calibdata.CurrentSensor_1.LowGain_Plus = gain_buf;
				App_Calibdata.CurrentSensor_1.LowOffset_Plus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x01u)
			{
				App_Calibdata.CurrentSensor_1.LowGain_Minus = gain_buf;
				App_Calibdata.CurrentSensor_1.LowOffset_Minus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x02u)
			{
				App_Calibdata.CurrentSensor_1.HighGain_Plus = gain_buf;
				App_Calibdata.CurrentSensor_1.HighOffset_Plus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x03u)
			{
				App_Calibdata.CurrentSensor_1.HighGain_Minus = gain_buf;
				App_Calibdata.CurrentSensor_1.HighOffset_Minus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x04u)
			{
				App_Calibdata.Pack1V_Gain = gain_buf ; 
				App_Calibdata.Pack1V_Offset = (float)offset_buf ; 
			}
			else if(Calibrate_Data[1u] == 0x05u)
			{
				App_Calibdata.Pack2V_Gain = gain_buf ; 
				App_Calibdata.Pack2V_Offset = (float)offset_buf ; 
			}
			else if(Calibrate_Data[1u] == 0x06u)
			{
				App_Calibdata.BusV_Gain = gain_buf ; 
				App_Calibdata.BusV_Offset = (float)offset_buf ;					
			}
			else if(Calibrate_Data[1u] == 0x07u)
			{
				App_Calibdata.Ref5V_Gain = gain_buf ; 
				App_Calibdata.Ref5V_Offset = (float)offset_buf ;					
			}			
			else if(Calibrate_Data[1u] == 0x08u)
			{
				App_Calibdata.CurrentSensor_2.LowGain_Plus = gain_buf;
				App_Calibdata.CurrentSensor_2.LowOffset_Plus = offset_buf;
			}
			else if(Calibrate_Data[1] == 0x09u)
			{
				App_Calibdata.CurrentSensor_2.LowGain_Minus = gain_buf;
				App_Calibdata.CurrentSensor_2.LowOffset_Minus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x0Au)
			{
				App_Calibdata.CurrentSensor_2.HighGain_Plus = gain_buf;
				App_Calibdata.CurrentSensor_2.HighOffset_Plus = offset_buf;
			}
			else if(Calibrate_Data[1u] == 0x0Bu)
			{
				App_Calibdata.CurrentSensor_2.HighGain_Minus = gain_buf;
				App_Calibdata.CurrentSensor_2.HighOffset_Minus = offset_buf;
			}			
			else if(Calibrate_Data[1u] == 0x0Cu)
			{

			}
			else
			{
			

			}

			Calib_Set_Calibdata(&App_Calibdata);
			
			break;
			
		default:	break;
	}

	fdata = App_Calibdata.Pack1V_Gain*1000.f;
	cal_buf[0u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[1u] = (uint8)((uint16)fdata & 0xFFu) ;
	fdata = App_Calibdata.Pack1V_Offset*10.f;
	cal_buf[2u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[3u] = (uint8)((uint16)fdata & 0xFFu) ;

	fdata = App_Calibdata.Pack2V_Gain*1000.f;
	cal_buf[4u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[5u] = (uint8)((uint16)fdata & 0xFFu) ;
	fdata = App_Calibdata.Pack2V_Offset*10.f;
	cal_buf[6u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[7u] = (uint8)((uint16)fdata & 0xFFu) ;

	fdata = App_Calibdata.BusV_Gain*1000.f;
	cal_buf[8u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[9u] = (uint8)((uint16)fdata & 0xFFu) ;
	fdata = App_Calibdata.BusV_Offset*10.f;
	cal_buf[10u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[11u] = (uint8)((uint16)fdata & 0xFFu) ;

	fdata = App_Calibdata.Ref5V_Gain*1000.f;
	cal_buf[12u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[13u] = (uint8)((uint16)fdata & 0xFFu) ;
	fdata = App_Calibdata.Ref5V_Offset/10.f;
	cal_buf[14u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[15u] = (uint8)((uint16)fdata & 0xFFu) ;

	fdata = App_Calibdata.CurrentSensor_1.LowGain_Plus*1000.f;
	cal_buf[16u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[17u] = (uint8)((uint16)fdata & 0xFFu) ;
	s16data = (sint16)App_Calibdata.CurrentSensor_1.LowOffset_Plus;
	cal_buf[18u] = (uint8)((uint16)s16data >> 8u) ;
	cal_buf[19u] = (uint8)((uint16)s16data & 0xFFu) ;
	fdata = App_Calibdata.CurrentSensor_1.LowGain_Minus*1000.f;
	cal_buf[20u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[21u] = (uint8)((uint16)fdata & 0xFFu) ;
	s16data = (sint16)App_Calibdata.CurrentSensor_1.LowOffset_Minus;
	cal_buf[22u] = (uint8)((uint16)s16data >> 8u) ;
	cal_buf[23u] = (uint8)((uint16)s16data & 0xFFu) ;

	fdata = App_Calibdata.CurrentSensor_1.HighGain_Plus*1000.f;
	cal_buf[24u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[25u] = (uint8)((uint16)fdata & 0xFFu) ;
	s16data = (sint16)App_Calibdata.CurrentSensor_1.HighOffset_Plus;
	cal_buf[26u] = (uint8)((uint16)s16data >> 8u) ;
	cal_buf[27u] = (uint8)((uint16)s16data & 0xFFu) ;
	fdata = App_Calibdata.CurrentSensor_1.HighGain_Minus*1000.f;
	cal_buf[28u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[29u] = (uint8)((uint16)fdata & 0xFFu) ;
	s16data = (sint16)App_Calibdata.CurrentSensor_1.HighOffset_Minus;
	cal_buf[30u] = (uint8)((uint16)s16data >> 8u) ;
	cal_buf[31u] = (uint8)((uint16)s16data & 0xFFu) ;

	fdata = App_Calibdata.CurrentSensor_2.LowGain_Plus*1000.f;
	cal_buf[32u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[33u] = (uint8)((uint16)fdata & 0xFFu) ;
	s16data = (sint16)App_Calibdata.CurrentSensor_2.LowOffset_Plus;
	cal_buf[34u] = (uint8)((uint16)s16data >> 8u) ;
	cal_buf[35u] = (uint8)((uint16)s16data & 0xFFu) ;
	fdata = App_Calibdata.CurrentSensor_2.LowGain_Minus*1000.f;
	cal_buf[36u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[37u] = (uint8)((uint16)fdata & 0xFFu) ;
	s16data = (sint16)App_Calibdata.CurrentSensor_2.LowOffset_Minus;
	cal_buf[38u] = (uint8)((uint16)s16data >> 8u) ;
	cal_buf[39u] = (uint8)((uint16)s16data & 0xFFu) ;

	fdata = App_Calibdata.CurrentSensor_2.HighGain_Plus*1000.f;
	cal_buf[40u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[41u] = (uint8)((uint16)fdata & 0xFFu) ;
	s16data = (sint16)App_Calibdata.CurrentSensor_2.HighOffset_Plus;
	cal_buf[42u] = (uint8)((uint16)s16data >> 8u) ;
	cal_buf[43u] = (uint8)((uint16)s16data & 0xFFu) ;
	fdata = App_Calibdata.CurrentSensor_2.HighGain_Minus*1000.f;
	cal_buf[44u] = (uint8)((uint16)fdata >> 8u) ;
	cal_buf[45u] = (uint8)((uint16)fdata & 0xFFu) ;
	s16data = (sint16)App_Calibdata.CurrentSensor_2.HighOffset_Minus;
	cal_buf[46u] = (uint8)((uint16)s16data >> 8u) ;
	cal_buf[47u] = (uint8)((uint16)s16data & 0xFFu) ;
	
	cal_buf[48u] = 0u ;
	cal_buf[49u] = 0u ;

	Crc_Calculation(crc, cal_buf, CALIBDATA_LENGTH-2u);
    memnadd(cal_buf, crc, CALIBDATA_LENGTH-2u, 2u);

	(void)EepromEx_WriteCalibData(cal_buf);
	
}


/*---------------------------------------------------------------------*/
/**
* @brief Get Clibration save flag function
* @param void
* @return BOOL bCalibDataSaveFlag value
*/
BOOL Get_IsSaveCalibData(void)
{
    return bCalibDataSaveFlag;
}

/*---------------------------------------------------------------------*/
/**
* @brief Set Clibration save flag function
* @param BOOL Set the bCalibDataSaveFlag
* @return
*/
void Set_SaveCalibData(BOOL SetV)
{
    bCalibDataSaveFlag = SetV;
}


