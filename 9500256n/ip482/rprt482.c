
#include "../carrier/apc8620.h"
#include "ip482.h"
/*
{+D}
    SYSTEM:		Acromag IP482 Digital I/O Board

    MODULE NAME:	rprt482() - read port

    VERSION:		A

    CREATION DATE:	10/28/03

    CODED BY:		FJM

    ABSTRACT:		The module reads an input value from a port.

    CALLING SEQUENCE:	status = rprt482(ptr, port);
			  where:
			    status (long)
			      The returned value of the I/O port
			      or error flag.
			    ptr (pointer to structure)
			      Pointer to the board memory map structure.
			    port (unsigned)
			      The target port input or output. 0=Input, 1=output


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


long rprt482(ptr, port)

struct ip482 *ptr;
unsigned port;

{

/*
    ENTRY POINT OF THE ROUTINE
*/

	if( port )
	    return ((long)input_word(ptr->nHandle, (word*)&ptr->brd_ptr->DigitalOut));
	else
		return ((long)input_word(ptr->nHandle, (word*)&ptr->brd_ptr->DigitalInput));

}
