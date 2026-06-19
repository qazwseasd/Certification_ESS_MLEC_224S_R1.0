/*
 * App_Soc.c
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_Soc.h"
#include <math.h>


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define USER_SOC_USING              TRUE

#define SOC_ZERO_CRN				(1000.0f / 3600.0f)
#define SOC_RECOGNITION_TIME		5u

#define SOC_DETECT_FULL_DISCHARGE	0x1u
#define SOC_DETECT_FULL_CHARGE		0x2u
#define SOC_COMPLETE_FCC			0x4u
#define SOC_COMPENSATE_SOC			20u         //(uint16)(2 * 10)
#define SOC_INIT_TIME				1800u       //(uint16)(30 * 60)
#define SOC_FCC_DEFAULT				60000u	

#define SOC_Step_Num                21u
#define TEMP_Step_Num               8u

#define SOH_Step_Num                21u

#define CONV_mVtoV                  0.001f
#define CONV_mAtoA                  0.001f

#define SOC_MAXREAL					1000.0f		/* [0.01 %] */
#define SOC_MAXUSER					950.0f		/* [0.01 %] */
#define SOC_MINUSER					100.0f		/* [0.01 %] */
#define SOC_MINREAL					0.0f		/* [0.01 %] */


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/
static const uint16 OCV_TABLE[SOC_Step_Num][TEMP_Step_Num] =
{
/*   -20,     -10,     0,       10,      20       30       40       50   [?? */
    {3132u,   3119u,   3150u,   3142u,   3128u,   3113u,   3101u,   3208u},  /*   0 % */
    {3384u,   3378u,   3377u,   3376u,   3375u,   3373u,   3371u,   3383u},  /*   5 % */
    {3430u,   3422u,   3421u,   3421u,   3419u,   3418u,   3418u,   3428u},  /*  10 % */
    {3484u,   3478u,   3480u,   3479u,   3477u,   3475u,   3473u,   3484u},  /*  15 % */
    {3535u,   3532u,   3534u,   3534u,   3532u,   3530u,   3528u,   3535u},  /*  20 % */
    {3570u,   3573u,   3574u,   3576u,   3575u,   3573u,   3571u,   3576u},  /*  25 % */
    {3595u,   3598u,   3599u,   3603u,   3604u,   3605u,   3606u,   3609u},  /*  30 % */
    {3618u,   3620u,   3621u,   3626u,   3628u,   3629u,   3631u,   3634u},  /*  35 % */
    {3642u,   3643u,   3645u,   3650u,   3651u,   3653u,   3655u,   3659u},  /*  40 % */
    {3670u,   3671u,   3673u,   3677u,   3679u,   3682u,   3684u,   3688u},  /*  45 % */
    {3708u,   3709u,   3711u,   3715u,   3717u,   3719u,   3722u,   3727u},  /*  50 % */
    {3760u,   3759u,   3762u,   3767u,   3768u,   3770u,   3771u,   3778u},  /*  55 % */
    {3824u,   3827u,   3832u,   3834u,   3837u,   3839u,   3840u,   3840u},  /*  60 % */
    {3876u,   3882u,   3883u,   3884u,   3885u,   3886u,   3886u,   3886u},  /*  65 % */
    {3920u,   3922u,   3922u,   3923u,   3924u,   3926u,   3926u,   3926u},  /*  70 % */
    {3964u,   3965u,   3964u,   3966u,   3967u,   3968u,   3968u,   3968u},  /*  75 % */
    {4016u,   4016u,   4016u,   4017u,   4018u,   4019u,   4020u,   4019u},  /*  80 % */
    {4066u,   4066u,   4067u,   4068u,   4069u,   4070u,   4071u,   4069u},  /*  85 % */
    {4096u,   4096u,   4096u,   4098u,   4099u,   4100u,   4101u,   4100u},  /*  90 % */
    {4119u,   4119u,   4119u,   4122u,   4123u,   4123u,   4124u,   4122u},  /*  95 % */
    {4173u,   4172u,   4174u,   4174u,   4174u,   4175u,   4174u,   4169u}   /* 100 % */
}; 

static const sint16 raw_standard[TEMP_Step_Num]   =
{
    -200,   -100,   0,  100,    200,    300,    400,    500     /* [0.1 ?? */
};

static const uint16 Minimum_Cut[SOH_Step_Num][TEMP_Step_Num] =
{
/*   -20,     -10,     0,       10,      20      30      40     50   [?? */
    {162u,    115u,    80u,     30u,     0u,  	 0u,   	 0u,   	0u},  /* 100 % */
    {170u,    121u,    84u,     32u,     2u,  	 0u,   	 0u,   	0u},  /*  99 % */
	{178u,    127u,    88u,     33u,     3u,  	 0u,   	 0u,   	0u},  /*  98 % */
	{186u,    132u,    92u,     35u,     5u,  	 0u,   	 0u,   	0u},  /*  97 % */
	{194u,    138u,    96u,     36u,     6u,  	 0u,   	 0u,   	0u},  /*  96 % */
	{203u,    144u,    100u,    38u,     8u,  	 0u,   	 0u,   	0u},  /*  95 % */
	{211u,    150u,    104u,    39u,     9u,  	 0u,   	 0u,   	0u},  /*  94 % */
	{219u,    155u,    108u,    41u,     11u,  	 0u,   	 0u,   	0u},  /*  93 % */
	{227u,    161u,    112u,    42u,     12u,  	 0u,   	 0u,   	0u},  /*  92 % */
	{235u,    167u,    116u,    44u,     14u,  	 0u,   	 0u,   	0u},  /*  91 % */
	{243u,    173u,    120u,    45u,     15u,  	 0u,   	 0u,   	0u},  /*  90 % */
	{241u,    178u,    124u,    47u,     17u,  	 0u,   	 0u,   	0u},  /*  89 % */
	{259u,    184u,    128u,    47u,     18u,  	 0u,   	 0u,   	0u},  /*  88 % */
	{267u,    190u,    132u,    50u,     20u,  	 0u,   	 0u,   	0u},  /*  87 % */
	{275u,    196u,    136u,    51u,     21u,  	 0u,   	 0u,   	0u},  /*  86 % */
	{284u,    201u,    140u,    53u,     23u,  	 0u,   	 0u,   	0u},  /*  85 % */
	{292u,    207u,    144u,    54u,     24u,  	 0u,   	 0u,   	0u},  /*  84 % */
	{300u,    213u,    148u,    56u,     26u,  	 0u,   	 0u,   	0u},  /*  83 % */
	{308u,    219u,    152u,    57u,     27u,  	 0u,   	 0u,   	0u},  /*  82 % */
	{316u,    224u,    156u,    59u,     29u,  	 0u,   	 0u,   	0u},  /*  81 % */
	{324u,    230u,    160u,    60u,     30u,  	 0u,   	 0u,   	0u}   /*  80 % */
}; 


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static BOOL     Soc_InitFlag = FALSE;
static float    g_f_capacity_buf;
static float    Soc_ChargeWh = 0.0f;
static float    Soc_DischargeWh = 0.0f;
BOOL    g_b_Discharge_flag = FALSE;

CAPACITY_DATA	static CapacityData;
static	uint16	SOC_Real;
static	uint16	SOC_Temp;
static	uint16	SOC_User;
static  uint16	CV_SOC;



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
* @brief	SOC Initialization function
* @param	void
* @return	void
*/
extern void Soc_Init(void)
/*
	Initialize SOC 									
*/
{	
	uint16 V_CellAvg;

	V_CellAvg = Adbms_Get_V_CellAvg();

	uint16 nowSOC = Soc_Calculate_UseOCVTable(V_CellAvg);
	uint16	difference;

	if((CapacityData.Complete_Init_FCC & SOC_COMPLETE_FCC) > 0u)
	{
		SOC_Real = (uint16)(CapacityData.Capacity * SOC_MAX_SOC / CapacityData.FCC);

		if (SOC_Real >= nowSOC)
		{
			difference = SOC_Real - nowSOC;
		}
		else
		{
			difference = nowSOC - SOC_Real;
		}

		if(difference >= SOC_COMPENSATE_SOC)
		{
            SOC_Real     = nowSOC;
            CapacityData.Capacity = (uint32)(CapacityData.FCC / SOC_MAX_SOC * nowSOC);
		}
	}
	else
	{
		SOC_Real 	  = nowSOC;
		CapacityData.FCC 	  = SOC_FCC_DEFAULT;
		CapacityData.Init_FCC = SOC_FCC_DEFAULT;
		CapacityData.Capacity = (uint32)(CapacityData.FCC * nowSOC / SOC_MAX_SOC);
	}
	
	g_f_capacity_buf = (float)CapacityData.Capacity;
	Soc_ChargeWh = (float)CapacityData.Chg_Wh;
	Soc_DischargeWh = (float)CapacityData.Dch_Wh;
	CapacityData.Complete_Init_FCC = 0x7;
}


/*---------------------------------------------------------------------*/
/**
* @brief	SOC Calculation Task
* @param	void
* @return	void
*/
extern void Soc_Calculation_Task(void)
{
	uint16 MinCut = Calculate_MinimumSOCCut(SOH_Get_SOH());	/* SOH = 100% */
	uint16 MaxCut = 1000u;
	bool flag = CurrLimit_Get_FullyCHG_Flag();
	uint16	difference;
	uint16 u16data;
	float fdata;

	uint16 V_CellMax;
	uint16 V_CellMin;
	uint16 V_CellAvg;

	V_CellMax = Adbms_Get_V_CellMax();
	V_CellMin = Adbms_Get_V_CellMin();
	V_CellAvg = Adbms_Get_V_CellAvg();

	if(Soc_InitFlag == TRUE)
	{	
		uint16        nowSOC;
		uint16        OCV_SOC; 	
		float               buf = 0.0f;
		static uint8           recognition_cnt = 0;
		static uint16          integralcrn_zerocnt = 0;
		
		if((V_CellMax >= SOC_CV_SOC100) && (flag == TRUE))
		{
			if(recognition_cnt++ >= (SOC_RECOGNITION_TIME - 1u))
			{			
				if((CapacityData.Complete_Init_FCC & SOC_COMPLETE_FCC) == SOC_COMPLETE_FCC)
				{
					if((CapacityData.Complete_Init_FCC & SOC_DETECT_FULL_CHARGE) == 0u)
					{
						CapacityData.Complete_Init_FCC |= (SOC_DETECT_FULL_CHARGE | SOC_DETECT_FULL_DISCHARGE);
						CapacityData.FCC = CapacityData.Init_FCC;
						g_f_capacity_buf = (float)CapacityData.Init_FCC;
					}
					else
					{
						g_f_capacity_buf = (float)CapacityData.FCC;
						Soh_SetUpdateFlag(TRUE);									
					}
				}
			}
		}
		else if((V_CellMin <= SOC_CV_SOC0) && (flag == TRUE))
		{	
			if(recognition_cnt++ >= (SOC_RECOGNITION_TIME - 1u))
			{
				if((CapacityData.Complete_Init_FCC & SOC_COMPLETE_FCC) == SOC_COMPLETE_FCC)
				{
					if((CapacityData.Complete_Init_FCC & SOC_DETECT_FULL_DISCHARGE) == 0u)
					{
						CapacityData.Complete_Init_FCC |= (SOC_DETECT_FULL_CHARGE | SOC_DETECT_FULL_DISCHARGE);
						CapacityData.FCC = CapacityData.Init_FCC;							
					}
				}
				g_f_capacity_buf = 0.0f;
			}
		}
		else
		{
			recognition_cnt = 0;
		}			
						
		if(Curr_GetCurr1secSum() != 0)
		{		
			buf  = (float)Curr_GetCurr1secSum() / (float)Curr_GetCurr1secCnt() / 3600.0f;
		}

		Curr_SetCurr1secSum(0);
		Curr_SetCurr1secCnt(0);

		if (buf < (SOC_ZERO_CRN * -1.0f))
		{
			g_b_Discharge_flag = TRUE;
			if ((buf * -1.0f) < g_f_capacity_buf)
            {
                g_f_capacity_buf += buf;
            }
			else
            {
                g_f_capacity_buf = 0.0f;
            }
            buf = buf * -1.0f;
			CapacityData.DchCntIntegral += (uint32)buf;            //(uint32)(buf * -1);
			if (CapacityData.DchCntIntegral >= CapacityData.FCC * 80u / 100u)
			{
				CapacityData.DchCntIntegral -= (CapacityData.FCC * 80u / 100u);
				CapacityData.Cycle_Count += 1u;
			}

            CapacityData.Accum_DischargeMilliAmps += buf;  //(buf * -1);
			if(CapacityData.Accum_DischargeMilliAmps > 1000.0f)
			{
                CapacityData.Accum_DischargeAmps += 1u;
                CapacityData.Accum_DischargeMilliAmps -= 1000.0f;
			}

//			Soc_DischargeWh += ((buf * (float)-1) * (float)Main_PackData.Rated_Volt * (float)0.1 * CONV_mAtoA);
			Soc_DischargeWh += (buf * (float)AppInit_Get_Rated_Volt() * 0.1f * CONV_mAtoA);
			CapacityData.Dch_Wh = (uint16)Soc_DischargeWh;
			if(CapacityData.Dch_Wh >= 1000u)
			{
				CapacityData.Dch_kWh += 1u;
				CapacityData.Dch_Wh -= 1000u;
				Soc_DischargeWh -= 1000.0f;
			}

			integralcrn_zerocnt = 0u;
//			g_u8_FullChargeFlag = FALSE;
		}
		else if(buf > SOC_ZERO_CRN)
		{
//			if (g_f_capacity_buf < ((float)CapacityData.FCC - buf)) 	g_f_capacity_buf += buf;
//			else														g_f_capacity_buf = CapacityData.FCC;
			g_f_capacity_buf += buf ;
			if(g_f_capacity_buf > (float)CapacityData.FCC)
			{
				CapacityData.FCC = (uint32)g_f_capacity_buf;
				if(CapacityData.FCC > CapacityData.Init_FCC)
				{
					CapacityData.FCC = CapacityData.Init_FCC;
					g_f_capacity_buf = (float)CapacityData.Init_FCC;
				}
			}
			
            CapacityData.Accum_ChargeMilliAmps += buf;
			if(CapacityData.Accum_ChargeMilliAmps > 1000.0f)
			{
                CapacityData.Accum_ChargeAmps += 1u;
                CapacityData.Accum_ChargeMilliAmps -= 1000.0f;
			}

			Soc_ChargeWh += (buf * (float)AppInit_Get_Rated_Volt() * 0.1f * CONV_mAtoA);
			CapacityData.Chg_Wh = (uint16)Soc_ChargeWh;
			if(CapacityData.Chg_Wh >= 1000u)
			{
				CapacityData.Chg_kWh += 1u;
				CapacityData.Chg_Wh -= 1000u;
				Soc_ChargeWh -= 1000.0f;
			}

			integralcrn_zerocnt = 0u;
//			g_u8_FullDischargeFlag = FALSE;			
		}			
		else
		{
			if ((buf < g_f_capacity_buf) && (buf > (g_f_capacity_buf * -1.0f)))
            {
                g_f_capacity_buf += buf;
            }
			else
            {
                g_f_capacity_buf = 0.0f;
            }
			
			if(integralcrn_zerocnt++ >= (SOC_INIT_TIME - 1u))
			{
				integralcrn_zerocnt = 0;
				OCV_SOC = Soc_Calculate_UseOCVTable(V_CellAvg);			
				nowSOC = (uint16)((uint32)g_f_capacity_buf * (uint32)1000. / CapacityData.FCC);
				
				if(ManaMsm_Get_System_State() != SYS_DETECTING_FCC)
				{
					if (OCV_SOC >= nowSOC)
					{
						difference = OCV_SOC - nowSOC;
					}
					else
					{
						difference = nowSOC - SOC_Real;
					}

            		if(difference >= SOC_COMPENSATE_SOC)
            		{
						g_f_capacity_buf = (float)CapacityData.FCC * (float)OCV_SOC / (float)1000.;
						Soh_SetUpdateFlag(TRUE);
						EepromEx_SetUpdateCapacityData_Flag(TRUE);
            		}					
				}
			}			
		}
		
		CapacityData.Capacity = (uint32)g_f_capacity_buf;	
		nowSOC = (uint16)((uint32)g_f_capacity_buf * (uint32)SOC_MAX_SOC / CapacityData.FCC);
		
		if(nowSOC >= SOC_MAX_SOC) 
		{
		    SOC_Real = SOC_MAX_SOC;
		}
		else
		{
		    SOC_Real = nowSOC;
		}
	}

	CV_SOC = Soc_Calculate_UseOCVTable(V_CellAvg);

#if (USER_SOC_USING == TRUE)

	if(SOC_Real <= MinCut)
	{
		SOC_Temp	= (uint16)SOC_MINREAL;
	}
	else if(SOC_Real >= MaxCut)
	{
		SOC_Temp	= (uint16)SOC_MAXREAL;
	}
	else
	{
		u16data = SOC_Real - MinCut;
		fdata = (SOC_MAXREAL - SOC_MINREAL) / ((float)MaxCut - (float)MinCut);
		fdata = (float)u16data * fdata;
		SOC_Temp	= (uint16)fdata;
	}


	if(SOC_Temp <= (uint16)SOC_MINUSER)
	{
		SOC_User	= (uint16)SOC_MINREAL;
	}
	else if(SOC_Temp >= (uint16)SOC_MAXUSER)
	{
		SOC_User	= (uint16)SOC_MAXREAL;
	}
	else
	{
		u16data = SOC_Temp - (uint16)SOC_MINUSER;
		fdata = (SOC_MAXREAL - SOC_MINREAL) / (SOC_MAXUSER - SOC_MINUSER);
		fdata = (float)u16data * fdata;
		SOC_User	= (uint16)fdata;
	}
	
#else

    SOC_User = SOC_Real;	
    
#endif 
}

#if 1
/*---------------------------------------------------------------------*/
/**
* @brief	Calculate the SOC used to OCV table, apply all temperature area
* @param	cell_v - Cell Voltage
* @return	uint16 - SOC
*/
extern	uint16 Soc_Calculate_UseOCVTable(uint16 cell_v)
{
	uint16		i;
	sint16		nowTemp;	/* [0.1 ?? */
	uint16		nowCellv;	/* [mV] */
	uint16		arr_std		= 0;
	uint16		ocvtable_atTemp[SOC_Step_Num]	= {0};
	uint16		nowSOC		= 0;	/* [0.1 %] */
	volatile float	buf			= 0.f;
	float	fdata;

	nowTemp		= Adbms_Get_T_CellAvg();
	nowCellv	= cell_v;

	if (nowTemp <= raw_standard[0])
	{
		for(i=0; i<SOC_Step_Num; i++)
		{
			ocvtable_atTemp[i]	= OCV_TABLE[i][0];
		}
	}
	else if (nowTemp >= raw_standard[TEMP_Step_Num - 1u])
	{
		for (i=0; i<SOC_Step_Num; i++)
		{
		    ocvtable_atTemp[i]	= OCV_TABLE[i][TEMP_Step_Num - 1u];
		}
	}
	else
	{
		for (i=0; i<TEMP_Step_Num; i++)
		{
			if (raw_standard[i] > nowTemp)
			{
				arr_std	= i;
				break;
			}
			else{}
		}
		if (arr_std > 0u)
		{
			for (i=0; i<SOC_Step_Num; i++)
			{
				buf	= (float)(((float)OCV_TABLE[i][arr_std] - (float)OCV_TABLE[i][arr_std-1u])
					  / ((float)raw_standard[arr_std] - (float)raw_standard[arr_std-1u]));

				fdata =  ((float)nowTemp - (float)raw_standard[arr_std-1u]) * buf;
				ocvtable_atTemp[i] = (uint16)OCV_TABLE[i][arr_std-1u] + (uint16)fdata;
			}
		}
	}

	if		(nowCellv <  ocvtable_atTemp[0])
	{
		nowSOC	= 0u;
	}
	else if (nowCellv >= ocvtable_atTemp[SOC_Step_Num - 1u])
	{
	    nowSOC	= SOC_MAX_SOC;
	}
	else
	{
		for (i=1; i<SOC_Step_Num; i++)
		{
			if (ocvtable_atTemp[i] > nowCellv)
			{
				arr_std	= i;
				break;
			}
		}

		if (arr_std > 0u)
		{
			buf	= (float)50./ ((float)ocvtable_atTemp[arr_std] - (float)ocvtable_atTemp[arr_std-1u]);
			fdata = buf * ((float)nowCellv - (float)ocvtable_atTemp[arr_std-1u]);
			nowSOC = (uint16)fdata;
			nowSOC += (uint16)(i-1u) * 50u;
			if (nowSOC >= SOC_MAX_SOC)
			{
				nowSOC	= SOC_MAX_SOC;
			}
			else{}
		}
	}

	return	nowSOC;
}

#else
/*---------------------------------------------------------------------*/
/**
* @brief	Calculate the SOC used to OCV table, Only 20 degree 
* @param	cell_v - Cell Voltage
* @return	uint16 - SOC
*/
uint16 Soc_Calculate_UseOCVTable(uint16 cell_v)								// debug: 20.06.23
{
    uint16 i;
    uint16 nowCellv = 0u;
    uint16 ocvtable_atTemp[22] = {0};     
    uint16 arr_std = 0u;             
    uint16 nowSOC = 0u;
    float buf = 0.; 

    /********************************
    * Lookup Tables *
    * Cell Model: E60A 
    ********************************/
    const uint16 OCV_TABLE[SOC_Step_Num][TEMP_Step_Num] =
    {
    /*   -20,    -10,    0,      10,     20      30      40     50   [?? */
        {2700,   2700,   2700,   2700,   2700,   2700,   2700,   2700},  /*   0 % */
        {3493,   3452,   3415,   3386,   3335,   3308,   3297,   3284},  /*   5 % */
        {3531,   3504,   3469,   3431,   3398,   3390,   3389,   3388},  /*  10 % */
        {3560,   3545,   3518,   3489,   3445,   3438,   3432,   3430},  /*  15 % */
        {3583,   3571,   3558,   3535,   3504,   3495,   3489,   3487},  /*  20 % */
        {3603,   3593,   3584,   3572,   3550,   3543,   3536,   3531},  /*  25 % */
        {3624,   3614,   3606,   3598,   3585,   3581,   3577,   3573},  /*  30 % */
        {3648,   3637,   3628,   3620,   3611,   3607,   3609,   3610},  /*  35 % */
        {3676,   3662,   3654,   3643,   3634,   3631,   3634,   3635},  /*  40 % */
        {3713,   3697,   3685,   3672,   3662,   3658,   3661,   3663},  /*  45 % */
        {3756,   3739,   3727,   3711,   3697,   3693,   3696,   3699},  /*  50 % */
        {3800,   3789,   3778,   3761,   3744,   3739,   3741,   3743},  /*  55 % */
        {3846,   3844,   3836,   3825,   3808,   3802,   3806,   3808},  /*  60 % */
        {3893,   3891,   3885,   3876,   3864,   3860,   3863,   3863},  /*  65 % */
        {3933,   3930,   3916,   3915,   3906,   3904,   3905,   3906},  /*  70 % */
        {3974,   3972,   3958,   3958,   3950,   3948,   3949,   3948},  /*  75 % */
        {4015,   4018,   4014,   4000,   4002,   3999,   4000,   4000},  /*  80 % */
        {4050,   4057,   4057,   4055,   4052,   4050,   4051,   4051},  /*  85 % */
        {4075,   4082,   4085,   4086,   4086,   4085,   4086,   4086},  /*  90 % */
        {4104,   4108,   4111,   4115,   4115,   4114,   4115,   4114},  /*  95 % */
        {4144,   4152,   4165,   4167,   4168,   4169,   4170,   4167}   /* 100 % */
    }; 

    nowCellv = cell_v;

    for(i=0; i<SOC_Step_Num; i++)
    {
    	ocvtable_atTemp[i] = OCV_TABLE[i][3];
    }

    if (nowCellv < ocvtable_atTemp[0])
    {
        nowSOC = 0u;
    }
    else if(nowCellv >= ocvtable_atTemp[SOC_Step_Num])
    {
        nowSOC = SOC_MAX_SOC;
    }
    else
    {
    	for (i=0; i<SOC_Step_Num; i++)
    	{
    		if (ocvtable_atTemp[i] > nowCellv)
    		{
    			arr_std = i;
    			break;
    		}
    	}
		buf	= (float)50./ ((float)ocvtable_atTemp[arr_std] - (float)ocvtable_atTemp[arr_std-1u]);
		nowSOC = (uint16)(buf * ((float)nowCellv - (float)ocvtable_atTemp[arr_std-1u]));
		nowSOC += (uint16)(i-1u) * 50u;

    	if(nowSOC >= SOC_MAX_SOC)
    	{
    	    nowSOC = SOC_MAX_SOC;
    	}
    	else
    	{
    	    nowSOC += 10u;
    	}
    }	

    return nowSOC;
}
#endif

/*---------------------------------------------------------------------*/
/**
* @brief	Calculate the Minimum SOC Cut at each Temperature
* @param	SOH - System SOH
* @return	uint16 - SOC Cut
*/
extern	uint16 Calculate_MinimumSOCCut(uint16 SOH)
{
	uint16		i;
	sint16		nowTemp;
	uint16		nowSOH;
	uint16		arr_std		= 0;
	uint16		MincutTable_atTemp[SOH_Step_Num]	= {0};
	uint16		nowCut;	/* [0.1 %] */
	volatile float	buf			= 0.f;
	float fdata;

	nowTemp		= Adbms_Get_T_CellAvg();
	nowSOH		= SOH;		

	if (nowTemp <= raw_standard[0])
	{
		for(i=0; i<SOH_Step_Num; i++)
		{
			MincutTable_atTemp[i]	= Minimum_Cut[i][0];
		}
	}
	else if (nowTemp >= raw_standard[TEMP_Step_Num - 1u])
	{
		for (i=0; i<SOH_Step_Num; i++)
		{
		    MincutTable_atTemp[i]	= Minimum_Cut[i][TEMP_Step_Num - 1u];
		}
	}
	else
	{
		for (i=0; i<TEMP_Step_Num; i++)
		{
			if (raw_standard[i] > nowTemp)
			{
				arr_std	= i;
				break;
			}
			else{}
		}

		if (arr_std > 0u)
		{
			for (i=0; i<SOH_Step_Num; i++)
			{
				buf	= (float)(((float)Minimum_Cut[i][arr_std] - (float)Minimum_Cut[i][arr_std-1u])
					  / ((float)raw_standard[arr_std] - (float)raw_standard[arr_std-1u]));
				fdata = ((float)nowTemp - (float)raw_standard[arr_std-1u]) * buf;
				MincutTable_atTemp[i] = (uint16)Minimum_Cut[i][arr_std-1u] + (uint16)fdata;
			}
		}
	}

	if(nowSOH >= 100u)
	{
		nowCut = MincutTable_atTemp[0];
	}
	else if(nowSOH <= 80u)
	{
		nowCut = MincutTable_atTemp[20];
	}
	else
	{

		for(i=0; i<SOH_Step_Num; i++)
		{
			if ( nowSOH >= (100u-i)  )
			{
				arr_std = i;
				break;
			}
		}
		
		nowCut = MincutTable_atTemp[arr_std];
	}
	

	return	nowCut;
	
}


/*---------------------------------------------------------------------*/
/**
* @brief    Set SOC Init Complete Flag
* @param    BOOL - True : Complete Init SOC
* @return   void
*/
extern void Soc_SetSocInitFlag(BOOL flag)
{
	Soc_InitFlag = flag;
}

/*---------------------------------------------------------------------*/
/**
* @brief Calculate the cell voltage used to OCV table
* @param R_SOC - RSOC
* @return float - Cell voltage
*/
extern float Calculate_CellV_useOCVTable(uint16 R_SOC)
{
    uint16 i;
    uint16 arr_std = 0u;
    uint16 ocvtable_atTemp[SOC_Step_Num];
    uint16 nowCellV = 0u;
    uint16 div_SOC = 0u;
    uint16 nowSOC;    
    sint16 nowTemp;
    float s_temp;
    float div_cellV; 
    float rv;
    float buf;
	float fdata;
	uint16	u16data;
    
    nowTemp = Adbms_Get_T_CellAvg();
    nowSOC  = R_SOC;
    
	if (nowTemp <= raw_standard[0])
	{
		for(i=0u; i<SOC_Step_Num; i++)
		{
			ocvtable_atTemp[i]	= OCV_TABLE[i][0];
		}
	}
	else if (nowTemp >= raw_standard[TEMP_Step_Num - 1u])
	{
		for (i=0u; i<SOC_Step_Num; i++)
		{
		    ocvtable_atTemp[i]	= OCV_TABLE[i][TEMP_Step_Num - 1u];
		}
	}
	else
	{
		for (i=0u; i<TEMP_Step_Num; i++)
		{
			if (raw_standard[i] > nowTemp)
			{
				arr_std	= i;
				break;
			}
			else{}
		}

		if (arr_std > 0u)
		{
			for (i=0u; i<SOC_Step_Num; i++)
			{
				buf	= (float)(((float)OCV_TABLE[i][arr_std] - (float)OCV_TABLE[i][arr_std-1u])
					  / ((float)raw_standard[arr_std] - (float)raw_standard[arr_std-1u]));
				fdata = ((float)nowTemp - (float)raw_standard[arr_std-1u]) * buf;
				ocvtable_atTemp[i] = (uint16)OCV_TABLE[i][arr_std-1u] + (uint16)fdata;
			}
		}
	}

    for(arr_std = 0u; arr_std < SOC_Step_Num; arr_std++)
    {
        if(nowSOC < (arr_std * 50u))
        {
            break;
        }
    }
    
    div_cellV = 50.f; 
    if(arr_std > 0u)
    {
        div_SOC = (arr_std - 1u) * 50u;
    }

    if (arr_std < (SOC_Step_Num - 1u))
    {
		u16data = ocvtable_atTemp[arr_std + 1u] - ocvtable_atTemp[arr_std];
		fdata = (float)u16data / div_cellV;

		u16data = nowSOC - div_SOC;
		s_temp = (float)u16data * fdata;
		
		nowCellV = ocvtable_atTemp[arr_std] + (uint16)s_temp;
    }

    if(nowSOC == 0u)
    {
        rv = (float)((float)ocvtable_atTemp[0] * (float)0.001);
    }
    else if(nowSOC >= 1000u)
    {
        rv = (float)((float)ocvtable_atTemp[20] * (float)0.001);
    }
    else
    {
        rv = (float)((float)nowCellV * (float)0.001);
    }

    return rv;
}

/*---------------------------------------------------------------------*/
/**
* @brief    Capacity Value Update Function
* @param    float - Capacity Value
* @return   void
*/
extern void Soc_UpdataCapacityData(float CapacityValue)
{
    g_f_capacity_buf = CapacityValue;   
}


/*---------------------------------------------------------------------*/
/**
* @brief	Calculate the SOC used to OCV table, apply all temperature area
* @param	cell_v - Cell Voltage
* @return	uint16 - SOC
*/
extern	uint16 Soc_Calculate_UseOCVTable_SameTemp(uint16 cell_v)
{
	uint16		i;
	sint16		nowTemp;	/* [0.1 ?? */
	uint16		nowCellv;	/* [mV] */
	uint16		arr_std		= 0;
	uint16		ocvtable_atTemp[SOC_Step_Num]	= {0};
	uint16		nowSOC		= 0;	/* [0.1 %] */
	volatile float	buf			= 0.f;
	float 	fdata;

	nowTemp		= (sint16)300;
	nowCellv	= cell_v;

	for (i=0; i<TEMP_Step_Num; i++)
	{
		if (raw_standard[i] > nowTemp)
		{
			arr_std	= i;
			break;
		}
		else{}
	}

	if (arr_std > 0u)
	{
		for (i=0; i<SOC_Step_Num; i++)
		{
			buf	= (float)(((float)OCV_TABLE[i][arr_std] - (float)OCV_TABLE[i][arr_std-1u])
				  / ((float)raw_standard[arr_std] - (float)raw_standard[arr_std-1u]));

			fdata = ((float)nowTemp - (float)raw_standard[arr_std-1u]) * buf;
			ocvtable_atTemp[i] = (uint16)OCV_TABLE[i][arr_std-1u] + (uint16)fdata;
		}
	}
	

	if		(nowCellv <  ocvtable_atTemp[0])
	{
		nowSOC	= 0u;
	}
	else if (nowCellv >= ocvtable_atTemp[SOC_Step_Num - 1u])
	{
	    nowSOC	= SOC_MAX_SOC;
	}
	else
	{
		for (i=1; i<SOC_Step_Num; i++)
		{
			if (ocvtable_atTemp[i] > nowCellv)
			{
				arr_std	= i;
				break;
			}
		}

		if (arr_std > 0u)
		{
			buf	= (float)50./ ((float)ocvtable_atTemp[arr_std] - (float)ocvtable_atTemp[arr_std-1u]);
			fdata = buf * ((float)nowCellv - (float)ocvtable_atTemp[arr_std-1u]);
			nowSOC = (uint16)fdata;
			nowSOC += (uint16)(i-1u) * 50u;
			if (nowSOC >= SOC_MAX_SOC)
			{
				nowSOC	= SOC_MAX_SOC;
			}
			else{}
		}
	}

	return	nowSOC;
}


/*---------------------------------------------------------------------*/
void SOC_Get_CapacityData(CAPACITY_DATA* Capadata)
{
    (void)memcpy(Capadata, &CapacityData, sizeof(CAPACITY_DATA));
}


/*---------------------------------------------------------------------*/
void SOC_Set_CapacityData(CAPACITY_DATA* Capadata)
{
    (void)memcpy(&CapacityData, Capadata, sizeof(CAPACITY_DATA));
}


/*---------------------------------------------------------------------*/
uint16 SOC_Get_ContOpenCount_UnderLoad(void)
{
    return CapacityData.ContOpenCount_UnderLoad;
}


/*---------------------------------------------------------------------*/
void SOC_Set_ContOpenCount_UnderLoad(uint16 ContOpenCount)
{
    CapacityData.ContOpenCount_UnderLoad = ContOpenCount;
}


/*---------------------------------------------------------------------*/
uint32 SOC_Get_Accum_ChargeAmps(void)
{
    return CapacityData.Accum_ChargeAmps;
}


/*---------------------------------------------------------------------*/
uint32 SOC_Get_Accum_DischargeAmps(void)
{
    return CapacityData.Accum_DischargeAmps;
}


/*---------------------------------------------------------------------*/
uint32 SOC_Get_Chg_kWh(void)
{
    return CapacityData.Chg_kWh;
}


/*---------------------------------------------------------------------*/
uint32 SOC_Get_Dch_kWh(void)
{
    return CapacityData.Dch_kWh;
}


/*---------------------------------------------------------------------*/
uint32 SOC_Get_Init_FCC(void)
{
    return CapacityData.Init_FCC;
}


/*---------------------------------------------------------------------*/
uint32 SOC_Get_FCC(void)
{
    return CapacityData.FCC;
}


/*---------------------------------------------------------------------*/
void SOC_Set_FCC(uint32 FCC)
{
    CapacityData.FCC = FCC;
}


/*---------------------------------------------------------------------*/
uint32 SOC_Get_Capacity(void)
{
    return CapacityData.Capacity;
}


/*---------------------------------------------------------------------*/
void SOC_Set_Capacity(uint32 Capacity)
{
    CapacityData.Capacity = Capacity;
}


/*---------------------------------------------------------------------*/
uint16 SOC_Get_CycleCount(void)
{
    return CapacityData.Cycle_Count;
}


/*---------------------------------------------------------------------*/
uint16 SOC_Get_SOC_Real(void)
{
    return SOC_Real;
}


/*---------------------------------------------------------------------*/
uint16 SOC_Get_SOC_Temp(void)
{
    return SOC_Temp;
}


/*---------------------------------------------------------------------*/
uint16 SOC_Get_SOC_User(void)
{
    return SOC_User;
}


/*---------------------------------------------------------------------*/
uint16 SOC_Get_CV_SOC(void)
{
    return CV_SOC;
}



