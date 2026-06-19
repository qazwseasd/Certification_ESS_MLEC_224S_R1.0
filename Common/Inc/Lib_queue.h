/*
 * Lib_queue.h
 *
 *  Created on: Sep 25, 2025
 *      Author: ETI
 */

#ifndef INC_LIB_QUEUE_H_
#define INC_LIB_QUEUE_H_


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/
#include "App_Def.h"



/*****************************************************************************/
/*  2. Global Definitions                                                    */
/*****************************************************************************/
#define Q_BUF	256u

/*****************************************************************************/
/*  3. Global Typedefs                                                       */
/*****************************************************************************/
typedef struct
{
	char buf[Q_BUF];
	unsigned int wptr;
	unsigned int rptr;
}UART_QUEUE;


/*****************************************************************************/
/*  4. Declarations of Global Variables                                      */
/*****************************************************************************/


/*****************************************************************************/
/*  5(Last). Prototypes of Global Functions                                  */
/*****************************************************************************/
extern unsigned int qsize(const UART_QUEUE *q);
// extern void qinit(UART_QUEUE *q);
extern bool qget(UART_QUEUE *q, char *c);
extern void qput(UART_QUEUE *q, char c);

extern BOOL Ascii2Hex(const uint8* ascii, uint16* hex,  uint16 len);
extern BOOL Ascii2Nibble(const uint8 ascii, uint16* hex);
extern BOOL Hex2Ascii(const uint16 hex, uint8* ascii, uint16 len);
extern BOOL Nibble2Ascii(const uint8 nibble, uint8* ascii);

extern void memncpy(uint8* dest, const uint8* src, uint16 index, uint16 len);
extern void memnadd(uint8* dest, const uint8* src, uint16 index, uint16 len);
extern BOOL memncmp(const uint8* dest, const uint8* src, uint16 index, uint16 len);




#endif /* INC_LIB_QUEUE_H_ */
