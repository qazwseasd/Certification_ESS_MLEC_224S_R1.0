/*
 * Api_Adbms.c
 *
 *  Created on: Sep 16, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Api_Adbms.h"

/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define ADBMS_NORMAL_FIL	0.7
#define ADBMS_SLOW_FIL      0.9
#define ADBMS_OpenDiag		(2000u / 2u)

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
CLASSB static Adbms6815_Data Adbms6815Data[DEF_MAX_NUM_OF_BMIC];

CLASSB static bool			Adbms_FirstReadComplete;
CLASSB static uint8			DischargeRes_CheckCnt;
CLASSB static uint8 		Adbms_Read_Mode;
CLASSB static uint16		Balance_Resister;
CLASSB static MODULEDATA	ModuleData[DEF_MAX_NUM_OF_BMIC];
CLASSB static uint16		V_CellMin;
CLASSB static uint16		V_CellMax;
CLASSB static uint8			V_CellModNoMin;
CLASSB static uint8			V_CellModNoMax;
CLASSB static uint8			V_CellCellNoMin;
CLASSB static uint8			V_CellCellNoMax;
CLASSB static uint16		V_CellSum;
CLASSB static uint16		V_CellAvg;
CLASSB static uint16		V_CellDev;
CLASSB static sint16		T_CellMin;
CLASSB static sint16 		T_CellMax;
CLASSB static uint8			T_CellModNoMin;
CLASSB static uint8			T_CellModNoMax;
CLASSB static uint8			T_CellCellNoMax;
CLASSB static uint8			T_CellCellNoMin;
CLASSB static sint16 		T_CellAvg;
CLASSB static sint16 		T_CellDev;
CLASSB static uint16		V_ModMax;
CLASSB static uint16		V_ModMin;
CLASSB static uint16		V_ModDev;
static uint16				Fan_On_State1;
static uint16				Fan_On_State2;
static uint8				Fan_On_Num;
static uint16				BalanceCheck;


const uint16 Adbms_Temp_Table[34u] = {
//  -40 	-35 	-30 	-25 	-20 	-15 	-10 	-5 	    0 	    5 
    28541u, 28103u, 27568u, 26925u, 26165u, 25287u, 24286u, 23165u, 21940u, 20631u, 
//  10 	    15 	    20 	    25 	    30 	    35 	    40 	    45 	    50 	    55 
    19257u, 17841u, 16413u, 15000u, 13620u, 12299u, 11053u, 9889u,  8815u, 	7835u, 
//  60 	    65 	    70 	    75 	    80 	    85 	    90 	    95 	    100 	105 
    6948u, 	6164u, 	5465u, 	4842u, 	4290u, 	3804u, 	3376u, 	2996u, 	2662u, 	2371u, 
//  110 	115 	120 	125 						
    2114u, 	1888u, 	1688u, 	1513u
};


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
void Adbms_Measurement(void);
void Adbms_Sub_Measurement(void);
void Adbms_OrganizeCellVolt(void);
void Adbms_SelectCellMaxMinVolt(void);
void Adbms_CalculationCellAvgVolt(void);
void Adbms_CalculationCellDevVolt(void);
void Adbms_OrganizeCellTemp(void);
void Adbms_SelectCellMaxMinTemp(void);
void Adbms_CalculationCellAvgTemp(void);
void Adbms_CalculationCellDevTemp(void);
void Adbms_CalculationModuleVolt(void);
void Adbms_SelectModuleMaxMinVolt(void);
void Adbms_CalculationModuleDevVolt(void);
void Adbms_CalculationBoardTemp(void);
void Adbms_UpdateFanState(void);
void Adbms_UpdateCellBalancingStatus(void);
sint16  Adbms_AnalogToBoardTempConversion(uint16 adv);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
void Adbms6815_Init_Api(void)
{
	Adbms6815_SetDefaultConfig(Adbms6815Data);
}


#if 1u
/*---------------------------------------------------------------------*/
/**
 * @brief	Measure ADBMS data
 * @param 	void
 * @return	void
*/
void Adbms_Measurement(void)
{
    CLASSB static uint8   ADM_Seq;
	CLASSB static uint8   Diag_code;
	CLASSB static uint16  Counter_1Min;
	CLASSB static bool	DischargeRes_CheckFlag;
	CLASSB static uint8	count1;
	CLASSB static uint8	count2;
	CLASSB static uint8	count3;
	sint16 I_PackMeas;

	I_PackMeas = Curr_Get_I_PackMeas();

	#ifdef Test_Adbms_Measurement
	I_PackMeas = 10u;
	BalanceCheck = 0u;
	#endif

	if ((Counter_1Min++ >= (ADBMS_OpenDiag / ADBMS_READ_PERIOD)) && (ADM_Seq == 19u))
	{
		Counter_1Min = 0u;
		DischargeRes_CheckFlag = TRUE;
	}

	if(DischargeRes_CheckFlag == FALSE)
	{
        switch(ADM_Seq)
        {
            case 0u:     
                Adbms6815_WriteConfig_A(Adbms6815Data);		/* Set config ADBMS6815 */
    			ADM_Seq++;
                break;

    		case 1u:     
                Adbms6815_WriteConfig_B(Adbms6815Data);
				Balance_Resister = (uint16)Adbms_CheckCellBalanceR(Adbms6815Data);
    			ADM_Seq++;
                break;

    		case 2u:
    			Adbms6815_ReadConfig_A(Adbms6815Data);		/* Read config ADBMS6815 */
    			ADM_Seq++;
                break;
    		
    		case 3u:
    			Adbms6815_ReadConfig_B(Adbms6815Data);		
    			ADM_Seq++;
                break;
    			
            case 4u:     
    			Adbms6815_CellConvert();	/* Converted command of Cell Voltage */
    			ADM_Seq++;
                break;

            case 5u:     /* Read command of Cell Voltage */
                Adbms6815_ReadCellVolt(Adbms6815Data, 0u);
    			ADM_Seq++;
    			break;

    		case 6u:
    			Adbms6815_ReadCellVolt(Adbms6815Data, 1u);		
    			ADM_Seq++;
                break;

    		case 7u:
                Adbms6815_ReadCellVolt(Adbms6815Data, 2u);
    			ADM_Seq++;
    			break;

    		case 8u:
    			Adbms6815_ReadCellVolt(Adbms6815Data, 3u);
    			ADM_Seq++;
    			break;

    		case 9u:
    			ADM_Seq++;
    			break;

    		case 10u: 
				ADM_Seq++; 
				break;
				
    		case 11u: 
				Adbms6815_AuxConvert();	/* Converted command of GPIO Input Voltage */  
				ADM_Seq++; 
				break;

            case 12u:     /* Read command of GPIO Input Voltage */
    			Adbms6815_ReadAuxVolt(Adbms6815Data, 0u);
    			ADM_Seq++;
    			break;

    		case 13u:
    			Adbms6815_ReadAuxVolt(Adbms6815Data, 1u);
    			ADM_Seq++;
    			break;

    		case 14u:
    			Adbms6815_ReadAuxVolt(Adbms6815Data, 2u);
    			ADM_Seq++;
    			break;

    		case 15u:
    			Adbms6815_StatusConvert();					/* Convert status */
    			ADM_Seq++;
                break;

            case 16u:     /* Status Read*/
                Adbms6815_ReadStatus(Adbms6815Data, 0u);
    			ADM_Seq++;
    			break;

    		case 17u:
    			Adbms6815_ReadStatus(Adbms6815Data, 1u);
    			ADM_Seq++;
    			break;

    		case 18u:
    			Adbms6815_ReadStatus(Adbms6815Data, 2u);
    			Adbms_FirstReadComplete = TRUE;
    			ADM_Seq++;
                break;

    		case 19u: 
    			ADM_Seq = 0u; 
    			Adbms_Read_Mode = 1u; 
    			break;
    			
            default:
                break;
        }
	}
	else if (DischargeRes_CheckFlag == TRUE)
	{		
    	switch(Diag_code)
        {
            case 0u:     
                if (Adbms6815_CheckCellOpenWire(Adbms6815Data) == TRUE)
    			{	
    				Diag_code++;
    			}
    			break;

    		case 1u:
    			if (Adbms6815_CheckAuxOpenWire(Adbms6815Data) == TRUE)
    			{
    		        Diag_code++;				
    			}
    			break;

    		case 2u:	
    			count1++;

    			if(count1 == 11u)
    			{
    				count1 = 0u;
    				Adbms6815_AuxConvert();
    				DischargeRes_CheckFlag = FALSE;
    				Diag_code++;
    			}
    			break;

    		case 3u:
    			count2++;
    				
    				if(count2 == 2u)
    				{
    					count2 = 0u;
    					Diag_code++;
    				}
    			break;


    		case 4u:
    			if ((I_PackMeas 	> (20 * (-1)))
    	  			&& (I_PackMeas 	< (20       ))
    	  			&& (BalanceCheck == 0u))			 
    			{
    				if (Adbms6815_CheckDischargeResistor(Adbms6815Data) == TRUE)
    				{
    					DischargeRes_CheckCnt++;
    					DischargeRes_CheckFlag = FALSE;
    					Diag_code = 0u;
    				}
    			}
    				
    			else 
    			{
    				count3++;
    				
    				if(count3 == 18u)
    				{
    					count3 = 0u;
    					DischargeRes_CheckFlag = FALSE;
    					Diag_code = 0u;
    				}
    			}
    			break;	

    		default:
    			break;
    	}
	}
	else
	{

	}	
}

/*---------------------------------------------------------------------*/
/**
 * @brief	Measure Sub ADBMS data
 * @param 	void
 * @return	void
*/
void Adbms_Sub_Measurement(void)
{
    CLASSB static uint8   ADM_Seq;

    switch(ADM_Seq)
    {
        case 0u:     
            Adbms6815_Sub_WriteConfig_A(Adbms6815Data);		/* Set config ADBMS6815 */
			ADM_Seq++;
            break;

		case 1u:     
            Adbms6815_Sub_WriteConfig_B(Adbms6815Data);
			Balance_Resister = (uint16)Adbms_CheckCellBalanceR(Adbms6815Data);
			ADM_Seq++;
            break;

		case 2u:     
            Adbms6815_Sub_ReadConfig_A(Adbms6815Data);		/* Set config ADBMS6815 */
			ADM_Seq++;
            break;

		case 3u:     
            Adbms6815_Sub_ReadConfig_B(Adbms6815Data);		
			ADM_Seq++;
            break;

        case 4u:     /* Converted command of Cell Voltage */
			Adbms6815_Sub_CellConvert();
			ADM_Seq++;
            break;

        case 5u:     /* Read command of Cell Voltage */
            Adbms6815_Sub_ReadCellVolt(Adbms6815Data, 0u);
			ADM_Seq++;
			break;

		case 6u:
			Adbms6815_Sub_ReadCellVolt(Adbms6815Data, 1u);					  
			ADM_Seq++;
            break;

		case 7u:
            Adbms6815_Sub_ReadCellVolt(Adbms6815Data, 2u);
			ADM_Seq++;
			break;

		case 8u:
			Adbms6815_Sub_ReadCellVolt(Adbms6815Data, 3u);
			ADM_Seq++;
			break;

		case 9u:
			ADM_Seq++;
			break;
			
		case 10u: 
			ADM_Seq++; 
			break;
			
		case 11u: 
			Adbms6815_Sub_AuxConvert();	/* Converted command of GPIO Input Voltage */
			ADM_Seq++;
			break;

        case 12u:     
			Adbms6815_Sub_ReadAuxVolt(Adbms6815Data, 0u);	/* Read command of GPIO Input Voltage */
			ADM_Seq++;
			break;

		case 13u:
			Adbms6815_Sub_ReadAuxVolt(Adbms6815Data, 1u);
			ADM_Seq++;
			break;

		case 14u:
			Adbms6815_Sub_ReadAuxVolt(Adbms6815Data, 2u);
			ADM_Seq++;
			break;

		case 15u:
			Adbms6815_Sub_StatusConvert();					/* Convert status */
			ADM_Seq++;
            break;

        case 16u:     
            Adbms6815_Sub_ReadStatus(Adbms6815Data, 0u);		/* Status Read*/
			ADM_Seq++;
			break;

		case 17u:
			Adbms6815_Sub_ReadStatus(Adbms6815Data, 1u);
			ADM_Seq++;
			break;

		case 18u:
			Adbms6815_Sub_ReadStatus(Adbms6815Data, 2u);
			Adbms_FirstReadComplete = TRUE;
			ADM_Seq++;
            break;

		case 19u: 
			ADM_Seq = 0u; 
			Adbms_Read_Mode = 0u; 
			break;


        default:
            break;
    }
    
}

#endif


/*---------------------------------------------------------------------*/
/**
 * @brief   ADBMS data Read TASK
 * @param 	void
 * @return	void
*/
void Adbms_Read_Task(void)
{
	#ifdef Test_Adbms_Read_Task
	Adbms_Read_Mode = 1u;
	#endif

	switch(Adbms_Read_Mode)
    {
        case 0u:     
            Adbms_Measurement();
            break;

		case 1u:     
            Adbms_Sub_Measurement();
            break;

		default:
            break;
    }

}


/*---------------------------------------------------------------------*/
/**
 * @brief   ADBMS data Calculation TASK
 * @param 	void
 * @return	void
*/
void Adbms_Calculation_Task(void)
{
	if (Adbms_FirstReadComplete == TRUE)
	{
		Adbms_OrganizeCellVolt();
		Adbms_SelectCellMaxMinVolt();
		Adbms_CalculationCellAvgVolt();
		Adbms_CalculationCellDevVolt();

		Adbms_OrganizeCellTemp();
		Adbms_SelectCellMaxMinTemp();
		Adbms_CalculationCellAvgTemp();
		Adbms_CalculationCellDevTemp();

		Adbms_CalculationModuleVolt();
		Adbms_SelectModuleMaxMinVolt();
		Adbms_CalculationModuleDevVolt();

		Adbms_CalculationBoardTemp();
		Adbms_UpdateFanState();
		Adbms_UpdateCellBalancingStatus();
	}
		
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Organize Cell voltage from ADBMS data
 * @param 	void
 * @return	void
*/
void Adbms_OrganizeCellVolt(void)
{
	uint8 bmic_num;
	uint8 cell_num;
	CLASSB static uint8 Bcount;
	uint8 Bcounter;
	uint16 NumOf_Bmic;
	uint16 NumOfCellinModule;
		

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfCellinModule = Parameter_Get_NumOfCellinModule();
	
	Bcounter = (uint8)Balance_Resister;
	 
	if(Bcounter == 0u)
	{
		if(Bcount < 100u)
		{
			Bcount++;
		}
	}

	else
	{
		Bcount = 0u;
	}

	if(Bcount >= 5u)
	{
		#ifdef Test_Adbms_OrganizeCellVolt
		for(cell_num = 0u; cell_num < 4u; cell_num++)
		{
			Adbms6815Data[0].CVR[cell_num] = 33000u;
		}

		Adbms6815Data[0].CVR[4u] = 20u;
		
		for(cell_num = 4u; cell_num < NumOfCellinModule; cell_num++)
		{
			Adbms6815Data[0].CVR[cell_num+1u] = 33000u;
		}
		
		#else

		#endif
		
		for(bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
		{
			for(cell_num = 0u; cell_num < 4u; cell_num++)
			{
				ModuleData[bmic_num].CellVolt[cell_num] = Adbms6815Data[bmic_num].CVR[cell_num] / 10u;
			}

			for(cell_num = 4u; cell_num < NumOfCellinModule; cell_num++)
			{
				ModuleData[bmic_num].CellVolt[cell_num] = Adbms6815Data[bmic_num].CVR[cell_num+1u] / 10u;
			}

			ModuleData[bmic_num].BusbarVolt = Adbms6815Data[bmic_num].CVR[4] / 10u;
		}
	}
	
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Select Max & Min value of Cell voltage 
 * @param 	void
 * @return	void
*/
void Adbms_SelectCellMaxMinVolt(void)
{
	uint8   bmic_num;
	uint8	 cell_num;
	uint16  max_cellvolt_inPack;
	uint16	 min_cellvolt_inPack;
	uint8   max_cellvolt_inPack_No;
	uint8	 min_cellvolt_inPack_No;
	uint8   max_cellvolt_inPack_ModuleNo;
	uint8 	 min_cellvolt_inPack_ModuleNo;
	bool allow_changeValue = FALSE;
	uint16 NumOf_Bmic;
	uint16 NumOfCellinModule;
		

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfCellinModule = Parameter_Get_NumOfCellinModule();

	max_cellvolt_inPack		   	 = 0x0000u;
	min_cellvolt_inPack		   	 = 0xFFFFu;
	max_cellvolt_inPack_No	   	 = 0u;
	min_cellvolt_inPack_No	   	 = 0u;
	max_cellvolt_inPack_ModuleNo = 0u;
	min_cellvolt_inPack_ModuleNo = 0u;

	#ifdef Test_Adbms_SelectCellMaxMinVolt
	for(bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
    {
		for(cell_num = 0u; cell_num < NumOfCellinModule; cell_num++)
        {
			ModuleData[bmic_num].CellVolt[cell_num] = 3300u;
		}
	}
	ModuleData[6u].CellVolt[6u] = 3280u;
	ModuleData[7u].CellVolt[7u] = 3320u;
	#else

	#endif

	for(bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
    {
		uint16	max_cellvolt_inModule 		   = 0x0000u;
		uint16 min_cellvolt_inModule 		   = 0xFFFFu;
		uint8 max_cellvolt_inModule_No 	   = 0u;
		uint8 min_cellvolt_inModule_No 	   = 0u;
		uint8 max_cellvolt_inModule_ModuleNo = 0u;
		uint8 min_cellvolt_inModule_ModuleNo = 0u;

		for(cell_num = 0u; cell_num < NumOfCellinModule; cell_num++)
        {
            if(ModuleData[bmic_num].CellVolt[cell_num] > max_cellvolt_inModule)
            {
                max_cellvolt_inModule 			= ModuleData[bmic_num].CellVolt[cell_num];
				max_cellvolt_inModule_ModuleNo 	= bmic_num;
				max_cellvolt_inModule_No 		= cell_num;
            }
			else
			{

			}
            if(ModuleData[bmic_num].CellVolt[cell_num] < min_cellvolt_inModule)
			{
                min_cellvolt_inModule 			= ModuleData[bmic_num].CellVolt[cell_num];
				min_cellvolt_inModule_ModuleNo 	= bmic_num;
				min_cellvolt_inModule_No 		= cell_num;
            }
			else
			{

			}
        }

			allow_changeValue = TRUE;

	        ModuleData[bmic_num].CellMaxVolt = max_cellvolt_inModule;
	        ModuleData[bmic_num].CellMinVolt = min_cellvolt_inModule;
			
			if (max_cellvolt_inModule > max_cellvolt_inPack)
			{
				max_cellvolt_inPack				= max_cellvolt_inModule;
				max_cellvolt_inPack_No			= max_cellvolt_inModule_No;
				max_cellvolt_inPack_ModuleNo	= max_cellvolt_inModule_ModuleNo;
			}
			else
			{

			}
			if (min_cellvolt_inModule < min_cellvolt_inPack)
			{
				min_cellvolt_inPack				= min_cellvolt_inModule;
				min_cellvolt_inPack_No			= min_cellvolt_inModule_No;
				min_cellvolt_inPack_ModuleNo	= min_cellvolt_inModule_ModuleNo;
			}
			else
			{

			}
		
	}

	if (allow_changeValue == TRUE)
	{
		V_CellMax 	  	= max_cellvolt_inPack;
		V_CellCellNoMax = max_cellvolt_inPack_No;
		V_CellModNoMax  = max_cellvolt_inPack_ModuleNo;
		V_CellMin	 	= min_cellvolt_inPack;
		V_CellCellNoMin = min_cellvolt_inPack_No;
		V_CellModNoMin  = min_cellvolt_inPack_ModuleNo;
	}
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Calculate Average Cell voltage 
 * @param 	void
 * @return	void
*/
void Adbms_CalculationCellAvgVolt(void)
{
	uint8  bmic_num;
	uint8  cell_num;
	uint32 sum_cellvolt_inPack;
	uint16 NumOf_Bmic;
	uint16 NumOfCellinModule;
		

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfCellinModule = Parameter_Get_NumOfCellinModule();

	sum_cellvolt_inPack = 0;

	for(bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
    {
		uint32 sum_cellvolt_inModule = 0u;

		for(cell_num = 0u; cell_num < NumOfCellinModule; cell_num++)
        {   
			sum_cellvolt_inModule += ModuleData[bmic_num].CellVolt[cell_num];
        }

		sum_cellvolt_inPack += sum_cellvolt_inModule;

		V_CellSum = (uint16)(sum_cellvolt_inPack / 100u);
		
        ModuleData[bmic_num].CellAvgVolt = (uint16)(sum_cellvolt_inModule / (uint32)NumOfCellinModule);
	}

	V_CellAvg = (uint16)(sum_cellvolt_inPack / ((uint32)NumOf_Bmic * (uint32)NumOfCellinModule));
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Calculate cell voltage deviation
 * @param 	void
 * @return	void
*/
void Adbms_CalculationCellDevVolt(void)
{
	uint8 bmic_num;
	uint16 NumOf_Bmic;
		
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		ModuleData[bmic_num].CellDevVolt = 
			ModuleData[bmic_num].CellMaxVolt - ModuleData[bmic_num].CellMinVolt;
	}

	V_CellDev = V_CellMax - V_CellMin;
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Organize Cell Temperature from ADBMS data
 * @param 	void
 * @return	void
*/
void Adbms_OrganizeCellTemp(void)
{
	uint8 bmic_num;
	uint8 temp_num;
	CLASSB static uint8 seq;
	CLASSB static uint8 count1;
	CLASSB static uint8 count2;
	uint16 NumOf_Bmic;
	uint16 NumOfTempinModule;
		

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfTempinModule = Parameter_Get_NumOfTempinModule();

	 switch(seq)
    {
        case 0u: 
			for(bmic_num = 0u; bmic_num < 21u; bmic_num++)
			{
				if(count1 <= 20u)
				{
					for(temp_num = 0u; temp_num < NumOfTempinModule; temp_num++)
			        {
						ModuleData[bmic_num].CellTemp[temp_num] 
							= Adbms_AnalogToBoardTempConversion(Adbms6815Data[bmic_num].GV[temp_num]);
					}
					count1++;
				}

				else
				{
					for(temp_num = 0u; temp_num < NumOfTempinModule; temp_num++)
			        {
						float CellTemp = 
							FILTER(ModuleData[bmic_num].CellTemp[temp_num],
								   Adbms_AnalogToBoardTempConversion(Adbms6815Data[bmic_num].GV[temp_num]),
								   ADBMS_NORMAL_FIL);

						ModuleData[bmic_num].CellTemp[temp_num] = (sint16)CellTemp;
					}
				}
			}
			seq++;
			break;
			
		case 1u:	
			for(bmic_num = 21u; bmic_num < NumOf_Bmic; bmic_num++)
			{
				if(count2 <= 20u)
				{
					for(temp_num = 0u; temp_num < NumOfTempinModule; temp_num++)
			        {
						ModuleData[bmic_num].CellTemp[temp_num] 
							= Adbms_AnalogToBoardTempConversion(Adbms6815Data[bmic_num].GV[temp_num]);
					}
					count2++;
				}

				else
				{
					for(temp_num = 0u; temp_num < NumOfTempinModule; temp_num++)
			        {
						float CellTemp = 
							FILTER(ModuleData[bmic_num].CellTemp[temp_num],
								   Adbms_AnalogToBoardTempConversion(Adbms6815Data[bmic_num].GV[temp_num]),
								   ADBMS_NORMAL_FIL);
						
						ModuleData[bmic_num].CellTemp[temp_num] = (sint16)CellTemp;
					}
				}
			}
			seq = 0u;
			break;

        default:
            break;
	 }
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Select Max & Min value of Cell Temperature 
 * @param 	void
 * @return	void
*/
void Adbms_SelectCellMaxMinTemp(void)
{
	uint8	 bmic_num;
	uint8	 temp_num;
	sint16  max_celltemp_inPack;
	sint16  min_celltemp_inPack;
	uint8   max_celltemp_inPack_No;
	uint8 	 min_celltemp_inPack_No;
	uint8   max_celltemp_inPack_ModuleNo; 	 
	uint8   min_celltemp_inPack_ModuleNo;
	uint16 NumOf_Bmic;
	uint16 NumOfTempinModule;
		

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfTempinModule = Parameter_Get_NumOfTempinModule();

	max_celltemp_inPack		   	 = -400;
	min_celltemp_inPack		   	 = 1250;
	max_celltemp_inPack_No	   	 = 0u;
	min_celltemp_inPack_No	   	 = 0u;
	max_celltemp_inPack_ModuleNo = 0u;
	min_celltemp_inPack_ModuleNo = 0u;

	for(bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		sint16	max_celltemp_inModule 		   = -400;
		sint16 min_celltemp_inModule          = 1250;
		uint8	max_celltemp_inModule_No 	   = 0u;
		uint8	max_celltemp_inModule_ModuleNo = 0u;

		for(temp_num = 0u; temp_num < NumOfTempinModule; temp_num++)
		{
			if(ModuleData[bmic_num].CellTemp[temp_num] > max_celltemp_inModule)
			{
				max_celltemp_inModule	= ModuleData[bmic_num].CellTemp[temp_num];
				max_celltemp_inModule_ModuleNo 	= bmic_num;
				max_celltemp_inModule_No 		= temp_num;
			}
			else
			{

			}
			if(ModuleData[bmic_num].CellTemp[temp_num] < min_celltemp_inModule)
			{
				min_celltemp_inModule = ModuleData[bmic_num].CellTemp[temp_num];
				max_celltemp_inModule_ModuleNo 	= bmic_num;
				max_celltemp_inModule_No 		= temp_num;
			}
			else
			{

			}
		}

		ModuleData[bmic_num].CellMaxTemp = max_celltemp_inModule;
		ModuleData[bmic_num].CellMinTemp = min_celltemp_inModule;
		
		if (max_celltemp_inModule > max_celltemp_inPack)
		{
			max_celltemp_inPack 			= max_celltemp_inModule;
			max_celltemp_inPack_No			= max_celltemp_inModule_No;
			max_celltemp_inPack_ModuleNo	= max_celltemp_inModule_ModuleNo;
		}
		else
		{

		}
		if (min_celltemp_inModule < min_celltemp_inPack)
		{
			uint8	min_celltemp_inModule_No 	= 0u;
			uint8	min_celltemp_inModule_ModuleNo = 0u;
			
			min_celltemp_inPack 			= min_celltemp_inModule;
			min_celltemp_inPack_No			= min_celltemp_inModule_No;
			min_celltemp_inPack_ModuleNo	= min_celltemp_inModule_ModuleNo;
		}
		else
		{

		}
	}

	T_CellMax 	 	= max_celltemp_inPack;
	T_CellCellNoMax = max_celltemp_inPack_No;
	T_CellModNoMax  = max_celltemp_inPack_ModuleNo;
	T_CellMin		= min_celltemp_inPack;
	T_CellCellNoMin = min_celltemp_inPack_No;
	T_CellModNoMin  = min_celltemp_inPack_ModuleNo;
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Calculate Average Cell temperature 
 * @param 	void
 * @return	void
*/
void Adbms_CalculationCellAvgTemp(void)
{
	uint8	bmic_num;
	uint8	temp_num;
	sint32 sum_celltemp_inPack;
	uint32 udata;
	uint16 NumOf_Bmic;
	uint16 NumOfTempinModule;
		

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfTempinModule = Parameter_Get_NumOfTempinModule();

	sum_celltemp_inPack = 0;
	
	for(bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		sint16 sum_celltemp_inModule = 0;

		for(temp_num = 0u; temp_num < NumOfTempinModule; temp_num++)
		{
			sum_celltemp_inModule += ModuleData[bmic_num].CellTemp[temp_num];
		}

		sum_celltemp_inPack += sum_celltemp_inModule;
		
		ModuleData[bmic_num].CellAvgTemp = sum_celltemp_inModule / (sint16)NumOfTempinModule;
	}

	udata = (uint32)sum_celltemp_inPack / ((uint32)NumOf_Bmic * (uint32)NumOfTempinModule);
	T_CellAvg = (sint16)(udata);
}



/*---------------------------------------------------------------------*/
/**
 * @brief   Calculate cell temperature deviation
 * @param 	void
 * @return	void
*/
void Adbms_CalculationCellDevTemp(void)
{
	uint8 bmic_num;
	uint16 NumOf_Bmic;

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		ModuleData[bmic_num].CellDevTemp 
			= ModuleData[bmic_num].CellMaxTemp - ModuleData[bmic_num].CellMinTemp;
	}

	T_CellDev = T_CellMax - T_CellMin;
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Calculate Module voltage
 * @param 	void
 * @return	void
*/
void Adbms_CalculationModuleVolt(void)
{
	uint8 bmic_num;
	uint16 NumOf_Bmic;

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	
	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		float ModuleVolt = 
			FILTER(ModuleData[bmic_num].ModuleVolt, Adbms6815Data[bmic_num].SC, ADBMS_SLOW_FIL);

		ModuleData[bmic_num].ModuleVolt = (uint16)ModuleVolt;
	}
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Select Max & Min value of Module voltage
 * @param 	void
 * @return	void
*/
void Adbms_SelectModuleMaxMinVolt(void)
{
	uint8  bmic_num;
	uint16 NumOf_Bmic;

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	V_ModMax = 0x0000u;
	V_ModMin = 0xFFFFu;

	for(bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{

		if (ModuleData[bmic_num].ModuleVolt > V_ModMax)
		{
			V_ModMax	= ModuleData[bmic_num].ModuleVolt;
		}
		else
		{

		}
		if (ModuleData[bmic_num].ModuleVolt < V_ModMin)
		{
			V_ModMin	= ModuleData[bmic_num].ModuleVolt;
		}
		else
		{

		}
	}
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Calculate Module voltage deviation
 * @param 	void
 * @return	void
*/
void Adbms_CalculationModuleDevVolt(void)
{
	V_ModDev = V_ModMax - V_ModMin;
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Calculate Board Temperature
 * @param 	void
 * @return	void
*/
void Adbms_CalculationBoardTemp(void)
{
	uint8 bmic_num;
	static uint8 seq = 0u;
	uint16 NumOf_Bmic;

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	 switch(seq)
    {
        case 0u: 
			for (bmic_num = 0u; bmic_num < 21u; bmic_num++)
			{
				float BoardTemp;
				
				if((bmic_num % 2u) == 0u)
				{ 
					BoardTemp = 
					FILTER(ModuleData[bmic_num].BoardTemp,
						   Adbms_AnalogToBoardTempConversion(Adbms6815Data[bmic_num].GV[5u]),
						   ADBMS_NORMAL_FIL);

					ModuleData[bmic_num].BoardTemp = (sint16)BoardTemp;
				}

				else
				{
					BoardTemp =
					FILTER(ModuleData[bmic_num].BoardTemp,
					       Adbms_AnalogToBoardTempConversion(Adbms6815Data[bmic_num].GV[6u]),
						   ADBMS_NORMAL_FIL);

					ModuleData[bmic_num].BoardTemp = (sint16)BoardTemp;
				}
			}
			seq++;
			break;

		case 1u: 
			for (bmic_num = 21u; bmic_num < NumOf_Bmic; bmic_num++)
			{
				float BoardTemp;
				
				if((bmic_num % 2u) == 0u)
				{				 
					BoardTemp = 
					FILTER(ModuleData[bmic_num].BoardTemp,
						Adbms_AnalogToBoardTempConversion(Adbms6815Data[bmic_num].GV[5u]),
						ADBMS_NORMAL_FIL);

					ModuleData[bmic_num].BoardTemp = (sint16)BoardTemp;
				}

				else
				{				
					BoardTemp = 
					FILTER(ModuleData[bmic_num].BoardTemp,
						Adbms_AnalogToBoardTempConversion(Adbms6815Data[bmic_num].GV[6u]),
						ADBMS_NORMAL_FIL);
			
					ModuleData[bmic_num].BoardTemp = (sint16)BoardTemp;	
				}
			}
			seq = 0u;
			break;

        default:
            break;

	}
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Update FAN State
 * @param 	void
 * @return	void
*/
void Adbms_UpdateFanState(void)
{
	uint16 bmic_num;
	uint8 counter = 0u;
	uint16 NumOf_Bmic;

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	#ifdef Test_Adbms_UpdateFanState
	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		Adbms6815Data[bmic_num].GV[6u] = 30000u;
	}
	Adbms6815Data[4u].GV[6u] = 0u;
	Adbms6815Data[5u].GV[6u] = 0u;
	#endif
	
	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		if((bmic_num % 2u) == 0u)
		{
			if(Adbms6815Data[bmic_num].GV[6u] > 20000u)
			{
				ModuleData[bmic_num].FANState = (uint16)TRUE;
			}
			else 
			{
				ModuleData[bmic_num].FANState = (uint16)FALSE;
			}
		
			if(ModuleData[bmic_num].FANState == (uint16)TRUE)
			{
				if(bmic_num < 32u)
				{
					Fan_On_State1 |= ((uint16)1u << ((uint16)bmic_num / 2u));
					counter++;
				}

				else
				{
					Fan_On_State2 |= ((uint16)1u << (((uint16)bmic_num - 32u) / 2u));
					counter++;
				}
			}

			else 
			{
				if(bmic_num < 32u)
				{
					Fan_On_State1 &= ~((uint16)1u << ((uint16)bmic_num / 2u));
				}

				else
				{
					Fan_On_State2 &= ~((uint16)1u << (((uint16)bmic_num - 32u) / 2u));
				}
			}

		}
	}

	Fan_On_Num = counter;
	
}


/*---------------------------------------------------------------------*/
/**
 * @brief   Update Cell balancing status
 * @param 	void
 * @return	void
*/
void Adbms_UpdateCellBalancingStatus(void)
{
	uint8 bmic_num;
	uint8 i;
	uint8 Bal_Output[2];
	uint16 BalnceCount[DEF_MAX_NUM_OF_BMIC];
	uint16 NumOf_Bmic;

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	#ifdef Test_Adbms_UpdateCellBalancingStatus
	Adbms6815Data[0u].ReadConfig_CfgB_Reg5 = 0xAu;
	Adbms6815Data[0u].ReadConfig_CfgB_Reg4 = 0xA5u;
	#endif

	for(i = 0; i < NumOf_Bmic; i++)
    {
        Bal_Output[0] = Adbms6815Data[i].ReadConfig_CfgB_Reg5 & 0x0Fu;
        Bal_Output[1] = Adbms6815Data[i].ReadConfig_CfgB_Reg4;

        BalnceCount[i] = ((uint16)Bal_Output[0] << 8) + (uint16)Bal_Output[1];
   	}   


	
	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		ModuleData[bmic_num].BalanceState 
			= (BalnceCount[bmic_num] & 0x000Fu) + ((BalnceCount[bmic_num] & 0x01E0u) >> 1u);
	}
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Convert analoge input data to temperature
* @param 	adv - Analoge input
* @return 	sint16 - temperature
*/
sint16 Adbms_AnalogToBoardTempConversion(uint16 adv)
{
	uint16   i;
	uint16	  temp = 0u;
	sint16   rv = 0;
	uint16 now_temp;
	uint16	d;
	uint16 f;

	#ifdef Test_Adbms_AnalogToBoardTempConversion
	adv = 15000u;
	#else

	#endif
	
	if		(adv > Adbms_Temp_Table[0u])
	{
		rv = -400;
	}
	else if (adv < Adbms_Temp_Table[33u])		
	{
		rv = 1250;
	}
	else
	{
		for (i = 0u; i < 34u; i++)										
		{
			if(adv > Adbms_Temp_Table[i])
			{				
				temp = i;
				break;
			}
		}

		if(temp == 0u)
		{
			rv = -400;
		}

		else
		{
			f = (Adbms_Temp_Table[(temp - 1u)] - Adbms_Temp_Table[temp]) / 50u;

			for(i = 0u; i < 50u; i++)
			{
				d = i * f;
				now_temp = Adbms_Temp_Table[(temp - 1u)] - d;
				
				if(i == 49u)
				{
				    rv = -400 + ((sint16)temp * 50) - 1;
				}
				else if(adv > now_temp)
				{
					rv = -400 + (((sint16)temp - 1) * 50) + (sint16)i - 1;
					break;
				}
				else
				{

				}
			}
		}
	}

	return rv;
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS6815's first read state get
* @param	void
* @return 	bool - Adbms_FirstReadComplete state
*/
bool Adbms_FirstReadState(void)
{
    return Adbms_FirstReadComplete;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get RLEC communication counter
* @param	void
* @return	ErrorFlag
*/
bool Adbms_GetCommError_F(void)
{
	uint8	rv;

	rv	= FALSE;

	if (Adbms6815_F_Get6815PEC_ErrorCount() != 0u)
	{
		rv	= TRUE;
	}
	else
	{

	}

	return	(BOOL)rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get RLEC communication counter
* @param	void
* @return	ErrorFlag
*/
bool Adbms_GetCommError_R(void)
{
	uint8	rv;

	rv	= FALSE;

	if (Adbms6815_R_Get6815PEC_ErrorCount() != 0u)
	{
		rv	= TRUE;
	}
	else
	{

	}

	return	(BOOL)rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Reset RLEC communication counter
* @param	void
* @return	void
*/
void Adbms_ResetCommError_F(void)
{
	Adbms6815_F_Set6815PEC_ErrorCount(0u);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Reset RLEC communication counter
* @param	void
* @return	void
*/
void Adbms_ResetCommError_R(void)
{
	Adbms6815_R_Set6815PEC_ErrorCount(0u);
}


/*---------------------------------------------------------------------*/
/**
* @brief	Check if there is Cell V offline error
* @param	void
* @return	void
*/
bool Adbms_GetErrorVCellOffline(void)
{
	uint8 VCellOpenWire_State;
	uint8 bmic_num;
	uint8 cell_num;
	uint8 Result = FALSE;
	uint16 NumOf_Bmic;
	uint16 NumOfCellinModule;
		

	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfCellinModule = Parameter_Get_NumOfCellinModule();

	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		for (cell_num = 0u; cell_num < 4u; cell_num++)
		{
			VCellOpenWire_State = Adbms6815Data[bmic_num].OWD[cell_num];

			if (VCellOpenWire_State != OWD_NO_ERROR)
			{
				Result = TRUE;
			}
		}

		for (cell_num = 4u; cell_num < NumOfCellinModule; cell_num++)
		{
			VCellOpenWire_State = Adbms6815Data[bmic_num].OWD[cell_num + 1u];
			if (VCellOpenWire_State != OWD_NO_ERROR)
			{
				Result = TRUE;
			}
		}
	}

	return (BOOL)Result;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Check if there is Cell T offline error
* @param	void
* @return	void
*/
bool Adbms_GetErrorTCellOffline(void)
{
	uint8 		TCellOpenWire_State;
	uint8 		bmic_num;
	uint8		aux_num;
	bool 	Result = FALSE;
	uint16 NumOf_Bmic;
	uint16 NumOfTempinModule;
		
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfTempinModule = Parameter_Get_NumOfTempinModule();

	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		for (aux_num = 0u; aux_num < NumOfTempinModule; aux_num++)
		{
			TCellOpenWire_State = Adbms6815Data[bmic_num].OAXD[aux_num];
			if (TCellOpenWire_State != 0u)
			{
				Result = TRUE;
			}
		}
	}

	return Result;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Check if there is Cell Discharge Resister error
* @param	void
* @return	void
*/
bool Adbms_GetErrorDischargeResistor(void)
{
	uint8 CellDchResError_State;
	uint8 bmic_num;
	uint8 res_num;
	uint8 Result = FALSE;
	uint16 NumOf_Bmic;
	uint16 NumOfCellinModule;
		
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfCellinModule = Parameter_Get_NumOfCellinModule();

	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		for (res_num = 0u; res_num < 4u; res_num++)
		{
			CellDchResError_State = Adbms6815Data[bmic_num].DischargeR_Diag[res_num];
			if (CellDchResError_State != OWD_NO_ERROR)
			{
				Result = TRUE;
			}
		}

		for (res_num = 4u; res_num < NumOfCellinModule; res_num++)
		{
			CellDchResError_State = Adbms6815Data[bmic_num].DischargeR_Diag[res_num + 1u];
			if (CellDchResError_State != OWD_NO_ERROR)
			{
				Result = TRUE;
			}
		}
	}

	return (BOOL)Result;
}


/*---------------------------------------------------------------------*/
/**
* @brief	Get Check count of Discharge Resister
* @param	void
* @return	uint8 - Check count of Discharge Resister
*/
uint8 Adbms_GetDischargeRes_CheckCnt(void)
{
	return DischargeRes_CheckCnt;
}


/*---------------------------------------------------------------------*/
void Adbms_Set_CellBalanceCmd(uint16* Cmd, Adbms6815_FETCtrl FetCtrl)
{
	Adbms6815_SetCellBalanceStatus(Cmd, Adbms6815Data, FetCtrl);
}


/*---------------------------------------------------------------------*/
uint8 Adbms_Get_CellBalanceStatus(uint16* Status)
{
    uint8 rv = 0u;

    if(Adbms6815_GetCellBalanceStatus(Status, Adbms6815Data) == 1u)
    {
        rv = 1u;
    }

    return rv;
}


/*---------------------------------------------------------------------*/
sint16 Adbms_Get_BoardTemp(uint16 Bmic_No)
{
    return ModuleData[Bmic_No].BoardTemp;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_CellVolt(uint16 Bmic_No, uint8 num)
{
    return ModuleData[Bmic_No].CellVolt[num];
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_CellMaxVolt(uint16 Bmic_No)
{
    return ModuleData[Bmic_No].CellMaxVolt;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_CellMinVolt(uint16 Bmic_No)
{
    return ModuleData[Bmic_No].CellMinVolt;
}


/*---------------------------------------------------------------------*/
sint16 Adbms_Get_CellTemp(uint16 Bmic_No, uint8 num)
{
    return ModuleData[Bmic_No].CellTemp[num];
}


/*---------------------------------------------------------------------*/
sint16 Adbms_Get_CellMaxTemp(uint16 Bmic_No)
{
    return ModuleData[Bmic_No].CellMaxTemp;
}


/*---------------------------------------------------------------------*/
sint16 Adbms_Get_CellMinTemp(uint16 Bmic_No)
{
    return ModuleData[Bmic_No].CellMinTemp;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_V_CellMin(void)
{
    return V_CellMin;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_V_CellMax(void)
{
    return V_CellMax;
}


/*---------------------------------------------------------------------*/
uint8 Adbms_Get_V_CellModNoMin(void)
{
    return V_CellModNoMin;
}


/*---------------------------------------------------------------------*/
uint8 Adbms_Get_V_CellCellNoMin(void)
{
    return V_CellCellNoMin;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_V_CellSum(void)
{
    return V_CellSum;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_V_CellAvg(void)
{
    return V_CellAvg;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_V_CellDev(void)
{
    return V_CellDev;
}


/*---------------------------------------------------------------------*/
sint16 Adbms_Get_T_CellMin(void)
{
    return T_CellMin;
}


/*---------------------------------------------------------------------*/
sint16 Adbms_Get_T_CellMax(void)
{
    return T_CellMax;
}


/*---------------------------------------------------------------------*/
uint8 Adbms_Get_T_CellModNoMax(void)
{
    return T_CellModNoMax;
}


/*---------------------------------------------------------------------*/
uint8 Adbms_Get_T_CellCellNoMax(void)
{
    return T_CellCellNoMax;
}


/*---------------------------------------------------------------------*/
sint16 Adbms_Get_T_CellAvg(void)
{
    return T_CellAvg;
}


/*---------------------------------------------------------------------*/
sint16 Adbms_Get_T_CellDev(void)
{
    return T_CellDev;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_ModuleVolt(uint16 Bmic_No)
{
    return ModuleData[Bmic_No].ModuleVolt;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_V_ModMax(void)
{
    return V_ModMax;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_V_ModMin(void)
{
    return V_ModMin;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_V_ModDev(void)
{
    return V_ModDev;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_Fan_On_State1(void)
{
    return Fan_On_State1;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_Fan_On_State2(void)
{
    return Fan_On_State2;
}


/*---------------------------------------------------------------------*/
uint8 Adbms_Get_Fan_On_Num(void)
{
    return Fan_On_Num;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_BalanceState(uint16 Bmic_No)
{
    return ModuleData[Bmic_No].BalanceState;
}


/*---------------------------------------------------------------------*/
uint16 Adbms_Get_MalfCode(uint16 Bmic_No)
{
    return ModuleData[Bmic_No].MalfCode;
}


/*---------------------------------------------------------------------*/
void Adbms_Set_BalanceCheck(uint16 BCheck)
{
    BalanceCheck = BCheck;
}

/*---------------------------------------------------------------------*/
uint16 Adbms_Get_BusbarVolt(uint16 Bmic_No)
{
    return ModuleData[Bmic_No].BusbarVolt;
}


/* End of L2Api_Adbms.c */
