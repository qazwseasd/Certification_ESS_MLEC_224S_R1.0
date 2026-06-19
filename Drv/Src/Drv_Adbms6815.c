/*
 * Drv_Adbms6815.c
 *
 *  Created on: Sep 9, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Drv_Adbms6815.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
/* ADBMS6815 Command definition */
#define ADBMS6815_CMD_WRCFGA		(uint16)(0x0001)	/* Write configuration register group A */
#define ADBMS6815_CMD_RDCFGA		(uint16)(0x0002)	/* Read configuration register group A */
#define ADBMS6815_CMD_WRCFGB		(uint16)(0x0024)	/* Write configuration register group B */
#define ADBMS6815_CMD_RDCFGB		(uint16)(0x0026)	/* Read configuration register group B */

#define ADBMS6815_CMD_RDCVA		    (uint16)(0x0004)	/* Read cell voltage register group A */
#define ADBMS6815_CMD_RDCVB		    (uint16)(0x0006)	/* Read cell voltage register group B */
#define ADBMS6815_CMD_RDCVC		    (uint16)(0x0008)	/* Read cell voltage register group C */
#define ADBMS6815_CMD_RDCVD		    (uint16)(0x000A)	/* Read cell voltage register group D */
#define ADBMS6815_CMD_RDCVALL       (uint16)(0x0038)	/* Read All Cell Results */

#define ADBMS6815_CMD_RDCDA         (uint16)(0x0030)    /* Read Cell Diagnostic Register Group A */
#define ADBMS6815_CMD_RDCDB         (uint16)(0x0031)    /* Read Cell Diagnostic Register Group B */
#define ADBMS6815_CMD_RDCDC         (uint16)(0x0032)    /* Read Cell Diagnostic Register Group C */
#define ADBMS6815_CMD_RDCDD         (uint16)(0x0033)    /* Read Cell Diagnostic Register Group D */
#define ADBMS6815_CMD_RDCDALL       (uint16)(0x003A)    /* Read All Cell Diagnostic Results */

#define ADBMS6815_CMD_RDAUXA        (uint16)(0x000C)    /* Read Auxiliary Register Group A */
#define ADBMS6815_CMD_RDAUXB        (uint16)(0x000E)    /* Read Auxiliary Register Group B */
#define ADBMS6815_CMD_RDAUXC        (uint16)(0x000D)    /* Read Auxiliary Register Group C */

#define ADBMS6815_CMD_RDSTATA       (uint16)(0x0010)    /* Read Status Register Group A */
#define ADBMS6815_CMD_RDSTATB       (uint16)(0x0012)    /* Read Status Register Group B */
#define ADBMS6815_CMD_RDSTATC       (uint16)(0x0013)    /* Read Status Register Group C */

#define ADBMS6815_CMD_RDASALL       (uint16)(0x003A)    /* Read All Aux/Status Registers */

#define ADBMS6815_CMD_ADCV		    (uint16)(0x0260)	/* Start cell voltage ADC conversion and poll status */
#define ADBMS6815_CMD_CVOW		    (uint16)(0x0208)	/* Start Cell Voltage Open Wire ADC Conversion and Poll Status */

#define ADBMS6815_CMD_ADOW		    (uint16)(0x0228)	/* Start open wire ADC conversion and poll status */

#define ADBMS6815_CMD_CVPG		    (uint16)(0x0207)	/* Start Cell Voltage Pattern Generation and Poll Status */

#define ADBMS6815_CMD_CDPG		    (uint16)(0x021F)	/* Start Cell Diagnostic Pattern Generation and Poll Status */
#define ADBMS6815_CMD_ADOL		    (uint16)(0x0201)	/* Start Overlap Measurements of Cell 7 Voltage */
#define ADBMS6815_CMD_ADLEAK		(uint16)(0x0060)	/* Start Pin Leakage Measurement (Cx vs. Sx) */
#define ADBMS6815_CMD_ADSC		    (uint16)(0x0068)	/* Start Sx vs. C(x − 1) Measurement */

#define ADBMS6815_CMD_ADAX		    (uint16)(0x0400)	/* Start GPIOs ADC Conversion and Poll Status */
#define ADBMS6815_CMD_AXOW		    (uint16)(0x0410)	/* Start GPIOs Open Wire ADC Conversion and Poll Status */
#define ADBMS6815_CMD_AXPG		    (uint16)(0x0407)	/* Start Auxiliary Pattern Generation and Poll Status */

#define ADBMS6815_CMD_ADSTAT		(uint16)(0x0468)	/* Start Status Group ADC Conversion and Poll Status */
#define ADBMS6815_CMD_STATPG		(uint16)(0x040F)	/* Start Status Pattern Generation and Poll Status */

#define ADBMS6815_CMD_ADCVAX		(uint16)(0x046F)	/* Start Combined Cell Voltage and GPIO1, GPIO2 Conversion and Poll Status */
#define ADBMS6815_CMD_ADCVSC		(uint16)(0x0467)	/* Start Combined Cell Voltage and SC Conversion and Poll Status */

#define ADBMS6815_CMD_CLRCELL		(uint16)(0x0711)	/* Clear Cell Voltage Register Groups */
#define ADBMS6815_CMD_CLRAUX		(uint16)(0x0712)	/* Clear Auxiliary Register Groups */
#define ADBMS6815_CMD_CLRSTAT		(uint16)(0x0713)	/* Clear Status Register Groups */
#define ADBMS6815_CMD_CLRCD		    (uint16)(0x0716)	/* Clear Cell Diagnostic Register Groups */
#define ADBMS6815_CMD_CLRFLAG	    (uint16)(0x0717)	/* Clear Flags */

#define ADBMS6815_CMD_PLADC	        (uint16)(0x0714)	/* Poll ADC Conversion Status */
#define ADBMS6815_CMD_DIAGN	        (uint16)(0x0715)	/* Diagnose Mux and Poll Status */

#define ADBMS6815_CMD_WRCOMM	    (uint16)(0x0721)	/* Write COMM Register Group */
#define ADBMS6815_CMD_RDCOMM	    (uint16)(0x0722)	/* Read COMM Register Group */
#define ADBMS6815_CMD_STCOMM	    (uint16)(0x0723)	/* Start I2C/SPI Communication */

#define ADBMS6815_CMD_MUTE	        (uint16)(0x0028)	/* Mute Discharge */
#define ADBMS6815_CMD_UNMUTE        (uint16)(0x0029)	/* Unmute Discharge */

#define ADBMS6815_CMD_RDSID         (uint16)(0x002C)	/* Read Serial ID Register Group */
#define ADBMS6815_CMD_SRST          (uint16)(0x001F)	/* Soft Reset */
#define ADBMS6815_CMD_RSTCC         (uint16)(0x002E)	/* Reset Command Counter */

#define ADBMS6815_CBD_PUP_UP		(uint16)(0x0040)	/* PUP Pullup */

#define ADBMS6815_CBD_CH_ALL		(uint16)(0x0000)	/* ALL Cells */
#define ADBMS6815_CBD_MD_27K	    (uint16)((uint16)(0x0001) << 7)		/* ADCOPT = 0, FAST      */
#define ADBMS6815_CBD_MD_7K		    (uint16)((uint16)(0x0002) << 7)		/* ADCOPT = 0, NORMAL    */
#define ADBMS6815_CBD_MD_26		    (uint16)((uint16)(0x0003) << 7)		/* ADCOPT = 0, FILTERED  */
#define ADBMS6815_CBD_MD_14K	    (uint16)((uint16)(0x0001) << 7)		/* ADCOPT = 1, FAST      */
#define ADBMS6815_CBD_MD_3K		    (uint16)((uint16)(0x0002) << 7)		/* ADCOPT = 1, NORMAL    */
#define ADBMS6815_CBD_MD_2K		    (uint16)((uint16)(0x0003) << 7)		/* ADCOPT = 1, FILTERED  */

#define ADBMS6815_CBD_CHG_1_7       (uint16)(0x0001)	/* Cell 1, 7  */
#define ADBMS6815_CBD_CHG_2_8       (uint16)(0x0002)	/* Cell 2, 8  */
#define ADBMS6815_CBD_CHG_3_9       (uint16)(0x0003)	/* Cell 3, 9  */
#define ADBMS6815_CBD_CHG_4_10      (uint16)(0x0004)	/* Cell 4, 10  */
#define ADBMS6815_CBD_CHG_5_11      (uint16)(0x0005)	/* Cell 5, 11  */
#define ADBMS6815_CBD_CHG_6_12      (uint16)(0x0006)	/* Cell 6, 12  */

#define ADBMS6815_CBD_CHG_ALL	    (uint16)(0x0000)	/* GPIO 1~7, REF2, REF3  */
#define ADBMS6815_CBD_CHG_GPIO1	    (uint16)(0x0001)	/* GPIO 1  */
#define ADBMS6815_CBD_CHG_GPIO2	    (uint16)(0x0002)	/* GPIO 2  */
#define ADBMS6815_CBD_CHG_GPIO3	    (uint16)(0x0003)	/* GPIO 3  */
#define ADBMS6815_CBD_CHG_GPIO4	    (uint16)(0x0004)	/* GPIO 4  */
#define ADBMS6815_CBD_CHG_GPIO5	    (uint16)(0x0005)	/* GPIO 5  */
#define ADBMS6815_CBD_CHG_GPIO6	    (uint16)(0x0006)	/* GPIO 6  */
#define ADBMS6815_CBD_CHG_GPIO7	    (uint16)(0x0007)	/* GPIO 7  */
#define ADBMS6815_CBD_CHG_REF	    (uint16)(0x0008)	/* REF2, REF3*/

#define ADBMS6815_CBD_CHST_ALL	    (uint16)(0x0000)	/* Status Group Selection : SC, ITMP, VA, VD  */
#define ADBMS6815_CBD_CHST_SC	    (uint16)(0x0001)	/* SC   */
#define ADBMS6815_CBD_CHST_ITMP	    (uint16)(0x0002)	/* ITMP */
#define ADBMS6815_CBD_CHST_VA	    (uint16)(0x0003)	/* VA   */
#define ADBMS6815_CBD_CHST_VD	    (uint16)(0x0003)	/* VD   */

#define ADBMS6815_CFG_GPIO5		    ((uint8)(0x01) << 7)
#define ADBMS6815_CFG_GPIO4		    ((uint8)(0x01) << 6)
#define ADBMS6815_CFG_GPIO3		    ((uint8)(0x01) << 5)
#define ADBMS6815_CFG_GPIO2		    ((uint8)(0x01) << 4)
#define ADBMS6815_CFG_GPIO1		    ((uint8)(0x01) << 3)
#define ADBMS6815_CFG_REFON		    ((uint8)(0x01) << 2)
#define ADBMS6815_CFG_ADCOPT	    ((uint8)(0x01) << 0)

#define ADBMS6815_CRC_PASS			(uint8)0x00
#define ADBMS6815_CRC_FAIL	    	(uint8)0xFF

#define ADBMS6815_READ_DATA_SIZE    8u

#define ADBMS6815_CRC10_POLY		0x8F
#define ADBMS6815_CRC15_POLY		0x4599

#define ADBMS6815_VOV		    	4500.f	// 4.5V, Maximum Cell voltage
#define ADBMS6815_VUV	    		2500.f	// 2.5V, Minimum cell voltage

/*VTH_SPF and VTH_LF are fault detection thresholds that are selected based on the external circuit values in the system. 
 Contact the factory for guidance selecting VTH_SPF and VTH_LF.*/ 
#define ADBMS6815_VTH_SPF     		10000u
#define ADBMS6815_VTH_LF      		1000u

#define ADBMS6815_MIN_RATIO_V     	(float)0.03
#define ADBMS6815_MAX_RATIO_V     	(float)0.07



/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
/**
* @brief	PEC10 Table
*/
const uint16 Adbms6815_Crc10Table[256] = {
	0,	 143, 286, 401, 572, 691, 802, 941, 247, 120, 489, 358, 715, 580, 981, 858, 494, 353, 240, 127,
	978, 861, 716, 579, 281, 406,   7, 136, 805, 938, 571, 692, 988, 851, 706, 589, 480, 367, 254, 113,
	811, 932, 565, 698, 279, 408,   9, 134, 562, 701, 812, 931,  14, 129, 272, 415, 709, 586, 987, 852,
	249, 118, 487, 360, 823, 952, 553, 678, 267, 388,  21, 154, 960, 847, 734, 593, 508, 371, 226, 109,
	729, 598, 967, 840, 229, 106, 507, 372, 558, 673, 816, 959,  18, 157, 268, 387, 235, 100, 501, 378,
	727, 600, 969, 838,  28, 147, 258, 397, 544, 687, 830, 945, 261, 394,  27, 148, 825, 950, 551, 680,
	498, 381, 236,  99, 974, 833, 720, 607, 737, 622,1023, 880, 221,  82, 451, 332, 534, 665, 776, 903,
	42,  165, 308, 443, 783, 896, 529, 670, 307, 444,  45, 162,1016, 887, 742, 617, 452, 331, 218,  85,
	317, 434,  35, 172, 769, 910, 543, 656, 458, 325, 212,  91,1014, 889, 744, 615, 211,  92, 461, 322,
	751, 608,1009, 894,  36, 171, 314, 437, 536, 663, 774, 905, 470, 345, 200,  71,1002, 869, 756, 635,
	289, 430,  63, 176, 797, 914, 515, 652,  56, 183, 294, 425, 516, 651, 794, 917, 207,  64, 465, 350,
	755, 636,1005, 866, 522, 645, 788, 923,  54, 185, 296, 423, 765, 626, 995, 876, 193,  78, 479, 336,
	996, 875, 762, 629, 472, 343, 198,  73, 787, 924, 525, 642, 303, 416,  49, 190
};


/**
* @brief	PEC15 Table
*/
const uint16 Adbms6815_Crc15Table[256] = {
	0,      50585,  52907,  2866,   55503,  7510,   5732,   54269,  62471,  12702,  15020,  65333,  11464,  59729,  57955,  10234,  
	44439,  26638,  25404,  42661,	30040,  45249,  48115,  32362,  22928,  39945,  38715,  21154,  33119,  17606,  20468,  35437,
	23342,  40631,  38277,  20508,  33761,  18040,  19786,  35027,  44841,  27312,  24962,  42011,  30694,  45695,  47437,  31956,
	63161,  13088,  14354,  64907,  11894,  60399,  57565,  9540,   702,    50983,  52245,  2444,	55921,  8168,   5338,   53571,
	62405,  13916,  15726,  63735,  11018,  61075,  58785,  8248,   1986,   49755,  51561,  3312,   57101,  6804,   4518,   54335,
	24146,  39883,  37113,  21856,  34461,  17156,  18486,  36271,  43605,  28620,  25854,  41319,  29338,  46851,  48177,  31144,
	43243,  28018,  26176,  41945,	28708,  46525,  48783,  31510,  23788,  39285,  37447,  22494,  33827,  16826,  19080,  36625,
	1404,   49381,  52183,  3662,   56755,  6186,   4888,   54913,	61819,  13538,  16336,  64073,  10676,  60461,  59167,  8838,
	41491,  26506,  27832,  43297,  31452,  48965,  46199,  29166,  22036,  37773,  39103,  23846,	36571,  19266,  16496,  34281,
	3972,   51741,  49455,  1206,   55115,  4818,   6624,   56441,  64387,  15898,  13608,  61617,  9036,   59093,  60903,  10366,
	63805,  15524,  14230,  61967,  8690,   58475,  61273,  10944,  3386,   51363,  50065,  1544,   54773,  4204,   7006,   57031,
	21674,  37171,  39425,  24472,	35941,  18940,  17102,  34647,  41133,  25908,  28166,  43935,  30818,  48635,  46793,  29520,
	20950,  37967,  40829,  23268,  35097,  19584,  18354,  33323,	42449,  24648,  27514,  44771,  32030,  47239,  46005,  30252,
	64577,  14808,  13034,  63347,  9358,   57623,  59941,  12220,  2118,   52703,  50925,  884,	53385,  5392,   7714,   56251,
	2808,   53089,  50259,  458,    53815,  6062,   7324,   55557,  65279,  15206,  12372,  62925,  9776,   58281,  59547,  11522,
	42863,  25334,  27076,  44125,  32672,  47673,  45323,  29842,  21352,  38641,  40387,  22618,  35751,  20030,  17676,  32917
};


CLASSB static uint16 Adbms_PEC_error_count_F;
CLASSB static uint16 Adbms_PEC_error_count_R;

CLASSB static uint8 RLEC_FrontSPI_Error[(DEF_MAX_NUM_OF_BMIC / 2u)];
CLASSB static uint8 RLEC_RearSPI_Error[(DEF_MAX_NUM_OF_BMIC / 2u)];


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/
void Adbms6815_CreatePEC10Table(void);
void Adbms6815_CreatePEC15Table(void);
void Adbms6815_WriteDataGroup(uint16 Command, uint8 Buff[][ADBMS6815_READ_DATA_SIZE]);
void Adbms6815_Sub_WriteDataGroup(uint16 Command, uint8 Buff[][ADBMS6815_READ_DATA_SIZE]);
uint16  Adbms6815_CalculateCRC15(uint8 len, const uint8* data);
uint16  Adbms6815_CalculateCRC10(uint8* pDataBuf , bool bIsRxCmd, uint8 nLength);
void Adbms6815_Wakeup(void);
void Adbms6815_Sub_Wakeup(void);
void Adbms6815_ReadDataGroup(uint16 Command, uint8 Buff[][ADBMS6815_READ_DATA_SIZE], bool* CrcPassFlag);
void Adbms6815_Sub_ReadDataGroup(uint16 Command, uint8 Buff[][ADBMS6815_READ_DATA_SIZE], bool* CrcPassFlag);
void Adbms6815_WriteSingleCmd(uint16 RequestCMD);
void Adbms6815_Sub_WriteSingleCmd(uint16 RequestCMD);



/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
* @brief	Initialize ADBMS6815
* @param	void
* @return	void
*/
void Adbms6815_Init_Drv(void)
{
	HAL_GPIO_WritePin(Main_6820_RET_GPIO_Port, Main_6820_RET_Pin, GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(Sub_6820_RET_GPIO_Port, Sub_6820_RET_Pin, GPIO_PIN_SET);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Default Configuration Set
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_SetDefaultConfig(Adbms6815_Data* adbms_data)
{
    uint8  i;
    uint16 u16_temp;    
	float	fdata;

    /* default value Setting */
	for(i = 0u; i < DEF_MAX_NUM_OF_BMIC; i++)
    {
#ifdef NDEVICE      
		(&adbms_data[i])->WriteConfig_CfgA_Reg0 = 0x84;
    (&adbms_data[i])->WriteConfig_CfgA_Reg1	= 0;
    (&adbms_data[i])->WriteConfig_CfgA_Reg2	= 0x51;
    (&adbms_data[i])->WriteConfig_CfgA_Reg3	= 0xFF;
#else
    (&adbms_data[i])->WriteConfig_CfgA_Reg0 = 0b10000100;
    (&adbms_data[i])->WriteConfig_CfgA_Reg1	= 0;
    (&adbms_data[i])->WriteConfig_CfgA_Reg2	= 0b01010001;
    (&adbms_data[i])->WriteConfig_CfgA_Reg3	= 0b11111111;
#endif      
	(&adbms_data[i])->WriteConfig_CfgA_Reg4	= 0;
	(&adbms_data[i])->WriteConfig_CfgA_Reg5	= 0;

    
	fdata =	(ADBMS6815_VUV / (float)0.1 / (float)16);
	u16_temp = (uint16)fdata;
    (&adbms_data[i])->WriteConfig_CfgB_Reg0 = (uint8)(u16_temp & 0xFFu);
    (&adbms_data[i])->WriteConfig_CfgB_Reg1 	= (uint8)(u16_temp >> 8);
	
    fdata = (ADBMS6815_VOV / (float)0.1 / (float)16);
	u16_temp = (uint16)fdata;
    (&adbms_data[i])->WriteConfig_CfgB_Reg1 	|= (uint8)((u16_temp & 0xFu) << 4);
    (&adbms_data[i])->WriteConfig_CfgB_Reg2 	= (uint8)(u16_temp >> 4);

    (&adbms_data[i])->WriteConfig_CfgB_Reg3	= 0;
    (&adbms_data[i])->WriteConfig_CfgB_Reg4 = 0;
    (&adbms_data[i])->WriteConfig_CfgB_Reg5	= 0;
            
    }
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Configuration register A setting function
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_WriteConfig_A(Adbms6815_Data* adbms_data)
{
    uint8 ConfigData[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
    uint8 i;
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
    
    for(i = 0; i < NumOf_Bmic; i++)
    {
        ConfigData[i][0] = (&adbms_data[i])->WriteConfig_CfgA_Reg0;
        ConfigData[i][1] = (&adbms_data[i])->WriteConfig_CfgA_Reg1;
        ConfigData[i][2] = (&adbms_data[i])->WriteConfig_CfgA_Reg2;
        ConfigData[i][3] = (&adbms_data[i])->WriteConfig_CfgA_Reg3;
        ConfigData[i][4] = (&adbms_data[i])->WriteConfig_CfgA_Reg4;
        ConfigData[i][5] = (&adbms_data[i])->WriteConfig_CfgA_Reg5;
    }

	Adbms6815_WriteDataGroup(ADBMS6815_CMD_WRCFGA, ConfigData);    
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Configuration register A setting Sub function
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_Sub_WriteConfig_A(Adbms6815_Data* adbms_data)
{
    uint8 ConfigData[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
    uint8 i;
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
    
    for(i = 0; i < NumOf_Bmic; i++)
    {
        ConfigData[i][0] = (&adbms_data[i])->WriteConfig_CfgA_Reg0;
        ConfigData[i][1] = (&adbms_data[i])->WriteConfig_CfgA_Reg1;
        ConfigData[i][2] = (&adbms_data[i])->WriteConfig_CfgA_Reg2;
        ConfigData[i][3] = (&adbms_data[i])->WriteConfig_CfgA_Reg3;
        ConfigData[i][4] = (&adbms_data[i])->WriteConfig_CfgA_Reg4;
        ConfigData[i][5] = (&adbms_data[i])->WriteConfig_CfgA_Reg5;
    }

	Adbms6815_Sub_WriteDataGroup(ADBMS6815_CMD_WRCFGA, ConfigData);    
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Configuration register B setting function
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_WriteConfig_B(Adbms6815_Data* adbms_data)
{
    uint8 ConfigData[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
    uint8 i;
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

    for(i = 0; i < NumOf_Bmic; i++)
    {
        ConfigData[i][0] = (&adbms_data[i])->WriteConfig_CfgB_Reg0;
        ConfigData[i][1] = (&adbms_data[i])->WriteConfig_CfgB_Reg1;
        ConfigData[i][2] = (&adbms_data[i])->WriteConfig_CfgB_Reg2;
        ConfigData[i][3] = (&adbms_data[i])->WriteConfig_CfgB_Reg3;
        ConfigData[i][4] = (&adbms_data[i])->WriteConfig_CfgB_Reg4;
        ConfigData[i][5] = (&adbms_data[i])->WriteConfig_CfgB_Reg5;
    }
    
	Adbms6815_WriteDataGroup(ADBMS6815_CMD_WRCFGB, ConfigData);	
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Configuration register B setting Sub function
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_Sub_WriteConfig_B(Adbms6815_Data* adbms_data)
{
    uint8 ConfigData[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
    uint8 i;
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

    for(i = 0; i < NumOf_Bmic; i++)
    {
        ConfigData[i][0] = (&adbms_data[i])->WriteConfig_CfgB_Reg0;
        ConfigData[i][1] = (&adbms_data[i])->WriteConfig_CfgB_Reg1;
        ConfigData[i][2] = (&adbms_data[i])->WriteConfig_CfgB_Reg2;
        ConfigData[i][3] = (&adbms_data[i])->WriteConfig_CfgB_Reg3;
        ConfigData[i][4] = (&adbms_data[i])->WriteConfig_CfgB_Reg4;
        ConfigData[i][5] = (&adbms_data[i])->WriteConfig_CfgB_Reg5;
    }
    
	Adbms6815_Sub_WriteDataGroup(ADBMS6815_CMD_WRCFGB, ConfigData);	
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Data group writing function
* @param 	Command - Command
* @param 	Buff - write data
* @return	void
*/
void Adbms6815_WriteDataGroup(uint16 Command, uint8 Buff[][ADBMS6815_READ_DATA_SIZE])
{
	uint16 CalcCRC;
	uint8  TxCMD_Buff[4];
    uint8  TxDATA_Buff[8];
	sint8  StackIndex;
	sint8	bmic_num = (sint8)Parameter_Get_NumOf_Bmic();
	

	TxCMD_Buff[0] = (uint8)((Command >> 8u) & 0x00FFu);
	TxCMD_Buff[1] = (uint8)((Command >> 0u) & 0x00FFu);
	CalcCRC = Adbms6815_CalculateCRC15(2u, TxCMD_Buff);
	TxCMD_Buff[2] = (uint8)((CalcCRC >> 8u) & 0x00FFu);
	TxCMD_Buff[3] = (uint8)((CalcCRC >> 0u) & 0x00FFu);

	Adbms6815_Wakeup();
	
	(void)DRV_SPI_Transmit(&hspi1, TxCMD_Buff, 4u);

    
	for(StackIndex = (bmic_num - 1); StackIndex >= 0; StackIndex--)
	{
        TxDATA_Buff[0] = Buff[StackIndex][0];
        TxDATA_Buff[1] = Buff[StackIndex][1];
        TxDATA_Buff[2] = Buff[StackIndex][2];
        TxDATA_Buff[3] = Buff[StackIndex][3];
        TxDATA_Buff[4] = Buff[StackIndex][4];
        TxDATA_Buff[5] = Buff[StackIndex][5];
        
        CalcCRC = Adbms6815_CalculateCRC10(TxDATA_Buff, DEF_True, 6u);
        TxDATA_Buff[6] = (uint8)((CalcCRC >> 8u) & 0x00FFu);
        TxDATA_Buff[7] = (uint8)((CalcCRC >> 0u) & 0x00FFu);

			
		(void)DRV_SPI_Transmit(&hspi1, TxDATA_Buff, 8u);
	}
	delay_us(2);

	HAL_GPIO_WritePin(Main_SPI_CS_GPIO_Port, Main_SPI_CS_Pin, GPIO_PIN_SET);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Data group writing Sub function
* @param 	Command - Command
* @param 	Buff - write data
* @return	void
*/
void Adbms6815_Sub_WriteDataGroup(uint16 Command, uint8 Buff[][ADBMS6815_READ_DATA_SIZE])
{
	uint16 CalcCRC;
	uint8  TxCMD_Buff[4];
    uint8  TxDATA_Buff[8];
	sint8  StackIndex;
	sint8	bmic_num = (sint8)Parameter_Get_NumOf_Bmic();

	TxCMD_Buff[0] = (uint8)((Command >> 8u) & 0x00FFu);
	TxCMD_Buff[1] = (uint8)((Command >> 0u) & 0x00FFu);
	CalcCRC = Adbms6815_CalculateCRC15(2u, TxCMD_Buff);
	TxCMD_Buff[2] = (uint8)((CalcCRC >> 8u) & 0x00FFu);
	TxCMD_Buff[3] = (uint8)((CalcCRC >> 0u) & 0x00FFu);

	Adbms6815_Sub_Wakeup();
	
	(void)DRV_SPI_Transmit(&hspi3, TxCMD_Buff, 4u);

    
//	for(StackIndex = (Parameter.NumOf_Bmic - 1); StackIndex >= 0; StackIndex--)
	for(StackIndex = 0; StackIndex < bmic_num; StackIndex++)
	{
        TxDATA_Buff[0] = Buff[StackIndex][0];
        TxDATA_Buff[1] = Buff[StackIndex][1];
        TxDATA_Buff[2] = Buff[StackIndex][2];
        TxDATA_Buff[3] = Buff[StackIndex][3];
        TxDATA_Buff[4] = Buff[StackIndex][4];
        TxDATA_Buff[5] = Buff[StackIndex][5];
        
        CalcCRC = Adbms6815_CalculateCRC10(TxDATA_Buff, DEF_True, 6u);
        TxDATA_Buff[6] = (uint8)((CalcCRC >> 8u) & 0x00FFu);
        TxDATA_Buff[7] = (uint8)((CalcCRC >> 0u) & 0x00FFu);
		
		(void)DRV_SPI_Transmit(&hspi3, TxDATA_Buff, 8u);
	}
	delay_us(2);

	HAL_GPIO_WritePin(Sub_SPI_CS_GPIO_Port, Sub_SPI_CS_Pin, GPIO_PIN_SET);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Function to calculate PEC15
* @details 	Calculation of CRC15
* @param 	len - Data length
* @param	data - Data 
* @return 	uint16 - result CRC15 value
*/
uint16 Adbms6815_CalculateCRC15(uint8 len, const uint8* data)	
{
	uint16 i;
	uint16 remainder;

	remainder = 16u;  // PEC seed //
	for(i = 0; i < len; i++)
	{
		remainder = (remainder << 8u) ^ Adbms6815_Crc15Table[(((remainder >> 7u) ^ data[i]) & 0xffu)]; // The CRC15 has a 0 in the LSB so the final value must be multiplied by 2 //
	}

	return (remainder * 2u);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Function to calculate PEC10
* @details 	When data being processed is data received from device, set bIsRxCmd to true, else set to false
* @param 	pDataBuf - pDataBuf
* @param 	bIsRxCmd - bIsRxCmd
* @param 	nLength - nLength
* @return 	uint16 - result CRC10 value
*/
uint16 Adbms6815_CalculateCRC10 (uint8* pDataBuf , bool bIsRxCmd, uint8 nLength)
{
    uint16 nRemainder = 16u;/* PEC_SEED */
    uint16 nPolynomial = 0x8Fu;/* x10 + x7 + x3 + x2 + x + 1 <- the CRC10 polynomial 100 1000 1111 */
    uint8 nByteIndex;
    uint8 nBitIndex;

	#ifdef Test_Adbms6815_CalculateCRC10
	bIsRxCmd = DEF_False;
	#endif

    for (nByteIndex = 0u; nByteIndex < nLength; ++nByteIndex)
    {
        /* calculate PEC table address */
        nRemainder = ((uint16)(nRemainder << 8)) ^ Adbms6815_Crc10Table[((uint16)((uint16)(nRemainder >> 2) ^ (uint8)pDataBuf[nByteIndex]) & (uint8)0xff)];
    }   
    
    if (bIsRxCmd == DEF_True) /* If array is from received buffer add command counter to crc calculation */
    {
        nRemainder ^= (uint16)(((uint16)pDataBuf[nLength] & (uint8)0xFC) << 2u);
    }
    /* Perform modulo-2 division, a bit at a time on rest of the bits */
    for (nBitIndex = 6u; nBitIndex > 0u; --nBitIndex)
    {
        /* Try to divide the current data bit */
        if ((nRemainder & 0x200u) > 0u)
        {
            nRemainder = (uint16)((nRemainder << 1u));
            nRemainder = (uint16)(nRemainder ^ nPolynomial);
        }
        else
        {
            nRemainder = (uint16)((nRemainder << 1u));
        }
    }

    return ((uint16) (nRemainder & 0x03FFu));
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS6815 Wake up function
* @param	void
* @return 	void
*/
void Adbms6815_Wakeup(void)
{
	uint8 BmicNo;
	uint8 dummy_tx = 0x00;   
    uint8 dummy_rx;          
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	
	for (BmicNo = 0; BmicNo < NumOf_Bmic; BmicNo++)
	{
		HAL_GPIO_WritePin(Main_SPI_CS_GPIO_Port, Main_SPI_CS_Pin, GPIO_PIN_RESET);
		(void)DRV_SPI_TransmitReceive(&hspi1, &dummy_tx, &dummy_rx, 1u);
	    HAL_GPIO_WritePin(Main_SPI_CS_GPIO_Port, Main_SPI_CS_Pin, GPIO_PIN_SET);
		
	}

	HAL_GPIO_WritePin(Main_SPI_CS_GPIO_Port, Main_SPI_CS_Pin, GPIO_PIN_RESET);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS6815 Wake up Sub function
* @param	void
* @return 	void
*/
void Adbms6815_Sub_Wakeup(void)
{
	uint8 BmicNo;
	uint8 dummy_tx = 0x00;   
    uint8 dummy_rx;
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	
	for (BmicNo = 0; BmicNo < NumOf_Bmic; BmicNo++)
	{
		HAL_GPIO_WritePin(Sub_SPI_CS_GPIO_Port, Sub_SPI_CS_Pin, GPIO_PIN_RESET);
		(void)DRV_SPI_TransmitReceive(&hspi3, &dummy_tx, &dummy_rx, 1u);
	    HAL_GPIO_WritePin(Sub_SPI_CS_GPIO_Port, Sub_SPI_CS_Pin, GPIO_PIN_SET);
		
	}

	HAL_GPIO_WritePin(Sub_SPI_CS_GPIO_Port, Sub_SPI_CS_Pin, GPIO_PIN_RESET);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Configuration register reading function
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_ReadConfig(Adbms6815_Data* adbms_data)
{
    Adbms6815_ReadConfig_A(adbms_data);
    Adbms6815_ReadConfig_B(adbms_data);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Configuration register reading Sub function
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_Sub_ReadConfig(Adbms6815_Data* adbms_data)
{
    Adbms6815_Sub_ReadConfig_A(adbms_data);
    Adbms6815_Sub_ReadConfig_B(adbms_data);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Configuration register A reading function
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_ReadConfig_A(Adbms6815_Data* adbms_data)
{
    uint8 ConfigAR[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
    uint8 i;
	bool CrcPassFlag[DEFAULT_NUMBER_OF_BMIC];
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
    
    Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCFGA, ConfigAR, CrcPassFlag);

    for(i = 0; i < NumOf_Bmic; i++)
    {
		if(CrcPassFlag[i] == DEF_True)
		{
	        (&adbms_data[i])->ReadConfig_CfgA_Reg0 = ConfigAR[i][0];
	        (&adbms_data[i])->ReadConfig_CfgA_Reg1 = ConfigAR[i][1];
	        (&adbms_data[i])->ReadConfig_CfgA_Reg2 = ConfigAR[i][2];
	        (&adbms_data[i])->ReadConfig_CfgA_Reg3 = ConfigAR[i][3];
	        (&adbms_data[i])->ReadConfig_CfgA_Reg4 = ConfigAR[i][4];
	        (&adbms_data[i])->ReadConfig_CfgA_Reg5 = ConfigAR[i][5];

			RLEC_FrontSPI_Error[(i / 2u)] = DEF_False;
		}
		
		else 
		{
			Adbms_PEC_error_count_F++;
			RLEC_FrontSPI_Error[(i / 2u)] = DEF_True;
		}
    }
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Configuration register A reading Sub function
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_Sub_ReadConfig_A(Adbms6815_Data* adbms_data)
{
    uint8 ConfigAR[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
    uint8 i;
	bool CrcPassFlag[DEFAULT_NUMBER_OF_BMIC];
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
    
    Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDCFGA, ConfigAR, CrcPassFlag);

    for(i = 0; i < NumOf_Bmic; i++)
    {
		if(CrcPassFlag[i] == DEF_True)
		{
	        (&adbms_data[i])->ReadConfig_CfgA_Reg0 = ConfigAR[i][0];
	        (&adbms_data[i])->ReadConfig_CfgA_Reg1 = ConfigAR[i][1];
	        (&adbms_data[i])->ReadConfig_CfgA_Reg2 = ConfigAR[i][2];
	        (&adbms_data[i])->ReadConfig_CfgA_Reg3 = ConfigAR[i][3];
	        (&adbms_data[i])->ReadConfig_CfgA_Reg4 = ConfigAR[i][4];
	        (&adbms_data[i])->ReadConfig_CfgA_Reg5 = ConfigAR[i][5];

			RLEC_RearSPI_Error[(i / 2u)] = DEF_False;
		}
		
		else 
		{
			Adbms_PEC_error_count_R++;
			RLEC_RearSPI_Error[(i / 2u)] = DEF_True;
		}
    }
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Configuration register B reading function
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_ReadConfig_B(Adbms6815_Data* adbms_data)
{
    uint8 ConfigBR[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
    uint8 i;
	bool CrcPassFlag[DEFAULT_NUMBER_OF_BMIC];
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

    Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCFGB, ConfigBR, CrcPassFlag);

    for(i=0; i< NumOf_Bmic; i++)
    {
		if(CrcPassFlag[i] == DEF_True)
		{
	        (&adbms_data[i])->ReadConfig_CfgB_Reg0 = ConfigBR[i][0];
	        (&adbms_data[i])->ReadConfig_CfgB_Reg1 = ConfigBR[i][1];
	        (&adbms_data[i])->ReadConfig_CfgB_Reg2 = ConfigBR[i][2];
	        (&adbms_data[i])->ReadConfig_CfgB_Reg3 = ConfigBR[i][3];
	        (&adbms_data[i])->ReadConfig_CfgB_Reg4 = ConfigBR[i][4];
	        (&adbms_data[i])->ReadConfig_CfgB_Reg5 = ConfigBR[i][5];

			RLEC_FrontSPI_Error[(i / 2u)] = DEF_False;
		}
		
		else 
		{
			Adbms_PEC_error_count_F++;
			RLEC_FrontSPI_Error[(i / 2u)] = DEF_True;
		}
    }
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Configuration register B reading Sub function
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_Sub_ReadConfig_B(Adbms6815_Data* adbms_data)
{
    uint8 ConfigBR[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
    uint8 i;
	bool CrcPassFlag[DEFAULT_NUMBER_OF_BMIC];
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

    Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDCFGB, ConfigBR, CrcPassFlag);

    for(i=0; i< NumOf_Bmic; i++)
    {
		if(CrcPassFlag[i] == DEF_True)
		{
	        (&adbms_data[i])->ReadConfig_CfgB_Reg0 = ConfigBR[i][0];
	        (&adbms_data[i])->ReadConfig_CfgB_Reg1 = ConfigBR[i][1];
	        (&adbms_data[i])->ReadConfig_CfgB_Reg2 = ConfigBR[i][2];
	        (&adbms_data[i])->ReadConfig_CfgB_Reg3 = ConfigBR[i][3];
	        (&adbms_data[i])->ReadConfig_CfgB_Reg4 = ConfigBR[i][4];
	        (&adbms_data[i])->ReadConfig_CfgB_Reg5 = ConfigBR[i][5];

			RLEC_RearSPI_Error[(i / 2u)] = DEF_False;
		}
		
		else 
		{
			Adbms_PEC_error_count_R++;
			RLEC_RearSPI_Error[(i / 2u)] = DEF_True;
		}
    }
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Data group reading function
* @param 	Command - Command
* @param 	Buff - read data
* @param 	*CrcPassFlag - crc result
* @return	void
*/
void Adbms6815_ReadDataGroup(uint16 Command, uint8 Buff[][ADBMS6815_READ_DATA_SIZE], bool* CrcPassFlag)
{
	uint8  TxBuff[4];
    uint16 CalcCRC;    
	uint8  Index;
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	 
	TxBuff[0] = (uint8)((Command >> 8u) & 0x00FFu);
	TxBuff[1] = (uint8)((Command >> 0u) & 0x00FFu);
	CalcCRC = Adbms6815_CalculateCRC15(2u, TxBuff);
	TxBuff[2] = (uint8)((CalcCRC >> 8u) & 0x00FFu);
	TxBuff[3] = (uint8)((CalcCRC >> 0u) & 0x00FFu);
	  
	Adbms6815_Wakeup();	

	(void)DRV_SPI_Transmit(&hspi1, TxBuff, 4u);
	
	for(Index = 0u; Index < NumOf_Bmic; Index++)
	{
		#ifdef Test_Adbms6815_ReadConfig_A
		if(Index == 0u || Index == 1u) 
		{
			Buff[Index][0] = 132u; 
			Buff[Index][1] = 0u; 
	        Buff[Index][2] = 81u; 
			Buff[Index][3] = 255u; 
			Buff[Index][4] = 0u; 
			Buff[Index][5] = 0u; 
	        Buff[Index][6] = 0x03; 
			Buff[Index][7] = 0x95;
		}
		else
		{
			Buff[Index][0] = 132u; 
			Buff[Index][1] = 0u; 
	        Buff[Index][2] = 81u; 
			Buff[Index][3] = 255u; 
			Buff[Index][4] = 0u; 
			Buff[Index][5] = 0u; 
	        Buff[Index][6] = 0x11; 
			Buff[Index][7] = 0x22;
		}
		
		#elif defined(Test_Adbms6815_ReadCellVolt)
		if(Index == 0u || Index == 1u) 
		{
	        Buff[Index][0] = 0xE8; 
			Buff[Index][1] = 0x80; 
	        Buff[Index][2] = 0xE9; 
			Buff[Index][3] = 0x80; 
			Buff[Index][4] = 0xEA; 
			Buff[Index][5] = 0x80; 
	        Buff[Index][6] = 0x03; 
			Buff[Index][7] = 0x1B;
        } 
		else 
		{
	        Buff[Index][0] = 0x00; 
			Buff[Index][1] = 0x80; 
	        Buff[Index][2] = 0xE9; 
			Buff[Index][3] = 0x80; 
			Buff[Index][4] = 0xEA; 
			Buff[Index][5] = 0x80; 
	        Buff[Index][6] = 0x03; 
			Buff[Index][7] = 0x1B;
        }
		
		#elif defined(Test_Adbms6815_ReadAuxVolt)
		Buff[Index][0] = 0x98; 
		Buff[Index][1] = 0x3A; 
        Buff[Index][2] = 0xAC; 
		Buff[Index][3] = 0x3B; 
		Buff[Index][4] = 0xC0; 
		Buff[Index][5] = 0x3C; 
        Buff[Index][6] = 0x01; 
		Buff[Index][7] = 0x21;

		#elif defined(Test_Adbms6815_ReadStatus)
		if(Index == 0u) 
		{
	        Buff[Index][0] = 0x90; 
			Buff[Index][1] = 0x33; 
	        Buff[Index][2] = 0x7F; 
			Buff[Index][3] = 0x53; 
			Buff[Index][4] = 0x7F; 
			Buff[Index][5] = 0x53; 
	        Buff[Index][6] = 0x03; 
			Buff[Index][7] = 0xAD;
        } 
		else if(Index == 1u)
		{
			Buff[Index][0] = 0x30; 
			Buff[Index][1] = 0x75; 
	        Buff[Index][2] = 0x00; 
			Buff[Index][3] = 0x00; 
			Buff[Index][4] = 0x00; 
			Buff[Index][5] = 0x00; 
	        Buff[Index][6] = 0x02; 
			Buff[Index][7] = 0x80;
		}
		else 
		{
	        Buff[Index][0] = 0x00; 
			Buff[Index][1] = 0x20; 
	        Buff[Index][2] = 0xFF; 
			Buff[Index][3] = 0xFF; 
			Buff[Index][4] = 0xFF; 
			Buff[Index][5] = 0xFF; 
	        Buff[Index][6] = 0x03; 
			Buff[Index][7] = 0xC0;
        }
		
		
		#elif defined(Test_Adbms6815_ReadDataGroup)
		if(Index == 0u || Index == 1u) 
		{
	        Buff[Index][0] = 0xE8; 
			Buff[Index][1] = 0x80; 
	        Buff[Index][2] = 0xE9; 
			Buff[Index][3] = 0x80; 
			Buff[Index][4] = 0xEA; 
			Buff[Index][5] = 0x80; 
	        Buff[Index][6] = 0x03; 
			Buff[Index][7] = 0x1B;
        } 
		else 
		{
	        Buff[Index][0] = 0xE8; 
			Buff[Index][1] = 0x80; 
	        Buff[Index][2] = 0xE9; 
			Buff[Index][3] = 0x80; 
			Buff[Index][4] = 0xEA; 
			Buff[Index][5] = 0x80; 
	        Buff[Index][6] = 0x11; 
			Buff[Index][7] = 0x22;
        }
		
		#else
		(void)DRV_SPI_Receive(&hspi1, Buff[Index], 8u);
		
		#endif
	}
	
	delay_us(2);
	
	HAL_GPIO_WritePin(Main_SPI_CS_GPIO_Port, Main_SPI_CS_Pin, GPIO_PIN_SET);

	for(Index = 0u; Index < NumOf_Bmic; Index++)
	{
        CalcCRC = Adbms6815_CalculateCRC10(Buff[Index], DEF_True, 6);

		if(CalcCRC != ((((uint16)(Buff[Index][6]) << 8u) | ((uint16)(Buff[Index][7]))) & 0x3FFu))
		{           		    
			CrcPassFlag[Index] = DEF_False;
		}
		else
		{
			CrcPassFlag[Index] = DEF_True;
		}
	}	
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS Data group reading Sub function
* @param 	Command - Command
* @param 	Buff - read data
* @param 	*CrcPassFlag - crc result
* @return	void
*/
void Adbms6815_Sub_ReadDataGroup(uint16 Command, uint8 Buff[][ADBMS6815_READ_DATA_SIZE], bool* CrcPassFlag)
{
	uint8  TxBuff[4];
    uint16 CalcCRC;    
	sint8  Index;
	sint8	bmic_num = (sint8)Parameter_Get_NumOf_Bmic();
	 
	TxBuff[0] = (uint8)((Command >> 8u) & 0x00FFu);
	TxBuff[1] = (uint8)((Command >> 0u) & 0x00FFu);
	CalcCRC = Adbms6815_CalculateCRC15(2u, TxBuff);
	TxBuff[2] = (uint8)((CalcCRC >> 8u) & 0x00FFu);
	TxBuff[3] = (uint8)((CalcCRC >> 0u) & 0x00FFu);
	  
	Adbms6815_Sub_Wakeup();

	(void)DRV_SPI_Transmit(&hspi3, TxBuff, 4u);

	
	for(Index = (bmic_num - 1); Index >= 0; Index--)
	{
		(void)DRV_SPI_Receive(&hspi3, Buff[Index], 8u);
	}
	
	delay_us(2);
	
	HAL_GPIO_WritePin(Sub_SPI_CS_GPIO_Port, Sub_SPI_CS_Pin, GPIO_PIN_SET);

	for(Index = 0; Index < bmic_num; Index++)
	{
        CalcCRC = Adbms6815_CalculateCRC10(Buff[Index], DEF_True, 6);

		if(CalcCRC != ((((uint16)(Buff[Index][6]) << 8u) | ((uint16)(Buff[Index][7]))) & 0x3FFu))
		{           		    
			CrcPassFlag[Index] = DEF_False;
		}
		else
		{
			CrcPassFlag[Index] = DEF_True;
		}
	}	
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Convertion of ADBMS6815 Read mode to Cell V
* @param 	void
* @return	void
*/
void Adbms6815_CellConvert(void)
{
    Adbms6815_WriteSingleCmd(ADBMS6815_CMD_ADCV | ADBMS6815_CBD_MD_7K | ADBMS6815_CBD_CH_ALL);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Convertion of ADBMS6815 Sub Read mode to Cell V
* @param 	void
* @return	void
*/
void Adbms6815_Sub_CellConvert(void)
{
    Adbms6815_Sub_WriteSingleCmd(ADBMS6815_CMD_ADCV | ADBMS6815_CBD_MD_7K | ADBMS6815_CBD_CH_ALL);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Write Singgle Command to ADBMS6815
* @param 	RequestCMD - Request command
* @return	void
*/
void Adbms6815_WriteSingleCmd(uint16 RequestCMD)
{
	uint16 CalcCRC;
	uint8  TxBuff[4];
           
	TxBuff[0] = (uint8)((RequestCMD >> 8u) & 0x00FFu);
	TxBuff[1] = (uint8)((RequestCMD >> 0u) & 0x00FFu);
	CalcCRC = Adbms6815_CalculateCRC15(2u, TxBuff);
	TxBuff[2] = (uint8)((CalcCRC >> 8u) & 0x00FFu);
	TxBuff[3] = (uint8)((CalcCRC >> 0u) & 0x00FFu);

	Adbms6815_Wakeup();
	
	(void)DRV_SPI_Transmit(&hspi1, TxBuff, 4u);

	delay_us(2);
	
	HAL_GPIO_WritePin(Main_SPI_CS_GPIO_Port, Main_SPI_CS_Pin, GPIO_PIN_SET);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Write Sub Singgle Command to ADBMS6815
* @param 	RequestCMD - Request command
* @return	void
*/
void Adbms6815_Sub_WriteSingleCmd(uint16 RequestCMD)
{
	uint16 CalcCRC;
	uint8  TxBuff[4];
           
	TxBuff[0] = (uint8)((RequestCMD >> 8u) & 0x00FFu);
	TxBuff[1] = (uint8)((RequestCMD >> 0u) & 0x00FFu);
	CalcCRC = Adbms6815_CalculateCRC15(2u, TxBuff);
	TxBuff[2] = (uint8)((CalcCRC >> 8u) & 0x00FFu);
	TxBuff[3] = (uint8)((CalcCRC >> 0u) & 0x00FFu);

	Adbms6815_Sub_Wakeup();
	
	(void)DRV_SPI_Transmit(&hspi3, TxBuff, 4u);

	delay_us(2);
	
	HAL_GPIO_WritePin(Sub_SPI_CS_GPIO_Port, Sub_SPI_CS_Pin, GPIO_PIN_SET);
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Read Cell voltage of ADBMS6815
* @param 	adbms_data - adbms data
* @param 	DataNo - Data No
* @return	void
*/
void Adbms6815_ReadCellVolt(Adbms6815_Data* adbms_data, uint8 DataNo)
{	
	uint8 BmicNo;
	bool crc_flag[DEFAULT_NUMBER_OF_BMIC];
    uint8 rbuf[DEFAULT_NUMBER_OF_BMIC][8];
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	switch (DataNo)
	{
		case 0:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
		
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCVA, rbuf, crc_flag);

		    for(BmicNo = 0; BmicNo < NumOf_Bmic; BmicNo++)
		    {		 
				if(crc_flag[BmicNo] == DEF_True)
				{
		    		(&adbms_data[BmicNo])->CVR[0] = ((uint16)rbuf[BmicNo][1] << 8) + (uint16)rbuf[BmicNo][0];
		    		(&adbms_data[BmicNo])->CVR[1] = ((uint16)rbuf[BmicNo][3] << 8) + (uint16)rbuf[BmicNo][2];
		    		(&adbms_data[BmicNo])->CVR[2] = ((uint16)rbuf[BmicNo][5] << 8) + (uint16)rbuf[BmicNo][4];	

					RLEC_FrontSPI_Error[(BmicNo / 2u)] = DEF_False;
		        }
				else 
				{
					Adbms_PEC_error_count_F++;	
					RLEC_FrontSPI_Error[(BmicNo / 2u)] = DEF_True;
				}
			}
			break;

		case 1:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCVB, rbuf, crc_flag);
			
			for(BmicNo = 0; BmicNo < NumOf_Bmic; BmicNo++)
			{		 
				if(crc_flag[BmicNo] == DEF_True)
				{
					(&adbms_data[BmicNo])->CVR[3] = ((uint16)rbuf[BmicNo][1] << 8) + (uint16)rbuf[BmicNo][0];
					(&adbms_data[BmicNo])->CVR[4] = ((uint16)rbuf[BmicNo][3] << 8) + (uint16)rbuf[BmicNo][2];
					(&adbms_data[BmicNo])->CVR[5] = ((uint16)rbuf[BmicNo][5] << 8) + (uint16)rbuf[BmicNo][4];	
				}
				
				else 
				{
					Adbms_PEC_error_count_F++;	
				}
			}
			break;

		case 2:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCVC, rbuf, crc_flag);
			
			for(BmicNo = 0; BmicNo < NumOf_Bmic; BmicNo++)
			{		 
				if(crc_flag[BmicNo] == DEF_True)
				{
					(&adbms_data[BmicNo])->CVR[6] = ((uint16)rbuf[BmicNo][1] << 8) + (uint16)rbuf[BmicNo][0];
					(&adbms_data[BmicNo])->CVR[7] = ((uint16)rbuf[BmicNo][3] << 8) + (uint16)rbuf[BmicNo][2];
					(&adbms_data[BmicNo])->CVR[8] = ((uint16)rbuf[BmicNo][5] << 8) + (uint16)rbuf[BmicNo][4];	
				}
				
				else 
				{
					Adbms_PEC_error_count_F++;	
				}
			}
			break;

		case 3:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCVD, rbuf, crc_flag);
			
			for(BmicNo = 0; BmicNo < NumOf_Bmic; BmicNo++)
			{		 
				if(crc_flag[BmicNo] == DEF_True)
				{
					(&adbms_data[BmicNo])->CVR[9] = ((uint16)rbuf[BmicNo][1] << 8) + (uint16)rbuf[BmicNo][0];
					(&adbms_data[BmicNo])->CVR[10] = ((uint16)rbuf[BmicNo][3] << 8) + (uint16)rbuf[BmicNo][2];
					(&adbms_data[BmicNo])->CVR[11] = ((uint16)rbuf[BmicNo][5] << 8) + (uint16)rbuf[BmicNo][4];	
				}
				
				else 
				{
					Adbms_PEC_error_count_F++;	
				}
			}
			break;

		default :
			break;
	}
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Sub Read Cell voltage of ADBMS6815
* @param 	adbms_data - adbms data
* @param 	DataNo - Data No
* @return	void
*/
void Adbms6815_Sub_ReadCellVolt(Adbms6815_Data* adbms_data, uint8 DataNo)
{	
	uint8 BmicNo;
	bool crc_flag[DEFAULT_NUMBER_OF_BMIC];
    uint8 rbuf[DEFAULT_NUMBER_OF_BMIC][8];
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	switch (DataNo)
	{
		case 0:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDCVA, rbuf, crc_flag);

		    for(BmicNo = 0; BmicNo < NumOf_Bmic; BmicNo++)
		    {		 
				if(crc_flag[BmicNo] == DEF_True)
				{
		    		(&adbms_data[BmicNo])->CVR[0] = ((uint16)rbuf[BmicNo][1] << 8) + (uint16)rbuf[BmicNo][0];
		    		(&adbms_data[BmicNo])->CVR[1] = ((uint16)rbuf[BmicNo][3] << 8) + (uint16)rbuf[BmicNo][2];
		    		(&adbms_data[BmicNo])->CVR[2] = ((uint16)rbuf[BmicNo][5] << 8) + (uint16)rbuf[BmicNo][4];	

					RLEC_RearSPI_Error[(BmicNo / 2u)] = DEF_False;
		        }
				else 
				{
					Adbms_PEC_error_count_R++; 
					RLEC_RearSPI_Error[(BmicNo / 2u)] = DEF_True;
				}
			}
			break;

		case 1:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDCVB, rbuf, crc_flag);
			
			for(BmicNo = 0; BmicNo < NumOf_Bmic; BmicNo++)
			{		 
				if(crc_flag[BmicNo] == DEF_True)
				{
					(&adbms_data[BmicNo])->CVR[3] = ((uint16)rbuf[BmicNo][1] << 8) + (uint16)rbuf[BmicNo][0];
					(&adbms_data[BmicNo])->CVR[4] = ((uint16)rbuf[BmicNo][3] << 8) + (uint16)rbuf[BmicNo][2];
					(&adbms_data[BmicNo])->CVR[5] = ((uint16)rbuf[BmicNo][5] << 8) + (uint16)rbuf[BmicNo][4];	
				}
				
				else 
				{
					Adbms_PEC_error_count_R++;	
				}
			}
			break;

		case 2:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDCVC, rbuf, crc_flag);
			
			for(BmicNo = 0; BmicNo < NumOf_Bmic; BmicNo++)
			{		 
				if(crc_flag[BmicNo] == DEF_True)
				{
					(&adbms_data[BmicNo])->CVR[6] = ((uint16)rbuf[BmicNo][1] << 8) + (uint16)rbuf[BmicNo][0];
					(&adbms_data[BmicNo])->CVR[7] = ((uint16)rbuf[BmicNo][3] << 8) + (uint16)rbuf[BmicNo][2];
					(&adbms_data[BmicNo])->CVR[8] = ((uint16)rbuf[BmicNo][5] << 8) + (uint16)rbuf[BmicNo][4];	
				}
				
				else 
				{
					Adbms_PEC_error_count_R++;	
				}
			}
			break;

		case 3:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDCVD, rbuf, crc_flag);
			
			for(BmicNo = 0; BmicNo < NumOf_Bmic; BmicNo++)
			{		 
				if(crc_flag[BmicNo] == DEF_True)
				{
					(&adbms_data[BmicNo])->CVR[9] = ((uint16)rbuf[BmicNo][1] << 8) + (uint16)rbuf[BmicNo][0];
					(&adbms_data[BmicNo])->CVR[10] = ((uint16)rbuf[BmicNo][3] << 8) + (uint16)rbuf[BmicNo][2];
					(&adbms_data[BmicNo])->CVR[11] = ((uint16)rbuf[BmicNo][5] << 8) + (uint16)rbuf[BmicNo][4];	
				}
				
				else 
				{
					Adbms_PEC_error_count_R++;	
				}
			}
			break;

		default :
			break;
	}
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Convertion of ADBMS6815 Read mode to Aux V
* @param 	void
* @return	void
*/
void Adbms6815_AuxConvert(void)
{
    Adbms6815_WriteSingleCmd(ADBMS6815_CMD_ADAX | ADBMS6815_CBD_MD_7K | ADBMS6815_CBD_CHG_ALL);    
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Convertion of ADBMS6815 Sub Read mode to Aux V
* @param 	void
* @return	void
*/
void Adbms6815_Sub_AuxConvert(void)
{
    Adbms6815_Sub_WriteSingleCmd(ADBMS6815_CMD_ADAX | ADBMS6815_CBD_MD_7K | ADBMS6815_CBD_CHG_ALL);    
}


/*---------------------------------------------------------------------*/
/*
* @brief	ADBMS6815 read aux data
* @param 	adbms_data - ADBMS6815_Data
* @param 	DataNo - ADBMS6815_Data No
* @return	void
*/
void Adbms6815_ReadAuxVolt(Adbms6815_Data* adbms_data, uint8 DataNo)
{
    uint8 rbuf[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
	uint8 i;
    bool crc_flag[DEFAULT_NUMBER_OF_BMIC];
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	switch (DataNo)
	{
		case 0:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDAUXA, rbuf, crc_flag);
			
			for(i = 0; i < NumOf_Bmic; i++)
			{
			    if(crc_flag[i] == DEF_True)
			    {
		            (&adbms_data[i])->REF2 = ((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0];
		            (&adbms_data[i])->GV[0] = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];
		            (&adbms_data[i])->GV[1] = ((uint16)rbuf[i][5] << 8) + (uint16)rbuf[i][4];

					RLEC_FrontSPI_Error[(i / 2u)] = DEF_False;
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
					RLEC_FrontSPI_Error[(i / 2u)] = DEF_True;
				}
		    }
			break;

		case 1:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDAUXB, rbuf, crc_flag);
			for(i = 0; i < NumOf_Bmic; i++)
			{
			    if(crc_flag[i] == DEF_True)
			    {
		            (&adbms_data[i])->GV[2] = ((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0];
		            (&adbms_data[i])->GV[3] = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];
		            (&adbms_data[i])->GV[4] = ((uint16)rbuf[i][5] << 8) + (uint16)rbuf[i][4];  
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
				}
		    }
			break;
			
		case 2:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDAUXC, rbuf, crc_flag);
			for(i = 0; i < NumOf_Bmic; i++)
			{
				if(crc_flag[i] == DEF_True)
				{
					(&adbms_data[i])->GV[5] = ((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0];
					(&adbms_data[i])->GV[6] = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];
					(&adbms_data[i])->REF3 = ((uint16)rbuf[i][5] << 8) + (uint16)rbuf[i][4];  
				}
				
				else 
				{
					Adbms_PEC_error_count_F++;
				}
			}
			break;

		default :
			break;
	}
}


/*---------------------------------------------------------------------*/
/*
* @brief	Sub ADBMS6815 read aux data
* @param 	adbms_data - ADBMS6815_Data
* @param 	DataNo - ADBMS6815_Data No
* @return	void
*/
void Adbms6815_Sub_ReadAuxVolt(Adbms6815_Data* adbms_data, uint8 DataNo)
{
    uint8 rbuf[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
	uint8 i;
    bool crc_flag[DEFAULT_NUMBER_OF_BMIC];
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	switch (DataNo)
	{
		case 0:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDAUXA, rbuf, crc_flag);
			for(i = 0; i < NumOf_Bmic; i++)
			{
			    if(crc_flag[i] == DEF_True)
			    {
		            (&adbms_data[i])->REF2 = ((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0];
		            (&adbms_data[i])->GV[0] = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];
		            (&adbms_data[i])->GV[1] = ((uint16)rbuf[i][5] << 8) + (uint16)rbuf[i][4];  

					RLEC_RearSPI_Error[(i / 2u)] = DEF_False;
		        }
				
				else 
				{
					Adbms_PEC_error_count_R++;
					RLEC_RearSPI_Error[(i / 2u)] = DEF_True;
				}
		    }
			break;

		case 1:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDAUXB, rbuf, crc_flag);
			for(i = 0; i < NumOf_Bmic; i++)
			{
			    if(crc_flag[i] == DEF_True)
			    {
		            (&adbms_data[i])->GV[2] = ((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0];
		            (&adbms_data[i])->GV[3] = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];
		            (&adbms_data[i])->GV[4] = ((uint16)rbuf[i][5] << 8) + (uint16)rbuf[i][4];  
		        }
				
				else 
				{
					Adbms_PEC_error_count_R++;
				}
		    }
			break;
			
		case 2:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDAUXC, rbuf, crc_flag);
			for(i = 0; i < NumOf_Bmic; i++)
			{
				if(crc_flag[i] == DEF_True)
				{
					(&adbms_data[i])->GV[5] = ((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0];
					(&adbms_data[i])->GV[6] = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];
					(&adbms_data[i])->REF3 = ((uint16)rbuf[i][5] << 8) + (uint16)rbuf[i][4];  
				}
				
				else 
				{
					Adbms_PEC_error_count_R++;
				}
			}
			break;

		default :
			break;
	}
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Convertion of ADBMS6815 Read mode to Status
* @param 	void
* @return	void
*/
void Adbms6815_StatusConvert(void)
{
    Adbms6815_WriteSingleCmd(ADBMS6815_CMD_ADSTAT | ADBMS6815_CBD_MD_7K | ADBMS6815_CBD_CHST_ALL);    
}


/*---------------------------------------------------------------------*/
/*
* @brief 	Convertion of ADBMS6815 Sub Read mode to Status
* @param 	void
* @return	void
*/
void Adbms6815_Sub_StatusConvert(void)
{
    Adbms6815_Sub_WriteSingleCmd(ADBMS6815_CMD_ADSTAT | ADBMS6815_CBD_MD_7K | ADBMS6815_CBD_CHST_ALL);    
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS6815 read status data
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_ReadStatus(Adbms6815_Data* adbms_data, uint8 DataNo)
{
    uint8  rbuf[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
	uint8  i;
	uint8	j;
    uint32 ouv_buf;
    bool crc_flag[DEFAULT_NUMBER_OF_BMIC];
	float fdata;
	uint16 u16data;
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	switch (DataNo)
	{
		case 0:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDSTATA, rbuf, crc_flag);
			for(i = 0; i < NumOf_Bmic; i++)
			{
			    if(crc_flag[i] == DEF_True)
			    {
		            (&adbms_data[i])->SC = (((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0]) * 2u;

					u16data = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];
					fdata = (float)u16data * 0.1f / 7.5f - 260.f;
		            (&adbms_data[i])->ITMP = (sint16)fdata;
					
		            (&adbms_data[i])->VA = ((uint16)rbuf[i][5] << 8) + (uint16)rbuf[i][4];  

					RLEC_FrontSPI_Error[(i / 2u)] = DEF_False;
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
					RLEC_FrontSPI_Error[(i / 2u)] = DEF_True;
				}
		    }
			break;

		case 1:
		   (void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDSTATB, rbuf, crc_flag);
		    for(i = 0; i < NumOf_Bmic; i++)
		    {
				if(crc_flag[i] == DEF_True)
		    	{
		            (&adbms_data[i])->VD = ((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0];
		            ouv_buf = ((uint32)rbuf[i][4] << 16) + ((uint32)rbuf[i][3] << 8) + (uint32)rbuf[i][2];
		            for(j = 0u; j < 12u; j++)
		            {
		                if((ouv_buf & ((uint32)1u << ((uint32)j * 2u))) != 0u)      
						{
							(&adbms_data[i])->CUV_State |=  ((uint16)1 << j);
		                }
		                else                                	
						{
							(&adbms_data[i])->CUV_State &= ~((uint16)1 << j); 
		                }

		                if((ouv_buf & ((uint32)1u << (((uint32)j * 2u) + 1u))) != 0u)   
						{
							(&adbms_data[i])->COV_State |=  ((uint16)1 << j);
		                }
		                else                                	
						{
							(&adbms_data[i])->COV_State &= ~((uint16)1 << j);
		                }
		            }	
		          	(&adbms_data[i])->OC_CNTR = rbuf[i][5];	        	
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
				}
		    }
			break;

		case 2:	
  		  	(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDSTATC, rbuf, crc_flag);
        	for(i = 0; i < NumOf_Bmic; i++)
        	{
				if(crc_flag[i] == DEF_True)
    			{
        		    (&adbms_data[i])->Stcr0 = rbuf[i][0];
        	    	(&adbms_data[i])->Stcr1 = rbuf[i][1];
        			(&adbms_data[i])->ADOL[0] = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];
        			(&adbms_data[i])->ADOL[1] = ((uint16)rbuf[i][5] << 8) + (uint16)rbuf[i][4];
        		}
				
				else 
				{
					Adbms_PEC_error_count_F++;
				}
    		}
			break;

		default :
			break;
	}
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS6815 Sub read status data
* @param 	adbms_data - ADBMS6815_Data
* @return	void
*/
void Adbms6815_Sub_ReadStatus(Adbms6815_Data* adbms_data, uint8 DataNo)
{
    uint8  rbuf[DEFAULT_NUMBER_OF_BMIC][ADBMS6815_READ_DATA_SIZE];
	uint8  i;
	uint8	j;
    uint32 ouv_buf;
    bool crc_flag[DEFAULT_NUMBER_OF_BMIC];
	float fdata;
	uint16 u16data;
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

	switch (DataNo)
	{
		case 0:
			(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDSTATA, rbuf, crc_flag);
			for(i = 0; i < NumOf_Bmic; i++)
			{
			    if(crc_flag[i] == DEF_True)
			    {
		            (&adbms_data[i])->SC = (((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0]) * 2u;     

					u16data = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];
					fdata = (float)u16data * 0.1f / 7.5f - 260.f;
		            (&adbms_data[i])->ITMP = (sint16)fdata;
					
		            (&adbms_data[i])->VA = ((uint16)rbuf[i][5] << 8) + (uint16)rbuf[i][4];  

					RLEC_RearSPI_Error[(i / 2u)] = DEF_False;
		        }
				
				else 
				{
					Adbms_PEC_error_count_R++;
					RLEC_RearSPI_Error[(i / 2u)] = DEF_True;
				}
		    }
			break;

		case 1:
		    (void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDSTATB, rbuf, crc_flag);
		    for(i = 0; i < NumOf_Bmic; i++)
		    {
				if(crc_flag[i] == DEF_True)
		    	{
		            (&adbms_data[i])->VD = ((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0];
		            ouv_buf = ((uint32)rbuf[i][4] << 16) + ((uint32)rbuf[i][3] << 8) + (uint32)rbuf[i][2];
		            for(j = 0u; j < 12u; j++)
		            {
		                if((ouv_buf & ((uint32)1u << ((uint32)j * 2u))) != 0u)       
						{
							(&adbms_data[i])->CUV_State |=  ((uint16)1 << j);
		                }
		                else                                	
						{
							(&adbms_data[i])->CUV_State &= ~((uint16)1 << j);
		                }

		                if((ouv_buf & ((uint32)1u << (((uint32)j * 2u) + 1u)))  != 0u)  
						{
							(&adbms_data[i])->COV_State |=  ((uint16)1 << j);
		                }
		                else                                
						{
							(&adbms_data[i])->COV_State &= ~((uint16)1 << j);
		                }
		            }	
		          	(&adbms_data[i])->OC_CNTR = rbuf[i][5];	        	
		        }
				
				else 
				{
					Adbms_PEC_error_count_R++;
				}
		    }
			break;

		case 2:	
  		  	(void)memset(rbuf, 0x00, sizeof(rbuf));
			Adbms6815_Sub_ReadDataGroup(ADBMS6815_CMD_RDSTATC, rbuf, crc_flag);
        	for(i = 0; i < NumOf_Bmic; i++)
        	{
				if(crc_flag[i] == DEF_True)
    			{
        		    (&adbms_data[i])->Stcr0 = rbuf[i][0];
        	    	(&adbms_data[i])->Stcr1 = rbuf[i][1];
        			(&adbms_data[i])->ADOL[0] = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];
        			(&adbms_data[i])->ADOL[1] = ((uint16)rbuf[i][5] << 8) + (uint16)rbuf[i][4];
        		}
				
				else 
				{
					Adbms_PEC_error_count_R++;
				}
    		}
			break;

		default :
			break;
	}
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS6815 check cell open wire
* @param 	adbms_data - ADBMS6815_Data
* @return 	uint8 - if sequence finished then return true
*/
bool Adbms6815_CheckCellOpenWire(Adbms6815_Data* adbms_data)
{	
    CLASSB static uint8 seq;
    uint8 i;
	uint8 j;    
    bool rv = DEF_False;
	CLASSB static bool crc_flag[DEFAULT_NUMBER_OF_BMIC];
    CLASSB static uint8 rbuf[DEFAULT_NUMBER_OF_BMIC][8];
    CLASSB static uint16 adm_buff[DEFAULT_NUMBER_OF_BMIC][3];
	uint16 NumOf_Bmic;
	uint16 NumOfCellinModule;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfCellinModule = Parameter_Get_NumOfCellinModule();
    
    switch(seq)
    {
        case 0:     /* Send CVOW Command */
            Adbms6815_WriteSingleCmd(ADBMS6815_CMD_CVOW | ADBMS6815_CBD_MD_7K | ADBMS6815_CBD_CH_ALL);        
			seq++;
			break;
            
        case 1:     /* Read CVOw Status */
            Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCDA, rbuf, crc_flag);

		    for(i = 0; i < NumOf_Bmic; i++)
		    {		 
				if(crc_flag[i] == DEF_True)
				{
		    		adm_buff[i][0] = ((uint16)rbuf[i][1] << 8u) + (uint16)rbuf[i][0];
		    		adm_buff[i][1] = ((uint16)rbuf[i][3] << 8u) + (uint16)rbuf[i][2];
		    		adm_buff[i][2] = ((uint16)rbuf[i][5] << 8u) + (uint16)rbuf[i][4];

					RLEC_FrontSPI_Error[(i / 2u)] = DEF_False;
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
					RLEC_FrontSPI_Error[(i / 2u)] = DEF_True;
				}
			}

			if(Adbms_PEC_error_count_F == 0u)
			{
			
			for(i = 0u; i < NumOf_Bmic; i++)
            {
				if(NumOfCellinModule >= 3u)
				{
	                for(j = 0u; j < 3u; j++)
	                {
	                    if	   (adm_buff[i][j] == 0x7FA0u)
						{
							(&adbms_data[i])->OWD[j] = OWD_DRF;
	                    }
	                    else if(adm_buff[i][j] == 0x7FC0u)   			
						{
							(&adbms_data[i])->OWD[j] = OWD_CVMIN;
	                    }
	                    else if(adm_buff[i][j] > ADBMS6815_VTH_SPF) 	
						{
							(&adbms_data[i])->OWD[j] = OWD_OPEN;
	                    }
	                    else if(adm_buff[i][j] < ADBMS6815_VTH_LF)  	
						{
							(&adbms_data[i])->OWD[j] = OWD_LATENT;
	                    }
	                    else                                        	
						{
							(&adbms_data[i])->OWD[j] = OWD_NO_ERROR;
	                    }
	                }
				}
				else
				{
					for(j = 0u; j < NumOfCellinModule; j++)
	                {
	                    if	   (adm_buff[i][j] == 0x7FA0u)				
						{
							(&adbms_data[i])->OWD[j] = OWD_DRF;
	                    }
	                    else if(adm_buff[i][j] == 0x7FC0u)   			
						{
							(&adbms_data[i])->OWD[j] = OWD_CVMIN;
	                    }
	                    else if(adm_buff[i][j] > ADBMS6815_VTH_SPF) 	
						{
							(&adbms_data[i])->OWD[j] = OWD_OPEN;
	                    }
	                    else if(adm_buff[i][j] < ADBMS6815_VTH_LF)  	
						{
							(&adbms_data[i])->OWD[j] = OWD_LATENT;
	                    }
	                    else                                        	
						{
							(&adbms_data[i])->OWD[j] = OWD_NO_ERROR;
	                    }
	                }
				}
            }

			}
			seq++;
			break;
		
		case 2:
            Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCDB, rbuf, crc_flag);

		    for(i = 0; i < NumOf_Bmic; i++)
		    {		 
				if(crc_flag[i] == DEF_True)
				{
		    		adm_buff[i][0] = ((uint16)rbuf[i][1] << 8u) + (uint16)rbuf[i][0];
		    		adm_buff[i][1] = ((uint16)rbuf[i][3] << 8u) + (uint16)rbuf[i][2];
		    		adm_buff[i][2] = ((uint16)rbuf[i][5] << 8u) + (uint16)rbuf[i][4];	
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
				}
			}

			if(Adbms_PEC_error_count_F == 0u)
			{
			
			for(i = 0u; i < NumOf_Bmic; i++)
            {
				if(NumOfCellinModule >= 6u)
				{
	                for(j = 0u; j < 3u; j++)
	                {
	                    if	   (adm_buff[i][j] == 0x7FA0u)				
						{
							(&adbms_data[i])->OWD[j + 3u] = OWD_DRF;
	                    }
	                    else if(adm_buff[i][j] == 0x7FC0u)   			
						{
							(&adbms_data[i])->OWD[j + 3u] = OWD_CVMIN;
	                    }
	                    else if(adm_buff[i][j] > ADBMS6815_VTH_SPF) 	
						{
							(&adbms_data[i])->OWD[j + 3u] = OWD_OPEN;
	                    }
	                    else if(adm_buff[i][j] < ADBMS6815_VTH_LF)  	
						{
							(&adbms_data[i])->OWD[j + 3u] = OWD_LATENT;
	                    }
	                    else                                        	
						{
							(&adbms_data[i])->OWD[j + 3u] = OWD_NO_ERROR;
	                    }
	                }
				}
				else if((NumOfCellinModule == 4u) || (NumOfCellinModule == 5u))
				{
					for(j = 0u; j < (NumOfCellinModule - 3u); j++)
	                {
	                    if	   (adm_buff[i][j] == 0x7FA0u)				
						{
							(&adbms_data[i])->OWD[j + 3u] = OWD_DRF;
	                    }
	                    else if(adm_buff[i][j] == 0x7FC0u)   			
						{
							(&adbms_data[i])->OWD[j + 3u] = OWD_CVMIN;
	                    }
	                    else if(adm_buff[i][j] > ADBMS6815_VTH_SPF) 	
						{
							(&adbms_data[i])->OWD[j + 3u] = OWD_OPEN;
	                    }
	                    else if(adm_buff[i][j] < ADBMS6815_VTH_LF)  	
						{
							(&adbms_data[i])->OWD[j + 3u] = OWD_LATENT;
	                    }
	                    else                                        	
						{
							(&adbms_data[i])->OWD[j + 3u] = OWD_NO_ERROR;
	                    }
	                }
				}
				else
				{
					/* Do nothing */
				}
            }

			}
			seq++;
			break;

        case 3:     /* Read CVOw Status */
            Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCDC, rbuf, crc_flag);

		    for(i = 0u; i < NumOf_Bmic; i++)
		    {		 
				if(crc_flag[i] == DEF_True)
				{
		    		adm_buff[i][0] = ((uint16)rbuf[i][1] << 8u) + (uint16)rbuf[i][0];
		    		adm_buff[i][1] = ((uint16)rbuf[i][3] << 8u) + (uint16)rbuf[i][2];
		    		adm_buff[i][2] = ((uint16)rbuf[i][5] << 8u) + (uint16)rbuf[i][4];	
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
				}
			}

			if(Adbms_PEC_error_count_F == 0u)
			{
		
			for(i = 0u; i < NumOf_Bmic; i++)
            {
				if(NumOfCellinModule >= 9u)
				{
	                for(j = 0u; j < 3u; j++)
	                {
	                    if	   (adm_buff[i][j] == 0x7FA0u)				
						{
							(&adbms_data[i])->OWD[j + 6u] = OWD_DRF;
	                    }
	                    else if(adm_buff[i][j] == 0x7FC0u)   			
						{
							(&adbms_data[i])->OWD[j + 6u] = OWD_CVMIN;
	                    }
	                    else if(adm_buff[i][j] > ADBMS6815_VTH_SPF) 	
						{
							(&adbms_data[i])->OWD[j + 6u] = OWD_OPEN;
	                    }
	                    else if(adm_buff[i][j] < ADBMS6815_VTH_LF)  	
						{
							(&adbms_data[i])->OWD[j + 6u] = OWD_LATENT;
	                    }
	                    else                                        	
						{
							(&adbms_data[i])->OWD[j + 6u] = OWD_NO_ERROR;
	                    }
	                }
				}

				else if((NumOfCellinModule == 7u) || (NumOfCellinModule == 8u))
				{
					for(j = 0u; j < (NumOfCellinModule - 6u); j++)
	                {
	                    if	   (adm_buff[i][j] == 0x7FA0u)				
						{
							(&adbms_data[i])->OWD[j + 6u] = OWD_DRF;
	                    }
	                    else if(adm_buff[i][j] == 0x7FC0u)   			
						{
							(&adbms_data[i])->OWD[j + 6u] = OWD_CVMIN;
	                    }
	                    else if(adm_buff[i][j] > ADBMS6815_VTH_SPF) 	
						{
							(&adbms_data[i])->OWD[j + 6u] = OWD_OPEN;
	                    }
	                    else if(adm_buff[i][j] < ADBMS6815_VTH_LF)  	
						{
							(&adbms_data[i])->OWD[j + 6u] = OWD_LATENT;
	                    }
	                    else                                        	
						{
							(&adbms_data[i])->OWD[j + 6u] = OWD_NO_ERROR;
	                    }
	                }
				}
				else
				{
					
				}
            }

			}
			seq++;
			break;
			
		case 4:
            Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCDD, rbuf, crc_flag);

		    for(i = 0u; i < NumOf_Bmic; i++)
		    {		 
				if(crc_flag[i] == DEF_True)
				{
		    		adm_buff[i][0] = ((uint16)rbuf[i][1] << 8u) + (uint16)rbuf[i][0];
		    		adm_buff[i][1] = ((uint16)rbuf[i][3] << 8u) + (uint16)rbuf[i][2];
		    		adm_buff[i][2] = ((uint16)rbuf[i][5] << 8u) + (uint16)rbuf[i][4];	
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
				}
			}

			if(Adbms_PEC_error_count_F == 0u)
			{

		    for(i = 0u; i < NumOf_Bmic; i++)
            {
				if(NumOfCellinModule == 12u)
				{
	                for(j = 0u; j < 3u; j++)
	                {
	                    if	   (adm_buff[i][j] == 0x7FA0u)				
						{
							(&adbms_data[i])->OWD[j + 9u] = OWD_DRF;
	                    }
	                    else if(adm_buff[i][j] == 0x7FC0u)   			
						{
							(&adbms_data[i])->OWD[j + 9u] = OWD_CVMIN;
	                    }
	                    else if(adm_buff[i][j] > ADBMS6815_VTH_SPF) 	
						{
							(&adbms_data[i])->OWD[j + 9u] = OWD_OPEN;
	                    }
	                    else if(adm_buff[i][j] < ADBMS6815_VTH_LF)  	
						{
							(&adbms_data[i])->OWD[j + 9u] = OWD_LATENT;
	                    }
	                    else                                        	
						{
							(&adbms_data[i])->OWD[j + 9u] = OWD_NO_ERROR;
	                    }
	                }
				}	

				else if((NumOfCellinModule == 10u) || (NumOfCellinModule == 11u))	
				{
	                for(j = 0u; j < (NumOfCellinModule - 9u); j++)
	                {
	                    if	   (adm_buff[i][j] == 0x7FA0u)				
						{
							(&adbms_data[i])->OWD[j + 9u] = OWD_DRF;
	                    }
	                    else if(adm_buff[i][j] == 0x7FC0u)   			
						{
							(&adbms_data[i])->OWD[j + 9u] = OWD_CVMIN;
	                    }
	                    else if(adm_buff[i][j] > ADBMS6815_VTH_SPF) 	
						{
							(&adbms_data[i])->OWD[j + 9u] = OWD_OPEN;
	                    }
	                    else if(adm_buff[i][j] < ADBMS6815_VTH_LF)  	
						{
							(&adbms_data[i])->OWD[j + 9u] = OWD_LATENT;
	                    }
	                    else                                        	
						{
							(&adbms_data[i])->OWD[j + 9u] = OWD_NO_ERROR;
	                    }
	                }
				}
				else
				{
					
				}
            }

			}
			rv = DEF_True;
			seq = 0;
			break;

        default:
            break;
    }            

    return rv;
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS6815 check aux open wire
* @param 	adbms_data - ADBMS6815_Data
* @return 	uint8 - if sequence finished then return true
*/
bool Adbms6815_CheckAuxOpenWire(Adbms6815_Data* adbms_data)
{          
    CLASSB static uint8  seq;
	uint8  i;
	uint8	j;    
	bool  rv = DEF_False;
	CLASSB static bool crc_flag[DEFAULT_NUMBER_OF_BMIC];
    CLASSB static uint8 rbuf[DEFAULT_NUMBER_OF_BMIC][8];
    CLASSB static uint16 adm_buff[DEFAULT_NUMBER_OF_BMIC][3];
	uint16 NumOf_Bmic;
	uint16 NumOfTempinModule;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	NumOfTempinModule = Parameter_Get_NumOfTempinModule();
		
    switch(seq)
    {
        case 0:     /* Send AXOW Command */
			Adbms6815_WriteSingleCmd(ADBMS6815_CMD_ADAX | ADBMS6815_CBD_MD_7K | ADBMS6815_CBD_CHG_ALL); 
			seq++;
            break;
            
        case 1:     /* Read Aux. */
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDAUXA, rbuf, crc_flag);

		    for(i = 0u; i < NumOf_Bmic; i++)
		    {		 
				if(crc_flag[i] == DEF_True)
				{
		    		adm_buff[i][0] = ((uint16)rbuf[i][3] << 8u) + (uint16)rbuf[i][2];
		    		adm_buff[i][1] = ((uint16)rbuf[i][5] << 8u) + (uint16)rbuf[i][4];	
					RLEC_FrontSPI_Error[(i / 2u)] = DEF_False;
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
					RLEC_FrontSPI_Error[(i / 2u)] = DEF_True;
				}
			}

			if(Adbms_PEC_error_count_F == 0u)
			{			
    			for(i = 0u; i < NumOf_Bmic; i++)
                {
    				if(NumOfTempinModule >= 2u)
    				{
    	                for(j = 0u; j < 2u; j++)
    	                {
    	                    if(adm_buff[i][j] >= 0x7450u)   
    						{
    							(&adbms_data[i])->OAXD[j] = DEF_True;
    	                    }
    	                   	else                       							   
    						{
    							(&adbms_data[i])->OAXD[j] = DEF_False;
    	                   	}
    	                }
    				}

    				else
    				{
    					for(j = 0u; j < NumOfTempinModule; j++)
    	                {
    	                    if(adm_buff[i][j] >= 0x7450u)   
    						{
    							(&adbms_data[i])->OAXD[j] = DEF_True;
    	                    }
    	                   	else                       							    
    						{
    							(&adbms_data[i])->OAXD[j] = DEF_False;
    	                   	}
    	                }
    				}
                }			
			}
			seq++;
			break;

		case 2:
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDAUXB, rbuf, crc_flag);

		    for(i = 0u; i < NumOf_Bmic; i++)
		    {		 
				if(crc_flag[i] == DEF_True)
				{
		    		adm_buff[i][0] = ((uint16)rbuf[i][1] << 8u) + (uint16)rbuf[i][0];
		    		adm_buff[i][1] = ((uint16)rbuf[i][3] << 8u) + (uint16)rbuf[i][2];
		    		adm_buff[i][2] = ((uint16)rbuf[i][5] << 8u) + (uint16)rbuf[i][4];	
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
				}
			}

			if(Adbms_PEC_error_count_F == 0u)
			{

			for(i = 0u; i < NumOf_Bmic; i++)
            {
				if(NumOfTempinModule >= 5u)
				{
	                for(j = 0u; j < 3u; j++)
	                {
	                    if(adm_buff[i][j] >= 0x7450u)   
						{
							(&adbms_data[i])->OAXD[j + 2u] = DEF_True;
	                    }
	                   	else                       							  	 	
						{
							(&adbms_data[i])->OAXD[j + 2u] = DEF_False;
	                   	}
	                }
				}

				else if((NumOfTempinModule == 3u) || (NumOfTempinModule == 4u))
				{
					for(j = 0u; j < (NumOfTempinModule - 2u); j++)
	                {
	                    if(adm_buff[i][j] >= 0x7450u)   
						{
							(&adbms_data[i])->OAXD[j + 2u] = DEF_True;
	                    }
	                   	else                       					        		
						{
							(&adbms_data[i])->OAXD[j + 2u] = DEF_False;
	                   	}
	                }
				}
				else
				{
					
				}
            }

			}
			seq++;
			break;

		case 3:
			Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDAUXC, rbuf, crc_flag);

		    for(i = 0u; i < NumOf_Bmic; i++)
		    {		 
				if(crc_flag[i] == DEF_True)
				{
		    		adm_buff[i][0] = ((uint16)rbuf[i][1] << 8) + (uint16)rbuf[i][0];
		    		adm_buff[i][1] = ((uint16)rbuf[i][3] << 8) + (uint16)rbuf[i][2];	
		        }
				
				else 
				{
					Adbms_PEC_error_count_F++;
				}
			}

			if(Adbms_PEC_error_count_F == 0u)
			{
			
            for(i = 0u; i < NumOf_Bmic; i++)
            {
				if(NumOfTempinModule == 7u)
				{
	                for(j = 0u; j < (NumOfTempinModule - 5u); j++)
	                {
	                    if(adm_buff[i][j] >= 0x7450u)   
						{
							(&adbms_data[i])->OAXD[j + 5u] = DEF_True;
	                    }
	                   	else                       							  	   
						{
							(&adbms_data[i])->OAXD[j + 5u] = DEF_False;
	                   	}
	                }
				}

				else if(NumOfTempinModule == 6u)
				{
					for(j = 0u; j < (NumOfTempinModule - 5u); j++)
	                {
	                    if(adm_buff[i][j] >= 0x7450u)   
						{
							(&adbms_data[i])->OAXD[j + 5u] = DEF_True;
	                    }
	                   	else                       					        		
						{
							(&adbms_data[i])->OAXD[j + 5u] = DEF_False;
	                   	}
	                }
				}
				else
				{
					
				}
            }

			}
			rv = DEF_True;
			seq = 0;
			break;
            
        default:    
            break;            
    }            

    return rv;    
}


/*---------------------------------------------------------------------*/
/*
* @brief 	ADBMS6815 check discharge registor 
* @param 	adbms_data - ADBMS6815_Data
* @return 	uint8 - if sequence finished then return true
*/
bool Adbms6815_CheckDischargeResistor(Adbms6815_Data* adbms_data)
{
    CLASSB static uint8 seq;
	CLASSB static uint8 R_Cnt;
    uint8 i;
	CLASSB static uint8 j;    
#ifdef NDEVICE      
    uint16 bal_buf = 0x0041;   
#else
    uint16 bal_buf = 0b000001000001;   
#endif
    float ratioV;
	bool min_check_fail;
	bool max_check_fail;
    bool rv = DEF_False;
	CLASSB static bool crc_flag[DEFAULT_NUMBER_OF_BMIC];
	CLASSB static uint8 wbuf[DEFAULT_NUMBER_OF_BMIC][8];
    CLASSB static uint8 rbuf[DEFAULT_NUMBER_OF_BMIC][8];
    CLASSB static uint16 adm_buff[DEFAULT_NUMBER_OF_BMIC][12];
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

    switch(seq)
    {
        case 0:
            for(i = 0; i < NumOf_Bmic; i++)
            {				
                wbuf[i][0] = (&adbms_data[i])->WriteConfig_CfgB_Reg0;
				wbuf[i][1] = (&adbms_data[i])->WriteConfig_CfgB_Reg1;
				wbuf[i][2] = (&adbms_data[i])->WriteConfig_CfgB_Reg2;
				wbuf[i][3] = (&adbms_data[i])->WriteConfig_CfgB_Reg3;
                wbuf[i][4] = 0;
                wbuf[i][5] = 0;
                
                bal_buf = bal_buf << R_Cnt;
                wbuf[i][4] = (uint8)(bal_buf & 0xFFu);
                wbuf[i][5] = (uint8)(bal_buf >> 8u) & 0x0Fu;
            }
			
			Adbms6815_WriteDataGroup(ADBMS6815_CMD_WRCFGB, wbuf);	
			Adbms6815_WriteSingleCmd(ADBMS6815_CMD_ADSC | ADBMS6815_CBD_MD_7K);
			
			seq++;
            break;

        case 1:  
				
			if(j <= 2u)
			{
				Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCDA, rbuf, crc_flag);

				for(i = 0u; i < NumOf_Bmic; i++)
		   		{		 
					if(crc_flag[i] == DEF_True)
					{
			    		adm_buff[i][j] = ((uint16)rbuf[i][(j * 2u) + 1u] << 8u) + (uint16)rbuf[i][j * 2u];

			        }
					
					else
					{
						Adbms_PEC_error_count_F++;
					}
				}
			}

			else
			{
				Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCDB, rbuf, crc_flag);

				for(i = 0u; i < NumOf_Bmic; i++)
		   		{		 
					if(crc_flag[i] == DEF_True)
					{
			    		adm_buff[i][j] = ((uint16)rbuf[i][((j - 3u) * 2u) + 1u] << 8u) + (uint16)rbuf[i][(j - 3u) * 2u];

			        }
					
					else
					{
						Adbms_PEC_error_count_F++;
					}
				}
			}

			if(Adbms_PEC_error_count_F == 0u)
			{

		    for(i = 0u; i < NumOf_Bmic; i++)
            {
                    if((&adbms_data[i])->CVR[j] > 0u)
                    {
                        ratioV = (float)((float)adm_buff[i][j] / (float)(&adbms_data[i])->CVR[j]);

						min_check_fail = (ratioV < (float)ADBMS6815_MIN_RATIO_V);
						max_check_fail = (ratioV > (float)ADBMS6815_MAX_RATIO_V);

                        if(min_check_fail || max_check_fail)
                        {
                            (&adbms_data[i])->DischargeR_Diag[j] = DEF_True;
                        }
                        else
                        {
                            (&adbms_data[i])->DischargeR_Diag[j] = DEF_False;
                        }
                    }
					else
                    {
                        (&adbms_data[i])->DischargeR_Diag[j] = DEF_True;
                    }
		    }
			}
				
			seq++;
			break;

		case 2:		
			if(j <= 2u)
			{
				Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCDC, rbuf, crc_flag);

				for(i = 0u; i < NumOf_Bmic; i++)
		   		{		 
					if(crc_flag[i] == DEF_True)
					{
			    		adm_buff[i][j] = ((uint16)rbuf[i][(j * 2u) + 1u] << 8u) + (uint16)rbuf[i][j * 2u];

			        }
					
					else
					{
						Adbms_PEC_error_count_F++;
					}
				}
			}

			else
			{
				Adbms6815_ReadDataGroup(ADBMS6815_CMD_RDCDD, rbuf, crc_flag);

				for(i = 0u; i < NumOf_Bmic; i++)
		   		{		 
					if(crc_flag[i] == DEF_True)
					{
			    		adm_buff[i][j] = ((uint16)rbuf[i][((j - 3u) * 2u) + 1u] << 8u) + (uint16)rbuf[i][(j - 3u) * 2u];

			        }
					
					else
					{
						Adbms_PEC_error_count_F++;
					}
				}
			}

			if(Adbms_PEC_error_count_F == 0u)
			{

		    for(i = 0u; i < NumOf_Bmic; i++)
            {
                    if((&adbms_data[i])->CVR[j + 6u] > 0u)
                    {
						ratioV = (float)((float)adm_buff[i][j] / (float)(&adbms_data[i])->CVR[j + 6u]);

                        min_check_fail = (ratioV < (float)ADBMS6815_MIN_RATIO_V);
						max_check_fail = (ratioV > (float)ADBMS6815_MAX_RATIO_V);

                        if(min_check_fail || max_check_fail)
                        {
                            (&adbms_data[i])->DischargeR_Diag[j + 6u] = DEF_True;
                        }
                        else
                        {
                            (&adbms_data[i])->DischargeR_Diag[j + 6u] = DEF_False;
                        }
                    }
                    else
                    {
                        (&adbms_data[i])->DischargeR_Diag[j + 6u] = DEF_True;
                    }
		    }
			}

			
			if(j <= 4u)
			{
				j++;
				R_Cnt++;
				seq = 0u;
			}
			else
			{
				j = 0u;
				R_Cnt = 0u;
				seq = 0u;
				rv = DEF_True;
			}
			
			break;
            
        default :
			break;                    
    }

    return rv;
}



/*---------------------------------------------------------------------*/
/*
 * @brief 	Get Cell balance status       
 * @param 	*GetV - Get data
 * @param 	adbms_data - ADBMS6815_Data
 * @return	BOOL, DEF_True - Balancing on, DEF_False - X
*/
uint8 Adbms6815_GetCellBalanceStatus(uint16* GetV, Adbms6815_Data* adbms_data)
{
    uint8 i;
    uint8 temp[2];
    uint8 rv = 0u;
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
    
    for(i = 0; i < NumOf_Bmic; i++)
    {
        temp[0] = (&adbms_data[i])->ReadConfig_CfgB_Reg5 & 0x0Fu;
        temp[1] = (&adbms_data[i])->ReadConfig_CfgB_Reg4;

        GetV[i] =  ((uint16)temp[0] << 8) + (uint16)temp[1];
        if(GetV[i] != 0u)
        {
            rv = 1u;
        }
    }    

    return rv;
}


/*---------------------------------------------------------------------*/
/*
 * @brief 	Set Cell balance status       
 * @param 	*SetV - Set data
 * @param 	adbms_data - ADBMS6815_Data
 * @param   Fet_Ctrl - Balance Control mode
 * @return	void
*/
void Adbms6815_SetCellBalanceStatus(const uint16* SetV, Adbms6815_Data* adbms_data, Adbms6815_FETCtrl Fet_Ctrl)
{
    uint8 i;
    uint16 temp;
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();

    if(Fet_Ctrl == ALL_OFF)
    {
        for(i = 0u; i < NumOf_Bmic; i++)
        {        
            temp = 0;
            (&adbms_data[i])->WriteConfig_CfgB_Reg5 = (uint8)(temp >> 8u) & 0x0Fu;
            (&adbms_data[i])->WriteConfig_CfgB_Reg4 = (uint8)(temp & 0xFFu);
        }    
    }
    else if(Fet_Ctrl == ODD)
    {
        for(i = 0u; i < NumOf_Bmic; i++)
        {                    
            temp = SetV[i] & 0xAA5u; // except Busbar
            (&adbms_data[i])->WriteConfig_CfgB_Reg5 = (uint8)(temp >> 8u) & 0x0Fu;
            (&adbms_data[i])->WriteConfig_CfgB_Reg4 = (uint8)(temp & 0xFFu);
        }    
    }
    else if(Fet_Ctrl == EVEN)
    {
        for(i = 0u; i < NumOf_Bmic; i++)
        {        
            temp = SetV[i] & 0x54Au; // except Busbar
            (&adbms_data[i])->WriteConfig_CfgB_Reg5 = (uint8)(temp >> 8u) & 0x0Fu;
            (&adbms_data[i])->WriteConfig_CfgB_Reg4 = (uint8)(temp & 0xFFu);
        }    
    }
    else
    {
		
    }
}


/*---------------------------------------------------------------------*/
/*
 * @brief 	Get ADBMS PEC Error Count of front line
 * @param 	void
 * @return	uint16 - PEC Error Count of front line
*/
uint16 Adbms6815_F_Get6815PEC_ErrorCount(void)
{
	return Adbms_PEC_error_count_F;
}


/*---------------------------------------------------------------------*/
/*
 * @brief 	Get ADBMS PEC Error Count of rear line
 * @param 	void
 * @return	uint16 - PEC Error Count of rear line
*/
uint16 Adbms6815_R_Get6815PEC_ErrorCount(void)
{
	return Adbms_PEC_error_count_R;
}


/*---------------------------------------------------------------------*/
/*
 * @brief 	Set ADBMS PEC Error Count of front line
 * @param 	error_cnt - PEC Error Count of front line
 * @return	void
*/
void Adbms6815_F_Set6815PEC_ErrorCount(uint16 error_cnt)
{
	Adbms_PEC_error_count_F = error_cnt;
}


/*---------------------------------------------------------------------*/
/*
 * @brief 	Set ADBMS PEC Error Count of rear line
 * @param 	error_cnt - PEC Error Count of rear line
 * @return	void
*/
void Adbms6815_R_Set6815PEC_ErrorCount(uint16 error_cnt)
{
	Adbms_PEC_error_count_R = error_cnt;
}


/*---------------------------------------------------------------------*/
/*
 * @brief 	Check if there is Front line SPI Error
 * @param 	void
 * @return	uint32 - Front line SPI Error status
*/
uint32 Adbms6815_FrontSPI_Error_Check(void)
{
	uint8 bmic_num;
	uint16 FrontSPI_Error1 = 0u;
	uint16 FrontSPI_Error2 = 0u;
	uint32 FrontSPI_Error_Status;
	uint16 NumOf_Bmic;
	
	
	#ifdef Test_Adbms6815_SPI_Error_Check
	NumOf_Bmic = 14u;
	#else
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	#endif

	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		if((bmic_num % 2u) == 0u)
		{
			if(RLEC_FrontSPI_Error[(bmic_num / 2u)] == 1u)
			{
				if(bmic_num < 32u)
				{
					FrontSPI_Error1 |= ((uint16)1u << ((uint16)bmic_num / 2u));
				}

				else
				{
					FrontSPI_Error2 |= ((uint16)1u << (((uint16)bmic_num - 32u) / 2u));
				}
			}
			else if(RLEC_FrontSPI_Error[(bmic_num / 2u)] == 0u)
			{
				if(bmic_num < 32u)
				{
					FrontSPI_Error1 &= ~((uint16)1u << ((uint16)bmic_num / 2u));
				}

				else
				{
					FrontSPI_Error2 &= ~((uint16)1u << (((uint16)bmic_num - 32u) / 2u));
				}
			}

			else
			{
				
			}
		}
	}

	FrontSPI_Error_Status = (uint32)FrontSPI_Error2 & (uint32)0x00FFu;
	FrontSPI_Error_Status = FrontSPI_Error_Status << (uint32)16u;
	FrontSPI_Error_Status |= (uint32)FrontSPI_Error1;

//	FrontSPI_Error_Status = ((FrontSPI_Error2 & 0x00FF) << (uint32)16) + (FrontSPI_Error1 & 0xFFFF);

	
	return FrontSPI_Error_Status;
}


/*---------------------------------------------------------------------*/
/*
 * @brief 	Check if there is Rear line SPI Error
 * @param 	void
 * @return	uint32 - Rear line SPI Error status
*/
uint32 Adbms6815_RearSPI_Error_Check(void)
{
	uint8 bmic_num;
	uint16 RearSPI_Error1 = 0u;
	uint16 RearSPI_Error2 = 0u;
	uint32 RearSPI_Error_Status;
	uint16 NumOf_Bmic;

	#ifdef Test_Adbms6815_SPI_Error_Check
	NumOf_Bmic = 14u;
	#else
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
	#endif

	for (bmic_num = 0u; bmic_num < NumOf_Bmic; bmic_num++)
	{
		if((bmic_num % 2u) == 0u)
		{
			if(RLEC_RearSPI_Error[((uint16)bmic_num / 2u)] == 1u)
			{
				if(bmic_num < 32u)
				{
					RearSPI_Error1 |= ((uint16)1u << ((uint16)bmic_num / 2u));
				}

				else
				{
					RearSPI_Error2 |= ((uint16)1u << (((uint16)bmic_num - 32u) / 2u));
				}
			}

			else if(RLEC_RearSPI_Error[(bmic_num / 2U)] == 0u)
			{
				if(bmic_num < 32u)
				{
					RearSPI_Error1 &= ~((uint16)1u << ((uint16)bmic_num / 2u));
				}

				else
				{
					RearSPI_Error2 &= ~((uint16)1u << (((uint16)bmic_num - 32u) / 2u));
					
				}
			}
			
			else
			{
				
			}
		}
	}

	RearSPI_Error_Status = (uint32)RearSPI_Error2 & (uint32)0x00FFu;
	RearSPI_Error_Status = RearSPI_Error_Status << (uint32)16u;
	RearSPI_Error_Status |= (uint32)RearSPI_Error1;

//	RearSPI_Error_Status = ((RearSPI_Error2 & 0x00FF) << (uint32)16) + (RearSPI_Error1 & 0xFFFF);

	
	return RearSPI_Error_Status;
}

/*---------------------------------------------------------------------*/
/*
 * @brief 	Check Cell balance Resister       
 * @param 	adbms_data - ADBMS6815_Data
 * @return	BOOL, TRUE - Balancing on, FALSE - X
*/
bool Adbms_CheckCellBalanceR(Adbms6815_Data* adbms_data)
{
    uint8 i;
    uint8 temp[2];
    bool rv = FALSE;
	uint16 	Bal_Status[DEF_MAX_NUM_OF_BMIC];
	uint16 NumOf_Bmic;
	
	NumOf_Bmic = Parameter_Get_NumOf_Bmic();
    
    for(i = 0; i < NumOf_Bmic; i++)
    {
        temp[0] = (&adbms_data[i])->WriteConfig_CfgB_Reg5 & 0x0Fu;
        temp[1] = (&adbms_data[i])->WriteConfig_CfgB_Reg4;

        Bal_Status[i] =  ((uint16)temp[0] << 8) + (uint16)temp[1];
        if(Bal_Status[i] != 0u)
        {
            rv = TRUE;
        }
    }    

    return rv;
}


/*---------------------------------------------------------------------*/

