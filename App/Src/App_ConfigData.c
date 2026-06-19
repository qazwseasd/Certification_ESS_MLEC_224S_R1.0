/*
 * Api_ConfigData.c
 *
 *  Created on: Sep 19, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include <App_ConfigData.h>


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
 * @brief	initialize global variable & configuration data
 * @param 	void
 * @return	void
*/
void ConfigData_Init(void)
/*
	initialize global variable & configuration data. 
	if exist parameter data in external eeprom  then load parameter data from external eeprom 
	else set parameter data to Default data
	likewise OCV table 	
*/
{	
	ConfigData_SetDefaultConfigData();
/*	
	uint8 Try_num;
	
	if(EepromEx_IsSavedParameter() == SAVED)
	{
		 load parameter data from external eeprom 
		for(Try_num = 0u; Try_num < EEPROMEX_WR_TRY_NUM; Try_num++)
		{
			EepromEx_ResetErrorFlag();
			if(!EepromEx_InOutParameters((&Parameter), 					get, EEPROMEX_BLOCK1_DATA_QTY_BYTE, EEPROMEX_BLOCK1_ADDR_OFFSET))
			{
				EepromEx_SetErrorFlag(0u);
			}
	        if(!EepromEx_InOutParameters((&Parameter.OCV_Warning), 		get, EEPROMEX_BLOCK2_DATA_QTY_BYTE, EEPROMEX_BLOCK2_ADDR_OFFSET))
			{
				EepromEx_SetErrorFlag(1u);
	        }
	        if(!EepromEx_InOutParameters((&Parameter.OCV_W_Release), 	get, EEPROMEX_BLOCK3_DATA_QTY_BYTE, EEPROMEX_BLOCK3_ADDR_OFFSET))
			{
				EepromEx_SetErrorFlag(2u);
	        }
	        if(!EepromEx_InOutParameters((&Parameter.OCV_Fault), 		get, EEPROMEX_BLOCK4_DATA_QTY_BYTE, EEPROMEX_BLOCK4_ADDR_OFFSET))
			{
				EepromEx_SetErrorFlag(3u);
	        }
	        if(!EepromEx_InOutParameters((&Parameter.OCV_F_Release), 	get, EEPROMEX_BLOCK5_DATA_QTY_BYTE, EEPROMEX_BLOCK5_ADDR_OFFSET))
			{
				EepromEx_SetErrorFlag(4u);
	        }
			if(EepromEx_GetErrorFlag() == 0u) 
			{
				break;
			}
		}

		if(EepromEx_GetErrorFlag() != 0u)
		{
			setting configdata to default value  
			ConfigData_SetDefaultConfigData();
			
			for(Try_num = 0u; Try_num < EEPROMEX_WR_TRY_NUM; Try_num++)
			{					
				EepromEx_ResetErrorFlag();
				
				if(EepromEx_InOutParameters((&Parameter), 					set, EEPROMEX_BLOCK1_DATA_QTY_BYTE, EEPROMEX_BLOCK1_ADDR_OFFSET)) 
				{
					HAL_Delay(5u);
					if(!EepromEx_InOutParameters((&Parameter), 					get, EEPROMEX_BLOCK1_DATA_QTY_BYTE, EEPROMEX_BLOCK1_ADDR_OFFSET))
					{
						EepromEx_SetErrorFlag(0u);
					}		
				}
				else
				{
					EepromEx_SetErrorFlag(0u);
				}
				
				if(EepromEx_InOutParameters((&Parameter.OCV_Warning), 			set, EEPROMEX_BLOCK2_DATA_QTY_BYTE, EEPROMEX_BLOCK2_ADDR_OFFSET)) 
				{
					HAL_Delay(5u);
					if(!EepromEx_InOutParameters((&Parameter.OCV_Warning), 			get, EEPROMEX_BLOCK2_DATA_QTY_BYTE, EEPROMEX_BLOCK2_ADDR_OFFSET))
					{
						EepromEx_SetErrorFlag(1u);
					}		
				}
				else
				{
					EepromEx_SetErrorFlag(1u);
				}

				if(EepromEx_InOutParameters((&Parameter.OCV_W_Release), 		set, EEPROMEX_BLOCK3_DATA_QTY_BYTE, EEPROMEX_BLOCK3_ADDR_OFFSET)) 
				{
					HAL_Delay(5u);
					if(!EepromEx_InOutParameters((&Parameter.OCV_W_Release), 		get, EEPROMEX_BLOCK3_DATA_QTY_BYTE, EEPROMEX_BLOCK3_ADDR_OFFSET))
					{
						EepromEx_SetErrorFlag(2u);
					}		
				}
				else
				{
					EepromEx_SetErrorFlag(2u);
				}

				if(EepromEx_InOutParameters((&Parameter.OCV_Fault), 			set, EEPROMEX_BLOCK4_DATA_QTY_BYTE, EEPROMEX_BLOCK4_ADDR_OFFSET)) 
				{
					HAL_Delay(5u);
					if(!EepromEx_InOutParameters((&Parameter.OCV_Fault), 			get, EEPROMEX_BLOCK4_DATA_QTY_BYTE, EEPROMEX_BLOCK4_ADDR_OFFSET))
					{
						EepromEx_SetErrorFlag(3u);
					}		
				}
				else
				{
					EepromEx_SetErrorFlag(3u);
				}

				if(EepromEx_InOutParameters((&Parameter.OCV_F_Release), 		set, EEPROMEX_BLOCK5_DATA_QTY_BYTE, EEPROMEX_BLOCK5_ADDR_OFFSET)) 
				{
					HAL_Delay(5u);
					if(!EepromEx_InOutParameters((&Parameter.OCV_F_Release), 		get, EEPROMEX_BLOCK5_DATA_QTY_BYTE, EEPROMEX_BLOCK5_ADDR_OFFSET))
					{
						EepromEx_SetErrorFlag(4u);
					}		
				}
				else
				{
					EepromEx_SetErrorFlag(4u);
				}


				if(EepromEx_GetErrorFlag() == 0u)
				{
					(void)EepromEx_SetSavedParameterFlag(TRUE);
					HAL_Delay(5u);
					break;
				}
			}
		}		
	}
	else
	{
        ConfigData_SetDefaultConfigData();
		setting configdata to default value  
		for(Try_num = 0u; Try_num < EEPROMEX_WR_TRY_NUM; Try_num++)
		{					
			EepromEx_ResetErrorFlag();
			
			if(EepromEx_InOutParameters((&Parameter), 					set, EEPROMEX_BLOCK1_DATA_QTY_BYTE, EEPROMEX_BLOCK1_ADDR_OFFSET)) 
				{
					HAL_Delay(5u);
					if(!EepromEx_InOutParameters((&Parameter), 					get, EEPROMEX_BLOCK1_DATA_QTY_BYTE, EEPROMEX_BLOCK1_ADDR_OFFSET))
					{
						EepromEx_SetErrorFlag(0u);
					}		
				}
				else
				{
					EepromEx_SetErrorFlag(0u);
				}
				
				if(EepromEx_InOutParameters((&Parameter.OCV_Warning), 			set, EEPROMEX_BLOCK2_DATA_QTY_BYTE, EEPROMEX_BLOCK2_ADDR_OFFSET)) 
				{
					HAL_Delay(5u);
					if(!EepromEx_InOutParameters((&Parameter.OCV_Warning), 			get, EEPROMEX_BLOCK2_DATA_QTY_BYTE, EEPROMEX_BLOCK2_ADDR_OFFSET))
					{
						EepromEx_SetErrorFlag(1u);
					}		
				}
				else
				{
					EepromEx_SetErrorFlag(1u);
				}

				if(EepromEx_InOutParameters((&Parameter.OCV_W_Release), 		set, EEPROMEX_BLOCK3_DATA_QTY_BYTE, EEPROMEX_BLOCK3_ADDR_OFFSET)) 
				{
					HAL_Delay(5u);
					if(!EepromEx_InOutParameters((&Parameter.OCV_W_Release), 		get, EEPROMEX_BLOCK3_DATA_QTY_BYTE, EEPROMEX_BLOCK3_ADDR_OFFSET))
					{
						EepromEx_SetErrorFlag(2u);
					}		
				}
				else
				{
					EepromEx_SetErrorFlag(2u);
				}

				if(EepromEx_InOutParameters((&Parameter.OCV_Fault), 			set, EEPROMEX_BLOCK4_DATA_QTY_BYTE, EEPROMEX_BLOCK4_ADDR_OFFSET)) 
				{
					HAL_Delay(5u);
					if(!EepromEx_InOutParameters((&Parameter.OCV_Fault), 			get, EEPROMEX_BLOCK4_DATA_QTY_BYTE, EEPROMEX_BLOCK4_ADDR_OFFSET))
					{
						EepromEx_SetErrorFlag(3u);
					}		
				}
				else
				{
					EepromEx_SetErrorFlag(3u);
				}

				if(EepromEx_InOutParameters((&Parameter.OCV_F_Release), 		set, EEPROMEX_BLOCK5_DATA_QTY_BYTE, EEPROMEX_BLOCK5_ADDR_OFFSET)) 
				{
					HAL_Delay(5u);
					if(!EepromEx_InOutParameters((&Parameter.OCV_F_Release), 		get, EEPROMEX_BLOCK5_DATA_QTY_BYTE, EEPROMEX_BLOCK5_ADDR_OFFSET))
					{
						EepromEx_SetErrorFlag(4u);
					}		
				}
				else
				{
					EepromEx_SetErrorFlag(4u);
				}

			
			if(EepromEx_GetErrorFlag() == 0u)
			{
				(void)EepromEx_SetSavedParameterFlag(TRUE);
				HAL_Delay(5u);
				break;
			}
		}
	}
*/
}




