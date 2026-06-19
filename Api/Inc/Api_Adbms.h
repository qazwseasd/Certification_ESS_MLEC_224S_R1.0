/*
 * Api_Adbms.h
 *
 *  Created on: Sep 16, 2025
 *      Author: ETI
 */

#ifndef INC_API_ADBMS_H_
#define INC_API_ADBMS_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Api_Def.h"

/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#ifdef Test_Adbms_Calculation_Task
#define ADBMS_READ_PERIOD	20u
#else
#define ADBMS_READ_PERIOD	10u
#endif

/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/
typedef struct
{
	/*	Saved Module Data */
	uint16	ModuleVolt;			
	uint16	CellVolt[DEF_MAX_NUM_OF_CELL_PER_BMIC];
	uint16	BusbarVolt;
	uint16 	CellMaxVolt;
	uint16 	CellMinVolt;
	uint16	CellAvgVolt;
	uint16	CellDevVolt;
	sint16	CellTemp[DEF_MAX_NUM_OF_TEMP_PER_BMIC];
	sint16	CellMaxTemp;
	sint16	CellMinTemp;
	sint16	CellAvgTemp;
	sint16	CellDevTemp;
	sint16	BoardTemp;
	uint16	FANState;
	uint16	BalanceState;
	uint16	MalfCode;
}MODULEDATA;


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void Adbms6815_Init_Api(void);
extern void Adbms_Read_Task(void);
extern void Adbms_Calculation_Task(void);
extern bool Adbms_FirstReadState(void);
extern bool Adbms_GetCommError_F(void);
extern bool Adbms_GetCommError_R(void);
extern void Adbms_ResetCommError_F(void);
extern void Adbms_ResetCommError_R(void);
extern bool Adbms_GetErrorVCellOffline(void);
extern bool Adbms_GetErrorTCellOffline(void);
extern bool Adbms_GetErrorDischargeResistor(void);
extern uint8   Adbms_GetDischargeRes_CheckCnt(void);
extern void Adbms_Set_CellBalanceCmd(uint16* Cmd, Adbms6815_FETCtrl FetCtrl);
extern uint8 Adbms_Get_CellBalanceStatus(uint16* Status);
extern sint16 Adbms_Get_BoardTemp(uint16 Bmic_No);
extern uint16 Adbms_Get_CellVolt(uint16 Bmic_No, uint8 num);
extern uint16 Adbms_Get_CellMaxVolt(uint16 Bmic_No);
extern uint16 Adbms_Get_CellMinVolt(uint16 Bmic_No);
extern sint16 Adbms_Get_CellTemp(uint16 Bmic_No, uint8 num);
extern sint16 Adbms_Get_CellMaxTemp(uint16 Bmic_No);
extern sint16 Adbms_Get_CellMinTemp(uint16 Bmic_No);
extern uint16 Adbms_Get_V_CellMin(void);
extern uint16 Adbms_Get_V_CellMax(void);
extern uint8 Adbms_Get_V_CellModNoMin(void);
extern uint8 Adbms_Get_V_CellCellNoMin(void);
extern uint16 Adbms_Get_V_CellSum(void);
extern uint16 Adbms_Get_V_CellAvg(void);
extern uint16 Adbms_Get_V_CellDev(void);
extern sint16 Adbms_Get_T_CellMin(void);
extern sint16 Adbms_Get_T_CellMax(void);
extern uint8 Adbms_Get_T_CellModNoMax(void);
extern uint8 Adbms_Get_T_CellCellNoMax(void);
extern sint16 Adbms_Get_T_CellAvg(void);
extern sint16 Adbms_Get_T_CellDev(void);
extern uint16 Adbms_Get_ModuleVolt(uint16 Bmic_No);
extern uint16 Adbms_Get_V_ModMax(void);
extern uint16 Adbms_Get_V_ModMin(void);
extern uint16 Adbms_Get_V_ModDev(void);
extern uint16 Adbms_Get_Fan_On_State1(void);
extern uint16 Adbms_Get_Fan_On_State2(void);
extern uint8 Adbms_Get_Fan_On_Num(void);
extern uint16 Adbms_Get_BalanceState(uint16 Bmic_No);
extern uint16 Adbms_Get_MalfCode(uint16 Bmic_No);
extern void Adbms_Set_BalanceCheck(uint16 BCheck);
extern uint16 Adbms_Get_BusbarVolt(uint16 Bmic_No);


extern const uint16 Adbms_Temp_Table[34u];



#endif /* INC_API_ADBMS_H_ */
