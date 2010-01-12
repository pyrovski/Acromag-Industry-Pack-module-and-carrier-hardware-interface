
#include "../carrier/apc8620.h"
#include "ip409.h"

/*
{+D}
    SYSTEM:		    Library Software

    FILENAME:		    cnfg409.c

    MODULE NAME:	    cnfg409 - configure 409 board

    VERSION:		    A

    CREATION DATE:	    05/19/98

    CODED BY:		    FJM

    ABSTRACT:		    This module is used to perform the configure function
			    for the IP409 board.

    CALLING
	SEQUENCE:	    cnfg409(ptr);

			    where:
				ptr (pointer to structure)
				    Pointer to the configuration block structure.

    MODULE TYPE:      void

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
    for the IP409 board.  A pointer to the Configuration Block will
    be passed to this routine.  The routine will use a pointer
    within the Configuration Block to reference the registers
    on the Board.  Based on flag bits in the Attribute and
    Parameter Flag words in the Configuration Block, the board
    will be configured and various registers will be updated with
    new information which will be transfered from the Configuration
    Block to registers on the Board.
*/



void cnfg409(c_blk)

struct cblk409 *c_blk;

{

/*
    declare local storage
*/

int i;

/*
    ENTRY POINT OF ROUTINE:

    Check to see if the Interrupt Vector Register is to be updated.
    Update the Vector Register before enabling Global Interrupts so that
    the board will always output the correct vectors upon interrupt.
*/

    if(c_blk->param & VECT )
	    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->int_vect, (word)c_blk->vector);

/*
    If the Direction Register is to be updated, then update it.
*/

    if(c_blk->param & DIR)
    {
 	  for( i = 0; i < 2; i++)
		output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->dir_reg[i], c_blk->direction[i]);
    }

/*
    If the Type Select Register is to be updated, then update it.
*/

    if(c_blk->param & INT_TYPE)
    {
	  for( i = 0; i < 2; i++)
	    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->type_reg[i], c_blk->type[i]);
    }

/*
    If the Interrupt Polarity Register is to be updated . . .
*/

    if(c_blk->param & INT_POLARITY)
    {
	  for( i = 0; i < 2; i++)
		output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->pol_reg[i], c_blk->polarity[i]);
    }

/*
    If the Interrupt Status Register is to be updated, then update it.
*/

    if(c_blk->param & INT_STATUS)
    {
	  for( i = 0; i < 2; i++)
	    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->sts_reg[i], c_blk->int_status[i]);
    }


/*
    If the Interrupt Enable Register is to be updated then do so.
*/

    if(c_blk->param & INT_ENAB)
    {
	  for( i = 0; i < 2; i++)
	    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->en_reg[i], c_blk->enable[i]);
    }
}
