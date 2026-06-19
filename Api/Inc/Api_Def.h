/*
 * Api_Def.h
 *
 *  Created on: Mar 25, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  Define to prevent recursive inclusion                                    */
/*****************************************************************************/

#ifndef INC_API_DEF_H_
#define INC_API_DEF_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Def.h"
#include "Api_Init.h"
#include "Api_Task.h"
#include "Api_CtrlLed.h"
#include "Api_CtrlHv.h"
#include "Api_Adcin.h"
#include "Api_Adbms.h"
#include "Api_AuxVolt.h"
#include "Api_BoardTemp.h"
#include "Api_Curr.h"
#include "Api_HvilEpo.h"
#include "Api_IsoR.h"
#include "Api_PackVolt.h"
#include "Api_Ref5v.h"
#include "Api_Calibdata.h"
#include "Api_ManaMsm.h"



/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/

/* HV Control	*/
#define Api_CtrlHv_Vbus_TimeOut				500u
#define Api_CtrlHv_On_TimeOut				500u

/* SD Card	*/
#define API_CtrlSd_Data_Path				("/ETI_Log")
#define API_CtrlSd_FW_Path					("/ETI_FW")

/* LED	*/
#define API_CtrlLed_Peri_Task		 		100u	/* [ms] */

#define API_CtrlLed_Peri_Continuous			(uint8)(   0u						   )
#define API_CtrlLed_Peri_Init				(uint8)( 100u	/ API_CtrlLed_Peri_Task)	/*	[ms]	*/
#define API_CtrlLed_Peri_Normal				(uint8)( 200u	/ API_CtrlLed_Peri_Task)	/*	[ms]	*/
#define API_CtrlLed_Peri_Config				(uint8)( 500u	/ API_CtrlLed_Peri_Task)	/*	[ms]	*/
#define API_CtrlLed_Peri_Calib				(uint8)(1000u	/ API_CtrlLed_Peri_Task)	/*	[ms]	*/
#define API_CtrlLed_Peri_DetectFcc			(uint8)(1500u	/ API_CtrlLed_Peri_Task)	/*	[ms]	*/
#define API_CtrlLed_Peri_ReadLog			(uint8)(2000u	/ API_CtrlLed_Peri_Task)	/*	[ms]	*/

/* TCP Client	*/
#define API_TcpClient_ServerIP0				(uint8)  192u
#define API_TcpClient_ServerIP1				(uint8)  168u
#define API_TcpClient_ServerIP2				(uint8)  137u
#define API_TcpClient_ServerIP3				(uint8)  1u

#define API_TcpClient_ServerPort			(uint16) 813u

/* TCP Server	*/
#define API_TcpServer_ServerPort			(uint16) 502u


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/


#endif /* INC_API_DEF_H_ */
