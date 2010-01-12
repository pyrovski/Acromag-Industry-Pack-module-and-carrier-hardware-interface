#include "ip1k100.h"

/*
{+D}
   SYSTEM:          Acromag I/O Board

   MODULE NAME:     isr1k100.c - interrupt handler

   VERSION:         A

   CREATION DATE:   10/01/01

   CODED BY:        FM

   ABSTRACT:        This routine performs interrupt exception handling
                    for interrupts on the IP1k100 I/O board.

   CALLING
	SEQUENCE:   This subroutine runs as a result of an interrupt occuring.

   MODULE TYPE:

   I/O RESOURCES:

   SYSTEM
	RESOURCES:

   MODULES
	CALLED:

   REVISIONS:

 DATE      BY       PURPOSE
-------- ----  ------------------------------------------------

{-D}
*/

/*
   MODULES FUNCTIONAL DETAILS:
*/


void isr_1k100(void* pData)

{

/*
	Local data areas
*/

int  i,j;     	              /* loop control */
word b_mask;                  /* bit interrupt bit mask */
struct handler_data* hData;   /* Interrupt handler structure */
struct cblk1k100* cblk;	      /* configuration block */

/*
	Entry point of routine:
*/

	/*  Initialize pointers */
	hData = (struct handler_data*)pData;
	cblk = (struct cblk1k100*)hData->hd_ptr;

 	cblk->sblk_ptr->type = input_word( (word*)&cblk->brd_ptr->type_reg); /* interrupt type */

 	cblk->sblk_ptr->enable = input_word( (word*)&cblk->brd_ptr->en_reg); /* interrupt enable */

	cblk->sblk_ptr->int_status = input_word((word*)&cblk->brd_ptr->sts_reg); /* interrupt status */

	if( cblk->sblk_ptr->int_status )	/* non-zero if interrupt pending */
	{
	    /* Check each bit of the status register for a interrupt pending */
	    for( j = 0; j < 8; j++ )		/* prioritization bit 7 to 0 */
  	    {   
	        b_mask = 0x80 >> j;		/* form interrupting mask */
	        if( b_mask & cblk->sblk_ptr->int_status ) /* non-zero if interrupt pending */
	        {
		     if( b_mask & cblk->sblk_ptr->type )  /* non-zero if COS selected */
		     {  
				/* nothing for COS now */
		     }
		     else				     /* LEVEL selected */
		     {
			  cblk->sblk_ptr->enable &= (~b_mask); /* disable this bit */
		     }
		}
	    } 
            output_word( (word*)&cblk->brd_ptr->en_reg, cblk->sblk_ptr->enable);
            output_word( (word*)&cblk->brd_ptr->sts_reg,cblk->sblk_ptr->int_status);/* clear pending interrupts */
	}
}

