/*
 * App_Ir.c
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Ir.h"


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
static uint8 Ir_InitBuf;


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static 	uint16	CV_IR;
static  uint16 	RealCellV;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/*---------------------------------------------------------------------*/
/**
* @brief	IR initialization
* @param	void
* @return	void
*/
extern BOOL Ir_Init(void)
{
	return TRUE;
}

/*---------------------------------------------------------------------*/
/**
* @brief	IR calculation task
* @param	void
* @return	void
*/
extern void Ir_Calculation_Task(void)
{
	static float IR_buf[16];
	static uint8    IR_cnt = 0u;
	static uint16   old_CV = 0u;
	static sint16   old_Current = 0;
	static uint8    IR_Measure_seq = 0u;
	
	uint16     delta_current;
	uint16     delta_voltage;
	float   IR_V;
	float   IR_sum = 0.0f;
	uint8      i;
	float   fdata;	

	uint16 NumOfCellinModule;
	uint16 NumberOfSeries;

	NumOfCellinModule = Parameter_Get_NumOfCellinModule();
	NumberOfSeries = Parameter_Get_NumberOfSeries();

	if((IR_Measure_seq == 0u) || (Ir_InitBuf > 0u))
	{
		Ir_InitBuf = 0u;

		IR_Measure_seq = 1u;

		IR_V = (float)((float)CV_IR / (float)1000.0);
		for(i = 0u; i < 16u; i++)
		{
			IR_buf[i] = IR_V;
		}				
	}
	else
	{
		if(SOC_Get_SOC_Real() > 200u)
		{
			if(Curr_Get_I_PackMeas() >= old_Current)	
			{
				delta_current = (uint16)Curr_Get_I_PackMeas() - (uint16)old_Current;
			}
			else									
			{
				delta_current = (uint16)old_Current - (uint16)Curr_Get_I_PackMeas();
			}

			if(delta_current >= 10u)
			{
				if(PackVolt_Get_V_PackMeas() >= old_CV)	
				{
					delta_voltage = PackVolt_Get_V_PackMeas() - old_CV;
				}
				else									
				{
					delta_voltage = old_CV - PackVolt_Get_V_PackMeas();
				}
				
				IR_V = (float)delta_voltage / (float)delta_current;		
				
				IR_buf[IR_cnt] = IR_V;
				IR_cnt++;
				if(IR_cnt >= 16u)	
				{
					IR_cnt = 0u;	
				}
			}
		}
	}	
	
	for(i = 0u; i < 16u; i++) 
	{
		IR_sum += IR_buf[i];
	}
	IR_V = IR_sum / 16.0f;
	CV_IR = (uint16)IR_V * 1000u;

	fdata = (float)PackVolt_Get_V_PackMeas() - ((float)Curr_Get_I_PackMeas() * IR_V) / ((float)0.01 * (float)NumOfCellinModule * (float)NumberOfSeries);
	RealCellV = (uint16)fdata;	
	
	old_Current = Curr_Get_I_PackMeas();
	old_CV = PackVolt_Get_V_PackMeas();			
}


/*---------------------------------------------------------------------*/
/**
* @brief	IR buffer setting flag
* @param	uint8 - set flag
* @return	void
*/
extern void Ir_SetIRInitBuf(uint8 ir_initbuf)
{
	Ir_InitBuf = ir_initbuf;
}


/*---------------------------------------------------------------------*/
uint16 IR_Get_CV_IR(void)
{
    return CV_IR;
}


/*---------------------------------------------------------------------*/
void IR_Set_CV_IR(uint16 IR)
{
    CV_IR = IR;
}


/*---------------------------------------------------------------------*/
uint16 IR_Get_RealCellV(void)
{
    return RealCellV;
}



