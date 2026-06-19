/*
 * Default_Parameters.h
 *
 *  Created on: Sep 9, 2025
 *      Author: ETI
 */

#ifndef INC_DEFAULT_PARAMETERS_H_
#define INC_DEFAULT_PARAMETERS_H_

/************************************************************************
* Macro definitions
************************************************************************/
/*_____________________________________________________________________________________________
 		# Item										#Value		# Real 		# Unit	# Scale
______________________________________________________________________________________________*/

/* 1500V Mode */
#define DEFAULT_CONFIG_VER							1			// 0.1				0.1
#define DEFAULT_NUMBER_OF_BMIC						42			// 42		ea		1
#define DEFAULT_NUMBER_OF_PARALLEL					3			// 3		ea		1
#define DEFAULT_NUMBER_OF_SERIES					42			// 42		ea		1
#define DEFAULT_NUMBER_OF_CELL_inModule				8			// 8		ea		1
#define DEFAULT_NUMBER_OF_Temp_inModule				4			// 4		ea		1
#define DEFAULT_OPERATE_FAN_TEMP					30			// 30		℃		1
#define DEFAULT_DEV_FOR_BALANCE_START				30	 		// 0.03		V		0.001
#define DEFAULT_DEV_FOR_BALANCE_STOP				10	 		// 0.01		V		0.001
#define DEFAULT_MAX_CELL_V                          4200        // 4.200    V       0.001
#define DEFAULT_MIN_CELL_V                          2700        // 2.700    V       0.001
#define DEFAULT_NOMI_CELL_V                         3719        // 3.719    V       0.001
#define DEFAULT_CELL_CAPACITY                       60000       // 60000    mAh     1
#define DEFAULT_CONT_REF_V                          50          // 5        V       0.1
#define DEFAULT_DEBUG_ENABLE                        0           // 0                1

#define DEFAULT_OCV_WARNING							4200		// 4.20 	V		0.001
#define DEFAULT_OPV_WARNING							14112		// 1411.2	V		0.1
#define DEFAULT_UCV_WARNING							3000		// 3.0		V		0.001
#define DEFAULT_UPV_WARNING							10080		// 1008		V		0.1
#define DEFAULT_COT_WARNING							510			// 51 		℃		0.1
#define DEFAULT_DOT_WARNING							510			// 51 		℃		0.1
#define DEFAULT_CUT_WARNING							-170		// -17 		℃		0.1
#define DEFAULT_DUT_WARNING							-170		// -17		℃		0.1
#define DEFAULT_COC_WARNING							900			// 90		A		0.1
#define DEFAULT_DOC_WARNING							900			// 90		A		0.1
#define DEFAULT_LOWSOC_WARNING						100			// 10		%		0.1
#define DEFAULT_CELL_IMBALANCE_WARNING				300			// 0.3		V		0.001
#define DEFAULT_MODULE_IMBALANCE_WARNING			2400		// 2.4		V		0.001
#define DEFAULT_ISOLATION_WARNING					1100		// 1100		kOhm	1
#define DEFAULT_SLAVE_COMM_WARNING					50			// 5		sec		0.1
#define DEFAULT_MASTER_COMM_WARNING					50			// 5		sec		0.1
#define DEFAULT_OAV_WARNING							32000		// 32		V		0.001
#define DEFAULT_UAV_WARNING							18000		// 18		V		0.001
#define DEFAULT_RLEC_MALFUNCTION_WARNING			50			// 5		sec		0.1

#define DEFAULT_OCV_FAULT							4230		// 4.23		V		0.001
#define DEFAULT_OPV_FAULT							14212		// 1421.2	V		0.1
#define DEFAULT_UCV_FAULT							2700		// 2.7		V		0.001
#define DEFAULT_UPV_FAULT							9072 		// 907.2	V		0.1
#define DEFAULT_COT_FAULT							540			// 54		℃		0.1
#define DEFAULT_DOT_FAULT							540			// 54		℃		0.1
#define DEFAULT_CUT_FAULT							-190		// -19		℃		0.1
#define DEFAULT_DUT_FAULT							-190		// -19		℃		0.1
#define DEFAULT_COC_FAULT							945			// 94.5		A		0.1
#define DEFAULT_DOC_FAULT							945			// 94.5		A		0.1
#define DEFAULT_CELL_IMBALANCE_FAULT				500			// 0.5		V		0.001
#define DEFAULT_MODULE_IMBALANCE_FAULT				4000		// 4		V		0.001
#define DEFAULT_ISOLATION_FAULT						1000		// 1000		kOhm	1
#define DEFAULT_SLAVE_COMM_FAULT					100			// 10		sec		0.1
#define DEFAULT_MASTER_COMM_FAULT					100			// 10		sec		0.1
#define DEFAULT_OAV_FAULT							36000		// 36		V		0.001
#define DEFAULT_UAV_FAULT							9000		// 9		V		0.001
#define DEFAULT_FUSE_DEAD_FAULT						2000		// 200		V		0.1
#define DEFAULT_RLEC_MALFUNCTION_FAULT				200			// 20		sec		0.1
#define DEFAULT_DEADCELL_FAULT						2000		// 2		V		0.001

#define DEFAULT_OCV_WARNING_RELEASE					4190		// 4.19		V		0.001
#define DEFAULT_OPV_WARNING_RELEASE					14078		// 1407.8	V  		0.1
#define DEFAULT_UCV_WARNING_RELEASE					3200		// 3.2		V		0.001
#define DEFAULT_UPV_WARNING_RELEASE					10752		// 1075.2	V		0.1
#define DEFAULT_COT_WARNING_RELEASE					500			// 50		℃		0.1
#define DEFAULT_DOT_WARNING_RELEASE					500			// 50		℃		0.1
#define DEFAULT_CUT_WARNING_RELEASE					-150		// -15		℃		0.1
#define DEFAULT_DUT_WARNING_RELEASE					-150		// -15		℃		0.1
#define DEFAULT_COC_WARNING_RELEASE					855			// 85.5		A		0.1
#define DEFAULT_DOC_WARNING_RELEASE					855			// 85.5		A		0.1
#define DEFAULT_LOWSOC_WARNING_RELEASE				110			// 11		%		0.1
#define DEFAULT_CELL_IMBALANCE_WARNING_RELEASE		290			// 0.29		V		0.001
#define DEFAULT_MODULE_IMBALANCE_WARNING_RELEASE	2320		// 2.32		V		0.001
#define DEFAULT_ISOLATION_WARNING_RELEASE			1500		// 1500		kOhm	1
#define DEFAULT_SLAVE_COMM_WARNING_RELEASE			20			// 2		sec		0.1
#define DEFAULT_MASTER_COMM_WARNING_RELEASE			20			// 2		sec		0.1
#define DEFAULT_OAV_WARNING_RELEASE					31000		// 31		V		0.001
#define DEFAULT_UAV_WARNING_RELEASE					19000		// 19		V		0.001
#define DEFAULT_RLEC_MALFUNCTION_WARNING_RELEASE	50			// 5		sec		0.1

#define DEFAULT_OCV_FAULT_RELEASE					4200		// 4.20		V		0.001
#define DEFAULT_OPV_FAULT_RELEASE					14112		// 1411.2	V		0.1
#define DEFAULT_UCV_FAULT_RELEASE					3000		// 2.7		V		0.001
#define DEFAULT_UPV_FAULT_RELEASE					10080		// 1008		V		0.1
#define DEFAULT_COT_FAULT_RELEASE					510			// 51		℃		0.1
#define DEFAULT_DOT_FAULT_RELEASE					510			// 51		℃		0.1
#define DEFAULT_CUT_FAULT_RELEASE					-180		// -18		℃		0.1
#define DEFAULT_DUT_FAULT_RELEASE					-180		// -18		℃		0.1
#define DEFAULT_COC_FAULT_RELEASE					900			// 90		A		0.1
#define DEFAULT_DOC_FAULT_RELEASE					900			// 90		A		0.1
#define DEFAULT_CELL_IMBALANCE_FAULT_RELEASE		300			// 0.3		V		0.001
#define DEFAULT_MODULE_IMBALANCE_FAULT_RELEASE		2400		// 2.4		V	    0.001
#define DEFAULT_ISOLATION_FAULT_RELEASE				1100		// 1100		kOhm	1
#define DEFAULT_SLAVE_COMM_FAULT_RELEASE			20			// 2		sec		0.1
#define DEFAULT_MASTER_COMM_FAULT_RELEASE			20			// 2		sec		0.1
#define DEFAULT_OAV_FAULT_RELEASE					32000		// 32		V		0.001
#define DEFAULT_UAV_FAULT_RELEASE					18000		// 18		V		0.001
#define DEFAULT_FUSE_DEAD_FAULT_RELEASE				3000		// 300		V		0.1
#define DEFAULT_RLEC_MALFUNCTION_FAULT_RELEASE		70			// 7		sec		0.1
#define DEFAULT_DEADCELL_FAULT_RELEASE				2700		// 2.7		V		0.001

/* 1000V Mode */
#define DEFAULT2_CONFIG_VER							1			// 0.1				0.1
#define DEFAULT2_NUMBER_OF_BMIC						28			// 28		ea		1
#define DEFAULT2_NUMBER_OF_PARALLEL					3			// 3		ea		1
#define DEFAULT2_NUMBER_OF_SERIES					28			// 28		ea		1
#define DEFAULT2_NUMBER_OF_CELL_inModule			8			// 8		ea		1
#define DEFAULT2_NUMBER_OF_Temp_inModule			4			// 4		ea		1
#define DEFAULT2_OPERATE_FAN_TEMP					30			// 30		℃		1
#define DEFAULT2_DEV_FOR_BALANCE_START				30	 		// 0.03		V		0.001
#define DEFAULT2_DEV_FOR_BALANCE_STOP				10	 		// 0.01		V		0.001
#define DEFAULT2_MAX_CELL_V                         4200        // 4.200    V       0.001
#define DEFAULT2_MIN_CELL_V                         2700        // 2.700    V       0.001
#define DEFAULT2_NOMI_CELL_V                        3719        // 3.719    V       0.001
#define DEFAULT2_CELL_CAPACITY                      60000       // 60000    mAh     1
#define DEFAULT2_CONT_REF_V                         50          // 5        V       0.1
#define DEFAULT2_DEBUG_ENABLE                       0           // 0                1

#define DEFAULT2_OCV_WARNING						4200		// 4.2	 	V		0.001
#define DEFAULT2_OPV_WARNING						9408		// 940.8	V		0.1
#define DEFAULT2_UCV_WARNING						3000		// 3.0		V		0.001
#define DEFAULT2_UPV_WARNING						6720		// 672		V		0.1
#define DEFAULT2_COT_WARNING						510			// 51 		℃		0.1
#define DEFAULT2_DOT_WARNING						510			// 51 		℃		0.1
#define DEFAULT2_CUT_WARNING						-170		// -17 		℃		0.1
#define DEFAULT2_DUT_WARNING						-170		// -17		℃		0.1
#define DEFAULT2_COC_WARNING						900			// 90		A		0.1
#define DEFAULT2_DOC_WARNING						900			// 90		A		0.1
#define DEFAULT2_LOWSOC_WARNING						100			// 10		%		0.1
#define DEFAULT2_CELL_IMBALANCE_WARNING				300			// 0.3		V		0.001
#define DEFAULT2_MODULE_IMBALANCE_WARNING			2400		// 2.4		V		0.001
#define DEFAULT2_ISOLATION_WARNING					1100		// 1100		kOhm	1
#define DEFAULT2_SLAVE_COMM_WARNING					50			// 5		sec		0.1
#define DEFAULT2_MASTER_COMM_WARNING				50			// 5		sec		0.1
#define DEFAULT2_OAV_WARNING						32000		// 32		V		0.001
#define DEFAULT2_UAV_WARNING						18000		// 18		V		0.001
#define DEFAULT2_RLEC_MALFUNCTION_WARNING			50			// 5		sec		0.1

#define DEFAULT2_OCV_FAULT							4230		// 4.23		V		0.001
#define DEFAULT2_OPV_FAULT							9475		// 947.5	V		0.1
#define DEFAULT2_UCV_FAULT							2700		// 2.7		V		0.001
#define DEFAULT2_UPV_FAULT							6048 		// 604.8	V		0.1
#define DEFAULT2_COT_FAULT							540			// 54		℃		0.1
#define DEFAULT2_DOT_FAULT							540			// 54		℃		0.1
#define DEFAULT2_CUT_FAULT							-190		// -19		℃		0.1
#define DEFAULT2_DUT_FAULT							-190		// -19		℃		0.1
#define DEFAULT2_COC_FAULT							945			// 94.5		A		0.1
#define DEFAULT2_DOC_FAULT							945			// 94.5		A		0.1
#define DEFAULT2_CELL_IMBALANCE_FAULT				500			// 0.5		V		0.001
#define DEFAULT2_MODULE_IMBALANCE_FAULT				4000		// 4		V		0.001
#define DEFAULT2_ISOLATION_FAULT					1000		// 1000		kOhm	1
#define DEFAULT2_SLAVE_COMM_FAULT					100			// 10		sec		0.1
#define DEFAULT2_MASTER_COMM_FAULT					100			// 10		sec		0.1
#define DEFAULT2_OAV_FAULT							36000		// 36		V		0.001
#define DEFAULT2_UAV_FAULT							9000		// 9		V		0.001
#define DEFAULT2_FUSE_DEAD_FAULT					2000		// 200		V		0.1
#define DEFAULT2_RLEC_MALFUNCTION_FAULT				200			// 20		sec		0.1
#define DEFAULT2_DEADCELL_FAULT						2000		// 2		V		0.001

#define DEFAULT2_OCV_WARNING_RELEASE				4190		// 4.19		V		0.001
#define DEFAULT2_OPV_WARNING_RELEASE				9385		// 938.5	V  		0.1
#define DEFAULT2_UCV_WARNING_RELEASE				3200		// 3.2		V		0.001
#define DEFAULT2_UPV_WARNING_RELEASE				7168		// 716.8	V		0.1
#define DEFAULT2_COT_WARNING_RELEASE				500			// 50		℃		0.1
#define DEFAULT2_DOT_WARNING_RELEASE				500			// 50		℃		0.1
#define DEFAULT2_CUT_WARNING_RELEASE				-150		// -15		℃		0.1
#define DEFAULT2_DUT_WARNING_RELEASE				-150		// -15		℃		0.1
#define DEFAULT2_COC_WARNING_RELEASE				855			// 85.5		A		0.1
#define DEFAULT2_DOC_WARNING_RELEASE				855			// 85.5		A		0.1
#define DEFAULT2_LOWSOC_WARNING_RELEASE				110			// 11		%		0.1
#define DEFAULT2_CELL_IMBALANCE_WARNING_RELEASE		290			// 0.29		V		0.001
#define DEFAULT2_MODULE_IMBALANCE_WARNING_RELEASE	2320		// 2.32		V		0.001
#define DEFAULT2_ISOLATION_WARNING_RELEASE			1500		// 1500		kOhm	1
#define DEFAULT2_SLAVE_COMM_WARNING_RELEASE			20			// 2		sec		0.1
#define DEFAULT2_MASTER_COMM_WARNING_RELEASE		20			// 2		sec		0.1
#define DEFAULT2_OAV_WARNING_RELEASE				31000		// 31		V		0.001
#define DEFAULT2_UAV_WARNING_RELEASE				19000		// 19		V		0.001
#define DEFAULT2_RLEC_MALFUNCTION_WARNING_RELEASE	50			// 5		sec		0.1

#define DEFAULT2_OCV_FAULT_RELEASE					4200		// 4.2		V		0.001
#define DEFAULT2_OPV_FAULT_RELEASE					9408		// 940.8	V		0.1
#define DEFAULT2_UCV_FAULT_RELEASE					3000		// 3.0		V		0.001
#define DEFAULT2_UPV_FAULT_RELEASE					6720		// 672		V		0.1
#define DEFAULT2_COT_FAULT_RELEASE					510			// 51		℃		0.1
#define DEFAULT2_DOT_FAULT_RELEASE					510			// 51		℃		0.1
#define DEFAULT2_CUT_FAULT_RELEASE					-180		// -18		℃		0.1
#define DEFAULT2_DUT_FAULT_RELEASE					-180		// -18		℃		0.1
#define DEFAULT2_COC_FAULT_RELEASE					900			// 90		A		0.1
#define DEFAULT2_DOC_FAULT_RELEASE					900			// 90		A		0.1
#define DEFAULT2_CELL_IMBALANCE_FAULT_RELEASE		300			// 0.3		V		0.001
#define DEFAULT2_MODULE_IMBALANCE_FAULT_RELEASE		2400		// 2.4		V	    0.001
#define DEFAULT2_ISOLATION_FAULT_RELEASE			1100		// 1100		kOhm	1
#define DEFAULT2_SLAVE_COMM_FAULT_RELEASE			20			// 2		sec		0.1
#define DEFAULT2_MASTER_COMM_FAULT_RELEASE			20			// 2		sec		0.1
#define DEFAULT2_OAV_FAULT_RELEASE					32000		// 32		V		0.001
#define DEFAULT2_UAV_FAULT_RELEASE					18000		// 18		V		0.001
#define DEFAULT2_FUSE_DEAD_FAULT_RELEASE			3000		// 300		V		0.1
#define DEFAULT2_RLEC_MALFUNCTION_FAULT_RELEASE		70			// 7		sec		0.1
#define DEFAULT2_DEADCELL_FAULT_RELEASE				2700		// 2.7		V		0.001



#endif /* INC_DEFAULT_PARAMETERS_H_ */
