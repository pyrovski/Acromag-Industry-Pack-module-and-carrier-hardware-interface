
#include "../carrier/apc8620.h"
#include "ip1k100.h"

/* 
{+D}
    SYSTEM:             Acromag IP1k100 Digital I/O Board

    FILENAME:           wprt1k100.c
        
    MODULE NAME:        wprt1k100() - write output port
                            
    VERSION:            A
    
    CREATION DATE:      10/01/01
    
    CODED BY:           FJM
    
    ABSTRACT:           This module writes output values to a single I/O port.
    
    CALLING SEQUENCE:   status = wprt1k100(ptr, port, value);
                          where:
                            status (long)
                              The returned error status.
							ptr (pointer to structure)
							  Pointer to the configuration block structure.
                            port (unsigned)
                              The target I/O port number.
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

    This module writes a 16-bit hex output value (from 0000 to FFFF) to the output port.
    The valid value of "port" is from 0 to 2. Otherwise, a -1 is returned.
*/


long wprt1k100(c_blk, port, value)

struct cblk1k100 *c_blk;
unsigned port;              /* the I/O port number */
unsigned value;             /* the output value */

{

/*
    ENTRY POINT OF ROUTINE
*/

    if (port > 2)	/* error checking */
       return(-1);

    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->io_port[port], value );
    return(0);
}
