/*
 * Drv_AdcEx.h
 *
 *  Created on: Sep 11, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/
#ifndef INC_DRV_ADCEX_H_
#define INC_DRV_ADCEX_H_

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Drv_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define ADCEX_ADS7844_CH0	0x00u
#define ADCEX_ADS7844_CH1	0x40u
#define ADCEX_ADS7844_CH2	0x10u
#define ADCEX_ADS7844_CH3	0x50u
#define ADCEX_ADS7844_CH4	0x20u
#define ADCEX_ADS7844_CH5	0x60u
#define ADCEX_ADS7844_CH6	0x30u
#define ADCEX_ADS7844_CH7	0x70u
								
#define ADCEX_ADS7841_CH0	0x10u
#define ADCEX_ADS7841_CH1	0x50u
#define ADCEX_ADS7841_CH2	0x20u
#define ADCEX_ADS7841_CH3	0x60u

#define ADCEX_CH_ISOR_H		ADCEX_ADS7841_CH0
#define ADCEX_CH_ISOR_L		ADCEX_ADS7841_CH1
#define ADCEX_CH_PACKVOLT	ADCEX_ADS7841_CH2
#define ADCEX_CH_BUSVOLT	ADCEX_ADS7841_CH3

#define ADCEX_CH_CURR_LOW		ADCEX_ADS7844_CH0
#define ADCEX_CH_CURR_HIGH		ADCEX_ADS7844_CH1
#define ADCEX_CH_CURR_LOW_Sub	ADCEX_ADS7844_CH2
#define ADCEX_CH_CURR_HIGH_Sub	ADCEX_ADS7844_CH3
#define ADCEX_CH_REF5V			ADCEX_ADS7844_CH5
#define ADCEX_CH_AUX5V			ADCEX_ADS7844_CH6
#define ADCEX_CH_SiGAUX			ADCEX_ADS7844_CH7




/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/

/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern uint8 AdcEx_Init(GPIO_TypeDef* cs_port, uint16 cs_pin);
extern uint16  AdcEx_GetRawData(GPIO_TypeDef* cs_port, uint16 cs_pin, uint8 ch);


#endif /* INC_DRV_ADCEX_H_ */
