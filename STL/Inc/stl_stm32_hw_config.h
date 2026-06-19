/*
  ******************************************************************************
  * @file    stl_stm32_hw_config.h
  * @author  MCD Application Team
  * @brief   Header file of STM32 HW configuration
  *          In order to be CMSIS agnostic, HW resources used by STL are re-defined here
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STL_STM32_HW_CONFIG_H
#define STL_STM32_HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  volatile uint32_t DR;          /*!< CRC Data register,                           Address offset: 0x00 */
  volatile uint8_t  IDR;         /*!< CRC Independent data register,               Address offset: 0x04 */
  uint8_t           RESERVED0;   /*!< Reserved,                                                    0x05 */
  uint16_t          RESERVED1;   /*!< Reserved,                                                    0x06 */
  volatile uint32_t CR;          /*!< CRC Control register,                        Address offset: 0x08 */
} STL_Wrapper_CRC_TypeDef;


typedef struct
{
  volatile uint32_t CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
  volatile uint32_t PLLCFGR;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
  volatile uint32_t CFGR;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
  volatile uint32_t CIR;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
  volatile uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
  volatile uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
  volatile uint32_t AHB3RSTR;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
  uint32_t      RESERVED0;     /*!< Reserved, 0x1C                                                                    */
  volatile uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
  volatile uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
  uint32_t      RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
  volatile uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
  volatile uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
  volatile uint32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
  uint32_t      RESERVED2;     /*!< Reserved, 0x3C                                                                    */
  volatile uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
  volatile uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
  uint32_t      RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
  volatile uint32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
  volatile uint32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
  volatile uint32_t AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
  uint32_t      RESERVED4;     /*!< Reserved, 0x5C                                                                    */
  volatile uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
  volatile uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
  uint32_t      RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
  volatile uint32_t BDCR;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
  volatile uint32_t CSR;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
  uint32_t      RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
  volatile uint32_t SSCGR;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
  volatile uint32_t PLLI2SCFGR;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
  volatile uint32_t PLLSAICFGR;    /*!< RCC PLLSAI configuration register,                           Address offset: 0x88 */
  volatile uint32_t DCKCFGR;       /*!< RCC Dedicated Clocks configuration register,                 Address offset: 0x8C */
} STL_Wrapper_RCC_TypeDef;


/* Exported constants --------------------------------------------------------*/
#define STL_WRP_PERIPH_BASE                        (0x40000000UL)

#define STL_WRP_AHB1PERIPH_BASE                    (STL_WRP_PERIPH_BASE + 0x00020000UL)
#define STL_WRP_CRC_BASE                           (STL_WRP_AHB1PERIPH_BASE + 0x00003000UL)
#define STL_WRP_RCC_BASE                           (STL_WRP_AHB1PERIPH_BASE + 0x00003800UL)

#define STL_WRP_CRC                                ((STL_Wrapper_CRC_TypeDef *) STL_WRP_CRC_BASE)
#define STL_WRP_RCC                                ((STL_Wrapper_RCC_TypeDef *) STL_WRP_RCC_BASE)

#define STL_WRP_CRC_CR_DR_RESET                    0x00000001U

#define STL_WRP_RCC_AHB1ENR_CRCEN_Pos              (12UL)
#define STL_WRP_RCC_AHB1ENR_CRCEN_Msk              (0x1U << STL_WRP_RCC_AHB1ENR_CRCEN_Pos)   /*!< 0x00001000 */
#define STL_WRP_RCC_AHB1ENR_CRCEN                  STL_WRP_RCC_AHB1ENR_CRCEN_Msk

/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* STL_STM32_HW_CONFIG_H */
