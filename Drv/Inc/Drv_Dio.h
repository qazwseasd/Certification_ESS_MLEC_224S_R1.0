/*
 * Drv_Dio.h
 *
 *  Created on: Sep 11, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/
#ifndef INC_DRV_DIO_H_
#define INC_DRV_DIO_H_

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
typedef enum
{
    LED_ALL,
    LED_YELLOW,
    LED_GREEN,
    LED_RED,
}LED_SELECT;

/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void  Dio_Earn_BoardID(void);
extern uint8 Dio_GetHVILEPO(void);
extern void Dio_SetFET_ForPackVolt(bool OnOff);
extern void Dio_SetFET_ForBusVolt(bool OnOff);
extern void Dio_SetFET_ForIsoR_High(bool OnOff);
extern void Dio_SetFET_ForIsoR_Low(bool OnOff);
extern uint8 Dio_GetFET_ForIsoR_High(void);
extern uint8 Dio_GetFET_ForIsoR_Low(void);
extern void Dio_Clear_ADCChipSelectPin(uint8 port, uint8 pin);
extern void Dio_Set_ADCChipSelectPin(uint8 port, uint8 pin);
extern void Dio_Set_Self_HVIL_Ctrl(bool OnOff);
void	Dio_Init(void);
void	Dio_Toggle(GPIO_TypeDef *port, uint16_t pin);
void	Dio_Set(GPIO_TypeDef *port, uint16_t pin);
void	Dio_Clr(GPIO_TypeDef *port, uint16_t pin);
extern uint16 Dio_Get_BoardID(void);



#endif /* INC_DRV_DIO_H_ */
