/*
 * App_Soc.h
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */

#ifndef INC_APP_SOC_H_
#define INC_APP_SOC_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define SOC_CV_SOC0			2700u    	// [0.001 V]
#define SOC_CV_SOC100		4200u	    // [0.001 V]

#define SOC_MAX_SOC			1000u		/* [0.1 %] */

extern  BOOL    g_b_Discharge_flag;			// added Ver1.71


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/
typedef struct
{
    uint16      SaveCheck;
	uint16		Complete_Init_FCC;
	
	uint32		Init_FCC;
	uint32		FCC;
	uint32		Capacity;	
	uint32 		DchCntIntegral;
	
	uint16 		Cycle_Count;
    uint16      ContOpenCount_UnderLoad;
	
    float   	Accum_ChargeMilliAmps;
	float		Accum_DischargeMilliAmps;
	
    uint32      Accum_ChargeAmps;
	uint32		Accum_DischargeAmps;	
	uint32	    Chg_kWh;
	uint32      Dch_kWh;
	
	uint16      Chg_Wh;
	uint16      Dch_Wh;
	uint16      dummy;
    uint16		CRC1;
}CAPACITY_DATA;


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void Soc_Init(void);
extern void Soc_Calculation_Task(void);
extern void Soc_SetSocInitFlag(BOOL flag);
extern void Soc_SetCapacity(uint32 cap);
extern float Calculate_CellV_useOCVTable(uint16 R_SOC);
extern uint16  Calculate_MinimumSOCCut(uint16 SOH);
extern uint16  Soc_Calculate_UseOCVTable(uint16 cell_v);
extern uint16  Soc_Calculate_UseOCVTable_SameTemp(uint16 cell_v);
extern void Soc_UpdataCapacityData(float CapacityValue);
extern void SOC_Get_CapacityData(CAPACITY_DATA* Capadata);
extern void SOC_Set_CapacityData(CAPACITY_DATA* Capadata);
extern uint16 SOC_Get_ContOpenCount_UnderLoad(void);
extern void SOC_Set_ContOpenCount_UnderLoad(uint16 ContOpenCount);
extern uint32 SOC_Get_Accum_ChargeAmps(void);
extern uint32 SOC_Get_Accum_DischargeAmps(void);
extern uint32 SOC_Get_Chg_kWh(void);
extern uint32 SOC_Get_Dch_kWh(void);
extern uint32 SOC_Get_Init_FCC(void);
extern uint32 SOC_Get_FCC(void);
extern void SOC_Set_FCC(uint32 FCC);
extern uint32 SOC_Get_Capacity(void);
extern void SOC_Set_Capacity(uint32 Capacity);
extern uint16 SOC_Get_CycleCount(void);
extern uint16 SOC_Get_SOC_Real(void);
extern uint16 SOC_Get_SOC_Temp(void);
extern uint16 SOC_Get_SOC_User(void);
extern uint16 SOC_Get_CV_SOC(void);




#endif /* INC_APP_SOC_H_ */
