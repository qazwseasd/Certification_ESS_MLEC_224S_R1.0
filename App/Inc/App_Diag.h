/*
 * App_Diag.h
 *
 *  Created on: Sep 22, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/
#ifndef INC_APP_DIAG_H_
#define INC_APP_DIAG_H_

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/
typedef struct 
{
	uint32_t cntIntrADC;
	uint32_t cntIntrCANRx;
	uint32_t cntIntrBMIC1Rx;
	uint32_t cntIntrBMIC1Tx;
	uint32_t cntIntrBMIC2Rx;
	uint32_t cntIntrBMIC2Tx;
	uint32_t cntIntrSPITx;
	uint32_t cntIntrSPIRx;
} STL_CntInterrupt;

typedef struct 
{
	uint32 	FailCode;
	uint32 	crcNum;
	
}CLASSB_DATA; 


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/
extern STL_CntInterrupt StlCntInterrupt;


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern uint32	Diag_Init(void);
extern void	Diag_Task(void);
extern void Set_TROfflineVCell(BOOL SetV);
extern BOOL Get_TROfflineVCell(void);
extern void Set_TROfflineTCell(BOOL SetV);
extern BOOL Get_TROfflineTCell(void);
extern void Set_TROfflineBMIC(BOOL SetV);
extern BOOL Get_TROfflineBMIC(void);
extern uint32 Diag_Get_ClassbFailcode(void);
extern uint32 Diag_Get_FlagsInfo(void);
extern uint32 Diag_Get_FlagsWarn(void);
extern uint32 Diag_Get_FlagsFault(void);
extern void Diag_Set_FlagsFault(uint32 fault);
extern uint32 Diag_Get_FlagsPFault(void);
extern void Diag_Set_FlagsPFault(uint32 Pfault);



#ifdef IEC60730_LIB
#include "stl_user_api.h"

extern CLASSB STL_MemSubset_t FlashSubsetRunTime;
extern CLASSB STL_MemConfig_t FlashConfigRunTime;
extern CLASSB STL_MemSubset_t RamSubsetRunTime;
extern CLASSB STL_MemConfig_t RamConfigRunTime;
extern CLASSB STL_TmStatus_t StlCpuTm1LStatus;
extern CLASSB STL_TmStatus_t StlCpuTm7Status;
extern CLASSB STL_TmStatus_t StlCpuTmCBStatus;
extern CLASSB STL_TmStatus_t StlFlashStatus;
extern CLASSB STL_TmStatus_t StlRamStatus;
extern CLASSB STL_TmStatus_t StlClkStatus;
extern CLASSB volatile uint16_t tmpCC4_last;
extern CLASSB volatile uint32_t PeriodValue;
extern CLASSB volatile uint32_t PeriodValueInv;
extern CLASSB volatile uint32_t LSIPeriodFlag;
extern CLASSB_CRC const uint32_t ClassB_Golden_CRC;


extern void STL_Init(void);
extern void STL_RunFullTest(void);
extern void IEC60730_Task(void);
//extern const unsigned int __ICFEDIT_region_CLASSB_start__;
//extern const unsigned int __ICFEDIT_region_CLASSB_end__;

/* Value of the system clock frequency at run time in Hz */
#define SYSTCLK_AT_RUN (uint32_t)(168000000uL)
/* Value of the Internal LSI oscillator in Hz */
#define LSI_Freq    ((uint32_t)32000uL)
/* CLK frequency above this limit considered as harmonics in case of HSE */
#define CLK_LimitHigh(fcy) ((uint32_t)(((fcy)/LSI_Freq)*4u*5u)/4u) /* (Value + 25%) */
/* CLK frequency below this limit considered as sub-harmonics in case of HSE */
#define CLK_LimitLow(fcy) ((uint32_t)(((fcy)/LSI_Freq)*4u*3u)/4u)  /* (Value - 25%) */
/* if SYSCK is derived from HSI, the upper fixed clock limits can be set more severe
   e.g., ...*2u*6u)/5 resp. ...*2u*4u)/5 (~ value +/- 20%) but such a severe restriction
   can require including some adaptive flow to compensate possible temperature drift
   of HSI by making acceptable window defined by these limits variable in time */

//#define TEST_RAM_START_ADDR ((uint32_t)&__ICFEDIT_region_CLASSB_start__)
//#define TEST_RAM_END_ADDR_FULL ((uint32_t)&__ICFEDIT_region_CLASSB_end__)
#define TEST_RAM_START_ADDR 0x20010000UL
#define TEST_RAM_END_ADDR_FULL 0x2001FFFFUL
#define TEST_ROM_START_ADDR 0x8000000UL
#pragma section="ROM_CONTENT"
#define TEST_ROM_END_ADDR ((uint32_t)__section_end("ROM_CONTENT"))
#define TEST_RAM_SECTION_NB_RUN 8u    /* only 8 RAM sections are tested during runtime. */
#define RAM_SECTION_SIZE 128U
#endif


#endif /* INC_APP_DIAG_H_ */
