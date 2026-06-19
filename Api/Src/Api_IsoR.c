/*
 * Api_IsoR.c
 *
 *  Created on: Sep 19, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Api_IsoR.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define PV_RANGE        		15u
#define ISOR_RANGE     			29u
#define ON_WAIT_TIME   			(5u * 1000u / 100u)
#define OFF_WAIT_TIME  			1u 	// (0.1 * 1000 / 100)
#define STABLE_STATE_LMT    	(5u)
#define DETECT_STABLE_REG   	(0u)
#define ISOR_MEAS_PERIOD        (100u)
#define SAMP_MEASURE_PERIOD     (300u / ISOR_MEAS_PERIOD)
#define ISOR_SAMPLING_TIME		9u
#define ISOR_AVERAGE_SHIFT		3u


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/
typedef enum
{
    Iso_Init = 0,
    Iso_P_Meas,
    Iso_N_Meas,
    Iso_Calc,
    Iso_Stop,    
    ISO_Wait_1,
    ISO_Wait_2,
    ISO_Wait_3,
    ISO_Wait_4,
    Iso_P_On,
    Iso_N_On,
    Iso_Init_Meas,
}IsolationSeq;

typedef struct
{
    IsolationSeq    Iso_Meas_Seq;
    IsolationSeq    old_Iso_Meas_Seq;
    bool  			Iso_Command;    
    uint16 			Iso_P_Array;
    uint8  			Iso_P_Index;
    uint16 			Iso_N_Array;
    uint8  			Iso_N_Index;
    uint16 			nowISOR[ISOR_RANGE];
    uint16 			Iso_P;
    uint16 			Iso_N;
    uint16 			Diff_PN;
    uint16 			Iso_Z;
}IsolationVar;

typedef enum
{
	ISO_INIT_P_ON,
	ISO_INIT_P_OFF,
	ISO_INIT_N_ON,
	ISO_INIT_N_OFF,
	ISO_INIT_CALC,
}ISO_INIT_M;


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
CLASSB static IsolationVar 	IsoR_Var;
CLASSB static IsoR_MeasStatus  IsoR_OperationStatus;

CLASSB static bool 	IsoR_MeasureCmd;

CLASSB static uint16	ISO_Resistor;


/**
* @brief Isolation Resistor table
*
*/ 
const uint16	IsoR_LookupTbl[ISOR_RANGE] =
{
	0,    100,	119,  141,  168,  200,  238,  282,  336,  400,	476,  564,  672,  800,	952, 1128, 
	1344, 1600,	1904, 2256,	2688, 3200,	3808, 4512,	5376, 6400,	7616, 9024,	10000
};

/**
* @brief Isolation pack voltage table
*
*/ 
const uint16   IsoPV_Lookup[PV_RANGE] = 
{
    1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000
};

/**
* @brief Isolation raw value table by Pack Voltage
*
*/   
const uint16 IsoR_RawPN_Table[PV_RANGE][ISOR_RANGE] = {
/*100V*/  	{1561u, 1562u, 1562u, 1562u, 1562u, 1563u, 1563u, 1563u, 1564u, 1564u, 1565u, 1565u, 1566u, 1567u, 1568u, 1569u, 
			 1570u, 1572u, 1574u, 1576u, 1578u, 1581u, 1584u, 1587u, 1590u, 1594u, 1599u, 1603u, 1652u},
/*200V*/  	{1452u, 1454u, 1454u, 1454u, 1454u, 1455u, 1455u, 1456u, 1457u, 1458u, 1459u, 1460u, 1461u, 1463u, 1465u, 1467u, 
			 1470u, 1473u, 1477u, 1481u, 1486u, 1491u, 1497u, 1503u, 1510u, 1518u, 1526u, 1535u, 1631u},
/*300V*/  	{1343u, 1345u, 1346u, 1346u, 1347u, 1347u, 1348u, 1349u, 1350u, 1352u, 1353u, 1355u, 1357u, 1360u, 1363u, 1366u, 
			 1370u, 1375u, 1380u, 1386u, 1393u, 1401u, 1410u, 1419u, 1430u, 1441u, 1454u, 1467u, 1611u},
/*400V*/  	{1234u, 1237u, 1237u, 1238u, 1239u, 1240u, 1241u, 1242u, 1243u, 1245u, 1247u, 1250u, 1253u, 1256u, 1260u, 1265u, 
			 1270u, 1276u, 1283u, 1291u, 1300u, 1311u, 1323u, 1335u, 1349u, 1365u, 1381u, 1398u, 1591u},
/*500V*/  	{1125u, 1128u, 1129u, 1130u, 1131u, 1132u, 1133u, 1135u, 1137u, 1139u, 1142u, 1145u, 1148u, 1153u, 1157u, 1163u, 
			 1170u, 1178u, 1186u, 1196u, 1208u, 1221u, 1236u, 1251u, 1269u, 1288u, 1309u, 1330u, 1570u},
/*600V*/  	{1016u, 1020u, 1021u, 1022u, 1023u, 1024u, 1026u, 1028u, 1030u, 1033u, 1036u, 1039u, 1044u, 1049u, 1055u, 1062u, 
			 1070u, 1079u, 1090u, 1101u, 1115u, 1131u, 1149u, 1167u, 1189u, 1212u, 1237u, 1262u, 1550u},
/*700V*/  	{907u,  912u,  913u,  914u,  915u,  917u,  919u,  921u,  923u,  927u,  930u,  934u,  939u,  945u,  952u,  960u,  
			 970u,  980u,  993u,  1007u, 1023u, 1041u, 1062u, 1084u, 1108u, 1135u, 1164u, 1194u, 1530u},
/*800V*/  	{798u,  803u,  804u,  806u,  807u,  809u,  811u,  814u,  817u,  820u,  824u,  829u,  835u,  842u,  850u,  859u, 
			 869u,  882u,  896u,  912u,  930u,  951u,  974u,  1000u, 1028u, 1059u, 1092u, 1126u, 1509u},
/*900V*/  	{688u,  695u,  696u,  698u,  699u,  701u,  704u,  707u,  710u,  714u,  719u,  724u,  731u,  738u,  747u,  757u, 
			 769u,  783u,  799u,  817u,  838u,  861u,  887u,  916u,  948u,  982u,  1020u, 1058u, 1489u},
/*1000V*/ 	{579u,  587u,  588u,  590u,  591u,  594u,  596u,  600u,  603u,  608u,  613u,  619u,  626u,  635u,  645u,  656u,  
			 669u,  685u,  702u,  722u,  745u,  771u,  800u,  832u,  867u,  906u,  947u,  990u,  1468u},
/*1100V*/ 	{470u,  478u,  480u,  482u,  484u,  486u,  489u,  492u,  497u,  502u,  507u,  514u,  522u,  531u,  542u,  554u, 
			 569u,  586u,  606u,  627u,  653u,  681u,  713u,  748u,  787u,  829u,  875u,  921u,  1448u},
/*1200V*/ 	{361u,  370u,  372u,  373u,  376u,  378u,  382u,  385u,  390u,  395u,  402u,  409u,  417u,  428u,  439u,  453u, 
			 469u,  487u,  509u,  532u,  560u,  592u,  626u,  664u,  707u,  753u,  802u,  853u,  1428u},
/*1300V*/ 	{252u,  262u,  263u,  265u,  268u,  271u,  274u,  278u,  283u,  289u,  296u,  304u,  313u,  324u,  337u,  351u, 
			 369u,  389u,  412u,  438u,  468u,  502u,  539u,  580u,  626u,  676u,  730u,  785u,  1407u},
/*1400V*/ 	{143u,  153u,  155u,  157u,  160u,  163u,  167u,  171u,  177u,  183u,  190u,  198u,  209u,  220u,  234u,  250u, 
			 269u,  290u,  315u,  343u,  375u,  412u,  452u,  496u,  546u,  600u,  658u,  717u,  1387u},
/*1500V*/ 	{34u,   45u,   47u,   49u,   52u,   56u,   60u,   64u,   70u,   77u,   84u,   93u,   104u,  117u,  132u,  148u, 
			 169u,  192u,  218u,  248u,  283u,  322u,  365u,  412u,  466u,  524u,  585u,  649u,  1367u},
};


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
void 					IsoR_Measurement(IsolationVar* IsoVar);
bool 					IsoR_Init_Measurement(void);
uint16  					Get_IsoAD_Positive(void);
uint16  					Get_IsoAD_Negative(void);
void 					Calc_ISOR_Table(uint16* nowISORTable, uint16 nowPV);
uint16  					Calc_ISO_R(const uint16* nowISORTable, uint16 Diff_PN);
uint16  					U16_Project_From_Samples(uint16 Sample1, uint16 Sample2, uint16 Sample3, uint8 LinearPeriods, bool bLinearOnly);
uint16	 					InterpolLinear_inU16_outU16(uint16 input, const uint16* table_in, const uint16* table_out, uint8 table_size);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
 * @brief	Isolation Resister Initialization
 * @param 	void
 * @return	BOOL, TRUE - Success, FALSE - X
*/
bool IsoR_Init(void)
{
	Dio_SetFET_ForIsoR_High(OFF);
	Dio_SetFET_ForIsoR_Low(OFF);
	return (BOOL)TRUE;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Isolation Resister Measurement TASK
 * @param 	void
 * @return	void
*/
void IsoR_Measurement_Task(void)
{
	IsoR_Measurement(&IsoR_Var);
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Set Isolation Resister Measurement Command
 * @param 	cmd - command
 * @return	void
*/
void IsoR_SetMeasCommand(bool cmd)
{
	IsoR_MeasureCmd = cmd;
}


/*---------------------------------------------------------------------*/
/**
* @brief Function to determine whether to measure isolation according to system status and isolation command
* @param void
* @return 
*/
bool IsoR_IsMeasurement(void)
{
	bool result;
	SYSTEM_STATE System_State;

	#ifdef Test_IsoR_IsMeasurement_1_On
	System_State = SYS_DRIVE;
	IsoR_MeasureCmd = TRUE;
	#elif defined(Test_IsoR_IsMeasurement_1_Off)
	System_State = SYS_DRIVE;
	IsoR_MeasureCmd = FALSE;
	#elif defined(Test_IsoR_IsMeasurement_2)
	System_State = SYS_IDLE;
	#else
	System_State = ManaMsm_Get_System_State();
	#endif

	if((System_State == SYS_DRIVE) || (System_State == SYS_WAIT)) 
	{
		if(IsoR_MeasureCmd == TRUE)
		{
			result = TRUE;
		}
		else
		{		    
			result = FALSE;
		}
	}
	else
	{
		result = TRUE;
	}

    return (BOOL)result;	
}


/*---------------------------------------------------------------------*/
/**
* @brief Isolation measurement function
* @param IsolationVar
* @return 
*/
void IsoR_Measurement(IsolationVar* IsoVar)
{
#define ISOR_ERR_CNT_LMT    3u

    uint16 temp;
    IsolationVar* m = IsoVar;
    uint16 nowPV;
    CLASSB static uint16 WaitCount;
    CLASSB static uint16 old_val;
	CLASSB static uint16 StableCheckCount;
    CLASSB static bool NotFisrtMeasure;
    CLASSB static uint8 IsoR_ErrCnt;
    uint16 new_val;     
	uint8 High_State;
	uint8 Low_State;

	#ifdef Test_IsoR_Measurement
	m->Iso_Command = TRUE;
	#else
    m->Iso_Command =  IsoR_IsMeasurement();
	#endif

    switch(m->Iso_Meas_Seq)
    {
        case Iso_Init:
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;          
            m->Iso_P_Index = 0;
            m->Iso_N_Index = 0;  

			High_State = Dio_GetFET_ForIsoR_High();
			Low_State = Dio_GetFET_ForIsoR_Low();
            if((High_State != 0u) || (Low_State != 0u))
            {
                Dio_SetFET_ForIsoR_High(OFF);
                Dio_SetFET_ForIsoR_Low(OFF);
            }
            
            IsoR_OperationStatus = ISO_NoOperation;
            
            if(m->Iso_Command == TRUE)
            {
                m->Iso_Meas_Seq = Iso_Init_Meas;
                IsoR_OperationStatus = ISO_Notyet_Process;
            }
            break;
            
        case Iso_Init_Meas:
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
            if(IsoR_Init_Measurement())
            {
                 m->Iso_Meas_Seq = Iso_P_On;
            }
            break;
            
        case Iso_P_On:
			#ifdef Test_IsoR_Measurement_2
			m->Iso_Command = FALSE;
			#endif
				
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
            if(m->Iso_Command == FALSE)
            {
                m->Iso_Meas_Seq = Iso_Stop;
                break;
            }                             
            Dio_SetFET_ForIsoR_High(ON);
            m->Iso_Meas_Seq = ISO_Wait_3;
            IsoR_OperationStatus = ISO_In_Process;
            break;
            
        case ISO_Wait_3:
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
            if(m->Iso_Command == FALSE)
            {
                m->Iso_Meas_Seq = Iso_Stop;
                break;
            }             
            if(WaitCount++ >= (ON_WAIT_TIME-1u))
            {
                m->Iso_Meas_Seq = Iso_P_Meas;
                WaitCount = 0;
                StableCheckCount = 0;
            }
			#ifdef Test_IsoR_Measurement
			new_val = 10u;
			#else
            new_val = Get_IsoAD_Positive();
			#endif
			
            if(MAX(old_val, new_val) - MIN(old_val, new_val) == DETECT_STABLE_REG)
            {
                StableCheckCount++;
                if(StableCheckCount >= STABLE_STATE_LMT)
                {
                    StableCheckCount = 0;
                    m->Iso_Meas_Seq = Iso_P_Meas;
                    WaitCount = 0;                    
                }
            }
            else    
            {
                StableCheckCount = 0;
            }
            old_val = new_val;
            IsoR_OperationStatus = ISO_In_Process;            
            break;
           
        case Iso_P_Meas:     
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
            if(m->Iso_Command == FALSE)
            {
                m->Iso_Meas_Seq = Iso_Stop;
                break;
            }

			#ifdef Test_IsoR_Measurement
			temp = 10u;
			#else
            temp = Get_IsoAD_Positive();
			#endif
            m->Iso_P_Array = temp;
                   
            Dio_SetFET_ForIsoR_High(OFF);
            m->Iso_Meas_Seq = ISO_Wait_1;           
            IsoR_OperationStatus = ISO_In_Process;

            break;
            
        case ISO_Wait_1:
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
            if(m->Iso_Command == FALSE)
            {
                m->Iso_Meas_Seq = Iso_Stop;
                break;
            }             

            if(NotFisrtMeasure != TRUE) 
			{
				m->Iso_Meas_Seq = Iso_N_On;
            }
            else            
			{
				m->Iso_Meas_Seq = Iso_Calc;
            }

            NotFisrtMeasure = TRUE;
            WaitCount = 0;

            break;
            
        case Iso_N_On:
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
            if(m->Iso_Command == FALSE)
            {
                m->Iso_Meas_Seq = Iso_Stop;
                break;
            }              
            Dio_SetFET_ForIsoR_Low(ON);
            m->Iso_Meas_Seq = ISO_Wait_4;
            IsoR_OperationStatus = ISO_In_Process;
            break;
            
        case ISO_Wait_4:
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
            if(m->Iso_Command == FALSE)
            {
                m->Iso_Meas_Seq = Iso_Stop;
                break;
            }             
            if(WaitCount++ >= (ON_WAIT_TIME-1u))
            {
                m->Iso_Meas_Seq = Iso_N_Meas;
                WaitCount = 0u;    
                StableCheckCount = 0u;
            }
			#ifdef Test_IsoR_Measurement
			new_val = 610u;
			#else
            new_val = Get_IsoAD_Negative();
			#endif
            
            if(MAX(old_val, new_val) - MIN(old_val, new_val) == DETECT_STABLE_REG)
            {
                StableCheckCount++;
                if(StableCheckCount >= STABLE_STATE_LMT)
                {
                    StableCheckCount = 0u;
                    m->Iso_Meas_Seq = Iso_N_Meas;
                    WaitCount = 0u;                    
                }
            }
            else    
            {
                StableCheckCount = 0u;
            }            
            old_val = new_val;
            IsoR_OperationStatus = ISO_In_Process;            
            break;      
            
        case Iso_N_Meas:
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
            if(m->Iso_Command == FALSE)
            {
                m->Iso_Meas_Seq = Iso_Stop;
                break;
            }          

			#ifdef Test_IsoR_Measurement
			temp = 610u;
			#else
            temp = Get_IsoAD_Negative();
			#endif
            m->Iso_N_Array = temp;
                            
            Dio_SetFET_ForIsoR_Low(OFF);
            m->Iso_Meas_Seq = ISO_Wait_2;
            IsoR_OperationStatus = ISO_In_Process;            
            break;
            
        case ISO_Wait_2:
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
            if(m->Iso_Command == FALSE)
            {
                m->Iso_Meas_Seq = Iso_Stop;
                break;
            }             
            m->Iso_Meas_Seq = Iso_Calc;
            WaitCount = 0u;
            IsoR_OperationStatus = ISO_In_Process;            
            break;
            
        case Iso_Calc:
            IsoR_OperationStatus = ISO_Done;
            
            if		(m->old_Iso_Meas_Seq == ISO_Wait_1)	
			{
				m->Iso_Meas_Seq = Iso_N_On;
            }
            else if (m->old_Iso_Meas_Seq == ISO_Wait_2)	
			{
				m->Iso_Meas_Seq = Iso_P_On;
            }
            else 										
			{
				m->Iso_Meas_Seq = Iso_P_On;
            }

            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
            
            if(m->Iso_Command == FALSE)
            {
                m->Iso_Meas_Seq = Iso_Stop;
                break;
            }

			#ifdef Test_IsoR_Measurement
			nowPV = 10000u;
			#else
			nowPV = PackVolt_Get_V_PackMeas();
			#endif

			#ifdef Test_IsoR_Measurement_3
			m->Iso_P = 100u;
            m->Iso_N = 90u;
			#else
			m->Iso_P = m->Iso_P_Array;
            m->Iso_N = m->Iso_N_Array;
			#endif
			
            Calc_ISOR_Table(m->nowISOR, nowPV);

            if(m->Iso_N > m->Iso_P)
            {
                m->Diff_PN = m->Iso_N - m->Iso_P;
                m->Iso_Z = Calc_ISO_R(m->nowISOR, m->Diff_PN);
                IsoR_ErrCnt = 0u;
            }
            else
            {
                IsoR_ErrCnt++;
                if(IsoR_ErrCnt >= (ISOR_ERR_CNT_LMT - 1u))
                {
                    IsoR_ErrCnt = 0u;
                      
                    /* Need to Error Processing */
                    IsoR_OperationStatus = ISO_Fail;
                }
            }
            if(m->Iso_Command == FALSE) 
			{
				m->Iso_Z = 10000u;
            }
            ISO_Resistor = m->Iso_Z;            
            if(ISO_Resistor > 10000u)   
			{
				ISO_Resistor = 10000u;
            }
                        
            break;
            
        case Iso_Stop:
            m->old_Iso_Meas_Seq = m->Iso_Meas_Seq;
			High_State = Dio_GetFET_ForIsoR_High();
			Low_State = Dio_GetFET_ForIsoR_Low();
            if((High_State != 0u) || (Low_State != 0u))
            {
                Dio_SetFET_ForIsoR_High(OFF);
                Dio_SetFET_ForIsoR_Low(OFF);
            }
            
            if(m->Iso_Command == TRUE)
            {
                m->Iso_Meas_Seq = Iso_P_On;
            }
            else
            {
                m->Iso_Z = 10000;
                ISO_Resistor = m->Iso_Z;
            }
            IsoR_OperationStatus = ISO_NoOperation;
            break;
            
        default:    break;        
    }
        
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Isolation Resister init Measurement
 * @param 	void
 * @return	BOOL, TRUE - Success, FALSE - X
*/
bool IsoR_Init_Measurement(void)
{
    CLASSB static ISO_INIT_M IIMS;
    CLASSB static uint16 M_Count;
    bool 	   rv = FALSE;
	CLASSB static uint16 InitMeas[3u];
	CLASSB static uint16 m_index;        // debug
    
    switch(IIMS)
    {
        case ISO_INIT_P_ON:
            Dio_SetFET_ForIsoR_High(ON);
            M_Count = 0;
            m_index = 0;
            IIMS = ISO_INIT_P_OFF;
            break;
            
        case ISO_INIT_P_OFF:            
            if((M_Count % SAMP_MEASURE_PERIOD) == 2u)
            {
                InitMeas[m_index] = Get_IsoAD_Positive();
//                RackData.ChgCurrLimit = InitMeas[m_index] * 10;  // debug
                m_index++;
            }
            if(M_Count >= (SAMP_MEASURE_PERIOD * 3u))
            {
                IsoR_Var.Iso_P = U16_Project_From_Samples(InitMeas[0u], InitMeas[1u], InitMeas[2u], 3u, TRUE);
                IIMS = ISO_INIT_N_ON;
                Dio_SetFET_ForIsoR_High(OFF);
            }
            else 
			{
				M_Count++;
            }
            break;
            
        case ISO_INIT_N_ON:
            Dio_SetFET_ForIsoR_Low(ON);
            M_Count = 0;
            m_index = 0;
            IIMS = ISO_INIT_N_OFF;
            break;
            
        case ISO_INIT_N_OFF:
            if((M_Count % SAMP_MEASURE_PERIOD) == 2u)
            {
                InitMeas[m_index] = Get_IsoAD_Negative();
//                RackData.DchCurrLimit = InitMeas[m_index] * 10;  // debug
                m_index++;                
            }
            if(M_Count >= (SAMP_MEASURE_PERIOD * 3u))
            {
                IsoR_Var.Iso_N = U16_Project_From_Samples(InitMeas[0u], InitMeas[1u], InitMeas[2u], 3u, TRUE);
                IIMS = ISO_INIT_CALC;
                Dio_SetFET_ForIsoR_Low(OFF);
            }
            else 
			{
				M_Count++;  
            }
            break;
            
        case ISO_INIT_CALC:
            IsoR_Var.Diff_PN = IsoR_Var.Iso_N - IsoR_Var.Iso_P;                
            Calc_ISOR_Table(IsoR_Var.nowISOR, PackVolt_Get_V_PackMeas());                
            IsoR_Var.Iso_Z = Calc_ISO_R(IsoR_Var.nowISOR, IsoR_Var.Diff_PN);                
            ISO_Resistor = IsoR_Var.Iso_Z;
            
            IIMS = ISO_INIT_P_ON;        
          
            rv = TRUE;
            break;
            
        default:    break;           
    }

    return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief Measurement Isolation positive point
* @param void
* @return uint32 - IsoR Positive AD
*/
uint16 Get_IsoAD_Positive(void)
{
	uint32 ad_raw = 0u;
	uint8 i;

	for(i = 0u; i < ISOR_SAMPLING_TIME; i++)
	{
		if(i > 0u)	 
		{
			ad_raw += AdcEx_GetRawData(Main_ADC1_CS_GPIO_Port, Main_ADC1_CS_Pin, ADCEX_CH_ISOR_H);
		}
	}
	
	return (uint16)(ad_raw >> ISOR_AVERAGE_SHIFT);
}


/*---------------------------------------------------------------------*/
/**
* @brief Measurement Isolation negative point
* @param void
* @return uint32 - IsoR Negative AD
*/
uint16 Get_IsoAD_Negative(void)
{
	uint32 ad_raw = 0;
	uint8 i;
	
	for(i = 0; i < ISOR_SAMPLING_TIME; i++)
	{
		if(i > 0u)	 
		{
			ad_raw += AdcEx_GetRawData(Main_ADC1_CS_GPIO_Port, Main_ADC1_CS_Pin, ADCEX_CH_ISOR_L);
		}
	}
	
	return (uint16)(ad_raw >> ISOR_AVERAGE_SHIFT);
}


/*---------------------------------------------------------------------*/
/**
* @brief Function to create Isolation table corresponding to Pack voltage
* @param nowISORTable - Save the created table, 
* @param nowPV - Pack Voltage
* @return 
*/
void Calc_ISOR_Table(uint16* nowISORTable, uint16 nowPV)
{
    uint8 i;
    uint16 IsoR_TBL[ISOR_RANGE];
    uint8 arr_std = 0u;
    float buf;
	sint16 sdata;
	uint16 udata;
	float fdata;
	
    
	if(nowPV <= IsoPV_Lookup[0u])
	{		
		for (i=0u; i<ISOR_RANGE; i++)   
		{
			IsoR_TBL[i] = IsoR_RawPN_Table[0u][i];
		}
	}
	else if(nowPV >= IsoPV_Lookup[PV_RANGE-1u])
	{
		for (i=0u; i<ISOR_RANGE; i++)	
		{
			IsoR_TBL[i] = IsoR_RawPN_Table[14u][i];
		}
	}
	else
	{
		for (i=0u; i<PV_RANGE; i++)							
		{
			if (IsoPV_Lookup[i] > nowPV ) 
			{
				arr_std = i;
				break;
			}
		}

		if(arr_std == 0u)
		{
			for (i=0u; i<ISOR_RANGE; i++)   
			{
				IsoR_TBL[i] = IsoR_RawPN_Table[0u][i];
			}
		}
		
		else
		{
			for (i=0u; i<ISOR_RANGE; i++)
			{	
				sdata = (sint16)IsoR_RawPN_Table[arr_std-1u][i] - (sint16)IsoR_RawPN_Table[arr_std][i];
				udata = IsoPV_Lookup[arr_std] - IsoPV_Lookup[arr_std-1u];
				buf = (float)sdata / (float)udata;

				udata = nowPV - IsoPV_Lookup[arr_std-1u];
				fdata = (float)udata * buf;
				IsoR_TBL[i] = IsoR_RawPN_Table[arr_std-1u][i] - (uint16)fdata;
			}
		}
	}

	(void)memcpy(nowISORTable, IsoR_TBL, sizeof(IsoR_TBL));
}


/*---------------------------------------------------------------------*/
/**
* @brief Function to calculation of isolation resistor
* @param nowISORTable - isolation resistor table, 
* @param Diff_PN - different of positive and negative raw value
* @return isolation resistor 
*/
uint16 Calc_ISO_R(const uint16* nowISORTable, uint16 Diff_PN)
{
    uint16 ISO_R;

    ISO_R = InterpolLinear_inU16_outU16(Diff_PN, nowISORTable, IsoR_LookupTbl, ISOR_RANGE);    
    
    return ISO_R;
}


/*---------------------------------------------------------------------*/
/********************************************************************//**
 * Project From Samples routine.
 *
 * This routine receives 3 samples taken along an exponential curve such
 * as an RC curve and projects the final value.  The samples must be taken
 * at equal time intervals.
 *
 * In an exponential curve of this nature, the difference between the first
 * and second samples is expected to be larger than the difference between
 * the second and third samples.  If this is not true or if the bLinearOnly
 * parameter is set, an estimation is performed which simply projects the
 * final value using a linear slope for the number of sample periods specified
 * in parameter LinearPeriods.  (For example, 12 sample periods was determined
 * experimentally to be somewhat optimal for the isolation A/D measurement.)
 * The projection works for either increasing or decreasing values.
 * If the projected result is less than zero, zero is returned.
 ***********************************************************************/
uint16 U16_Project_From_Samples(uint16 Sample1, uint16 Sample2, uint16 Sample3, uint8 LinearPeriods, bool bLinearOnly)
{
   sint32 result;
   sint32 Samp1;
   sint32 Samp2;
   sint32 Samp3;
   sint32 temps1;
   sint32 temps2;
   sint32 temps3;
   sint32 SampAvg;

   Samp1 = (sint32) Sample1;
   Samp2 = (sint32) Sample2;
   Samp3 = (sint32) Sample3;
   temps1 = Samp2 - Samp1;
   temps2 = Samp3 - Samp2;
   temps3 = Samp3 - Samp1;
   SampAvg = (Samp3 - Samp1) / 2;

   /*
    * Simple check for exponential curve.
    * This also avoids a divide-by-zero when the differences between the
    * three samples are equal.
    */
   if (((ABS(temps1)) > (ABS(temps2))) && (!bLinearOnly))
   {
      //result = Samp1 + (temps1 * temps1 / ((2 * temps1) - temps3));
      result = Samp1 + ((temps3 * 2) - (temps2 + SampAvg)) * 2;
   }
   else
   {
      result = Samp1 + ((sint8)LinearPeriods * SampAvg);
   }

   if (result < 0)
   {
      result = 0;
   }
   return (uint16)result;

} /* U16_Project_From_Samples() */


/*---------------------------------------------------------------------*/
/**
* @brief	Calculate linear interpolation
* @param	input		: input data for interpolation
* @param	table_in	: table of the input data for interpolation
* @param	table_out	: table of the output data for interpolation
* @param	arr_size	: size of data array
* @param	arr_aline	: temporary data array to aline
* @return	result of the interpolation
*/
uint16	InterpolLinear_inU16_outU16(uint16 input, const uint16* table_in, const uint16* table_out, uint8 table_size)
{
	uint8	i;
	uint16	output = 0u;
	uint16 udata1;
	uint16 udata2;
	uint16 udata3;
	float fdata;
	
	if		(input  < table_in[0u])
	{
		output = table_out[0u];	/* [kOhm] */
	}
	else if (input >= table_in[table_size - 1u])
	{
		output = table_out[table_size - 1u];	/* [kOhm] */
	}
	else
	{
		for (i=1u; i<table_size; i++)
		{
			if (input < table_in[i])
			{
//				output
//					= table_out[i - 1u]
//					+ (uint16)( (float)(table_out[i]	- table_out[i - 1u])
//						   * (float)(input			- table_in [i - 1u])
//						   / (float)(table_in [i]	- table_in [i - 1u]) );	
				
				udata1 = table_out[i]	- table_out[i - 1u];
				udata2 = input			- table_in [i - 1u];
				udata3 = table_in [i]	- table_in [i - 1u];
				fdata = (float)udata1 * (float)udata2 / (float)udata3;
				output = table_out[i - 1u] + (uint16)fdata;				/* [kOhm] */
						   
				break;
			}
			else
			{

			}
		}
	}
	
	return output;
}


/*---------------------------------------------------------------------*/
/**
 * @brief	Get Isolation Measure Operation status
 * @param 	void
 * @return	IsoR_MeasStatus
*/
IsoR_MeasStatus IsoR_Get_MeasOperationStatus(void)
{
    return IsoR_OperationStatus;
}


/*---------------------------------------------------------------------*/
uint16 IsoR_Get_ISO_Resistor(void)
{
    return ISO_Resistor;
}


/*---------------------------------------------------------------------*/
void IsoR_Set_ISO_Resistor(uint16 IsoR)
{
    ISO_Resistor = IsoR;
}


