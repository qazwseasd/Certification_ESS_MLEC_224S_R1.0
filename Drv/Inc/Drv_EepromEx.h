/*
 * Drv_EepromEx.h
 *
 *  Created on: Sep 15, 2025
 *      Author: ETI
 */

#ifndef INC_DRV_EEPROMEX_H_
#define INC_DRV_EEPROMEX_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define EEPROMEX_MAXSIZE 	0x3FFFFu
#define EEPROMEX_PAGESIZE	256u

#define EEPROMEX_CMD_WRSR	0x01u
#define EEPROMEX_CMD_WRITE	0x02u
#define EEPROMEX_CMD_READ	0x03u
#define EEPROMEX_CMD_WRDI	0x04u
#define EEPROMEX_CMD_RDSR	0x05u
#define EEPROMEX_CMD_WREN	0x06u
#define EEPROMEX_CMD_PE		0x42u
#define EEPROMEX_CMD_RDID	0xabu
#define EEPROMEX_CMD_DPD	0xb9u
#define EEPROMEX_CMD_CE		0xc7u
#define EEPROMEX_CMD_SE		0xd8u


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern uint8 EepromEx_Init(void);
extern bool EepromEx_ReadData(uint32 u16ByteIdx, uint32 u16Len, uint8* eData);
extern bool EepromEx_WriteData(uint32 u16ByteIdx, uint32 u16Len, uint8* eData);



#endif /* INC_DRV_EEPROMEX_H_ */
