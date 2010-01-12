
#include "../carrier/apc8620.h"
#include "ip405.h"

/* 
{+D}
    SYSTEM:             Acromag IP405 Digital I/O  Board

    FILENAME:           wpnt405.c
        
    MODULE NAME:        wpnt405() - write output point
                            
    VERSION:            A
    
    CREATION DATE:      05/19/98
    
    CODED BY:           FJM
        
    ABSTRACT:           Module writes an output value to a single I/O point.
    
    CALLING SEQUENCE:   status = wpnt405(ptr, port, point, value);
                          where:
                            status (long)
                              The returned error status.
 							ptr (pointer to structure)
							  Pointer to the configuration block structure.
                            port (unsigned)
                              The target I/O port number.
                            point (unsigned)
                              The target I/O point number
                            value (unsigned)
                              The output value.
                            
    MODULE TYPE:        long
    
    I/O RESOURCES:
    
    SYSTEM RESOURCES:   

    MODULES CALLED:
    
    REVISIONS:
    
      DATE      BY      PURPOSE
    --------   -----    ---------------------------------------------------
    
{-D}
*/


/*  
    MODULES FUNCTION DETAILS

        This module writes a value (0 or 1) to an I/O point of an I/O port.
        The valid values of "port" are 0 to 2 and "point" are from 0 to 15.
        Otherwise, a -1 is returned.
*/
    
long wpnt405(c_blk, port, point, value)

struct status_blk *c_blk;
unsigned port;              /* the I/O port number */
unsigned point;             /* the I/O point of a port */
unsigned value;             /* the output value */

{

/*
    DECLARE LOCAL DATA AREAS:
*/

    unsigned bpos;				/* bit position */
/*
    ENTRY POINT OF ROUTINE
*/

    if (port > 2 || point > 15 || value > 1)    /* error checking */
       return(-1);

    bpos = 1 << point;
    value <<= point;

    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->io_map[port].io_port,
		(input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->io_map[port].io_port) & ~bpos) | value);
    return(0);
}

