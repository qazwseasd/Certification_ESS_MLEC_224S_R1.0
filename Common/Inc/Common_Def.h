/*
 * Common_Def.h
 *
 *  Created on: Aug 18, 2025
 *      Author: ETI
 */

/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/
#ifndef INC_COMMON_DEF_H_
#define INC_COMMON_DEF_H_

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "Common_Include.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/

#define DEF_False						false
#define DEF_True						true

#define DEF_Ok							0u
#define DEF_Error						1u

#define DEF_Disable						false
#define DEF_Enable						true

#define DEF_Off							false
#define DEF_On							true

#define DEF_Close						false
#define DEF_Open						true

#define DEF_Voltage_Resolution			10
#define DEF_Grouping_Minimum_DeltaV		50	* DEF_Voltage_Resolution	/* [V] */

#define DEF_MIN(a,b)                	((a) < (b) ? (a) : (b))


#define DEF_MAX_NUM_OF_CELL_PER_BMIC	12u	
#define DEF_MAX_NUM_OF_TEMP_PER_BMIC	7u

#define DEF_MAX_NUM_OF_BMIC				42u	

#define OFF								false
#define ON								true

#define VIRTUAL							false
#define REAL							true

#define nop()							{  asm("NOP"); }	//_NOP()
#define WDR()	 						{  asm("wdr"); }

#define SET_BIT_Func(DATA, BIT)				((DATA) |= ((uint16)1u << (BIT)))	// Set Bit     
#define CLR_BIT_Func(DATA, BIT)  			((DATA) &= ~((uint16)1u << (BIT)))	// Clear Bit
#define TGL_BIT_Func(DATA, BIT)  			((DATA) ^= ((uint16)1u << (BIT)))	// Toggle Bit
#define CHK_BIT_Func(DATA, BIT)				((DATA) & ((uint16)1u << (BIT)))	// Check Bit

/* --- Boolean Macros --- */
#define FALSE							false
#define TRUE							true
#define BOOL							bool
/* --- Bit Operation Macros --- */

#define SET_PORT_PIN(PORT, PIN)    	     HAL_GPIO_WritePin(PORT, PIN, GPIO_PIN_SET);   
#define CLR_PORT_PIN(PORT, PIN)    	     HAL_GPIO_WritePin(PORT, PIN, GPIO_PIN_RESET);   

#define READ_PIN_INPUT(PORT, PIN_MASK)   (((PORT)->IDR & (PIN_MASK)) ? 1U : 0U)
#define READ_PIN_OUTPUT(PORT, PIN_MASK)  (((PORT)->ODR & (PIN_MASK)) ? 1U : 0U)
//#define READ_PIN_INPUT(PORT, PIN)        HAL_GPIO_ReadPin(PORT, PIN);


/* --- Math Helper Macros --- */
#define ABS(a)                           (((a) >= 0) ?  (a) : -(a))
#define MIN(a,b)                         (((a) < (b)) ? (a) : (b))
#define MAX(a,b)                         (((a) > (b)) ? (a) : (b))


#define FILTER(old, new, coef)  		( ( ((float)(old)) * ((float)(coef)) ) \
										    + ( ((float)(new)) * (1.0F - ((float)(coef))) ) )


#define DEF_NUMBER_OF_MOVING			8u								//
#define	DEF_MOVING_AVERAGE_SHIFT		3u	

#define EMPTY							false
#define SAVED							true

#define Version							10u


/* 1000V Mode */
 #define Drawer_14ea


/* Debug Mode Setting */
// #define Debuging_Mode


/* Functional Safety Test */
// #define TestMode
// #define Cell_OverVoltage_Test
// #define Cell_UnderVoltage_Test
// #define Pack_OverVoltage_Test
// #define Pack_UnderVoltage_Test
// #define Temp_Over_Test
// #define Temp_Under_Test
// #define Current_Test
// #define Isolation_Test
// #define BMIC_Error_Test
// #define BMIC_OPENError_Test
// #define Dead_Fuse
// #define Dead_Cell
// #define Aux_Voltage_Test
// #define PackV_CellV


/* Module Test */
// #define Test_PackVolt_Measurement
// #define Test_BusVolt_Measurement
// #define Test_Pack1V_Gain
// #define Test_BusV_Gain
// #define Test_PackVolt_MeasSeq

// #define Test_Curr_Init
// #define Test_Curr_Measurement_1
// #define Test_Curr_Measurement_low
// #define Test_Curr_Measurement_high
// #define Test_Curr_Measurement_zero
// #define Test_Curr_Calculation_Task
// #define Test_Curr_Calculation_Task_2

// #define Test_IsoR_IsMeasurement_1_On
// #define Test_IsoR_IsMeasurement_1_Off
// #define Test_IsoR_IsMeasurement_2
// #define Test_IsoR_Measurement 
// #define Test_IsoR_Measurement_2 
// #define Test_IsoR_Measurement_3 

// #define Test_Adbms_Read_Task
// #define Test_Adbms_Calculation_Task 
// #define Test_Adbms_OrganizeCellVolt
// #define Test_Adbms_SelectCellMaxMinVolt
// #define Test_Adbms_AnalogToBoardTempConversion
// #define Test_Adbms_UpdateFanState
// #define Test_Adbms_UpdateCellBalancingStatus
// #define Test_Adbms_Measurement

// #define Test_Adbms6815_ReadConfig_A
// #define Test_Adbms6815_ReadCellVolt
// #define Test_Adbms6815_ReadAuxVolt
// #define Test_Adbms6815_ReadStatus
// #define Test_Adbms6815_ReadDataGroup
// #define Test_Adbms6815_CalculateCRC10
// #define Test_Adbms6815_SPI_Error_Check

// #define Test_System_InitSequence
// #define Test_System_State_Task_SYS_EMERGENCY

// #define Test_Diag_Init
// #define Test_Diag_Module
// #define Test_Diag_VOverCell
// #define Test_Diag_VOverPack
// #define Test_Diag_VOverPack_224S
// #define Test_Diag_VUnderCell
// #define Test_Diag_VUnderPack
// #define Test_Diag_VUnderPack_224S
// #define Test_Diag_TOverCellChg
// #define Test_Diag_TOverCellDch
// #define Test_Diag_TUnderCellChg
// #define Test_Diag_TUnderCellDch
// #define Test_Diag_IOverChg
// #define Test_Diag_IOverDch
// #define Test_Diag_RIsoLow
// #define Test_Diag_CommErrBmic
// #define Test_Diag_RelayErrPos
// #define Test_Diag_RelayErrNeg
// #define Test_Diag_RelayErrPre
// #define Test_Diag_DeadFuse
// #define Test_Diag_DeadCell
// #define Test_Diag_OfflineVCell
// #define Test_Diag_OfflineTCell
// #define Test_Diag_OfflineISen
// #define Test_Diag_PackVCellV

// #define Test_CanVcuTx_Task

// #define Test_Ref5v_Calculation





/* -- Class B section -- */
#define CLASSB __attribute__((section(".classb")))
#define CLASSB_CRC __attribute__((section(".classb_crc")))



/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/

typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef uint64_t	uint64;

typedef int8_t		sint8;
typedef int16_t		sint16;
typedef int32_t		sint32;
typedef int64_t		sint64;



typedef enum
{
	get = 0x00,
	set = 0x01
}BUF_INOUT;


typedef enum{
	SYS_INIT = 1,
	SYS_IDLE,
	SYS_WAIT,
	SYS_DRIVE,
	SYS_SHUTDOWN,
	SYS_ERROR,	
	SYS_WARMING,
	SYS_EMERGENCY,
	SYS_CONFIGURATION,
	SYS_CALIBRATION,
	SYS_DETECTING_FCC,
	SYS_SELFTEST,
	SYS_LOGREAD,	
}SYSTEM_STATE;

typedef enum
{
	RS_OPEN 	= 0x00,	
	RS_CHANGING = 0x01,
	RS_CLOSE 	= 0x02,
	RS_FAULT 	= 0x03,
	RS_PRECHG 	= 0x04
}RELAY_STATUS;




/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/



/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/


#endif /* INC_COMMON_DEF_H_ */
