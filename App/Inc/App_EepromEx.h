/*
 * Api_EepromEx.h
 *
 *  Created on: Sep 19, 2025
 *      Author: ETI
 */

#ifndef INC_APP_EEPROMEX_H_
#define INC_APP_EEPROMEX_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define EEPROMEX_WR_TRY_NUM					5u

#define EEPROMEX_CALIBDATA_ADDR				768u

#define EEPROMEX_BLOCK1_DATA_QTY_WORD       15u
#define EEPROMEX_BLOCK2_DATA_QTY_WORD       19u
#define EEPROMEX_BLOCK3_DATA_QTY_WORD       19u
#define EEPROMEX_BLOCK4_DATA_QTY_WORD       20u
#define EEPROMEX_BLOCK5_DATA_QTY_WORD       20u

#define EEPROMEX_BLOCK_ALL_DATA_QTY_WORD	(EEPROMEX_BLOCK1_DATA_QTY_WORD + \
										  	EEPROMEX_BLOCK2_DATA_QTY_WORD + \
										    EEPROMEX_BLOCK3_DATA_QTY_WORD + \
										  	EEPROMEX_BLOCK4_DATA_QTY_WORD + \
										  	EEPROMEX_BLOCK5_DATA_QTY_WORD)

#define EEPROMEX_BLOCK1_DATA_QTY_BYTE       (EEPROMEX_BLOCK1_DATA_QTY_WORD * 2u)
#define EEPROMEX_BLOCK2_DATA_QTY_BYTE       (EEPROMEX_BLOCK2_DATA_QTY_WORD * 2u)
#define EEPROMEX_BLOCK3_DATA_QTY_BYTE       (EEPROMEX_BLOCK3_DATA_QTY_WORD * 2u)
#define EEPROMEX_BLOCK4_DATA_QTY_BYTE       (EEPROMEX_BLOCK4_DATA_QTY_WORD * 2u)
#define EEPROMEX_BLOCK5_DATA_QTY_BYTE       (EEPROMEX_BLOCK5_DATA_QTY_WORD * 2u)

#define EEPROMEX_BLOCK1_ADDR_OFFSET         128u
#define EEPROMEX_BLOCK2_ADDR_OFFSET         256u
#define EEPROMEX_BLOCK3_ADDR_OFFSET         384u
#define EEPROMEX_BLOCK4_ADDR_OFFSET         512u
#define EEPROMEX_BLOCK5_ADDR_OFFSET         640u


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void EepromEx_Control_Task(void);
extern bool EepromEx_SaveEventIndex(void);
extern bool EepromEx_SavePEventIndex(void);
extern bool EepromEx_LoadEventIndex(void);
extern bool EepromEx_LoadPEventIndex(void);
extern void EepromEx_SetEventIndex(uint16 idx);
extern void EepromEx_SetPEventIndex(uint16 idx);
extern uint16  EepromEx_GetEventIndex(void);
extern uint16  EepromEx_GetPEventIndex(void);
extern bool EepromEx_SetSavedFaultHistoryFlag(bool save);
extern bool EepromEx_SetSavedPFaultHistoryFlag(bool save);
extern bool EepromEx_WriteStringData(void);
extern void EepromEx_GetEventLog(uint16 index, uint8* buf);
extern void EepromEx_GetPEventLog(uint16 index, uint8* buf);
extern bool EepromEx_IsSavedParameter(void);
extern bool EepromEx_SetSavedParameterFlag(bool save);
extern bool EepromEx_ReadStringData(void);
extern bool EepromEx_ReadCalibData (void* buf);
extern bool EepromEx_WriteCalibData (void* buf);
extern bool	EepromEx_IsSavedCalibData(void);
extern bool EepromEx_SetSavedCalibDataFlag(bool save);
extern bool EepromEx_IsSavedFaultHistory(void);
extern bool EepromEx_IsSavedPFaultHistory(void);
extern void EepromEx_SetUpdateCapacityData_Flag(bool flag);
extern bool EepromEx_InOutParameters(void* buf, BUF_INOUT SG, uint32 len, uint32 Offset);
// extern bool EepromEx_WriteCfgData(void);
extern void EepromEx_SetErrorFlag(uint8 bit);
extern void EepromEx_ResetErrorFlag(void);
extern uint8   EepromEx_GetErrorFlag(void);
extern void EepromEx_Set_SaveFaultHistoryFlag(BOOL SetV);
extern BOOL EepromEx_Get_SaveFaultHistoryFlag(void);
extern void EepromEx_Set_SavePFaultHistoryFlag(BOOL SetV);
extern BOOL EepromEx_Get_SavePFaultHistoryFlag(void);
extern void EepromEx_Set_SaveEventIdxFlag(BOOL SetV);
extern BOOL EepromEx_Get_SaveEventIdxFlag(void);
extern void EepromEx_Set_SavePEventIdxFlag(BOOL SetV);
extern BOOL EepromEx_Get_SavePEventIdxFlag(void);
extern BOOL EepromEx_Update_CLASSB_PfaultLog(void);




#endif /* INC_APP_EEPROMEX_H_ */
