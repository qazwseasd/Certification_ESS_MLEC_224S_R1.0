/*
 * Drv_Tick.c
 *
 *  Created on: Mar 19, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Tick.h"


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
 * @brief   Initializes the system tick timer.
 * @param   void
 * @retval  void
 */
void Tick_Init(void)
{
    /* No initialization required for system tick at the moment */
}


/**
 * @brief   Returns the current system tick value.
 * @param   void
 * @retval  uint32
 *          - Current system tick value in milliseconds
 */
uint32 Tick_Get(void)
{
    uint32 Sys_Tick;

    Sys_Tick = HAL_GetTick();

    return Sys_Tick;
}


/**
 * @brief   Checks if the system tick has changed.
 * @param   void
 * @retval  uint8
 *          - On  (1u): If the system tick has changed
 *          - Off (0u): If the system tick has not changed
 */
bool Tick_IsChange(void)
{
    static	uint32	Sys_Tick_Pre 	= 0u;
    		uint32	Sys_Tick_Now;
    		bool	Sys_Tick_Event;

    Sys_Tick_Now = HAL_GetTick();
/*
    if (Sys_Tick_Pre != Sys_Tick_Now)
    {
        Sys_Tick_Event = DEF_On;
    }
    else
    {
        Sys_Tick_Event = DEF_Off;
    }

    Sys_Tick_Pre = Sys_Tick_Now;
*/

 	if (Sys_Tick_Now - Sys_Tick_Pre >= 10u)
    {
        Sys_Tick_Event = DEF_On;
		Sys_Tick_Pre = Sys_Tick_Now;
    }
    else
    {
        Sys_Tick_Event = DEF_Off;
		
    }

    return Sys_Tick_Event;
}
