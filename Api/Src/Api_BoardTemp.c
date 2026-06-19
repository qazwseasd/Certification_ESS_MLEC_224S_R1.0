/*
 * App_MeasTAdcIn.c
 *
 *  Created on: May 27, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include <Api_BoardTemp.h>


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/

#define MeasTAdcIn_TVTable_Size		( (sizeof(MeasTAdcIn_TVTable)) / (sizeof(MeasTAdcIn_TVTable[0])) )
#define	MeasTAdcIn_Ratio_TBoard		( (float)0.000886446886446 )
#define MeasTAdcIn_T_Resolution		( (float)0.1 )

#define BOARDT_NORMAL_FIL	(float)0.9

/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/

typedef struct
{
	float	Temperature;
	float	Vout;
} MeasTAdcIn_TVTable_t;


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/
static sint16 BoardTemp_ADRaw;
static sint16 BoardTemp_FiltedValue;
uint16 AdcIn_Adc3Buff[AdcIn_Adc3Buff_Len];


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/

static	const	MeasTAdcIn_TVTable_t	MeasTAdcIn_TVTable[] =
{
/*	Temp[℃], Vout[V]	*/

	{ -55, 2.4847f },
	{ -50, 2.4292f },
	{ -45, 2.3735f },
	{ -40, 2.3177f },
	{ -35, 2.2617f },
	{ -30, 2.2054f },
	{ -25, 2.1490f },
	{ -20, 2.0923f },
	{ -15, 2.0355f },
	{ -10, 1.9785f },
	{  -5, 1.9214f },
	{   0, 1.8639f },
	{   5, 1.8063f },
	{  10, 1.7485f },
	{  15, 1.6905f },
	{  20, 1.6322f },
	{  25, 1.5740f },
	{  30, 1.5154f },
	{  35, 1.4566f },
	{  40, 1.3977f },
	{  45, 1.3383f },
	{  50, 1.2787f },
	{  55, 1.2197f },
	{  60, 1.1599f },
	{  65, 1.1000f },
	{  70, 1.0399f },
	{  75, 0.9791f },
	{  80, 0.9191f },
	{  85, 0.8592f },
	{  90, 0.7975f },
	{  95, 0.7364f },
	{ 100, 0.6746f },
	{ 105, 0.6136f },
	{ 110, 0.5520f },
	{ 115, 0.4901f },
	{ 120, 0.4280f },
	{ 125, 0.3653f },
	{ 130, 0.3030f },
};

static	sint16	T_BoardMlec;
/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/
/**
* @brief	Save SD Card Task
* @param	void
* @return	void
*/
void 	MeasTAdcIn_Task(void)
{
    static bool BoardTemp_MeasStart = FALSE;
	float temp_result;
    
	MeasTAdcIn_Meas_TMbms();
	if(BoardTemp_MeasStart == FALSE)
	{
	    BoardTemp_MeasStart = TRUE;
	    BoardTemp_FiltedValue = BoardTemp_ADRaw;
	}
	
    temp_result = FILTER(BoardTemp_FiltedValue, BoardTemp_ADRaw, BOARDT_NORMAL_FIL);
	BoardTemp_FiltedValue = (sint16)temp_result;

    T_BoardMlec = BoardTemp_FiltedValue;

}


/**
* @brief	Measure and process the value
* @param	void
* @return	void
*/

void	MeasTAdcIn_Meas_TMbms(void)
{
	uint8	idx;

	float	Vout;
	float	Vhigh, Vlow;
	float	Thigh, Tlow;
	float	TBoard = 0.0f;
	float	Ratio;

    /* Modif By SKAHN _250909_SAT */
	//Vout = (float)AdcIn_Get_Adc3BuffValue(DRV_AdcIn_Adc3Ch_TBoard) * MeasTAdcIn_Ratio_TBoard;
    AdcSmState AdcState = Api_ADC3_Read_SingleChannel();

    switch(AdcState)
    {
        case ADC_SM_DONE:
            Vout = (float)AdcIn_Adc3Buff[DRV_AdcIn_Adc3Ch_TBoard] * MeasTAdcIn_Ratio_TBoard;
            
            if (Vout >= MeasTAdcIn_TVTable[0].Vout) 
			{
				BoardTemp_ADRaw = -550;
				return;
			}

			if (Vout <= MeasTAdcIn_TVTable[MeasTAdcIn_TVTable_Size - 1u].Vout)
			{
				BoardTemp_ADRaw = -1300;
				return;
			}

			for (idx = 0u; idx < (MeasTAdcIn_TVTable_Size - 1u); idx++)
			{
				Vhigh	= MeasTAdcIn_TVTable[(idx)].Vout;
				Vlow	= MeasTAdcIn_TVTable[(idx + 1u)].Vout;

				if ((Vout >= Vlow) && (Vout <= Vhigh))
				{
					Thigh	= MeasTAdcIn_TVTable[(idx)].Temperature / MeasTAdcIn_T_Resolution;
					Tlow	= MeasTAdcIn_TVTable[(idx + 1u)].Temperature / MeasTAdcIn_T_Resolution;
					Ratio	= ((Vout - Vlow) / (Vhigh - Vlow));
					TBoard	= Tlow + (Ratio * (Thigh - Tlow));
				}
			}
			BoardTemp_ADRaw = (sint16)TBoard;
			break;
        case ADC_SM_ERROR:
            break;
        default: 
            break;
    }

    /* Modif By SKAHN _250909_END */

}


/*---------------------------------------------------------------------*/
sint16 BoardTemp_Get_T_BoardMlec(void)
{
    return T_BoardMlec;
}


