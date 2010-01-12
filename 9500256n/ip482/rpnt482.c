
#include "../carrier/apc8620.h"
#include "ip482.h"
/*
{+D}
    SYSTEM:		Acromag IP482 Digital I/O Board

    MODULE NAME:	rpnt482() - read point

    VERSION:		A

    CREATION DATE:	10/28/03

    CODED BY:		FJM

    ABSTRACT:		The module reads a input value from a single I/O point.

    CALLING SEQUENCE:	status=rpnt482(ptr, port, point);
			  where:
			    status (long)
			      The returned value of the I/O point
			      or error flag.
			    ptr (pointer to structure)
			      Pointer memory map structure.
			    port (unsigned)
			      The target port input or output. 0=Input, 1=output
			    point (unsigned)
			      The target input point.

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

	This module reads a value from an I/O point and
*/

long rpnt482(ptr, port, point)

struct ip482 *ptr;
unsigned port; 	    /* the port */
unsigned point;	    /* the I/O point of a port */

{

/*
    ENTRY POINT OF ROUTINE
*/

	if( port )
	{
	  if ( (unsigned)input_word(ptr->nHandle, (word*)&ptr->brd_ptr->DigitalOut) & (1 << point) )
	    return(1);
	  else
	    return(0);
	}
	else
	{
 	  if ( (unsigned)input_word(ptr->nHandle, (word*)&ptr->brd_ptr->DigitalInput) & (1 << point) )
	    return(1);
	  else
	    return(0);
	}
}

