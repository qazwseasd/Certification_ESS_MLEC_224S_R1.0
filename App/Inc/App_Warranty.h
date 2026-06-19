/*
 * App_Warranty.h
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */

#ifndef INC_APP_WARRANTY_H_
#define INC_APP_WARRANTY_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define WARRANTY_EEP_ADDR       2560u


/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void Warranty_Init(void);
extern void Warranty_Task(void);
extern void WarrantyData_Load(void);
extern void WarrantyData_Save(void);
extern char* Get_WarrantyAddress(void);
extern char* Get_WarrantyTempAddress(void);
extern BOOL IsWarrantyDataChanged(void);
extern BOOL Get_IsSaveWarrantyData(void);
extern void Set_SaveWarrantyData(BOOL SetV);


#endif /* INC_APP_WARRANTY_H_ */
