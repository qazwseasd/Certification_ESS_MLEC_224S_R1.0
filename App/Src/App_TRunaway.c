/*
 * App_TRunaway.c
 *
 *  Created on: Oct 23, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_TRunaway.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define DetectTR_TCellDevNum	3u
#define DetectTR_TCellMaxNum	3u
#define DetectTR_TCellNum		3u
#define DetectTR_VCellNum		5u
#define DetectTR_IsolationRNum	20u
#define DetectTR_LineOff		5u


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
static uint8		DetectTR_TCellDevCnt	= 0u;
static uint8		DetectTR_TCellMaxCnt	= 0u;
static uint8		DetectTR_IsolationR		= 0u;
static uint8		DetectTR_VCellOffLine	= 0u;
static uint8		DetectTR_TCellOffLine	= 0u;
static uint8		DetectTR_BMICOffLine	= 0u;


static sint16		DetectTR_TCellPrev[DEF_MAX_NUM_OF_BMIC][DEF_MAX_NUM_OF_TEMP_PER_BMIC];	/* [0.1 ℃] */
static uint8		DetectTR_TCellCnt [DEF_MAX_NUM_OF_BMIC][DEF_MAX_NUM_OF_TEMP_PER_BMIC];

static uint16		DetectTR_VCellMin[DetectTR_VCellNum];	/* [1 mV] */
static uint8		DetectTR_VCellCnt	= 0u;

static  uint8		Trunaway_V_BMICNum;
static  uint8		Trunaway_V_CellNum;
static	uint8		Trunaway_T_BMICNum;
static	uint8		Trunaway_T_CellNum;


bool	DetectTR_Condition_1;	/* (TCellmax - Taverage) >= 35℃ */
bool	DetectTR_Condition_2;	/* (TCellMax) ≥ 60 [℃] */
bool	DetectTR_Condition_3;	/* (TCellRiseRate) > 1 [℃/s], lasting 3s */
bool	DetectTR_Condition_4;	/* (VCellDropMax) > 1.213 [V] in 5s */
bool	DetectTR_Condition_5;	/* (RIso) ≤ (100 [Ω/V] + margin 50kΩ */
bool	DetectTR_Condition_6;	/* (VCellmin) <= 1.7V */
bool	DetectTR_Condition_7;	/* (VCellSensing OFF) */
bool	DetectTR_Condition_8;	/* (TCellSensing OFF) */
bool	DetectTR_Condition_9;	/* (BMIC ERROR) */
bool	DetectTR_ThermalRunaway	= FALSE;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
extern	void	DetectTR_Task(void)
{
	uint8	i;
	uint8	j;
	
	uint16	VCellDrop;	/* [0.1 mV] */
	
	static	bool	Measured_TCell	= FALSE;
	
	static	bool	State1 = FALSE;
	static	bool	State2 = FALSE;

	static	uint8	Count = 0u;

	uint16	V_CellMin;
	uint8	V_CellCellNoMin;
	uint8	V_CellModNoMin;
	sint16	T_CellMax;
	uint8	T_CellCellNoMax;
	uint8	T_CellModNoMax;
	uint16	NumOf_Bmic;
	uint16  NumOfTempinModule;

	V_CellMin = Adbms_Get_V_CellMin();
	V_CellCellNoMin = Adbms_Get_V_CellCellNoMin();
	V_CellModNoMin = Adbms_Get_V_CellModNoMin();
	T_CellMax = Adbms_Get_T_CellMax();
	T_CellCellNoMax = Adbms_Get_T_CellCellNoMax();
	T_CellModNoMax = Adbms_Get_T_CellModNoMax();
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfTempinModule = Parameter_Get_NumOfTempinModule();

	if(ManaMsm_Get_System_State() != SYS_INIT)
	{

		if(Count <= 5u)
		{
			Count++;
		}

		else
		{
			/* TCellmax - TCellaverage >= 35℃ */
			if (Adbms_Get_T_CellDev() >= 350)
			{
				DetectTR_TCellDevCnt++;
				if(DetectTR_TCellDevCnt >= DetectTR_TCellDevNum)
				{
					Trunaway_T_BMICNum = T_CellModNoMax;
					Trunaway_T_CellNum = T_CellCellNoMax;
					DetectTR_Condition_1 = TRUE;
				}
			}
			else
			{
				DetectTR_TCellDevCnt = 0u;
				DetectTR_Condition_1 = FALSE;
			}


			/* TCellmax ≥ 60℃ */
			if (T_CellMax >= 600)
			{
				DetectTR_TCellMaxCnt++;
				if(DetectTR_TCellMaxCnt >= DetectTR_TCellMaxNum)
				{
					Trunaway_T_BMICNum = T_CellModNoMax;
					Trunaway_T_CellNum = T_CellCellNoMax;
					DetectTR_Condition_2 = TRUE;
				}
			}
			else
			{
				DetectTR_TCellMaxCnt = 0u;
				DetectTR_Condition_2	= FALSE;
			}
			
			
			/* dT/dt ≥ 1℃/s (3s) */
			if (Measured_TCell == FALSE)
			{
				Measured_TCell	= TRUE;
				for (i=0u; i< NumOf_Bmic; i++)
				{
					for (j=0u; j <NumOfTempinModule; j++)
					{
		 				DetectTR_TCellPrev[i][j]	= Adbms_Get_CellTemp(i, j);
		 				DetectTR_TCellCnt [i][j]	= 0u;
					}
				}
				DetectTR_Condition_3	= FALSE;
			}
			else
			{
				for (i=0u; i< NumOf_Bmic; i++)
				{
					for (j=0u; j< NumOfTempinModule; j++)
					{
						if ( Adbms_Get_CellTemp(i, j) > (DetectTR_TCellPrev[i][j] + 10) )
						{
							DetectTR_TCellCnt[i][j]++;
							if (DetectTR_TCellCnt[i][j] >= DetectTR_TCellNum)
							{
								DetectTR_TCellCnt[i][j]	= DetectTR_TCellNum;
								DetectTR_Condition_3	= TRUE;
								
								if(State1 == FALSE)
								{
									State1 = TRUE;
									Trunaway_T_BMICNum = i;
									Trunaway_T_CellNum = j;
								}
							}
							else{}
						}
						else
						{
							if (DetectTR_TCellCnt[i][j] > 0u)
							{
								DetectTR_TCellCnt[i][j]--;
							}
							else{}
						}
						DetectTR_TCellPrev[i][j]	= Adbms_Get_CellTemp(i, j);
					}
				}
			}
			
			
			/* voltage variation of the cells >= 1.213V */
			if (DetectTR_VCellCnt < DetectTR_VCellNum)
			{
				DetectTR_VCellMin[DetectTR_VCellCnt]	= V_CellMin;
				DetectTR_VCellCnt++;
				DetectTR_Condition_4	= FALSE;
			}
			else
			{
				if (DetectTR_VCellMin[0u] > V_CellMin)
				{
					VCellDrop	= DetectTR_VCellMin[0u] - V_CellMin;
				}
				else
				{
					VCellDrop	= 0u;
				}
				(void)memmove(DetectTR_VCellMin, &(DetectTR_VCellMin[1u]), 2u * (DetectTR_VCellNum - 1u));	/* (Destination, Source, Size) */
				DetectTR_VCellMin[DetectTR_VCellNum - 1u]	= V_CellMin;

				if (VCellDrop > 1213u)
				{
					if(State2 == FALSE)
					{
						State2 = TRUE;
						Trunaway_V_BMICNum = V_CellModNoMin;
						Trunaway_V_CellNum = V_CellCellNoMin;
					}
					DetectTR_Condition_4	= TRUE;
				}
				else
				{

				}
			}

			/* isolation resistance <= 100Ω/V */
			if (IsoR_IsMeasurement() == TRUE)
			{
				if (IsoR_Get_ISO_Resistor() <= 200u)
				{
					DetectTR_IsolationR++;
					if(DetectTR_IsolationR >= DetectTR_IsolationRNum)
					{
						DetectTR_Condition_5	= TRUE;
					}
				}
				else
				{
					DetectTR_IsolationR = 0u;
					DetectTR_Condition_5	= FALSE;
				}
			}
			else
			{
				DetectTR_Condition_5	= FALSE;
			}


			/* (VCellmin) <= 1.7V */
			if (V_CellMin <= 1700u)
			{
				Trunaway_V_BMICNum = V_CellModNoMin;
				Trunaway_V_CellNum = V_CellCellNoMin;
				DetectTR_Condition_6 	= TRUE;
			}
			else
			{
				DetectTR_Condition_6	= FALSE;
			}


			/* VCell Line OFF */
			if (Get_TROfflineVCell() == TRUE)
			{
				DetectTR_VCellOffLine++;
				if(DetectTR_VCellOffLine >= DetectTR_LineOff)
				{
					DetectTR_Condition_7 	= TRUE;
				}
			}
			else
			{
				DetectTR_VCellOffLine = 0u;
				DetectTR_Condition_7	= FALSE;
			}


			/* TCell Line OFF */
			if (Get_TROfflineTCell() == TRUE)
			{
				DetectTR_TCellOffLine++;
				if(DetectTR_TCellOffLine >= DetectTR_LineOff)
				{
					DetectTR_Condition_8 	= TRUE;
				}
			}
			else
			{
				DetectTR_TCellOffLine = 0u;
				DetectTR_Condition_8	= FALSE;
			}


			/* BMIC Line OFF */
			if (Get_TROfflineBMIC() == TRUE)
			{
				DetectTR_BMICOffLine++;
				if(DetectTR_BMICOffLine >= DetectTR_LineOff)
				{
					DetectTR_Condition_9	= TRUE;
				}
			}
			else
			{
				DetectTR_BMICOffLine = 0u;
				DetectTR_Condition_9	= FALSE;
			}
			
			

			/* Determine if the thermal runaway is detected */
			if( ((DetectTR_Condition_1 == TRUE) || (DetectTR_Condition_2 == TRUE) || (DetectTR_Condition_3 == TRUE) 
				|| (DetectTR_Condition_8 == TRUE)) && (DetectTR_Condition_6 == TRUE) )
			{
				DetectTR_ThermalRunaway	= TRUE;
			}

			if( ((DetectTR_Condition_1 == TRUE) || (DetectTR_Condition_2 == TRUE) || (DetectTR_Condition_3 == TRUE) 
				|| (DetectTR_Condition_6 == TRUE)) && (DetectTR_Condition_5 == TRUE) )
			{
				DetectTR_ThermalRunaway	= TRUE;
			}

			if(  ((DetectTR_Condition_7 == TRUE) || (DetectTR_Condition_9 == TRUE)) && ((DetectTR_Condition_1 == TRUE) 
				|| (DetectTR_Condition_2 == TRUE) || (DetectTR_Condition_3 == TRUE)) )
			{
				DetectTR_ThermalRunaway	= TRUE;
			}

			if( (DetectTR_Condition_2 == TRUE) && (DetectTR_Condition_3 == TRUE) )
			{
				DetectTR_ThermalRunaway	= TRUE;
			}

			if( (DetectTR_Condition_2 == TRUE) && (DetectTR_Condition_4 == TRUE) && (DetectTR_Condition_5 == TRUE) )
			{
				DetectTR_ThermalRunaway	= TRUE;
			}

			else{}
		}

	}
}


/*---------------------------------------------------------------------*/
uint8 TRunaway_Get_Trunaway_V_BMICNum(void)
{
    return Trunaway_V_BMICNum;
}


/*---------------------------------------------------------------------*/
uint8 TRunaway_Get_Trunaway_V_CellNum(void)
{
    return Trunaway_V_CellNum;
}


/*---------------------------------------------------------------------*/
uint8 TRunaway_Get_Trunaway_T_BMICNum(void)
{
    return Trunaway_T_BMICNum;
}


/*---------------------------------------------------------------------*/
uint8 TRunaway_Get_Trunaway_T_CellNum(void)
{
    return Trunaway_T_CellNum;
}


