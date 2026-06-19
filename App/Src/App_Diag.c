/*
 * App_Diag.c
 *
 *  Created on: Sep 22, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include <App_Diag.h>


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define DIAG_B_V_OVER_CELL				    0x01UL
#define DIAG_B_V_OVER_PACK				    0x02UL
#define DIAG_B_V_UNDER_CELL				    0x04UL
#define DIAG_B_V_UNDER_PACK				    0x08UL
#define	DIAG_B_T_OVER_CELL_CHG			    0x10UL
#define	DIAG_B_T_OVER_CELL_DCH			    0x20UL
#define	DIAG_B_T_UNDER_CELL_CHG			    0x40UL
#define	DIAG_B_T_UNDER_CELL_DCH			    0x80UL
#define DIAG_B_I_OVER_CHG				    0x0100UL
#define DIAG_B_I_OVER_DCH				    0x0200UL
#define DIAG_B_SOC_LOW					    0x0400UL
#define	DIAG_B_V_IMBAL_CELL				    0x0800UL
#define DIAG_B_V_IMBAL_MODULE			    0x1000UL
#define	DIAG_B_R_ISO_LOW				    0x2000UL
#define DIAG_B_COMM_ERR_BMIC			    0x4000UL
#define DIAG_B_COMM_ERR_VCAN			    0x8000UL
#define	DIAG_B_V_OVER_AUX				    0x010000UL
#define DIAG_B_V_UNDER_AUX				    0x020000UL
#define DIAG_B_RELAY_ERR_PRE			    0x040000UL
#define DIAG_B_RELAY_ERR_NEG			    0x080000UL
#define	DIAG_B_RELAY_ERR_POS			    0x100000UL
#define DIAG_B_DEAD_FUSE				    0x200000UL
#define DIAG_B_DEAD_CELL				    0x400000UL
#define DIAG_B_T_IMBAL_CELL				    0x800000UL
#define DIAG_B_BAL_ERR_VCELL			    0x01000000UL
#define DIAG_B_OFFLINE_VCELL			    0x02000000UL
#define DIAG_B_OFFLINE_TCELL			    0x04000000UL
#define DIAG_B_OFFLINE_ISEN				    0x08000000UL
#define DIAG_B_FAN_Error				    0x10000000UL
#define DIAG_B_PackVCellV					0x20000000UL
#define DIAG_B_HVIL_OPEN				    0x40000000UL
#define DIAG_B_CLASSB_ERR				    0x80000000UL

#define DIAG_ISET_TIME						20u			/* [0.1 sec] */
#define DIAG_ICLR_TIME						10u			/* [0.1 sec] */
#define DIAG_WSET_TIME						50u			/* [0.1 sec] */
#define DIAG_WCLR_TIME						10u			/* [0.1 sec] */
#define DIAG_FSET_TIME						50u			/* [0.1 sec] */
#define DIAG_FSET_TIME2						10u			/* [0.1 sec] */
#define DIAG_FCLR_TIME						10u			/* [0.1 sec] */
#define DIAG_PSET_TIME						100u		/* [0.1 sec] */

#define DIAG_INIT_FAULT_CURRENT				(sint16)100	/* [0.1 A] */
#define DIAG_PFAULT_CURRENT					(sint16)50		/* [0.1 A] */
#define DIAG_MIN_CURR_FOR_CHECK_LOAD		(sint16)10		/* [0.1 A] */

#define DIAG_PFALUT_V_OVER_CELL				4250u
#define DIAG_PFALUT_V_UNDER_CELL			2550u

/* 1500V Mode */
#ifndef Drawer_14ea
#define DIAG_PFALUT_V_OVER_PACK				14280u
#define DIAG_PFALUT_V_UNDER_PACK			8568u

/* 1000V Mode */
#else
#define DIAG_PFALUT_V_OVER_PACK				9520u
#define DIAG_PFALUT_V_UNDER_PACK			5712u
#endif

#define DIAG_PFALUT_T_OVER_CELL				(sint16)550
#define DIAG_PFALUT_T_UNDER_CELL			(sint16)-195


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/
CLASSB static bool  TR_OFFLINE_VCELL;
CLASSB static bool  TR_OFFLINE_TCELL;
CLASSB static bool  TR_COMM_ERR_BMIC;


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
CLASSB static CLASSB_DATA		Classb_Data;

CLASSB static uint32 	FlagsInfo;
CLASSB static uint32 	FlagsWarn;
CLASSB static uint32 	FlagsFault;
CLASSB static uint32 	FlagsPFault;

CLASSB static PARAMETER	Diag_Parameter;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
void Diag_VOverCell(void);
void Diag_VOverPack(void);
void Diag_VUnderCell(void);
void Diag_VUnderPack(void);
void Diag_TOverCellChg(void);
void Diag_TOverCellDch(void);
void Diag_TUnderCellChg(void);
void Diag_TUnderCellDch(void);
void Diag_IOverChg(void);
void Diag_IOverDch(void);
void Diag_RIsoLow(void);
void Diag_CommErrBmic(void);
void Diag_CommErrVcan(void);
void Diag_VOverAux(void);
void Diag_VUnderAux(void);
void Diag_RelayErrPos(void);
void Diag_RelayErrNeg(void);
void Diag_RelayErrPre(void);
void Diag_DeadFuse(void);
void Diag_DeadCell(void);
void Diag_SocLow(void);
void Diag_VImbalCell(void);
void Diag_VImbalModule(void);
void Diag_HvilOpen(void);
void Diag_TImbalCell(void);
void Diag_BalErrVCell(void);
void Diag_OfflineVCell(void);
void Diag_OfflineTCell(void);
void Diag_OfflineISen(void);
void Diag_ClassB(void);
void Diag_Fan_Status(void);
void Diag_PackVCellV(void);
void Diag_ControlTimeSpan(uint8* pISetCnt, uint8* pIClrCnt, uint8* pWSetCnt, uint8* pWClrCnt,
								uint8* pFSetCnt, uint8* pFClrCnt, uint8* pPSetCnt,
								uint8 ISetSpan, uint8 IClrSpan, uint8 WSetSpan, uint8 WClrSpan,
								uint8 FSetSpan, uint8 FClrSpan, uint8 PSetSpan,
								BOOL ISetFlag, BOOL IClrFlag, BOOL WSetFlag, BOOL WClrFlag,
								BOOL FSetFlag, BOOL FClrFlag, BOOL PSetFlag,
								uint32	DiagFlagBit);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/

/**
* @brief	Diagnostic initialization
* @param	void
* @return	uint32 - Fault
*/
uint32	Diag_Init(void)
{
	uint32	rv	= 0;
	uint32	rv2	= 0;

	Parameter_Get_ParameterData(&Diag_Parameter);
	
#ifndef TestMode
#ifndef Test_Diag_Module

	static BOOL Diag_LoadSavedFaultFlag = FALSE;
	static BOOL Diag_LoadSavedPFaultFlag = FALSE;
	
	static uint32	SavedFault	= 0u;
	static uint32	SavedPFault	= 0u;

	uint16 V_CellMax;
	uint16 V_PackMeas;
	uint16 V_CellMin;
	uint16 V_CellDev;
	sint16 T_CellMax;
	sint16 T_CellMin;
	sint16 I_PackMeas;
	uint16 V_ModDev;
	uint16 ISO_Resistor;
	uint16 V_Aux;

	#ifdef Test_Diag_Init
	V_CellMax = 4300u;
	V_PackMeas = 9000u;
	V_CellMin = 2600u;
	V_CellDev =	600u;
	T_CellMax = 550;
	T_CellMin = -200;
	I_PackMeas = -950;
	V_ModDev = 5000u;
	ISO_Resistor = 900u;
	V_Aux = 8000u;
	#else
	V_CellMax = Adbms_Get_V_CellMax();
	V_PackMeas = PackVolt_Get_V_PackMeas();
	V_CellMin = Adbms_Get_V_CellMin();
	V_CellDev =	Adbms_Get_V_CellDev();
	T_CellMax = Adbms_Get_T_CellMax();
	T_CellMin = Adbms_Get_T_CellMin();
	I_PackMeas = Curr_Get_I_PackMeas();
	V_ModDev = Adbms_Get_V_ModDev();
	ISO_Resistor = IsoR_Get_ISO_Resistor();
	V_Aux = AuxVolt_Get_V_Aux();
	#endif


	/* OV */
	if (V_CellMax >= Diag_Parameter.OCV_Fault)
	{
		rv	|= DIAG_B_V_OVER_CELL;
	}
	if (V_PackMeas >= Diag_Parameter.OPV_Fault)
	{
	    rv	|= DIAG_B_V_OVER_PACK;
	}
	/* UV */
	if (V_CellMin <= Diag_Parameter.UCV_Fault)
	{
	    rv	|= DIAG_B_V_UNDER_CELL;
	}
	if (V_PackMeas <= Diag_Parameter.UPV_Fault)
    {
        rv	|= DIAG_B_V_UNDER_PACK;
    }
	/* OT */
	if (T_CellMax > Diag_Parameter.DOT_Fault)
	{
	    rv	|= DIAG_B_T_OVER_CELL_DCH;
	}
	/* UT */
	if (T_CellMin < Diag_Parameter.DUT_Fault)
    {
        rv	|= DIAG_B_T_UNDER_CELL_DCH;
    }
	/* OC */
	if (I_PackMeas >= DIAG_INIT_FAULT_CURRENT)
	{
	    rv	|= DIAG_B_I_OVER_CHG;
    }
	if (I_PackMeas <= (DIAG_INIT_FAULT_CURRENT * (-1)) )
	{
	    rv	|= DIAG_B_I_OVER_DCH;
	}
	/* The others */
	if (V_CellDev >= Diag_Parameter.CIBV_Fault)
	{
	    rv	|= DIAG_B_V_IMBAL_CELL;
	}
	if (V_ModDev >= Diag_Parameter.MIBV_Fault)
	{
	    rv	|= DIAG_B_V_IMBAL_MODULE;
	}
	if (ISO_Resistor <= Diag_Parameter.ISO_Fault)
	{
	    rv	|= DIAG_B_R_ISO_LOW;
	}
	if (V_Aux >= Diag_Parameter.OAV_Fault)
	{
	    rv	|= DIAG_B_V_OVER_AUX;
	}
	if (V_Aux <= Diag_Parameter.UAV_Fault)
	{
	    rv	|= DIAG_B_V_UNDER_AUX;
	}
	if (V_CellMin <= Diag_Parameter.DEAD_CELL_Fault)
	{
	    rv	|= DIAG_B_DEAD_CELL;
	}
	if (HvilEpo_IsNormal() == FALSE)
	{
	    rv	|= DIAG_B_HVIL_OPEN;
	}
	else
	{
		if(V_PackMeas <= Diag_Parameter.FUSE_Fault)
		{
		    rv	|= DIAG_B_DEAD_FUSE;
		}
	}



	if ( (EepromEx_IsSavedFaultHistory() == TRUE) && (Diag_LoadSavedFaultFlag == FALSE) )
	{
		uint8	e_buf[16];

		Diag_LoadSavedFaultFlag	= TRUE;
		EepromEx_GetEventLog(EepromEx_GetEventIndex() - 1u, e_buf);
		SavedFault
			= ((uint32)e_buf[12] << 24u)
			+ ((uint32)e_buf[13] << 16u)
			+ ((uint32)e_buf[14] <<  8u)
			+ ((uint32)e_buf[15]      );
	}

	rv						 = (rv | SavedFault);
	FlagsFault = rv;



	if ( (EepromEx_IsSavedPFaultHistory() == TRUE) && (Diag_LoadSavedPFaultFlag == FALSE) )
	{
		uint8	e_buf2[12];

		Diag_LoadSavedPFaultFlag	= TRUE;
		EepromEx_GetPEventLog(EepromEx_GetPEventIndex() - 1u, e_buf2);
		SavedPFault
			= ((uint32)e_buf2[8] << 24u)
			+ ((uint32)e_buf2[9] << 16u)
			+ ((uint32)e_buf2[10] <<  8u)
			+ ((uint32)e_buf2[11]      );
	}

	rv2						 = (rv2 | SavedPFault);
	FlagsPFault = rv2;

#endif
#endif	

	return	rv + rv2;

	
}


/*---------------------------------------------------------------------*/
void Diag_Task(void)
/**
* @brief	Diagnostic Task function
* @param	void
* @return	void
*/
{
	SYSTEM_STATE System_State;

	System_State = ManaMsm_Get_System_State();

	
	if((System_State != SYS_INIT)
	&& (System_State != SYS_CONFIGURATION)
	&& (System_State != SYS_CALIBRATION)
	&& (System_State != SYS_LOGREAD))
	{
		#ifdef Cell_OverVoltage_Test
		
			Diag_VOverCell();
		#endif

		#ifdef Pack_OverVoltage_Test
		
			Diag_VOverPack();
		#endif

		#ifdef Cell_UnderVoltage_Test
		
			Diag_VUnderCell();
		#endif

		#ifdef Pack_UnderVoltage_Test
		
			Diag_VUnderPack();
		#endif

		#ifdef Temp_Over_Test
		
			Diag_TOverCellChg();
			Diag_TOverCellDch();
		#endif

		#ifdef Temp_Under_Test
		
			Diag_TUnderCellChg();
			Diag_TUnderCellDch();
		#endif
		
		#ifdef BMIC_Error_Test
			Diag_CommErrBmic();
		#endif

		#ifdef Aux_Voltage_Test
			Diag_VOverAux();
			Diag_VUnderAux();
		#endif

		#ifdef BMIC_OPENError_Test
			Diag_OfflineVCell();
			Diag_OfflineTCell();
		#endif

		#ifdef PackV_CellV
			PackVolt_Set_V_PackMeas(11800);
			PackVolt_Set_V_BusMeas(11800);
			Diag_PackVCellV();
		#endif

		#ifdef Isolation_Test
			static uint8 count = 0u;

			if(count <= 10u)
			{
				count++;
			}
			if(count > 10u)
			{
				Diag_RIsoLow();
			}
		#endif

		#ifdef Dead_Fuse
			Diag_DeadFuse();
		#endif

		#ifdef Dead_Cell
			Diag_DeadCell();
		#endif


		#ifndef TestMode

			Diag_VOverCell();
			Diag_VOverPack();
			Diag_VUnderCell();
			Diag_VUnderPack();
			Diag_TOverCellChg();
			Diag_TOverCellDch();
			Diag_TUnderCellChg();
			Diag_TUnderCellDch();
			Diag_RIsoLow();
			
			Diag_BalErrVCell();
			Diag_OfflineVCell();
			Diag_OfflineTCell();

			Diag_CommErrBmic();
			Diag_VOverAux();
			Diag_VUnderAux();
			Diag_RelayErrPos();
			Diag_RelayErrNeg();
			Diag_RelayErrPre();
			Diag_DeadFuse();
			Diag_DeadCell();
			Diag_SocLow();
			Diag_VImbalCell();
			Diag_VImbalModule();
			Diag_HvilOpen();
			Diag_TImbalCell();
			Diag_OfflineISen();
			Diag_ClassB();
			Diag_Fan_Status();
			Diag_PackVCellV();
		#endif

		if(System_State != SYS_DETECTING_FCC)
		{
			#ifndef TestMode
			
				Diag_CommErrVcan();
				Diag_IOverChg();
				Diag_IOverDch();
			#endif
			#ifdef Current_Test
				Diag_OfflineISen();
				Diag_IOverChg();
				Diag_IOverDch();
			#endif
		}

		
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Over Cell Voltage Warning / Fault
* @param	void
* @return	void
*/
void Diag_VOverCell(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint16	DiagFSet;
//	uint16	DiagFClr;
	uint16	DiagPSet;
	uint32	DiagFlagBit;

	sint16 I_PackMeas;

	#ifdef Test_Diag_VOverCell
	DiagValue = 4231u;
	I_PackMeas = 20;
	#else
	DiagValue	= Adbms_Get_V_CellMax();
	I_PackMeas = Curr_Get_I_PackMeas();
	#endif
	
	DiagISet	= Diag_Parameter.OCV_W_Release;
	DiagIClr	= Diag_Parameter.OCV_W_Release;
	DiagWSet	= Diag_Parameter.OCV_Warning;
	DiagWClr	= Diag_Parameter.OCV_W_Release;
	DiagFSet	= Diag_Parameter.OCV_Fault;
//	DiagFClr	= Diag_Parameter.OCV_F_Release;
	DiagPSet	= DIAG_PFALUT_V_OVER_CELL;
	DiagFlagBit	= DIAG_B_V_OVER_CELL;

	

	/* Info */
	if ( (DiagValue > DiagISet)
	   && (I_PackMeas > (sint16)10) )
	{
	    ISetFlag	= TRUE;
	}
	else
	{
	    ISetFlag	= FALSE;
	}
	if (DiagValue < DiagIClr)
	{
	    IClrFlag	= TRUE;
    }
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if ( (DiagValue > DiagWSet)
	   && (I_PackMeas > (sint16)10) )
	{
	    WSetFlag	= TRUE;
    }
	else
    {
        WSetFlag	= FALSE;
    }
	if (DiagValue < DiagWClr)
	{
	    WClrFlag	= TRUE;
    }
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if ( (DiagValue > DiagFSet)
	   && (I_PackMeas > 10) )
	{
	    FSetFlag	= TRUE;
	}
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;
	
	/* Permanent Fault */
	if (DiagValue > DiagPSet)
	{
	    PSetFlag	= TRUE;
	}
	else
	{
		PSetFlag	= FALSE;
    }

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Over Pack Voltage Warning / Fault
* @param	void
* @return	void
*/
void Diag_VOverPack(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint16	DiagFSet;
//	uint16	DiagFClr;
	uint16	DiagPSet;
	uint32	DiagFlagBit;
	sint16	I_PackMeas;
	SYSTEM_STATE System_State;
	uint16	V_BusMeas;
	uint16	V_PackMeas;

	#ifdef Test_Diag_VOverPack
	I_PackMeas = 20;
	System_State = SYS_DRIVE;
	V_BusMeas = 14213u;
	V_PackMeas = 14213u;
	#elif defined(Test_Diag_VOverPack_224S)
	I_PackMeas = 20;
	System_State = SYS_DRIVE;
	V_BusMeas = 9476u;
	V_PackMeas = 9476u;
	#else
	I_PackMeas = Curr_Get_I_PackMeas();
	System_State = ManaMsm_Get_System_State();
	V_BusMeas = PackVolt_Get_V_BusMeas();
	V_PackMeas = PackVolt_Get_V_PackMeas();
	#endif

	if ( (System_State == SYS_DRIVE)
	  && (V_BusMeas > V_PackMeas) )
	{
		DiagValue	= V_BusMeas;
	}
	else
	{
		DiagValue	= V_PackMeas;
	}

	DiagISet	= Diag_Parameter.OPV_W_Release;
	DiagIClr	= Diag_Parameter.OPV_W_Release;
	DiagWSet	= Diag_Parameter.OPV_Warning;
	DiagWClr	= Diag_Parameter.OPV_W_Release;
	DiagFSet	= Diag_Parameter.OPV_Fault;
//	DiagFClr	= Diag_Parameter.OPV_F_Release;
	DiagPSet	= DIAG_PFALUT_V_OVER_PACK;
	DiagFlagBit	= DIAG_B_V_OVER_PACK;

	/* Info */
	if ( (DiagValue > DiagISet)
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    ISetFlag	= TRUE;
	}
	else
	{
	    ISetFlag	= FALSE;
    }
	if (DiagValue < DiagIClr)
	{
	    IClrFlag	= TRUE;
	}
	else
	{
	    IClrFlag	= FALSE;
    }
	/* Warning */
	if ( (DiagValue > DiagWSet)
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    WSetFlag	= TRUE;
    }
	else
	{
	    WSetFlag	= FALSE;
    }
	if (DiagValue < DiagWClr)
	{
	    WClrFlag	= TRUE;
    }
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if ( (DiagValue > DiagFSet)
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    FSetFlag	= TRUE;
    }
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	if (DiagValue > DiagPSet)
	{
	    PSetFlag	= TRUE;
	}
	else
	{
		PSetFlag	= FALSE;
    }

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Under Cell Voltage Warning / Fault
* @param	void
* @return	void
*/
void Diag_VUnderCell(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint16	DiagFSet;
//	uint16	DiagFClr;
	uint16	DiagPSet;
	uint32	DiagFlagBit;

	sint16	I_PackMeas;

	#ifdef Test_Diag_VUnderCell
	I_PackMeas = -20;
	DiagValue = 2699u;
	#else
	I_PackMeas = Curr_Get_I_PackMeas();
	DiagValue	= Adbms_Get_V_CellMin();
	#endif
	
	DiagISet	= Diag_Parameter.UCV_W_Release;
	DiagIClr	= Diag_Parameter.UCV_W_Release;
	DiagWSet	= Diag_Parameter.UCV_Warning;
	DiagWClr	= Diag_Parameter.UCV_W_Release;
	DiagFSet	= Diag_Parameter.UCV_Fault;
//	DiagFClr	= Diag_Parameter.UCV_F_Release;
	DiagPSet	= DIAG_PFALUT_V_UNDER_CELL;
	DiagFlagBit	= DIAG_B_V_UNDER_CELL;

	/* Info */
	if ( (DiagValue < DiagISet)
	   && (I_PackMeas < -10) )
	{
	    ISetFlag	= TRUE;
    }
	else
    {
		ISetFlag	= FALSE;
    }
	if (DiagValue > DiagIClr)
	{
	    IClrFlag	= TRUE;
    }
	else
    {
        IClrFlag	= FALSE;
    }

	/* Warning */
	if ( (DiagValue < DiagWSet)
	   && (I_PackMeas < -10) )
	{
	    WSetFlag	= TRUE;
    }
	else
	{
	    WSetFlag	= FALSE;
    }
	if (DiagValue > DiagWClr)
	{
	    WClrFlag	= TRUE;
    }
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if ( (DiagValue < DiagFSet)
	   && (I_PackMeas < -10) )
	{
	    FSetFlag	= TRUE;
    }
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	if (DiagValue < DiagPSet)
	{
	    PSetFlag	= TRUE;
	}
	else
	{
		PSetFlag	= FALSE;
    }

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Under Pack Voltage Warning / Fault
* @param	void
* @return	void
*/
void Diag_VUnderPack(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint16	DiagFSet;
//	uint16	DiagFClr;
	uint16	DiagPSet;
	uint32	DiagFlagBit;

	sint16	I_PackMeas;
	SYSTEM_STATE System_State;
	uint16	V_BusMeas;
	uint16	V_PackMeas;

	#ifdef Test_Diag_VUnderPack
	I_PackMeas = -20;
	System_State = SYS_DRIVE;
	V_BusMeas = 9071u;
	V_PackMeas = 9071u;
	#elif defined(Test_Diag_VOverPack_224S)
	I_PackMeas = -20;
	System_State = SYS_DRIVE;
	V_BusMeas = 6047u;
	V_PackMeas = 6047u;
	#else
	I_PackMeas = Curr_Get_I_PackMeas();
	System_State = ManaMsm_Get_System_State();
	V_BusMeas = PackVolt_Get_V_BusMeas();
	V_PackMeas = PackVolt_Get_V_PackMeas();
	#endif

	if ( (System_State == SYS_DRIVE)
	  && (V_BusMeas < V_PackMeas) )
	{
		DiagValue	= V_BusMeas;
	}
	else
	{
		DiagValue	= V_PackMeas;
	}

	DiagISet	= Diag_Parameter.UPV_W_Release;
	DiagIClr	= Diag_Parameter.UPV_W_Release;
	DiagWSet	= Diag_Parameter.UPV_Warning;
	DiagWClr	= Diag_Parameter.UPV_W_Release;
	DiagFSet	= Diag_Parameter.UPV_Fault;
//	DiagFClr	= Diag_Parameter.UPV_F_Release;
	DiagPSet	= DIAG_PFALUT_V_UNDER_PACK;
	DiagFlagBit	= DIAG_B_V_UNDER_PACK;

	/* Info */
	if ( (DiagValue < DiagISet)
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))))
	{
	    ISetFlag	= TRUE;
    }
	else
	{
	    ISetFlag	= FALSE;
    }
	if (DiagValue > DiagIClr)
	{
	    IClrFlag	= TRUE;
    }
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if ( (DiagValue < DiagWSet)
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))))
	{
	    WSetFlag	= TRUE;
    }
	else
	{
	    WSetFlag	= FALSE;
    }
	if (DiagValue > DiagWClr)
	{
	    WClrFlag	= TRUE;
    }
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if ( (DiagValue < DiagFSet)
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))))
	{
	    FSetFlag	= TRUE;
    }
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	if (DiagValue < DiagPSet)
	{
	    PSetFlag	= TRUE;
	}
	else
	{
		PSetFlag	= FALSE;
    }

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Over Temperature(When Charging) Warning / Fault
* @param	void
* @return	void
*/
void Diag_TOverCellChg(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	sint16	DiagValue;
	sint16	DiagISet;
	sint16	DiagIClr;
	sint16	DiagWSet;
	sint16	DiagWClr;
	sint16	DiagFSet;
//	sint16	DiagFClr;
	sint16	DiagPSet;
	uint32	DiagFlagBit;

	sint16	I_PackMeas;

	#ifdef Test_Diag_TOverCellChg
	I_PackMeas = 20;
	DiagValue	= 541;
	#else
	I_PackMeas = Curr_Get_I_PackMeas();
	DiagValue	= Adbms_Get_T_CellMax();
	#endif

	
	DiagISet	= Diag_Parameter.COT_W_Release;
	DiagIClr	= Diag_Parameter.COT_W_Release;
	DiagWSet	= Diag_Parameter.COT_Warning;
	DiagWClr	= Diag_Parameter.COT_W_Release;
	DiagFSet	= Diag_Parameter.COT_Fault;
//	DiagFClr	= Diag_Parameter.COT_F_Release;
	DiagPSet	= DIAG_PFALUT_T_OVER_CELL;
	DiagFlagBit	= DIAG_B_T_OVER_CELL_CHG;

	/* Info */
	if ( (DiagValue > DiagISet)
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    ISetFlag	= TRUE;
    }
	else
	{
	    ISetFlag	= FALSE;
    }
	if (DiagValue < DiagIClr)
	{
	    IClrFlag	= TRUE;
    }
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if ( (DiagValue > DiagWSet)
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    WSetFlag	= TRUE;
    }
	else
	{
	    WSetFlag	= FALSE;
    }
	if (DiagValue < DiagWClr)
	{
	    WClrFlag	= TRUE;
    }
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if ( (DiagValue > DiagFSet)
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    FSetFlag	= TRUE;
    }
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	if ((DiagValue > DiagPSet)
		&& (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    PSetFlag	= TRUE;
	}
	else
	{
		PSetFlag	= FALSE;
    }

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Over Temperature(When Discharging) Warning / Fault
* @param	void
* @return	void
*/
void	Diag_TOverCellDch(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	sint16	DiagValue;
	sint16	DiagISet;
	sint16	DiagIClr;
	sint16	DiagWSet;
	sint16	DiagWClr;
	sint16	DiagFSet;
//	sint16	DiagFClr;
	sint16	DiagPSet;
	uint32	DiagFlagBit;

	sint16	I_PackMeas;

	#ifdef Test_Diag_TOverCellDch
	I_PackMeas = -20;
	DiagValue	= 541;
	#else
	I_PackMeas = Curr_Get_I_PackMeas();
	DiagValue	= Adbms_Get_T_CellMax();
	#endif
	
	DiagISet	= Diag_Parameter.DOT_W_Release;
	DiagIClr	= Diag_Parameter.DOT_W_Release;
	DiagWSet	= Diag_Parameter.DOT_Warning;
	DiagWClr	= Diag_Parameter.DOT_W_Release;
	DiagFSet	= Diag_Parameter.DOT_Fault;
//	DiagFClr	= Diag_Parameter.DOT_F_Release;
	DiagPSet	= DIAG_PFALUT_T_OVER_CELL;
	DiagFlagBit	= DIAG_B_T_OVER_CELL_DCH;

	/* Info */
	if ( (DiagValue > DiagISet)
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))))
	{
	    ISetFlag	= TRUE;
    }
	else
	{
	    ISetFlag	= FALSE;
    }
	if (DiagValue < DiagIClr)
	{
	    IClrFlag	= TRUE;
    }
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if ( (DiagValue > DiagWSet)
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))))
	{
	    WSetFlag	= TRUE;
    }
	else
	{
	    WSetFlag	= FALSE;
    }
	if (DiagValue < DiagWClr)
	{
	    WClrFlag	= TRUE;
    }
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if ( (DiagValue > DiagFSet)
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))))
	{
	    FSetFlag	= TRUE;
    }
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	if (DiagValue > DiagPSet)
	{
	    PSetFlag	= TRUE;
	}
	else
	{
		PSetFlag	= FALSE;
    }

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/

/**
* @brief	Judged Under Temperature(When Charging) Warning / Fault
* @param	void
* @return	void
*/
void Diag_TUnderCellChg(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	sint16	DiagValue;
	sint16	DiagISet;
	sint16	DiagIClr;
	sint16	DiagWSet;
	sint16	DiagWClr;
	sint16	DiagFSet;
//	sint16	DiagFClr;
	sint16	DiagPSet;
	uint32	DiagFlagBit;

	sint16	I_PackMeas;

	#ifdef Test_Diag_TUnderCellChg
	I_PackMeas = 20;
	DiagValue	= -191;
	#else
	I_PackMeas = Curr_Get_I_PackMeas();
	DiagValue	= Adbms_Get_T_CellMin();
	#endif
	
	DiagISet	= Diag_Parameter.CUT_W_Release;
	DiagIClr	= Diag_Parameter.CUT_W_Release;
	DiagWSet	= Diag_Parameter.CUT_Warning;
	DiagWClr	= Diag_Parameter.CUT_W_Release;
	DiagFSet	= Diag_Parameter.CUT_Fault;
//	DiagFClr	= Diag_Parameter.CUT_F_Release;
	DiagPSet	= DIAG_PFALUT_T_UNDER_CELL;
	DiagFlagBit	= DIAG_B_T_UNDER_CELL_CHG;

	/* Info */
	if ( (DiagValue < DiagISet)
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    ISetFlag	= TRUE;
    }
	else
	{
	    ISetFlag	= FALSE;
    }
	if (DiagValue > DiagIClr)
	{
	    IClrFlag	= TRUE;
    }
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if ( (DiagValue < DiagWSet)
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    WSetFlag	= TRUE;
    }
	else
	{
	    WSetFlag	= FALSE;
    }
	if (DiagValue > DiagWClr)
	{
	    WClrFlag	= TRUE;
    }
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if ( (DiagValue < DiagFSet)
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    FSetFlag	= TRUE;
    }
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	if ((DiagValue < DiagPSet)
		&& (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    PSetFlag	= TRUE;
	}
	else
	{
		PSetFlag	= FALSE;
    }

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Under Temperature(When Discharging) Warning / Fault
* @param	void
* @return	void
*/
void Diag_TUnderCellDch(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	sint16	DiagValue;
	sint16	DiagISet;
	sint16	DiagIClr;
	sint16	DiagWSet;
	sint16	DiagWClr;
	sint16	DiagFSet;
//	sint16	DiagFClr;
	sint16	DiagPSet;
	uint32	DiagFlagBit;

	sint16	I_PackMeas;

	#ifdef Test_Diag_TUnderCellDch
	I_PackMeas = -20;
	DiagValue	= -191;
	#else
	I_PackMeas = Curr_Get_I_PackMeas();
	DiagValue	= Adbms_Get_T_CellMin();
	#endif
	
	DiagISet	= Diag_Parameter.DUT_W_Release;
	DiagIClr	= Diag_Parameter.DUT_W_Release;
	DiagWSet	= Diag_Parameter.DUT_Warning;
	DiagWClr	= Diag_Parameter.DUT_W_Release;
	DiagFSet	= Diag_Parameter.DUT_Fault;
//	DiagFClr	= Diag_Parameter.DUT_F_Release;
	DiagPSet	= DIAG_PFALUT_T_UNDER_CELL;
	DiagFlagBit	= DIAG_B_T_UNDER_CELL_DCH;

	/* Info */
	if ( (DiagValue < DiagISet)
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))))
	{
	    ISetFlag	= TRUE;
    }
	else
	{
	    ISetFlag	= FALSE;
    }
	if (DiagValue > DiagIClr)
	{
	    IClrFlag	= TRUE;
    }
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if ( (DiagValue < DiagWSet)
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))))
	{
	    WSetFlag	= TRUE;
    }
	else
	{
	    WSetFlag	= FALSE;
    }
	if (DiagValue > DiagWClr)
	{
	    WClrFlag	= TRUE;
    }
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if ( (DiagValue < DiagFSet)
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))))
	{
	    FSetFlag	= TRUE;
    }
    else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	if (DiagValue < DiagPSet)
	{
	    PSetFlag	= TRUE;
	}
	else
	{
		PSetFlag	= FALSE;
    }

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Over Charge Current Warning / Fault
* @param	void
* @return	void
*/
void Diag_IOverChg(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	sint16	DiagValue;
	sint16	DiagISet;
	sint16	DiagIClr;
	sint16	DiagWSet;
	sint16	DiagWClr;
	sint16	DiagFSet;
//	sint16	DiagFClr;
	uint32	DiagFlagBit;

    uint32 bufDiag;
	SYSTEM_STATE System_State;
	sint16 I_PackMeas;
	sint16 ChgCurrLimit;
	
	System_State = ManaMsm_Get_System_State();
	I_PackMeas = Curr_Get_I_PackMeas();
	ChgCurrLimit = CurrLimit_Get_ChgCurrLimit();

	#ifdef Test_Diag_IOverChg
	DiagValue	= 946;
	#else
	DiagValue	= Curr_Get_I_PackMeas();
	#endif
	
	DiagISet	= Diag_Parameter.COC_W_Release;
	DiagIClr	= Diag_Parameter.COC_W_Release;
	DiagWSet	= Diag_Parameter.COC_Warning;
	DiagWClr	= Diag_Parameter.COC_W_Release;
	DiagFSet	= Diag_Parameter.COC_Fault;
//	DiagFClr	= Diag_Parameter.COC_F_Release;
	DiagFlagBit	= DIAG_B_I_OVER_CHG;

	/* Info */
	bufDiag = ((uint32)ChgCurrLimit * (uint32)105 / (uint32)100) + (uint32)20; /* 105 % */
	if ( ( (DiagValue > DiagISet)
		|| ( (System_State == SYS_DRIVE)
       && ( DiagValue > (sint16)bufDiag) ) )
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
    {
		ISetFlag	= TRUE;
    }
	else
	{
	    ISetFlag	= FALSE;
    }
	if ( (DiagValue < DiagIClr)
	  && (DiagValue <= ChgCurrLimit) )
	{
	    IClrFlag	= TRUE;
    }
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	bufDiag = ((uint32)ChgCurrLimit * (uint32)110 / (uint32)100) + (uint32)20; /* 110 % */
	if ( ( (DiagValue > DiagWSet)
		|| ( (System_State == SYS_DRIVE)
		  && ( DiagValue > (sint16)bufDiag) ) )
	   && (I_PackMeas > DIAG_MIN_CURR_FOR_CHECK_LOAD) )
	{
	    WSetFlag	= TRUE;
	}
	else
	{
	    WSetFlag	= FALSE;
    }
	if ( (DiagValue < DiagWClr)
	  && (DiagValue <= ChgCurrLimit) )
	{
	    WClrFlag	= TRUE;
    }
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if (DiagValue > DiagFSet)
    {
		FSetFlag	= TRUE;
    }
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	PSetFlag	= FALSE;

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Over Discharge Current Warning / Fault
* @param	void
* @return	void
*/
void Diag_IOverDch(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	sint16	DiagValue;
	sint16	DiagISet;
	sint16	DiagIClr;
	sint16	DiagWSet;
	sint16	DiagWClr;
	sint16	DiagFSet;
//	sint16	DiagFClr;
	uint32	DiagFlagBit;

    uint32 bufDiag;
	SYSTEM_STATE System_State;
	sint16 I_PackMeas;
	sint16 DchCurrLimit;
	
	System_State = ManaMsm_Get_System_State();
	I_PackMeas = Curr_Get_I_PackMeas();
	DchCurrLimit = CurrLimit_Get_DchCurrLimit();

	#ifdef Test_Diag_IOverDch
	DiagValue	= 946;
	#else
	DiagValue	= Curr_Get_I_PackMeas() * (-1);
	#endif
	
	DiagISet	= Diag_Parameter.DOC_W_Release;
	DiagIClr	= Diag_Parameter.DOC_W_Release;
	DiagWSet	= Diag_Parameter.DOC_Warning;
	DiagWClr	= Diag_Parameter.DOC_W_Release;
	DiagFSet	= Diag_Parameter.DOC_Fault;
//	DiagFClr	= Diag_Parameter.DOC_F_Release;
	DiagFlagBit	= DIAG_B_I_OVER_DCH;

	/* Info */
	bufDiag = ((uint32)DchCurrLimit * (uint32)105 / (uint32)100) + (uint32)20; /* 105 % */
	if ( ( (DiagValue > DiagISet)
		|| ( (System_State == SYS_DRIVE)
		  && ( DiagValue > (sint16)bufDiag) ) )
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))) )
	{
	    ISetFlag	= TRUE;
    }
	else
	{
	    ISetFlag	= FALSE;
	}
	if ( (DiagValue < DiagIClr)
	  && (DiagValue <= DchCurrLimit) )
	{
	    IClrFlag	= TRUE;
	}
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	bufDiag = ((uint32)DchCurrLimit * (uint32)110 / (uint32)100) + (uint32)20; /* 110 % */
	if ( ( (DiagValue > DiagWSet)
		|| ( (System_State == SYS_DRIVE)
		  && ( DiagValue > (sint16)bufDiag) ) )
	   && (I_PackMeas < (DIAG_MIN_CURR_FOR_CHECK_LOAD * (-1))) )
    {
        WSetFlag	= TRUE;
    }
	else
	{
	    WSetFlag	= FALSE;
    }
	if ( (DiagValue < DiagWClr)
	  && (DiagValue <= DchCurrLimit) )
	{
	    WClrFlag	= TRUE;
    }
	else
	{
	    WClrFlag	= FALSE;
    }
	/* Fault */
	if (DiagValue > DiagFSet)
    {
        FSetFlag	= TRUE;
    }
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Isolation warning / fault
* @param	void
* @return	void
*/
void Diag_RIsoLow(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint16	DiagFSet;
//	uint16	DiagFClr;
	uint32	DiagFlagBit;

	#ifdef Test_Diag_RIsoLow
	DiagValue	= 999u;
	#else
	DiagValue   = IsoR_Get_ISO_Resistor();
	#endif
	
	DiagISet	= Diag_Parameter.ISO_W_Release;
	DiagIClr	= Diag_Parameter.ISO_W_Release;
	DiagWSet	= Diag_Parameter.ISO_Warning;
	DiagWClr	= Diag_Parameter.ISO_W_Release;
	DiagFSet	= Diag_Parameter.ISO_Fault;
//	DiagFClr	= Diag_Parameter.ISO_F_Release;
	DiagFlagBit	= DIAG_B_R_ISO_LOW;

	/* Info */
	if (DiagValue < DiagISet)
	{
	    ISetFlag	= TRUE;
	}
	else
	{
	    ISetFlag	= FALSE;
	}
	if (DiagValue > DiagIClr)
	{
	    IClrFlag	= TRUE;
	}
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if (DiagValue < DiagWSet)
	{
	    WSetFlag	= TRUE;
	}
	else
	{
	    WSetFlag	= FALSE;
	}
	if (DiagValue > DiagWClr)
	{
	    WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if (DiagValue < DiagFSet)
	{
	    FSetFlag	= TRUE;
	}
	else
	{
	    FSetFlag	= FALSE;
	}
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    
	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Slave communication warning / fault
* @param	void
* @return	void
*/
void Diag_CommErrBmic(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= (uint8)Diag_Parameter.SCE_W_Release;
	uint8	IClrSpan	= ISetSpan;
	uint8	WSetSpan	= (uint8)Diag_Parameter.SCE_Warning;
	uint8	WClrSpan	= (uint8)Diag_Parameter.SCE_W_Release;
	uint8	FSetSpan	= (uint8)Diag_Parameter.SCE_Fault;
	uint8	FClrSpan	= (uint8)Diag_Parameter.SCE_F_Release;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	uint32	errorF;
	uint32	errorR;

	/* Determine diagnostic conditions */
	uint32	DiagFlagBit;

	DiagFlagBit	= DIAG_B_COMM_ERR_BMIC;

	#ifdef Test_Diag_CommErrBmic
	errorF	= 0x1FFFFEu;
	errorR  = 0X1u;;
	#else
	errorF = Adbms6815_FrontSPI_Error_Check();
	errorR = Adbms6815_RearSPI_Error_Check();
	#endif

	/* Info */
	if ((errorF	!= 0u) || (errorR != 0u))
	{
	    ISetFlag	= TRUE;
	}
	else
	{
	    ISetFlag	= FALSE;
	}
	if ((errorF	== 0u) && (errorR == 0u))
	{
	    IClrFlag	= TRUE;
	}
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if ((errorF	!= 0u) || (errorR != 0u))
	{
	    WSetFlag	= TRUE;
	}
	else
	{
	    WSetFlag	= FALSE;
	}
	if ((errorF	== 0u) && (errorR == 0u))
	{
	    WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if ((errorF	!= 0u) && (errorR != 0u))
	{
	    FSetFlag	= TRUE;
		Set_TROfflineBMIC(TRUE);
	}
	else
	{
	    FSetFlag	= FALSE;
		Set_TROfflineBMIC(FALSE);
    }
	FClrFlag	= FALSE;

	Adbms_ResetCommError_F();
	Adbms_ResetCommError_R();

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Master communication warning / fault
* @param	void
* @return	void
*/
void Diag_CommErrVcan(void)
{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= 2;	/* [0.1 sec] */
	uint8	IClrSpan	= 2;	/* [0.1 sec] */
	uint8	WSetSpan	= 2;	/* [0.1 sec] */
	uint8	WClrSpan	= 2;	/* [0.1 sec] */
	uint8	FSetSpan	= 2;	/* [0.1 sec] */
	uint8	FClrSpan	= 2;	/* [0.1 sec] */
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint16	DiagFSet;
//	uint16	DiagFClr;
	uint32	DiagFlagBit;
	DiagValue	= (uint16)CanVcuRx_Get_OfflineCnt();
	DiagISet	= Diag_Parameter.MCE_W_Release;
	DiagIClr	= Diag_Parameter.MCE_W_Release;
	DiagWSet	= Diag_Parameter.MCE_Warning;
	DiagWClr	= Diag_Parameter.MCE_W_Release;
	DiagFSet	= Diag_Parameter.MCE_Fault;
//	DiagFClr	= Diag_Parameter.MCE_F_Release;
	DiagFlagBit	= DIAG_B_COMM_ERR_VCAN;

	/* Info */
	if (DiagValue > DiagISet)
	{
	    ISetFlag	= TRUE;
    }
	else
	{
	    ISetFlag	= FALSE;
    }
	if (DiagValue < DiagIClr)
	{
	    IClrFlag	= TRUE;
	}
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if (DiagValue > DiagWSet)
	{
	    WSetFlag	= TRUE;
	}
	else
	{
	    WSetFlag	= FALSE;
    }
	if (DiagValue < DiagWClr)
	{
	    WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if (DiagValue > DiagFSet)
	{
	    FSetFlag	= TRUE;
	}
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Auxiliary Over voltage warning / fault
* @param	void
* @return	void
*/
void Diag_VOverAux(void)
{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint16	DiagFSet;
//	uint16	DiagFClr;
	uint32	DiagFlagBit;

	DiagValue	= AuxVolt_Get_V_Aux();
	DiagISet	= Diag_Parameter.OAV_W_Release;
	DiagIClr	= Diag_Parameter.OAV_W_Release;
	DiagWSet	= Diag_Parameter.OAV_Warning;
	DiagWClr	= Diag_Parameter.OAV_W_Release;
	DiagFSet	= Diag_Parameter.OAV_Fault;
//	DiagFClr	= Diag_Parameter.OAV_F_Release;
	DiagFlagBit	= DIAG_B_V_OVER_AUX;

	/* Info */
	if (DiagValue > DiagISet)
	{
	    ISetFlag	= TRUE;
	}
	else
	{
	    ISetFlag	= FALSE;
    }
	if (DiagValue < DiagIClr)
	{
	    IClrFlag	= TRUE;
    }
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if (DiagValue > DiagWSet)
	{
	    WSetFlag	= TRUE;
    }
	else
	{
	    WSetFlag	= FALSE;
	}
	if (DiagValue < DiagWClr)
	{
	    WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if (DiagValue > DiagFSet)
	{
	    FSetFlag	= TRUE;
	}
	else
	{
	    FSetFlag	= FALSE;
	}
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Auxiliary Under voltage warning / fault
* @param	void
* @return	void
*/
void Diag_VUnderAux(void)
{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint16	DiagFSet;
//	uint16	DiagFClr;
	uint32	DiagFlagBit;

	DiagValue	= AuxVolt_Get_V_Aux();
	DiagISet	= Diag_Parameter.UAV_W_Release;
	DiagIClr	= Diag_Parameter.UAV_W_Release;
	DiagWSet	= Diag_Parameter.UAV_Warning;
	DiagWClr	= Diag_Parameter.UAV_W_Release;
	DiagFSet	= Diag_Parameter.UAV_Fault;
//	DiagFClr	= Diag_Parameter.UAV_F_Release;
	DiagFlagBit	= DIAG_B_V_UNDER_AUX;

	/* Info */
	if (DiagValue < DiagISet)
	{
	    ISetFlag	= TRUE;
    }
	else
	{
	    ISetFlag	= FALSE;
	}
	if (DiagValue > DiagIClr)
	{
	    IClrFlag	= TRUE;
	}
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if (DiagValue < DiagWSet)
	{
	    WSetFlag	= TRUE;
	}
	else
	{
	    WSetFlag	= FALSE;
	}
	if (DiagValue > DiagWClr)
	{
	    WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if (DiagValue < DiagFSet)
	{
	    FSetFlag	= TRUE;
    }
	else
	{
	    FSetFlag	= FALSE;
	}
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Positive Relay warning / fault
* @param	void
* @return	void
*/
void Diag_RelayErrPos(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= 1;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint8	DiagValue;
	uint32	DiagFlagBit;

	#ifdef Test_Diag_RelayErrPos
	DiagValue = 0x4u;
	#else
	DiagValue = Relay_Get_Relay_OP_ErrCode();
	#endif
	
	DiagFlagBit	= DIAG_B_RELAY_ERR_POS;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;

	/* Fault */
	if ((DiagValue & RELAY_POS_FLAG) != 0u)
	{
		FSetFlag	= TRUE;
	}
	else
	{
		FSetFlag	= FALSE;
	}
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Negative Relay warning / fault
* @param	void
* @return	void
*/
void Diag_RelayErrNeg(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= 1;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint8	DiagValue;
	uint32	DiagFlagBit;

	#ifdef Test_Diag_RelayErrNeg
	DiagValue = 0x2u;
	#else
	DiagValue = Relay_Get_Relay_OP_ErrCode();
	#endif
	
	DiagFlagBit	= DIAG_B_RELAY_ERR_NEG;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;

	/* Fault */
	if ((DiagValue & RELAY_NEG_FLAG) != 0u)
	{
		FSetFlag	= TRUE;
	}
	else
	{
		FSetFlag	= FALSE;
	}
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Precharge Relay warning / fault
* @param	void
* @return	void
*/
void Diag_RelayErrPre(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= 1;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint8	DiagValue;
	uint32	DiagFlagBit;

	#ifdef Test_Diag_RelayErrPre
	DiagValue = 0x1u;
	#else
	DiagValue = Relay_Get_Relay_OP_ErrCode();
	#endif
	
	DiagFlagBit	= DIAG_B_RELAY_ERR_PRE;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;

	/* Fault */
	if ((DiagValue & RELAY_PRE_FLAG) != 0u)
	{
		FSetFlag	= TRUE;
	}
	else
	{
		FSetFlag	= FALSE;
	}
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Fuse Dead fault
* @param	void
* @return	void
*/
void Diag_DeadFuse(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagFSet;
//	uint16	DiagFClr;
	uint32	DiagFlagBit;

	#ifdef Test_Diag_DeadFuse
	DiagValue = 1999u;
	#else
	DiagValue	= PackVolt_Get_V_PackMeas();
	#endif
	
	DiagFSet	= Diag_Parameter.FUSE_Fault;
//	DiagFClr	= Diag_Parameter.FUSE_F_Release;
	DiagFlagBit	= DIAG_B_DEAD_FUSE;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;

	/* Fault */
	if ((HvilEpo_IsNormal() == TRUE) && (DiagValue < DiagFSet))
	{
	    FSetFlag	= TRUE;
	}
	else
	{
	    FSetFlag	= FALSE;
    }
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Cell Dead warning / fault
* @param	void
* @return	void
*/
void Diag_DeadCell(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagFSet;
//	uint16	DiagFClr;
	uint32	DiagFlagBit;

	SYSTEM_STATE	System_State;

	#ifdef Test_Diag_DeadCell
	DiagValue = 1999u;
	#else
	DiagValue	= Adbms_Get_V_CellMin();
	#endif

	System_State = ManaMsm_Get_System_State();
	
	DiagFSet	= Diag_Parameter.DEAD_CELL_Fault;
//	DiagFClr	= Diag_Parameter.DEAD_CELL_F_Release;
	DiagFlagBit	= DIAG_B_DEAD_CELL;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;

	/* Fault */
	if ( (DiagValue < DiagFSet)
	  && (System_State != SYS_SHUTDOWN) )
	{
	    FSetFlag	= TRUE;
	}
	else
	{
	    FSetFlag	= FALSE;
	}
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Low battery Warning
* @param	void
* @return	void
*/
void Diag_SocLow(void)

{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint32	DiagFlagBit;

	DiagValue	= SOC_Get_SOC_Real();
	DiagISet	= Diag_Parameter.LSOC_W_Release;
	DiagIClr	= Diag_Parameter.LSOC_W_Release;
	DiagWSet	= Diag_Parameter.LSOC_Warning;
	DiagWClr	= Diag_Parameter.LSOC_W_Release;
	DiagFlagBit	= DIAG_B_SOC_LOW;

	/* Info */
	if (DiagValue < DiagISet)
	{
	    ISetFlag	= TRUE;
	}
	else
	{
	    ISetFlag	= FALSE;
	}
	if (DiagValue > DiagIClr)
	{
	    IClrFlag	= TRUE;
	}
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if (DiagValue < DiagWSet)
	{
	    WSetFlag	= TRUE;
	}
	else
	{
	    WSetFlag	= FALSE;
	}
	if (DiagValue > DiagWClr)
	{
	    WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	FSetFlag	= FALSE;
	FClrFlag	= TRUE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Imbalanced cell voltage
* @param	void
* @return	void
*/
void Diag_VImbalCell(void)
{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint16	DiagFSet;
	uint16	DiagFClr;
	uint32	DiagFlagBit;

	DiagValue	= Adbms_Get_V_CellDev();
	DiagISet	= Diag_Parameter.CIBV_W_Release;
	DiagIClr	= Diag_Parameter.CIBV_W_Release;
	DiagWSet	= Diag_Parameter.CIBV_Warning;
	DiagWClr	= Diag_Parameter.CIBV_W_Release;
	DiagFSet	= Diag_Parameter.CIBV_Fault;
	DiagFClr	= Diag_Parameter.CIBV_F_Release;
	DiagFlagBit	= DIAG_B_V_IMBAL_CELL;

	/* Info */
	if (DiagValue > DiagISet)
	{
	    ISetFlag	= TRUE;
	}
	else
	{
	    ISetFlag	= FALSE;
	}
	if (DiagValue < DiagIClr)
	{
	    IClrFlag	= TRUE;
	}
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if (DiagValue > DiagWSet)
	{
	    WSetFlag	= TRUE;
	}
	else
	{
	    WSetFlag	= FALSE;
	}
	if (DiagValue < DiagWClr)
	{
	    WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if (DiagValue > DiagFSet)
	{
	    FSetFlag	= TRUE;
	}
	else
	{
	    FSetFlag	= FALSE;
	}
	if (DiagValue < DiagFClr)
	{
	    FClrFlag	= TRUE;
	}
	else
	{
	    FClrFlag	= FALSE;
    }

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Imbalanced module voltage
* @param	void
* @return	void
*/
void Diag_VImbalModule(void)
{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16	DiagValue;
	uint16	DiagISet;
	uint16	DiagIClr;
	uint16	DiagWSet;
	uint16	DiagWClr;
	uint16	DiagFSet;
	uint16	DiagFClr;
	uint32	DiagFlagBit;

	DiagValue	= Adbms_Get_V_ModDev();
	DiagISet	= Diag_Parameter.MIBV_W_Release;
	DiagIClr	= Diag_Parameter.MIBV_W_Release;
	DiagWSet	= Diag_Parameter.MIBV_Warning;
	DiagWClr	= Diag_Parameter.MIBV_W_Release;
	DiagFSet	= Diag_Parameter.MIBV_Fault;
	DiagFClr	= Diag_Parameter.MIBV_F_Release;
	DiagFlagBit	= DIAG_B_V_IMBAL_MODULE;

	/* Info */
	if (DiagValue > DiagISet)
	{
	    ISetFlag	= TRUE;
	}
	else
	{
	    ISetFlag	= FALSE;
	}
	if (DiagValue < DiagIClr)
	{
	    IClrFlag	= TRUE;
	}
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	if (DiagValue > DiagWSet)
	{
	    WSetFlag	= TRUE;
	}
	else
	{
	    WSetFlag	= FALSE;
	}
	if (DiagValue < DiagWClr)
	{
	    WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	if (DiagValue > DiagFSet)
	{
	    FSetFlag	= TRUE;
	}
	else
	{
	    FSetFlag	= FALSE;
	}
	if (DiagValue < DiagFClr)
	{
	    FClrFlag	= TRUE;
    }
	else
	{
	    FClrFlag	= FALSE;
    }

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    
	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged EPO fault
* @param	void
* @return	void
*/
void Diag_HvilOpen(void)
{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME2;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint32	DiagFlagBit;

	DiagFlagBit	= DIAG_B_HVIL_OPEN;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;

	/* Fault */
	if (HvilEpo_IsNormal() == FALSE)
	{
	    FSetFlag	= TRUE;
	}
	else
	{
	    FSetFlag	= FALSE;
	}
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Imbalanced Cell Temperature
* @param	void
* @return	void
*/
void Diag_TImbalCell(void)
{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	sint16	DiagValue;
	sint16	DiagISet;
	sint16	DiagIClr;
	uint32	DiagFlagBit;

	DiagValue	= Adbms_Get_T_CellDev();
	DiagISet	=  200;
	DiagIClr	=  200;
	DiagFlagBit	= DIAG_B_T_IMBAL_CELL;

	/* Info */
	if (DiagValue > DiagISet)
	{
	    ISetFlag	= TRUE;
	}
	else
	{
	    ISetFlag	= FALSE;
	}
	if (DiagValue < DiagIClr)
	{
	    IClrFlag	= TRUE;
	}
	else
	{
	    IClrFlag	= FALSE;
    }

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;

	/* Fault */
	FSetFlag	= FALSE;
	FClrFlag	= TRUE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Cell Volatage Balancing fault
* @param	void
* @return	void
*/
void Diag_BalErrVCell(void)
{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint32	DiagFlagBit;

	DiagFlagBit	= DIAG_B_BAL_ERR_VCELL;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	if (Adbms_GetErrorDischargeResistor() == TRUE)
	{
		WSetFlag	= TRUE;
	}
	else
	{
	    WSetFlag	= FALSE;
    }
	if (Adbms_GetErrorDischargeResistor() == FALSE)
	{
		WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	FSetFlag	= FALSE;
	FClrFlag	= TRUE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Cell Volatage Sensor Offline fault
* @param	void
* @return	void
*/
void Diag_OfflineVCell(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint32	DiagFlagBit;
	bool	Diagvalue;

	#ifdef Test_Diag_OfflineVCell
	Diagvalue = TRUE;
	#else
	Diagvalue = Adbms_GetErrorVCellOffline();
	#endif

	DiagFlagBit	= DIAG_B_OFFLINE_VCELL;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;

	/* Fault */
	if (Diagvalue == TRUE)
	{
	    FSetFlag	= TRUE;
		Set_TROfflineVCell(TRUE);
	}
	else
	{
	    FSetFlag	= FALSE;
		Set_TROfflineVCell(FALSE);
		
	}
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    
	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Cell Temperature Sensor Offline fault
* @param	void
* @return	void
*/
void Diag_OfflineTCell(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint32	DiagFlagBit;
	bool 	Diagvalue;

	#ifdef Test_Diag_OfflineTCell
	Diagvalue = TRUE;
	#else
	Diagvalue = Adbms_GetErrorTCellOffline();
	#endif

	DiagFlagBit	= DIAG_B_OFFLINE_TCELL;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;

	/* Fault */
	if (Diagvalue == TRUE)
	{
	    FSetFlag	= TRUE;
		Set_TROfflineTCell(TRUE);
	}
	else
	{
	    FSetFlag	= FALSE;
		Set_TROfflineTCell(FALSE);
    }
    FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Judged Current Sensor Offline fault
* @param	void
* @return	void
*/
void Diag_OfflineISen(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint32	DiagFlagBit;
	bool currOffline;
	bool refFault;

	#ifdef Test_Diag_OfflineISen
	currOffline = TRUE;
	refFault = FALSE;
	#else
	currOffline = Curr_IsSensorOffline();
	refFault    = Ref5V_IsFault();
	#endif

	DiagFlagBit	= DIAG_B_OFFLINE_ISEN;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;

	/* Fault */
	if((currOffline == TRUE) || (refFault == TRUE))
	{
		FSetFlag	= TRUE;
	}
	else
	{
    	FSetFlag	= FALSE;
	}
	FClrFlag	= FALSE;

	/* Permanent Fault */
	    PSetFlag	= FALSE;
    

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}



/*---------------------------------------------------------------------*/
/**
* @brief	Judged ClassB Test fault
* @param	void
* @return	void
*/
void Diag_ClassB(void)
{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint32	DiagFlagBit;

	DiagFlagBit	= DIAG_B_CLASSB_ERR;


	/* Info */
		ISetFlag	= FALSE;
		IClrFlag	= TRUE;

	/* Warning */
	if (Classb_Data.FailCode != 0u)
	{
	    WSetFlag	= TRUE;
    }
	else
	{
	    WSetFlag	= FALSE;
	}
	if (Classb_Data.FailCode == 0u)
	{
	    WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }
	/* Fault */
	FSetFlag	= FALSE;
	FClrFlag	= TRUE;

	/* Permanent Fault */
	PSetFlag	= FALSE;

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}

/*---------------------------------------------------------------------*/
/**
* @brief	Judged Fan Status fault
* @param	void
* @return	void
*/
void Diag_Fan_Status(void)
{
	static	uint8	ISetCnt;
	static	uint8	IClrCnt;
	static	uint8	WSetCnt;
	static	uint8	WClrCnt;
	static	uint8	FSetCnt;
	static	uint8	FClrCnt;
	static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint32	DiagFlagBit;
	uint8  	Fan_On_Num;
	uint16	NumOf_Bmic;

	DiagFlagBit	= DIAG_B_FAN_Error;
	Fan_On_Num = Adbms_Get_Fan_On_Num();
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();	

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	if((Fan_GetState() == FAN_SW_ON) && (Fan_On_Num != (NumOf_Bmic / 2u)))
	{
		WSetFlag	= TRUE;
	}
	else
	{
	    WSetFlag	= FALSE;
	}
	if((Fan_GetState() == FAN_SW_ON) && (Fan_On_Num == (NumOf_Bmic / 2u)))
	{
		WClrFlag	= TRUE;
	}
	else
	{
	    WClrFlag	= FALSE;
    }

	/* Fault */
	FSetFlag	= FALSE;
	FClrFlag	= TRUE;

	/* Permanent Fault */
	PSetFlag	= FALSE;

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}



/*---------------------------------------------------------------------*/
/**
* @brief	Judged deviation with Pack voltage & Cell voltage sum
* @param	void
* @return	void
*/
void Diag_PackVCellV(void)
{
	CLASSB static	uint8	ISetCnt;
	CLASSB static	uint8	IClrCnt;
	CLASSB static	uint8	WSetCnt;
	CLASSB static	uint8	WClrCnt;
	CLASSB static	uint8	FSetCnt;
	CLASSB static	uint8	FClrCnt;
	CLASSB static	uint8	PSetCnt;

	uint8	ISetSpan	= DIAG_ISET_TIME;
	uint8	IClrSpan	= DIAG_ICLR_TIME;
	uint8	WSetSpan	= DIAG_WSET_TIME;
	uint8	WClrSpan	= DIAG_WCLR_TIME;
	uint8	FSetSpan	= DIAG_FSET_TIME;
	uint8	FClrSpan	= DIAG_FCLR_TIME;
	uint8	PSetSpan	= DIAG_PSET_TIME;

	BOOL	ISetFlag;
	BOOL	IClrFlag;
	BOOL	WSetFlag;
	BOOL	WClrFlag;
	BOOL	FSetFlag;
	BOOL	FClrFlag;
	BOOL	PSetFlag;

	/* Determine diagnostic conditions */
	uint16 CellSum;
	uint16 PackMeas;
	uint16 DiagValue;
	uint16 DiagPSet;
//	uint16 DiagFClr;
	uint32	DiagFlagBit;

	#ifdef Test_Diag_PackVCellV
	CellSum = 10000u;
	PackMeas = 10300u;
	#else	
	CellSum = Adbms_Get_V_CellSum();
	PackMeas = PackVolt_Get_V_PackMeas();
	#endif

	if(CellSum > PackMeas)
	{
		DiagValue = CellSum - PackMeas;
	}
	else
	{
		DiagValue = PackMeas - CellSum;
	}
	DiagPSet	=  200;
	DiagFlagBit	= DIAG_B_PackVCellV;

	/* Info */
	ISetFlag	= FALSE;
	IClrFlag	= TRUE;

	/* Warning */
	WSetFlag	= FALSE;
	WClrFlag	= TRUE;


	/* Fault */
	FSetFlag	= FALSE;
	FClrFlag	= TRUE;

	/* Permanent Fault */
	if (DiagValue > DiagPSet)
	{
	    PSetFlag	= TRUE;
	}
	else
	{
		PSetFlag	= FALSE;
    }

	/* Call the function to control the time span */
	Diag_ControlTimeSpan(	&ISetCnt, &IClrCnt, &WSetCnt, &WClrCnt,
							&FSetCnt, &FClrCnt, &PSetCnt,
							ISetSpan, IClrSpan, WSetSpan, WClrSpan,
							FSetSpan, FClrSpan, PSetSpan,
							ISetFlag, IClrFlag, WSetFlag, WClrFlag,
							FSetFlag, FClrFlag, PSetFlag,
							DiagFlagBit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set TROfflineVCell falg
* @param	void
* @return	void
*/
void Set_TROfflineVCell(BOOL SetV)
{
	TR_OFFLINE_VCELL = SetV;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get TROfflineVCell
* @param	void
* @return	void
*/
BOOL Get_TROfflineVCell(void)
{
    return TR_OFFLINE_VCELL;
}

/*---------------------------------------------------------------------*/
/**
* @brief	Set TROfflineVCell falg
* @param	void
* @return	void
*/
void Set_TROfflineTCell(BOOL SetV)
{
	TR_OFFLINE_TCELL = SetV;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get TROfflineVCell
* @param	void
* @return	void
*/
BOOL Get_TROfflineTCell(void)
{
    return TR_OFFLINE_TCELL;
}

/*---------------------------------------------------------------------*/
/**
* @brief	Set TROfflineVCell falg
* @param	void
* @return	void
*/
void Set_TROfflineBMIC(BOOL SetV)
{
	TR_COMM_ERR_BMIC = SetV;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get TROfflineVCell
* @param	void
* @return	void
*/
BOOL Get_TROfflineBMIC(void)
{
    return TR_COMM_ERR_BMIC;
}


/*---------------------------------------------------------------------*/
uint32 Diag_Get_ClassbFailcode(void)
{
    return Classb_Data.FailCode;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Diagnosis Conrtol function
* @param	pISetCnt - Inform Set timer
* @param	pIClrCnt - Inform Clear timer
* @param	pWSetCnt - Warning Set timer
* @param	pWClrCnt - Warning Clear timer
* @param	pFSetCnt - Fault Set timer
* @param	pFClrCnt - Fault Clear timer
* @param	pPSetCnt - Permanent Fault Set timer
* @param	ISetSpan - Inform Set Span
* @param	IClrSpan - Inform Clear Span
* @param	WSetSpan - Warning Set Span
* @param	WClrSpan - Warning Clear Span
* @param	FSetSpan - Fault Set Span
* @param	FClrSpan - Fault Clear Span
* @param	PSetSpan - Permanent Fault Set Span
* @param	ISetFlag - Inform Set Flag
* @param	IClrFlag - Inform Clear Flag
* @param	WSetFlag - Warning Set Flag
* @param	WClrFlag - Warning Clear Flag
* @param	FSetFlag - Fault Set Flag
* @param	FClrFlag - Fault Clear Flag
* @param	PSetFlag - Permanent Fault Set Flag
* @param	DiagFlagBit - Diagnosis Flag bit
* @return	void
*/
void Diag_ControlTimeSpan(	uint8* pISetCnt, uint8* pIClrCnt, uint8* pWSetCnt, uint8* pWClrCnt,
								uint8* pFSetCnt, uint8* pFClrCnt, uint8* pPSetCnt,
								uint8 ISetSpan, uint8 IClrSpan, uint8 WSetSpan, uint8 WClrSpan,
								uint8 FSetSpan, uint8 FClrSpan, uint8 PSetSpan,
								BOOL ISetFlag, BOOL IClrFlag, BOOL WSetFlag, BOOL WClrFlag,
								BOOL FSetFlag, BOOL FClrFlag, BOOL PSetFlag,
								uint32	DiagFlagBit)
{
	#ifdef Test_Diag_Module
	FSetFlag = TRUE;
	PSetFlag = FALSE;
	FClrFlag = FALSE;
	FSetSpan = 50u;
	DiagFlagBit = DIAG_B_V_OVER_PACK;
	#endif

	/* Info */
	if ( (FlagsWarn & DiagFlagBit) != 0u )
	{
		/* Set the Info flag */
		FlagsInfo |= DiagFlagBit;
		*pIClrCnt	= 0;
	}
	else
	{
		if      ( (FlagsInfo & DiagFlagBit) == 0u )
		{
			if (ISetFlag == TRUE)
			{
				*pISetCnt	+= 1u;
				if (*pISetCnt >= ISetSpan)
				{
					/* Set the Info flag */
					FlagsInfo |= DiagFlagBit;
					*pIClrCnt	= 0u;
				}
			}
			else
            {
				*pISetCnt	= 0u;
            }
		}
		else// if ( (Main_PackData.FlagsInfo & DiagFlagBit) != 0 )
		{
			if (IClrFlag == TRUE)
			{
				*pIClrCnt	+= 1u;
				if (*pIClrCnt >= IClrSpan)
				{
					/* Clear the Info flag */
					FlagsInfo &= ~(DiagFlagBit);
					*pISetCnt	= 0u;
				}
			}
			else
            {
				*pIClrCnt	= 0u;
            }
		}
//		else;
	}

	/* Warning */
	if ( (FlagsFault & DiagFlagBit) != 0u )
	{
		/* Set the Warning flag */
		FlagsWarn |= DiagFlagBit;
		*pWClrCnt	= 0u;
	}
	else
	{
		if      ( (FlagsWarn & DiagFlagBit) == 0u )
		{
			if (WSetFlag == TRUE)
			{
				*pWSetCnt	+= 1u;
				if (*pWSetCnt >= WSetSpan)
				{
					/* Set the Warning flag */
					FlagsWarn |= DiagFlagBit;
					*pWClrCnt	= 0u;
				}
			}
			else
			{
                *pWSetCnt	= 0u;
            }
		}
		else// if ( (Main_PackData.FlagsWarn & DiagFlagBit) != 0 )
		{
			if (WClrFlag == TRUE)
			{
				*pWClrCnt	+= 1u;
				if (*pWClrCnt >= WClrSpan)
				{
					/* Clear the Warning flag */
					FlagsWarn &= ~(DiagFlagBit);
					*pWSetCnt	= 0u;
				}
				else
				{
				}
			}
			else
			{
				*pWClrCnt	= 0u;
			}
		}
//		else;
	}

	/* Fault */
	if ( (FlagsPFault & DiagFlagBit) != 0u )
	{
		/* Set the Fault flag */
		FlagsFault |= DiagFlagBit;
		*pFClrCnt	= 0u;
	}
	else
	{
		if      ( (FlagsFault & DiagFlagBit) == 0u )
		{
			if (FSetFlag == TRUE)
			{
				#ifdef TestMode
				Led_On(DRV_Led_Red);
				Led_Set_Period(DRV_Led_Red,		DRV_Led_Peri_0ms);
				#endif
				*pFSetCnt	+= 1u;
				if (*pFSetCnt >= FSetSpan)
				{
					/* Set the Fault flag */
					FlagsFault |= DiagFlagBit;
					*pFClrCnt	= 0u;
				}
				else
				{
				}
			}
			else
			{
				*pFSetCnt	= 0u;
			}
		}
		else// if ( (FlagsFault & DiagFlagBit) != 0 )
		{
			if (FClrFlag == TRUE)
			{
				*pFClrCnt	+= 1u;
				if (*pFClrCnt >= FClrSpan)
				{
					/* Clear the Fault flag */
					FlagsFault &= ~(DiagFlagBit);
					*pFSetCnt	= 0u;
				}
				else
				{
				}
			}
			else
			{
				*pFClrCnt	= 0u;
			}
		}
//		else;
	}

	/* Permanent Fault */
	if		( (FlagsPFault & DiagFlagBit) == 0u )
	{
		if (PSetFlag == TRUE)
		{
			*pPSetCnt	+= 1u;
			if (*pPSetCnt >= PSetSpan)
			{
				/* Set the Permanent Fault flag */
				#ifdef TestMode
				Led_Off(DRV_Led_Red);
				Led_Set_Period(DRV_Led_Red,		DRV_Led_Peri_0ms);
				#endif
				FlagsPFault |= DiagFlagBit;
			}
			else
			{

			}
		}
		else
		{
			*pPSetCnt	= 0u;
		}
	}
//	else;
}


/*---------------------------------------------------------------------*/
uint32 Diag_Get_FlagsInfo(void)
{
    return FlagsInfo;
}


/*---------------------------------------------------------------------*/
uint32 Diag_Get_FlagsWarn(void)
{
    return FlagsWarn;
}


/*---------------------------------------------------------------------*/
uint32 Diag_Get_FlagsFault(void)
{
    return FlagsFault;
}


/*---------------------------------------------------------------------*/
void Diag_Set_FlagsFault(uint32 fault)
{
    FlagsFault = fault;
}



/*---------------------------------------------------------------------*/
uint32 Diag_Get_FlagsPFault(void)
{
    return FlagsPFault;
}


/*---------------------------------------------------------------------*/
void Diag_Set_FlagsPFault(uint32 Pfault)
{
    FlagsPFault = Pfault;
}



/*---------------------------------------------------------------------*/
#ifdef IEC60730_LIB   
CLASSB STL_MemSubset_t FlashSubsetRunTime;
CLASSB STL_MemConfig_t FlashConfigRunTime;
CLASSB STL_MemSubset_t RamSubsetRunTime;
CLASSB STL_MemConfig_t RamConfigRunTime;
CLASSB STL_TmStatus_t StlCpuTm1LStatus;
CLASSB STL_TmStatus_t StlCpuTm7Status;
CLASSB STL_TmStatus_t StlCpuTmCBStatus;
CLASSB STL_TmStatus_t StlFlashStatus;
CLASSB STL_TmStatus_t StlRamStatus;
CLASSB STL_TmStatus_t StlClkStatus;
CLASSB volatile uint16_t tmpCC4_last;
CLASSB volatile uint32_t PeriodValue;
CLASSB volatile uint32_t PeriodValueInv;
CLASSB volatile uint32_t LSIPeriodFlag;
static STL_Status_t STL_InitClock_Xcross_Measurement(void);
static STL_Status_t STL_RunClockTest(STL_TmStatus_t *clk_sts);
void FailSafe_Handler(uint32_t fail_code);
void checkFlashCRC(STL_TmStatus_t* result);
void STL_RunStep(void);
void TIM5_IRQHandler(void);
extern CRC_HandleTypeDef hcrc;

CLASSB STL_CntInterrupt StlCntInterrupt;
#ifdef Drawer_14ea
CLASSB_CRC const uint32_t ClassB_Golden_CRC = 0xBBFF9058UL;
#else
CLASSB_CRC const uint32_t ClassB_Golden_CRC = 0xA4D84894UL;
#endif


/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static STL_Status_t STL_InitClock_Xcross_Measurement(void)
{
  CLASSB static TIM_HandleTypeDef Htim5;
  TIM_IC_InitTypeDef sConfigIC = {0};

  __HAL_RCC_TIM5_CLK_ENABLE();

  /* TIM5 IC init */
  Htim5.Instance = TIM5;
  Htim5.Init.Prescaler = 0;
  Htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  Htim5.Init.Period = 0xFFFFFFFFU;   // 32bit max
  Htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  Htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_IC_Init(&Htim5) != HAL_OK)
  {
    return STL_KO;
  }

  /* Configure the Input Capture channel */
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV8;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&Htim5, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    return STL_KO;
  }

  /* Remap to LSI */
  if (HAL_TIMEx_RemapConfig(&Htim5, TIM_TIM5_LSI) != HAL_OK)
  {
    return STL_KO;
  }

  /* Enable CC4 interrupt */
  __HAL_TIM_ENABLE_IT(&Htim5, TIM_IT_CC4);

  /* NVIC */
  HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM5_IRQn);

  /* Start timer WITH interrupt */
  if (HAL_TIM_IC_Start_IT(&Htim5, TIM_CHANNEL_4) != HAL_OK)
  {
    return STL_KO;
  }

  return STL_OK;
}


/**
  * @brief  Verification result of the cross check frequencies process
  * @param  : clk_sts  Pointer to Test Module status to be written to.
  * @retval : None
  */
STL_Status_t STL_RunClockTest(STL_TmStatus_t *clk_sts)
{
  STL_Status_t tst_res;
  uint32_t periodValue_Local = PeriodValue;
  uint32_t periodValueInv_Local = PeriodValueInv;
  uint32_t lsiPeriodFlag_Local = LSIPeriodFlag;

  *clk_sts = STL_PARTIAL_PASSED;
  /* next line can produce compilation warning due to accessing of two volatile
     variables making single result value integrity pair. The pair is changed
     simultaneously exclusively at TIM5 IRQ and its integrity is verified here
     before the clock measurement is compared if it fits within expected range */
  if (((periodValue_Local ^ periodValueInv_Local) == 0xFFFFFFFFuL)\
  &&   (lsiPeriodFlag_Local == 0xAAAA5555u) )
  {
  #ifndef ARTI_FAILING_CLK_TM
    *clk_sts = STL_PASSED;
    if (PeriodValue < CLK_LimitLow(SYSTCLK_AT_RUN))
    {
      *clk_sts = STL_FAILED;    /* Sub-harmonics: HSE -25% below expected */
    }
    if (PeriodValue > CLK_LimitHigh(SYSTCLK_AT_RUN))
    {
      *clk_sts = STL_FAILED;    /* Harmonics: HSE +25% above expected */
    }
  #else
    *clk_sts = STL_FAILED;
  #endif /* ARTI_FAILING_CLK_TM */

    /* clear flag here to ensure the latest LSI measurement result will be taken into account at next check */
    LSIPeriodFlag = 0u;
    tst_res = STL_OK;
  }
  else
  {
    tst_res = STL_KO; /* Clock measurement flow error */
  }
  return(tst_res);
}

void FailSafe_Handler(uint32_t fail_code)
{
	uint8	TxData[APP_CanVcuTx_MaxBuf_Size]	= {0u};
	static	CAN_TxHeaderTypeDef CanVcuTx_hdr = {0};
	CanVcuTx_hdr.IDE = CAN_ID_EXT;
	CanVcuTx_hdr.RTR = CAN_RTR_DATA;

	Classb_Data.FailCode = fail_code;

	/* External WatchDog Off */
	HAL_GPIO_WritePin(RST_WDT_EN_GPIO_Port, RST_WDT_EN_Pin, GPIO_PIN_RESET);

	/* Relay Off */
	if(Curr_Get_I_PackMeas() > 10)
	{
		Relay_SetNegRelayLow(RELAY_OFF);
		HAL_Delay(500u);
		Relay_SetPosRelayLow(RELAY_OFF);
	}
	else if(Curr_Get_I_PackMeas() < (10 * (-1)))
	{
		Relay_SetPosRelayLow(RELAY_OFF);
		HAL_Delay(500u);
		Relay_SetNegRelayLow(RELAY_OFF);
	}
	else
	{
		Relay_SetNegRelayLow(RELAY_OFF);
		HAL_Delay(100u);
		Relay_SetPosRelayLow(RELAY_OFF);
	}

	/* EEPROM Save */
	while(EepromEx_Update_CLASSB_PfaultLog() == FALSE)
	{
		HAL_Delay(5u);		
	}

	while (TRUE)
	{
		CanVcuTx_hdr.ExtId 	= APP_CanVcuTx_Id_ClassB_Fail;
		CanVcuTx_hdr.DLC	= 8u;
		TxData[0] = (uint8)Classb_Data.FailCode;
		TxData[1] = (uint8)( ((uint16)StlCntInterrupt.cntIntrCANRx >> 8u) & 0xFFu );
		TxData[2] = (uint8)( ((uint16)StlCntInterrupt.cntIntrCANRx >> 0u) & 0xFFu );
		(void)Can_Send_MsgFrame( APP_CanVcuTx_Port, &CanVcuTx_hdr, TxData );
			
		Led_On(DRV_Led_Red);
			Led_On(DRV_Led_Yellow);
			Led_On(DRV_Led_Green);
		HAL_Delay(100u);
		Led_Off(DRV_Led_Red);
			Led_Off(DRV_Led_Yellow);
			Led_Off(DRV_Led_Green);
		HAL_Delay(900u);
		(void)HAL_IWDG_Refresh(&hiwdg);
	}
}

/**
  * @brief Single test sequence configuration
  * @param  None
  * @retval None
  */
void STL_Init(void)
{
  //Flash test config
  FlashSubsetRunTime.StartAddr = TEST_ROM_START_ADDR;
  FlashSubsetRunTime.EndAddr = TEST_ROM_END_ADDR;
  FlashSubsetRunTime.pNext = NULL;
  FlashConfigRunTime.pSubset = &FlashSubsetRunTime;
  FlashConfigRunTime.NumSectionsAtomic = 1; /* split test into sections of 1kB */


   /* RAM test Config
     ***************
     In this example, single continuous area (subset) is tested under unique configuration setting.
     User can apply wider set of subsets tested sequentially to cover separated memory areas
     as well as different configurations to be applied for selected subset(s) - see UM */
  RamSubsetRunTime.StartAddr = TEST_RAM_START_ADDR;
  RamSubsetRunTime.EndAddr = TEST_RAM_START_ADDR + TEST_RAM_SECTION_NB_RUN * RAM_SECTION_SIZE - 1u;
  RamSubsetRunTime.pNext = NULL;
  RamConfigRunTime.pSubset = &RamSubsetRunTime;
  RamConfigRunTime.NumSectionsAtomic = 1; /* split test into sections of 128 bytes */
  /* RamConfig.NumSectionsAtomic = (2 * TEST_RAM_SECTION_NB);  */  /* - use this setting for one shot */


  /* test status initialization */
  StlCpuTm1LStatus = STL_NOT_TESTED;
  StlCpuTm7Status = STL_NOT_TESTED;
  StlCpuTmCBStatus = STL_NOT_TESTED;
  StlFlashStatus= STL_NOT_TESTED;
  StlRamStatus= STL_NOT_TESTED;

  if (STL_SCH_Init() != STL_OK)
  {
    FailSafe_Handler(SCH_ERR_CODE + DEF_PROG_OFFSET);
  }

  /* Clock test Config */
  if(STL_InitClock_Xcross_Measurement() != STL_OK)
  {
    FailSafe_Handler(CXM_ERR_CODE + DEF_PROG_OFFSET);
  }

  /* Wait for two subsequent LSI periods measurements to perform initial crosscheck computation */
  LSIPeriodFlag = 0u;
  while (LSIPeriodFlag == 0u)
  { }
  LSIPeriodFlag = 0u;
  while (LSIPeriodFlag == 0u)
  { }
}

/**
  * @brief CRC calculation in flash area
  * @param  result: STL_PASSED->no problem
  * @retval STL_PASSED: no problem, others: error
  */

void checkFlashCRC(STL_TmStatus_t* result)
{
  volatile uint32_t flash_crc32 = 0u;
  uint32_t total_len = ((TEST_ROM_END_ADDR - TEST_ROM_START_ADDR) / 4u);  // Convert bytes to words (32-bit)

  *result = STL_ERROR;
  
  // HAL CRC Initialization
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    return;
  }

  // Flash CRC calculation
  flash_crc32 = HAL_CRC_Calculate(&hcrc, *(uint32_t (*)[])TEST_ROM_START_ADDR, total_len);
  Classb_Data.crcNum = flash_crc32;

  // CRC compare
  if (flash_crc32 == ClassB_Golden_CRC)
  {
    *result = STL_PASSED;
  }

  // HAL CRC Deinit
  (void)HAL_CRC_DeInit(&hcrc);
}


/**
  * @brief Full CPU, FLASH, RAM, and clock test sequence to be run at application startup
  * @param  None
  * @retval None
  */
void STL_RunFullTest(void)
{
  /* CPU TM1L */
  if (STL_SCH_RunCpuTM1L(&StlCpuTm1LStatus) != STL_OK)
  {
    FailSafe_Handler(TM1L_ERR_CODE + DEF_PROG_OFFSET);
  }
  if (StlCpuTm1LStatus != STL_PASSED)
  {
    FailSafe_Handler(TM1L_ERR_CODE);
  }
  /* CPU TM7 */
  if (STL_SCH_RunCpuTM7(&StlCpuTm7Status) != STL_OK)
  {
    FailSafe_Handler(TM7_ERR_CODE + DEF_PROG_OFFSET);
  }
  if (StlCpuTm7Status != STL_PASSED)
  {
    FailSafe_Handler(TM7_ERR_CODE);
  }
  /* CPU TMCB */
  if (STL_SCH_RunCpuTMCB(&StlCpuTmCBStatus) != STL_OK)
  {
    FailSafe_Handler(TMCB_ERR_CODE + DEF_PROG_OFFSET);
  }
  if (StlCpuTmCBStatus != STL_PASSED)
  {
    FailSafe_Handler(TMCB_ERR_CODE);
  }

  //Flash CRC check
  checkFlashCRC(&StlFlashStatus);

  #ifndef TestMode
  if (StlFlashStatus != STL_PASSED)
  {
    FailSafe_Handler(TMF_ERR_CODE);
  }
  StlFlashStatus = STL_NOT_TESTED;
  #endif

  /* The following configuration tests the entire user-defined range of the RAM used by the program */
  /* RAM subset: */
  STL_MemSubset_t RamSubsetFullTest = {
      .StartAddr = TEST_RAM_START_ADDR,
      .EndAddr   = TEST_RAM_END_ADDR_FULL,
      .pNext     = NULL
  };
  STL_MemConfig_t RamConfigFullTest = {
      .NumSectionsAtomic = UINT32_MAX,
      .pSubset = &RamSubsetFullTest
  };

  	(void)RamSubsetFullTest.StartAddr;
	(void)RamSubsetFullTest.EndAddr;
	(void)RamSubsetFullTest.pNext;

  if (STL_SCH_InitRam(&StlRamStatus) != STL_OK)
  {
    FailSafe_Handler(TMR_ERR_CODE + DEF_PROG_OFFSET);
  }
  
  if (STL_SCH_ConfigureRam(&StlRamStatus, &RamConfigFullTest) != STL_OK)
  {
    FailSafe_Handler(TMR_ERR_CODE + DEF_PROG_OFFSET);
  }
  else if (StlRamStatus != STL_NOT_TESTED)
  {
    FailSafe_Handler(TMR_ERR_CODE);
  }
  else
  {

  }
	
  
  if (STL_SCH_RunRamTM(&StlRamStatus) != STL_OK)
  {
    FailSafe_Handler(TMR_ERR_CODE + DEF_PROG_OFFSET);
  }
  else if (StlRamStatus != STL_PASSED)
  {
    FailSafe_Handler(TMR_ERR_CODE);
  }
  else
  {

  }
	
  StlRamStatus = STL_NOT_TESTED;

#if defined(ARTI_FAILING_RAM_TM) && defined(ARTI_FAILING_DURING_FULL_TEST)
  STL_SCH_StopArtifFailing();
#endif /* ARTI_FAILING_RAM_TM */

  /************/
  /* Clock TM */
  /************/
  StlClkStatus = STL_NOT_TESTED;
  if (STL_RunClockTest(&StlClkStatus) != STL_OK)
  {
    FailSafe_Handler(CLK_ERR_CODE + DEF_PROG_OFFSET);
  }
  if (StlClkStatus != STL_PASSED)
  {
    FailSafe_Handler(CLK_ERR_CODE);
  }
}

/*----------------------------------------------------------------------------*/
/**
  * @brief Single partial step of the test sequence
  * @param  None
  * @retval None
  */
void STL_RunStep(void)
{
  /* Example of the test sequence execution
     **************************************
     In this example, a unique sequence performing all the tests in one shot is applied.
     Note it is fully upon user to define order and repetition frequency of all the API calls.
     The same sequence of the calls can be repeated regularly or few different sequences can be
     applied at different time slots. All the tests can be repeated without any limitation
     except for clock test which can not be called earlier than 8 LSI periods to prepare new clock
     computation result in between two comparisons (otherwise the clock API call fails). */

  CLASSB static uint8 count;

  /********************/
  /* CPU Test modules */
  /********************/

#ifdef ARTI_FAILING_CPU_TM
  /* Artificial failing feature -
     when activated, it forces the STL outputs to predefined values */
  ArtifFailing.aCpuTmStatus[0] = STL_PASSED;
  ArtifFailing.aCpuTmStatus[1] = STL_PASSED;
  ArtifFailing.aCpuTmStatus[2] = STL_FAILED;
  STL_SCH_StartArtifFailing(&ArtifFailing);
#endif /* ARTI_FAILING_CPU_TM */

  /* CPU TM1L */
  if (STL_SCH_RunCpuTM1L(&StlCpuTm1LStatus) != STL_OK)
  {
    FailSafe_Handler(TM1L_ERR_CODE + DEF_PROG_OFFSET);
  }
  if (StlCpuTm1LStatus != STL_PASSED)
  {
    FailSafe_Handler(TM1L_ERR_CODE);
  }
  /* CPU TM7 */
  if (STL_SCH_RunCpuTM7(&StlCpuTm7Status) != STL_OK)
  {
    FailSafe_Handler(TM7_ERR_CODE + DEF_PROG_OFFSET);
  }
  if (StlCpuTm7Status != STL_PASSED)
  {
    FailSafe_Handler(TM7_ERR_CODE);
  }
  /* CPU TMCB */
  if (STL_SCH_RunCpuTMCB(&StlCpuTmCBStatus) != STL_OK)
  {
    FailSafe_Handler(TMCB_ERR_CODE + DEF_PROG_OFFSET);
  }
  if (StlCpuTmCBStatus != STL_PASSED)
  {
    FailSafe_Handler(TMCB_ERR_CODE);
  }

  if (StlRamStatus == STL_NOT_TESTED)
  {
    /* Initialize & configure Flash TM */
    if (STL_SCH_InitRam(&StlRamStatus) != STL_OK)
    {
      FailSafe_Handler(TMR_ERR_CODE + DEF_PROG_OFFSET);
    }
    if (STL_SCH_ConfigureRam(&StlRamStatus, &RamConfigRunTime) != STL_OK)
    {
      FailSafe_Handler(TMR_ERR_CODE + DEF_PROG_OFFSET);
    }
    else if (StlRamStatus != STL_NOT_TESTED)
    {
      FailSafe_Handler(TMR_ERR_CODE);
    }
	else
	{
		
	}
  }
  /* run partial atomic test */
  if (STL_SCH_RunRamTM(&StlRamStatus) != STL_OK)
  {
    FailSafe_Handler(TMR_ERR_CODE + DEF_PROG_OFFSET);
  }
  if (StlRamStatus != STL_PARTIAL_PASSED)
  {
    if (StlRamStatus == STL_PASSED)
    {
      /* test completed successfully - to be restarted */
      StlRamStatus= STL_NOT_TESTED;
      #if defined(STL_EVAL_MODE)
        /* BSP_LED_Toggle(LED2); */
      #endif  /* STL_EVAL_MODE */
    }
    else
    {
      FailSafe_Handler(TMR_ERR_CODE);
    }
  }

  /************/
  /* Clock TM */
  /************/
  /* When artificial failing is activated for clock module, the failure status is
     forced by the API module itself available as open source code */
  StlClkStatus = STL_NOT_TESTED;
  if (STL_RunClockTest(&StlClkStatus) != STL_OK)
  {
    FailSafe_Handler(CLK_ERR_CODE + DEF_PROG_OFFSET);
  }
  if (StlClkStatus != STL_PASSED)
  {
    FailSafe_Handler(CLK_ERR_CODE);
  }

	//Interrupt handler check
	if(count <= 2u)
	{
		StlCntInterrupt.cntIntrCANRx = 0;
		count++;
	}
	else
	{
		if(StlCntInterrupt.cntIntrCANRx > 100u)
		{
			FailSafe_Handler(INTR_ERR_CODE + DEF_PROG_OFFSET);
		}
		else
		{
			StlCntInterrupt.cntIntrCANRx = 0;
		}
	}
}
#endif
/*----------------------------------------------------------------------------*/
/**
  * @brief TIM5 IC interrupt handler
  * @param None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
  uint16_t tmpCC4_last_cpy;

  if ((TIM5->SR & TIM_SR_CC4IF) != 0u )
  {
    /* store previous captured value */
    tmpCC4_last_cpy = tmpCC4_last;
    /* get currently captured value */
    tmpCC4_last = (uint16_t)(TIM5->CCR4);
    /* The CC4IF flag is already cleared here by reading CCR4 register */

    /* overwrite results only in case the data is required */
    if (LSIPeriodFlag == 0u)
    {
      /* Skip over the measurement at case of timer over capture */
      if ((TIM5->SR & TIM_SR_CC4OF) == 0u)
      {
        /* Compute period length and double store its value secured at integrity pair  */
        PeriodValue = ((uint32_t)(tmpCC4_last) - (uint32_t)(tmpCC4_last_cpy)) & 0xFFFFuL;
        PeriodValueInv = ~PeriodValue;
        /* Set Flag confirming secured storage tested at main loop */
        LSIPeriodFlag = 0xAAAA5555uL;
      }
      else
      {
        /* ignore computation in case of IC overcapture */
        TIM5->SR &= (uint16_t)(~TIM_SR_CC4OF);
      }
    }
    /* ignore computation in case data is not required */
  }
}

/*---------------------------------------------------------------------*/
/**
* @brief	Do safety checking
* @param	void
* @return	void
*/
void IEC60730_Task(void)
{
#ifdef IEC60730_LIB          
  STL_RunStep();
	

#endif
}


/*---------------------------------------------------------------------*/
/* End of App_Diag.c */
