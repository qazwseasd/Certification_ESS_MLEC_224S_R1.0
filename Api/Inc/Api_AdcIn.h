/*
 * Api_AdcIn.h
 *
 *  Created on: Sep 9, 2025
 *      Author: ETI
 */

#ifndef INC_API_ADCIN_H_
#define INC_API_ADCIN_H_

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Api_Def.h"

/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define ADC3_Rank1          0u
#define ADC3_Rank2          1u
#define ADC3_Rank3          2u

#define ADC3_IN8_INDEX              ADC3_Rank1
#define DRV_AdcIn_Adc3Ch_TBoard		ADC3_IN8_INDEX

/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/

// ---- 측정 상태 정의 ----
typedef enum {
    ADC_SM_IDLE = 0,
    ADC_SM_READY,
	ADC_SM_START,        // ADC 변환 시작
	ADC_SM_WAIT,         // 변환 완료 대기 (IRQ/DMA 플래그 또는 폴링)
	ADC_SM_ACCUMULATE,   // N회 샘플 누적
	ADC_SM_CONVERT,      // 전압→온도 변환, 보정/필터
	ADC_SM_DONE,         // 결과 준비 완료
	ADC_SM_ERROR
} AdcSmState;

// ---- 측정 상태 정의 ----
typedef enum {
    
    SM_Init = 0,
    SM_READY,
	SM_START,        // ADC 변환 시작
	SM_WAIT,         // 변환 완료 대기 (IRQ/DMA 플래그 또는 폴링)
	SM_ACCUMULATE,   // N회 샘플 누적
	SM_CONVERT,      // 전압→온도 변환, 보정/필터
	SM_DONE,         // 결과 준비 완료
	SM_ERROR
} AdcDMASmState;


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern uint16_t Api_ADC3_GetValue(void);
//AdcSmState Api_ADC_Read_SingleChannel(ADC_HandleTypeDef *hadc, uint32_t ch, uint32_t samp);
extern AdcSmState Api_ADC3_Read_SingleChannel(void);

#endif /* INC_API_ADCIN_H_ */
