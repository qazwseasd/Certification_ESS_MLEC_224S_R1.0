/*
 * App_Task.c
 *
 *  Created on: Mar 14, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_Task.h"


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
* @brief	Task Initialization
* @param	void
* @return	void
*/
void	Task_Init(void)
{
	(void)Task_Register( CanVcuTx_Task, 	   		10u 					);  	// 0 
	(void)Task_Register( Can_Process_Task, 			CAN_TXTOMASTER_PERIOD	); 		// 1 
	(void)Task_Register( System_State_Task,			10u						);  	// 2 
	(void)Task_Register( Curr_Read_Task,	 		10u						); 		// 3 
	(void)Task_Register( Adbms_Read_Task, 			ADBMS_READ_PERIOD		);  	// 4 
	(void)Task_Register( AuxVolt_Read_Task,	 		10u						);  	// 5 
	(void)Task_Register( Relay_Control_Task,		10u						);   	// 6 

	(void)Task_Register( PackVolt_Read_Task,		20u						);  	// 7 
	
	(void)Task_Register( EepromEx_Control_Task,		100u					);  	// 8 
	(void)Task_Register( CellBalance_Control_Task, 	100u					);  	// 9 
	(void)Task_Register( IsoR_Measurement_Task,     100u					);  	// 10
	(void)Task_Register( Curr_Calculation_Task,		100u					);  	// 11
	(void)Task_Register( Diag_Task, 		 		100u					);  	// 12
	
	(void)Task_Register( Led_Task,					100u					);		// 13
	(void)Task_Register( CtrlLed_Task,				200u					);		// 14
	(void)Task_Register( Wdt_Task,		 			200u					);		// 15

	(void)Task_Register( Adbms_Calculation_Task,	200u					);  	// 16
	(void)Task_Register( MeasTAdcIn_Task,  			200u					);  	// 17
	(void)Task_Register( CurrLimit_Calculation_Task,200u					);  	// 18
	(void)Task_Register( Ir_Calculation_Task, 		200u					);		// 19

	(void)Task_Register( Fan_Control_Task,      	1000u					);  	// 20
	(void)Task_Register( Warranty_Task,     		1000u					);  	// 21
	(void)Task_Register( Soc_Calculation_Task, 		1000u					);  	// 22
	(void)Task_Register( Soh_Calculation_Task,      1000u					);  	// 23
	(void)Task_Register( SOH_Calculation_Task2,  	1000u					);  	// 24
	(void)Task_Register( Soue_Calculation_Task,  	1000u					);  	// 25
	(void)Task_Register( DetectTR_Task, 		 	1000u					);  	// 26

#ifdef IEC60730_LIB      
	(void)Task_Register( IEC60730_Task, 		 	1000u					);  	// 27
#endif  

}

