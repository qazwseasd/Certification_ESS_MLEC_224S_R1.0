/*
 * App_Init.c
 *
 *  Created on: Mar 25, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "App_Init.h"


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
static	uint16 	MaxChgVolt;
static	uint16	MinDchVolt;
static	uint16  Rated_Volt;
static	sint32 	MaxCurrent;

CLASSB static	PARAMETER Init_Paramter;
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
* @brief	Application Initialization
* @param	void
* @return	void
*/
void	Init_App(void)
{
	float temp;
	
	CanVcuTx_Init();
	ConfigData_Init(); 
	CalibData_Init();
	(void)EepromEx_ReadStringData();
	Warranty_Init();
	Dio_SetFET_ForIsoR_High(OFF);
	Dio_SetFET_ForIsoR_Low(OFF);
	CanUiRx_Init();
	
	CellBalance_Init();
	System_Init();
	Fan_Control_Init();
	Relay_App_Init();


	Parameter_Get_ParameterData(&Init_Paramter);
	temp = (float)Init_Paramter.Max_Cell_Voltage * (float)Init_Paramter.NumberOfSeries * (float)Init_Paramter.NumOfCellinModule * (float)10 * (float)0.001;
	MaxChgVolt = (uint16)temp;
    temp = (float)Init_Paramter.Min_Cell_Voltage * (float)Init_Paramter.NumberOfSeries * (float)Init_Paramter.NumOfCellinModule * (float)10 * (float)0.001;
    MinDchVolt = (uint16)temp;
    temp = (float)Init_Paramter.Nomi_Cell_Voltage * (float)Init_Paramter.NumberOfSeries * (float)Init_Paramter.NumOfCellinModule * (float)10 * (float)0.001;
    Rated_Volt = (uint16)temp;    
    temp = (float)Init_Paramter.FCC_of_Cell * (float)Init_Paramter.NumberOfParallel * (float)10 * (float)0.001;
    MaxCurrent = (sint32)temp;

	IsoR_Set_ISO_Resistor(10000u);
	
	Task_Init();

	
}

/*---------------------------------------------------------------------*/
uint16 AppInit_Get_MaxChgVolt(void)
{
    return MaxChgVolt;
}


/*---------------------------------------------------------------------*/
uint16 AppInit_Get_MinDchVolt(void)
{
    return MinDchVolt;
}


/*---------------------------------------------------------------------*/
uint16 AppInit_Get_Rated_Volt(void)
{
    return Rated_Volt;
}


/*---------------------------------------------------------------------*/
sint32 AppInit_Get_MaxCurrent(void)
{
    return MaxCurrent;
}




