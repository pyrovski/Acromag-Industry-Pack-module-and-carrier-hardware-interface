
#include "../carrier/apc8620.h"
#include "ip400.h"

/*
{+D}
    SYSTEM:	    Library Software

    FILENAME:	    cnfg400.c

    MODULE NAME:    cnfg400 - configure 400 board

    VERSION:	    A

    CREATION DATE:  05/19/98

    CODED BY:	    FJM

    ABSTRACT:       This module is used to perform the configure function
		    for the IP400 board.

    CALLING
	SEQUENCE:   cnfg400(ptr);
		    where:
			ptr (pointer to structure)
			    Pointer to the configuration block structure.

    MODULE TYPE:    void

    I/O RESOURCES:

    SYSTEM
	RESOURCES:

    MODULES
	CALLED:

    REVISIONS:

  DATE	  BY	    PURPOSE
-------  ----	------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

    This module is used to perform the "configure board" function
    for the IP400 board.  A pointer to the Configuration Block will
    be passed to this routine.  The routine will use a pointer
    within the Configuration Block to reference the registers
    on the Board.  Based on flag bits in the Attribute and
    Parameter Flag words in the Configuration Block, the board
    will be configured and various registers will be updated with
    new information which will be transfered from the Configuration
    Block to registers on the Board.
*/



void cnfg400(c_blk)

struct cblk400 *c_blk;

{


/*
    ENTRY POINT OF ROUTINE:

    Check to see if the Interrupt Vector Register is to be updated.
    Update the Vector Register before enabling Global Interrupts so that
    the board will always output the correct vectors upon interrupt.
*/

    if(c_blk->param & VECT )
	   output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->int_vect, ((word)c_blk->vector) );

/*
    If the Type Select Register is to be updated, then update it.
*/

    if(c_blk->param & INT_TYPE)
    	output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->type_reg, c_blk->type );

/*
    If the Interrupt Polarity Register is to be updated . . .
*/

    if(c_blk->param & INT_POLARITY)
	output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->pol_reg, c_blk->polarity );

/*
    If the Interrupt Enable Register is to be updated then do so.
*/

    if(c_blk->param & INT_ENAB)
	output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->en_reg, c_blk->enable );

/*
    If the Interrupt Status Register is to be updated, then update it.
*/

    if(c_blk->param & INT_STATUS)
    	output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->sts_reg, c_blk->int_status );
}
