/*
 * App_Soh.c
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_Soh.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/
static const uint16 Cycle_Table[26] =										
{
	8100,	7800,	7500,	7200,	6900,	6600,	6300,	6000,				// 75% ~ 82%
 	5700,	5400,	5176,	4949,	4791,	4385,	4163,	3994,				// 83% ~ 90%	
 	3355,	2750,	2496,	2285,	2049,	1761,	920,	388,	252, 	0	// 91% ~ 100%
};


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/
static BOOL g_b_SOH_updateflag = FALSE;


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static 	uint16 	SOH;


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
* @brief    State of Health Initialization
* @param    void
* @return   void
*/
void Soh_Init(void)
{
	uint16 Cycle_Count;
	uint32 FCC;
	uint32 Init_FCC;

	Cycle_Count = SOC_Get_CycleCount();
	FCC = SOC_Get_FCC();
	Init_FCC = SOC_Get_Init_FCC();

	if(g_b_SOH_updateflag == TRUE)
	{
		uint16 Cycle_SOH = 0;
		uint16 FCC_SOH = 100u;
		
		g_b_SOH_updateflag = FALSE;

		for (uint16 i = 0; i < ((uint16)sizeof(Cycle_Table) / 2u); i++)
		{
			if(Cycle_Count >= Cycle_Table[i])
			{
				Cycle_SOH = 100u - 25u + i;
				break;
			}
		}

		if(Init_FCC != 0u)
		{
			FCC_SOH = (uint16)(FCC * 100UL / Init_FCC);
		}
		
		if(FCC_SOH > 100u)
		{
		    FCC_SOH = 100u;
		}

		SOH = MIN(Cycle_SOH, FCC_SOH);
	}

}


/*---------------------------------------------------------------------*/
/**
* @brief    State of Health Calculation
* @param    void
* @return   void
*/
void Soh_Calculation_Task(void)
{
	uint16 Cycle_Count;
	uint32 FCC;
	uint32 Init_FCC;

	Cycle_Count = SOC_Get_CycleCount();
	FCC = SOC_Get_FCC();
	Init_FCC = SOC_Get_Init_FCC();

	if(g_b_SOH_updateflag == TRUE)
	{
		uint16 Cycle_SOH = 0;
		uint16 FCC_SOH = 100u;

		g_b_SOH_updateflag = FALSE;

		for (uint16 i = 0; i < ((uint16)sizeof(Cycle_Table) / 2u); i++)
		{
			if(Cycle_Count >= Cycle_Table[i])
			{
				Cycle_SOH = 100u - 25u + i;
				break;
			}
		}

		if(Init_FCC != 0u)
		{
			FCC_SOH = (uint16)(FCC * 100UL / Init_FCC);
		}
		
		if(FCC_SOH > 100u)
		{
		    FCC_SOH = 100u;
		}

		SOH = MIN(Cycle_SOH, FCC_SOH);
	}

}


/*---------------------------------------------------------------------*/
/**
* @brief    Get SOH update flag 
* @param    void
* @return   BOOL, TRUE - Update, FALSE - X
*/
BOOL Soh_GetUpdateFlag(void)
{
	return g_b_SOH_updateflag;
}


/*---------------------------------------------------------------------*/
/**
* @brief    Set SOH update flag 
* @param    TRUE - Update, FALSE - X
* @return   void
*/
void Soh_SetUpdateFlag(BOOL flag)
{
	g_b_SOH_updateflag = flag;
}


/*---------------------------------------------------------------------*/
typedef enum
{
    SOH_Init = 0,
    SOH_Ready,
    SOH_Charge,
    SOH_WaitStable,
    SOH_WaitStable_forCal,
    SOH_Calculation,
    SOH_End,
}SOH_SEQ;

typedef struct
{
    SOH_SEQ     Seq;
    uint32         before_Capacity;
    uint32         after_Capacity;
    uint16         befor_SOC;
    uint16         stable_SOC;
}SOH_VAR;

/*---------------------------------------------------------------------*/
/**
* @brief    SOH Calculation task 2, using current compare method
* @param    TRUE - Update, FALSE - X
* @todo     Need to test
* @return   void
*/
extern void SOH_Calculation_Task2(void)
{
#define STABLE_TIME_LMT         (30u * 60u)
#define DETECT_TIME_LMT         (5u-1u)
#define DETECT_CHG_CURR         (sint16)10
#define DETECT_DCH_CURR         (sint16)-10


    static  SOH_VAR  Soh_var;
    static  uint16      Counter1;
    static  uint16      Counter2;

    uint16     delta_SOC;
    uint32     SOC_delta_Capacity;
    uint32     Acc_delta_Capacity;
    bool    fcc_changeFlag;  
	float	Fdata;
	uint32 	u32data;

	uint32 	Capacity;
	uint32	FCC;

	uint16	V_CellAvg;
	sint16	T_CellMin;
	sint16	I_PackMeas;

	Capacity = SOC_Get_Capacity();
	FCC = SOC_Get_FCC();

	V_CellAvg = Adbms_Get_V_CellAvg();
	T_CellMin = Adbms_Get_T_CellMin();
	I_PackMeas = Curr_Get_I_PackMeas();
	
    switch(Soh_var.Seq)
    {
        /* SOH Initialize */
        case SOH_Init:
            /* Undecided : may need to implement a routine to determine if 30 minutes have passed since the last power off */
            Soh_var.Seq = SOH_WaitStable;           
            Soh_var.before_Capacity = Capacity;
            Soh_var.after_Capacity = Capacity;
            Soh_var.befor_SOC = Soc_Calculate_UseOCVTable_SameTemp(V_CellAvg);
            Soh_var.stable_SOC = SOC_Get_SOC_Real();
            Counter1 = 0u;            
            Counter2 = 0u;            
            break;
            
        case SOH_Ready:
            if(I_PackMeas > DETECT_CHG_CURR)
            {
                Counter2 = 0u;
                if(Counter1 >= DETECT_TIME_LMT)
                {
                    Counter1 = 0u;
                    Soh_var.Seq = SOH_Charge;
                }
                else
                {
                   Counter1++; 
                }
            }
            else if(I_PackMeas < DETECT_DCH_CURR)
            {
                Counter1 = 0u;
                if(Counter2 >= DETECT_TIME_LMT)
                {
                    Counter2 = 0u;
                    Soh_var.Seq = SOH_WaitStable;
                }
                else
                {
                   Counter2++; 
                }                
            }
            else
            {
                Counter1 = 0u;
                Counter2 = 0u;
                Soh_var.before_Capacity = Capacity;
                Soh_var.befor_SOC = Soc_Calculate_UseOCVTable_SameTemp(V_CellAvg);
            }            
            break;
            
        case SOH_Charge:
            if(I_PackMeas < DETECT_DCH_CURR)
            {
                Counter2 = 0u;
                if(Counter1 >= DETECT_TIME_LMT)
                {
                    Counter1 = 0;
                    Soh_var.Seq = SOH_WaitStable;
                }
                else
                {
                    Counter1++;
                }
            }
            else if((I_PackMeas <= DETECT_CHG_CURR) && (I_PackMeas >= DETECT_DCH_CURR))
            {
                Counter1 = 0u;
                if(Counter2 >= DETECT_TIME_LMT)
                {
                    Counter2 = 0;
                    Soh_var.Seq = SOH_WaitStable_forCal;
                    Soh_var.after_Capacity = Capacity;
                }
                else
                {
                    Counter2++;
                }                
            }
            else
            {
                if(Counter1 > 0u)
                {
                    Counter1--; 
                }            
                if(Counter2 > 0u)
                {
                    Counter2--; 
                }
            }
            break;
            
        case SOH_WaitStable:
            /* SOH Seq move to SOH_End after wait for 30 minutes */
            if((I_PackMeas <= DETECT_CHG_CURR) && (I_PackMeas >= DETECT_DCH_CURR))
            {
                if(Counter1 >= STABLE_TIME_LMT)
                {
                    Counter1 = 0u;
                    Soh_var.Seq = SOH_End;                     
                }
                else
                {
                    Counter1++;
                }
            }
            else
            {
                if(Counter1 > 0u)
                {
                    Counter1--; 
                }                  
            }            
            break;
            
        case SOH_WaitStable_forCal:
            /* SOH Seq move to SOH_Calculation after wait for 30 minutes */
            if((I_PackMeas <= DETECT_CHG_CURR) && (I_PackMeas >= DETECT_DCH_CURR))
            {
                if(Counter1 >= STABLE_TIME_LMT)
                {
                    Counter1 = 0u;
                    Soh_var.Seq = SOH_Calculation;                     
                }
                else
                {
                    Counter1++;
                }
            }
            else
            {
                if(Counter1 > 0u)
                {
                    Counter1--; 
                }
                
                if(Counter2 > DETECT_TIME_LMT)
                {
                    Counter2 = 0u;
                    Counter1 = 0u;
                    Soh_var.Seq = SOH_WaitStable;                    
                }
                else
                {
                    Counter2++;
                }
            }            
            break;      
            
        case SOH_Calculation:
            /* calculate soc using ocv table */
            Soh_var.stable_SOC = Soc_Calculate_UseOCVTable_SameTemp(V_CellAvg);
            delta_SOC = Soh_var.stable_SOC - Soh_var.befor_SOC;

			u32data = ((uint32)delta_SOC * FCC);
			Fdata = (float)u32data / 1000.0f;
			
            SOC_delta_Capacity = (uint32)Fdata;
            Acc_delta_Capacity = Soh_var.after_Capacity - Soh_var.before_Capacity;		
            
            if(SOC_delta_Capacity > (Acc_delta_Capacity + (FCC / 1000u)))
            {
                FCC -= (SOC_delta_Capacity - Acc_delta_Capacity) / 10u;
				SOC_Set_FCC(FCC);
                fcc_changeFlag = TRUE; 
            }
            else if(SOC_delta_Capacity < (Acc_delta_Capacity - (FCC / 1000u)))
            {
                FCC += (Acc_delta_Capacity - SOC_delta_Capacity) / 10u;
				SOC_Set_FCC(FCC);
                fcc_changeFlag = TRUE;
            }
            else
            {
                fcc_changeFlag = FALSE;
            }

            if(fcc_changeFlag == TRUE)
            {
				Fdata = (float)Soh_var.stable_SOC * (float)FCC / 1000.f;
                Capacity = (uint32)Fdata;
				SOC_Set_Capacity(Capacity);
                Soc_UpdataCapacityData((float)Capacity);
            }

			EepromEx_SetUpdateCapacityData_Flag(TRUE);
			
            Soh_var.Seq = SOH_End;
                
            break;
            
        case SOH_End:
            Soh_var.Seq = SOH_Ready;
            Counter1 = 0u;
            Counter2 = 0u;
            Soh_var.before_Capacity = Capacity;
            Soh_var.befor_SOC = Soc_Calculate_UseOCVTable_SameTemp(V_CellAvg);
			Soh_SetUpdateFlag(TRUE);
            break;
            
        default:
            Soh_var.Seq = SOH_Ready;
            break;
    }

	if((T_CellMin < 200) || (T_CellMin > 450))
	{
		Soh_var.Seq = SOH_WaitStable;
	}
}


/*---------------------------------------------------------------------*/
uint16 SOH_Get_SOH(void)
{
    return SOH;
}


