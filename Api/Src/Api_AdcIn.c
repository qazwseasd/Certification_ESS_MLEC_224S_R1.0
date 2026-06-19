/*
 * Api_AdcIn.c
 *
 *  Created on: Sep 9, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Api_Adcin.h"

/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/

/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/
typedef enum {
    TEMP_SM_IDLE = 0,
    TEMP_SM_START,        // ADC 변환 시작
    TEMP_SM_WAIT,         // 변환 완료 대기 (IRQ/DMA 플래그 또는 폴링)
    TEMP_SM_ACCUMULATE,   // N회 샘플 누적
    TEMP_SM_CONVERT,      // 전압→온도 변환, 보정/필터
    TEMP_SM_DONE,         // 결과 준비 완료
    TEMP_SM_ERROR
} TempSmState;


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/
extern ADC_HandleTypeDef hadc3;

/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/
#define TaskPeriod_ADC 200u  // [per/ms]
#define ConvWaitTime   200u // [per/ms]
#define ConvTimeout    (ConvWaitTime/TaskPeriod_ADC) 
#define NUM_OF_CONVERSION_ADC3 1u

/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/
void Api_ADC_Config_SingleChannel(ADC_HandleTypeDef *hadc, uint32_t ch, uint32_t samp);
//AdcSmState Api_ADC_Read_SingleChannel(ADC_HandleTypeDef *hadc, uint32_t ch, uint32_t samp);


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/

void Api_ADC_Config_SingleChannel(ADC_HandleTypeDef *hadc, uint32_t ch, uint32_t samp) 
{
    ADC_ChannelConfTypeDef s = {0};
    s.Channel      = ch;                        // 예: ADC_CHANNEL_4/5/6 ...
    s.Rank         = 1;                         // L=1이므로 SQ1만 사용
    s.SamplingTime = samp;                      // 채널별 샘플링타임
    (void)HAL_ADC_ConfigChannel(hadc, &s);           // SQRx/SMPRx 갱신
}


AdcSmState Api_ADC3_Read_SingleChannel(void) 
{   
    static AdcSmState ADC_SM_State = ADC_SM_IDLE;
    static uint16 delayCnt = 0;
    switch (ADC_SM_State)
    {
        case ADC_SM_IDLE:
        	Api_ADC_Config_SingleChannel(&hadc3, 8, ADC_SAMPLETIME_56CYCLES);
        	ADC_SM_State = ADC_SM_START;
        	break;
            
        case ADC_SM_START:   // 1) 변환 시작 (SWSTART)
            if (HAL_ADC_Start(&hadc3) != HAL_OK)
            {
                ADC_SM_State = ADC_SM_ERROR;
            }
            else{ 
                ADC_SM_State = ADC_SM_WAIT;
            }
            break;
            
        case ADC_SM_WAIT:    // 2) 변환 완료 대기 (폴링)
            if(delayCnt < ConvTimeout) 
			{
				delayCnt++;
			}
            else
            {
                delayCnt = 0;
                if (HAL_ADC_PollForConversion(&hadc3, 10) != HAL_OK)
                {
    			    (void)HAL_ADC_Stop(&hadc3);
    			    ADC_SM_State = ADC_SM_ERROR;
    		    }
                else{
                    AdcIn_Adc3Buff[0] = (uint16_t)HAL_ADC_GetValue(&hadc3);
                    ADC_SM_State = ADC_SM_DONE;
                }
            }        
            break;
            
        case ADC_SM_DONE:   // 3) 결과 읽기 (DR)
    		 ADC_SM_State = ADC_SM_IDLE;   
            break;
        
         case ADC_SM_ERROR:
            break;
        default :
        	break;
    }

   return ADC_SM_State;
}

uint16_t Api_ADC3_GetValue(void)
{
    return (uint16_t)HAL_ADC_GetValue(&hadc3);
}



