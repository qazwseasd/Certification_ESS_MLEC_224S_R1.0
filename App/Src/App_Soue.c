/*
 * App_Soue.c
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_Soue.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define USER_SOUE_USING     TRUE

#define SOUE_MAXREAL		(float)100
#define SOUE_MAXUSER		(float)95
#define SOUE_MINUSER		(float)10
#define SOUE_MINREAL		(float)0


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
static	uint8 	SOUE;


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
* @brief    State of usable energe Initialization
* @param    void
* @return   void
*/
extern BOOL Soue_Init(void)
{
	return TRUE;
}

/*---------------------------------------------------------------------*/
/**
* @brief    State of usable energe Calculation task
* @param    void
* @return   void
*/
extern void Soue_Calculation_Task(void)
{
	static uint16  Soue_Value[DEF_NUMBER_OF_MOVING];
	static BOOL	Soue_CalcStart = FALSE;
    static uint8   Soue_cnt = 0u;	
    
    uint8   SOUE_Real;
    uint8   SOUE_User;	
	uint16   Soue_sum = 0u;
	uint16   Soue_buf;
	uint8    i;
	float kWh;	
	float temp;
    float nowCellV;
    float PackV;
	float fdata;
	uint32 capacity;
	uint32 FCC;
	uint16 NumOfCellinModule;
	uint16 NumberOfSeries;
	sint16 I_PackMeas;

	capacity = SOC_Get_Capacity();
	FCC = SOC_Get_FCC();
	NumOfCellinModule = Parameter_Get_NumOfCellinModule();
	NumberOfSeries = Parameter_Get_NumberOfSeries();
	I_PackMeas = Curr_Get_I_PackMeas();

    if((I_PackMeas > 10) || (I_PackMeas < -10))
    {
        nowCellV = Calculate_CellV_useOCVTable(SOC_Get_SOC_Real());
        PackV = nowCellV * (float)NumOfCellinModule * (float)NumberOfSeries;    
        kWh = (float)((float)capacity * PackV);
    }
    else
    {
        kWh = (float)((float)capacity * ((float)PackVolt_Get_V_PackMeas() * (float)0.1 ));
    }

#if 0	
	static uint16  old_pv = 0u;
	static sint16  old_pc = 0u;

	if((Main_PackData.I_PackMeas > 10) || (Main_PackData.I_PackMeas < -10))
	{
		uint16   differ_pv;
		sint16   differ_pc;
		float IR;
	
		differ_pv = (MAX(Main_PackData.V_PackMeas, old_pv) - MIN(Main_PackData.V_PackMeas, old_pv)) / 10u;
		differ_pc = (MAX(Main_PackData.I_PackMeas, old_pc) - MIN(Main_PackData.I_PackMeas, old_pc)) / (sint16)10;
	
		if(differ_pc > 0)
		{
		    IR = (float)((float)differ_pv / (float)differ_pc);
		}
		else
		{
		    IR = 0.;		     
		}

		if(Main_PackData.I_PackMeas > 10)
		{
			kWh = (float)((float)CapacityData.Capacity * ((float)Main_PackData.V_PackMeas * (float)0.1 - (IR * (float)Main_PackData.I_PackMeas * (float)0.1))); 
		}
		else
		{
			kWh = (float)((float)CapacityData.Capacity * ((float)Main_PackData.V_PackMeas * (float)0.1 + (IR * (float)Main_PackData.I_PackMeas * (float)0.1))); 
		}		
	}
	else
	{
		kWh = (float)((float)CapacityData.Capacity * ((float)Main_PackData.V_PackMeas * (float)0.1 ));
	}
	
	old_pc = Main_PackData.I_PackMeas;
	old_pv = Main_PackData.V_PackMeas;
#endif 

	//Soue_buf = (uint16)((float)(kWh * (float)100) / (float)((float)CapacityData.FCC * (float)Main_PackData.Rated_Volt * (float)0.1));
	temp = ((float)(kWh * (float)100) / (float)((float)FCC * (float)AppInit_Get_MaxChgVolt() * (float)0.1));
    Soue_buf = (uint16)temp;
    
	if(!Soue_CalcStart)
	{
		for(i=0; i<DEF_NUMBER_OF_MOVING; i++)
		{
		    Soue_Value[i] = Soue_buf; 
		}
		Soue_CalcStart = TRUE;
	}
	else
	{
		Soue_Value[Soue_cnt] = Soue_buf ; 
		if(Soue_cnt++ >= (DEF_NUMBER_OF_MOVING - 1u))
		{
		    Soue_cnt = 0;
		}
	}
	for(i=0; i<DEF_NUMBER_OF_MOVING; i++)
	{
	    Soue_sum += Soue_Value[i];
	}
	
    SOUE_Real = (uint8)(Soue_sum >> (uint8)DEF_MOVING_AVERAGE_SHIFT);
	
#if (USER_SOUE_USING == TRUE)    
    if      (SOUE_Real <= (uint8)SOUE_MINUSER)
    {
        SOUE_User  = (uint8)SOUE_MINREAL;
    }
    else if (SOUE_Real >= (uint8)SOUE_MAXUSER)
    {
        SOUE_User  = (uint8)SOUE_MAXREAL;
    }
    else
    {
		fdata = ((float)SOUE_Real - SOUE_MINUSER)
                                        * (SOUE_MAXREAL - SOUE_MINREAL) 
                                        / (SOUE_MAXUSER - SOUE_MINUSER)	;
        SOUE_User  = (uint8)fdata;
    }        
    SOUE = SOUE_User;
    
#else    
    if(SOUE_Real > SOUE_MAXREAL)
    {
        SOUE = SOUE_MAXREAL;
    }
    else
    {
        SOUE = SOUE_Real;
    }
#endif 


}


/*---------------------------------------------------------------------*/
uint8 SOUE_Get_SOUE(void)
{
    return SOUE;
}

