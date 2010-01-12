
#include "../carrier/apc8620.h"
#include "ip408.h"

/* 
{+D}
    SYSTEM:             Acromag IP408 Digital I/O  Board

    FILENAME:           wpnt408.c
        
    MODULE NAME:        wpnt408() - write output point
                            
    VERSION:            A
    
    CREATION DATE:      01/03/95
    
    CODED BY:           FJM
        
    ABSTRACT:           Module writes an output value to a single I/O point.
    
    CALLING SEQUENCE:   status = wpnt408(ptr, port, point, value);
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

        This module writes a value (0 or 1) to a point of an output port.
        The valid values of "port" are 0 to 1 and "point" are from 0 to 15.
        Otherwise, a -1 is returned.
*/
    
long wpnt408(c_blk, port, point, value)

struct cblk408 *c_blk;
unsigned port;              /* the I/O port number */
unsigned point;             /* the I/O point of a port */
unsigned value;             /* the output value */

{

/*
    DECLARE LOCAL DATA AREAS:
*/

    struct map408 *map_ptr;	/* pointer to board memory map */
    unsigned bpos;			/* bit position */
	unsigned nValue;		/* current value of port */

/*
    ENTRY POINT OF ROUTINE:
    Initialize local storage.  The board memory map pointer is initialized.
*/

    map_ptr = (struct map408 *)c_blk->brd_ptr;

    if (port > 1 || point > 15 || value > 1)    /* error checking */
       return(-1);
    else 
    {
        bpos = 1 << point;
        value <<= point;
		nValue = input_word(c_blk->nHandle,(word*)&map_ptr->out_port[port]);
        output_word(c_blk->nHandle,(word*)&map_ptr->out_port[port], ( nValue & ~bpos ) | value);
        return(0);
    }
}


