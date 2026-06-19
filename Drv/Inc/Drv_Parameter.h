/*
 * Drv_Parameter.h
 *
 *  Created on: Apr 22, 2026
 *      Author: ETI
 */


/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/
#ifndef INC_DRV_PARAMETER_H_
#define INC_DRV_PARAMETER_H_

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Drv_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/
typedef struct 
{
    uint16 Config_Ver;
	uint16 NumOf_Bmic;		
	uint16 NumberOfParallel;
	uint16 NumberOfSeries;
	uint16 NumOfCellinModule;
	uint16 NumOfTempinModule;	
	sint16 Operate_Fan_Temp;	
	uint16 Dev_for_BalanceStart;
	uint16 Dev_for_BalanceStop;
    uint16 Max_Cell_Voltage;
    uint16 Min_Cell_Voltage;
    uint16 Nomi_Cell_Voltage;
    uint16 FCC_of_Cell;    
    uint16 Contactor_Operating_RefV;
    uint16 DebugEnable;
    
	uint16 Para_Block1_crc;

	// Warning
	uint16	OCV_Warning;			// Over Cell Voltage Warning
	uint16 	OPV_Warning;			// Over Pack Voltage Warning
	uint16 	UCV_Warning;			// Under Cell Voltage Warning
	uint16 	UPV_Warning;			// Under Pack Voltage Warning
	sint16 	COT_Warning;			// Charging Over Temperature Warning		
	sint16 	DOT_Warning;			// Discharging Over Temperature Warning		
	sint16 	CUT_Warning;			// Charging Under Temperature Warning
	sint16 	DUT_Warning;			// Discharging Under Temperature Warning
	sint16 	COC_Warning;			// Over Current Warning
	sint16 	DOC_Warning;
	uint16 	LSOC_Warning; 			// Low Battery Warning
	uint16 	CIBV_Warning;			// Imbalance Voltage Warning 
	uint16 	MIBV_Warning;			// Module Imbalance voltage warning
	uint16	ISO_Warning;			// Isolation resistor warning
	uint16	SCE_Warning;			// Slave Communication Warning
	uint16	MCE_Warning;			// Master Communication Warning
	uint16	OAV_Warning;			// Over Auxilary Voltage Warning 
	uint16 	UAV_Warning;			// Under Axillary Voltage Warning
	uint16 	SLAVE_Warning;			// RLEC Malfunction Warning

    uint16 	Para_Block2_crc;

	// Warning Release
	uint16	OCV_W_Release;			// Over Cell Voltage Warning
	uint16 	OPV_W_Release;			// Over Pack Voltage Warning
	uint16 	UCV_W_Release;			// Under Cell Voltage Warning
	uint16 	UPV_W_Release;			// Under Pack Voltage Warning
	sint16 	COT_W_Release;			// Charging Over Temperature Warning		
	sint16 	DOT_W_Release;			// Discharging Over Temperature Warning		
	sint16 	CUT_W_Release;			// Charging Under Temperature Warning
	sint16 	DUT_W_Release;			// Discharging Under Temperature Warning
	sint16 	COC_W_Release;			// Over Current Warning
	sint16 	DOC_W_Release;
	uint16 	LSOC_W_Release; 		// Low SOC Warning
	uint16 	CIBV_W_Release;			// Imbalance Voltage Warning 	
	uint16 	MIBV_W_Release;		// Module Imbalance voltage warning
	uint16 	ISO_W_Release;			// Isolation resistor warning
	uint16	SCE_W_Release;			// Slave Communication Warning
	uint16	MCE_W_Release;			// Master Communication Warning
	uint16 	OAV_W_Release;			// Over Auxilary Voltage Warning 
	uint16 	UAV_W_Release;			// Under Axillary Voltage Warning
	uint16 	SLAVE_W_Release;		// RLEC Malfunction Warning

	uint16 	Para_Block3_crc;
	
	// Fault
	uint16 	OCV_Fault; 				// Over Cell Voltage Fault 
	uint16 	OPV_Fault;				// Over Pack Voltage Fault
	uint16 	UCV_Fault;				// Under Cell Voltage Fault 
	uint16 	UPV_Fault;				// Under Pack Voltage Fault
	sint16 	COT_Fault;				// Charging Over Temperature Fault
	sint16 	DOT_Fault;				// Discharging Over Temperature Fault
	sint16 	CUT_Fault;				// Charging Under Temperature Fault
	sint16 	DUT_Fault;				// Discharging Under Temperature Fault
	sint16 	COC_Fault;				// Over Current Fault 
	sint16 	DOC_Fault;
	uint16 	CIBV_Fault;				// Imbalance Voltage Fault
	uint16 	MIBV_Fault;				// Module Imbalance voltage Fault
	uint16 	ISO_Fault;				// Isolation Fault 
	uint16 	SCE_Fault;				// Slave Communication Fault
	uint16 	MCE_Fault;				// Master Communication Fault
	uint16 	OAV_Fault;				// Over Auxilary Voltage Fault 
	uint16 	UAV_Fault;				// Under Auxilary Voltage Fault 	
	uint16 	FUSE_Fault;
	uint16 	SLAVE_Fault;
	uint16 	DEAD_CELL_Fault;

	uint16 	Para_Block4_crc;

	// Fault Release
	uint16 	OCV_F_Release; 			// Over Cell Voltage Fault 
	uint16 	OPV_F_Release;			// Over Pack Voltage Fault
	uint16 	UCV_F_Release;			// Under Cell Voltage Fault 
	uint16 	UPV_F_Release;			// Under Pack Voltage Fault
	sint16 	COT_F_Release;			// Charging Over Temperature Fault
	sint16 	DOT_F_Release;			// Discharging Over Temperature Fault
	sint16 	CUT_F_Release;			// Charging Under Temperature Fault
	sint16 	DUT_F_Release;			// Discharging Under Temperature Fault
	sint16 	COC_F_Release;			// Over Current Fault 
	sint16 	DOC_F_Release;
	uint16 	CIBV_F_Release;			// Imbalance Voltage Fault
	uint16 	MIBV_F_Release;			// Module Imbalance voltage Fault
	uint16 	ISO_F_Release;			// Isolation Fault 
	uint16 	SCE_F_Release;			// Slave Communication Fault
	uint16 	MCE_F_Release;			// Master Communication Fault
	uint16 	OAV_F_Release;			// Over Auxilary Voltage Fault 
	uint16 	UAV_F_Release;			// Under Auxilary Voltage Fault 
	uint16 	FUSE_F_Release;		
	uint16 	SLAVE_F_Release;
	uint16 	DEAD_CELL_F_Release;	

	uint16 	Para_Block5_crc;
}PARAMETER ; 


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void ConfigData_SetDefaultConfigData(void);
extern void Parameter_Get_ParameterData(PARAMETER* parameter);
extern void Parameter_Set_ParameterData(PARAMETER* parameter);
extern uint16 Parameter_Get_NumOf_Bmic(void);
extern uint16 Parameter_Get_NumberOfParallel(void);
extern uint16 Parameter_Get_NumberOfSeries(void);
extern uint16 Parameter_Get_NumOfCellinModule(void);
extern uint16 Parameter_Get_NumOfTempinModule(void);
extern sint16 Parameter_Get_Operate_Fan_Temp(void);
extern uint16 Parameter_Get_BalanceStartDev(void);
extern uint16 Parameter_Get_BalanceStopDev(void);
extern uint16 Parameter_Get_Max_Cell_Voltage(void);
extern uint16 Parameter_Get_Min_Cell_Voltage(void);
extern uint16 Parameter_Get_Nomi_Cell_Voltage(void);
extern uint16 Parameter_Get_FCC_of_Cell(void);
extern uint16 Parameter_Get_Contactor_Operating_RefV(void);
extern sint16 Parameter_Get_COC_Warning(void);
extern sint16 Parameter_Get_DOC_Warning(void);
extern uint16 Parameter_Get_OCV_Fault(void);
extern uint16 Parameter_Get_UCV_Fault(void);
extern uint16 Parameter_Get_MCE_Fault(void);
extern char* Get_ParameterAddress(void);



#endif /* INC_DRV_PARAMETER_H_ */
