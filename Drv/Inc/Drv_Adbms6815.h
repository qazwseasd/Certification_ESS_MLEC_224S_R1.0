/*
 * Drv_Adbms6815.h
 *
 *  Created on: Sep 9, 2025
 *      Author: ETI
 */

#ifndef INC_DRV_ADBMS6815_H_
#define INC_DRV_ADBMS6815_H_

/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Def.h"


/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
/*    
    Configuration Register Group A
    Register  Read/Write    Bit 7       Bit 6       Bit 5       Bit 4       Bit 3       Bit 2       Bit 1       Bit 0       Default
    CFGAR0    Read/write    REFON       ADCOPT      PS[1]       PS[0]       CVMIN[1]    CVMIN[0]    MCAL        COMM_BK     0x00
    CFGAR1    Read/write    FLAG_D[7]   FLAG_D[6]   FLAG_D[5]   FLAG_D[4]   FLAG_D[3]   FLAG_D[2]   FLAG_D[1]   FLAG_D[0]   0x00
    CFGAR2    Read/write    SOAKON      OWRNG       OWA[2]      OWA[1]      OWA[0]      OWC[2]      OWC[1]      OWC[0]      0x00
    CFGAR3    Read/write    RSVD        GPO[7]      GPO[6]      GPO[5]      GPO[4]      GPO[3]      GPO[2]      GPO[1]      0xFF
    CFGAR4    Read          RSVD        GPI[7]      GPI[6]      GPI[5]      GPI[4]      GPI[3]      GPI[2]      GPI[1]      0b0XXXXXXX
    CFGAR5    Read          REV[3]      REV[2]      REV[1]      REV[0]      DTYPE[3]    DTYPE[2]    DTYPE[1]    DTYPE[0]    0bXXXX0000
*/

/*
    Table 69. Configuration Register Group B
    Register  Read/Write    Bit 7   Bit 6    Bit 5    Bit 4    Bit 3    Bit 2    Bit 1    Bit 0    Default
    CFGBR0    Read/write    VUV[7]  VUV[6]   VUV[5]   VUV[4]   VUV[3]   VUV[2]   VUV[1]   VUV[0]    0x00
    CFGBR1    Read/write    VOV[3]  VOV[2]   VOV[1]   VOV[0]   VUV[11]  VUV[10]  VUV[9]   VUV[8]    0xF0
    CFGBR2    Read/write    VOV[11] VOV[10]  VOV[9]   VOV[8]   VOV[7]   VOV[6]   VOV[5]   VOV[4]    0xFF
    CFGBR3    Read/write    DTMEN   DTRNG    DCTO[5]  DCTO[4]  DCTO[3]  DCTO[2]  DCTO[1]  DCTO[0]   0x00
    CFGBR4    Read/write    DCC[8]  DCC[7]   DCC[6]   DCC[5]   DCC[4]   DCC[3]   DCC[2]   DCC[1]    0x00
    CFGBR5    Read/write    MUTE_ST RSVD     RSVD     RSVD     DCC[12]  DCC[11]  DCC[10]  DCC[9]    0x00    
*/


#define OWD_NO_ERROR	0u
#define OWD_DRF	 		1u
#define OWD_CVMIN 		2u
#define OWD_OPEN 		3u
#define OWD_LATENT 		4u

/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/
/*
typedef struct
{
    uint8    Reg0;
    uint8    Reg1;
    uint8    Reg2;
    uint8    Reg3;
    uint8    Reg4;
    uint8    Reg5;
	uint8    dummy1;
    uint8    dummy2;
}CFGAR;

typedef struct
{
    uint8    Reg0;
    uint8    Reg1;
    uint8    Reg2;
    uint8    Reg3;
    uint8    Reg4;
    uint8    Reg5;
	uint8    dummy1;
    uint8    dummy2;
}CFGBR;

typedef struct
{
    CFGAR   CfgA;
    CFGBR   CfgB;
}ConfigReg;


typedef enum
{
    OWD_NO_ERROR,
	OWD_DRF,
	OWD_CVMIN,
	OWD_OPEN,
	OWD_LATENT
}OWD_Status;
*/

typedef struct
{
//	ConfigReg WriteConfig;
//  ConfigReg ReadConfig;
	
	uint8	WriteConfig_CfgA_Reg0;
	uint8	WriteConfig_CfgA_Reg1;
	uint8	WriteConfig_CfgA_Reg2;
	uint8	WriteConfig_CfgA_Reg3;
	uint8	WriteConfig_CfgA_Reg4;
	uint8	WriteConfig_CfgA_Reg5;
	
	uint8	WriteConfig_CfgB_Reg0;
	uint8	WriteConfig_CfgB_Reg1;
	uint8	WriteConfig_CfgB_Reg2;
	uint8	WriteConfig_CfgB_Reg3;
	uint8	WriteConfig_CfgB_Reg4;
	uint8	WriteConfig_CfgB_Reg5;
	
	uint8	ReadConfig_CfgA_Reg0;
	uint8	ReadConfig_CfgA_Reg1;
	uint8	ReadConfig_CfgA_Reg2;
	uint8	ReadConfig_CfgA_Reg3;
	uint8	ReadConfig_CfgA_Reg4;
	uint8	ReadConfig_CfgA_Reg5;
	
	uint8	ReadConfig_CfgB_Reg0;
	uint8	ReadConfig_CfgB_Reg1;
	uint8	ReadConfig_CfgB_Reg2;
	uint8	ReadConfig_CfgB_Reg3;
	uint8	ReadConfig_CfgB_Reg4;
	uint8	ReadConfig_CfgB_Reg5;

	uint16   CVR[DEF_MAX_NUM_OF_CELL_PER_BMIC];        		// Cell Voltage Register
    uint16   REF2;
    uint16   GV[DEF_MAX_NUM_OF_TEMP_PER_BMIC];
    uint16   REF3;
    uint16   SC;
	
    sint16   ITMP;
    uint16   VA;
    uint16   VD;
    uint16   COV_State;
	
    uint16   CUV_State;
	uint16   ADOL[2];
	
    uint8    OC_CNTR;
    uint8 	  Stcr0;
    uint8 	  Stcr1;
	uint8	 OWD[DEF_MAX_NUM_OF_CELL_PER_BMIC]; // Cell Open Wire Status    
    uint8    OAXD[DEF_MAX_NUM_OF_TEMP_PER_BMIC];				// Aux. Open Wire Sataus
    uint8    DischargeR_Diag[DEF_MAX_NUM_OF_CELL_PER_BMIC];

	//    OWD_Status OWD[DEF_MAX_NUM_OF_CELL_PER_BMIC];   
}Adbms6815_Data;

typedef enum
{
    ALL_OFF,
    ODD,
    EVEN
}Adbms6815_FETCtrl;


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern void Adbms6815_Init_Drv(void);
extern void Adbms6815_SetDefaultConfig(Adbms6815_Data* adbms_data);
extern void Adbms6815_WriteConfig_A(Adbms6815_Data* adbms_data);
extern void Adbms6815_WriteConfig_B(Adbms6815_Data* adbms_data);
extern void Adbms6815_Sub_WriteConfig(Adbms6815_Data* adbms_data);
extern void Adbms6815_Sub_WriteConfig_A(Adbms6815_Data* adbms_data);
extern void Adbms6815_Sub_WriteConfig_B(Adbms6815_Data* adbms_data);
extern void Adbms6815_ReadConfig(Adbms6815_Data* adbms_data);
extern void Adbms6815_ReadConfig_A(Adbms6815_Data* adbms_data);
extern void Adbms6815_ReadConfig_B(Adbms6815_Data* adbms_data);
extern void Adbms6815_Sub_ReadConfig(Adbms6815_Data* adbms_data);
extern void Adbms6815_Sub_ReadConfig_A(Adbms6815_Data* adbms_data);
extern void Adbms6815_Sub_ReadConfig_B(Adbms6815_Data* adbms_data);
extern void Adbms6815_CellConvert(void);
extern void Adbms6815_Sub_CellConvert(void);
extern void Adbms6815_ReadCellVolt(Adbms6815_Data* adbms_data, uint8 DataNo);
extern void Adbms6815_Sub_ReadCellVolt(Adbms6815_Data* adbms_data, uint8 DataNo);
extern void Adbms6815_AuxConvert(void);
extern void Adbms6815_Sub_AuxConvert(void);
extern void Adbms6815_ReadAuxVolt(Adbms6815_Data* adbms_data, uint8 DataNo);
extern void Adbms6815_Sub_ReadAuxVolt(Adbms6815_Data* adbms_data, uint8 DataNo);
extern void Adbms6815_StatusConvert(void);
extern void Adbms6815_Sub_StatusConvert(void);
extern void Adbms6815_ReadStatus(Adbms6815_Data* adbms_data, uint8 DataNo);
extern void Adbms6815_Sub_ReadStatus(Adbms6815_Data* adbms_data, uint8 DataNo);
extern bool   Adbms6815_CheckCellOpenWire(Adbms6815_Data* adbms_data);
extern bool   Adbms6815_CheckAuxOpenWire(Adbms6815_Data* adbms_data);
extern bool   Adbms6815_CheckDischargeResistor(Adbms6815_Data* adbms_data);
extern uint8 Adbms6815_GetCellBalanceStatus(uint16* GetV, Adbms6815_Data* adbms_data);
extern void Adbms6815_SetCellBalanceStatus(const uint16* SetV, Adbms6815_Data* adbms_data, Adbms6815_FETCtrl Fet_Ctrl);
extern uint16  Adbms6815_F_Get6815PEC_ErrorCount(void);
extern uint16  Adbms6815_R_Get6815PEC_ErrorCount(void);
extern void Adbms6815_F_Set6815PEC_ErrorCount(uint16 error_cnt);
extern void Adbms6815_R_Set6815PEC_ErrorCount(uint16 error_cnt);
extern uint32 Adbms6815_FrontSPI_Error_Check(void);
extern uint32 Adbms6815_RearSPI_Error_Check(void);
extern bool Adbms_CheckCellBalanceR(Adbms6815_Data* adbms_data);
extern const uint16 Adbms6815_Crc10Table[256];
extern const uint16 Adbms6815_Crc15Table[256];






#endif /* INC_DRV_ADBMS6815_H_ */
