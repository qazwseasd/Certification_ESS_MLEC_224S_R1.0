/*
 * Api_Task.c
 *
 *  Created on: Aug 8, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Api_Task.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/

#define Task_MaxNum		32u
#define Task_LogNum		256u



/*****************************************************************************/
/*  3. Local Typedefs                                                        */
/*****************************************************************************/

typedef struct
{
	Task_Func_t	Func;
	uint32		Period_ms;
	uint32		Last_RunTime;
} Task_Info_t;


/*****************************************************************************/
/*  4. Declarations of Imported Variables                                    */
/*****************************************************************************/


/*****************************************************************************/
/*  5. Definitions of Global Variables                                       */
/*****************************************************************************/


/*****************************************************************************/
/*  6. Definitions and Declarations of Local Variables                       */
/*****************************************************************************/

CLASSB static	Task_Info_t	Task_Fifo[Task_MaxNum];

//	static	uint8		Task_Log_Idx[Task_LogNum];
//	static	uint32		Task_Log_RunTime[Task_LogNum];

CLASSB static	uint8		Task_Cnt;
//	static	uint8		Task_Log_Cnt	= 0u;


/*****************************************************************************/
/*  7. Prototypes of Imported Functions                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  8. Prototypes of Local Functions                                         */
/*****************************************************************************/


/*****************************************************************************/
/*  9(Last). Definitions of Functions                                        */
/*****************************************************************************/

void	Task_RunScheduler(void)
{
	uint32		Task_Scheduler_NowTime;
	uint8		taskIdx;
	uint32		elapsed;

	uint32		startCycle;
	uint32		endCycle;
	uint32		duration_us;
	uint32		total_duration_us		= 0u;
//	static 		uint32		test = 0u;

	/* Run only when system tick has advanced */
	if (Tick_IsChange() == DEF_False)
	{
		return;
	}

	Task_Scheduler_NowTime = Tick_Get();

	/* Loop through all registered tasks */
	for (taskIdx = 0u; taskIdx < Task_Cnt; taskIdx++)
	{
		duration_us = 0u;
		elapsed = Task_Scheduler_NowTime - Task_Fifo[taskIdx].Last_RunTime;

		if ((Task_Fifo[taskIdx].Period_ms == 10u) || (elapsed >= Task_Fifo[taskIdx].Period_ms))
		{
			if (Task_Fifo[taskIdx].Func != NULL)
			{
				startCycle 	= Dwt_GetCycle();
				Task_Fifo[taskIdx].Func();
				Task_Fifo[taskIdx].Last_RunTime = Task_Scheduler_NowTime;
				endCycle	= Dwt_GetCycle();
				duration_us	= Dwt_Cycles_To_us(endCycle - startCycle);
			}
		}

		total_duration_us += duration_us;
		/* Logging execution result */
/*
		Task_Log_Idx[Task_Log_Cnt] 		= taskIdx;
		Task_Log_RunTime[Task_Log_Cnt]	= total_duration_us;
		Task_Log_Cnt++;
		if (Task_Log_Cnt >= Task_LogNum)
		{
			Task_Log_Cnt = 0u;
		}
		else{}
*/
/*
		if(test <= total_duration_us)
		{
			test = total_duration_us;
			Main_PackData.test1 = (uint8)(total_duration_us >> 24u);
			Main_PackData.test2 = (uint8)(total_duration_us >> 16u);
			Main_PackData.test3 = (uint8)(total_duration_us >> 8u);
			Main_PackData.test4 = (uint8)(total_duration_us);
		}

		if (total_duration_us > 8000u)
		{
			Main_PackData.test8++;
			return;
		}
*/
	}
}



/**
 * @brief  Register a new task in the scheduler.
 *
 * This function adds a task into the `Task_Fifo` table with the given
 * function pointer, execution period, and priority.
 *
 * @param  Func      Pointer to the task function.
 * @param  period    Task execution period in milliseconds.
 * @param  priority  Task priority (lower value = higher priority).
 *
 * @retval uint8
 *   - DEF_True  : Task successfully registered.
 *   - DEF_False : Registration failed (maximum task count reached).
 *
 * @note
 *   - Maximum number of tasks is limited by `Task_MaxNum`.
 *   - Each task's last runtime is initialized to the current tick (`Tick_Get()`).
 *   - Registered tasks will be managed by `Task_RunScheduler()`.
 */
uint8	Task_Register(Task_Func_t Func, uint32 period)
{
	uint32	nowTime = Tick_Get();

	if (Task_Cnt >= Task_MaxNum)
	{
		return DEF_False;
	}

	Task_Fifo[Task_Cnt]
			   = (Task_Info_t){ Func, period, nowTime };
	Task_Cnt++;

	return DEF_True;
}

