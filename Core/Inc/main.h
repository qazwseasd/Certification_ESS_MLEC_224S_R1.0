/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RST_WDT_EN_Pin GPIO_PIN_4
#define RST_WDT_EN_GPIO_Port GPIOE
#define RST_EXT_WDT_Pin GPIO_PIN_5
#define RST_EXT_WDT_GPIO_Port GPIOE
#define HW_EPO_EN_Pin GPIO_PIN_0
#define HW_EPO_EN_GPIO_Port GPIOF
#define Sig_HW_EPO_CTL_Pin GPIO_PIN_1
#define Sig_HW_EPO_CTL_GPIO_Port GPIOF
#define MAIN_CON_L1_AD_Pin GPIO_PIN_4
#define MAIN_CON_L1_AD_GPIO_Port GPIOF
#define MAIN_CON_H1_AD_Pin GPIO_PIN_5
#define MAIN_CON_H1_AD_GPIO_Port GPIOF
#define PRE_CON_AD_Pin GPIO_PIN_6
#define PRE_CON_AD_GPIO_Port GPIOF
#define FAN_CON_AD_Pin GPIO_PIN_7
#define FAN_CON_AD_GPIO_Port GPIOF
#define HW_EPO_CTL_AD_Pin GPIO_PIN_9
#define HW_EPO_CTL_AD_GPIO_Port GPIOF
#define BD_TEMP_AD_Pin GPIO_PIN_10
#define BD_TEMP_AD_GPIO_Port GPIOF
#define Main_SPI_CS_Pin GPIO_PIN_4
#define Main_SPI_CS_GPIO_Port GPIOA
#define Main_6820_RET_Pin GPIO_PIN_4
#define Main_6820_RET_GPIO_Port GPIOC
#define Iso_R_H_FET_EN_Pin GPIO_PIN_8
#define Iso_R_H_FET_EN_GPIO_Port GPIOE
#define Iso_R_L_FET_EN_Pin GPIO_PIN_9
#define Iso_R_L_FET_EN_GPIO_Port GPIOE
#define Iso_PACK_FET_EN_Pin GPIO_PIN_10
#define Iso_PACK_FET_EN_GPIO_Port GPIOE
#define Iso_BUS_FET_EN_Pin GPIO_PIN_11
#define Iso_BUS_FET_EN_GPIO_Port GPIOE
#define FAN_CON_1_PWM_Pin GPIO_PIN_13
#define FAN_CON_1_PWM_GPIO_Port GPIOE
#define FAN_CON_1_H_EN_Pin GPIO_PIN_15
#define FAN_CON_1_H_EN_GPIO_Port GPIOE
#define PRE_CON_L_EN_Pin GPIO_PIN_10
#define PRE_CON_L_EN_GPIO_Port GPIOD
#define PRE_CON_H_EN_Pin GPIO_PIN_11
#define PRE_CON_H_EN_GPIO_Port GPIOD
#define Led_Yellow_Pin GPIO_PIN_13
#define Led_Yellow_GPIO_Port GPIOD
#define Led_Red_Pin GPIO_PIN_14
#define Led_Red_GPIO_Port GPIOD
#define Led_Green_Pin GPIO_PIN_15
#define Led_Green_GPIO_Port GPIOD
#define IDSEG0_Pin GPIO_PIN_2
#define IDSEG0_GPIO_Port GPIOG
#define IDSEG1_Pin GPIO_PIN_3
#define IDSEG1_GPIO_Port GPIOG
#define IDSEG2_Pin GPIO_PIN_4
#define IDSEG2_GPIO_Port GPIOG
#define IDSEG3_Pin GPIO_PIN_5
#define IDSEG3_GPIO_Port GPIOG
#define IDSEG4_Pin GPIO_PIN_6
#define IDSEG4_GPIO_Port GPIOG
#define IDSEG5_Pin GPIO_PIN_7
#define IDSEG5_GPIO_Port GPIOG
#define IDCOM_Pin GPIO_PIN_8
#define IDCOM_GPIO_Port GPIOG
#define M1__Aux_Con_Pin GPIO_PIN_9
#define M1__Aux_Con_GPIO_Port GPIOA
#define M1__Aux_ConA10_Pin GPIO_PIN_10
#define M1__Aux_ConA10_GPIO_Port GPIOA
#define MAIN_CON_L1_L_EN_Pin GPIO_PIN_6
#define MAIN_CON_L1_L_EN_GPIO_Port GPIOD
#define MAIN_CON_L1_H_EN_Pin GPIO_PIN_7
#define MAIN_CON_L1_H_EN_GPIO_Port GPIOD
#define MAIN_CON_H1_L_EN_Pin GPIO_PIN_9
#define MAIN_CON_H1_L_EN_GPIO_Port GPIOG
#define MAIN_CON_H1_H_EN_Pin GPIO_PIN_10
#define MAIN_CON_H1_H_EN_GPIO_Port GPIOG
#define Main_ADC2_CS_Pin GPIO_PIN_12
#define Main_ADC2_CS_GPIO_Port GPIOG
#define Main_IEE_CS_Pin GPIO_PIN_13
#define Main_IEE_CS_GPIO_Port GPIOG
#define Main_ADC1_CS_Pin GPIO_PIN_15
#define Main_ADC1_CS_GPIO_Port GPIOG
#define Sub_SPI_CS_Pin GPIO_PIN_0
#define Sub_SPI_CS_GPIO_Port GPIOE
#define Sub_6820_RET_Pin GPIO_PIN_1
#define Sub_6820_RET_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
