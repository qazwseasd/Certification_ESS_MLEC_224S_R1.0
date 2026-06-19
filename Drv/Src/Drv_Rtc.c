/*
 * Drv_Rtc.c
 *
 *  Created on: Sep 15, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Rtc.h"


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
void Drv_Rtc_SetTime(uint8 hour, uint8 min, uint8 sec)
{
    RTC_TimeTypeDef sTime;
    sTime.Hours = hour;
    sTime.Minutes = min;
    sTime.Seconds = sec;
    (void)HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}

void Drv_Rtc_SetDate(uint8 year, uint8 month, uint8 date, uint8 weekday)
{
    RTC_DateTypeDef sDate;
    sDate.Year = year;
    sDate.Month = month;
    sDate.Date = date;
    sDate.WeekDay = weekday;
    (void)HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

HAL_StatusTypeDef Drv_Rtc_GetTime(RTC_TimeTypeDef *time)
{
    return HAL_RTC_GetTime(&hrtc, time, RTC_FORMAT_BIN);
}

HAL_StatusTypeDef Drv_Rtc_GetDate(RTC_DateTypeDef *date)
{
    return HAL_RTC_GetDate(&hrtc, date, RTC_FORMAT_BIN);
}

