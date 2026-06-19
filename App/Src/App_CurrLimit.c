/*
 * App_CurrLimit.c
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include <App_CurrLimit.h>


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define CURRLIMIT_DETECT_END_CURR		90u										/* [0.1 A] */
#define CURRLIMIT_DETECT_END_TIME		(3000u / CURRLIMIT_CALCULATION_PERIOD)	/* 3000 [ms] */

#define CURRLIMIT_SIZE_TEMP_AXIS		13u
#define CURRLIMIT_SIZE_SOC_AXIS			8u

#define CURRLIMIT_MIN_CURR				(float)0.01	                            /* [A] */
#define CURRLIMIT_MAX_REMAIN_CHG_TIME	3600u	                                /* [minute] */

#define CURRLIMIT_BY_CELL_V_PERIOD		1000u
#define CURRLIMIT_BY_CELL_V_CNT			((CURRLIMIT_BY_CELL_V_PERIOD \
										/ CURRLIMIT_CALCULATION_PERIOD) - 1u)
										
#define CURRLIMIT_SOC_STOP_CHG			950u		                            /* [0.1 %] */
#define CURRLIMIT_SOC_STOP_DCH			100u		                            /* [0.1 %] */

#define CURRLIMIT_TIME_PEAK_LIMIT		150u		                            /* [0.2 sec] */
#define CURRLIMIT_TIME_PEAK_REST		10u                             		/* [0.2 sec] */


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/
typedef struct
{
	/* Input */
	uint16		SOC;							/* [%] */
	float	Temp;							/* [0.1 ℃] */
	uint8		SocStopChg;						/* [%] */
	float	IPack;							/* [A] */
	float	Fcc1Per;						/* [Ah] */

	/* Output */
	float	RemainTime;						/* [minute] */

	/* Internal */
	float	TableByTemp[CURRLIMIT_SIZE_SOC_AXIS];	/* [A] */
}handle_CurrLimit_RemainChgTime;


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/
/**
* @brief    Axis data of Temperature (When Charging) 
*/
static const float	CurrLimit_AxisTempChg[CURRLIMIT_SIZE_TEMP_AXIS]	=
{
	-310.f,	-300.f,	-210.f,	-200.f,	-150.f,	-100.f,	0.f,	100.f,	200.f,	300.f,	400.f,	500.f,	550.f		/* [0.1 ℃] */
};

/**
* @brief    Axis data of SOC (When Charging) 
*/
static const float	CurrLimit_AxisSocChg[CURRLIMIT_SIZE_SOC_AXIS]	=
{
	0.f,		700.f,	750.f,	800.f,	850.f,	900.f,	950.f,	1000.f	/* [0.1 %] */
};

/**
* @brief	continuous Current limit table (When Charging) 
*/
/* 4S 3P*/
static const float	CurrLimit_ChgTable_Conti[CURRLIMIT_SIZE_TEMP_AXIS][CURRLIMIT_SIZE_SOC_AXIS] =
{ /* 0		70		75		80		85		90		95		100 [%] */
	{0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	/* -31 [℃] */ 
	{0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	/* -30 [℃] */
	{0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	/* -21 [℃] */ 
	{45.f,	21.f,	15.f,	9.f,	9.f,	9.f,	9.f,	0.f},	/* -20 [℃] */ 
	{72.f,	31.f,	24.f,	17.f,	13.f,	9.f,	9.f,	0.f},	/* -15 [℃] */
	{69.f,	39.f,	30.f,	21.f,	15.f,	9.f,	9.f,	0.f},	/* -10 [℃] */
	{90.f,	69.f,	54.f,	39.f,	28.f,	18.f,	9.f,	0.f},	/*	 0 [℃] */
	{90.f,	90.f,	85.f,	60.f,	44.f,	27.f,	14.f,	0.f},	/*	10 [℃] */
	{90.f,	90.f,	85.f,	81.f,	59.f,	36.f,	18.f,	0.f},	/*	20 [℃] */ 	
	{90.f,	90.f,	90.f,	90.f,	66.f,	42.f,	21.f,	0.f},	/*	30 [℃] */
	{90.f,	90.f,	90.f,	90.f,	66.f,	42.f,	21.f,	0.f},	/*	40 [℃] */
	{45.f,	45.f,	45.f,	45.f,	33.f,	21.f,	11.f,	0.f},	/*	50 [℃] */
	{0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f}	/*	55 [℃] */
};	/* [A] */


static handle_CurrLimit_RemainChgTime	hCurrLimit_RemainChgTime_Proc;

static BOOL	CurrLimit_FullyChgFlag	= FALSE;
static BOOL	CurrLimit_FullyDchFlag	= FALSE;


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static 	sint16 	ChgCurrLimitConti;
static 	sint16 	DchCurrLimitConti;
static 	sint16 	ChgCurrLimitPeak;
static 	sint16 	DchCurrLimitPeak;
static 	sint16 	ChgCurrLimit;
static 	sint16 	DchCurrLimit;
// static  uint16  RemainChgTime;



/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
void CurrLimit_CalcRemainChgTime_Init(handle_CurrLimit_RemainChgTime* Handle);
void CurrLimit_CalcCurrLimit(void);
uint16	 CurrLimit_InterpolateCurrLimit(const float CurrLmtTable[][CURRLIMIT_SIZE_SOC_AXIS],
									const float AxisTemp[], const float AxisSoc[],
									float nowTemp, uint16 nowSOC);
sint16	 CurrLimit_SelectPeakNormalChg(void);
sint16	 CurrLimit_SelectPeakNormalDch(void);
void CurrLimit_CalcRemainChgTime_Proc(handle_CurrLimit_RemainChgTime* Handle);
void CurrLimit_CalcRemainChgTime(handle_CurrLimit_RemainChgTime* Handle);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
* @brief	Current limit Initialization function
* @param	void
* @return	void
*/
void CurrLimit_Init(void)
{
	CurrLimit_CalcRemainChgTime_Init(&hCurrLimit_RemainChgTime_Proc);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Initialization function of Calculating remaining charge time
* @param	*Handle - handle value
* @return	void
*/
void CurrLimit_CalcRemainChgTime_Init(handle_CurrLimit_RemainChgTime* Handle)
{
	Handle->SocStopChg	= (uint8)(CURRLIMIT_SOC_STOP_CHG / 10u);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Calculaing Currentlimit Task function
* @param	void
* @return	void
*/
void CurrLimit_Calculation_Task(void)
{
	CurrLimit_CalcCurrLimit();
	CurrLimit_CalcRemainChgTime_Proc(&hCurrLimit_RemainChgTime_Proc);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Calculaton of Currentlimit 
* @param	void
* @return	void
*/
void CurrLimit_CalcCurrLimit(void)
{

 /**
 * @brief	Peak Current limit table (When Charging) 
 */    
 /* 4S 3P*/	
    static const float CurrLimit_ChgTable_Peak[CURRLIMIT_SIZE_TEMP_AXIS][CURRLIMIT_SIZE_SOC_AXIS] =
	{ /* 0		70		75		80		85		90		95		100 [%] */
		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	/* -31 [℃] */ 
		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	/* -30 [℃] */
		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	/* -21 [℃] */ 
		{45.f,	21.f,	15.f,	9.f,	9.f,	9.f,	9.f,	0.f},	/* -20 [℃] */ 
		{72.f,	31.f,	24.f,	17.f,	13.f,	9.f,	9.f,	0.f},	/* -15 [℃] */
		{69.f,	39.f,	30.f,	21.f,	15.f,	9.f,	9.f,	0.f},	/* -10 [℃] */
		{90.f,	69.f,	54.f,	39.f,	28.f,	18.f,	9.f,	0.f},	/*	 0 [℃] */
		{90.f,	90.f,	85.f,	60.f,	44.f,	27.f,	14.f,	0.f},	/*	10 [℃] */
		{90.f,	90.f,	85.f,	81.f,	59.f,	36.f,	18.f,	0.f},	/*	20 [℃] */ 	
		{90.f,	90.f,	90.f,	90.f,	66.f,	42.f,	21.f,	0.f},	/*	30 [℃] */
		{90.f,	90.f,	90.f,	90.f,	66.f,	42.f,	21.f,	0.f},	/*	40 [℃] */
		{45.f,	45.f,	45.f,	45.f,	33.f,	21.f,	11.f,	0.f},	/*	50 [℃] */
		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f}	/*	55 [℃] */
	};	/* [A] */


  /**
  * @brief	 Peak Current limit table (When Discharging) 
  */   
  /* 4S 3P*/	
    static const float CurrLimit_DchTable_Peak[CURRLIMIT_SIZE_TEMP_AXIS][CURRLIMIT_SIZE_SOC_AXIS] =
    { /* 0      5       10      15      20      25      30      100 [%] */
		{0.f,   0.f,    0.f,    0.f,    0.f,    0.f,    0.f,    0.f},     /* -31 [℃] */
		{0.f,   0.f,    0.f,	0.f,  	0.f,   	0.f,   	0.f,   	0.f},     /* -30 [℃] */
		{0.f,   0.f,    0.f,	0.f,  	0.f,   	0.f,   	0.f,   	0.f},     /* -21 [℃] */
        {0.f,   9.f,    9.f,	11.f,   21.f,   29.f,   36.f,   78.f},    /* -20 [℃] */
        {0.f,   9.f,    9.f,   	19.f,   36.f,   44.f,   53.f,   84.f},    /* -15 [℃] */
        {0.f,   9.f,    9.f,   	27.f,   51.f,   60.f,   69.f,   90.f},    /* -10 [℃] */
        {0.f,   9.f,   	12.f,   51.f,   90.f,   90.f,   90.f,   90.f},    /*   0 [℃] */
        {0.f,   45.f,   90.f,  	90.f,   90.f,   90.f,   90.f,   90.f},    /*  10 [℃] */
        {0.f,   45.f,   90.f,   90.f,   90.f,   90.f,   90.f,   90.f},    /*  20 [℃] */
        {0.f,   45.f,   90.f,   90.f,   90.f,   90.f,   90.f,   90.f},    /*  30 [℃] */
        {0.f,   45.f,   90.f,   90.f,   90.f,   90.f,   90.f,   90.f},    /*  40 [℃] */
        {0.f,   45.f,   45.f,   45.f,   45.f,   45.f,   45.f,   45.f},    /*  50 [℃] */
        {0.f,   0.f,    0.f,    0.f,    0.f,    0.f,    0.f,    0.f},     /*  55 [℃] */
    };  /* [A] */


  /**
 * @brief	Continuous Current limit table (When Discharging) 
 */   
 /* 4S 3P*/
    static const float CurrLimit_DchTable_Conti[CURRLIMIT_SIZE_TEMP_AXIS][CURRLIMIT_SIZE_SOC_AXIS] =
    { /* 0      5       10      15      20      25      30      100 [%] */
		{0.f,   0.f,    0.f,    0.f,    0.f,    0.f,    0.f,    0.f},     /* -31 [℃] */
		{0.f,   0.f,    0.f,	0.f,  	0.f,   	0.f,   	0.f,   	0.f},     /* -30 [℃] */
		{0.f,   0.f,    0.f,	0.f,  	0.f,   	0.f,   	0.f,   	0.f},     /* -21 [℃] */
        {0.f,   9.f,    9.f,	11.f,   21.f,   29.f,   36.f,   78.f},    /* -20 [℃] */
        {0.f,   9.f,    9.f,   	19.f,   36.f,   44.f,   53.f,   84.f},    /* -15 [℃] */
        {0.f,   9.f,    9.f,   	27.f,   51.f,   60.f,   69.f,   90.f},    /* -10 [℃] */
        {0.f,   9.f,   	12.f,   51.f,   90.f,   90.f,   90.f,   90.f},    /*   0 [℃] */
        {0.f,   45.f,   90.f,  	90.f,   90.f,   90.f,   90.f,   90.f},    /*  10 [℃] */
        {0.f,   45.f,   90.f,   90.f,   90.f,   90.f,   90.f,   90.f},    /*  20 [℃] */
        {0.f,   45.f,   90.f,   90.f,   90.f,   90.f,   90.f,   90.f},    /*  30 [℃] */
        {0.f,   45.f,   90.f,   90.f,   90.f,   90.f,   90.f,   90.f},    /*  40 [℃] */
        {0.f,   45.f,   45.f,   45.f,   45.f,   45.f,   45.f,   45.f},    /*  50 [℃] */
        {0.f,   0.f,    0.f,    0.f,    0.f,    0.f,    0.f,    0.f},     /*  55 [℃] */
    };  /* [A] */


	/**
	* @brief    Axis data of Temperature (When Discharging) 
	*/
    const float  CurrLimit_AxisTempDch[CURRLIMIT_SIZE_TEMP_AXIS] =
    {
        -310.f,	-300.f,	-210.f,	-200.f,	-150.f,	-100.f,	0.f,	100.f,	200.f,	300.f,	400.f,	500.f,	550.f     /* [0.1 ℃] */
    };

	/**
	* @brief	Axis data of SOC (When Discharging) 
	*/
    const float  CurrLimit_AxisSocDch[CURRLIMIT_SIZE_SOC_AXIS]   =
    {
        0.f,	50.f,		100.f,	150.f,	200.f,	250.f,	300.f,    1000.f    /* [0.1 %] */
    };


	sint16			tempo_s16_1;
	sint16			tempo_s16_2;
	sint16			Soc_ChgLimit	= 0;	/* [0.1 A] */
	sint16			Soc_DchLimit	= 0;	/* [0.1 A] */
	sint16			Peak_ChgLimit	= 0;	/* [0.1 A] */
	sint16			Peak_DchLimit	= 0;	/* [0.1 A] */
	static uint16	DetectCnt		= 0;
	static uint8	CellV_Calc_Cnt	= 0;
	static uint16	CurrBuf			= 0;	/* [0.1 A] */
    static sint16  CellV_ChgLimit  = 2700;  /* [0.1 A] */   /* Cell : 4S3P E60A */
    static sint16  CellV_DchLimit  = 3600; /* [0.1 A] */   /* Cell : 4S3P E60A */
	static BOOL	CellV_ChgCalc	= FALSE;
	static BOOL	CellV_DchCalc	= FALSE;
	static BOOL	ChargeProhibit	= FALSE;
	static BOOL	ChargeComplete	= FALSE;

	sint16         sbuf;

	uint16	V_CellMax;
	uint16	V_CellMin;
	sint16	T_CellMax;
	sint16	T_CellMin;
	uint16	SOC_Real;
	uint16 	OCV_Fault;
	uint16  UCV_Fault;
	sint16 COC_Warning;
	sint16 DOC_Warning;
	sint16 I_PackMeas;
	SYSTEM_STATE System_State;
	
	V_CellMax = Adbms_Get_V_CellMax();
	V_CellMin = Adbms_Get_V_CellMin();
	T_CellMax = Adbms_Get_T_CellMax();
	T_CellMin = Adbms_Get_T_CellMin();
	SOC_Real = SOC_Get_SOC_Real();
	OCV_Fault = Parameter_Get_OCV_Fault();
	UCV_Fault = Parameter_Get_UCV_Fault();
	COC_Warning = Parameter_Get_COC_Warning();
	DOC_Warning = Parameter_Get_DOC_Warning();
	I_PackMeas = Curr_Get_I_PackMeas();
	System_State = ManaMsm_Get_System_State();
	
#if 0	// MBMS Scope -HMJ-
	static uint8	DecreasePercent	= 0;	/* [%] */
#endif

	/* Calculation by the SOC */
	/* To detect charge complete */
	if ( (V_CellMax	>= OCV_Fault)
	  && (I_PackMeas	<= (sint16)CURRLIMIT_DETECT_END_CURR) )
	{
		DetectCnt++;
		if (DetectCnt++ >= CURRLIMIT_DETECT_END_TIME)
		{
			/* Fully charged */
			Soc_ChgLimit			= 0;
			ChargeComplete			= TRUE;
			CurrLimit_FullyChgFlag	= TRUE;
		}
	}
	else
	{
		/* Charging in progress */
		DetectCnt		= 0u;
		tempo_s16_1	= (sint16)CurrLimit_InterpolateCurrLimit(	CurrLimit_ChgTable_Conti,
														CurrLimit_AxisTempChg, CurrLimit_AxisSocChg,
														(float)T_CellMin, SOC_Real);
		tempo_s16_2	= (sint16)CurrLimit_InterpolateCurrLimit(	CurrLimit_ChgTable_Conti,
														CurrLimit_AxisTempChg, CurrLimit_AxisSocChg,
														(float)T_CellMax, SOC_Real);
		if (tempo_s16_1 < tempo_s16_2)
		{
		    Soc_ChgLimit	= tempo_s16_1;
		}
		else
		{
		    Soc_ChgLimit	= tempo_s16_2;
		}
		tempo_s16_1	= (sint16)CurrLimit_InterpolateCurrLimit(	CurrLimit_ChgTable_Peak,
														CurrLimit_AxisTempChg, CurrLimit_AxisSocChg,
														(float)T_CellMin, SOC_Real);
		tempo_s16_2	= (sint16)CurrLimit_InterpolateCurrLimit(	CurrLimit_ChgTable_Peak,
														CurrLimit_AxisTempChg, CurrLimit_AxisSocChg,
														(float)T_CellMax, SOC_Real);
		if (tempo_s16_1 < tempo_s16_2)
		{
		    Peak_ChgLimit	= tempo_s16_1;
		}
		else
		{
		    Peak_ChgLimit	= tempo_s16_2;
		}
	}
	
	/* To detect discharge complete */
	if ( (V_CellMin	<= UCV_Fault)
	  && (I_PackMeas	>= ((sint16)CURRLIMIT_DETECT_END_CURR * (-1))) )
	{
		DetectCnt++;
		if (DetectCnt++ >= CURRLIMIT_DETECT_END_TIME)
		{
			/* Fully discharged */
			Soc_DchLimit	= 0;
			CurrLimit_FullyDchFlag	= TRUE;
		}
	}
	else
	{
		/* Discharging in progress */
		DetectCnt		= 0;
		tempo_s16_1	= (sint16)CurrLimit_InterpolateCurrLimit(	CurrLimit_DchTable_Conti,
														CurrLimit_AxisTempDch, CurrLimit_AxisSocDch,
														(float)T_CellMin, SOC_Real);
		tempo_s16_2	= (sint16)CurrLimit_InterpolateCurrLimit(	CurrLimit_DchTable_Conti,
														CurrLimit_AxisTempDch, CurrLimit_AxisSocDch,
														(float)T_CellMax, SOC_Real);
		if (tempo_s16_1 < tempo_s16_2)
		{
            Soc_DchLimit	= tempo_s16_1;
        }
		else
		{
            Soc_DchLimit	= tempo_s16_2;
        }
		tempo_s16_1	= (sint16)CurrLimit_InterpolateCurrLimit(	CurrLimit_DchTable_Peak,
														CurrLimit_AxisTempDch, CurrLimit_AxisSocDch,
														(float)T_CellMin, SOC_Real);
		tempo_s16_2	= (sint16)CurrLimit_InterpolateCurrLimit(	CurrLimit_DchTable_Peak, 
														CurrLimit_AxisTempDch, CurrLimit_AxisSocDch,
														(float)T_CellMax, SOC_Real);
		if (tempo_s16_1 < tempo_s16_2)
		{
		    Peak_DchLimit	= tempo_s16_1;
		}
		else
		{
			Peak_DchLimit	= tempo_s16_2;
	    }
	}
 
	/* Calculation by the cell voltage */
	if (CellV_Calc_Cnt++ >= CURRLIMIT_BY_CELL_V_CNT)
	{
		CellV_Calc_Cnt	= 0u;
		if (Curr_Get_I_PackMeas() >= 10)
		{
			/* To decrease the charge current limit by the cell voltage */
			if (CellV_ChgCalc == FALSE)
			{
				if (V_CellMax >= OCV_Fault)
				{
					sbuf			= Curr_Get_I_PackMeas() - 5;
					CurrBuf			= (uint16)sbuf;
					CellV_ChgLimit	= (sint16)CurrBuf;
					CellV_ChgCalc	= TRUE;
				}
			}
			else if (CellV_ChgCalc == TRUE)
			{
				if (V_CellMax >= OCV_Fault)
				{
					if (CurrBuf >= 10u)
					{
					    CurrBuf	-= 5u;
				    }
					CellV_ChgLimit	= (sint16)CurrBuf;
				}
			}
			else
			{}
			CellV_DchCalc	= FALSE;
		}
		else if (Curr_Get_I_PackMeas() <= -10)
		{
			/* To decrease the discharge current limit by the cell voltage */
			if (CellV_DchCalc == FALSE)
			{
				if (V_CellMin <= UCV_Fault)
				{
				    sbuf            = (Curr_Get_I_PackMeas() * (-1)) - 5;
					CurrBuf			= (uint16)sbuf;
					CellV_DchLimit	= (sint16)CurrBuf;
					CellV_DchCalc	= TRUE;
				}
			}
			else
			{
				if (V_CellMin <= UCV_Fault)
				{
					if (CurrBuf >= 10u)
					{
					    CurrBuf	-= 5u;
					}					 
					CellV_DchLimit = (sint16)CurrBuf;
				}            
			}
			CellV_ChgCalc	= FALSE;
		}
		else
		{
			CellV_ChgCalc	= FALSE;
			CellV_DchCalc	= FALSE;
			CellV_ChgLimit	= COC_Warning;
			CellV_DchLimit	= DOC_Warning;
		}
	}    

	if (CellV_ChgLimit < 20)
	{
		CellV_ChgLimit			= 0;
		ChargeComplete			= TRUE;
		CurrLimit_FullyChgFlag	= TRUE;
	}    

	if (CellV_DchLimit < 20)
	{
		CellV_DchLimit	= 0;
	}

	/* To decide the current limit */
	if ( (System_State == SYS_DRIVE)
	  || (System_State == SYS_DETECTING_FCC) )
	{
		ChgCurrLimitConti	= MIN(Soc_ChgLimit, CellV_ChgLimit);
		if (ChgCurrLimitConti < 0)
		{
		    ChgCurrLimitConti	= 0;
		}
		DchCurrLimitConti	= MIN(Soc_DchLimit, CellV_DchLimit);
		if (DchCurrLimitConti < 0)
		{
		    DchCurrLimitConti	= 0;
		}		
        ChgCurrLimitPeak	= Peak_ChgLimit;
        DchCurrLimitPeak	= Peak_DchLimit;

		if (ChargeComplete == TRUE)
		{
			if ( ( SOC_Real <= (SOC_MAX_SOC - (SOC_MAX_SOC / 50u)) )	/* 98 % */
			  && (g_b_Discharge_flag == TRUE) )
			{
				ChargeComplete	= FALSE;
				ChargeProhibit	= FALSE;
			}
			
			if (SOC_Real >= SOC_MAX_SOC)
			{
			    ChgCurrLimitConti	= 0;
			    ChargeProhibit				= TRUE;
			}
			else
			{
			    if (ChargeProhibit == TRUE)
				{
				    ChgCurrLimitConti	= 0;
				}
			    else
				{
				    ChgCurrLimitConti	= 20;
				}
			}
		}
	}
	else
	{
		ChgCurrLimitConti	= 0;
		DchCurrLimitConti	= 0;
		ChgCurrLimitPeak	= 0;
		DchCurrLimitPeak	= 0;
	}

	/* Decreasing by warning flags */
#if 0	// MBMS Scope -HMJ-
	if (Main_PackData.FlagsWarn != 0)
	{
		DecreasePercent	+= 2u;
		if (DecreasePercent >= 50u)
		{
            DecreasePercent	=  50u;
		}
		Main_PackData.ChgCurrLimitPeak	= (sint16)( (uint32)Main_PackData.ChgCurrLimitConti
										   * (100 - DecreasePercent) / 100 );
		Main_PackData.ChgCurrLimitConti	= Main_PackData.ChgCurrLimitPeak;
		Main_PackData.DchCurrLimitPeak	= (sint16)( (uint32)Main_PackData.DchCurrLimitConti
										   * (100 - DecreasePercent) / 100 );
		Main_PackData.DchCurrLimitConti	= Main_PackData.DchCurrLimitPeak;
	}
	else
	{
		if ( (CAN_VcuBms.B1.bit.HvStatus == CAN_HvStatus_AcCharging)
		  || (CAN_VcuBms.B1.bit.HvStatus == CAN_HvStatus_DcCharging) )
        {}
		else
		{
			if (DecreasePercent	>= 2u)
			{
                DecreasePercent	-= 2u;
			}
            else
			{
                DecreasePercent	= 0u;
            }
		}
	}
#endif
	/* Limiting by min. charge currents */
	if ( (ChgCurrLimitConti > 0 )
	  && (ChgCurrLimitConti < 20) )
	{
	    ChgCurrLimitConti	= 20;
	}

	/* Limiting by SOC for CHG/DCH stop */
	if (SOC_Real >= CURRLIMIT_SOC_STOP_CHG)
	{
		ChgCurrLimitConti	= 0;
		ChgCurrLimitPeak	= 0;
	}
	
	if (SOC_Real <= CURRLIMIT_SOC_STOP_DCH)
	{
		DchCurrLimitConti	= 0;
		DchCurrLimitPeak	= 0;
	}	

	/* Limiting by fault flags */
	if (Diag_Get_FlagsFault() != 0u)
	{
		ChgCurrLimitConti	= 0;
		ChgCurrLimitPeak	= 0;
		DchCurrLimitConti	= 0;
		DchCurrLimitPeak	= 0;
	}	

	/* Select current limit */
//	Main_PackData.ChgCurrLimit	= CurrLimit_SelectPeakNormalChg();
//	Main_PackData.DchCurrLimit	= CurrLimit_SelectPeakNormalDch();

	ChgCurrLimit	= ChgCurrLimitConti;
	DchCurrLimit	= DchCurrLimitConti;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Find Current limit value
* @param	CurrLmtTable - Current limit table
* @param	AxisTemp - Axis data of temperature
* @param	AxisSoc - Axis data of SOC
* @param	nowTemp - Now Temperature
* @param	nowSOC - Now SOC
* @return	uint16 - Current limit value
*/
uint16	 CurrLimit_InterpolateCurrLimit(const float CurrLmtTable[][CURRLIMIT_SIZE_SOC_AXIS],
									const float AxisTemp[], const float AxisSoc[],
									float nowTemp, uint16 nowSOC)
{
	uint8		i;
	uint8		iTemp;	/* Row    index of the table */
	uint8		iSoc;	/* Column index of the table */

	float	TableByTemp[CURRLIMIT_SIZE_SOC_AXIS];	/* [A] */
	float	CLimit;							/* [A] */

	/* To make a table of the SOC distribution of the current limits
		corresponding to the current temperature */
	iTemp	= 0;
	if ( (nowTemp <= AxisTemp[0])
	  || (nowTemp >= AxisTemp[(CURRLIMIT_SIZE_TEMP_AXIS - 1u)]) )
	{
		for (i = 0u; i < CURRLIMIT_SIZE_SOC_AXIS; i++)
		{
		    TableByTemp[i]	= CURRLIMIT_MIN_CURR;
		}
	}
	else
	{
		for (i = 1u; i < CURRLIMIT_SIZE_TEMP_AXIS; i++)
		{
			/* To find the index of the axis corresponding to the current temperature */
			if (AxisTemp[i] > nowTemp)
			{
				iTemp	= i - 1u;
				break;
			}
		}
		for (i = 0u; i < CURRLIMIT_SIZE_SOC_AXIS; i++)
		{
			if ((sint32)CurrLmtTable[(iTemp + 1u)][i] == (sint32)CurrLmtTable[iTemp][i])
			{
                TableByTemp[i]	= CurrLmtTable[iTemp][i];
			}
            else
			{
				TableByTemp[i]
					= CurrLmtTable[iTemp][i]
					+ ( (CurrLmtTable[(iTemp + 1u)][i] - CurrLmtTable[iTemp][i])
					  * (nowTemp				  	  - AxisTemp[iTemp])
					  / (AxisTemp[(iTemp + 1u)]		  - AxisTemp[iTemp]) );
			}
		}
	}

	/* -------------------------------------------------------------------------- */
	/* To calculate the current limit */
	iSoc	= 0;
	if      (nowSOC <= (uint16)AxisSoc[0])
	{
	    CLimit	= TableByTemp[0];
	}
	else if (nowSOC >= (uint16)AxisSoc[(CURRLIMIT_SIZE_SOC_AXIS - 1u)])
	{
	    CLimit	= TableByTemp[(CURRLIMIT_SIZE_SOC_AXIS - 1u)];
	}
	else
	{
		for (i = 1u; i < CURRLIMIT_SIZE_SOC_AXIS; i++)
		{
			/* To find the index of the axis corresponding to the current SOC */
			if (AxisSoc[i] > (float)nowSOC)
			{
				iSoc	= i - 1u;
				break;
			}
		}
		if ((sint32)TableByTemp[(iSoc + 1u)] == (sint32)TableByTemp[iSoc])
		{
		    CLimit	= TableByTemp[iSoc];
		}
		else
		{
			CLimit
				= TableByTemp[iSoc]
				+ ( (TableByTemp[(iSoc + 1u)] - TableByTemp[iSoc])
				  * ((float)nowSOC			 - AxisSoc[iSoc])
				  / (AxisSoc[iSoc+1u] - AxisSoc[iSoc]) );
		}
	}

    CLimit *= (float)10;                /* [0.1 A] */    
	return	(uint16)CLimit;
}


/*---------------------------------------------------------------------*/
/**
* @brief	function that determines Charge Currentlimit mode whether Peak or Normal
* @param	void
* @return	sint16 - Charge Current limit result value 
*/
sint16	CurrLimit_SelectPeakNormalChg(void)
{
	static	uint16		Time_OverLimitNormal;	/* [0.2 sec] */
	static	uint8		Time_InLimitNormal;		/* [0.2 sec] */
	static	BOOL	OverLimitNormal_Flag;
	sint16				rv;						/* [0.1 A] */

	if (Curr_Get_I_PackMeas() > ChgCurrLimitConti)
	{
		Time_OverLimitNormal++;
		if (Time_OverLimitNormal > CURRLIMIT_TIME_PEAK_LIMIT)
		{
			Time_OverLimitNormal	= CURRLIMIT_TIME_PEAK_LIMIT;
			OverLimitNormal_Flag	= TRUE;
		}
	}
	else
	{
		if (Time_OverLimitNormal == 0u)
		{
			Time_OverLimitNormal	= 0u;
			OverLimitNormal_Flag	= FALSE;
		}
		else
		{
			Time_InLimitNormal++;
			if (Time_InLimitNormal >= CURRLIMIT_TIME_PEAK_REST)
			{
				Time_InLimitNormal	= 0u;
				Time_OverLimitNormal--;
			}
		}
	}
	if (OverLimitNormal_Flag == TRUE)
	{
	    rv	= ChgCurrLimitConti;
	}
	else
	{
	    rv	= ChgCurrLimitPeak;
	}
	
	/* Peak limit is only used in driving situations */
#if 0	// MBMS Scope -HMJ-
	if (CAN_VcuBms.B1.bit.HvStatus != CAN_HvStatus_DriveReady)
	{
        rv	= Main_PackData.ChgCurrLimitConti;
	}
#endif
	return	rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	function that determines Discharge Currentlimit mode whether Peak or Normal
* @param	void
* @return	sint16 - Discharge Current limit result value 
*/
sint16	CurrLimit_SelectPeakNormalDch(void)
{
	static	uint16		Time_OverLimitNormal;	/* [0.2 sec] */
	static	uint8		Time_InLimitNormal;		/* [0.2 sec] */
	static	BOOL	OverLimitNormal_Flag;
	sint16				rv;						/* [0.1 A] */

	if ( (Curr_Get_I_PackMeas() * (-1)) > DchCurrLimitConti )
	{
		Time_OverLimitNormal++;
		if (Time_OverLimitNormal > CURRLIMIT_TIME_PEAK_LIMIT)
		{
			Time_OverLimitNormal	= CURRLIMIT_TIME_PEAK_LIMIT;
			OverLimitNormal_Flag	= TRUE;
		}
	}
	else
	{
		if (Time_OverLimitNormal == 0u)
		{
			Time_OverLimitNormal	= 0u;
			OverLimitNormal_Flag	= FALSE;
		}
		else
		{
			Time_InLimitNormal++;
			if (Time_InLimitNormal >= CURRLIMIT_TIME_PEAK_REST)
			{
				Time_InLimitNormal	= 0u;
				Time_OverLimitNormal--;
			}
		}
	}
	if (OverLimitNormal_Flag == TRUE)
	{
	    rv	= DchCurrLimitConti;
	}
	else
	{
	    rv	= DchCurrLimitPeak;
    }
    
	/* Peak limit is only used in driving situations */
#if 0	// MBMS Scope -HMJ-
	if (CAN_VcuBms.B1.bit.HvStatus != CAN_HvStatus_DriveReady)
	{
        rv	= Main_PackData.DchCurrLimitConti;
    }
#endif
	return	rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	function of Caculating Remain Charge Time
* @param	handle - handel value
* @return	void
*/
void CurrLimit_CalcRemainChgTime_Proc(handle_CurrLimit_RemainChgTime* Handle)
{
	static	uint16	PeriodCnt;	/* [0.2 sec] */

	uint32 FCC;

	FCC = SOC_Get_FCC();

	PeriodCnt++;
	if ( PeriodCnt >= (5u * 60u) )
	{
		PeriodCnt	= 0u;

		/* Update the input data */
		Handle->SOC		= SOC_Get_SOC_Real() / 10u;
		Handle->Temp	= (float)Adbms_Get_T_CellAvg();
		Handle->IPack	= (float)Curr_Get_I_PackMeas() / 10.f;
		Handle->Fcc1Per	= (float)FCC / 100000.f;

		/* Update the output data */

		/* Call the execution function */
#if 0	// MBMS Scope -HMJ-
		if ( (CAN_VcuBms.B1.bit.HvStatus == CAN_HvStatus_AcCharging)
		  || (CAN_VcuBms.B1.bit.HvStatus == CAN_HvStatus_DcCharging) )
		{
            CurrLimit_CalcRemainChgTime(Handle);
        }
		else
		{
            Handle->RemainTime	= 0;
        }
#endif
		/* Export the output data */
//		RemainChgTime	= (uint16)Handle->RemainTime;
	}

}


/*---------------------------------------------------------------------*/
/**
* @brief	Calculate the remaining charging time
* @param	Handle : handle pointer of this function
* @return	void
*/
void CurrLimit_CalcRemainChgTime(handle_CurrLimit_RemainChgTime* Handle)
{
	uint8		i;
	uint8		iTemp;	/* Row    index of the table */
	uint8		iSoc;	/* Column index of the table */
	volatile float	ICalc;	/* [A] */
	volatile float	Time;	/* [minute] */

	/* To make a table of the SOC distribution of the current limits
		corresponding to the current temperature */
	iTemp	= 0u;
	if ( (Handle->Temp <= CurrLimit_AxisTempChg[0])
	  || (Handle->Temp >= CurrLimit_AxisTempChg[(CURRLIMIT_SIZE_TEMP_AXIS - 1u)]) )
	{
		for (i = 0u; i < CURRLIMIT_SIZE_SOC_AXIS; i++)
		{
		    Handle->TableByTemp[i]	= CURRLIMIT_MIN_CURR;
		}
	}
	else
	{
		for (i = 1u; i < CURRLIMIT_SIZE_TEMP_AXIS; i++)
		{
			/* To find the index of the axis corresponding to the current temperature */
			if (CurrLimit_AxisTempChg[i] > Handle->Temp)
			{
				iTemp	= i - 1u;
				break;
			}
		}
		for (i = 0u; i < CURRLIMIT_SIZE_SOC_AXIS; i++)
		{
			if ((sint32)CurrLimit_ChgTable_Conti[iTemp+1u][i] == (sint32)CurrLimit_ChgTable_Conti[iTemp][i])
			{
			    Handle->TableByTemp[i]
					= CurrLimit_ChgTable_Conti[iTemp][i];
		    }
			else
			{
				Handle->TableByTemp[i]
					= CurrLimit_ChgTable_Conti[iTemp][i]
					+ ( (CurrLimit_ChgTable_Conti[iTemp+1u][i] - CurrLimit_ChgTable_Conti[iTemp][i])
					  * (Handle->Temp             - CurrLimit_AxisTempChg[iTemp])
					  / (CurrLimit_AxisTempChg[iTemp+1u] - CurrLimit_AxisTempChg[iTemp]) );
			}
		}
	}

	/* To calculate the current limit value for each SOC % corresponding to the cell temperature
		and the remaining charging time */
	iSoc	= 0u;
	ICalc	= 0.f;
	Time	= 0.f;
	if ( Handle->SOC >= ((uint16)Handle->SocStopChg - 1u) )
	{	/* SOC 94 ~ 100 % */
		Handle->RemainTime	= 0.f;
	}
	else
	{
		/* SOC 0 ~ 93 % */
		Handle->RemainTime	= 0.f;
		for (i = 1u; i < Handle->SocStopChg; i++)
		{
			if ( ((float)i * (float)10) > CurrLimit_AxisSocChg[iSoc] )
			{
			    iSoc++;
			}
			if      (iSoc >= CURRLIMIT_SIZE_SOC_AXIS)
			{
				ICalc	= Handle->TableByTemp[CURRLIMIT_SIZE_SOC_AXIS-1u];
				if (Handle->IPack < CURRLIMIT_MIN_CURR)
				{
				    Handle->IPack	= CURRLIMIT_MIN_CURR;
				}
				if (ICalc  > Handle->IPack)
				{
				    ICalc	= Handle->IPack;
				}

				if (ICalc == (float)0.)
				{
				    ICalc	= 0.001f;
				}

				Time	= Handle->Fcc1Per / ICalc * (float)60;
				if ( (Handle->RemainTime + Time) > (float)CURRLIMIT_MAX_REMAIN_CHG_TIME)
				{
				    Handle->RemainTime	= (float)CURRLIMIT_MAX_REMAIN_CHG_TIME;
				}
				else
				{
				    Handle->RemainTime	+= Time;
				}
			}
			else if (i > Handle->SOC)
			{
				if (iSoc > 0u)
				{
					ICalc	= Handle->TableByTemp[iSoc-1u]
							+ ( (Handle->TableByTemp[iSoc] - Handle->TableByTemp[iSoc-1u])
							  * ( ((float)i * (float)10)        - CurrLimit_AxisSocChg[iSoc-1u] )
							  / (CurrLimit_AxisSocChg[iSoc] - CurrLimit_AxisSocChg[iSoc-1u]) );
					if (Handle->IPack < CURRLIMIT_MIN_CURR)
					{
						Handle->IPack	= CURRLIMIT_MIN_CURR;
					}
					if (ICalc  > Handle->IPack)
					{
						ICalc	= Handle->IPack;
					}

					if (ICalc == (float)0.)
					{
						ICalc	= 0.001f;
					}

					Time	= Handle->Fcc1Per / ICalc * (float)60;
					if ( (Handle->RemainTime + Time) > CURRLIMIT_MIN_CURR)
					{
						Handle->RemainTime	= CURRLIMIT_MIN_CURR;
					}
					else
					{
						Handle->RemainTime	+= Time;
					}
				}
			}
			else
			{
			}
		}
	}
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get flag of Fully charged
* @param	void
* @return	BOOL, TRUE - Fully Charged, FALSE - X
*/
BOOL CurrLimit_Get_FullyCHG_Flag(void)
{
	return CurrLimit_FullyChgFlag;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get flag of Fully Discharged
* @param	void
* @return	BOOL, TRUE - Fully Discharged, FALSE - X
*/
BOOL CurrLimit_Get_FullyDCH_Flag(void)
{
	return CurrLimit_FullyDchFlag;
}


/*---------------------------------------------------------------------*/
sint16 CurrLimit_Get_ChgCurrLimit(void)
{
    return ChgCurrLimit;
}


/*---------------------------------------------------------------------*/
sint16 CurrLimit_Get_DchCurrLimit(void)
{
    return DchCurrLimit;
}


