/*
 * Drv_Parameter.c
 *
 *  Created on: Apr, 2026
 *      Author: ETI
 */

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Parameter.h"


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


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
CLASSB static PARAMETER 		Parameter;


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
 * @brief	Set default configuration data
 * @param 	void
 * @return	void
*/
void ConfigData_SetDefaultConfigData(void)
{
	#ifndef Drawer_14ea
	
    Parameter.Config_Ver			= DEFAULT_CONFIG_VER;
	Parameter.NumOf_Bmic 			= DEFAULT_NUMBER_OF_BMIC;
	Parameter.NumberOfParallel 		= DEFAULT_NUMBER_OF_PARALLEL;
	Parameter.NumberOfSeries 		= DEFAULT_NUMBER_OF_SERIES;
    Parameter.NumOfCellinModule     = DEFAULT_NUMBER_OF_CELL_inModule;
    Parameter.NumOfTempinModule     = DEFAULT_NUMBER_OF_Temp_inModule;
	Parameter.Operate_Fan_Temp 		= DEFAULT_OPERATE_FAN_TEMP;	
	Parameter.Dev_for_BalanceStart 	= DEFAULT_DEV_FOR_BALANCE_START;
	Parameter.Dev_for_BalanceStop 	= DEFAULT_DEV_FOR_BALANCE_STOP;
    Parameter.Max_Cell_Voltage      = DEFAULT_MAX_CELL_V;
    Parameter.Min_Cell_Voltage      = DEFAULT_MIN_CELL_V;
    Parameter.Nomi_Cell_Voltage     = DEFAULT_NOMI_CELL_V;
    Parameter.FCC_of_Cell           = DEFAULT_CELL_CAPACITY;
    Parameter.Contactor_Operating_RefV 
                                    = DEFAULT_CONT_REF_V;
    Parameter.DebugEnable           = DEFAULT_DEBUG_ENABLE;
    
	Parameter.OCV_Warning 			= DEFAULT_OCV_WARNING;
	Parameter.OPV_Warning 			= DEFAULT_OPV_WARNING;
	Parameter.UCV_Warning 			= DEFAULT_UCV_WARNING;
	Parameter.UPV_Warning 			= DEFAULT_UPV_WARNING;
	Parameter.COT_Warning 			= DEFAULT_COT_WARNING;
	Parameter.DOT_Warning 			= DEFAULT_DOT_WARNING;
	Parameter.CUT_Warning 			= DEFAULT_CUT_WARNING;
	Parameter.DUT_Warning 			= DEFAULT_DUT_WARNING;
	Parameter.COC_Warning 			= DEFAULT_COC_WARNING;
	Parameter.DOC_Warning 			= DEFAULT_DOC_WARNING;
	Parameter.LSOC_Warning 			= DEFAULT_LOWSOC_WARNING;
	Parameter.CIBV_Warning 			= DEFAULT_CELL_IMBALANCE_WARNING;
	Parameter.MIBV_Warning 			= DEFAULT_MODULE_IMBALANCE_WARNING;
	Parameter.ISO_Warning 			= DEFAULT_ISOLATION_WARNING;
	Parameter.SCE_Warning 			= DEFAULT_SLAVE_COMM_WARNING;
	Parameter.MCE_Warning 			= DEFAULT_MASTER_COMM_WARNING;
	Parameter.OAV_Warning 			= DEFAULT_OAV_WARNING;
	Parameter.UAV_Warning 			= DEFAULT_UAV_WARNING;
	Parameter.SLAVE_Warning 		= DEFAULT_RLEC_MALFUNCTION_WARNING;

	Parameter.OCV_Fault 			= DEFAULT_OCV_FAULT;
	Parameter.OPV_Fault 			= DEFAULT_OPV_FAULT;
	Parameter.UCV_Fault 			= DEFAULT_UCV_FAULT;
	Parameter.UPV_Fault 			= DEFAULT_UPV_FAULT;
	Parameter.COT_Fault 			= DEFAULT_COT_FAULT;
	Parameter.DOT_Fault 			= DEFAULT_DOT_FAULT;
	Parameter.CUT_Fault 			= DEFAULT_CUT_FAULT;
	Parameter.DUT_Fault 			= DEFAULT_DUT_FAULT;
	Parameter.COC_Fault 			= DEFAULT_COC_FAULT;
	Parameter.DOC_Fault 			= DEFAULT_DOC_FAULT;
	Parameter.CIBV_Fault 			= DEFAULT_CELL_IMBALANCE_FAULT;
	Parameter.MIBV_Fault 			= DEFAULT_MODULE_IMBALANCE_FAULT;
	Parameter.ISO_Fault 			= DEFAULT_ISOLATION_FAULT; 
	Parameter.SCE_Fault 			= DEFAULT_SLAVE_COMM_FAULT;
	Parameter.MCE_Fault 			= DEFAULT_MASTER_COMM_FAULT;
	Parameter.OAV_Fault 			= DEFAULT_OAV_FAULT;
	Parameter.UAV_Fault 			= DEFAULT_UAV_FAULT;
	Parameter.FUSE_Fault 			= DEFAULT_FUSE_DEAD_FAULT;
	Parameter.SLAVE_Fault 			= DEFAULT_RLEC_MALFUNCTION_FAULT;
	Parameter.DEAD_CELL_Fault 		= DEFAULT_DEADCELL_FAULT;
		
	Parameter.OCV_W_Release 		= DEFAULT_OCV_WARNING_RELEASE;
	Parameter.OPV_W_Release 		= DEFAULT_OPV_WARNING_RELEASE;
	Parameter.UCV_W_Release 		= DEFAULT_UCV_WARNING_RELEASE;
	Parameter.UPV_W_Release 		= DEFAULT_UPV_WARNING_RELEASE;
	Parameter.COT_W_Release 		= DEFAULT_COT_WARNING_RELEASE;
	Parameter.DOT_W_Release 		= DEFAULT_DOT_WARNING_RELEASE;
	Parameter.CUT_W_Release 		= DEFAULT_CUT_WARNING_RELEASE;
	Parameter.DUT_W_Release 		= DEFAULT_DUT_WARNING_RELEASE;
	Parameter.COC_W_Release 		= DEFAULT_COC_WARNING_RELEASE;
	Parameter.DOC_W_Release 		= DEFAULT_DOC_WARNING_RELEASE;
	Parameter.LSOC_W_Release		= DEFAULT_LOWSOC_WARNING_RELEASE;
	Parameter.CIBV_W_Release 		= DEFAULT_CELL_IMBALANCE_WARNING_RELEASE;
	Parameter.MIBV_W_Release 		= DEFAULT_MODULE_IMBALANCE_WARNING_RELEASE;
	Parameter.ISO_W_Release 		= DEFAULT_ISOLATION_WARNING_RELEASE;
	Parameter.SCE_W_Release 		= DEFAULT_SLAVE_COMM_WARNING_RELEASE;
	Parameter.MCE_W_Release 		= DEFAULT_MASTER_COMM_WARNING_RELEASE;
	Parameter.OAV_W_Release 		= DEFAULT_OAV_WARNING_RELEASE;
	Parameter.UAV_W_Release 		= DEFAULT_UAV_WARNING_RELEASE;
	Parameter.SLAVE_W_Release 		= DEFAULT_RLEC_MALFUNCTION_WARNING_RELEASE;
 
	Parameter.OCV_F_Release 		= DEFAULT_OCV_FAULT_RELEASE;
	Parameter.OPV_F_Release 		= DEFAULT_OPV_FAULT_RELEASE;
	Parameter.UCV_F_Release 		= DEFAULT_UCV_FAULT_RELEASE;
	Parameter.UPV_F_Release 		= DEFAULT_UPV_FAULT_RELEASE;
	Parameter.COT_F_Release 		= DEFAULT_COT_FAULT_RELEASE;
	Parameter.DOT_F_Release 		= DEFAULT_DOT_FAULT_RELEASE;
	Parameter.CUT_F_Release 		= DEFAULT_CUT_FAULT_RELEASE;
	Parameter.DUT_F_Release 		= DEFAULT_DUT_FAULT_RELEASE;
	Parameter.COC_F_Release 		= DEFAULT_COC_FAULT_RELEASE;
	Parameter.DOC_F_Release 		= DEFAULT_DOC_FAULT_RELEASE;
	Parameter.CIBV_F_Release 		= DEFAULT_CELL_IMBALANCE_FAULT_RELEASE;
	Parameter.MIBV_F_Release 		= DEFAULT_MODULE_IMBALANCE_FAULT_RELEASE;
	Parameter.ISO_F_Release 		= DEFAULT_ISOLATION_FAULT_RELEASE;
	Parameter.SCE_F_Release 		= DEFAULT_SLAVE_COMM_FAULT_RELEASE;
	Parameter.MCE_F_Release 		= DEFAULT_MASTER_COMM_FAULT_RELEASE;
	Parameter.OAV_F_Release 		= DEFAULT_OAV_FAULT_RELEASE;
	Parameter.UAV_F_Release 		= DEFAULT_UAV_FAULT_RELEASE;
	Parameter.FUSE_F_Release 		= DEFAULT_FUSE_DEAD_FAULT_RELEASE;
	Parameter.SLAVE_F_Release 		= DEFAULT_RLEC_MALFUNCTION_FAULT_RELEASE;
	Parameter.DEAD_CELL_F_Release 	= DEFAULT_DEADCELL_FAULT_RELEASE;

	#else
	
	Parameter.Config_Ver			= DEFAULT2_CONFIG_VER;
	Parameter.NumOf_Bmic 			= DEFAULT2_NUMBER_OF_BMIC;
	Parameter.NumberOfParallel 		= DEFAULT2_NUMBER_OF_PARALLEL;
	Parameter.NumberOfSeries 		= DEFAULT2_NUMBER_OF_SERIES;
    Parameter.NumOfCellinModule     = DEFAULT2_NUMBER_OF_CELL_inModule;
    Parameter.NumOfTempinModule     = DEFAULT2_NUMBER_OF_Temp_inModule;
	Parameter.Operate_Fan_Temp 		= DEFAULT2_OPERATE_FAN_TEMP;	
	Parameter.Dev_for_BalanceStart 	= DEFAULT2_DEV_FOR_BALANCE_START;
	Parameter.Dev_for_BalanceStop 	= DEFAULT2_DEV_FOR_BALANCE_STOP;
    Parameter.Max_Cell_Voltage      = DEFAULT2_MAX_CELL_V;
    Parameter.Min_Cell_Voltage      = DEFAULT2_MIN_CELL_V;
    Parameter.Nomi_Cell_Voltage     = DEFAULT2_NOMI_CELL_V;
    Parameter.FCC_of_Cell           = DEFAULT2_CELL_CAPACITY;
    Parameter.Contactor_Operating_RefV 
                                    = DEFAULT2_CONT_REF_V;
    Parameter.DebugEnable           = DEFAULT2_DEBUG_ENABLE;
    
	Parameter.OCV_Warning 			= DEFAULT2_OCV_WARNING;
	Parameter.OPV_Warning 			= DEFAULT2_OPV_WARNING;
	Parameter.UCV_Warning 			= DEFAULT2_UCV_WARNING;
	Parameter.UPV_Warning 			= DEFAULT2_UPV_WARNING;
	Parameter.COT_Warning 			= DEFAULT2_COT_WARNING;
	Parameter.DOT_Warning 			= DEFAULT2_DOT_WARNING;
	Parameter.CUT_Warning 			= DEFAULT2_CUT_WARNING;
	Parameter.DUT_Warning 			= DEFAULT2_DUT_WARNING;
	Parameter.COC_Warning 			= DEFAULT2_COC_WARNING;
	Parameter.DOC_Warning 			= DEFAULT2_DOC_WARNING;
	Parameter.LSOC_Warning 			= DEFAULT2_LOWSOC_WARNING;
	Parameter.CIBV_Warning 			= DEFAULT2_CELL_IMBALANCE_WARNING;
	Parameter.MIBV_Warning 			= DEFAULT2_MODULE_IMBALANCE_WARNING;
	Parameter.ISO_Warning 			= DEFAULT2_ISOLATION_WARNING;
	Parameter.SCE_Warning 			= DEFAULT2_SLAVE_COMM_WARNING;
	Parameter.MCE_Warning 			= DEFAULT2_MASTER_COMM_WARNING;
	Parameter.OAV_Warning 			= DEFAULT2_OAV_WARNING;
	Parameter.UAV_Warning 			= DEFAULT2_UAV_WARNING;
	Parameter.SLAVE_Warning 		= DEFAULT2_RLEC_MALFUNCTION_WARNING;

	Parameter.OCV_Fault 			= DEFAULT2_OCV_FAULT;
	Parameter.OPV_Fault 			= DEFAULT2_OPV_FAULT;
	Parameter.UCV_Fault 			= DEFAULT2_UCV_FAULT;
	Parameter.UPV_Fault 			= DEFAULT2_UPV_FAULT;
	Parameter.COT_Fault 			= DEFAULT2_COT_FAULT;
	Parameter.DOT_Fault 			= DEFAULT2_DOT_FAULT;
	Parameter.CUT_Fault 			= DEFAULT2_CUT_FAULT;
	Parameter.DUT_Fault 			= DEFAULT2_DUT_FAULT;
	Parameter.COC_Fault 			= DEFAULT2_COC_FAULT;
	Parameter.DOC_Fault 			= DEFAULT2_DOC_FAULT;
	Parameter.CIBV_Fault 			= DEFAULT2_CELL_IMBALANCE_FAULT;
	Parameter.MIBV_Fault 			= DEFAULT2_MODULE_IMBALANCE_FAULT;
	Parameter.ISO_Fault 			= DEFAULT2_ISOLATION_FAULT; 
	Parameter.SCE_Fault 			= DEFAULT2_SLAVE_COMM_FAULT;
	Parameter.MCE_Fault 			= DEFAULT2_MASTER_COMM_FAULT;
	Parameter.OAV_Fault 			= DEFAULT2_OAV_FAULT;
	Parameter.UAV_Fault 			= DEFAULT2_UAV_FAULT;
	Parameter.FUSE_Fault 			= DEFAULT2_FUSE_DEAD_FAULT;
	Parameter.SLAVE_Fault 			= DEFAULT2_RLEC_MALFUNCTION_FAULT;
	Parameter.DEAD_CELL_Fault 		= DEFAULT2_DEADCELL_FAULT;
		
	Parameter.OCV_W_Release 		= DEFAULT2_OCV_WARNING_RELEASE;
	Parameter.OPV_W_Release 		= DEFAULT2_OPV_WARNING_RELEASE;
	Parameter.UCV_W_Release 		= DEFAULT2_UCV_WARNING_RELEASE;
	Parameter.UPV_W_Release 		= DEFAULT2_UPV_WARNING_RELEASE;
	Parameter.COT_W_Release 		= DEFAULT2_COT_WARNING_RELEASE;
	Parameter.DOT_W_Release 		= DEFAULT2_DOT_WARNING_RELEASE;
	Parameter.CUT_W_Release 		= DEFAULT2_CUT_WARNING_RELEASE;
	Parameter.DUT_W_Release 		= DEFAULT2_DUT_WARNING_RELEASE;
	Parameter.COC_W_Release 		= DEFAULT2_COC_WARNING_RELEASE;
	Parameter.DOC_W_Release 		= DEFAULT2_DOC_WARNING_RELEASE;
	Parameter.LSOC_W_Release		= DEFAULT2_LOWSOC_WARNING_RELEASE;
	Parameter.CIBV_W_Release 		= DEFAULT2_CELL_IMBALANCE_WARNING_RELEASE;
	Parameter.MIBV_W_Release 		= DEFAULT2_MODULE_IMBALANCE_WARNING_RELEASE;
	Parameter.ISO_W_Release 		= DEFAULT2_ISOLATION_WARNING_RELEASE;
	Parameter.SCE_W_Release 		= DEFAULT2_SLAVE_COMM_WARNING_RELEASE;
	Parameter.MCE_W_Release 		= DEFAULT2_MASTER_COMM_WARNING_RELEASE;
	Parameter.OAV_W_Release 		= DEFAULT2_OAV_WARNING_RELEASE;
	Parameter.UAV_W_Release 		= DEFAULT2_UAV_WARNING_RELEASE;
	Parameter.SLAVE_W_Release 		= DEFAULT2_RLEC_MALFUNCTION_WARNING_RELEASE;
 
	Parameter.OCV_F_Release 		= DEFAULT2_OCV_FAULT_RELEASE;
	Parameter.OPV_F_Release 		= DEFAULT2_OPV_FAULT_RELEASE;
	Parameter.UCV_F_Release 		= DEFAULT2_UCV_FAULT_RELEASE;
	Parameter.UPV_F_Release 		= DEFAULT2_UPV_FAULT_RELEASE;
	Parameter.COT_F_Release 		= DEFAULT2_COT_FAULT_RELEASE;
	Parameter.DOT_F_Release 		= DEFAULT2_DOT_FAULT_RELEASE;
	Parameter.CUT_F_Release 		= DEFAULT2_CUT_FAULT_RELEASE;
	Parameter.DUT_F_Release 		= DEFAULT2_DUT_FAULT_RELEASE;
	Parameter.COC_F_Release 		= DEFAULT2_COC_FAULT_RELEASE;
	Parameter.DOC_F_Release 		= DEFAULT2_DOC_FAULT_RELEASE;
	Parameter.CIBV_F_Release 		= DEFAULT2_CELL_IMBALANCE_FAULT_RELEASE;
	Parameter.MIBV_F_Release 		= DEFAULT2_MODULE_IMBALANCE_FAULT_RELEASE;
	Parameter.ISO_F_Release 		= DEFAULT2_ISOLATION_FAULT_RELEASE;
	Parameter.SCE_F_Release 		= DEFAULT2_SLAVE_COMM_FAULT_RELEASE;
	Parameter.MCE_F_Release 		= DEFAULT2_MASTER_COMM_FAULT_RELEASE;
	Parameter.OAV_F_Release 		= DEFAULT2_OAV_FAULT_RELEASE;
	Parameter.UAV_F_Release 		= DEFAULT2_UAV_FAULT_RELEASE;
	Parameter.FUSE_F_Release 		= DEFAULT2_FUSE_DEAD_FAULT_RELEASE;
	Parameter.SLAVE_F_Release 		= DEFAULT2_RLEC_MALFUNCTION_FAULT_RELEASE;
	Parameter.DEAD_CELL_F_Release 	= DEFAULT2_DEADCELL_FAULT_RELEASE;
	
	#endif
}


/*---------------------------------------------------------------------*/
void Parameter_Get_ParameterData(PARAMETER* parameter)
{
    (void)memcpy(parameter, &Parameter, sizeof(PARAMETER));
}


/*---------------------------------------------------------------------*/
void Parameter_Set_ParameterData(PARAMETER* parameter)
{
    (void)memcpy(&Parameter, parameter, sizeof(PARAMETER));
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_NumOf_Bmic(void)
{
    return Parameter.NumOf_Bmic;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_NumberOfParallel(void)
{
    return Parameter.NumberOfParallel;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_NumberOfSeries(void)
{
    return Parameter.NumberOfSeries;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_NumOfCellinModule(void)
{
    return Parameter.NumOfCellinModule;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_NumOfTempinModule(void)
{
    return Parameter.NumOfTempinModule;
}


/*---------------------------------------------------------------------*/
sint16 Parameter_Get_Operate_Fan_Temp(void)
{
    return Parameter.Operate_Fan_Temp;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_BalanceStartDev(void)
{
    return Parameter.Dev_for_BalanceStart;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_BalanceStopDev(void)
{
    return Parameter.Dev_for_BalanceStop;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_Max_Cell_Voltage(void)
{
    return Parameter.Max_Cell_Voltage;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_Min_Cell_Voltage(void)
{
    return Parameter.Min_Cell_Voltage;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_Nomi_Cell_Voltage(void)
{
    return Parameter.Nomi_Cell_Voltage;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_FCC_of_Cell(void)
{
    return Parameter.FCC_of_Cell;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_Contactor_Operating_RefV(void)
{
    return Parameter.Contactor_Operating_RefV;
}


/*---------------------------------------------------------------------*/
sint16 Parameter_Get_COC_Warning(void)
{
    return Parameter.COC_Warning;
}


/*---------------------------------------------------------------------*/
sint16 Parameter_Get_DOC_Warning(void)
{
    return Parameter.DOC_Warning;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_OCV_Fault(void)
{
    return Parameter.OCV_Fault;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_UCV_Fault(void)
{
    return Parameter.UCV_Fault;
}


/*---------------------------------------------------------------------*/
uint16 Parameter_Get_MCE_Fault(void)
{
    return Parameter.MCE_Fault;
}


/*---------------------------------------------------------------------*/
char* Get_ParameterAddress(void)
{
    return (char*)&Parameter;
}






