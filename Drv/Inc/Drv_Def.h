/*
 * Drv_Def.h
 *
 *  Created on: Aug 20, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/

#ifndef INC_DRV_DEF_H_
#define INC_DRV_DEF_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Common_Def.h"
#include "Common_Include.h"
// #include "Common_Debug.h"

#include "Drv_Init.h"
#include "Drv_Tick.h"
#include "Drv_Dwt.h"
#include "Drv_Led.h"
#include "Drv_Wdt.h"
#include "Drv_Can.h"
#include "Drv_AdcIn.h"
#include "Drv_AdcEx.h"
#include "Drv_Delay.h"
#include "Drv_Adbms6815.h"
#include "Drv_Dio.h"
#include "Drv_Spi.h"
#include "Drv_Fan.h"
#include "Drv_Relay.h"
#include "Drv_Rtc.h"
#include "Drv_EepromEx.h"
#include "Drv_Parameter.h"



/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/

/* ADC	*/
#define DRV_AdcIn_Num_Adc3Ch			3u

/* Modif By SKAHN _250908_SAT */
/*
#define DRV_AdcIn_Adc3Ch_VAux			ADC_CHANNEL_6
#define DRV_AdcIn_Adc3Ch_VPwr			ADC_CHANNEL_7
#define DRV_AdcIn_Adc3Ch_TBoard			ADC_CHANNEL_8
*/
#define AdcIn_Adc3Buff_Len		16u

/* Modif By SKAHN _250908_END */


/* CAN	*/
#define DRV_Can_RxSize_FrameN			8u		/* [byte], CAN RX frame size : normal */

#define DRV_Can_1_IdType				CAN_ID_EXT
#define DRV_Can_2_IdType				CAN_ID_EXT

#define DRV_Can_1_Filter1				0x18FF01CAUL
#define DRV_Can_1_Mask1					0xFFF00000UL

#define DRV_Can_1_Filter2				0x18AAAAF3UL
#define DRV_Can_1_Mask2					0xFFFFF0FFUL

#define DRV_Can_2_Filter1				0x1CFF01ECUL
#define DRV_Can_2_Mask1					0xFFFF00FFUL

#define DRV_Can_2_Filter_StartBank		14u		/* 0 ~ 27	*/


/* LED	*/
#define DRV_Led_NumOf					3u

#define DRV_Led_Yellow					0u
#define DRV_Led_Red						1u
#define DRV_Led_Green					2u

#if (DRV_Led_NumOf >= 1u)
#define DRV_Led_1_Port					Led_Yellow_GPIO_Port
#define DRV_Led_1_Pin					Led_Yellow_Pin
#endif
#if (DRV_Led_NumOf >= 2u)
#define DRV_Led_2_Port					Led_Red_GPIO_Port
#define DRV_Led_2_Pin					Led_Red_Pin
#endif
#if (DRV_Led_NumOf >= 3u)
#define DRV_Led_3_Port					Led_Green_GPIO_Port
#define DRV_Led_3_Pin					Led_Green_Pin
#endif
#if (DRV_Led_NumOf >= 4u)
#define DRV_Led_4_Port					()
#define DRV_Led_4_Pin					()
#endif
#if (DRV_Led_NumOf >= 5u)
#define DRV_Led_5_Port					()
#define DRV_Led_5_Pin					()
#endif

/* The following LED periods assume
 * that the cycle time of the Led_Task() function is 100 ms.
 * If the cycle time of Led_Task() is modified,
 * the LED periods below must also be adjusted accordingly. */
#define DRV_Led_Peri_0ms				 0u
#define DRV_Led_Peri_100ms				 1u
#define DRV_Led_Peri_200ms				 2u
#define DRV_Led_Peri_500ms				 5u
#define DRV_Led_Peri_1000ms				10u
#define DRV_Led_Peri_1500ms				15u
#define DRV_Led_Peri_2000ms				20u


/* External WDT */
#define Wdt_Reset_Pin                   GPIO_PIN_5
#define Wdt_Reset_GPIO_Port             GPIOE


/* Pack	*/
#define DRV_Pack_NumOf					1
#define DRV_Pack_MinNumOfForDrive		1

/* RTC (Real Time Clock)	*/
#define DRV_Rtc_Year					25u
#define DRV_Rtc_Month					RTC_MONTH_JULY
#define DRV_Rtc_Date					 1u
#define DRV_Rtc_Hour					13u
#define DRV_Rtc_Minute					49u
#define DRV_Rtc_Second					11u
#define DRV_Rtc_WeekDay					RTC_WEEKDAY_TUESDAY

/* SPI         */
//#define HANDLE_SPI_EEPROM            &hspi2;

/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/

#endif /* INC_DRV_DEF_H_ */
