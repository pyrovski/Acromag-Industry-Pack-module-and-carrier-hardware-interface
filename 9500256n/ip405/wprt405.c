
#include "../carrier/apc8620.h"
#include "ip405.h"

/* 
{+D}
    SYSTEM:             Acromag IP405 Digital I/O Board

    FILENAME:           wprt405.c
        
    MODULE NAME:        wprt405() - write output port
                            
    VERSION:            A
    
    CREATION DATE:      05/19/98
    
    CODED BY:           FJM
    
    ABSTRACT:           This module writes output values to a single I/O port.
    
    CALLING SEQUENCE:   status = wprt405(ptr, port, value);
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

    This module writes a 16-bit hex output value (from 0000 to FFFF) to 
    an I/O port.
    The valid value of "port" is from 0 to 2. Otherwise, a -1 is returned.
*/


long wprt405(c_blk, port, value)

struct status_blk *c_blk;
unsigned port;              /* the I/O port number */
unsigned value;             /* the output value */

{

/*
    ENTRY POINT OF ROUTINE
*/

    if (port > 2 || value > 0xFFFF)	/* error checking */
       return(-1);

    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->io_map[port].io_port, value);
    return(0);
}

