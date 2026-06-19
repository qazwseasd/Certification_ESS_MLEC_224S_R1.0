/*
 * Drv_Init.c
 *
 *  Created on: Mar 25, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Init.h"


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
 * @brief   Initializes all hardware components.
 *
 * @details This function initializes all the necessary hardware modules in the system,
 *          Each initialization function is called to set up the respective hardware components.
 *
 * @param   None
 *          No input parameters are required for this function.
 *
 * @retval  None
 *          This function does not return a value.
 */
void	Init_Hw(void)
{
	Tick_Init();
	Dwt_Init();
	Led_Init();
    
	Can_Init(&hcan1);
	(void)AdcEx_Init(Main_ADC1_CS_GPIO_Port, Main_ADC1_CS_Pin);
    (void)AdcEx_Init(Main_ADC2_CS_GPIO_Port, Main_ADC2_CS_Pin);
    (void)EepromEx_Init();
	(void)Fan_Init();
	(void)Relay_Init();

    Wdt_Init();

	Dio_Earn_BoardID();
	Adbms6815_Init_Drv();


}

