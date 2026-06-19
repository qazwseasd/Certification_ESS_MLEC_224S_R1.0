/*
 * App_Warranty.c
 *
 *  Created on: Sep 23, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Warranty.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/
#define MINUTE_lmt          60u                     ///< 1 minute count threshold
#define CHARGE_I_lmt        10u                     ///< Charging current judgment criteria
#define DISCHARGE_I_lmt     (sint16)-10                ///< Discharging current judgment criteria
#define LOWTEMP_lmt         (sint16)-100               ///< Low temperature judgment criteria
#define EXT_LOWTEMP_lmt     (sint16)-150               ///< Extreme low temperature judgment criteria
#define HIGHTEMP_lmt        (sint16)450                ///< High temperature judgment criteria
#define EXT_HIGHTEMP_lmt    (sint16)500                ///< Extreme high temperature judgment criteria


/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/
typedef struct
{
    uint32     Total_OperateTime_Min;                  /**< Store total operation time in minutes */
    uint32     Last_OperateTime_Min;                   /**< Store last operation time in minutes */
    uint32     Longest_OperateTime_Min;                /**< Store longest operation time in minutes */

    uint32     Total_ChargeTime_Min;                   /**< Store total charge time in minutes */
    uint32     Total_ChargePower_kWh;                  /**< Store total charge power in kWh */
    uint32     Total_ChargeCurrent_Ah;                 /**< Store total charge current in Ah */
    sint16     Max_ChargeCurrent_A;                    /**< Store max. charge current in A */
    uint16     ChargeCount_Full;                       /**< Store full charge count */
    uint16     ChargeTime_LowTemp_Min;                 /**< Store charge time at low temperature in minutes */
    uint16     ChargeTime_HighTemp_Min;                /**< Store charge time at High temperature in minutes */
    uint16     ChargeTime_ExtremLowTemp_Min;           /**< Store charge time at extreme low temperature in minutes */
    uint16     ChargeTime_ExtremHighTemp_Min;          /**< Store charge time at extreme high temperature in minutes */

    uint32     Total_DischargeTime_Min;                /**< Store total discharge time in minutes */
    uint32     Total_DischargePower_kWh;               /**< Store total discharge power in kWh */
    uint32     Total_DischargeCurrent_Ah;              /**< Store total discharge current in Ah */
    sint16     Max_DischargeCurrent_A;                 /**< Store max. discharge current in A */
    uint16     DischargeCount_Empty;                   /**< Store full discharge count */
    uint16     DischargeTime_LowTemp_Min;              /**< Store discharge time at low temperature in minutes */
    uint16     DischargeTime_HighTemp_Min;             /**< Store discharge time at High temperature in minutes */
    uint16     DischargeTime_ExtremLowTemp_Min;        /**< Store discharge time at extreme low temperature in minutes */
    uint16     DischargeTime_ExtremHighTemp_Min;       /**< Store discharge time at extreme high temperature in minutes */

    sint16     MaxCellTemp;                            /**< Store max. cell temperatarue */
    sint16     MinCellTemp;                            /**< Store min cell temperatarue */
    uint16     MaxCellVoltage;                         /**< Store max. cell voltage */
    uint16     MinCellVoltage;                         /**< Store min. cell voltage */
    uint16     MaxPackVoltage;                         /**< Store max. pack voltage */
    uint16     MinPackVoltage;                         /**< Store min. pack voltage */

    uint32     Total_BalanceTime_Min;                  /**< Store total cell balance time in minutes */
    uint32     Longest_BalanceTime_Min;                /**< Store longest cell balance time in minutes */
    uint32     Total_CoolerTime_Min;                   /**< Store total cooler operation time in minutes */
    uint32     Longest_CoolerTime_Min;                 /**< Store longest cooler operation time in minutes */
    uint16     ContactorOnCount;                       /**< Store Contactor ON Count */
    uint16     ContactorOffCount_UnderLoad;            /**< Store Contactor Off Count under load */

	uint16     dummy;
    uint16     ArrCRC;                                    /**< Store CRC Value */
}WarrantyData;



/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/
static WarrantyData     Warranty;                   ///< Store warranty information 
static WarrantyData     Warranty_temp;              ///< Saving warranty information received via CAN communication

static bool             bWarrantyDataSaveFlag; 


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
void Default_WarrantyData(void);

/*---------------------------------------------------------------------*/
/**
* @brief    Warranty Initialization Function
*           This function loads the existing Warranty value stored in EEPROM.
* @param    void
* @return
*/
void Warranty_Init(void)
{
    WarrantyData_Load();
	HAL_Delay(5u);
    (void)memcpy(&Warranty_temp, &Warranty, sizeof(WarrantyData));
    bWarrantyDataSaveFlag = FALSE;
}

/*---------------------------------------------------------------------*/
/**
* @brief    Warranty Task function,
*           Called every second to update warranty-related data.
* @param    void
* @return
*/
void Warranty_Task(void)
{
//#define SAVE_WARRANTY_INTERVAL              10u             // 10 minutes
    static uint32  ThisCycle_OperateTime_Min   = 0u;
    static uint8   OpTime_Sec                  = 0u;
    static uint8   ChargeTime_Sec              = 0u;
    static uint8   DischargeTime_Sec           = 0u;
    static uint8   ChargeTimeLowTemp_Sec       = 0u;
    static uint8   DischargeTimeLowTemp_Sec    = 0u;
    static uint8   ChargeTimeExtLowTemp_Sec    = 0u;
    static uint8   DischargeTimeExtLowTemp_Sec = 0u;
    static uint8   ChargeTimeHighTemp_Sec      = 0u;
    static uint8   DischargeTimeHighTemp_Sec   = 0u;
    static uint8   ChargeTimeExtHighTemp_Sec   = 0u;
    static uint8   DischargeTimeExtHighTemp_Sec= 0u;
    static uint8   Balance_OpTime_Sec          = 0u;
    static uint16  Balance_OpTime_Min          = 0u;
    static uint8   FAN_OpTime_Sec              = 0u;
    static uint16  FAN_OpTime_Min              = 0u;
	static uint8	MaxMin_Count				= 0u;
//    static uint8   SaveIntervalCount           = 0u;
    static bool DetectFullCharge_1          = FALSE;
	static bool DetectFullCharge_2			= FALSE;
	static bool DetectFullDischarge_1       = FALSE;
    static bool DetectFullDischarge_2       = FALSE;
    static bool ContOnState                 = FALSE;

	uint16	V_CellMax;
	uint16	V_CellMin;
	sint16  T_CellMax;
	sint16  T_CellMin;
	SYSTEM_STATE System_State;
	uint16	SOC_User;

	V_CellMax = Adbms_Get_V_CellMax();
	V_CellMin = Adbms_Get_V_CellMin();
	T_CellMax = Adbms_Get_T_CellMax();
	T_CellMin = Adbms_Get_T_CellMin();
	System_State = ManaMsm_Get_System_State();
	SOC_User = SOC_Get_SOC_User();

    sint16 MaxCurr;

    if((System_State != SYS_CALIBRATION) &&
       (System_State != SYS_CONFIGURATION) &&
       (System_State != SYS_LOGREAD)
    )
    {
        /* Charge time calculation */
        if(Curr_Get_I_PackMeas() >= (sint16)CHARGE_I_lmt)
        {
            if(ChargeTime_Sec >= (MINUTE_lmt - 1u))
            {
                ChargeTime_Sec = 0u;
                Warranty.Total_ChargeTime_Min++;
                Warranty.Total_ChargePower_kWh = SOC_Get_Chg_kWh();
                Warranty.Total_ChargeCurrent_Ah = SOC_Get_Accum_ChargeAmps();
            }
            else
            {
                ChargeTime_Sec++;
            }

            MaxCurr = Curr_GetChargeMax();
            if(MaxCurr > Warranty.Max_ChargeCurrent_A)
            {
                Warranty.Max_ChargeCurrent_A = MaxCurr;
            }

            if(T_CellMin < LOWTEMP_lmt)
            {
                if(ChargeTimeLowTemp_Sec >= (MINUTE_lmt - 1u))
                {
                    ChargeTimeLowTemp_Sec = 0u;
                    Warranty.ChargeTime_LowTemp_Min++;
                }
                else
                {
                    ChargeTimeLowTemp_Sec++;
                }

                if(T_CellMin < EXT_LOWTEMP_lmt)
                {
                    if(ChargeTimeExtLowTemp_Sec >= (MINUTE_lmt - 1u))
                    {
                        ChargeTimeExtLowTemp_Sec = 0u;
                        Warranty.ChargeTime_ExtremLowTemp_Min++;
                    }
                    else
                    {
                        ChargeTimeExtLowTemp_Sec++;
                    }
                }
            }

            if(T_CellMax > HIGHTEMP_lmt)
            {
                if(ChargeTimeHighTemp_Sec >= (MINUTE_lmt - 1u))
                {
                    ChargeTimeHighTemp_Sec = 0u;
                    Warranty.ChargeTime_HighTemp_Min++;
                }
                else
                {
                    ChargeTimeHighTemp_Sec++;
                }

                if(T_CellMax > EXT_HIGHTEMP_lmt)
                {
                    if(ChargeTimeExtHighTemp_Sec >= (MINUTE_lmt - 1u))
                    {
                        ChargeTimeExtHighTemp_Sec = 0u;
                        Warranty.ChargeTime_ExtremHighTemp_Min++;
                    }
                    else
                    {
                        ChargeTimeExtHighTemp_Sec++;
                    }
                }
            }

        }

        if(DetectFullCharge_1 == FALSE)
        {
			if((SOC_User >= 970u) && (SOC_User <= 989u))
			{
				DetectFullCharge_2 = TRUE;
			}

            if((SOC_User >= 990u) && (DetectFullCharge_2 == TRUE))
            {
                DetectFullCharge_1 = TRUE;
				DetectFullCharge_2 = FALSE;
                Warranty.ChargeCount_Full++;
            }
        }
        else
        {
            if(SOC_User <= 950u)
            {
                DetectFullCharge_1 = FALSE;
            }
        }

        /* Discharge time calculation */
        if(Curr_Get_I_PackMeas() <= (sint16)DISCHARGE_I_lmt)
        {
            if(DischargeTime_Sec >= (MINUTE_lmt - 1u))
            {
                DischargeTime_Sec = 0u;
                Warranty.Total_DischargeTime_Min++;
                Warranty.Total_DischargePower_kWh = SOC_Get_Dch_kWh();
                Warranty.Total_DischargeCurrent_Ah = SOC_Get_Accum_DischargeAmps();
            }
            else
            {
                DischargeTime_Sec++;
            }

            MaxCurr = Curr_GetDischargeMax();
            if(MaxCurr > Warranty.Max_DischargeCurrent_A)
            {
                Warranty.Max_DischargeCurrent_A = MaxCurr;
            }

            if(T_CellMin < LOWTEMP_lmt)
            {
                if(DischargeTimeLowTemp_Sec >= (MINUTE_lmt - 1u))
                {
                    DischargeTimeLowTemp_Sec = 0u;
                    Warranty.DischargeTime_LowTemp_Min++;
                }
                else
                {
                    DischargeTimeLowTemp_Sec++;
                }

                if(T_CellMin < EXT_LOWTEMP_lmt)
                {
                    if(DischargeTimeExtLowTemp_Sec >= (MINUTE_lmt - 1u))
                    {
                        DischargeTimeExtLowTemp_Sec = 0u;
                        Warranty.DischargeTime_ExtremLowTemp_Min++;
                    }
                    else
                    {
                        DischargeTimeExtLowTemp_Sec++;
                    }
                }
            }

            if(T_CellMax > HIGHTEMP_lmt)
            {
                if(DischargeTimeHighTemp_Sec >= (MINUTE_lmt - 1u))
                {
                    DischargeTimeHighTemp_Sec = 0u;
                    Warranty.DischargeTime_HighTemp_Min++;
                }
                else
                {
                    DischargeTimeHighTemp_Sec++;
                }

                if(T_CellMax > EXT_HIGHTEMP_lmt)
                {
                    if(DischargeTimeExtHighTemp_Sec >= (MINUTE_lmt - 1u))
                    {
                        DischargeTimeExtHighTemp_Sec = 0u;
                        Warranty.DischargeTime_ExtremHighTemp_Min++;
                    }
                    else
                    {
                        DischargeTimeExtHighTemp_Sec++;
                    }
                }
            }

        }

        if(DetectFullDischarge_1 == FALSE)
        {
			if((SOC_User <= 30u) && (SOC_User >= 11u))
			{
				DetectFullDischarge_2 = TRUE;
			}

            if((ManaMsm_Get_System_State() != SYS_INIT) && (SOC_User <= 10u) && (DetectFullDischarge_2 == TRUE))
            {
                DetectFullDischarge_1 = TRUE;
				DetectFullDischarge_2 = FALSE;
                Warranty.DischargeCount_Empty++;
            }
        }
        else
        {
            if(SOC_User >= 50u)
            {
                DetectFullDischarge_1 = FALSE;
            }
        }

       if(MaxMin_Count >= 10u)
		{
	        if(T_CellMax > Warranty.MaxCellTemp)
	        {
	            Warranty.MaxCellTemp = T_CellMax;
	        }

	        if(T_CellMin < Warranty.MinCellTemp)
	        {
	            Warranty.MinCellTemp = T_CellMin;
	        }

	        if(V_CellMax > Warranty.MaxCellVoltage)
	        {
	            Warranty.MaxCellVoltage = V_CellMax;
	        }

	        if(V_CellMin < Warranty.MinCellVoltage)
	        {
	            Warranty.MinCellVoltage = V_CellMin;
	        }

	        if(PackVolt_Get_V_PackMeas() > Warranty.MaxPackVoltage)
	        {
	            Warranty.MaxPackVoltage = PackVolt_Get_V_PackMeas();
	        }

	        if(PackVolt_Get_V_PackMeas() < Warranty.MinPackVoltage)
	        {
	            Warranty.MinPackVoltage = PackVolt_Get_V_PackMeas();
	        }
		}

		else
		{
			MaxMin_Count++;
		}

        if(CellBalance_Get_NumberOfBalancingCell() != 0u)
        {
            if(Balance_OpTime_Sec >= (MINUTE_lmt - 1u))
            {
                Balance_OpTime_Sec = 0u;
                Warranty.Total_BalanceTime_Min++;
                Balance_OpTime_Min++;

                if(Balance_OpTime_Min > Warranty.Longest_BalanceTime_Min)
                {
                    Warranty.Longest_BalanceTime_Min = Balance_OpTime_Min;
                }
            }
            else
            {
                Balance_OpTime_Sec++;
            }
        }
        else
        {
            Balance_OpTime_Sec = 0u;
            Balance_OpTime_Min = 0u;
        }

        if(AppFAN_Get_FAN_State() == 1u)
        {
            if(FAN_OpTime_Sec >= (MINUTE_lmt - 1u))
            {
                FAN_OpTime_Sec = 0u;
                Warranty.Total_CoolerTime_Min++;
                FAN_OpTime_Min++;

                if(FAN_OpTime_Min > Warranty.Longest_CoolerTime_Min)
                {
                    Warranty.Longest_CoolerTime_Min = FAN_OpTime_Min;
                }
            }
            else
            {
                FAN_OpTime_Sec++;
            }
        }
        else
        {
            FAN_OpTime_Sec = 0u;
            FAN_OpTime_Min = 0u;
        }

        uint8 RelayState = Relay_GetAllRelay();
        if(ContOnState == FALSE)
        {
            if(RelayState == (RELAY_NEG_FLAG + RELAY_POS_FLAG))
            {
                ContOnState = TRUE;
                Warranty.ContactorOnCount++;
            }
        }
        else
        {
            if(RelayState == RELAY_ALL_OFF)
            {
                ContOnState = FALSE;
            }
        }
        Warranty.ContactorOffCount_UnderLoad = SOC_Get_ContOpenCount_UnderLoad();
		

        /* Operation time calculation */
        if(OpTime_Sec >= (MINUTE_lmt - 1u))
        {
            OpTime_Sec = 0u;
            ThisCycle_OperateTime_Min++;
            Warranty.Total_OperateTime_Min++;
			Warranty.Last_OperateTime_Min = ThisCycle_OperateTime_Min;
            if(ThisCycle_OperateTime_Min >= Warranty.Longest_OperateTime_Min)
            {
                Warranty.Longest_OperateTime_Min = ThisCycle_OperateTime_Min;
            }
#if 0
            if(SaveIntervalCount >= (SAVE_WARRANTY_INTERVAL - 1u))
            {
                SaveIntervalCount = 0u;
                Set_SaveWarrantyData(TRUE);
            }
            else
            {
                SaveIntervalCount++;
            }
#endif
        }
        else
        {
            OpTime_Sec++;
        }
    }
}

/*---------------------------------------------------------------------*/
/**
* @brief    Function to load warranty data stored in eeprom
* @param    void
* @return   void
*/
void WarrantyData_Load(void)
{
#define RetryLmt        5u
    uint8 rSize = (uint8)sizeof(WarrantyData);
    uint8 rbuf[sizeof(WarrantyData)];
    uint8 i;
    bool UpFlag = FALSE;
	uint16	u16data;

    for(i = 0u; i < RetryLmt; i++)
    {
        (void)memset(rbuf, 0x00, sizeof(rbuf));

        /* Load from eeprom */
        (void)EepromEx_ReadData(WARRANTY_EEP_ADDR, rSize, rbuf);

        /* Check to CRC */
        if(Crc_Check(rbuf, rSize) == TRUE)
        {
            /* Data Save */
            UpFlag = TRUE;
            break;
        }
    }

    if(UpFlag == FALSE)
    {
        /* Save Default Value */
        Default_WarrantyData();
        WarrantyData_Save();
    }
    else
    {
        /* Apply Warranty data from eeprom data */
        Warranty.Total_OperateTime_Min              = ((uint32)rbuf[3] << 24u) + ((uint32)rbuf[2] << 16u) + ((uint32)rbuf[1] << 8u) + (uint32)rbuf[0];
        Warranty.Last_OperateTime_Min               = ((uint32)rbuf[7] << 24u) + ((uint32)rbuf[6] << 16u) + ((uint32)rbuf[5] << 8u) + (uint32)rbuf[4];
        Warranty.Longest_OperateTime_Min            = ((uint32)rbuf[11] << 24u) + ((uint32)rbuf[10] << 16u) + ((uint32)rbuf[9] << 8u) + (uint32)rbuf[8];

        Warranty.Total_ChargeTime_Min               = ((uint32)rbuf[15] << 24u) + ((uint32)rbuf[14] << 16u) + ((uint32)rbuf[13] << 8u) + (uint32)rbuf[12];
        Warranty.Total_ChargePower_kWh              = ((uint32)rbuf[19] << 24u) + ((uint32)rbuf[18] << 16u) + ((uint32)rbuf[17] << 8u) + (uint32)rbuf[16];
        Warranty.Total_ChargeCurrent_Ah             = ((uint32)rbuf[23] << 24u) + ((uint32)rbuf[22] << 16u) + ((uint32)rbuf[21] << 8u) + (uint32)rbuf[20];
		u16data	= ((uint16)rbuf[25] << 8u) + (uint16)rbuf[24];
		Warranty.Max_ChargeCurrent_A                = (sint16)u16data;
        Warranty.ChargeCount_Full                   = ((uint16)rbuf[27] << 8u) + (uint16)rbuf[26];
        Warranty.ChargeTime_LowTemp_Min             = ((uint16)rbuf[29] << 8u) + (uint16)rbuf[28];
        Warranty.ChargeTime_HighTemp_Min            = ((uint16)rbuf[31] << 8u) + (uint16)rbuf[30];
        Warranty.ChargeTime_ExtremLowTemp_Min       = ((uint16)rbuf[33] << 8u) + (uint16)rbuf[32];
        Warranty.ChargeTime_ExtremHighTemp_Min      = ((uint16)rbuf[35] << 8u) + (uint16)rbuf[34];

        Warranty.Total_DischargeTime_Min            = ((uint32)rbuf[39] << 24u) + ((uint32)rbuf[38] << 16u) + ((uint32)rbuf[37] << 8u) + (uint32)rbuf[36];
        Warranty.Total_DischargePower_kWh           = ((uint32)rbuf[43] << 24u) + ((uint32)rbuf[42] << 16u) + ((uint32)rbuf[41] << 8u) + (uint32)rbuf[40];
        Warranty.Total_DischargeCurrent_Ah          = ((uint32)rbuf[47] << 24u) + ((uint32)rbuf[46] << 16u) + ((uint32)rbuf[45] << 8u) + (uint32)rbuf[44];
		u16data = ((uint16)rbuf[49] << 8u) + (uint16)rbuf[48]; 
		Warranty.Max_DischargeCurrent_A             = (sint16)u16data;
        Warranty.DischargeCount_Empty               = ((uint16)rbuf[51] << 8u) + (uint16)rbuf[50];
        Warranty.DischargeTime_LowTemp_Min          = ((uint16)rbuf[53] << 8u) + (uint16)rbuf[52];
        Warranty.DischargeTime_HighTemp_Min         = ((uint16)rbuf[55] << 8u) + (uint16)rbuf[54];
        Warranty.DischargeTime_ExtremLowTemp_Min    = ((uint16)rbuf[57] << 8u) + (uint16)rbuf[56];
        Warranty.DischargeTime_ExtremHighTemp_Min   = ((uint16)rbuf[59] << 8u) + (uint16)rbuf[58];

		u16data = ((uint16)rbuf[61] << 8u) + (uint16)rbuf[60];
        Warranty.MaxCellTemp                        = (sint16)u16data;
		u16data = ((uint16)rbuf[63] << 8u) + (uint16)rbuf[62];
        Warranty.MinCellTemp                        = (sint16)u16data;
        Warranty.MaxCellVoltage                     = ((uint16)rbuf[65] << 8u) + (uint16)rbuf[64];
        Warranty.MinCellVoltage                     = ((uint16)rbuf[67] << 8u) + (uint16)rbuf[66];
        Warranty.MaxPackVoltage                     = ((uint16)rbuf[69] << 8u) + (uint16)rbuf[68];
        Warranty.MinPackVoltage                     = ((uint16)rbuf[71] << 8u) + (uint16)rbuf[70];

        Warranty.Total_BalanceTime_Min              = ((uint32)rbuf[75] << 24u) + ((uint32)rbuf[74] << 16u) + ((uint32)rbuf[73] << 8u) + (uint32)rbuf[72];
        Warranty.Longest_BalanceTime_Min            = ((uint32)rbuf[79] << 24u) + ((uint32)rbuf[78] << 16u) + ((uint32)rbuf[77] << 8u) + (uint32)rbuf[76];
        Warranty.Total_CoolerTime_Min               = ((uint32)rbuf[83] << 24u) + ((uint32)rbuf[82] << 16u) + ((uint32)rbuf[81] << 8u) + (uint32)rbuf[80];
        Warranty.Longest_CoolerTime_Min             = ((uint32)rbuf[87] << 24u) + ((uint32)rbuf[86] << 16u) + ((uint32)rbuf[85] << 8u) + (uint32)rbuf[84];
        Warranty.ContactorOnCount                   = ((uint16)rbuf[89] << 8u) + (uint16)rbuf[88];
        Warranty.ContactorOffCount_UnderLoad        = ((uint16)rbuf[91] << 8u) + (uint16)rbuf[90];
    }
}

/*---------------------------------------------------------------------*/
/**
* @brief Function to store warranty data in eeprom
* @param void
* @return
*/
void WarrantyData_Save(void)
{
    uint8 crc[2];

    Crc_Calculation(crc, (uint8*)&Warranty, (uint16)sizeof(WarrantyData) - 2u);
    Warranty.ArrCRC = ((uint16)crc[1] << 8u) + (uint16)crc[0];

    (void)EepromEx_WriteData(WARRANTY_EEP_ADDR, sizeof(WarrantyData), (uint8*)&Warranty);

    (void)memcpy(&Warranty_temp, &Warranty, sizeof(WarrantyData));
}

/*---------------------------------------------------------------------*/
/**
* @brief Default warranty data.
* @param void
* @return
*/
void Default_WarrantyData(void)
{
    Warranty.Total_OperateTime_Min              = 0u;
    Warranty.Last_OperateTime_Min               = 0u;
    Warranty.Longest_OperateTime_Min            = 0u;

    Warranty.Total_ChargeTime_Min               = 0u;
    Warranty.Total_ChargePower_kWh              = 0u;
    Warranty.Total_ChargeCurrent_Ah             = 0u;
    Warranty.Max_ChargeCurrent_A                = 0;
    Warranty.ChargeCount_Full                   = 0u;
    Warranty.ChargeTime_LowTemp_Min             = 0u;
    Warranty.ChargeTime_HighTemp_Min            = 0u;
    Warranty.ChargeTime_ExtremLowTemp_Min       = 0u;
    Warranty.ChargeTime_ExtremHighTemp_Min      = 0u;

    Warranty.Total_DischargeTime_Min            = 0u;
    Warranty.Total_DischargePower_kWh           = 0u;
    Warranty.Total_DischargeCurrent_Ah          = 0u;
    Warranty.Max_DischargeCurrent_A             = 0;
    Warranty.DischargeCount_Empty               = 0u;
    Warranty.DischargeTime_LowTemp_Min          = 0u;
    Warranty.DischargeTime_HighTemp_Min         = 0u;
    Warranty.DischargeTime_ExtremLowTemp_Min    = 0u;
    Warranty.DischargeTime_ExtremHighTemp_Min   = 0u;

    Warranty.MaxCellTemp                        = (sint16)0;
    Warranty.MinCellTemp                        = (sint16)1250;
    Warranty.MaxCellVoltage                     = 0u;
    Warranty.MinCellVoltage                     = 5000u;
    Warranty.MaxPackVoltage                     = 0u;
    Warranty.MinPackVoltage                     = 20000u;

    Warranty.Total_BalanceTime_Min              = 0u;
    Warranty.Longest_BalanceTime_Min            = 0u;
    Warranty.Total_CoolerTime_Min               = 0u;
    Warranty.Longest_CoolerTime_Min             = 0u;
    Warranty.ContactorOnCount                   = 0u;
    Warranty.ContactorOffCount_UnderLoad        = 0u;

//    WarrantyData_Save();
}

/*---------------------------------------------------------------------*/
/**
* @brief Function to check if there is a change in the Warranty value
         when the Warranty value is changed through CAN communication
* @param void
* @return BOOL TRUE : Changed, FALSE : Not Changed
*/
BOOL IsWarrantyDataChanged(void)
{
    BOOL rv = FALSE;

    if(memncmp((uint8*)&Warranty_temp, (uint8*)&Warranty, 0u, (uint16)sizeof(WarrantyData)) == FALSE)
    {
        (void)memcpy((uint8*)&Warranty, (uint8*)&Warranty_temp, sizeof(WarrantyData));
        rv = TRUE;
    }

    return rv;
}

/*---------------------------------------------------------------------*/
/**
* @brief Warranty structure variable storage address call function
* @param void
* @return char* Address of warranty structure variable
*/
char* Get_WarrantyAddress(void)
{
    return (char*)&Warranty;
}

/*---------------------------------------------------------------------*/
/**
* @brief Warranty temp structure variable storage address call function
* @param void
* @return char* Address of warranty temp structure variable
*/
char* Get_WarrantyTempAddress(void)
{
    return (char*)&Warranty_temp;
}

/*---------------------------------------------------------------------*/
/**
* @brief Get warranty save flag function
* @param void
* @return BOOL bWarrantyDataSaveFlag value
*/
BOOL Get_IsSaveWarrantyData(void)
{
    return bWarrantyDataSaveFlag;
}

/*---------------------------------------------------------------------*/
/**
* @brief Set warranty save flag function
* @param BOOL Set the bWarrantyDataSaveFlag
* @return
*/
void Set_SaveWarrantyData(BOOL SetV)
{
    bWarrantyDataSaveFlag = SetV;
}



