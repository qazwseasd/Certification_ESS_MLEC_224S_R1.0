/*
 * App_ManaMsm.c
 *
 *  Created on: May 20, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_ManaMsm.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define SYSTEM_STABILIZATION_TIME		199u		// have 1sec stabilization time	
#define SYSTEM_HEARTBEAT_TIME			1000u	    // [msec]
#define SYSTEM_HEARTBEAT_CNT_PERIOD		(SYSTEM_HEARTBEAT_TIME / SYSTEM_STATE_PERIOD)

#define SYSTEM_WAIT_RUNNING				0u
#define SYSTEM_WAIT_COMPLETE			1u
#define SYSTEM_WAIT_CANCEL				2u
#define SYSTEM_WAIT_ERROR				3u
#define SYSTEM_WAIT_TIMEOUT				500u		// 5sec
#define SYSTEM_WAIT_CONT_TIMEOUT		299u		// 300 - 1, 3sec

#define SYSTEM_SHUTDOWN_SEQ_TIMEOUT		500u
#define SYSTEM_SHUTDOWN_CONT_TIMEOUT	299u		// 300 - 1, 3sec

#define SYSTEM_NO_CHG					0u
#define SYSTEM_AC_CHG					1u
#define SYSTEM_DC_CHG					2u

/* for debug */
//#define SELF_HVIL_CTRL_ON               1


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/
typedef enum 
{
	IS_CURRENT_MEASURE,
	IS_KEY_CHECK,
	IS_DIAGNOSIS,
	IS_SOH,
	IS_SOC,
	IS_RELAY_CON,
	IS_COMPLETE,
	IS_ERROR
}INIT_STATE;


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static uint8 System_EmergencySeq = 0;
static	uint8	HeartBeat;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
INIT_STATE  System_InitSequence(void);
uint8 		System_Wait(void);
BOOL 		System_Drive(void);
BOOL 		System_Error(void);
BOOL 		System_Emergency(void);



/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
* @brief    System State Initialization
* @param    void
* @return   void
*/
extern void System_Init(void)
{
	ManaMsm_Set_System_State(SYS_INIT);
	HvilEpo_Self_SetState(ON);
}


/*---------------------------------------------------------------------*/
/**
* @brief    System State processing function
* @param    void
* @return   void
*/
extern void System_State_Task(void)
{
	static BOOL             InitComplete_flag = FALSE;
	static uint8	            heartbeat_cnt = 0u;
	static SYSTEM_STATE     Old_System_State = SYS_INIT;
	uint8 WaitState;
	
	SYSTEM_STATE	System_State;
	uint32	FlagsInfo;
	uint32	FlagsWarn;

	System_State = ManaMsm_Get_System_State();
	FlagsInfo = Diag_Get_FlagsInfo();
	FlagsWarn = Diag_Get_FlagsWarn();

	CtrlLed_Set_State(CtrlLed_St_Fault);
	Can_CountingTimeOut();

	if (++heartbeat_cnt >= (uint8)SYSTEM_HEARTBEAT_CNT_PERIOD)
	{
		heartbeat_cnt = 0u;
		HeartBeat++; 
	}

	#ifdef Test_System_State_Task_SYS_EMERGENCY
	InitComplete_flag = TRUE;
	#endif
	
	if ((System_State != SYS_CONFIGURATION)
	  && (System_State != SYS_CALIBRATION)
	  && (System_State != SYS_LOGREAD) )
	{
		if((HvilEpo_IsNormal() == FALSE) && InitComplete_flag)
		{
			ManaMsm_Set_System_State(SYS_EMERGENCY);
		}
		else
		{
			if(HvilEpo_IsNormal() == TRUE)
			{
			    if(System_EmergencySeq > 0u)
			    {
				    System_EmergencySeq = 0u;
				}
			}
			if(Diag_Get_FlagsFault() > 0UL)
			{
			    if(InitComplete_flag == TRUE)
				{
				    ManaMsm_Set_System_State(SYS_ERROR);
				}
			}
		}
	}

	switch(ManaMsm_Get_System_State())
	{
		case SYS_INIT:
			CtrlLed_Set_State(CtrlLed_St_Fault);
		
			if(System_InitSequence() == IS_COMPLETE)
			{
				InitComplete_flag = TRUE;
				Relay_SetControlCommand(RC_CHECK);				
				ManaMsm_Set_System_State(SYS_IDLE);
			}
			break;

		case SYS_IDLE:
			if(FlagsInfo == 0u || FlagsWarn == 0u)
			{
				CtrlLed_Set_State(CtrlLed_St_Normal);
			}
			else
			{
				CtrlLed_Set_State(CtrlLed_St_Warning);
			}
			
			if(Can_GetContactorCmd() == TRUE)
			{
			    ManaMsm_Set_System_State(SYS_WAIT); 
			}
			break;
			
		case SYS_WAIT:
			WaitState = System_Wait();
			CtrlLed_Set_State(CtrlLed_St_Normal);
			if		(WaitState == 1u)
			{
			    ManaMsm_Set_System_State(SYS_DRIVE);
			}
			else if (WaitState == 2u)
			{
			    ManaMsm_Set_System_State(SYS_IDLE);
			}
			else
            {           
            }
			break;
			
		case SYS_DRIVE:
			if(FlagsInfo == 0u || FlagsWarn == 0u)
			{
				CtrlLed_Set_State(CtrlLed_St_Normal);
			}
			else
			{
				CtrlLed_Set_State(CtrlLed_St_Warning);
			}
			
			if(System_Drive() == TRUE)
			{
			    ManaMsm_Set_System_State(SYS_IDLE);
			}
			break;
		
		case SYS_ERROR:
			CtrlLed_Set_State(CtrlLed_St_Fault);
			if(System_Error() == TRUE)
			{
			    ManaMsm_Set_System_State(SYS_IDLE);
			}
			break;
			
		case SYS_WARMING:
			break;
			
		case SYS_EMERGENCY:
			if(System_Emergency() == TRUE)
			{
			    ManaMsm_Set_System_State(SYS_IDLE);
			}
			break;
			
		case SYS_CONFIGURATION:		// only CAN Command
			CtrlLed_Set_State(CtrlLed_St_CFG);
			break;
			
		case SYS_CALIBRATION:		// only CAN Command
			CtrlLed_Set_State(CtrlLed_St_CFG);
			break;
		
		case SYS_DETECTING_FCC:
			CtrlLed_Set_State(CtrlLed_St_CFG);
			break;
		
		case SYS_SELFTEST:			// only CAN Command			
			break;
			
		case SYS_LOGREAD:			// only CAN Command
			CtrlLed_Set_State(CtrlLed_St_CFG);
			break;
			
		default:
			break;			
	}

    if(ManaMsm_Get_System_State() != Old_System_State)
    {               
        if(((uint8)ManaMsm_Get_System_State() < (uint8)SYS_CONFIGURATION) && ((uint8)Old_System_State < (uint8)SYS_CONFIGURATION))
        {
            EepromEx_SetUpdateCapacityData_Flag(TRUE);
            Set_SaveWarrantyData(TRUE);
        }	 
        Old_System_State = ManaMsm_Get_System_State();
	}

}


/*---------------------------------------------------------------------*/
/**
* @brief    Initialization Sequence            
* @param    void
* @return   INIT_STATE
*/
INIT_STATE System_InitSequence(void)
{
	static INIT_STATE is = IS_CURRENT_MEASURE;
	static uint16 StabilizeTimeCnt = 0;
	
	switch(is)
	{
		case IS_CURRENT_MEASURE :
			if(StabilizeTimeCnt++ >= SYSTEM_STABILIZATION_TIME)
			{
				StabilizeTimeCnt = 0;

				BOOL eventIndexLoaded  = EepromEx_LoadEventIndex();
				BOOL pEventIndexLoaded = EepromEx_LoadPEventIndex();

				if((eventIndexLoaded == TRUE) && (pEventIndexLoaded == TRUE))
				{
    				(void)Curr_Init();
    				is = IS_DIAGNOSIS;
				}
			}
#ifdef ADC_DEBUG			
			is = IS_DIAGNOSIS;
#endif			
			break;

		case IS_DIAGNOSIS:			
			if(Diag_Init() == 0UL)
			{   
			    is = IS_SOH;
			}
#ifdef ADC_DEBUG			
			is = IS_SOH;
#endif				
			break;

		case IS_SOH:
		    Soh_SetUpdateFlag(TRUE);
		    Soh_Calculation_Task();
//			CapacityData.FCC = (float)CapacityData.FCC * (float)Main_PackData.SOH / (float)100;
			is = IS_SOC;
			break;

		case IS_SOC:
			Soc_Init();
			Soc_SetSocInitFlag(TRUE);
			is = IS_COMPLETE;
			break;

		case IS_RELAY_CON :
			break;

		case IS_COMPLETE :
			break;

		default :
			break;
	}

	#ifdef Test_System_InitSequence
	is = IS_COMPLETE;
	#endif
	
	return is;
}


/*---------------------------------------------------------------------*/
/**
* @brief    Operation in wait state
* @param    void
* @return   2-exit unnormal , 1-wait, 0-exit normal
*/
uint8 System_Wait(void)
{
	static uint8 seq = 0;
    uint8 rv = 0;
	uint16  Contactor_Operating_RefV;

	Contactor_Operating_RefV = Parameter_Get_Contactor_Operating_RefV();

	switch(seq)
	{
		case 0:				// compare bus V and pack V
			if(Can_GetContactorCmd() == FALSE)
			{
			    rv = 2;
			}
			else
			{ 
			    if(PackVolt_Get_V_BusMeas() <= AppInit_Get_MinDchVolt())
				{
					seq = 1;
				}
				else
				{
					if((MAX(PackVolt_Get_V_BusMeas(), PackVolt_Get_V_PackMeas()) - 
					    MIN(PackVolt_Get_V_BusMeas(), PackVolt_Get_V_PackMeas())) <= Contactor_Operating_RefV)
					{
						seq = 1;
					}
				}
			}
			break;
		case 1:				// relay on
			Relay_SetControlCommand(RC_ON);
			seq = 2;		
			break;
			
		case 2:				// wait relay on complete
			if(Relay_GetControlCommand() == RC_CHECK)	//if((StringData_1.Relay_Status == RS_CLOSE) || (StringData_2.Relay_Status == RS_CLOSE))			
			{
				seq = 0;
				rv = 1; 						
			}
			break;
			
		default:	break;
	}

	return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief    Operation in Drive state
* @param    void
* @return   TRUE - Exit Drive State, FALSE - in Drive State
*/
BOOL System_Drive(void)
{
	static uint8 seq = 0;
    BOOL rv = FALSE;

	switch(seq)
	{
		case 0:
			if(Can_GetContactorCmd() == FALSE)
			{
				Relay_SetControlCommand(RC_OFF);				
				seq = 1;
			}
			break;
			
		case 1:
			if(Relay_GetControlCommand() == RC_CHECK)
			{
				seq = 0;
				rv = TRUE;
			}
			
			break;

		default:	break;
	}

	return rv ; 
}


/*---------------------------------------------------------------------*/
/**
* @brief    Operation in Error state
* @param    void
* @return   TRUE - Exit Error State, FALSE - in Error State
*/
BOOL System_Error(void)
{
	static uint8 seq = 0;
    BOOL rv = FALSE;

	switch(seq)
	{
		case 0:
			Relay_SetControlCommand(RC_OFF);
			seq = 1;			
			break;
			
		case 1:
			if(Relay_GetControlCommand() == RC_CHECK)		// wait relay off 		//if((StringData_1.Relay_Status == RS_OPEN) || (StringData_2.Relay_Status == RS_OPEN))
			{
				seq = 2u;
			}			
			break;

		case 2:
			if(Diag_Get_FlagsFault() == 0UL)	
			{
				seq = 0u; 
				rv = TRUE; 
			}
			break; 
			
		default :	break;
	}	

	return rv; 
}


/*---------------------------------------------------------------------*/
/**
* @brief    Operation in Emergency state
* @param    void
* @return   TRUE - Exit emergency state, FALSE - in emergency State
*/
BOOL System_Emergency(void)
{
    BOOL rv = FALSE;
    
	switch(System_EmergencySeq)
	{
		case 0:			
			Relay_SetAllRelayLow(RELAY_OFF);
			Relay_SetControlCommand(RC_CHECK);
			Relay_Set_Relay_Status(RS_OPEN);
			
			System_EmergencySeq = 1;
			break; 
			
		case 1:
			if(Relay_GetControlCommand() == RC_CHECK)		// wait relay off 		//if((StringData_1.Relay_Status == RS_OPEN) || (StringData_2.Relay_Status == RS_OPEN))
			{
				System_EmergencySeq = 2;
			}
			break;
			
		case 2:								// wait release 
			if(HvilEpo_IsNormal() == TRUE)
			{
				System_EmergencySeq = 0;
				rv = TRUE;
			}
			break;
			
		default:
			break;
	}	

	return rv;
}


/*---------------------------------------------------------------------*/
uint8 ManaMsm_Get_HeartBeat(void)
{
    return HeartBeat;
}



