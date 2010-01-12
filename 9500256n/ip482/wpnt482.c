
#include "../carrier/apc8620.h"
#include "ip482.h"
/*
{+D}
    SYSTEM:		Acromag IP482 Digital I/O Board

    FILENAME:	wpnt482.c

    MODULE
	NAME:		wpnt482() - write output point

    VERSION:		A

    CREATION DATE:	10/28/03

    CODED BY:		FJM

    ABSTRACT:		Module writes an output value to a single I/O point.

    CALLING SEQUENCE:	status = wpnt482(ptr, point, value);
			  where:
			    status (long)
			      The returned error status.
			    ptr (pointer to structure)
			      Pointer to the board memory map structure.
			    point (unsigned)
			      The target point number
			    value (unsigned)
			      The output value.

    MODULE TYPE:	long

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTION DETAILS
*/

long wpnt482(ptr, point, value)

struct ip482 *ptr;
unsigned point; 	    /* the I/O point of a port */
unsigned value; 	    /* the output value */

{

/*
    DECLARE LOCAL DATA AREAS:
*/

    unsigned bpos;		/* bit position */
    unsigned nValue;	/* current value of port */
/*
    ENTRY POINT OF ROUTINE
*/

    if (value > 1)	/* error checking */
		return(-1);
    else
    {
		bpos = 1 << point;
		value <<= point;
		nValue = input_word(ptr->nHandle, (word*)&ptr->brd_ptr->DigitalOut);
		output_word(ptr->nHandle, (word*)&ptr->brd_ptr->DigitalOut, ( nValue & ~bpos ) | value);
		return(0);
    }
}
