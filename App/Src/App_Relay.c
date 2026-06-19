/*
 * App_Relay.c
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Relay.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define RELAY_PRE_ON_TIME				(10u - 1u)
#define RELAY_PRE_OFF_TIME				(10u - 1u)				

#define RELAY_IN_CHARGE_OFF_TIME		(50u - 1u)
#define RELAY_IN_DISCHARGE_OFF_TIME		(50u - 1u)

#define RELAY_DELAY_FOR_CONFIRM			(30u - 1u)		

#define RELAY_PRECHARGE_TIMEOUT			500u

#define RELAY_MIN_CURR_FOR_CHECK_LOAD	(sint16)10		/* [1 A] */

#define DIAG_RELAY_ERR_PRE			0x040000UL
#define DIAG_RELAY_ERR_NEG			0x080000UL
#define	DIAG_RELAY_ERR_POS			0x100000UL


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/
typedef enum
{
	STATE_ON_CHECK,
	ON_OPERATE,
	COMPLETE_ON_OK,
	COMPLETE_ON_FAIL
}Relay_OnSeq;
	
typedef enum
{
	CHECK_STR,
	PLUS_ON,
	PLUS_OFF,
	MINUS_ON,
	MINUS_OFF,
	PRE_ON,
	PRE_OFF,
	CONFIRM_OFF,
	MINUS_OFF2,
	PLUS_OFF2,
}Relay_OperateStatus;

typedef enum
{
	STATE_OFF_CHECK,
	IN_CHARGING,
	IN_DISCHARGING,
	IN_NORMAL,
	COMPLETE_OFF_OK,
	COMPLETE_OFF_FAIL	
}Relay_OffSeq;


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static Relay_CtrlCmd    Relay_Control_Cmd;

CLASSB static BOOL             Relay_CriticalOpen_Cmd;
static uint8 	            Relay_Test_Cmd;
CLASSB static RELAY_STATUS			Relay_Status;

CLASSB static uint8			Relay_OP_ErrCode;
CLASSB static uint8			Relay_Input_V;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
BOOL Relay_IsOnComplete(void);
BOOL Relay_OnSequence(void);
BOOL Relay_IsOffComplete(void);
void Relay_Test(void);
bool Relay_IsFaultState(void);

/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
* @brief	Relay app initialization
* @param	void
* @return	void
*/
extern void Relay_App_Init(void)
{
    Relay_SetAllRelayHigh(RELAY_ON);
    Relay_SetAllRelayLow(RELAY_OFF);

    Relay_Control_Cmd = RC_READY;
    Relay_CriticalOpen_Cmd = FALSE;
    Relay_Test_Cmd = 0;
}

/*---------------------------------------------------------------------*/
/**
* @brief	Relay control sequence task
* @param	void
* @return	void
*/
void Relay_Control_Task(void)
{
	Relay_Input_V = Relay_GetAllRelay();
	SYSTEM_STATE System_State;

	System_State = ManaMsm_Get_System_State();

	switch(Relay_Control_Cmd)
	{
		case RC_READY: 	/* Standby running */
			Relay_Control_Cmd = RC_CHECK;
			break; 
			
		case RC_ON:	   	/* Relay on sequence */
			if(Relay_IsOnComplete() == TRUE)
			{
			    Relay_Control_Cmd = RC_READY;
			}
			break;
			
		case RC_OFF:	/* Relay off sequence */
			if(Relay_IsOffComplete() == TRUE)
			{
			    Relay_Control_Cmd = RC_READY;
			}
			break;
			
		case RC_CHECK:	/* Relay Status Checking */		
			if((HvilEpo_IsNormal() == TRUE)  &&  System_State != SYS_EMERGENCY)
			{
				static uint8 r_cnt = 0;
					
				if(Relay_GetMainRelayCommand() != Relay_Input_V)
				{
					if(r_cnt++ >= 2u)
					{
						uint8 buf;
						r_cnt = 0;
						
						Relay_Status = RS_FAULT; 
						buf = Relay_GetMainRelayCommand() ^ Relay_Input_V;
						if((buf & RELAY_PRE_FLAG) == RELAY_PRE_FLAG)
						{
						    Relay_OP_ErrCode |= RELAY_PRE_FLAG;
						}						
                        if((buf & RELAY_NEG_FLAG) == RELAY_NEG_FLAG)
						{
						    Relay_OP_ErrCode |= RELAY_NEG_FLAG;
                        }
						if((buf & RELAY_POS_FLAG) == RELAY_POS_FLAG)
						{
						    Relay_OP_ErrCode |= RELAY_POS_FLAG;
					    }

						if(Relay_Input_V > 0u)
						{
						    Relay_SetAllRelayLow(RELAY_OFF);
						    Relay_SetAllRelayHigh(RELAY_OFF);
						}
					}
				}			
				else
				{
                    if(Relay_IsFaultState() == FALSE)
                    {
				        Relay_SetAllRelayHigh(RELAY_ON);
				    }
				    r_cnt = 0;
				}
			}			
			break;

		case RC_TEST:			
			Relay_Test();			
			break;
			
		default :	break;
	}	
}

/*---------------------------------------------------------------------*/
/**
* @brief    Check if finish Relay On Sequence
* @param	void
* @return	BOOL TRUE - Complete, FALSE - In Process
*/
BOOL Relay_IsOnComplete(void)
{
	static Relay_OnSeq seq = STATE_ON_CHECK;	
	BOOL rv = FALSE;
	
	switch(seq)
	{
		/* Current relay condition check */
		case STATE_ON_CHECK:
			if(Diag_Get_FlagsFault() > 0UL)
			{
				seq = COMPLETE_ON_OK;
			}
			else
			{
				seq = ON_OPERATE;
			}

			break;
			
		case ON_OPERATE:
			if(Relay_OnSequence() == TRUE)
			{
				#ifdef TestMode
				#else
				if(Relay_Status == RS_FAULT)
				{
					Relay_SetAllRelayLow(RELAY_OFF);			
				}
				#endif
				seq = COMPLETE_ON_OK;
			}			
			break;
			
		case COMPLETE_ON_OK:
			seq = STATE_ON_CHECK;
			rv = TRUE;
			break;
			
		default :		break;
	}		
	
 	return rv;
}

/*---------------------------------------------------------------------*/
/**
* @brief	Relay On Sequence
* @param	void
* @return	BOOL TRUE - Complete, FALSE - In Process
*/
BOOL Relay_OnSequence(void)
{
#define PRECHARGE_V_RATIO           (float)0.9
#define RETRY_CONT_ON_LMT           (10u - 1u)

	static Relay_OperateStatus  OP          = CHECK_STR;
	static uint8                   Rcheck_cnt  = 0u;
	static uint16                  rcnt        = 0u;
    BOOL                        rv          = FALSE;
	Relay_State	                rs;
	float	fdata;

	switch(OP)
	{
	    /* Check relay error */
		case CHECK_STR:
			if((Relay_Status == RS_CLOSE) || (Relay_Status == RS_FAULT))
			{
				rv = TRUE; 		
			}
			else
			{
				Relay_Status = RS_CHANGING;
				OP = PLUS_ON;
			}								
			break;

    	/* Positive relay close */
		case PLUS_ON:		
			Relay_SetPosRelayLow(RELAY_ON);			
			OP = PRE_ON;
			Rcheck_cnt = 0u;
			rcnt = 0u;
			break;

		/* Precharge relay close */
		case PRE_ON:
			if (rcnt++ >= RELAY_PRE_ON_TIME)
			{
				rs = Relay_GetPosRelay();
				if (rs == RELAY_ON)
				{		
					Rcheck_cnt = 0u;
					rcnt = 0u;
					Relay_SetPreRelayLow(RELAY_ON);
					Relay_Status = RS_PRECHG;
					OP = MINUS_ON;
				}
				else
				{
					if(Rcheck_cnt++ >= RETRY_CONT_ON_LMT)
					{
						Rcheck_cnt = 0u;
						rcnt = 0u;						
						Relay_OP_ErrCode |= RELAY_POS_FLAG;
						Relay_Status = RS_FAULT;
						OP = CHECK_STR;
						rv = TRUE;
					}
				}		
			}
			break;

		/* Negative relay close */
		case MINUS_ON:
			fdata = (float)PackVolt_Get_V_PackMeas() * PRECHARGE_V_RATIO;
			if(PackVolt_Get_V_BusMeas() >= (uint16)fdata)
			{
				rcnt = 0u;
				Relay_SetNegRelayLow(RELAY_ON);
				OP = PRE_OFF;
			}			
			else
			{
				if(rcnt++ >= RELAY_PRECHARGE_TIMEOUT)
				{
					rcnt = 0u;
					Relay_OP_ErrCode |= RELAY_PRE_FLAG;
					Relay_Status = RS_FAULT;
					OP = CHECK_STR;
					rv = TRUE;					
				}
			}
			break;
		
		/* Precharge relay open after 100ms */
		case PRE_OFF:
			if ((rcnt++ >= RELAY_PRE_OFF_TIME))
			{
				rs = Relay_GetNegRelay(); 
				
				if(rs == RELAY_ON)
				{
					rcnt = 0u;
					Rcheck_cnt=0u;
					Relay_SetPreRelayLow(RELAY_OFF); 
					Relay_Status = RS_CLOSE; 
					OP = CHECK_STR; 
					rv = TRUE;
				}
				else
				{
					if(Rcheck_cnt++ >= RETRY_CONT_ON_LMT)
					{
						rcnt = 0u;
						Rcheck_cnt = 0u;
						Relay_OP_ErrCode |= (uint8)RELAY_NEG_FLAG;			
						Relay_Status = RS_FAULT; 
						OP = CHECK_STR; 
						rv = TRUE;
					}
				}
			}
			break;
		default :	break;						
	}	

	return rv;
}

/*---------------------------------------------------------------------*/
/**
* @brief    Check if finish Relay Off Sequence
* @param	void
* @return	BOOL TRUE - Complete, FALSE - In Process
*/
BOOL Relay_IsOffComplete(void)
{
	CLASSB static Relay_OffSeq 		seq;
	CLASSB static Relay_OperateStatus	OP;	
	CLASSB static uint16 rcnt;	 
    BOOL rv = FALSE;

	uint16	ContOpenCount;

	ContOpenCount = SOC_Get_ContOpenCount_UnderLoad();
	
	switch(seq)
	{
		/* Current relay condition check */
		case STATE_OFF_CHECK:					

			if(Curr_Get_I_PackMeas() > RELAY_MIN_CURR_FOR_CHECK_LOAD)
			{
				ContOpenCount++;
				SOC_Set_ContOpenCount_UnderLoad(ContOpenCount);
				seq = IN_CHARGING;
				OP = MINUS_OFF;
			}
			else if(Curr_Get_I_PackMeas() < (RELAY_MIN_CURR_FOR_CHECK_LOAD * (-1)))
			{
				ContOpenCount++;
				SOC_Set_ContOpenCount_UnderLoad(ContOpenCount);
				seq = IN_DISCHARGING;	
				OP = PLUS_OFF;
			}
			else
			{	
				if(Relay_Status == RS_OPEN)
				{
					seq = COMPLETE_OFF_OK;
				}
				else
				{
					if(Relay_CriticalOpen_Cmd == TRUE)
					{
					    Relay_CriticalOpen_Cmd = FALSE;
						seq = IN_DISCHARGING;
						OP = PLUS_OFF;
					}
					else
					{
						seq = IN_NORMAL; 		
						OP = PRE_ON;
					}
				}
			}			
						
			Relay_Status = RS_CHANGING;
			break;
			
		case IN_CHARGING:
			switch(OP)
			{
				case MINUS_OFF:
					Relay_SetNegRelayLow(RELAY_OFF);
					OP = PLUS_OFF;
					rcnt = 0;
					break;
				case PLUS_OFF:
					if (rcnt++ >= RELAY_IN_CHARGE_OFF_TIME)
					{
						rcnt = 0 ;
						Relay_SetPosRelayLow(RELAY_OFF);	
						OP = CONFIRM_OFF ;
					}
					break;					
				case CONFIRM_OFF:
					if(rcnt++ > RELAY_DELAY_FOR_CONFIRM)
					{
						rcnt = 0;						
						if(Relay_GetAllRelay() == 0u)
						{
							seq = COMPLETE_OFF_OK;
						}
						else
						{
							seq = COMPLETE_OFF_FAIL;	
						}			
					}
					break;					
				default :	break;
			}			
			break;
			
		case IN_DISCHARGING:
			switch (OP)
			{
				case PLUS_OFF:
					Relay_SetPosRelayLow(RELAY_OFF);	
					OP = MINUS_OFF;
					rcnt = 0;
					break;
				case MINUS_OFF:
					if(rcnt++ >= RELAY_IN_DISCHARGE_OFF_TIME)
					{
						rcnt = 0;
						Relay_SetNegRelayLow(RELAY_OFF);	
						OP  = CONFIRM_OFF;
					}
					break;
				case CONFIRM_OFF:
					if(rcnt++ > RELAY_DELAY_FOR_CONFIRM)
					{
						rcnt = 0;						
						if(Relay_GetAllRelay() == 0u)
						{
							seq = COMPLETE_OFF_OK;
						}
						else
						{	
							seq = COMPLETE_OFF_FAIL;	
						}	
					}
					break;
				default :	break; 
					
			}
			break;
			
		case IN_NORMAL:
			switch (OP)
			{
				case PRE_ON:
					if(Relay_Status == RS_OPEN)
					{
					    OP = CONFIRM_OFF;
					}    
					else
					{
						if(rcnt++ >= 9u)
						{				
							rcnt = 0;
							Relay_SetPreRelayLow(RELAY_ON);
							OP = MINUS_OFF2;										
						}
					}
					break;
				case MINUS_OFF2:
					if(rcnt++ >= 19u)					
					{
						rcnt = 0;
						Relay_SetNegRelayLow(RELAY_OFF);
						OP = PRE_OFF;
					}
					break;
				case PRE_OFF:
					if(rcnt++ >= 9u)
					{
						rcnt = 0;
						Relay_SetPreRelayLow(RELAY_OFF);
						OP = PLUS_OFF2;
					}
					break;
				case PLUS_OFF2:
					if(rcnt++ >= 9u)
					{
						rcnt = 0;
						Relay_SetPosRelayLow(RELAY_OFF);
						OP = CONFIRM_OFF;					
					}
					break;
				case CONFIRM_OFF:
					if(rcnt++ > RELAY_DELAY_FOR_CONFIRM)
					{
						rcnt = 0;						
						if(Relay_GetAllRelay() == 0u)
						{
							seq = COMPLETE_OFF_OK;
						}
						else
						{
	
							seq = COMPLETE_OFF_FAIL;	
						}	
					}				
					break;
				default :	break;
			}			
			break;
	
		case COMPLETE_OFF_OK:
			seq = STATE_OFF_CHECK;
			Relay_Status = RS_OPEN;
			rv = TRUE;
			break;

		case COMPLETE_OFF_FAIL:
			seq = STATE_OFF_CHECK;
			
			if(Relay_GetNegRelay() == RELAY_ON)
			{
			    Relay_OP_ErrCode |= RELAY_NEG_FLAG;
            }
			if(Relay_GetPosRelay() == RELAY_ON)
            {
                Relay_OP_ErrCode |= RELAY_POS_FLAG;
            }
			if(Relay_OP_ErrCode > 0u)
            {
                Relay_Status = RS_FAULT;
            }
			else
			{
			    Relay_Status = RS_OPEN; 
            }
			Relay_SetAllRelayLow(RELAY_OFF);
			rv = TRUE ; 					
			break;

		default :	break;
		
	}
	
	return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief    Relay Test function by CAN communication
* @param    void
* @return   void
*/
void Relay_Test(void)
{
	if(Relay_Test_Cmd > 0u)
	{
		switch(Relay_Test_Cmd)
		{
			case 0x10:
				Relay_SetNegRelayLow(RELAY_ON);
				break;
			
			case 0x11:
				Relay_SetNegRelayLow(RELAY_OFF);
				break;
			
			case 0x12:
				Relay_SetPosRelayLow(RELAY_ON);
				break;
			
			case 0x13:
				Relay_SetPosRelayLow(RELAY_OFF);
				break;
			
			case 0x14:
				Relay_SetPreRelayLow(RELAY_ON);
				break;
			
			case 0x15:
				Relay_SetPreRelayLow(RELAY_OFF);
				break;
			
			default:	
				break;
		}			
		
		Relay_Test_Cmd = FALSE;
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief    Set the relay control command by CAN communication
* @param    Relay_CtrlCmd - Relay Control Command
* @return   void
*/
extern void Relay_SetControlCommand(Relay_CtrlCmd cmd)
{
	Relay_Control_Cmd = cmd;
}


/*---------------------------------------------------------------------*/
/**
* @brief    Get the relay control command
* @param    void
* @return   Relay_CtrlCmd - Relay Control Command
*/
extern Relay_CtrlCmd Relay_GetControlCommand(void)
{
	return Relay_Control_Cmd;
}


/*---------------------------------------------------------------------*/
/**
* @brief    Set the relay test command by CAN communication
* @param    BOOL - Set Value
* @return   void
*/
extern void Relay_SetTestCommand(uint8 cmd)
{
	Relay_Test_Cmd = cmd;
}

/*---------------------------------------------------------------------*/
/**
* @brief    Set the relay critical open command by CAN communication
* @param    BOOL - Set Value
* @return   void
*/
extern void Relay_Set_CriticalOpen(BOOL cmd)
{
    Relay_CriticalOpen_Cmd = cmd;
}


/*---------------------------------------------------------------------*/
/**
* @brief    Relay fault state checking
* @param    void
* @return   bool TRUE - Fault, FALSE - normal
*/
bool Relay_IsFaultState(void)
{
    bool rv = FALSE;
    uint8 buf = 0u;

    if((Diag_Get_FlagsFault() & DIAG_RELAY_ERR_PRE) == DIAG_RELAY_ERR_PRE)
    {
        buf |= RELAY_PRE_FLAG;                
    }
    if((Diag_Get_FlagsFault() & DIAG_RELAY_ERR_NEG) == DIAG_RELAY_ERR_NEG)
    {
        buf |= RELAY_NEG_FLAG;                
    }
    if((Diag_Get_FlagsFault() & DIAG_RELAY_ERR_POS) == DIAG_RELAY_ERR_POS)
    {
        buf |= RELAY_POS_FLAG;                
    }

    if(buf > 0u)
    {
        rv = TRUE;
    }
    
    return rv;
}


/*---------------------------------------------------------------------*/
RELAY_STATUS Relay_Get_Relay_Status(void)
{
    return Relay_Status;
}


/*---------------------------------------------------------------------*/
void Relay_Set_Relay_Status(RELAY_STATUS status)
{
    Relay_Status = status;
}



/*---------------------------------------------------------------------*/
uint8 Relay_Get_Relay_OP_ErrCode(void)
{
    return Relay_OP_ErrCode;
}


/*---------------------------------------------------------------------*/
uint8 Relay_Get_Relay_Input_V(void)
{
    return Relay_Input_V;
}






