
#include "../carrier/apc8620.h"
#include "ip409.h"

/* 
{+D}
    SYSTEM:             Acromag IP409 Digital I/O Board

    FILENAME:           wprt409.c
        
    MODULE NAME:        wprt409() - write output port
                            
    VERSION:            A
    
    CREATION DATE:      05/19/98
    
    CODED BY:           FJM
    
    ABSTRACT:           This module writes output values to a single I/O port.
    
    CALLING SEQUENCE:   status = wprt409(ptr, port, value);
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


long wprt409(c_blk, port, value)

struct cblk409 *c_blk;
unsigned port;              /* the I/O port number */
unsigned value;             /* the output value */

{

/*
    ENTRY POINT OF ROUTINE
*/

    if (port > 1)	/* error checking */
       return(-1);

    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->io_port[port], value );
    return(0);
}
