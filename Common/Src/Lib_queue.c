/*
 * Lib_queue.c
 *
 *  Created on: Sep 25, 2025
 *      Author: ETI
 */


/*****************************************************************************/
/*  1. Files to include                                                      */
/*****************************************************************************/

#include "Lib_queue.h"


/*****************************************************************************/
/*  2. Local Definitions                                                     */
/*****************************************************************************/


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
* @brief    return now queue length
* @param    *q - queue structure pointer
* @return   unsigned int - queue size
*/
extern unsigned int qsize(const UART_QUEUE *q)
{
	return (( q->wptr - q->rptr ) % Q_BUF);
}

/*---------------------------------------------------------------------*/
/**
* @brief    Initialization queue
* @param    *q - queue structure pointer
* @return   void
*/		

/*
extern void qinit(UART_QUEUE *q)
{
	q->rptr = 0u;
 	q->wptr = 0u;

	(void*)memset(q->buf, 0, Q_BUF);
}
*/

/*---------------------------------------------------------------------*/
/**
* @brief    Get data from queue
* @param    *q - queue structure pointer
* @return   char - get data
*/		
extern bool qget(UART_QUEUE *q, char *c)
{
	bool rv = DEF_False;
    
	if ( qsize(q) > 0u ) 
	{
		*c = q->buf[q->rptr];
		q->rptr = ( q->rptr + 1u) % Q_BUF;
		
		rv = DEF_True;
	}

	return rv;
}

/*---------------------------------------------------------------------*/
/**
* @brief    Put data to queue
* @param    *q - queue structure pointer
* @param    c - data
* @return   void
*/
extern void qput(UART_QUEUE *q, char c)
{
	q->buf[q->wptr] = c;
	q->wptr = ( q->wptr + 1u) % Q_BUF;
}


/*---------------------------------------------------------------------*/
/**
* @brief    Convert Ascii code to nibble
* @param    ascii - ascii value
* @param    *hex - hex value pointer
* @return   TRUE - Success, FALSE - Fail
*/
extern BOOL Ascii2Nibble(const uint8 ascii, uint16* hex)
{
	BOOL rv = TRUE;
   // uint16 chr;
    	
	if ((ascii >= (uint8)0x30) && (ascii <= (uint8)0x39))	
	{	
		*hex = (uint16)((uint16)ascii - 0x30u);
	}
	else if ((ascii >= (uint8)0x41) && (ascii <= (uint8)0x46))
	{
		*hex = (uint16)((uint16)ascii - 0x37u);
	}
	else
	{
		*hex = (uint16)(0u);
		rv = FALSE;
	}
   
	return rv;
}


/*---------------------------------------------------------------------*/
/**
* @brief    Convert nibble to ascii code
* @param    nibble - nibble value
* @param    *ascii - ascii value pointer
* @return   TRUE - Success, FALSE - Fail
*/
extern BOOL Nibble2Ascii(const uint8 nibble, uint8* ascii)
{
    BOOL rv;

//	if ((nibble >= 0) && (nibble < 16))
    if(nibble < (uint8)16)
	{
//		if ((nibble >=0) && (nibble <= 9))	*ascii = (nibble + 0x30);
		if (nibble <= (uint8)9)
		{
		    *ascii = (nibble + (uint8)0x30);
		}
		else
		{
		    *ascii = (nibble + (uint8)0x37);
		}
		rv = TRUE;
	}
	else
	{
	    rv = FALSE;
	}

	return rv;
}

/*---------------------------------------------------------------------*/
/**
* @brief    function that copy memory from index to length
* @param    *dest - destination pointer
* @param    *dest - source pointer
* @param    index - start index
* @param    len - Length
* @return   void
*/
extern void memncpy(uint8* dest, const uint8* src, uint16 index, uint16 len)
{
	uint16 i;
//	char* d = (char*)dest;	
//	const char* s = (char*)src;
	
	for(i=0; i<len; i++)
	{
		//*(d + i + index) = *(s + i + index);
		dest[i + index] = src[i + index];
	}
}

/*---------------------------------------------------------------------*/
/**
* @brief    function that add memory from index to length
* @param    *dest - destination pointer
* @param    *dest - source pointer
* @param    index - start index
* @param    len - Length
* @return   void
*/
extern void memnadd(uint8* dest, const uint8* src, uint16 index, uint16 len)
{
	uint16 i;
//	char* d = (char*)dest ;	
//	const char* s = (char*)src;
	
	for(i=0; i<len; i++)
	{
		//*(d + i + index) = *(s + i);
		dest[i + index] = src[i];
	}
}

/*---------------------------------------------------------------------*/
/**
* @brief    function that compares memory from index to length
* @param    *dest - destination pointer
* @param    *dest - source pointer
* @param    index - start index
* @param    len - Length
* @return   TRUE - Same, FALSE - Different
*/
extern BOOL memncmp(const uint8* dest, const uint8* src, uint16 index, uint16 len)
{
	uint16 i;
//	const char* d = dest;	
//	const char* s = src;
    BOOL rv = TRUE;

	for(i=0; i<len; i++)
	{
		//if(*(d + i + index) != *(s+i))	return FALSE;
		if(dest[i + index] != src[i])
		{
		    rv = FALSE;
		    break;
		}
	}

	return rv;
}

