
#include "../carrier/apc8620.h"
#include "ip482.h"
/*
{+D}
    SYSTEM:		Acromag IP482 Digital I/O Board

    FILENAME:		wprt482.c

    MODULE NAME:	wprt482() - write output port

    VERSION:		A

    CREATION DATE:	10/28/03

    CODED BY:		FJM

    ABSTRACT:		This module writes output values to a single port.

    CALLING SEQUENCE:	status = wprt482(ptr, value);
			  where:
			    status (long)
			      The returned error status.
			    ptr (pointer to structure)
			      Pointer to the board memory map.
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


long wprt482(ptr, value)


struct ip482 *ptr;
unsigned value; 	    /* the output value */

{

/*
    ENTRY POINT OF ROUTINE
*/

	output_word(ptr->nHandle, (word*)&ptr->brd_ptr->DigitalOut, value);
	return(0);
}
