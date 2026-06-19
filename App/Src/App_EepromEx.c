/*
 * Api_EepromEx.c
 *
 *  Created on: Sep 19, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include <App_EepromEx.h>


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define EEPROMEX_MAX_EVENTBUF				3000u

/* Index H , Index L, Year, Month, Date, Hour, Minute, Second, Warning H, Warning L, Fault H, Fault L, Reserved 1~4 */
#define EEPROMEX_EVENT_START_ADDR			3074u
#define EEPROMEX_EVENT_FORMAT_SIZE			16u

#define EEPROMEX_PEVENT_START_ADDR			53074u
#define EEPROMEX_PEVENT_FORMAT_SIZE			12u

#define EEPROMEX_EVENT_IDX_SAVE_ADDR		3072u
#define EEPROMEX_EVENT_IDX_SIZE				2u

#define EEPROMEX_PEVENT_IDX_SAVE_ADDR		53072u
#define EEPROMEX_PEVENT_IDX_SIZE			2u

#define EEPROMEX_SAVED_CONFIG				0xAAu
#define EEPROMEX_NOT_SAVED_CONFIG			0x55u

#define EEPROMEX_STRING_DATA_ADDR			2048u
#define EEPROMEX_STRING_DATA_LENGTH			48u

#define EEPROMEX_SAVED_PARAMETER_FLAG_ADDR	0x0000u
#define EEPROMEX_PARAMETER_BLOCK_SIZE_WORD	(uint16)64u
#define EEPROMEX_PARAMETER_BLOCK_SIZE_BYTE	(uint16)(EEPROMEX_PARAMETER_BLOCK_SIZE_WORD * 2u)

#define EEPROMEX_SAVED_CALIBDATA_FLAG_ADDR	0x0002u
#define	EEPROMEX_CALIBDATA_LENGTH			52u

/********************************************************************
*						Fault Category								*
*  @ Service														*
*	1. NEG_CONTACTOR 	0x00080000  - bit19							*
*	2. POS_CONTACTOR 	0x00100000  - bit20							*
*	3. PRE_CONTACTOR 	0x00040000  - bit18							*
*	5. FUSE_DEAD		0x00200000  - bit21							*
*	6. CELL_DEAD		0x00400000  - bit22							*
*	7. PackVCellV		0X20000000	- bit29							*
********************************************************************/
#define EEPROMEX_SERVICE_FAULT_CODE		0x207C0000u	
													// app_diagnosis.h 참조
#define	EEPROMEX_SERVICE_FAULT_ADDR		3070u

/********************************************************************
*						Permanent Fault Category					*
*  @ Service														*
*	1. V_OVER_CELL		0x00000001  - bit0							*
*	2. V_OVER_PACK		0x00000002  - bit1							*
*	3. V_UNDER_CELL		0x00000004  - bit2							*
*	4. V_UNDER_PACK		0x00000008  - bit3							*
*	5. T_OVER_CELL_CHG	0x00000010  - bit4							*
*	6. T_OVER_CELL_DCH	0x00000020  - bit5							*
*	7. T_UNDER_CELL_CHG	0x00000040  - bit6							*
*	8. T_UNDER_CELL_DCH	0x00000080  - bit7							*
*	9. ClassB			0x80000000  - bit31							*
********************************************************************/

#define EEPROMEX_SERVICE_PFAULT_CODE		0x800000FFu

#define	EEPROMEX_SERVICE_PFAULT_ADDR		53070u


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
CLASSB static uint16  EepromEx_PFaultIDX;
static uint16  EepromEx_EventIDX = 0;
static bool EepromEx_UpdateCapacityData_Flag = FALSE;
static uint8   EepromEx_ErrorFlag = 0;
static bool EepromEx_SaveFaultHistoryFlag = FALSE;
static bool  	EepromEx_SavePFaultHistoryFlag = FALSE;
static bool EepromEx_SaveEventIdxFlag = FALSE;
static bool  	EepromEx_SavePEventIdxFlag = FALSE;

CAPACITY_DATA	static EEPROM_CapacityData;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
BOOL EepromEx_Update_EventLog(void);
BOOL EepromEx_Update_PfaultLog(void);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
* @brief	External EEPROM Control Task
* @param	void
* @return	void
*/
void EepromEx_Control_Task(void)
{
#define CAPACITYDATA_SAVE_INTERVAL          6000u         // 10 minutes

    static uint32  old_fault = 0u;
	static uint32	old_pfault = 0u;
//	static uint32  old_warning = 0u;    
    static uint16  CapacityData_Save_Count = 0u;

	SYSTEM_STATE 	System_State;
	uint32	FlagsFault;
	uint32	FlagsPFault;

	System_State = ManaMsm_Get_System_State();
	FlagsFault = Diag_Get_FlagsFault();
	FlagsPFault = Diag_Get_FlagsPFault();
    
    if(((uint8)System_State > (uint8)SYS_INIT) && (FlagsFault != old_fault))
    {
		if(EepromEx_Update_EventLog() == TRUE)
		{
			old_fault = Diag_Get_FlagsFault();  
		}
    }
	else if(((uint8)System_State > (uint8)SYS_INIT) && (FlagsPFault != old_pfault))
    {
		if(EepromEx_Update_PfaultLog() == TRUE)
		{
			old_pfault = Diag_Get_FlagsPFault();
		}
    }
/*
    else if(Get_IsSaveConfigData() == TRUE)
    {
        if(EepromEx_WriteCfgData() == TRUE)
        {
            Set_SaveConfigData(FALSE);             
            tempParameterUpdate();
        }           
    }
*/	
    else if(Get_IsSaveWarrantyData() == TRUE)
    {
        Set_SaveWarrantyData(FALSE);
        WarrantyData_Save();
    }
    else if(EepromEx_UpdateCapacityData_Flag == TRUE)
    {
        EepromEx_UpdateCapacityData_Flag = FALSE;
        (void)EepromEx_WriteStringData();
    }
	else if(Get_IsSaveCalibData() == TRUE)
    {
        Set_SaveCalibData(FALSE);
        CalibData_Wrtie();
    }
	else if(EepromEx_Get_SaveFaultHistoryFlag() == TRUE)
    {
		EepromEx_Set_SaveFaultHistoryFlag(FALSE);
		(void)EepromEx_SetSavedFaultHistoryFlag(FALSE);
	}
	else if(EepromEx_Get_SavePFaultHistoryFlag() == TRUE)
    {
		EepromEx_Set_SavePFaultHistoryFlag(FALSE);
		(void)EepromEx_SetSavedPFaultHistoryFlag(FALSE);
	}
	else if(EepromEx_Get_SaveEventIdxFlag() == TRUE)
    {
		EepromEx_Set_SaveEventIdxFlag(FALSE);
		(void)EepromEx_SaveEventIndex();
	}
	else if(EepromEx_Get_SavePEventIdxFlag() == TRUE)
    {
		EepromEx_Set_SavePEventIdxFlag(FALSE);
		(void)EepromEx_SavePEventIndex();
	}
		
	else
	{   
        if(CapacityData_Save_Count >= (CAPACITYDATA_SAVE_INTERVAL - 1u))    
        {
            CapacityData_Save_Count = 0u;
            EepromEx_UpdateCapacityData_Flag = TRUE;
        }
        else if(CapacityData_Save_Count == (CAPACITYDATA_SAVE_INTERVAL / 2u))
        {
            Set_SaveWarrantyData(TRUE);    
			CapacityData_Save_Count++;
        }
        else
        {
            CapacityData_Save_Count++; 
        }
    }

}


/*---------------------------------------------------------------------*/
/**
* @brief	Update Event Log of External EEPROM
* @param	void
* @return	void
*/
BOOL EepromEx_Update_EventLog(void)
{
	BOOL rv = FALSE;
    uint8  ebuf[EEPROMEX_EVENT_FORMAT_SIZE];
    RTC_DateTypeDef RTCd;
    RTC_TimeTypeDef RTCt;
	static uint8 Step = 0u;
	uint32 addr;
	uint32 offset;
	static uint8 step2 = 0u;
	static uint32 addr2 = 0u;
	static uint32 offset2 = 0u;

	switch (Step)
	{
		case 0u:
			(void)HAL_RTC_GetTime(&hrtc, &RTCt, RTC_FORMAT_BIN);
			(void)HAL_RTC_GetDate(&hrtc, &RTCd, RTC_FORMAT_BIN);	
		    
	        ebuf[0] = (uint8)(EepromEx_EventIDX >> 8u);
	        ebuf[1] = (uint8)(EepromEx_EventIDX & 0x00FFu);
	        ebuf[2] = RTCd.Year;
			ebuf[3] = RTCd.Month;
			ebuf[4] = RTCd.Date;
			ebuf[5] = RTCt.Hours;
			ebuf[6] = RTCt.Minutes;
			ebuf[7] = RTCt.Seconds;
			ebuf[8] = (uint8)(Diag_Get_FlagsWarn() >> 24u);
	        ebuf[9] = (uint8)(Diag_Get_FlagsWarn() >> 16u);
	        ebuf[10] = (uint8)(Diag_Get_FlagsWarn() >> 8u);
	        ebuf[11] = (uint8)(Diag_Get_FlagsWarn() & 0xFFu);
	        ebuf[12] = (uint8)(Diag_Get_FlagsFault() >> 24u);
	        ebuf[13] = (uint8)(Diag_Get_FlagsFault() >> 16u);
	        ebuf[14] = (uint8)(Diag_Get_FlagsFault() >> 8u);
	        ebuf[15] = (uint8)(Diag_Get_FlagsFault() & 0xFFu);

			if(EepromEx_EventIDX < EEPROMEX_MAX_EVENTBUF)
		    {
				addr = (((uint32)EepromEx_EventIDX * (uint32)EEPROMEX_EVENT_FORMAT_SIZE) + (uint32)EEPROMEX_EVENT_START_ADDR);

				if((addr + EEPROMEX_EVENT_FORMAT_SIZE) > (EEPROMEX_PAGESIZE * ((addr / EEPROMEX_PAGESIZE) + 1u)))
				{
					switch (step2)
					{
						case 0u:
							offset = (EEPROMEX_PAGESIZE * ((addr / EEPROMEX_PAGESIZE) + 1u)) - addr;

				        	if(EepromEx_WriteData(addr, offset, ebuf) == TRUE)
				            {
				                addr2 = addr + offset;
								offset2 = EEPROMEX_EVENT_FORMAT_SIZE - offset;
				            }
							step2++;
							break;

						case 1u:
							if(EepromEx_WriteData(addr2, offset2, &ebuf[EEPROMEX_EVENT_FORMAT_SIZE - offset2]) == TRUE)
				            {
				                EepromEx_EventIDX++;
				            }
							step2 = 0u;
							Step++;
							break;

						default:
							break;
					}	
				}
						
				else
				{
		            if(EepromEx_WriteData(addr, EEPROMEX_EVENT_FORMAT_SIZE, ebuf) == TRUE)
		            {
		                EepromEx_EventIDX++;
		            }
					Step++;
				}
			}
		        
	        else
	        {
	            EepromEx_EventIDX = 0u;
				Step++;
	        }
			break;

		case 1u:	
        	(void)EepromEx_SaveEventIndex();
			Step++;
			break;

		case 2u:
	        if((Diag_Get_FlagsFault() & (uint32)EEPROMEX_SERVICE_FAULT_CODE) > (uint32)0)
	        {
	            (void)EepromEx_SetSavedFaultHistoryFlag(TRUE);
	        }
			rv = TRUE;
			Step = 0;
			break;

		default :	
			break;
	}

	return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Update Permanent Fault Log of External EEPROM
* @param	void
* @return	void
*/
BOOL EepromEx_Update_PfaultLog(void)
{
	BOOL rv = FALSE;
    uint8  ebuf[EEPROMEX_PEVENT_FORMAT_SIZE] = {0,};
    RTC_DateTypeDef RTCd;
    RTC_TimeTypeDef RTCt;
	static uint8 Step;
	uint32 addr;
	uint32 offset;
	static uint8 step2 = 0u;
	static uint32 addr2 = 0u;
	static uint32 offset2 = 0u;

	switch (Step)
	{
		case 0u:
			(void)HAL_RTC_GetTime(&hrtc, &RTCt, RTC_FORMAT_BIN);
			(void)HAL_RTC_GetDate(&hrtc, &RTCd, RTC_FORMAT_BIN);

	       	ebuf[0] = (uint8)(EepromEx_PFaultIDX >> 8u);
	        ebuf[1] = (uint8)(EepromEx_PFaultIDX & 0x00FFu);
	        ebuf[2] = RTCd.Year;  
	        ebuf[3] = RTCd.Month;
	        ebuf[4] = RTCd.Date;
	        ebuf[5] = RTCt.Hours;
	        ebuf[6] = RTCt.Minutes;
	        ebuf[7] = RTCt.Seconds;
	        ebuf[8] = (uint8)(Diag_Get_FlagsPFault() >> 24u);
	        ebuf[9] = (uint8)(Diag_Get_FlagsPFault() >> 16u);
	        ebuf[10] = (uint8)(Diag_Get_FlagsPFault() >> 8u);
	        ebuf[11] = (uint8)(Diag_Get_FlagsPFault() & 0xFFu);


			if(EepromEx_PFaultIDX < EEPROMEX_MAX_EVENTBUF)
		    {
				addr = (((uint32)EepromEx_PFaultIDX * (uint32)EEPROMEX_PEVENT_FORMAT_SIZE) + (uint32)EEPROMEX_PEVENT_START_ADDR);

				if((addr + EEPROMEX_PEVENT_FORMAT_SIZE) > (EEPROMEX_PAGESIZE * ((addr / EEPROMEX_PAGESIZE) + 1u)))
				{
					switch (step2)
					{
						case 0u:
							offset = (EEPROMEX_PAGESIZE * ((addr / EEPROMEX_PAGESIZE) + 1u)) - addr;

				        	if(EepromEx_WriteData(addr, offset, ebuf) == TRUE)
				            {
				                addr2 = addr + offset;
								offset2 = EEPROMEX_PEVENT_FORMAT_SIZE - offset;
				            }
							step2++;
							break;

						case 1u:
							if(EepromEx_WriteData(addr2, offset2, &ebuf[EEPROMEX_EVENT_FORMAT_SIZE - offset2]) == TRUE)
				            {
				                EepromEx_PFaultIDX++;
				            }
							step2 = 0u;
							Step++;
							break;

						default:
							break;
					}	
				}
						
				else
				{
		            if(EepromEx_WriteData(addr, EEPROMEX_PEVENT_FORMAT_SIZE, ebuf) == TRUE)
		            {
		                EepromEx_PFaultIDX++;
		            }
					Step++;
				}
			}
		        
	        else
	        {
	            EepromEx_PFaultIDX = 0u;
				Step++;
	        }
			break;

		case 1u:
			(void)EepromEx_SavePEventIndex();
			Step++;
			break;

		case 2u:
			if((Diag_Get_FlagsPFault() & (uint32)EEPROMEX_SERVICE_PFAULT_CODE) > (uint32)0u)
        	{
            	(void)EepromEx_SetSavedPFaultHistoryFlag(TRUE);
        	}
			rv = TRUE;
			Step = 0;
			break;

		default :	
			break;
	}

	return rv;
}



/*---------------------------------------------------------------------*/
/**
* @brief	Save Event Index of External EEPROM 
* @param	void
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_SaveEventIndex(void)
{
	uint8 buf[2] = {0};
    BOOL rv = FALSE;
    
	buf[0] = (uint8)(EepromEx_EventIDX & 0x00FFu);
	buf[1] = (uint8)(EepromEx_EventIDX >> 8u);

	if (EepromEx_WriteData(EEPROMEX_EVENT_IDX_SAVE_ADDR, EEPROMEX_EVENT_IDX_SIZE, buf) == TRUE)
	{
		rv = TRUE;
	}

    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Save Permanent fault Index of External EEPROM 
* @param	void
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_SavePEventIndex(void)
{
	uint8 buf[2] = {0};
    BOOL rv = FALSE;
    
	buf[0] = (uint8)(EepromEx_PFaultIDX & 0x00FFu);
	buf[1] = (uint8)(EepromEx_PFaultIDX >> 8u);

	if (EepromEx_WriteData(EEPROMEX_PEVENT_IDX_SAVE_ADDR, EEPROMEX_PEVENT_IDX_SIZE, buf) == TRUE)
	{
		rv = TRUE;
	}

    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Load Event Index of External EEPROM 
* @param	void
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_LoadEventIndex(void)
{
	uint8 buf[2] = {0, 0};
	BOOL rv = FALSE;	
    
	if(EepromEx_ReadData(EEPROMEX_EVENT_IDX_SAVE_ADDR, EEPROMEX_EVENT_IDX_SIZE, buf) == TRUE)
	{
		EepromEx_EventIDX = ((uint16)(buf[1]) << 8) + (uint16)(buf[0]);
		rv = TRUE;
	}		

	return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Load Permanent fault Index of External EEPROM 
* @param	void
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_LoadPEventIndex(void)
{
	uint8 buf[2] = {0, 0};
	BOOL rv = FALSE;	
    
	if(EepromEx_ReadData(EEPROMEX_PEVENT_IDX_SAVE_ADDR, EEPROMEX_PEVENT_IDX_SIZE, buf) == TRUE)
	{
		EepromEx_PFaultIDX = ((uint16)(buf[1]) << 8) + (uint16)(buf[0]);
		rv = TRUE;
	}		

	return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set Event Index of External EEPROM 
* @param	idx - Index value
* @return	void
*/
void EepromEx_SetEventIndex(uint16 idx)
{
	EepromEx_EventIDX = idx;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get External EEPROM Event Index
* @param	void
* @return	uint16 - Event Index
*/
uint16 EepromEx_GetEventIndex(void)
{
	return EepromEx_EventIDX;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set Permanent fault Index of External EEPROM 
* @param	idx - Index value
* @return	void
*/
void EepromEx_SetPEventIndex(uint16 idx)
{
	EepromEx_PFaultIDX = idx;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get Permanent fault EEPROM Event Index
* @param	void
* @return	uint16 - Event Index
*/
uint16 EepromEx_GetPEventIndex(void)
{
	return EepromEx_PFaultIDX;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set saved fault history of External EEPROM 
* @param	BOOL, TRUE - Save, FALSE - X
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_SetSavedFaultHistoryFlag(BOOL save)
{
	uint8 sv;
	BOOL rv = FALSE;
    
	if (save == TRUE)
    {
        sv = EEPROMEX_SAVED_CONFIG;
    }
	else
    {
        sv = EEPROMEX_NOT_SAVED_CONFIG;
    }
    
	if (EepromEx_WriteData(EEPROMEX_SERVICE_FAULT_ADDR, 1u, &sv) == TRUE)
    {
        rv = TRUE;
    }

    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set saved fault history of External EEPROM 
* @param	BOOL, TRUE - Save, FALSE - X
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_SetSavedPFaultHistoryFlag(BOOL save)
{
	uint8 sv;
	BOOL rv = FALSE;
    
	if (save == TRUE)
    {
        sv = EEPROMEX_SAVED_CONFIG;
    }
	else
    {
        sv = EEPROMEX_NOT_SAVED_CONFIG;
    }
    
	if (EepromEx_WriteData(EEPROMEX_SERVICE_PFAULT_ADDR, 1u, &sv) == TRUE)
    {
        rv = TRUE;
    }

    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get Event Log of External EEPROM 
* @param	index - index value
* @param	*buf - buf data
* @return	void
*/
void EepromEx_GetEventLog(uint16 index, uint8* buf)
{
	uint32 addr;

	addr = ((uint32)index * EEPROMEX_EVENT_FORMAT_SIZE) + EEPROMEX_EVENT_START_ADDR;

	if(EepromEx_ReadData(addr, EEPROMEX_EVENT_FORMAT_SIZE, buf) == TRUE)
	{
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get Permanent fault Log of External EEPROM 
* @param	index - index value
* @param	*buf - buf data
* @return	void
*/
void EepromEx_GetPEventLog(uint16 index, uint8* buf)
{
	uint32 addr;

	addr = ((uint32)index * EEPROMEX_PEVENT_FORMAT_SIZE) + EEPROMEX_PEVENT_START_ADDR;

	if(EepromEx_ReadData(addr, EEPROMEX_PEVENT_FORMAT_SIZE, buf) == TRUE)
	{
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief	Check if Parameter of External EEPROM is saved 
* @param	void
* @return	BOOL, TRUE - Saved, FALSE - X
*/
BOOL EepromEx_IsSavedParameter(void)
{	
	uint8 RecData = 0;
    BOOL rv = FALSE;
    
	if(EepromEx_ReadData(EEPROMEX_SAVED_PARAMETER_FLAG_ADDR, 1u, &RecData) == TRUE)
	{
    	if (RecData == EEPROMEX_SAVED_CONFIG)
        {
            rv = SAVED;
        }	
	}
    
    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set saved parameter flag of External EEPROM
* @param	BOOL, TRUE - Saved, FALSE - X
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_SetSavedParameterFlag(BOOL save)
{
	uint8 sv;
	BOOL rv = FALSE;
    
	if (save == SAVED)
    {
        sv = EEPROMEX_SAVED_CONFIG;
    }
	else
    {
        sv = EEPROMEX_NOT_SAVED_CONFIG;
	}
	if (EepromEx_WriteData(EEPROMEX_SAVED_PARAMETER_FLAG_ADDR, 1u, &sv) == TRUE)
    {
        rv = TRUE;
    }

    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Read String data of External EEPROM
* @param	void
* @return	BOOL, TRUE - Success, FALSE - Reset
*/
BOOL EepromEx_ReadStringData(void)
{
	uint8 rbuf[EEPROMEX_STRING_DATA_LENGTH];    
    BOOL UpFlag = FALSE;
    uint8 i;
    BOOL rv = FALSE;
	uint16 u16data;
	uint16 NumberOfParallel;
	uint16 FCC_of_Cell;

	NumberOfParallel = Parameter_Get_NumberOfParallel();
	FCC_of_Cell = Parameter_Get_FCC_of_Cell();
    
    for(i= 0; i<5u; i++)
    {
        if(EepromEx_ReadData(EEPROMEX_STRING_DATA_ADDR, EEPROMEX_STRING_DATA_LENGTH, rbuf) == TRUE)
        {
            if((((uint16)rbuf[1] << 8) + (uint16)rbuf[0]) == 0xAAAAu)
            {
                /* CRC Check */
                if(Crc_Check(rbuf, EEPROMEX_STRING_DATA_LENGTH) == TRUE)
                {
                    /* Data Save */
                    UpFlag = TRUE;
                    break;
                }
             }
         }
     }

//	UpFlag = FALSE; debug
	if(UpFlag == TRUE)
	{
		EEPROM_CapacityData.SaveCheck 			  = (((uint16)rbuf[1] << 8) + (uint16)rbuf[0]);
		EEPROM_CapacityData.Complete_Init_FCC    = (((uint16)rbuf[3] << 8) + (uint16)rbuf[2]);
		EEPROM_CapacityData.Init_FCC             = ((uint32)rbuf[7] << 24) + ((uint32)rbuf[6] << 16) + ((uint32)rbuf[5] << 8) + (uint32)rbuf[4];		
		EEPROM_CapacityData.FCC                  = ((uint32)rbuf[11] << 24) + ((uint32)rbuf[10] << 16) + ((uint32)rbuf[9] << 8) + (uint32)rbuf[8];		
		EEPROM_CapacityData.Capacity             = ((uint32)rbuf[15] << 24) + ((uint32)rbuf[14] << 16) + ((uint32)rbuf[13] << 8) + (uint32)rbuf[12];		
        EEPROM_CapacityData.Cycle_Count          = ((uint16)rbuf[17] << 8) + (uint16)rbuf[16];
        EEPROM_CapacityData.DchCntIntegral       = ((uint32)rbuf[21] << 24) + ((uint32)rbuf[20] << 16) + ((uint32)rbuf[19] << 8) + (uint32)rbuf[18];
        EEPROM_CapacityData.ContOpenCount_UnderLoad = ((uint16)rbuf[23] << 8) + (uint16)rbuf[22];
        EEPROM_CapacityData.Accum_ChargeAmps     = ((uint32)rbuf[27] << 24) + ((uint32)rbuf[26] << 16) + ((uint32)rbuf[25] << 8) + (uint32)rbuf[24];
        EEPROM_CapacityData.Accum_DischargeAmps  = ((uint32)rbuf[31] << 24) + ((uint32)rbuf[30] << 16) + ((uint32)rbuf[29] << 8) + (uint32)rbuf[28];
		EEPROM_CapacityData.Chg_kWh              = ((uint32)rbuf[35] << 24) + ((uint32)rbuf[34] << 16) + ((uint32)rbuf[33] << 8) + (uint32)rbuf[32];
		EEPROM_CapacityData.Dch_kWh              = ((uint32)rbuf[39] << 24) + ((uint32)rbuf[38] << 16) + ((uint32)rbuf[37] << 8) + (uint32)rbuf[36];
		EEPROM_CapacityData.Chg_Wh               = ((uint16)rbuf[41] << 8) + (uint16)rbuf[40];
		EEPROM_CapacityData.Dch_Wh               = ((uint16)rbuf[43] << 8) + (uint16)rbuf[42];        
        u16data               = ((uint16)rbuf[45] << 8) + (uint16)rbuf[44];
		IR_Set_CV_IR(u16data);

		SOC_Set_CapacityData(&EEPROM_CapacityData);

//		EEPROM_CapacityData.Complete_Init_FCC = 7;												
//		EEPROM_CapacityData.Init_FCC = 50000;													//(Revision Date: 20.05.19)
//		EEPROM_CapacityData.FCC = 50000;														//(Revision Date: 20.05.19)		
	
		rv = TRUE;
	}
	else
	{   
	    /* Reset & eeprom fault */
	    EEPROM_CapacityData.SaveCheck            = 0xAAAAu;
		EEPROM_CapacityData.Complete_Init_FCC    = 0x7u;
		EEPROM_CapacityData.Init_FCC             = (uint32)NumberOfParallel * (uint32)FCC_of_Cell;
		EEPROM_CapacityData.FCC                  = EEPROM_CapacityData.Init_FCC;
		EEPROM_CapacityData.Capacity             = 0u;
        EEPROM_CapacityData.Cycle_Count          = 0u;
        EEPROM_CapacityData.DchCntIntegral       = 0u;
        EEPROM_CapacityData.ContOpenCount_UnderLoad = 0u;
        EEPROM_CapacityData.Accum_ChargeAmps     = 0u;
        EEPROM_CapacityData.Accum_DischargeAmps  = 0u;
		EEPROM_CapacityData.Chg_kWh              = 0u;
		EEPROM_CapacityData.Dch_kWh              = 0u;
		EEPROM_CapacityData.Chg_Wh               = 0u;
		EEPROM_CapacityData.Dch_Wh               = 0u;
		IR_Set_CV_IR(0u);

		SOC_Set_CapacityData(&EEPROM_CapacityData);
      
        if(EepromEx_WriteStringData() == TRUE)
        {
            
        }
		HAL_Delay(5u);
	}

	

	return rv;
}

/*---------------------------------------------------------------------*/
/**
* @brief	Write String data of External EEPROM
* @param	void
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_WriteStringData(void)
{
	uint8 tbuf[EEPROMEX_STRING_DATA_LENGTH];
    uint8 crc[2];
    BOOL rv = FALSE;

	SOC_Get_CapacityData(&EEPROM_CapacityData);

	tbuf[0] = (uint8)(EEPROM_CapacityData.SaveCheck & 0x00FFu);
	tbuf[1] = (uint8)(EEPROM_CapacityData.SaveCheck >> 8u);
	tbuf[2] = (uint8)(EEPROM_CapacityData.Complete_Init_FCC & 0x00FFu);
	tbuf[3] = (uint8)(EEPROM_CapacityData.Complete_Init_FCC >> 8u);
	tbuf[4] = (uint8)(EEPROM_CapacityData.Init_FCC & 0x00FFu);
	tbuf[5] = (uint8)(EEPROM_CapacityData.Init_FCC >> 8u);
	tbuf[6] = (uint8)(EEPROM_CapacityData.Init_FCC >> 16u);
	tbuf[7] = (uint8)(EEPROM_CapacityData.Init_FCC >> 24u);	
	tbuf[8] = (uint8)(EEPROM_CapacityData.FCC & 0x00FFu);
	tbuf[9] = (uint8)(EEPROM_CapacityData.FCC >> 8u);
	tbuf[10] = (uint8)(EEPROM_CapacityData.FCC >> 16u);
	tbuf[11] = (uint8)(EEPROM_CapacityData.FCC >> 24u);	
	tbuf[12] = (uint8)(EEPROM_CapacityData.Capacity & 0x00FFu);
	tbuf[13] = (uint8)(EEPROM_CapacityData.Capacity >> 8u);
	tbuf[14] = (uint8)(EEPROM_CapacityData.Capacity >> 16u);
	tbuf[15] = (uint8)(EEPROM_CapacityData.Capacity >> 24u);	
	tbuf[16] = (uint8)(EEPROM_CapacityData.Cycle_Count & 0x00FFu);
	tbuf[17] = (uint8)(EEPROM_CapacityData.Cycle_Count >> 8u);
	tbuf[18] = (uint8)(EEPROM_CapacityData.DchCntIntegral & 0x00FFu);
	tbuf[19] = (uint8)(EEPROM_CapacityData.DchCntIntegral >> 8u);
	tbuf[20] = (uint8)(EEPROM_CapacityData.DchCntIntegral >> 16u);
	tbuf[21] = (uint8)(EEPROM_CapacityData.DchCntIntegral >> 24u);	
	tbuf[22] = (uint8)(EEPROM_CapacityData.ContOpenCount_UnderLoad & 0x00FFu);
	tbuf[23] = (uint8)(EEPROM_CapacityData.ContOpenCount_UnderLoad >> 8u);
	tbuf[24] = (uint8)(EEPROM_CapacityData.Accum_ChargeAmps & 0x00FFu);
	tbuf[25] = (uint8)(EEPROM_CapacityData.Accum_ChargeAmps >> 8u);
	tbuf[26] = (uint8)(EEPROM_CapacityData.Accum_ChargeAmps >> 16u);
	tbuf[27] = (uint8)(EEPROM_CapacityData.Accum_ChargeAmps >> 24u);
	tbuf[28] = (uint8)(EEPROM_CapacityData.Accum_DischargeAmps & 0x00FFu);
	tbuf[29] = (uint8)(EEPROM_CapacityData.Accum_DischargeAmps >> 8u);
	tbuf[30] = (uint8)(EEPROM_CapacityData.Accum_DischargeAmps >> 16u);
	tbuf[31] = (uint8)(EEPROM_CapacityData.Accum_DischargeAmps >> 24u);
	tbuf[32] = (uint8)(EEPROM_CapacityData.Chg_kWh & 0x00FFu);
	tbuf[33] = (uint8)(EEPROM_CapacityData.Chg_kWh >> 8u);
	tbuf[34] = (uint8)(EEPROM_CapacityData.Chg_kWh >> 16u);
	tbuf[35] = (uint8)(EEPROM_CapacityData.Chg_kWh >> 24u);
	tbuf[36] = (uint8)(EEPROM_CapacityData.Dch_kWh & 0x00FFu);
	tbuf[37] = (uint8)(EEPROM_CapacityData.Dch_kWh >> 8u);
	tbuf[38] = (uint8)(EEPROM_CapacityData.Dch_kWh >> 16u);
	tbuf[39] = (uint8)(EEPROM_CapacityData.Dch_kWh >> 24u);
	tbuf[40] = (uint8)(EEPROM_CapacityData.Chg_Wh & 0x00FFu);
	tbuf[41] = (uint8)(EEPROM_CapacityData.Chg_Wh >> 8u);
	tbuf[42] = (uint8)(EEPROM_CapacityData.Dch_Wh & 0x00FFu);
	tbuf[43] = (uint8)(EEPROM_CapacityData.Dch_Wh >> 8u);
	tbuf[44] = (uint8)(IR_Get_CV_IR() & 0x00FFu);
	tbuf[45] = (uint8)(IR_Get_CV_IR() >> 8u);

    Crc_Calculation(crc, tbuf, EEPROMEX_STRING_DATA_LENGTH-2u);
    memnadd(tbuf, crc, EEPROMEX_STRING_DATA_LENGTH-2u, 2u);

	if(EepromEx_WriteData(EEPROMEX_STRING_DATA_ADDR, EEPROMEX_STRING_DATA_LENGTH, tbuf) == TRUE)
	{
	    rv = TRUE;
	}

    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Read calibration data of External EEPROM
* @param	*buf - buffer
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_ReadCalibData (void* buf)
{
    BOOL rv = FALSE;
    
	if(EepromEx_ReadData(EEPROMEX_CALIBDATA_ADDR, EEPROMEX_CALIBDATA_LENGTH, buf) == TRUE)
	{
	    rv = TRUE;
	}

    return rv;	
}


/*---------------------------------------------------------------------*/
/**
* @brief	Write calibration data of External EEPROM
* @param	*buf - buffer
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_WriteCalibData (void* buf) 
{
    BOOL rv = FALSE;

	if(EepromEx_WriteData(EEPROMEX_CALIBDATA_ADDR, EEPROMEX_CALIBDATA_LENGTH, buf) == TRUE)
	{
	    rv = TRUE;
	}

	return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Check if Calibration data of External EEPROM is saved 
* @param	void
* @return	uint8, saved(1) or empty(0)
*/
bool EepromEx_IsSavedCalibData(void)
{
	uint8 recData;
	bool rv = FALSE;

	if(EepromEx_ReadData(EEPROMEX_SAVED_CALIBDATA_FLAG_ADDR, 1u, &recData) == TRUE)
    {
    	if(recData == EEPROMEX_SAVED_CONFIG)
    	{
    	    rv = SAVED;
    	}
    }
    
	return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set saved Calibration data flag of External EEPROM
* @param	BOOL, TRUE - Saved, FALSE - X
* @return	BOOL, TRUE - Success, FALSE - X
*/
BOOL EepromEx_SetSavedCalibDataFlag(BOOL save)
{
	uint8 sv ;
	BOOL rv = FALSE;
	
	if (save == TRUE)
    {
        sv = EEPROMEX_SAVED_CONFIG;
    }
	else
    {
        sv = EEPROMEX_NOT_SAVED_CONFIG;
    }
    
	if (EepromEx_WriteData(EEPROMEX_SAVED_CALIBDATA_FLAG_ADDR, 1u, &sv) == TRUE)
    {
        rv = TRUE;
    }
    
    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Check if Fault History of External EEPROM is saved 
* @param	void
* @return	BOOL, TRUE - Saved, FALSE - X
*/
BOOL EepromEx_IsSavedFaultHistory(void)
{
	uint8 recData;
	BOOL rv = FALSE;
	
	if(EepromEx_ReadData(EEPROMEX_SERVICE_FAULT_ADDR, 1u, &recData) == TRUE)
    {
    	if (recData == EEPROMEX_SAVED_CONFIG)
    	{
    	    rv = SAVED;
    	}
    }
    
    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Check if permanent Fault History of External EEPROM is saved 
* @param	void
* @return	BOOL, TRUE - Saved, FALSE - X
*/
BOOL EepromEx_IsSavedPFaultHistory(void)
{
	uint8 recData;
	BOOL rv = FALSE;
	
	if(EepromEx_ReadData(EEPROMEX_SERVICE_PFAULT_ADDR, 1u, &recData) == TRUE)
    {
    	if (recData == EEPROMEX_SAVED_CONFIG)
    	{
    	    rv = SAVED;
    	}
    }
    
    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief Write CFG data to EEPROM
* @param void
* @return BOOL, True - success, False - fail
*/
/*
BOOL EepromEx_WriteCfgData(void)
{
	static uint8 try_num = 0;
	BOOL rv = FALSE;

	if (try_num < EEPROMEX_WR_TRY_NUM)
	{
		static uint8 block_num = 0;

		switch (block_num)
		{
			case 0:
				if(EepromEx_InOutParameters((&Parameter),					set, EEPROMEX_BLOCK1_DATA_QTY_BYTE, EEPROMEX_BLOCK1_ADDR_OFFSET) == FALSE)
				{
				    EepromEx_SetErrorFlag(0);
				}
				block_num++;
				break;

			case 1:
				if(EepromEx_InOutParameters((&Parameter),					get, EEPROMEX_BLOCK1_DATA_QTY_BYTE, EEPROMEX_BLOCK1_ADDR_OFFSET) == FALSE)
				{
				    EepromEx_SetErrorFlag(0);
				}
				block_num++;
				break;
				
			case 2:
				if(EepromEx_InOutParameters((&Parameter.OCV_Warning),		set, EEPROMEX_BLOCK2_DATA_QTY_BYTE, EEPROMEX_BLOCK2_ADDR_OFFSET) == FALSE)
				{
				    EepromEx_SetErrorFlag(1);
				}
				block_num++;
				break;

			case 3:
				if(EepromEx_InOutParameters((&Parameter.OCV_Warning),		get, EEPROMEX_BLOCK2_DATA_QTY_BYTE, EEPROMEX_BLOCK2_ADDR_OFFSET) == FALSE)
				{
				    EepromEx_SetErrorFlag(1);
				}
				block_num++;
				break;

			case 4:
				if(EepromEx_InOutParameters((&Parameter.OCV_W_Release),	set, EEPROMEX_BLOCK3_DATA_QTY_BYTE, EEPROMEX_BLOCK3_ADDR_OFFSET) == FALSE) 
				{
				    EepromEx_SetErrorFlag(2);
				}
				block_num++;
				break;

			case 5:
				if(EepromEx_InOutParameters((&Parameter.OCV_W_Release),	get, EEPROMEX_BLOCK3_DATA_QTY_BYTE, EEPROMEX_BLOCK3_ADDR_OFFSET) == FALSE) 
				{
				    EepromEx_SetErrorFlag(2);
				}
				block_num++;
				break;

			case 6:
				if(EepromEx_InOutParameters((&Parameter.OCV_Fault),		set, EEPROMEX_BLOCK4_DATA_QTY_BYTE, EEPROMEX_BLOCK4_ADDR_OFFSET) == FALSE)
				{
				    EepromEx_SetErrorFlag(3);
				}
				block_num++;
				break;

			case 7:
				if(EepromEx_InOutParameters((&Parameter.OCV_Fault),		get, EEPROMEX_BLOCK4_DATA_QTY_BYTE, EEPROMEX_BLOCK4_ADDR_OFFSET) == FALSE)
				{
				    EepromEx_SetErrorFlag(3);
				}
				block_num++;
				break;
	
			case 8:
				if(EepromEx_InOutParameters((&Parameter.OCV_F_Release),	set, EEPROMEX_BLOCK5_DATA_QTY_BYTE, EEPROMEX_BLOCK5_ADDR_OFFSET) == FALSE)
				{
				    EepromEx_SetErrorFlag(4);
				}
				block_num++;
				break;

			case 9:
				if(EepromEx_InOutParameters((&Parameter.OCV_F_Release),	get, EEPROMEX_BLOCK5_DATA_QTY_BYTE, EEPROMEX_BLOCK5_ADDR_OFFSET) == FALSE)
				{
				    EepromEx_SetErrorFlag(4);
				}
				block_num = 0;
			
				if(EepromEx_ErrorFlag == 0u)
				{
					if(EepromEx_SetSavedParameterFlag(TRUE) == TRUE)
					{
					    try_num = 0u;
					    rv = TRUE;
					}					
				}
				else
				{
					try_num++;
				}
				break;

			default:
				break;
		}
	}

	return rv;
}
*/

/*---------------------------------------------------------------------*/
/**
* @brief	Set Updata Capacity data falg of External EEPROM 
* @param	BOOL, TRUE - O, FALSE - X
* @return	void
*/
void EepromEx_SetUpdateCapacityData_Flag(BOOL flag)
{
	EepromEx_UpdateCapacityData_Flag = flag;
}


/*---------------------------------------------------------------------*/
/**
* @brief Read/Write parameter to external eeprom
* @param *buf - Read/Write Data
* @param SG - SET:Write, GET:Read
* @return TRUE - Success, FALSE - Fail
*/
BOOL EepromEx_InOutParameters(void* buf, BUF_INOUT SG, uint32 len, uint32 Offset)
{
	BOOL rv = FALSE;

	switch (SG)
	{
		case get:		
			if(EepromEx_ReadData(Offset, (len + CRC_BYTE), buf) == TRUE)
			{
				rv = Crc_Check(buf, ((uint16)len + (uint16)CRC_BYTE));
			}
			else
			{
				rv = FALSE;
			}
			break;

		case set:
			if(len > (EEPROMEX_PARAMETER_BLOCK_SIZE_BYTE - 2u))
			{
				rv = FALSE;
			}
			else
			{
				uint8 crc[2];
				
				Crc_Calculation(crc, buf, (uint16)len);
				
				memnadd(buf, crc, (uint16)len, CRC_BYTE);
				
				if(EepromEx_WriteData(Offset, (len + CRC_BYTE), buf) == TRUE)
				{
					rv = TRUE;
				}
				else
				{
					rv = FALSE;
				}
			}
			break;

			default:
				break;
	}

	return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set error falg of External EEPROM 
* @param	uint8, Error flag bit
* @return	void
*/
void EepromEx_SetErrorFlag(uint8 bit)
{
	EepromEx_ErrorFlag |= ((uint8)1 << bit);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Reset error falg of External EEPROM 
* @param	void
* @return	void
*/
void EepromEx_ResetErrorFlag(void)
{
	EepromEx_ErrorFlag = 0;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get error falg of External EEPROM 
* @param	void
* @return	uint8, Error flag bit
*/
uint8 EepromEx_GetErrorFlag(void)
{
	return EepromEx_ErrorFlag;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set SaveFaultHistory falg of External EEPROM 
* @param	void
* @return	void
*/
void EepromEx_Set_SaveFaultHistoryFlag(BOOL SetV)
{
	EepromEx_SaveFaultHistoryFlag = SetV;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get SaveFaultHistory falg of External EEPROM 
* @param	void
* @return	void
*/
BOOL EepromEx_Get_SaveFaultHistoryFlag(void)
{
    return EepromEx_SaveFaultHistoryFlag;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set SavePFaultHistory falg of External EEPROM 
* @param	void
* @return	void
*/
void EepromEx_Set_SavePFaultHistoryFlag(BOOL SetV)
{
	EepromEx_SavePFaultHistoryFlag = SetV;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get SavePFaultHistory falg of External EEPROM 
* @param	void
* @return	void
*/
BOOL EepromEx_Get_SavePFaultHistoryFlag(void)
{
    return EepromEx_SavePFaultHistoryFlag;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set SaveEventIdx falg of External EEPROM 
* @param	void
* @return	void
*/
void EepromEx_Set_SaveEventIdxFlag(BOOL SetV)
{
	EepromEx_SaveEventIdxFlag = SetV;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get SaveEventIdx falg of External EEPROM 
* @param	void
* @return	void
*/
BOOL EepromEx_Get_SaveEventIdxFlag(void)
{
    return EepromEx_SaveEventIdxFlag;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set SavePEventIdx falg of External EEPROM 
* @param	void
* @return	void
*/
void EepromEx_Set_SavePEventIdxFlag(BOOL SetV)
{
	EepromEx_SavePEventIdxFlag = SetV;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get SavePEventIdx falg of External EEPROM 
* @param	void
* @return	void
*/
BOOL EepromEx_Get_SavePEventIdxFlag(void)
{
    return EepromEx_SavePEventIdxFlag;
}

/*---------------------------------------------------------------------*/
/**
* @brief	Update Permanent Fault Log of External EEPROM
* @param	void
* @return	void
*/
BOOL EepromEx_Update_CLASSB_PfaultLog(void)
{
	BOOL rv = FALSE;
    uint8  ebuf[EEPROMEX_PEVENT_FORMAT_SIZE] = {0,};
    RTC_DateTypeDef RTCd;
    RTC_TimeTypeDef RTCt;
	CLASSB static uint8 Step;
	uint32 addr;
	uint32 offset;
	CLASSB static uint8 step2;
	CLASSB static uint32 addr2;
	CLASSB static uint32 offset2;
	uint32 u32data;

	switch (Step)
	{
		case 0u:
			u32data = Diag_Get_FlagsPFault() | 0x80000000u;
			Diag_Set_FlagsPFault(u32data);
			(void)HAL_RTC_GetTime(&hrtc, &RTCt, RTC_FORMAT_BIN);
			(void)HAL_RTC_GetDate(&hrtc, &RTCd, RTC_FORMAT_BIN);

	       	ebuf[0] = (uint8)(EepromEx_PFaultIDX >> 8u);
	        ebuf[1] = (uint8)(EepromEx_PFaultIDX & 0x00FFu);
	        ebuf[2] = RTCd.Year;  
	        ebuf[3] = RTCd.Month;
	        ebuf[4] = RTCd.Date;
	        ebuf[5] = RTCt.Hours;
	        ebuf[6] = 0xFF;
	        ebuf[7] = (uint8)(Diag_Get_ClassbFailcode());
	        ebuf[8] = (uint8)(Diag_Get_FlagsPFault() >> 24u);
	        ebuf[9] = (uint8)(Diag_Get_FlagsPFault() >> 16u);
	        ebuf[10] = (uint8)(Diag_Get_FlagsPFault() >> 8u);
	        ebuf[11] = (uint8)(Diag_Get_FlagsPFault() & 0xFFu);


			if(EepromEx_PFaultIDX < EEPROMEX_MAX_EVENTBUF)
		    {
				addr = (((uint32)EepromEx_PFaultIDX * (uint32)EEPROMEX_PEVENT_FORMAT_SIZE) + (uint32)EEPROMEX_PEVENT_START_ADDR);

				if((addr + EEPROMEX_PEVENT_FORMAT_SIZE) > (EEPROMEX_PAGESIZE * ((addr / EEPROMEX_PAGESIZE) + 1u)))
				{
					switch (step2)
					{
						case 0u:
							offset = (EEPROMEX_PAGESIZE * ((addr / EEPROMEX_PAGESIZE) + 1u)) - addr;

				        	if(EepromEx_WriteData(addr, offset, ebuf) == TRUE)
				            {
				                addr2 = addr + offset;
								offset2 = EEPROMEX_PEVENT_FORMAT_SIZE - offset;
				            }
							step2++;
							break;

						case 1u:
							if(EepromEx_WriteData(addr2, offset2, &ebuf[EEPROMEX_EVENT_FORMAT_SIZE - offset2]) == TRUE)
				            {
				                EepromEx_PFaultIDX++;
				            }
							step2 = 0u;
							Step++;
							break;

						default:
							break;
					}	
				}
						
				else
				{
		            if(EepromEx_WriteData(addr, EEPROMEX_PEVENT_FORMAT_SIZE, ebuf) == TRUE)
		            {
		                EepromEx_PFaultIDX++;
		            }
					Step++;
				}
			}
		        
	        else
	        {
	            EepromEx_PFaultIDX = 0u;
				Step++;
	        }
			break;

		case 1u:
			(void)EepromEx_SavePEventIndex();
			Step++;
			break;

		case 2u:
			if((Diag_Get_FlagsPFault() & (uint32)EEPROMEX_SERVICE_PFAULT_CODE) > (uint32)0u)
        	{
            	(void)EepromEx_SetSavedPFaultHistoryFlag(TRUE);
        	}
			rv = TRUE;
			Step = 0;
			break;

		default :	
			break;
	}

	return rv;
}



/* End of App_EepromEx.c */

