
#include "../carrier/apc8620.h"
#include "ip230.h"

/*
{+D}
    SYSTEM:	    IP230/235 Software

    FILENAME:	    timer.c

    MODULE NAME:    conv_time - routine to write to the conversion
		    timer.

    VERSION:	    REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is the routine that does writes to the
		    conversion timer.

    CALLING
	SEQUENCE:

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

       This module is the routine that performs writes to the
       conversion timer.
*/

void conv_time(amount, c_blk)
   int amount;	 /* data to be written */
   struct cblk230 *c_blk;
{
   output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->conv_tim, amount );
}






/*
{+D}
    SYSTEM:	    IP230/235 Software

    FILENAME:	    timer.c

    MODULE NAME:    tim_pre - routine to write to the timer prescaler.

    VERSION:	    REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is the routine that does writes to the
		    timer prescaler.

    CALLING
	SEQUENCE:

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

       This module is the routine that performs writes to the
       timer prescaler.
*/

void tim_pre(value, c_blk)
   int value;	/* data to be written */
   struct cblk230 *c_blk;

{
   output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->tim_pres, (word)value);
}
