
#include "../carrier/apc8620.h"
#include "ip408.h"

/*
{+D}
    SYSTEM:		Acromag IP408 Digital I/O Board

    FILENAME:		robt408.c

    MODULE NAME:	robt408() - read output point from data latch

    VERSION:		A

    CREATION DATE:	01/04/95

    CODED BY:		FJM

    ABSTRACT:		This module reads a single point from the output port.

    CALLING SEQUENCE:	status = robit408(ptr,port,point);
			  where:
			    status (long)
			      The returned value of the
			      Input point or error flag.
				ptr (pointer to structure)
				  Pointer to the configuration block structure.
			    port (unsigned)
			      The I/O port number
			    point (unsigned)
			      The target Input point.

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

    This module reads a value from the output port data latch and returns the
    current value of the point specified.  Valid values for "point" are from
    0 to 15; otherwise, a -1 is returned.  If no error, either 0 or 1 is
    returned.
*/

long robt408(c_blk, port, point)

struct cblk408 *c_blk;
unsigned port;              /* the I/O port number */
unsigned point;             /* the I/O point of a port */

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

    if (port > 1 || point > 15 )	/* error checking */
       return(-1);
    else 
    {
        if ( (unsigned)input_word(c_blk->nHandle,(word*)&map_ptr->out_port[port]) & (1 << point) )
            return(1);
        else 
            return(0);
    }
}


