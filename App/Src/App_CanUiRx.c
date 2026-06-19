/*
 * Api_CanUiRx.c
 *
 *  Created on: Mar 19, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_CanUiRx.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/


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

static	uint32	CanVcuRx_OfflineCnt = 0u;
static bool		 Can_ContactorCmd;
static bool 		 Can_Send_PackDataFlag 			= FALSE;
static bool 		 Can_Send_ModeChangeFlag 		= FALSE;
static bool		 Can_Send_RelayDataFlag 		= FALSE;
static bool		 Can_Send_FaultReleaseFlag 		= FALSE;
static bool		 Can_Send_ConfigDataFlag 		= FALSE;
static bool		 Can_Send_CalibDataFlag 		= FALSE;
static bool		 Can_Send_EventDataFlag 		= FALSE;
static bool		 Can_Send_DetectFccDataFlag 	= FALSE;

static uint8		 Can_ConfigData[8u];
static uint8   	 Can_CalibData[8u];
static uint8   	 Can_EventData[8u];
static uint8   	 Can_DetectFCCData[8u];

static uint16  	 Can_BmicNo = 0u;
static bool      bConfigSaveFlag                = FALSE;

static uint8	 BalanceTest;


// PARAMETER Can_temp_Parameter;


uint8		Config_Data[8u];
uint8		Calibrate_Data[8u];
uint8		Event_Data[8u];
uint8		DetectFCC_Data[8u];

uint16  	CAN_BmicNo;

uint16     Can_ConfigSave_DownCount    = 0u;




/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
void 	CanVcuRx_Reset_OfflineCnt( void );
void 		CanVcuRx_Increase_OfflineCnt(void);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/

void	CanUiRx_Init(void)
{
	Can_ProcessMsg_Cb_Register(&hcan1, CanUiRx_Callback);
	
//	(void)memcpy(&Can_temp_Parameter, &Parameter, sizeof(PARAMETER));
}


void	CanUiRx_Callback(Can_RxData_t	*RxData)
{
	uint8	ssId;
	uint8	ssMsg1; 
	uint8	ssMsg2;

	uint16  MLECID;
	SYSTEM_STATE System_State;

	MLECID = Dio_Get_BoardID();
	System_State = ManaMsm_Get_System_State();

	StlCntInterrupt.cntIntrCANRx++;

	if (RxData->Rtr == CAN_RTR_REMOTE)
	{
		return;
	}
	else
	{
		ssId	= (uint8)((uint32)(RxData->MsgID & 0x0000F000u) >> 12u);
		ssMsg1 	= (uint8)((uint32)(RxData->MsgID & 0x000F0000u) >> 16u);
		ssMsg2 	= (uint8)((uint32)(RxData->MsgID & 0x00000F00u) >>  8u);

		if (ssId == MLECID)
		{			
			switch (ssMsg1)
			{
				case 0x0Fu:

					switch (ssMsg2)
					{
						case 0x01u:
							/* Reset offline counter for VCU */
							CanVcuRx_Reset_OfflineCnt();

							Can_Send_PackDataFlag = TRUE;
							
							if		(RxData->Data[0] == 0x01u)	{	Can_ContactorCmd = FALSE;	}
							else if (RxData->Data[0] == 0x02u)	{	Can_ContactorCmd = TRUE;	}
							else{}

							if(!Can_ContactorCmd)
							{
								if((RxData->Data[1u] & 0x01u) != 0u)
								{
									Can_ContactorCmd = TRUE;
								}
								else								
								{
									Can_ContactorCmd = FALSE;
								}
							}
							else
							{

							}

    						if((RxData->Data[3u] & 0x01u) != 0u)
							{
								Fan_SetControlCMD(TRUE);
    						}
                            else									
							{
								Fan_SetControlCMD(FALSE);
                            }

            				if((RxData->Data[7u] & 0x01u) != 0u)
							{
								IsoR_SetMeasCommand(TRUE);
            				}
                            else                                    
							{
								IsoR_SetMeasCommand(FALSE);
                            }

							if(RxData->Data[6u] == 0x00u)	
							{	
								BalanceTest = 0u;	 
							}
							else if(RxData->Data[6u] == 0x01u)	
							{	
								BalanceTest = 1u;	 
							}
							else
							{
							}
	
							break;
							
						default:	break;
					}
					break;

				case 0xEu:
					switch(ssMsg2)
					{
						case 0x0u:	// mode Change
							if((System_State == SYS_IDLE)
							|| (System_State == SYS_CONFIGURATION)
							|| (System_State == SYS_CALIBRATION)
							|| (System_State == SYS_ERROR)
							|| (System_State == SYS_LOGREAD)
							|| (System_State == SYS_INIT)
							|| (System_State == SYS_DETECTING_FCC))
							{ 
								if	(RxData->Data[0u] == 0x00u)	
								{
									ManaMsm_Set_System_State(SYS_INIT); 
								}
								else if(RxData->Data[0u] == 0x01u)	
								{
									ManaMsm_Set_System_State(SYS_CONFIGURATION);
								}
								else if(RxData->Data[0u] == 0x02u)	
								{
									ManaMsm_Set_System_State(SYS_CALIBRATION); 
								}
								else if(RxData->Data[0u] == 0x03u)	
								{
									ManaMsm_Set_System_State(SYS_SELFTEST); 
								}
								else if(RxData->Data[0u] == 0x04u)	
								{
									ManaMsm_Set_System_State(SYS_LOGREAD); 
								}
								else if(RxData->Data[0u] == 0x05u)	
								{
									ManaMsm_Set_System_State(SYS_DETECTING_FCC); 
								}
								else
								{

								}
							
								if (RxData->Data[0u] <= 0x05u)
								{
									
									Can_Send_ModeChangeFlag = TRUE;
								}
								else if(RxData->Data[0u] == 0x0Au)
								{ 
									Can_Send_ModeChangeFlag = TRUE;									
								}
								else
								{

								}
							}		
							break;
							
						case 0x1u:		// configuration
							if(System_State == SYS_CONFIGURATION)
							{			
								memncpy(Can_ConfigData, RxData->Data, 0u, 8u);
								Can_Send_ConfigDataFlag = TRUE;								
							}	
							break;
							
						case 0x2u:		// calibration
							if(System_State == SYS_CALIBRATION)
							{															
								memncpy(Can_CalibData, RxData->Data, 0u, 8u);
								Can_Send_CalibDataFlag = TRUE;								
							}																							
							break;
							
						case 0x3u:		// Event read
							if(System_State == SYS_LOGREAD)
							{								
								memncpy(Can_EventData, RxData->Data, 0u , 8u);
								Can_Send_EventDataFlag = TRUE;								
							}							
							break;
							
						case 0x6u:		// Contactor test
							if((System_State == SYS_CALIBRATION) || (System_State == SYS_CONFIGURATION) || (System_State == SYS_DETECTING_FCC))
							{
								if((RxData->Data[0u] >= 0x10u) && (RxData->Data[0u] <= 0x1Bu))
								{
									Relay_SetControlCommand(RC_TEST);
									Relay_SetTestCommand(RxData->Data[0u]); 
								}			
								else if(RxData->Data[0u] == 0x1Fu)
								{
									Can_Send_RelayDataFlag = TRUE;									
								}
								else if(RxData->Data[0u] == 0x20u)
								{
									if(Relay_GetControlCommand() == RC_TEST)	
									{
										Relay_SetControlCommand(RC_CHECK);
									}
								}
								else
								{

								}
							}						
							break;
								
						case 0x07u:		// Forced Release Fault
							Can_Send_FaultReleaseFlag = TRUE;
							break;

						case 0x08u:		// Full charge capacity 
							if(System_State == SYS_DETECTING_FCC)
							{
								memncpy(Can_DetectFCCData, RxData->Data, 0u, 8u);
								Can_Send_DetectFccDataFlag = TRUE;
							}
							break;
							
						default:
							break;
					}
					break;

				case 0xCu:
					Can_BmicNo = 32u + (uint16)ssMsg2 + 1u;
					break;
					
				case 0xBu:
					Can_BmicNo = 16u + (uint16)ssMsg2 + 1u;
					break;

				case 0xAu:
					Can_BmicNo = (uint16)ssMsg2 + 1u;
					break;

				default:
					break;
			}
		}
	}
}



void Can_Process_Task(void)
{
	uint8	i;
//	static  BOOL    Can_ConfigSaveFlag          = FALSE;
	
		if(Can_Send_PackDataFlag != FALSE)
		{
			Can_Send_PackDataFlag = FALSE;
			for (i = 0u; i < 15u; i++)
			{
				CanVcuTx_Set_Debug_En(i);
			}
		}

		if(Can_Send_ModeChangeFlag != FALSE)
		{
			Can_Send_ModeChangeFlag = FALSE;
			CanVcuTx_Set_Debug_En(15u);
		}

		if(Can_Send_RelayDataFlag != FALSE)
		{
			Can_Send_RelayDataFlag = FALSE;
			CanVcuTx_Set_Debug_En(16u);
		}
		
		if(Can_BmicNo > 0u)
		{
			CAN_BmicNo = Can_BmicNo - 1u;
			for (i = 17u; i < 26u; i++)
			{
				CanVcuTx_Set_Debug_En(i);
			}
			
			Can_BmicNo = 0u;
		}

		if(Can_Send_FaultReleaseFlag != FALSE)
		{
			Can_Send_FaultReleaseFlag = FALSE;
			Diag_Set_FlagsFault(0u);
			EepromEx_Set_SaveFaultHistoryFlag(TRUE);
			Diag_Set_FlagsPFault(0u);
			EepromEx_Set_SavePFaultHistoryFlag(TRUE);
			CanVcuTx_Set_Debug_En(26u);
		}

		if(Can_Send_ConfigDataFlag == DEF_True)
		{
			Can_Send_ConfigDataFlag = FALSE;
			memncpy(Config_Data, Can_ConfigData, 0u , 8u);
		
			CanVcuTx_Set_Debug_En(27u);
		}
/*
	    if(Can_ConfigSave_DownCount == 0u)
	    {
	        if(Can_ConfigSaveFlag == TRUE)        
	        {
	            Can_ConfigSaveFlag = FALSE;
	            if(memncmp((uint8*)&Parameter, (uint8*)&Can_temp_Parameter, 0u, (uint16)sizeof(PARAMETER)) == FALSE)
	            {
	                (void)memcpy(&Parameter, &Can_temp_Parameter, sizeof(PARAMETER));
	                bConfigSaveFlag = TRUE;
	            }
	            if(IsWarrantyDataChanged() == TRUE)
	            {
	                Set_SaveWarrantyData(TRUE);
	            }            
	        }
	    } 
	    else
	    {
	        Can_ConfigSave_DownCount--;
	        Can_ConfigSaveFlag = TRUE;
	    }
*/
		if(Can_Send_CalibDataFlag != FALSE)
		{
			Can_Send_CalibDataFlag = FALSE;
			memncpy(Calibrate_Data, Can_CalibData, 0u , 8u);
			
			if(Calibrate_Data[0u] == 0x00u || Calibrate_Data[0u] == 0x01u)
			{
				CanVcuTx_Set_Debug_En(28u);
			}

			if(Calibrate_Data[0u] == 0x02u || Calibrate_Data[0u] == 0x03u)
			{
				 Set_SaveCalibData(TRUE);
			}
			
		}

		if(Can_Send_EventDataFlag != FALSE)
		{
			Can_Send_EventDataFlag = FALSE;
			memncpy(Event_Data, Can_EventData, 0u , 8u);

			CanVcuTx_Set_Debug_En(29u);
			
			if(Event_Data[0u] == 5u)
			{
				CanVcuTx_Set_Debug_En(30u);
			}
			
			else if(Event_Data[0u] == 2u)
			{
				CanVcuTx_Set_Debug_En(30u);
				CanVcuTx_Set_Debug_En(31u);
			}
			else
			{
			}
		}

		if(Can_Send_DetectFccDataFlag != FALSE)
		{
			Can_Send_DetectFccDataFlag = FALSE;
			memncpy(DetectFCC_Data, Can_DetectFCCData, 0u , 8u);

			CanVcuTx_Set_Debug_En(32u);
		}	


}


uint32	CanVcuRx_Get_OfflineCnt( void )
{
	return CanVcuRx_OfflineCnt;
}



void CanVcuRx_Increase_OfflineCnt( void )
{
	uint16 MCE_Fault;

	MCE_Fault = Parameter_Get_MCE_Fault();
	
	if(CanVcuRx_OfflineCnt <= MCE_Fault )
	{
		CanVcuRx_OfflineCnt++;
	}
}



void CanVcuRx_Reset_OfflineCnt( void )
{
	CanVcuRx_OfflineCnt = 0u;
}

/*---------------------------------------------------------------------*/
/**
* @brief	Function of Counting Master CAN communication time out 
* @param	void
* @return	void
*/
void Can_CountingTimeOut(void)
/*
	Increase timeout count 	
*/
{
	static uint8 inc_cnt = 0u;

	if(inc_cnt++ >= 9u)
	{
		inc_cnt = 0u;
		CanVcuRx_Increase_OfflineCnt();
	}
}

/*---------------------------------------------------------------------*/
/**
* @brief	Get Can message of Contactor command 
* @param	void
* @return	void
*/
bool Can_GetContactorCmd(void)
{
	return Can_ContactorCmd;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Check if Configuration data is saved 
* @param	void
* @return	BOOL, TRUE - saved, FALSE - X
*/
extern BOOL Get_IsSaveConfigData(void)
{
    return bConfigSaveFlag;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set Can message of save configuration data
* @param	SetV - set data
* @return	void
*/
extern void Set_SaveConfigData(BOOL SetV)
{
    bConfigSaveFlag = SetV;    
}


/*---------------------------------------------------------------------*/
/**
* @brief	Parameter update
* @param	void
* @return	void
*/
/*
extern void tempParameterUpdate(void)
{
    (void)memcpy(&Can_temp_Parameter, &Parameter, sizeof(PARAMETER)); 
}
*/

/*---------------------------------------------------------------------*/
uint8 CanUiRx_Get_BalanceTest(void)
{
    return BalanceTest;
}


