/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, RST_WDT_EN_Pin|RST_EXT_WDT_Pin|Iso_R_H_FET_EN_Pin|Iso_R_L_FET_EN_Pin
                          |Iso_PACK_FET_EN_Pin|Iso_BUS_FET_EN_Pin|FAN_CON_1_PWM_Pin|FAN_CON_1_H_EN_Pin
                          |Sub_SPI_CS_Pin|Sub_6820_RET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HW_EPO_EN_GPIO_Port, HW_EPO_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Main_SPI_CS_GPIO_Port, Main_SPI_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Main_6820_RET_GPIO_Port, Main_6820_RET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, PRE_CON_L_EN_Pin|PRE_CON_H_EN_Pin|Led_Yellow_Pin|Led_Red_Pin
                          |Led_Green_Pin|MAIN_CON_L1_L_EN_Pin|MAIN_CON_L1_H_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, IDCOM_Pin|MAIN_CON_H1_L_EN_Pin|MAIN_CON_H1_H_EN_Pin|Main_ADC2_CS_Pin
                          |Main_IEE_CS_Pin|Main_ADC1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RST_WDT_EN_Pin RST_EXT_WDT_Pin Iso_R_H_FET_EN_Pin Iso_R_L_FET_EN_Pin
                           Iso_PACK_FET_EN_Pin Iso_BUS_FET_EN_Pin FAN_CON_1_PWM_Pin FAN_CON_1_H_EN_Pin
                           Sub_SPI_CS_Pin Sub_6820_RET_Pin */
  GPIO_InitStruct.Pin = RST_WDT_EN_Pin|RST_EXT_WDT_Pin|Iso_R_H_FET_EN_Pin|Iso_R_L_FET_EN_Pin
                          |Iso_PACK_FET_EN_Pin|Iso_BUS_FET_EN_Pin|FAN_CON_1_PWM_Pin|FAN_CON_1_H_EN_Pin
                          |Sub_SPI_CS_Pin|Sub_6820_RET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : HW_EPO_EN_Pin */
  GPIO_InitStruct.Pin = HW_EPO_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HW_EPO_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Sig_HW_EPO_CTL_Pin */
  GPIO_InitStruct.Pin = Sig_HW_EPO_CTL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Sig_HW_EPO_CTL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Main_SPI_CS_Pin */
  GPIO_InitStruct.Pin = Main_SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Main_SPI_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Main_6820_RET_Pin */
  GPIO_InitStruct.Pin = Main_6820_RET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Main_6820_RET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PRE_CON_L_EN_Pin PRE_CON_H_EN_Pin Led_Yellow_Pin Led_Red_Pin
                           Led_Green_Pin MAIN_CON_L1_L_EN_Pin MAIN_CON_L1_H_EN_Pin */
  GPIO_InitStruct.Pin = PRE_CON_L_EN_Pin|PRE_CON_H_EN_Pin|Led_Yellow_Pin|Led_Red_Pin
                          |Led_Green_Pin|MAIN_CON_L1_L_EN_Pin|MAIN_CON_L1_H_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : IDSEG0_Pin IDSEG1_Pin IDSEG2_Pin IDSEG3_Pin
                           IDSEG4_Pin IDSEG5_Pin */
  GPIO_InitStruct.Pin = IDSEG0_Pin|IDSEG1_Pin|IDSEG2_Pin|IDSEG3_Pin
                          |IDSEG4_Pin|IDSEG5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : IDCOM_Pin MAIN_CON_H1_L_EN_Pin MAIN_CON_H1_H_EN_Pin Main_ADC2_CS_Pin
                           Main_IEE_CS_Pin Main_ADC1_CS_Pin */
  GPIO_InitStruct.Pin = IDCOM_Pin|MAIN_CON_H1_L_EN_Pin|MAIN_CON_H1_H_EN_Pin|Main_ADC2_CS_Pin
                          |Main_IEE_CS_Pin|Main_ADC1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : M1__Aux_Con_Pin M1__Aux_ConA10_Pin */
  GPIO_InitStruct.Pin = M1__Aux_Con_Pin|M1__Aux_ConA10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
