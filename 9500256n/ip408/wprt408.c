
#include "../carrier/apc8620.h"
#include "ip408.h"

/* 
{+D}
    SYSTEM:             Acromag IP408 Digital I/O Board

    FILENAME:           wprt408.c
        
    MODULE NAME:        wprt408() - write output port
                            
    VERSION:            A
    
    CREATION DATE:      01/02/95
    
    CODED BY:           FJM
    
    ABSTRACT:           This module writes output values to a single I/O port.
    
    CALLING SEQUENCE:   status = wprt408(ptr, port, value);
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
    a output port.
    The valid value of "port" is from 0 to 1. Otherwise, a -1 is returned.
*/


long wprt408(c_blk, port, value)

struct cblk408 *c_blk;
unsigned port;              /* the I/O port number */
unsigned value;             /* the output value */

{

/*
    declare local storage
*/

    struct map408 *map_ptr;	/* pointer to board memory map */

/*
    ENTRY POINT OF ROUTINE:
    Initialize local storage.  The board memory map pointer is initialized.
*/

    map_ptr = (struct map408 *)c_blk->brd_ptr;

    if (port > 1 || value > 0xFFFF)	/* error checking */
       return(-1);
    else 
    {
        output_word( c_blk->nHandle, (word*)&map_ptr->out_port[port], value);
        return(0);
    }
}


