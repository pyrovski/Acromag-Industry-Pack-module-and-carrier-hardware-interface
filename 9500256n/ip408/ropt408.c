
#include "../carrier/apc8620.h"
#include "ip408.h"

/*
{+D}
    SYSTEM:		Acromag IP408 Digital I/O Board

    FILENAME:		ropt408.c

    MODULE NAME:	ropt408() - read output port data latch

    VERSION:		A

    CREATION DATE:	01/04/95

    CODED BY:		FJM

    ABSTRACT:		This module reads a value from the output
			port data latch.

    CALLING SEQUENCE:	status = ropt408(ptr, port);
			  where:
			    status (long)
			      The returned value of the I/O port
			      or error flag.
				ptr (pointer to structure)
				  Pointer to the configuration block structure.
			    port (unsigned)
			      The I/O port number

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

    This module reads a value from the output port data latch, and returns
    the current value of all 16 points in the port.
*/

long ropt408(c_blk, port)

struct cblk408 *c_blk;
unsigned port;                  /* the I/O port number */


{

/*
    DECLARE LOCAL DATA AREAS:
*/

    struct map408 *map_ptr;	/* pointer to board memory map */

/*
    ENTRY POINT OF ROUTINE:
    Initialize local storage.  The board memory map pointer is initialized.
*/

    map_ptr = (struct map408 *)c_blk->brd_ptr;

    if (port > 1 )		/* error checking */
       return(-1);
    else 
       return ( (long)input_word(c_blk->nHandle,(word*)&map_ptr->out_port[port]) );
}


