/*
 * App_Fan.c
 *
 *  Created on: Sep 22, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Fan.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define FAN_TEMP_SCALE              (sint16)10
#define FAN_OP_RELEASE_DEGREE       (sint16)(3 * FAN_TEMP_SCALE)
#define FAN_ERROR_LMT               3u


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/
typedef enum{
    FAN_SEQ_OFF,
	FAN_SEQ_ON,
	FAN_SEQ_RUNNING,	
}Fan_Control_Seq;
	
typedef enum{
    FAN_CtrlMode_SELF,
    FAN_CtrlMode_REMOTE,
    FAN_CtrlMode_SELForREMOTE
}Fan_Control_Mode;

typedef struct{
    BOOL                Error_Flag;
    Fan_State           RemoteCtrlCmd;
    Fan_State           Output;
    Fan_State           State;
    Fan_Control_Seq     CtrlSeq;
    Fan_Control_Mode    CtrlMode;    
}FAN_CONTROL;


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/

/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static FAN_CONTROL  FanCtrl;
static uint8        FAN_State;


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
* @brief	Fan Control initialization
* @param	void
* @return	void
*/
void Fan_Control_Init(void)
{
	FanCtrl.Error_Flag      = FALSE;
	FanCtrl.RemoteCtrlCmd   = FAN_SW_OFF;
	FanCtrl.State           = FAN_SW_OFF;
	FanCtrl.Output          = FAN_SW_OFF;
	FanCtrl.CtrlSeq         = FAN_SEQ_OFF;
	FanCtrl.CtrlMode        = FAN_CtrlMode_REMOTE;

	(void)Fan_SetStateHigh(FAN_SW_ON); 
	(void)Fan_SetStateLow(FAN_SW_OFF); 	
}

/*---------------------------------------------------------------------*/
/**
* @brief	Fan Control Task
* @param	void
* @return	void
*/
void Fan_Control_Task(void)
{
    static uint16 FanErrorCount = 0u;
	sint16 T_CellMax;
	sint16 Operate_Fan_Temp;

	T_CellMax = Adbms_Get_T_CellMax();
	Operate_Fan_Temp = Parameter_Get_Operate_Fan_Temp();

    FanCtrl.State = Fan_GetState();
    FAN_State = (uint8)FanCtrl.State;
    
    if(FanCtrl.Error_Flag == FALSE)   
    {
        switch(FanCtrl.CtrlSeq)
        {
            case FAN_SEQ_OFF:       
                (void)Fan_SetStateHigh(FAN_SW_ON);
                
                if(FanCtrl.CtrlMode == FAN_CtrlMode_SELF)
                {
                    if(T_CellMax >= Operate_Fan_Temp)
                    {
                        FanCtrl.CtrlSeq = FAN_SEQ_ON;
                    }
                }
                else if(FanCtrl.CtrlMode == FAN_CtrlMode_REMOTE)
                {
                    if(FanCtrl.RemoteCtrlCmd == FAN_SW_ON)
                    {
                        FanCtrl.CtrlSeq = FAN_SEQ_ON;    
                    }
                }
                else if(FanCtrl.CtrlMode == FAN_CtrlMode_SELForREMOTE)
                {
                    if((T_CellMax >= Operate_Fan_Temp) || (FanCtrl.RemoteCtrlCmd == FAN_SW_ON))
                    {
                        FanCtrl.CtrlSeq = FAN_SEQ_ON;
                    }
                }
                else
                {}
                break;
                
            case FAN_SEQ_ON:
                (void)Fan_SetStateLow(FAN_SW_ON);
                FanCtrl.CtrlSeq = FAN_SEQ_RUNNING;
                FanCtrl.Output = FAN_SW_ON;
                break;
                
            case FAN_SEQ_RUNNING:                
                if(FanCtrl.CtrlMode == FAN_CtrlMode_SELF)
                {
                    if(T_CellMax < (Operate_Fan_Temp - FAN_OP_RELEASE_DEGREE))
                    {                        
                        FanCtrl.CtrlSeq = FAN_SEQ_OFF;
                    }
                }
                else if(FanCtrl.CtrlMode == FAN_CtrlMode_REMOTE)
                {
                    if(FanCtrl.RemoteCtrlCmd == FAN_SW_OFF)
                    {
                        FanCtrl.CtrlSeq = FAN_SEQ_OFF;
                    }
                }
                else if(FanCtrl.CtrlMode == FAN_CtrlMode_SELForREMOTE)
                {
                    if((T_CellMax < (Operate_Fan_Temp - FAN_OP_RELEASE_DEGREE)) && (FanCtrl.RemoteCtrlCmd == FAN_SW_OFF))
                    {
                        FanCtrl.CtrlSeq = FAN_SEQ_OFF;
                    }
                }
                else
                {}

                if(FanCtrl.CtrlSeq == FAN_SEQ_OFF)
                {
                    (void)Fan_SetStateLow(FAN_SW_OFF);
                    FanCtrl.Output = FAN_SW_OFF;
                }
                else
                {
                    if((FanCtrl.State == FAN_SW_OFF))       //|| (Main_PackData.FlagsWarn & DIAG_B_FAN_Error))
                    {
                        FanErrorCount++;
                        if(FanErrorCount >= (FAN_ERROR_LMT - 1u))
                        {
                            FanErrorCount = 0u;
                            FanCtrl.Error_Flag = TRUE;                                
                        }
                    }
                    else
                    {
                        if(FanErrorCount > 0u)
                        {
                            FanErrorCount--;
                        }
                    }
                }

                break;
                
            default:
                break;
        }
    }
    else
    {
        FanCtrl.CtrlSeq = FAN_SEQ_OFF;
        FanCtrl.Output = FAN_SW_OFF;
	    (void)Fan_SetStateLow(FAN_SW_OFF);
	    (void)Fan_SetStateHigh(FAN_SW_OFF);
    }

}


/*---------------------------------------------------------------------*/
/**
* @brief    Get Fan Control Command
* @param    void
* @return   BOOL, TRUE - Fan On, FALSE - X
*/
BOOL Fan_GetControlCMD(void)
{
	return (bool)FanCtrl.RemoteCtrlCmd;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Set Fan Control Command
* @param	Fan_ctrl_cmd - Fan Control Command
* @return	void
*/
void Fan_SetControlCMD(BOOL Fan_ctrl_cmd)
{
	FanCtrl.RemoteCtrlCmd = (Fan_State)Fan_ctrl_cmd;
}


/*---------------------------------------------------------------------*/
uint8 AppFAN_Get_FAN_State(void)
{
    return FAN_State;
}

/* End of App_Fan.c */

