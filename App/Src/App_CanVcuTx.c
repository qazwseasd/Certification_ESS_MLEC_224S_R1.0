/*
 * App_CanVcuTx.c
 *
 *  Created on: Aug 11, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_CanVcuTx.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define CONFIG_UPDATE_TIME	(1000u / CAN_TXTOMASTER_PERIOD)


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/

typedef void	(*CanVcuTx_Set_Data)( uint8 *TxData );

typedef struct
{
    uint32 				period_ms;
    uint32 				last_Tx_ms;
    CanVcuTx_Set_Data	Func;
} CanVcuTx_Msg_t;

typedef struct
{
    bool 				Send_f;
    CanVcuTx_Set_Data	Func;
} CanVcuTx_EventMsg_t;

/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/



/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/

static	CAN_TxHeaderTypeDef CanVcuTx_hdr = {0};
/*
static	CanVcuTx_Msg_t		CanVcuTx_Msg[] =
{
		{  100u, 0u, CanVcuTx_Set_ReportData1  },
};
*/
static	CanVcuTx_EventMsg_t		CanVcuTx_ReqMsg[] =
{
		{  DEF_False, CanVcuTx_Set_PackData1 },
		{  DEF_False, CanVcuTx_Set_PackData2 },
		{  DEF_False, CanVcuTx_Set_PackData3 },
		{  DEF_False, CanVcuTx_Set_PackData4 },
		{  DEF_False, CanVcuTx_Set_PackData5 },
		{  DEF_False, CanVcuTx_Set_PackData6 },
		{  DEF_False, CanVcuTx_Set_PackData7 },
		{  DEF_False, CanVcuTx_Set_PackData8 },
		{  DEF_False, CanVcuTx_Set_PackData9 },
		{  DEF_False, CanVcuTx_Set_PackData10 },
		{  DEF_False, CanVcuTx_Set_PackData11 },
		{  DEF_False, CanVcuTx_Set_PackData12 },
		{  DEF_False, CanVcuTx_Set_PackData13 },
		{  DEF_False, CanVcuTx_Set_PackData14 },
		{  DEF_False, CanVcuTx_Set_PackData15 },
		{  DEF_False, CanVcuTx_Set_ModeChange },
		{  DEF_False, CanVcuTx_Set_RelayData },
		{  DEF_False, CanVcuTx_Set_ModuleData1 },
		{  DEF_False, CanVcuTx_Set_ModuleData2 },
		{  DEF_False, CanVcuTx_Set_ModuleData3 },
		{  DEF_False, CanVcuTx_Set_ModuleData4 },
		{  DEF_False, CanVcuTx_Set_ModuleData5 },
		{  DEF_False, CanVcuTx_Set_ModuleData6 },
		{  DEF_False, CanVcuTx_Set_ModuleData7 },
		{  DEF_False, CanVcuTx_Set_ModuleData8 },
		{  DEF_False, CanVcuTx_Set_ModuleData9 },
		{  DEF_False, CanVcuTx_Set_FaultRelease },
		{  DEF_False, CanVcuTx_Set_ConfigrationData },
		{  DEF_False, CanVcuTx_Set_CalibrateData },
		{  DEF_False, CanVcuTx_Set_EventData1},
		{  DEF_False, CanVcuTx_Set_EventData2},
		{  DEF_False, CanVcuTx_Set_EventData3},
		{  DEF_False, CanVcuTx_Set_DetectFCCData },
		
};

CAPACITY_DATA	static CAN_CapacityData;
CLASSB static CALIB_DATA	CANVcu_Calibdata;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
void 	Set_BMIC_No(void);
void	Set_ConfigrationData(void);
void	Set_CalibrationData(void);
void	Set_EventData(void);
void	Set_DetectFCCData(void);

bool Can_InOutParameters(uint32 addr, BUF_INOUT bg, char* tbuf, bool real_vir);



/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/

/**
* @brief	Can TX to User Interface Initialization
* @param	void
* @return	void
*/
void	CanVcuTx_Init(void)
{
	CanVcuTx_hdr.IDE = CAN_ID_EXT;
	CanVcuTx_hdr.RTR = CAN_RTR_DATA;
}


void	CanVcuTx_Task(void)
{
	uint8	TxData[APP_CanVcuTx_MaxBuf_Size]	= {0u};

//	uint32	nowTime	= 0u;
	uint8	msgIdx;

//	nowTime = Tick_Get();
/*
	for ( msgIdx = 0u; msgIdx < APP_CanVcuTx_CmdMsg_Num; msgIdx++ )
	{
		if ( ( nowTime - CanVcuTx_Msg[msgIdx].last_Tx_ms ) >= CanVcuTx_Msg[msgIdx].period_ms )
		{
			CanVcuTx_Msg[msgIdx].Func( TxData );
			if ( Can_Send_MsgFrame( APP_CanVcuTx_Port, &CanVcuTx_hdr, TxData ) == DEF_True )
			{
				CanVcuTx_Msg[msgIdx].last_Tx_ms = nowTime;
			}
			else{}
		}
		else{}
	}
*/
	for ( msgIdx = 0u; msgIdx < APP_CanVcuTx_ReqMsg_Num; msgIdx++ )
	{
		if ( CanVcuTx_ReqMsg[msgIdx].Send_f == DEF_True )
		{
			CanVcuTx_ReqMsg[msgIdx].Func( TxData );
			if ( Can_Send_MsgFrame( APP_CanVcuTx_Port, &CanVcuTx_hdr, TxData ) == DEF_True )
			{
				CanVcuTx_ReqMsg[msgIdx].Send_f = DEF_False;
			}
		}
	}

}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ReportData1( uint8 *TxData )
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	(void)HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	(void)HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_ReportData1;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( (sDate.Year	>> 0u) & 0xFFu );
	TxData[1]		= (uint8)( (sDate.Month	>> 0u) & 0xFFu );
	TxData[2]		= (uint8)( (sDate.Date  >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (sTime.Hours   >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (sTime.Minutes  >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (sTime.Seconds  >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( (0u 	>> 0u) & 0xFFu );
	TxData[7]		= (uint8)( (0u 	>> 0u) & 0xFFu );

}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData1( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData1;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( ((uint32)ManaMsm_Get_System_State() >> 0u) & 0xFFu );
	TxData[1]		= (uint8)( (PackVolt_Get_V_BusMeas() >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( (PackVolt_Get_V_BusMeas() >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (PackVolt_Get_V_PackMeas() >> 8u) & 0xFFu );
	TxData[4]		= (uint8)( (PackVolt_Get_V_PackMeas() >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (SOC_Get_SOC_User() >> 8u) & 0xFFu );
	TxData[6]		= (uint8)( (SOC_Get_SOC_User() >> 0u) & 0xFFu );
	TxData[7]		= (uint8)( (SOH_Get_SOH() >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData2( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData2;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( ((uint16)Curr_Get_I_PackMeas() >> 8u) & 0xFFu );
	TxData[1]		= (uint8)( ((uint16)Curr_Get_I_PackMeas() >> 0u) & 0xFFu );
	TxData[2]		= (uint8)( ((uint16)CurrLimit_Get_ChgCurrLimit() >> 8u) & 0xFFu );
	TxData[3]		= (uint8)( ((uint16)CurrLimit_Get_ChgCurrLimit() >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( ((uint16)CurrLimit_Get_DchCurrLimit() >> 8u) & 0xFFu );
	TxData[5]		= (uint8)( ((uint16)CurrLimit_Get_DchCurrLimit() >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( (IsoR_Get_ISO_Resistor() >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( (IsoR_Get_ISO_Resistor() >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData3( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData3;
	CanVcuTx_hdr.DLC	= 8u;

	uint16	V_CellMax;
	uint16	V_CellMin;
	uint16	V_CellAvg;

	V_CellMax = Adbms_Get_V_CellMax();
	V_CellMin = Adbms_Get_V_CellMin();
	V_CellAvg = Adbms_Get_V_CellAvg();

	TxData[0]		= (uint8)( (V_CellMax >> 8u) & 0xFFu );
	TxData[1]		= (uint8)( (V_CellMax >> 0u) & 0xFFu );
	TxData[2]		= (uint8)( (V_CellMin >> 8u) & 0xFFu );
	TxData[3]		= (uint8)( (V_CellMin >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (V_CellAvg >> 8u) & 0xFFu );
	TxData[5]		= (uint8)( (V_CellAvg >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( (AuxVolt_Get_V_Aux() >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( (AuxVolt_Get_V_Aux() >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData4( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData4;
	CanVcuTx_hdr.DLC	= 8u;

	sint16 T_CellMax;
	sint16 T_CellMin;
	sint16 T_CellAvg;

	T_CellMax =	Adbms_Get_T_CellMax();
	T_CellMin = Adbms_Get_T_CellMin();
	T_CellAvg = Adbms_Get_T_CellAvg();

	TxData[0]		= (uint8)( ((uint16)T_CellMax >> 8u) & 0xFFu );
	TxData[1]		= (uint8)( ((uint16)T_CellMax >> 0u) & 0xFFu );
	TxData[2]		= (uint8)( ((uint16)T_CellMin >> 8u) & 0xFFu );
	TxData[3]		= (uint8)( ((uint16)T_CellMin >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( ((uint16)T_CellAvg >> 8u) & 0xFFu );
	TxData[5]		= (uint8)( ((uint16)T_CellAvg >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( ((uint16)BoardTemp_Get_T_BoardMlec() >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( ((uint16)BoardTemp_Get_T_BoardMlec() >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData5( uint8 *TxData )
{
	uint8 rly_buf=0u;
	uint16 bal_cnt;
	uint16 V_ModMax;
	uint16 V_ModMin;

	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData5;
	CanVcuTx_hdr.DLC	= 8u;

	bal_cnt = CellBalance_Get_NumberOfBalancingCell();

	V_ModMax = Adbms_Get_V_ModMax();
	V_ModMin = Adbms_Get_V_ModMin();

	if((Relay_GetMainRelayCommand() & 0x04u) != 0u)
	{
		rly_buf |= 0x01u;	// positive	coil
	}
	if((Relay_GetMainRelayCommand() & 0x02u) != 0u)
	{
		rly_buf |= 0x02u;	// negative coil
	}
	if((Relay_GetMainRelayCommand() & 0x01u) != 0u)
	{
		rly_buf |= 0x04u;	// precharge coil	
	}
	if((Relay_Get_Relay_Input_V() & 0x04u) != 0u)
	{
		rly_buf |= 0x08u;	// positive aux
	}
	if((Relay_Get_Relay_Input_V() & 0x02u) != 0u)
	{
		rly_buf |= 0x10u;	// negative aux	
	}

	TxData[0]		= (uint8)( (rly_buf >> 0u) & 0xFFu );
	TxData[1]		= (uint8)( ((uint8)IsoR_Get_MeasOperationStatus() >> 0u) & 0xFFu );
	TxData[2]		= (uint8)( (bal_cnt >> 8u) & 0xFFu );
	TxData[3]		= (uint8)( (bal_cnt >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (SOUE_Get_SOUE() >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( ((V_ModMax / 100u) >> 4u) & 0xFFu );
	TxData[6]		= (uint8)(((V_ModMax / 100u) << 4u) + (((V_ModMin / 100u) >> 8u) & 0x0Fu));
	TxData[7]		= (uint8)( ((V_ModMin / 100u) >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData6( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData6;
	CanVcuTx_hdr.DLC	= 8u;
	uint16	ContOpenCount_UnderLoad;
	uint16 	V_CellDev;
	sint16	T_CellDev;

	V_CellDev = Adbms_Get_V_CellDev();
	ContOpenCount_UnderLoad = SOC_Get_ContOpenCount_UnderLoad();
	T_CellDev = Adbms_Get_T_CellDev();

	TxData[0]		= (uint8)( (ContOpenCount_UnderLoad >> 8u) & 0xFFu );
	TxData[1]		= (uint8)( (ContOpenCount_UnderLoad >> 0u) & 0xFFu );
	TxData[2]		= (uint8)( ((uint16)T_CellDev >> 8u) & 0xFFu );
	TxData[3]		= (uint8)( ((uint16)T_CellDev >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (V_CellDev >> 8u) & 0xFFu );
	TxData[5]		= (uint8)( (V_CellDev >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( (SOC_Get_CV_SOC() >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( (SOC_Get_CV_SOC() >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData7( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData7;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( (AppInit_Get_MaxChgVolt() >> 8u) & 0xFFu );
	TxData[1]		= (uint8)( (AppInit_Get_MaxChgVolt() >> 0u) & 0xFFu );
	TxData[2]		= (uint8)( (AppInit_Get_MinDchVolt() >> 8u) & 0xFFu );
	TxData[3]		= (uint8)( (AppInit_Get_MinDchVolt() >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( ((uint16)AppInit_Get_MaxCurrent()  >> 8u) & 0xFFu );
	TxData[5]		= (uint8)( ((uint16)AppInit_Get_MaxCurrent()  >> 0u) & 0xFFu );

			if		(Diag_Get_FlagsPFault() != 0u)
			{
				TxData[6]		= (uint8)( (4u >> 0u) & 0xFFu );
			}
			else if (Diag_Get_FlagsFault() != 0u)
			{
				TxData[6]		= (uint8)( (3u >> 0u) & 0xFFu );
			}
			else if	(Diag_Get_FlagsWarn() != 0u)
			{
				TxData[6]		= (uint8)( (2u >> 0u) & 0xFFu );
			}
			else if (Diag_Get_FlagsInfo() != 0u)
			{
				TxData[6]		= (uint8)( (1u >> 0u) & 0xFFu );
			}
			else								
			{
				TxData[6]		= (uint8)( (0u >> 0u) & 0xFFu );
			}

	TxData[7]		= (uint8)( (ManaMsm_Get_HeartBeat() >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData8( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData8;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( (Diag_Get_FlagsInfo() >> 24u) & 0xFFu );
	TxData[1]		= (uint8)( (Diag_Get_FlagsInfo() >> 16u) & 0xFFu );
	TxData[2]		= (uint8)( (Diag_Get_FlagsInfo() >> 8u) & 0xFFu );
	TxData[3]		= (uint8)( (Diag_Get_FlagsInfo() >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (Diag_Get_FlagsWarn() >> 24u) & 0xFFu );
	TxData[5]		= (uint8)( (Diag_Get_FlagsWarn() >> 16u) & 0xFFu );
	TxData[6]		= (uint8)( (Diag_Get_FlagsWarn() >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( (Diag_Get_FlagsWarn() >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData9( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData9;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( (Diag_Get_FlagsFault() >> 24u) & 0xFFu );
	TxData[1]		= (uint8)( (Diag_Get_FlagsFault() >> 16u) & 0xFFu );
	TxData[2]		= (uint8)( (Diag_Get_FlagsFault() >> 8u) & 0xFFu );
	TxData[3]		= (uint8)( (Diag_Get_FlagsFault() >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (Diag_Get_FlagsPFault() >> 24u) & 0xFFu );
	TxData[5]		= (uint8)( (Diag_Get_FlagsPFault() >> 16u) & 0xFFu );
	TxData[6]		= (uint8)( (Diag_Get_FlagsPFault() >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( (Diag_Get_FlagsPFault() >> 0u) & 0xFFu );
}



/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData10( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData10;
	CanVcuTx_hdr.DLC	= 8u;

	uint32	chg_kwh;
	uint32	dch_kwh;

	chg_kwh = SOC_Get_Chg_kWh();
	dch_kwh = SOC_Get_Dch_kWh();

	TxData[0]		= (uint8)( (chg_kwh >> 24u) & 0xFFu );
	TxData[1]		= (uint8)( (chg_kwh >> 16u) & 0xFFu );
	TxData[2]		= (uint8)( (chg_kwh >> 8u) & 0xFFu );
	TxData[3]		= (uint8)( (chg_kwh >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (dch_kwh >> 24u) & 0xFFu );
	TxData[5]		= (uint8)( (dch_kwh >> 16u) & 0xFFu );
	TxData[6]		= (uint8)( (dch_kwh >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( (dch_kwh >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData11( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData11;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( ((uint8)Diag_Get_ClassbFailcode() >> 0u) & 0xFFu );
	TxData[1]		= (uint8)( ((uint16)StlCntInterrupt.cntIntrCANRx >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( ((uint16)StlCntInterrupt.cntIntrCANRx >> 0u) & 0xFFu );
	TxData[3]		= 0u;
	TxData[4]		= 0u;
	TxData[5]		= 0u;
	TxData[6]		= (uint8)( (SOC_Get_SOC_Real() >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( (SOC_Get_SOC_Real() >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData12( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData12;
	CanVcuTx_hdr.DLC	= 8u;

	uint16	Fan_On_State2;
	uint16	Fan_On_State1;
	uint32	FCC;
	uint8	Fan_On_Num;

	FCC = SOC_Get_FCC();
	Fan_On_State2 = Adbms_Get_Fan_On_State2();
	Fan_On_State1 = Adbms_Get_Fan_On_State1();
	Fan_On_Num = Adbms_Get_Fan_On_Num();

	TxData[0]		= (uint8)( (Fan_On_State2 >> 0u) & 0xFFu );
	TxData[1]		= (uint8)( (Fan_On_State1 >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( (Fan_On_State1 >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (Fan_On_Num >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (FCC >> 24u) & 0xFFu );
	TxData[5]		= (uint8)( (FCC >> 16u) & 0xFFu );
	TxData[6]		= (uint8)( (FCC >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( (FCC >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData13( uint8 *TxData )
{
	uint32 FrontSPI_Error_Status;
	uint32 RearSPI_Error_Status;
	uint16 SOC_Temp;
	
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData13;
	CanVcuTx_hdr.DLC	= 8u;

	FrontSPI_Error_Status = Adbms6815_FrontSPI_Error_Check();
	RearSPI_Error_Status = Adbms6815_RearSPI_Error_Check();
	SOC_Temp = SOC_Get_SOC_Temp();

	TxData[0]		= (uint8)( (FrontSPI_Error_Status >> 16u) & 0xFFu );
	TxData[1]		= (uint8)( (FrontSPI_Error_Status >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( (FrontSPI_Error_Status >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (RearSPI_Error_Status >> 16u) & 0xFFu );
	TxData[4]		= (uint8)( (RearSPI_Error_Status >> 8u) & 0xFFu );
	TxData[5]		= (uint8)( (RearSPI_Error_Status >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( (SOC_Temp >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( (SOC_Temp >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData14( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData14;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( ((uint8)DetectTR_ThermalRunaway >> 0u) & 0xFFu );
	TxData[1]		= (uint8)( (TRunaway_Get_Trunaway_V_BMICNum() >> 0u) & 0xFFu );
	TxData[2]		= (uint8)( (TRunaway_Get_Trunaway_V_CellNum() >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (TRunaway_Get_Trunaway_T_BMICNum() >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (TRunaway_Get_Trunaway_T_CellNum() >> 0u) & 0xFFu );
	TxData[5]		= 0u;
	TxData[6]		= 0u;
	TxData[7]		= 0u;
}



/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_PackData15( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_PackData15;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( (Version >> 0u) & 0xFFu );
	TxData[1]		= (uint8)( (ClassB_Golden_CRC >> 24u) & 0xFFu );
	TxData[2]		= (uint8)( (ClassB_Golden_CRC >> 16u) & 0xFFu );
	TxData[3]		= (uint8)( (ClassB_Golden_CRC >> 8u) & 0xFFu );
	TxData[4]		= (uint8)( (ClassB_Golden_CRC >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[7]		= (uint8)( (0u >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ModeChange( uint8 *TxData )
{
	uint16 MLECID;

	MLECID = Dio_Get_BoardID();
	
	uint32 mid = ((uint32)MLECID << 12u);

	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_ModeChange + mid;
	CanVcuTx_hdr.DLC	= 8u;

	if		(ManaMsm_Get_System_State() == SYS_INIT)
	{
			TxData[0u] = 0u;
	}
	else if	(ManaMsm_Get_System_State() == SYS_CONFIGURATION)	
	{
		TxData[0u] = 1u;
	}
	else if	(ManaMsm_Get_System_State() == SYS_CALIBRATION)	
	{
		TxData[0u] = 2u;
	}
	else if	(ManaMsm_Get_System_State() == SYS_SELFTEST)	
	{
		TxData[0u] = 3u;
	}
	else if	(ManaMsm_Get_System_State() == SYS_LOGREAD)		
	{
		TxData[0u] = 4u;
	}
	else if	(ManaMsm_Get_System_State() == SYS_DETECTING_FCC)
	{
		TxData[0u] = 5u;
	}
	else
	{

	}
	TxData[1]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[2]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[7]		= (uint8)( (0u >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_RelayData( uint8 *TxData )
{
	uint16 MLECID;

	MLECID = Dio_Get_BoardID();
	
	uint32 mid = ((uint32)MLECID << 12u);

	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_RelayData + mid;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( (Relay_GetMainRelayCommand() >> 0u) & 0xFFu );
	TxData[1]		= (uint8)( (90u >> 0u) & 0xFFu );
	TxData[2]		= (uint8)( (91u >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (92u >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (93u >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (94u >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( (95u >> 0u) & 0xFFu );
	TxData[7]		= (uint8)( (96u >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ModuleData1( uint8 *TxData )
{
	uint16 CellV0[DEF_MAX_NUM_OF_BMIC];
	uint16 CellV1[DEF_MAX_NUM_OF_BMIC];
	uint16 CellV2[DEF_MAX_NUM_OF_BMIC];

	CellV0[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 0u);
	CellV1[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 1u);
	CellV2[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 2u);
	
	Set_BMIC_No();

	TxData[0]		= 0u;
	TxData[1]		= (uint8)( (CellV0[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( (CellV0[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (CellV1[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[4]		= (uint8)( (CellV1[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (CellV2[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[6]		= (uint8)( (CellV2[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[7]		= 0u;
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ModuleData2( uint8 *TxData )
{
	uint16 CellV3[DEF_MAX_NUM_OF_BMIC];
	uint16 CellV4[DEF_MAX_NUM_OF_BMIC];
	uint16 CellV5[DEF_MAX_NUM_OF_BMIC];

	CellV3[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 3u);
	CellV4[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 4u);
	CellV5[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 5u);
	
	Set_BMIC_No();

	TxData[0]		= 1u;
	TxData[1]		= (uint8)( (CellV3[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( (CellV3[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (CellV4[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[4]		= (uint8)( (CellV4[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (CellV5[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[6]		= (uint8)( (CellV5[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[7]		= 0u;
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ModuleData3( uint8 *TxData )
{
	uint16 CellV6[DEF_MAX_NUM_OF_BMIC];
	uint16 CellV7[DEF_MAX_NUM_OF_BMIC];
	uint16 CellV8[DEF_MAX_NUM_OF_BMIC];

	CellV6[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 6u);
	CellV7[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 7u);
	CellV8[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 8u);

	Set_BMIC_No();

	TxData[0]		= 2u;
	TxData[1]		= (uint8)( (CellV6[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( (CellV6[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (CellV7[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[4]		= (uint8)( (CellV7[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (CellV8[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[6]		= (uint8)( (CellV8[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[7]		= 0u;
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ModuleData4( uint8 *TxData )
{
	uint16 CellV9[DEF_MAX_NUM_OF_BMIC];
	uint16 CellV10[DEF_MAX_NUM_OF_BMIC];
	uint16 CellV11[DEF_MAX_NUM_OF_BMIC];

	CellV9[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 9u);
	CellV10[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 10u);
	CellV11[CAN_BmicNo] = Adbms_Get_CellVolt(CAN_BmicNo, 11u);
	
	Set_BMIC_No();

	TxData[0]		= 3u;
	TxData[1]		= (uint8)( (CellV9[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( (CellV9[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (CellV10[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[4]		= (uint8)( (CellV10[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (CellV11[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[6]		= (uint8)( (CellV11[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[7]		= 0u;
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ModuleData5( uint8 *TxData )
{
	uint16 CellMaxVolt[DEF_MAX_NUM_OF_BMIC];
	uint16 CellMinVolt[DEF_MAX_NUM_OF_BMIC];
	uint16 ModuleVolt[DEF_MAX_NUM_OF_BMIC];

	CellMaxVolt[CAN_BmicNo] = Adbms_Get_CellMaxVolt(CAN_BmicNo);
	CellMinVolt[CAN_BmicNo] = Adbms_Get_CellMinVolt(CAN_BmicNo);
	ModuleVolt[CAN_BmicNo]	= Adbms_Get_ModuleVolt(CAN_BmicNo);
		
	Set_BMIC_No();

	TxData[0]		= 4u;
	TxData[1]		= (uint8)( (CellMaxVolt[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( (CellMaxVolt[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (CellMinVolt[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[4]		= (uint8)( (CellMinVolt[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (ModuleVolt[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[6]		= (uint8)( (ModuleVolt[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[7]		= 0u;
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ModuleData6( uint8 *TxData )
{
	sint16 CellT0[DEF_MAX_NUM_OF_BMIC];
	sint16 CellT1[DEF_MAX_NUM_OF_BMIC];
	sint16 CellT2[DEF_MAX_NUM_OF_BMIC];
	
	CellT0[CAN_BmicNo] = Adbms_Get_CellTemp(CAN_BmicNo, 0u);
	CellT1[CAN_BmicNo] = Adbms_Get_CellTemp(CAN_BmicNo, 1u);
	CellT2[CAN_BmicNo] = Adbms_Get_CellTemp(CAN_BmicNo, 2u);

	Set_BMIC_No();

	TxData[0]		= 5u;
	TxData[1]		= (uint8)( ((uint16)CellT0[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( ((uint16)CellT0[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( ((uint16)CellT1[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[4]		= (uint8)( ((uint16)CellT1[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( ((uint16)CellT2[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[6]		= (uint8)( ((uint16)CellT2[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[7]		= 0u;
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ModuleData7( uint8 *TxData )
{
	sint16 CellT3[DEF_MAX_NUM_OF_BMIC];
	sint16 CellT4[DEF_MAX_NUM_OF_BMIC];
	sint16 CellT5[DEF_MAX_NUM_OF_BMIC];
	
	CellT3[CAN_BmicNo] = Adbms_Get_CellTemp(CAN_BmicNo, 3u);
	CellT4[CAN_BmicNo] = Adbms_Get_CellTemp(CAN_BmicNo, 4u);
	CellT5[CAN_BmicNo] = Adbms_Get_CellTemp(CAN_BmicNo, 5u);
	
	Set_BMIC_No();

	TxData[0]		= 6u;
	TxData[1]		= (uint8)( ((uint16)CellT3[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( ((uint16)CellT3[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( ((uint16)CellT4[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[4]		= (uint8)( ((uint16)CellT4[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( ((uint16)CellT5[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[6]		= (uint8)( ((uint16)CellT5[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[7]		= 0u;
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ModuleData8( uint8 *TxData )
{
	sint16 BoardTemp[DEF_MAX_NUM_OF_BMIC];
	sint16 CellMaxTemp[DEF_MAX_NUM_OF_BMIC];
	sint16 CellMinTemp[DEF_MAX_NUM_OF_BMIC];

	BoardTemp[CAN_BmicNo] = Adbms_Get_BoardTemp(CAN_BmicNo);
	CellMaxTemp[CAN_BmicNo] = Adbms_Get_CellMaxTemp(CAN_BmicNo);
	CellMinTemp[CAN_BmicNo] = Adbms_Get_CellMinTemp(CAN_BmicNo);
		
	Set_BMIC_No();

	TxData[0]		= 7u;
	TxData[1]		= (uint8)( ((uint16)CellMaxTemp[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( ((uint16)CellMaxTemp[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( ((uint16)CellMinTemp[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[4]		= (uint8)( ((uint16)CellMinTemp[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( ((uint16)BoardTemp[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[6]		= (uint8)( ((uint16)BoardTemp[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[7]		= 0u;
}

/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ModuleData9( uint8 *TxData )
{
	Set_BMIC_No();
	
	uint16  BalanceState[DEF_MAX_NUM_OF_BMIC];
	uint16	MalfCode[DEF_MAX_NUM_OF_BMIC];
	uint16	Bal_Status[DEF_MAX_NUM_OF_BMIC];
	uint16	BusbarVolt[DEF_MAX_NUM_OF_BMIC];
	
	BalanceState[CAN_BmicNo] = Adbms_Get_BalanceState(CAN_BmicNo);
	MalfCode[CAN_BmicNo] = Adbms_Get_MalfCode(CAN_BmicNo);
	Bal_Status[CAN_BmicNo] = CellBalance_Get_BalStatus(CAN_BmicNo);
	BusbarVolt[CAN_BmicNo] = Adbms_Get_BusbarVolt(CAN_BmicNo);

	TxData[0]		= 8u;
	TxData[1]		= (uint8)( (BalanceState[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[2]		= (uint8)( (BalanceState[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (MalfCode[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (Bal_Status[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[5]		= (uint8)( (Bal_Status[CAN_BmicNo] >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( (BusbarVolt[CAN_BmicNo] >> 8u) & 0xFFu );
	TxData[7]		= (uint8)( (BusbarVolt[CAN_BmicNo] >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_FaultRelease( uint8 *TxData )
{
	CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_FaultRelase;
	CanVcuTx_hdr.DLC	= 8u;

	TxData[0]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[1]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[2]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[3]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[4]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[5]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[6]		= (uint8)( (0u >> 0u) & 0xFFu );
	TxData[7]		= (uint8)( (0u >> 0u) & 0xFFu );
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_ConfigrationData( uint8 *TxData )
{
	uint32 addr = 0u;
	uint8 data[2u];
	uint8 mode = Config_Data[0u];
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	uint8 eep_data[2u];

	uint8 i;
	
	switch(mode)
	{
		case 0x00u : 		//Config Read
			addr = ((uint32)Config_Data[1u] << 16u) + ((uint32)Config_Data[2u] << 8u) + (uint32)Config_Data[3u] ;		
			if(Can_InOutParameters(addr, get, (char *)data, REAL) )
			{
				Config_Data[4u] = data[1u];
				Config_Data[5u] = data[0u];

				Set_ConfigrationData();

				for(i=0u; i<8u; i++)
				{
					TxData[i] = Config_Data[i];
				}
			}
			else
			{

			}
			break;
/*			
		case 0x01u : 		 
			addr = ((uint32)Config_Data[1u] << 16u) + ((uint32)Config_Data[2u] << 8u) + (uint32)Config_Data[3u] ;
			data[0u] = Config_Data[5u];
			data[1u] = Config_Data[4u];

			Set_ConfigrationData();

			TxData[0u] = 0x01u;
			TxData[1u] = Config_Data[1u];
			TxData[2u] = Config_Data[2u];
			TxData[3u] = Config_Data[3u];
			TxData[4u] = data[1u];
			TxData[5u] = data[0u];
			TxData[6u] = 0x00u ;
			TxData[7u] = 0x00u ;

			if(Can_InOutParameters(addr, set, (char *)data, VIRTUAL))
			{
			    Can_ConfigSave_DownCount = CONFIG_UPDATE_TIME;
			}           
			break;
*/			
		case 0x02u : 		// Time Set 					

			(void)memset(&sTime, 0, sizeof(sTime));
			(void)memset(&sDate, 0, sizeof(sDate));
			
			sDate.Year = Config_Data[1u];
			sDate.Month = Config_Data[2u];
			sDate.Date = Config_Data[3u];
			sTime.Hours = Config_Data[4u];
			sTime.Minutes = Config_Data[5u];
			sTime.Seconds = Config_Data[6u];

			(void)HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			(void)HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

			Set_ConfigrationData();
			
			for(i=0u; i<8u; i++)
			{
				TxData[i] = Config_Data[i];
			}
			
			break;
			
		case 0x03u : 		// Time Get
			(void)HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			(void)HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);	

			Set_ConfigrationData();
		
			TxData[0u] = 0x03u ; 
			TxData[1u] = sDate.Year; 
			TxData[2u] = sDate.Month;
			TxData[3u] = sDate.Date;
			TxData[4u] = sTime.Hours;
			TxData[5u] = sTime.Minutes;
			TxData[6u] = sTime.Seconds;
			TxData[7u] = 0x00u ; 
			break;	
			
		case 0x04u:			// ocv read 											
			break;
			
		case 0x05u:			// ocv write
			break;
			
		case 0xFFu:
			eep_data[0u]= 0x55u;
			eep_data[1u] = 0x55u;
			if (EepromEx_WriteData(10u, 2u, eep_data) != FALSE)
			{
				while(DEF_True)	// reset system
				{

				}
			}	
			break;
			
		default:
			break;
	}		

}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_CalibrateData( uint8 *TxData )
{

	uint8 cal_no = Calibrate_Data[0u] ; 		
		float gain_buf = 0.0f ;
		sint32  offset_buf = 0 ;
		sint32	S32Data;
		float	fdata;
		
		switch(cal_no)
		{
			case 0x00u: 		// ad raw data

				Set_CalibrationData();
					
				if((Calibrate_Data[1u] == 0x00u) || (Calibrate_Data[1u] == 0x01u)) 
				{
					if(ManaMsm_Get_System_State() == SYS_CALIBRATION)
					{
						S32Data = Curr_GetCalibValueLow() / 10;
						TxData[2u] = (uint8)((uint16)S32Data >> 8u);
						TxData[3u] = (uint8)((uint16)S32Data & 0x00FFu);
					}
					else
					{
						S32Data = Curr_GetCurrUnfiltered() / 100;
						TxData[2u] = (uint8)((uint16)S32Data >> 8u);
						TxData[3u] = (uint8)((uint16)S32Data & 0x00FFu);
					}
					
					TxData[4u] = (uint8)(Curr_GetADRawLow() >> 8u);
					TxData[5u] = (uint8)(Curr_GetADRawLow() & 0x00FFu);
				}
				else if((Calibrate_Data[1u] == 0x02u) || (Calibrate_Data[1u] == 0x03u))
				{
					if(ManaMsm_Get_System_State() == SYS_CALIBRATION)
					{				
						S32Data = Curr_GetCalibValueHigh() / 10;
						TxData[2u] = (uint8)((uint16)S32Data >> 8u);
						TxData[3u] = (uint8)((uint16)S32Data & 0x00FFu);
					}
					else
					{
						S32Data = Curr_GetCurrUnfiltered() / 100;
						TxData[2u] = (uint8)((uint16)S32Data >> 8u);
						TxData[3u] = (uint8)((uint16)S32Data & 0x00FFu);
					}				
					TxData[4u] = (uint8)(Curr_GetADRawHigh() >> 8u);
					TxData[5u] = (uint8)(Curr_GetADRawHigh() & 0x00FFu);
				}
				else if(Calibrate_Data[1u] == 0x04u)
				{
					TxData[2u] = (uint8)(PackVolt_Get_V_PackMeas() >> 8u) ;
					TxData[3u] = (uint8)(PackVolt_Get_V_PackMeas() & 0x00FFu) ;
				}				
				else if(Calibrate_Data[1u] == 0x05u)
				{
				}							
				else if(Calibrate_Data[1u] == 0x06u)
				{
					TxData[2u] = (uint8)(PackVolt_Get_V_BusMeas() >> 8u) ;
					TxData[3u] = (uint8)(PackVolt_Get_V_BusMeas() & 0x00FFu) ;
				}				
				else if(Calibrate_Data[1u] == 0x07u)
				{
					TxData[2] = (uint8)(Ref5V_GetCalculatedValue() >> 8u) ;
					TxData[3] = (uint8)(Ref5V_GetCalculatedValue() & 0x00FFu) ;
				}				
				else if((Calibrate_Data[1u] == 0x08u) || (Calibrate_Data[1u] == 0x09u)) 
				{
					if(ManaMsm_Get_System_State() == SYS_CALIBRATION)
					{
						S32Data = Curr_GetCalibValueLow_Sub() / 10;
						TxData[2u] = (uint8)((uint16)S32Data >> 8u);
						TxData[3u] = (uint8)((uint16)S32Data & 0x00FFu);
					}
					else
					{
						S32Data = Curr_GetCurrUnfiltered_Sub() / 100;
						TxData[2u] = (uint8)((uint16)S32Data >> 8u);
						TxData[3u] = (uint8)((uint16)S32Data & 0x00FFu);
					}
					TxData[4u] = (uint8)(Curr_GetADRawLow_Sub() >> 8u);
					TxData[5u] = (uint8)(Curr_GetADRawLow_Sub() & 0x00FFu);
				}
				else if((Calibrate_Data[1u] == 0x0Au) || (Calibrate_Data[1u] == 0x0Bu)) 
				{
					if(ManaMsm_Get_System_State() == SYS_CALIBRATION)
					{		
						S32Data = Curr_GetCalibValueHigh_Sub() / 10;
						TxData[2u] = (uint8)((uint16)S32Data >> 8u);
						TxData[3u] = (uint8)((uint16)S32Data & 0x00FFu);
					}
					else
					{
						S32Data = Curr_GetCurrUnfiltered_Sub() / 100;
						TxData[2u] = (uint8)((uint16)S32Data >> 8u);
						TxData[3u] = (uint8)((uint16)S32Data & 0x00FFu);
					}				
					TxData[4u] = (uint8)(Curr_GetADRawHigh_Sub() >> 8u);
					TxData[5u] = (uint8)(Curr_GetADRawHigh_Sub() & 0x00FFu);
				}				
				else 
				{
	
				}
				*(TxData) = *(Calibrate_Data);
				TxData[1u] = Calibrate_Data[1u];
				//*(TxData + 4u) = 0x00u;		*(TxData + 5u) = 0x00u;
				TxData[6u] = 0x00u;		TxData[7u] = 0x00u;				
				break;
				
			case 0x01u: 		// read
				Set_CalibrationData();

				Calib_Get_Calibdata(&CANVcu_Calibdata);
				
				if(Calibrate_Data[1u] == 0x00u)
				{
					gain_buf = CANVcu_Calibdata.CurrentSensor_1.LowGain_Plus; 
					offset_buf = CANVcu_Calibdata.CurrentSensor_1.LowOffset_Plus;
				}
				else if(Calibrate_Data[1u] == 0x01u)
				{
					gain_buf = CANVcu_Calibdata.CurrentSensor_1.LowGain_Minus; 
					offset_buf = CANVcu_Calibdata.CurrentSensor_1.LowOffset_Minus;						
				}
				else if(Calibrate_Data[1u] == 0x02u)
				{
					gain_buf = CANVcu_Calibdata.CurrentSensor_1.HighGain_Plus; 
					offset_buf = CANVcu_Calibdata.CurrentSensor_1.HighOffset_Plus;						
				}
				else if(Calibrate_Data[1u] == 0x03u)
				{
					gain_buf = CANVcu_Calibdata.CurrentSensor_1.HighGain_Minus; 
					offset_buf = CANVcu_Calibdata.CurrentSensor_1.HighOffset_Minus;											
				}
				else if(Calibrate_Data[1u] == 0x04u)
				{
					gain_buf = CANVcu_Calibdata.Pack1V_Gain;
					fdata = CANVcu_Calibdata.Pack1V_Offset * 100.f * PACKVOLT_RESOLUTION;
					offset_buf = (sint16)fdata;
				}				
				else if(Calibrate_Data[1u] == 0x05u)
				{
					gain_buf = CANVcu_Calibdata.Pack2V_Gain;
					fdata = CANVcu_Calibdata.Pack2V_Offset * 100.f * PACKVOLT_RESOLUTION;
					offset_buf = (sint16)fdata;
				}
				else if(Calibrate_Data[1u] == 0x06u)
				{
					gain_buf = CANVcu_Calibdata.BusV_Gain;
					fdata = CANVcu_Calibdata.BusV_Offset * 100.f * PACKVOLT_RESOLUTION;
					offset_buf = (sint16)fdata;
				}				
				else if(Calibrate_Data[1u] == 0x07u)
				{
					gain_buf = CANVcu_Calibdata.Ref5V_Gain;
					fdata = CANVcu_Calibdata.Ref5V_Offset * 100.f * REF5V_RESOLUTION;
					offset_buf = (sint16)fdata;
				}		
				else if(Calibrate_Data[1u] == 0x08u)
				{
					gain_buf = CANVcu_Calibdata.CurrentSensor_2.LowGain_Plus; 
					offset_buf = CANVcu_Calibdata.CurrentSensor_2.LowOffset_Plus;
				}
				else if(Calibrate_Data[1u] == 0x09u)
				{
					gain_buf = CANVcu_Calibdata.CurrentSensor_2.LowGain_Minus; 
					offset_buf = CANVcu_Calibdata.CurrentSensor_2.LowOffset_Minus;						
				}
				else if(Calibrate_Data[1u] == 0x0Au)
				{
					gain_buf = CANVcu_Calibdata.CurrentSensor_2.HighGain_Plus; 
					offset_buf = CANVcu_Calibdata.CurrentSensor_2.HighOffset_Plus;						
				}
				else if(Calibrate_Data[1u] == 0x0Bu)
				{
					gain_buf = CANVcu_Calibdata.CurrentSensor_2.HighGain_Minus; 
					offset_buf = CANVcu_Calibdata.CurrentSensor_2.HighOffset_Minus;											
				}			
				else
				{
	
				}
			
				*(TxData) = *(Calibrate_Data);
				TxData[1u] = Calibrate_Data[1u];
				fdata = gain_buf * (float)1000.;
				TxData[2u] = (uint8)((uint16)fdata >> 8u);
				TxData[3u] = (uint8)((uint16)fdata & 0x00FFu);
				TxData[4u] = (uint8)((uint16)(offset_buf) >> 8u);
				TxData[5u] = (uint8)((uint16)(offset_buf) & 0x00FFu);
				TxData[6u] = 0x00u;		
				TxData[7u] = 0x00u;
				break;
				
			default:	break;
		}
}


/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_EventData1( uint8 *TxData )
{
	uint8  event_no  = Event_Data[0u];		
	uint16 r_index;
	uint16 r_index2;
	uint8  e_buf[16u]; 
	uint8  e_buf2[12u]; 
	uint16 temp_eventidx;

	uint8 i;
	
	switch(event_no)
	{
		case 0u :			// Index Read
			Set_EventData();
		
			temp_eventidx = EepromEx_GetEventIndex();
			
			TxData[0u] = 0u;
			TxData[1u] = (uint8)(temp_eventidx >> 8u);
			TxData[2u] = (uint8)(temp_eventidx & 0x00ffu);
			break;
			
		case 1u: 			// Index Write
			Set_EventData();
		
			temp_eventidx = ((uint16)Event_Data[1u] << 8u) + (uint16)Event_Data[2u];
			EepromEx_SetEventIndex(temp_eventidx);
		
			EepromEx_Set_SaveEventIdxFlag(TRUE);
		
			for(i=0u; i<8u; i++)
			{
				TxData[i] = Event_Data[i];
			}
			break;
			
		case 2u: 			// Log Read
			Set_EventData();
		
			r_index = ((uint16)Event_Data[1u] << 8u) + (uint16)Event_Data[2u];
			EepromEx_GetEventLog(r_index, (uint8 *)e_buf);
			TxData[0u] = 2u;
			TxData[1u] = 0u;
			TxData[2u] = e_buf[0u];
			TxData[3u] = e_buf[1u];
			TxData[4u] = e_buf[2u];
			TxData[5u] = e_buf[3u];
			TxData[6u] = e_buf[4u];
			TxData[7u] = e_buf[5u];
			break;

		case 3u :			// Index Read
			Set_EventData();
			
			temp_eventidx = EepromEx_GetPEventIndex();
			
			TxData[0u] = 0u;
			TxData[1u] = (uint8)(temp_eventidx >> 8u);
			TxData[2u] = (uint8)(temp_eventidx & 0x00ffu);
	
			break;
			
		case 4u: 			// Index Write
			Set_EventData();
			
			temp_eventidx = ((uint16)Event_Data[1u] << 8u) + (uint16)Event_Data[2u];
			EepromEx_SetPEventIndex(temp_eventidx);

			EepromEx_Set_SavePEventIdxFlag(TRUE);
		
			for(i=0u; i<8u; i++)
			{
				TxData[i] = Event_Data[i];
			}
			break;

		case 5u: 			// Log Read
			Set_EventData();
			
			r_index2 = ((uint16)Event_Data[1u] << 8u) + (uint16)Event_Data[2u];
			EepromEx_GetPEventLog(r_index2, (uint8 *)e_buf2);
			TxData[0u] = 3u;
			TxData[1u] = 0u;
			TxData[2u] = e_buf2[0u];
			TxData[3u] = e_buf2[1u];
			TxData[4u] = e_buf2[2u];
			TxData[5u] = e_buf2[3u];
			TxData[6u] = e_buf2[4u];
			TxData[7u] = e_buf2[5u];
			break;
			
		default :	
			break;
	}
}

void	CanVcuTx_Set_EventData2( uint8 *TxData )
{
	uint8  event_no  = Event_Data[0u];		
	uint16 r_index ;
	uint16 r_index2;
	uint8  e_buf[16u]; 
	uint8  e_buf2[12u]; 
	
	if(event_no == 2u)
	{
		Set_EventData();
		
		r_index = ((uint16)Event_Data[1u] << 8u) + (uint16)Event_Data[2u];
		EepromEx_GetEventLog(r_index, (uint8 *)e_buf);

		TxData[0u] = 2u;
		TxData[1u] = 1u;
		TxData[2u] = e_buf[6u];
		TxData[3u] = e_buf[7u];
		TxData[4u] = e_buf[8u];
		TxData[5u] = e_buf[9u];
		TxData[6u] = e_buf[10u];
		TxData[7u] = e_buf[11u];
	}

	else if(event_no == 5u)
	{
		Set_EventData();
			
		r_index2 = ((uint16)Event_Data[1u] << 8u) + (uint16)Event_Data[2u];
		EepromEx_GetPEventLog(r_index2, (uint8 *)e_buf2);

		TxData[0u] = 3u;
		TxData[1u] = 1u;
		TxData[2u] = e_buf2[6u];
		TxData[3u] = e_buf2[7u];
		TxData[4u] = e_buf2[8u];
		TxData[5u] = e_buf2[9u];
		TxData[6u] = e_buf2[10u];
		TxData[7u] = e_buf2[11u];
	}
	else
	{
	}
}


void	CanVcuTx_Set_EventData3( uint8 *TxData )
{
	uint8  event_no  = Event_Data[0u];		
	uint16 r_index;
	uint8  e_buf[16u]; 

	if(event_no == 2u)
	{
		Set_EventData();
		
		r_index = ((uint16)Event_Data[1u] << 8u) + (uint16)Event_Data[2u];
		EepromEx_GetEventLog(r_index, (uint8 *)e_buf);
		TxData[0u] = 2u;
		TxData[1u] = 2u;
		TxData[2u] = e_buf[12u];
		TxData[3u] = e_buf[13u];
		TxData[4u] = e_buf[14u];
		TxData[5u] = e_buf[15u];
		TxData[6u] = 0x00u ; 
		TxData[7u] = 0x00u ;
	}
}



/**
* @brief	Can TX to User Interface
* @param	void
* @return	CanUiTx_Data_t
*/
void	CanVcuTx_Set_DetectFCCData( uint8 *TxData )
{
	uint8 Detect_No = DetectFCC_Data[0u];
	uint8 String_No;
	uint8 Data_No;
	uint8 i;
	uint16	u16data;
	
	switch(Detect_No)
	{
		case 0u:					// read
			String_No = DetectFCC_Data[1u];
			Data_No = DetectFCC_Data[2u];
		
			SOC_Get_CapacityData(&CAN_CapacityData);
			
			if(String_No == 0u)
			{
				uint16 temp_data;
				float fdata;
				
				switch(Data_No)
				{
					case 0u:
						DetectFCC_Data[6u] = (uint8)(CAN_CapacityData.Cycle_Count >> 8u);
						DetectFCC_Data[7u] = (uint8)(CAN_CapacityData.Cycle_Count & 0xFFu);
						break;
					case 1u:
						DetectFCC_Data[4u] = (uint8)(CAN_CapacityData.DchCntIntegral >> 24u);
						DetectFCC_Data[5u] = (uint8)(CAN_CapacityData.DchCntIntegral >> 16u);					
						DetectFCC_Data[6u] = (uint8)(CAN_CapacityData.DchCntIntegral >> 8u);
						DetectFCC_Data[7u] = (uint8)(CAN_CapacityData.DchCntIntegral & 0xFFu);					
						break;
					case 2u:
						DetectFCC_Data[6u] = (uint8)(CAN_CapacityData.Complete_Init_FCC >> 8u);
						DetectFCC_Data[7u] = (uint8)(CAN_CapacityData.Complete_Init_FCC & 0xFFu);						
						break;
					case 3u:
						DetectFCC_Data[4u] = (uint8)(CAN_CapacityData.Init_FCC >> 24u);
						DetectFCC_Data[5u] = (uint8)(CAN_CapacityData.Init_FCC >> 16u);					
						DetectFCC_Data[6u] = (uint8)(CAN_CapacityData.Init_FCC >> 8u);
						DetectFCC_Data[7u] = (uint8)(CAN_CapacityData.Init_FCC & 0xFFu);
						break;
					case 4u:
						DetectFCC_Data[4u] = (uint8)(CAN_CapacityData.FCC >> 24u);
						DetectFCC_Data[5u] = (uint8)(CAN_CapacityData.FCC >> 16u);										
						DetectFCC_Data[6u] = (uint8)(CAN_CapacityData.FCC >> 8u);
						DetectFCC_Data[7u] = (uint8)(CAN_CapacityData.FCC & 0xFFu);
						break;

					case 5u:
						DetectFCC_Data[4u] = (uint8)(CAN_CapacityData.Capacity >> 24u);
						DetectFCC_Data[5u] = (uint8)(CAN_CapacityData.Capacity >> 16u);					
						DetectFCC_Data[6u] = (uint8)(CAN_CapacityData.Capacity >> 8u);
						DetectFCC_Data[7u] = (uint8)(CAN_CapacityData.Capacity & 0xFFu);
						break;						
					case 6u:
						DetectFCC_Data[4u] = (uint8)(CAN_CapacityData.Chg_kWh >> 24u);
						DetectFCC_Data[5u] = (uint8)(CAN_CapacityData.Chg_kWh >> 16u);
						DetectFCC_Data[6u] = (uint8)(CAN_CapacityData.Chg_kWh >> 8u);
						DetectFCC_Data[7u] = (uint8)(CAN_CapacityData.Chg_kWh & 0xFFu);						
						break;
					case 7u:
						DetectFCC_Data[4u] = (uint8)(CAN_CapacityData.Dch_kWh >> 24u);
						DetectFCC_Data[5u] = (uint8)(CAN_CapacityData.Dch_kWh >> 16u);
						DetectFCC_Data[6u] = (uint8)(CAN_CapacityData.Dch_kWh >> 8u);
						DetectFCC_Data[7u] = (uint8)(CAN_CapacityData.Dch_kWh & 0xFFu);						
						break;
					case 8u:
						DetectFCC_Data[6u] = (uint8)(CAN_CapacityData.Chg_Wh >> 8u);
						DetectFCC_Data[7u] = (uint8)(CAN_CapacityData.Chg_Wh & 0xFFu);
						break;

					case 9u:
						DetectFCC_Data[6u] = (uint8)(CAN_CapacityData.Dch_Wh >> 8u);
						DetectFCC_Data[7u] = (uint8)(CAN_CapacityData.Dch_Wh & 0xFFu);
						break;												
					case 0x0Au:
						DetectFCC_Data[6u] = (uint8)((uint16)Curr_Get_I_PackMeas() >> 8u);
						DetectFCC_Data[7u] = (uint8)((uint16)Curr_Get_I_PackMeas() & 0xFFu);
						break;					
					case 0x0Bu:
						fdata = (float)PackVolt_Get_V_PackMeas() / (float)2.4414;
						temp_data = (uint16)fdata;
						DetectFCC_Data[6u] = (uint8)(temp_data >> 8u);
						DetectFCC_Data[7u] = (uint8)(temp_data & 0xFFu);					
						break;
					case 0x0Cu:
						fdata = (float)PackVolt_Get_V_BusMeas() / (float)2.4414;
						temp_data = (uint16)fdata;
						DetectFCC_Data[6u] = (uint8)(temp_data >> 8u);
						DetectFCC_Data[7u] = (uint8)(temp_data & 0xFFu);					
						break;
					case 0x0Du:
						DetectFCC_Data[6u] = (uint8)(SOC_Get_SOC_Real() >> 8u);
						DetectFCC_Data[7u] = (uint8)(SOC_Get_SOC_Real() & 0xFFu);				
						break;
					case 0x0Eu:
						DetectFCC_Data[6u] = (uint8)(SOC_Get_CV_SOC() >> 8u);
						DetectFCC_Data[7u] = (uint8)(SOC_Get_CV_SOC() & 0xFFu);
						break;
					case 0x0Fu:
						fdata = (float)Adbms_Get_V_CellAvg() / (float)2.4414;
						temp_data = (uint16)fdata;
						DetectFCC_Data[6u] = (uint8)(temp_data >> 8u);
						DetectFCC_Data[7u] = (uint8)(temp_data & 0xFFu);
						break;					
					case 0x10u:
						fdata = (float)IR_Get_RealCellV() / (float)2.4414;
						temp_data = (uint16)fdata;
						DetectFCC_Data[6u] = (uint8)(temp_data >> 8u);
						DetectFCC_Data[7u] = (uint8)(temp_data & 0xFFu);
						break;
					case 0x11u:
						DetectFCC_Data[6u] = (uint8)(IR_Get_CV_IR() >> 8u);
						DetectFCC_Data[7u] = (uint8)(IR_Get_CV_IR() & 0xFFu);
						break;						
					case 0x12u:
						DetectFCC_Data[6u] = (uint8)((uint16)CurrLimit_Get_ChgCurrLimit() >> 8u);
						DetectFCC_Data[7u] = (uint8)((uint16)CurrLimit_Get_ChgCurrLimit() & 0xFFu);	
						break;
					case 0x13u:
						DetectFCC_Data[6u] = (uint8)((uint16)CurrLimit_Get_DchCurrLimit() >> 8u);
						DetectFCC_Data[7u] = (uint8)((uint16)CurrLimit_Get_DchCurrLimit() & 0xFFu);	
						break;					
					case 0x14u:
						DetectFCC_Data[7u] = (uint8)(Relay_Get_Relay_Status());
						break;
					default:	break;
				}				
			}
			else
			{
				
			}

			Set_DetectFCCData();
			
			for(i=0u; i<8u; i++)
			{
				TxData[i] = DetectFCC_Data[i];
			}
			break;
			
		case 1u:					// Write
			String_No = DetectFCC_Data[1u];
			Data_No = DetectFCC_Data[2u];		

			if(String_No == 0u)
			{
				switch(Data_No)
				{
					case 0u:	CAN_CapacityData.Cycle_Count = ((uint16)DetectFCC_Data[6u] << 8u) + (uint16)DetectFCC_Data[7u];	break;
					case 1u:	CAN_CapacityData.DchCntIntegral = ((uint32)DetectFCC_Data[6u] << 8u) + (uint32)DetectFCC_Data[7u];	break;
					case 2u:	CAN_CapacityData.Complete_Init_FCC = ((uint16)DetectFCC_Data[6] << 8u) + (uint16)DetectFCC_Data[7u];	break;
					case 3u:	CAN_CapacityData.Init_FCC = ((uint32)DetectFCC_Data[4u] << 24u) + ((uint32)DetectFCC_Data[5u] << 16u) 
					       						  + ((uint32)DetectFCC_Data[6u] <<  8u) + (uint32)DetectFCC_Data[7u]; break;
					case 4u:	CAN_CapacityData.FCC = ((uint32)DetectFCC_Data[4u] << 24u) + ((uint32)DetectFCC_Data[5u] << 16u) 
					       						  + ((uint32)DetectFCC_Data[6u] <<  8u) + (uint32)DetectFCC_Data[7u]; break;
					case 5u:	CAN_CapacityData.Capacity = ((uint32)DetectFCC_Data[4u] << 24u) + ((uint32)DetectFCC_Data[5u] << 16u) 
					       						  + ((uint32)DetectFCC_Data[6u] <<  8u) + (uint32)DetectFCC_Data[7u]; break;
					case 6u:	CAN_CapacityData.Chg_kWh = ((uint32)DetectFCC_Data[4u] << 24u) + ((uint32)DetectFCC_Data[5u] << 16u) + 
												  ((uint32)DetectFCC_Data[6u] << 8u) + (uint32)DetectFCC_Data[7u];	break;
					case 7u:	CAN_CapacityData.Dch_kWh = ((uint32)DetectFCC_Data[4] << 24u) + ((uint32)DetectFCC_Data[5u] << 16u) + 
												  ((uint32)DetectFCC_Data[6u] << 8u) + (uint32)DetectFCC_Data[7u];	break;												  
					case 8u:	CAN_CapacityData.Chg_Wh = ((uint16)DetectFCC_Data[6u] << 8u) + (uint16)DetectFCC_Data[7u];	break;
					case 9u:	CAN_CapacityData.Dch_Wh = ((uint16)DetectFCC_Data[6u] << 8u) + (uint16)DetectFCC_Data[7u];	break;
					case 0x11u:	u16data = ((uint16)DetectFCC_Data[6u] << 8u) + (uint16)DetectFCC_Data[7u];
					IR_Set_CV_IR(u16data);

					SOC_Set_CapacityData(&CAN_CapacityData);
		
					EepromEx_SetUpdateCapacityData_Flag(TRUE);
					break;
					
					default:	break;
				}

				Set_DetectFCCData();
				
				for(i=0u; i<8u; i++)
				{
					TxData[i] = DetectFCC_Data[i];
				}
			}
			else
			{
			}
			
			break;
		case 2u:					// Control
			break;
		default:	break;
	}

}



void	Set_BMIC_No(void)
{
	uint16 MLECID;
	uint16 NumOf_Bmic;

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	MLECID = Dio_Get_BoardID();
	
	uint32 mid = ((uint32)MLECID << 12u);

	if(CAN_BmicNo < NumOf_Bmic)
	{
		if (CAN_BmicNo >= 32u)	
		{
			CanVcuTx_hdr.ExtId = APP_CanVcuTx_Id_ModuleData3 + mid + (uint16)((CAN_BmicNo - 32u) << 8u);
		}
		else if(CAN_BmicNo >= 16u)	
		{
			CanVcuTx_hdr.ExtId = APP_CanVcuTx_Id_ModuleData2 + mid + (uint16)((CAN_BmicNo - 16u) << 8u);
		}
		else			
		{
			CanVcuTx_hdr.ExtId = APP_CanVcuTx_Id_ModuleData1 + mid +  (uint16)(CAN_BmicNo << 8u);	
		}
	}

	CanVcuTx_hdr.DLC	= 8u;
}


void	Set_ConfigrationData(void)
{
	uint16 MLECID;

	MLECID = Dio_Get_BoardID();
	
	uint32 mid = ((uint32)MLECID << 12u);

	CanVcuTx_hdr.ExtId = APP_CanVcuTx_Id_Configdata + mid;
	CanVcuTx_hdr.DLC	= 8u;
}


void	Set_CalibrationData(void)
{
	uint16 MLECID;

	MLECID = Dio_Get_BoardID();
	
	uint32 mid = ((uint32)MLECID << 12u);

	CanVcuTx_hdr.ExtId = APP_CanVcuTx_Id_Calibdata + mid;
	CanVcuTx_hdr.DLC	= 8u;
}


void	Set_EventData(void)
{
	uint16 MLECID;

	MLECID = Dio_Get_BoardID();
	
	uint32 mid = ((uint32)MLECID << 12u);

	CanVcuTx_hdr.ExtId = APP_CanVcuTx_Id_Eventdata + mid;
	CanVcuTx_hdr.DLC	= 8u;
}


void	Set_DetectFCCData(void)
{
	uint16 MLECID;

	MLECID = Dio_Get_BoardID();
	
	uint32 mid = ((uint32)MLECID << 12u);

	CanVcuTx_hdr.ExtId = APP_CanVcuTx_Id_DetectFCCdata + mid;
	CanVcuTx_hdr.DLC	= 8u;
}


/**
* @brief Read / Write Parameter data 
* @param addr - Parameter address 
* @param bg - Get:Read, Set:Write
* @param *tbuf - Read / Write Data
* @return TRUE:Success, FALSE:Fail
*/
bool Can_InOutParameters(uint32 addr, BUF_INOUT bg, char* tbuf, bool real_vir)
{
	char* temp = Get_ParameterAddress();	
    uint16 param_addr;
	uint32 address = addr;
	bool result = TRUE;

    if(address < (EEPROMEX_BLOCK5_ADDR_OFFSET + EEPROMEX_BLOCK5_DATA_QTY_BYTE + CRC_BYTE))
    {
    	if (real_vir == VIRTUAL)
    	{
//    		temp = (char*)&Can_temp_Parameter;
    	}
    	else
    	{
    		temp = Get_ParameterAddress();
    	}
    }    
    else if(address >= WARRANTY_EEP_ADDR)
    {
    	if (real_vir == VIRTUAL)
    	{    
            temp = Get_WarrantyTempAddress();        
        }
        else
        {
            temp = Get_WarrantyAddress();        
        }        
    }
    else        
    {
        result = FALSE;        
    }

    if(result == TRUE)
    {
        if(address >= WARRANTY_EEP_ADDR)
        {
            address = address - WARRANTY_EEP_ADDR;        
        }
        else if	(address >= EEPROMEX_BLOCK5_ADDR_OFFSET)
        {
        	address -= EEPROMEX_BLOCK5_ADDR_OFFSET;
        	address = address + EEPROMEX_BLOCK1_DATA_QTY_BYTE + CRC_BYTE
        				+ EEPROMEX_BLOCK2_DATA_QTY_BYTE + CRC_BYTE
        				+ EEPROMEX_BLOCK3_DATA_QTY_BYTE + CRC_BYTE
        				+ EEPROMEX_BLOCK4_DATA_QTY_BYTE + CRC_BYTE;
        }
        else if (address >= EEPROMEX_BLOCK4_ADDR_OFFSET)
        {
        	address -= EEPROMEX_BLOCK4_ADDR_OFFSET;
        	address = address + EEPROMEX_BLOCK1_DATA_QTY_BYTE + CRC_BYTE
        				+ EEPROMEX_BLOCK2_DATA_QTY_BYTE + CRC_BYTE
        				+ EEPROMEX_BLOCK3_DATA_QTY_BYTE + CRC_BYTE;
        }
        else if (address >= EEPROMEX_BLOCK3_ADDR_OFFSET)
        {
        	address -= EEPROMEX_BLOCK3_ADDR_OFFSET;
        	address = address + EEPROMEX_BLOCK1_DATA_QTY_BYTE + CRC_BYTE
        				+ EEPROMEX_BLOCK2_DATA_QTY_BYTE + CRC_BYTE;
        }	
        else if (address >= EEPROMEX_BLOCK2_ADDR_OFFSET)
        {
        	address -= EEPROMEX_BLOCK2_ADDR_OFFSET;
        	address = address + EEPROMEX_BLOCK1_DATA_QTY_BYTE + CRC_BYTE;
        }	
        else if (address >= EEPROMEX_BLOCK1_ADDR_OFFSET)
        {
        	address -= EEPROMEX_BLOCK1_ADDR_OFFSET;
        }
        else
        {
        	result = FALSE;
        }
        
        param_addr = (uint16)address;

        if (bg == get)
        {	
            tbuf[0u] = temp[param_addr];
            tbuf[1u] = temp[param_addr + 1u];
        }
        else
        {
            temp[param_addr] 	 = tbuf[0u];
            temp[param_addr + 1u] = tbuf[1u];		
        }
    }
	
	return result;
}


void	CanVcuTx_Set_Debug_En( uint8 msgIdx )
{
	if (msgIdx < APP_CanVcuTx_ReqMsg_Num)
	{
		CanVcuTx_ReqMsg[msgIdx].Send_f = DEF_True;
	}
	else{}
}


bool	CanVcuTx_Get_Debug_En( uint8 msgIdx )
{
	if (msgIdx < APP_CanVcuTx_ReqMsg_Num)
	{
		return CanVcuTx_ReqMsg[msgIdx].Send_f;
	}
	else
	{
		return DEF_False;
	}
}


void CanVcuTx_Send_Test_Message(void)
{
    
    CAN_TxHeaderTypeDef TxHeader;
    uint8 TxData[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    uint32 TxMailbox;

    TxHeader.StdId = 0x123;            // 11-bit Standard ID
    TxHeader.ExtId = 0x01;             // Extended ID (사용 안 하면 무시)
    TxHeader.RTR = CAN_RTR_DATA;       // Data Frame
    TxHeader.IDE = CAN_ID_STD;         // 표준 ID 모드
    TxHeader.DLC = 8;                  // 데이터 길이 (0~8)
    TxHeader.TransmitGlobalTime = DISABLE;
    
    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) == HAL_OK)
    {
        // 전송 실패 시 재시도 또는 에러 처리
    }

}
