
#include "../carrier/apc8620.h"
#include "ip236.h"

/*
{+D}
    SYSTEM:		  Library Software - ip236 Board

    MODULE NAME:	  build control - build control word for ip236

    VERSION:	  V1.0

    CREATION
	DATE:		  12/29/98

    CODED BY:	  F.J.M.

    ABSTRACT:	  This builds up the control word to be used in
		        converting the ip236 board.

    CALLING
	SEQUENCE:     build_control(ptr);
			    where:
				ptr (pointer to structure)
				    Pointer to the configuration block structure.

    MODULE TYPE:    int

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
*/



byte build_control(c_blk)

struct conf_blk *c_blk;

{

/*
    declare local storage
*/

    byte control;		/* board control register */

/*
    ENTRY POINT OF ROUTINE:
    Build up control word
*/

    control = 0;		/* initialize control register value */

    switch( c_blk->mode[c_blk->current_channel])
    {
	case 0:			/* select single conversion */
		control |= 2;
	break;
	case 1:			/* continuous conversions */
		control |= 4;
	break;
	case 2:			/* external trigger input */
		control |= 6;
	break;

	default:		/* disabled */
				/* nothing to do */
	break;
    }

    if( c_blk->trigger[c_blk->current_channel] == 1)	/* select triggering */
 	control |= 8;		/* output */


    if( c_blk->interrupt_enable[c_blk->current_channel] == 1) /* interrupt enabled */
       control |= 16;
    
    switch( c_blk->threshold[c_blk->current_channel])	/* select threshold */
    {
	case 1:			/* 4 Values Remaining */
		control |= 32;
	break;

	case 2:			/* 16 Values Remaining */
		control |= 64;
	break;

	case 3:			/* 64 Values Remaining */
		control |= 96;
	break;

	default:		/* disabled */
				/* nothing to do */
	break;
    }
 
    return(control);		/* return control register */
}




/*
{+D}
    SYSTEM:		Library Software - ip236 Board

    MODULE NAME:	cnfg236 - configure ip236

    VERSION:		V1.0

    CREATION DATE:	12/29/98

    CODED BY:		F.J.M.

    ABSTRACT:		This configures the ip236 board.

    CALLING
	SEQUENCE:	cnfg236(ptr);
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
*/



void cnfg236(c_blk)

struct conf_blk *c_blk;

{

byte build_control();		/* generate control register value */
void reset_channel();		/* reset single channel */

/*
    ENTRY POINT OF ROUTINE:
*/
    reset_channel(c_blk);	/* reset channel */
    output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->int_vector, c_blk->interrupt_vector );
    output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->dac_map[c_blk->current_channel].prescaler, (byte)c_blk->prescaler[c_blk->current_channel] );
    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->dac_map[c_blk->current_channel].counter, c_blk->counter[c_blk->current_channel] );
    output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->dac_map[c_blk->current_channel].control, build_control(c_blk) ); 
}
