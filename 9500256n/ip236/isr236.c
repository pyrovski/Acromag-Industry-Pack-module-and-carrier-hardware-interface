#include "ip236.h"


/*
{+D}
   SYSTEM:	    Acromag Input Board

   MODULE NAME:     isr236.c - interrupt handler

   VERSION:	    A

   CREATION DATE:   12/30/98

   CODED BY:	    FM

   ABSTRACT:	    These routines perform interrupt exception handling	
		    for interrupts on the IP236 Input board.

   CALLING
	SEQUENCE:
		    This subroutine runs as a result of an interrupt occuring.

   MODULE TYPE:    n/a

   I/O RESOURCES:

   SYSTEM
	RESOURCES:

   MODULES
	CALLED:

   REVISIONS:

 DATE	   BY	    PURPOSE
-------- ----  ------------------------------------------------

{-D}
*/

/*
   MODULES FUNCTIONAL DETAILS:

*/


void isr_236(void* pData)

{

/*
	Local data areas
*/

struct handler_data* hData;   /* Interrupt handler structure */
struct conf_blk* cblk;	      /* configuration block */
int i;				/* counter */


/*
	Entry point of routine:
*/

	/*  Initialize pointers */
	hData = (struct handler_data*)pData;
	cblk = (struct conf_blk*)hData->hd_ptr;

	cblk->interrupt_flag = input_byte( (byte*)&cblk->brd_ptr->int_status ); /* update interrupt status */

	for(i = 0; i < NUM_CHAN; i++)
	{
		if( cblk->interrupt_flag & ( 1 << i))/* interrupt pending for this channel? */	
			wro236(cblk, i);	     /* service channel */
	}
}
