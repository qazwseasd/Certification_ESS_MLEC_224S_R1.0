/*
 * App_Def.h
 *
 *  Created on: Mar 25, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/

#ifndef APP_INC_APP_DEF_H_
#define APP_INC_APP_DEF_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include <Api_BoardTemp.h>
#include "App_Main.h"
#include "Api_Def.h"
#include "App_Init.h"
#include "App_Task.h"
#include "App_ManaMsm.h"
#include "App_CanUiRx.h"
#include "App_CanVcuTx.h"
#include "App_CellBalance.h"
#include "App_ConfigData.h"
#include "App_EepromEx.h"
#include "App_Soc.h"
#include "App_Soh.h"
#include "App_Soue.h"
#include "App_CurrLimit.h"
#include "App_Ir.h"
#include "App_Relay.h"
#include "App_Warranty.h"
#include "App_Diag.h"
#include "App_Fan.h"
#include "App_Calibdata.h"
#include "App_TRunaway.h"
#include "Lib_CRC.h"
#include "Lib_queue.h"



/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/

/* -- CanVcuTx -- */
#define APP_CanVcuTx_Port				&hcan1

#define APP_CanVcuTx_CmdMsg_Num			1u
#define APP_CanVcuTx_ReqMsg_Num			33u

#define APP_CanVcuTx_MaxBuf_Size		8u

#define APP_CanVcuTx_Id_ReportData1		0x1CFE07ECu

#define APP_CanVcuTx_Id_ClassB_Fail		0x1CFE00ECu

#define APP_CanVcuTx_Id_PackData1		0x1CFF01ECu
#define APP_CanVcuTx_Id_PackData2		0x1CFF02ECu
#define APP_CanVcuTx_Id_PackData3		0x1CFF03ECu
#define APP_CanVcuTx_Id_PackData4		0x1CFF04ECu
#define APP_CanVcuTx_Id_PackData5		0x1CFF05ECu
#define APP_CanVcuTx_Id_PackData6		0x1CFF06ECu
#define APP_CanVcuTx_Id_PackData7		0x1CFF07ECu
#define APP_CanVcuTx_Id_PackData8		0x1CFF08ECu
#define APP_CanVcuTx_Id_PackData9		0x1CFF09ECu
#define APP_CanVcuTx_Id_PackData10		0x1CFF0AECu
#define APP_CanVcuTx_Id_PackData11		0x1CFF0BECu
#define APP_CanVcuTx_Id_PackData12		0x1CFF0CECu
#define APP_CanVcuTx_Id_PackData13		0x1CFF0DECu
#define APP_CanVcuTx_Id_PackData14		0x1CFF0EECu
#define APP_CanVcuTx_Id_PackData15		0x1CFF0FECu

#define APP_CanVcuTx_Id_ModeChange		0x1CFE00ECu
#define APP_CanVcuTx_Id_RelayData		0x1CFE0AECu

#define APP_CanVcuTx_Id_ModuleData1		0x1CFA00ECu
#define APP_CanVcuTx_Id_ModuleData2		0x1CFB00ECu
#define APP_CanVcuTx_Id_ModuleData3		0x1CFC00ECu

#define APP_CanVcuTx_Id_FaultRelase		0x1CFE0BECu

#define APP_CanVcuTx_Id_Configdata		0x1CFE01ECu
#define APP_CanVcuTx_Id_Calibdata		0x1CFE02ECu
#define APP_CanVcuTx_Id_Eventdata		0x1CFE03ECu
#define APP_CanVcuTx_Id_DetectFCCdata	0x1CFE0CECu




/* -- CanVcuRx -- */
/* Modif By SKAHN _250825_SAT */
#define APP_CanVcuRx_Id_Cmd1			0x18AAAAF3u
/* Modif By SKAHN _250825_END */

/* -- can2 -- */
#define APP_CanPackTx_Port				&hcan2
#define APP_CanPackTx_MaxBuf_Size		8u

#define	APP_CanPackTx_Id_Cmd1			0x18FF01CAu

/* MeasISens	*/
#define APP_MeasISens_Min_Current		10	/* [0.1A]	*/


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/


#endif /* APP_INC_APP_DEF_H_ */
