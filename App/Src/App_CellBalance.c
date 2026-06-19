/*
 * App_CellBalance.c
 *
 *  Created on: Sep 10, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_CellBalance.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define CELLBALANCE_STOP				0u
#define CELLBALANCE_SEARCH				1u
#define CELLBALANCE_RUN					2u
#define CELLBALANCE_HUSE				3u

#define CELLBALANCE_STARTVOLT			5000u    	/* [mV] */
#define CELLBALANCE_STOPVOLT			5000u		/* [mV] */
#define CELLBALANCE_SAFETYVOLT  		3000u    	/* [mV] */

#define CELLBALANCE_PROGRESS_TIME		2000u    	/* [msec] */
#define CELLBALANCE_HUSE_TIME    		1000u     	/* [msec] */
#define CELLBALANCE_INTERVAL    		100u		/* [msec] */

#define CELLBALANCE_OVER_TEMP       	(50 * 10)	/* 50'C, [0.1'C] */
#define CELLBALANCE_OVER_TEMP_RELEASE	(45 * 10)	/* 45'C, [0.1'C] */

#define CELLBALANCE_SOC_LIMIT			(20u * 10u)	/* 20%, [0.1%] */
#define CELLBALANCE_MIN_CURR			((sint16)2  * (sint16)10)	/* 2A,  [0.1A] */


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/



/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/
static CellBalance_Ctrl_t	CellBalance_Ctrl;


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static bool CellBalance_ForcedOff = DEF_Off;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
bool 	CellBalance_IsOverTemp(void);



/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
 * @brief Cell balance Initialization function       
 * @param void
 * @return 
*/
void CellBalance_Init(void)
{
    CellBalance_Ctrl.Bal_StartV  = CELLBALANCE_STARTVOLT;
    CellBalance_Ctrl.Bal_StopV 	 = CELLBALANCE_STOPVOLT;
    CellBalance_Ctrl.Bal_SafetyV = CELLBALANCE_SAFETYVOLT;    

    CellBalance_Ctrl.Bal_Cnt 	 = 0u;
    CellBalance_Ctrl.Bal_Seq 	 = CELLBALANCE_STOP;
    CellBalance_Ctrl.Bal_Count   = 0u;
}


/*---------------------------------------------------------------------*/
/**
 * @brief Cell balance control function       
 * @param void
 * @return
*/
void CellBalance_Control_Task(void)
{
	
	if(Adbms_FirstReadState() == DEF_True)
	{    
		uint16  bmic_num;
		uint8  cell_num;
		static uint8 checkCount = 0u;
		static Adbms6815_FETCtrl Fet_ctrl = ODD;

		bool is_over_temp = CellBalance_IsOverTemp();
		uint32 front_spi_ok = Adbms6815_FrontSPI_Error_Check();
		uint32 rear_spi_ok  = Adbms6815_RearSPI_Error_Check();

		uint16 V_CellMin;
		uint16 Dev_for_BalanceStart;
		uint16 Dev_for_BalanceStop;
		uint16 NumOf_Bmic;
		uint16 NumOfCellinModule;
		uint16 SOC_Real;
		sint16 I_PackMeas;

		V_CellMin = Adbms_Get_V_CellMin();
		Dev_for_BalanceStart = Parameter_Get_BalanceStartDev();
		Dev_for_BalanceStop = Parameter_Get_BalanceStopDev();
		NumOf_Bmic = Parameter_Get_NumOf_Bmic();
		NumOfCellinModule = Parameter_Get_NumOfCellinModule();
		SOC_Real = SOC_Get_SOC_Real();
		I_PackMeas = Curr_Get_I_PackMeas();

		if(CanUiRx_Get_BalanceTest() == 0u)
		{
			if(checkCount >= 1u)
			{
				CellBalance_Ctrl.Bal_StartV = CELLBALANCE_STARTVOLT;
	            CellBalance_Ctrl.Bal_StopV = CELLBALANCE_STOPVOLT;
	            CellBalance_Ctrl.Bal_Seq = CELLBALANCE_STOP;
	            CellBalance_Ctrl.Bal_Cnt = 0u;
				checkCount = 0;
			}
			else
			{
		    	if(  (SOC_Real >= CELLBALANCE_SOC_LIMIT)
		    	  && (I_PackMeas 	> (sint16)CELLBALANCE_MIN_CURR * (-1))
		    	  && (I_PackMeas 	< (sint16)CELLBALANCE_MIN_CURR       ) 
		    	  && (is_over_temp == DEF_False)
		    	  && (front_spi_ok == 0u)
		    	  && (rear_spi_ok == 0u)	)
		        {
		            CellBalance_Ctrl.Bal_StartV = V_CellMin + Dev_for_BalanceStart;
		            CellBalance_Ctrl.Bal_StopV = V_CellMin + Dev_for_BalanceStop;
		        }
		        else
		        {
		            CellBalance_Ctrl.Bal_StartV = CELLBALANCE_STARTVOLT;
		            CellBalance_Ctrl.Bal_StopV = CELLBALANCE_STOPVOLT;
		            CellBalance_Ctrl.Bal_Seq = CELLBALANCE_STOP;
		            CellBalance_Ctrl.Bal_Cnt = 0u;
		        }
		        if(CellBalance_ForcedOff == DEF_True)
		        {
		            CellBalance_Ctrl.Bal_StartV = CELLBALANCE_STARTVOLT;
		            CellBalance_Ctrl.Bal_StopV = CELLBALANCE_STOPVOLT;
		            CellBalance_Ctrl.Bal_Seq = CELLBALANCE_STOP;
		            CellBalance_Ctrl.Bal_Cnt = 0u;
		        }
			}
		}

		else if((CanUiRx_Get_BalanceTest() == 1u) && (is_over_temp == DEF_False))
		{
			checkCount++;
			if(checkCount >= 5u)
			{
				checkCount = 5;
			}
			CellBalance_Ctrl.Bal_StartV = 3100u;
	        CellBalance_Ctrl.Bal_StopV = 3000u;
		}

		else
		{

		}
		
		if(Adbms_GetErrorDischargeResistor() == TRUE)
		{
			CellBalance_Ctrl.Bal_StartV = CELLBALANCE_STARTVOLT;
            CellBalance_Ctrl.Bal_StopV = CELLBALANCE_STOPVOLT;
            CellBalance_Ctrl.Bal_Seq = CELLBALANCE_STOP;
            CellBalance_Ctrl.Bal_Cnt = 0u;
		}

		if(Adbms_GetErrorVCellOffline() == TRUE)
		{
			CellBalance_Ctrl.Bal_StartV = CELLBALANCE_STARTVOLT;
            CellBalance_Ctrl.Bal_StopV = CELLBALANCE_STOPVOLT;
            CellBalance_Ctrl.Bal_Seq = CELLBALANCE_STOP;
            CellBalance_Ctrl.Bal_Cnt = 0u;
		}
		
        CellBalance_Ctrl.Bal_State = Adbms_Get_CellBalanceStatus(CellBalance_Ctrl.Bal_Status);
    	
    	switch(CellBalance_Ctrl.Bal_Seq)
    	{
		
    		case CELLBALANCE_STOP:
    			CellBalance_Ctrl.Bal_Cnt = 0u;
               
    			if(CellBalance_Ctrl.Bal_State == 1u)
    			{
    			    Adbms_Set_CellBalanceCmd(CellBalance_Ctrl.Bal_Output, ALL_OFF);
    			}
    			else
    			{
    				CellBalance_Ctrl.Bal_Count = 0u;
    			    CellBalance_Ctrl.Bal_Seq = CELLBALANCE_HUSE;
    			    for(bmic_num = 0; bmic_num < NumOf_Bmic; bmic_num++)
    			    {
    			        CellBalance_Ctrl.Bal_Output[bmic_num] = 0u;
    			    }
    			}
    			break;
    			
    		case CELLBALANCE_SEARCH:
			
    		    CellBalance_Ctrl.Bal_Cnt = 0u;
    		    CellBalance_Ctrl.Bal_Seq = CELLBALANCE_HUSE;
    		
    		    for(bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
    		    {
    				for(cell_num = 0u; cell_num < 4u; cell_num++)
    				{	
                        if(CellBalance_Ctrl.Bal_SafetyV >= Adbms_Get_CellVolt(bmic_num, cell_num))
                        {
                            CLR_BIT_Func(CellBalance_Ctrl.Bal_Output[bmic_num], cell_num);
                        }  
                        else
                        {
        					if(CHK_BIT_Func(CellBalance_Ctrl.Bal_Output[bmic_num], cell_num) > 0u)		
        					{						
        						if(CellBalance_Ctrl.Bal_StopV > Adbms_Get_CellVolt(bmic_num, cell_num))
        						{    					
        							CLR_BIT_Func(CellBalance_Ctrl.Bal_Output[bmic_num], cell_num);						
        						}
        						else
        						{
        						    CellBalance_Ctrl.Bal_Seq = CELLBALANCE_RUN;
        						}
        					}
        					else
        					{
                                if(CellBalance_Ctrl.Bal_StartV <= Adbms_Get_CellVolt(bmic_num, cell_num))
                                {                                
                                    SET_BIT_Func(CellBalance_Ctrl.Bal_Output[bmic_num], cell_num);
                                    CellBalance_Ctrl.Bal_Seq = CELLBALANCE_RUN;
                                }
        					}
                        }
    				}

    				for(cell_num = 4u; cell_num < NumOfCellinModule; cell_num++)
    				{	
                        if(CellBalance_Ctrl.Bal_SafetyV >= Adbms_Get_CellVolt(bmic_num, cell_num))
                        {
                            CLR_BIT_Func(CellBalance_Ctrl.Bal_Output[bmic_num], (cell_num + 1u));
                        }  
                        else
                        {
        					if(CHK_BIT_Func(CellBalance_Ctrl.Bal_Output[bmic_num], (cell_num + 1u)) > 0u)
        					{						
        						if(CellBalance_Ctrl.Bal_StopV > Adbms_Get_CellVolt(bmic_num, cell_num))
        						{    					
        							CLR_BIT_Func(CellBalance_Ctrl.Bal_Output[bmic_num], (cell_num + 1u));						
        						}
        						else
        						{
        						    CellBalance_Ctrl.Bal_Seq = CELLBALANCE_RUN;
        						}
        					}
        					else
        					{
                                if(CellBalance_Ctrl.Bal_StartV <= Adbms_Get_CellVolt(bmic_num, cell_num))
                                {                                
                                    SET_BIT_Func(CellBalance_Ctrl.Bal_Output[bmic_num], (cell_num + 1u));
                                    CellBalance_Ctrl.Bal_Seq = CELLBALANCE_RUN;
                                }
        					}
                        }
    				}
    		    }

    		    if		(Fet_ctrl == ODD)
    		    {
    		        Fet_ctrl = EVEN;
    		    }
    		    else if (Fet_ctrl == EVEN)
    		    {
    		        Fet_ctrl = ODD;
    		    }
    		    else
    		    {
				}
    		
    			Adbms_Set_CellBalanceCmd(CellBalance_Ctrl.Bal_Output, Fet_ctrl);					
    			break;
    			
    		case CELLBALANCE_RUN:
    			CellBalance_Ctrl.Bal_Count =  CellBalance_Count_NumberOfBalancingCell();
    			if(CellBalance_Ctrl.Bal_Cnt++ > (CELLBALANCE_PROGRESS_TIME / CELLBALANCE_INTERVAL))
    			{
    				CellBalance_Ctrl.Bal_Cnt = 0u;
    				CellBalance_Ctrl.Bal_Seq = CELLBALANCE_HUSE;
                    Adbms_Set_CellBalanceCmd(CellBalance_Ctrl.Bal_Output, ALL_OFF);
    			}

    			break;
    			
    		case CELLBALANCE_HUSE:
    			if(CellBalance_Ctrl.Bal_Cnt++ > (CELLBALANCE_HUSE_TIME / CELLBALANCE_INTERVAL))
    			{			
                    CellBalance_Ctrl.Bal_Cnt = 0u;
    			    CellBalance_Ctrl.Bal_Seq= CELLBALANCE_SEARCH;
    			}
    			else
    			{
    				if(CellBalance_Ctrl.Bal_State == 1u)
    				{
    				    Adbms_Set_CellBalanceCmd(CellBalance_Ctrl.Bal_Output, ALL_OFF);				    
    				}				
    			}
    			break;
    			
    		default:	
    			CellBalance_Ctrl.Bal_Seq = CELLBALANCE_STOP;
    			break;
    	}

		Adbms_Set_BalanceCheck(CellBalance_Ctrl.Bal_Count);
    }
}    


/*---------------------------------------------------------------------*/
/**
 * @brief Board Temperature check       
 * @param void
 * @return false - ok, true - fail
*/
bool CellBalance_IsOverTemp(void)
{
    static bool Over_RLEC_BoardTemp_Flag = DEF_False;
    uint8 i;
	uint16 NumOf_Bmic;
		
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
    
    if(Over_RLEC_BoardTemp_Flag == DEF_True)
    {
        for(i = 0u; i < NumOf_Bmic; i++)
        {
            if(Adbms_Get_BoardTemp(i) > (sint16)CELLBALANCE_OVER_TEMP_RELEASE)
            {
                break;
            }
        }        
        if(i >= NumOf_Bmic)
        {
            Over_RLEC_BoardTemp_Flag = DEF_False;
        }
    }
    else
    {
        for(i = 0u; i < NumOf_Bmic; i++)
        {
            if(Adbms_Get_BoardTemp(i) >= (sint16)CELLBALANCE_OVER_TEMP)
            {
                Over_RLEC_BoardTemp_Flag = DEF_True;
                break;
            }
        }
    }   

    return Over_RLEC_BoardTemp_Flag;
}


/*---------------------------------------------------------------------*/
/**
* @brief return number of Cell in balancing
* @param void
* @return uint16 number of cell in balancing
*/
uint16 CellBalance_Count_NumberOfBalancingCell(void)
{
	uint16 bmic_num;
	uint16 cell_num;
	uint16 counter = 0u;
	uint16 NumOf_Bmic;
	uint16 NumOfCellinModule;
		
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfCellinModule = Parameter_Get_NumOfCellinModule();
	
    	for(bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
    	{
            for(cell_num = 0u; cell_num < (NumOfCellinModule + 1u); cell_num++)
	        {
	            if((CellBalance_Ctrl.Bal_Output[bmic_num] & ((uint16)1 << cell_num)) > 0u)     
	            {    
	                counter++;     
	            }
	        }
    	}
    
	return counter;
}


/*---------------------------------------------------------------------*/
/**
* @brief return Number of balancing Cell
* @param void
* @return DEF_True - in Balancing, DEF_False - Not operation
*/
uint16 CellBalance_Get_NumberOfBalancingCell(void)
{
    return CellBalance_Ctrl.Bal_Count;
}


/*---------------------------------------------------------------------*/
/**
* @brief return Cell balance state
* @param void
* @return DEF_True - in Balancing, DEF_False - Not operation
*/
uint8 CellBalance_Get_BalancingState(void)
{
    return CellBalance_Ctrl.Bal_State;
}


/*---------------------------------------------------------------------*/
/**
* @brief forced balance on / off control
* @param void
* @return DEF_True - in Balancing, DEF_False - Not operation
*/
void CellBalance_Set_ForcedOff(bool fbc)
{
    CellBalance_ForcedOff = fbc;
}


/*---------------------------------------------------------------------*/
uint16 CellBalance_Get_BalStatus(uint16 Bmic_No)
{
    return CellBalance_Ctrl.Bal_Status[Bmic_No];
}




