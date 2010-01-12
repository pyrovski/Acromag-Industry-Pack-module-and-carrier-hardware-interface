
#include "../carrier/apc8620.h"
#include "ip405.h"

/* 
{+D}
    SYSTEM:             Acromag IP405 Digital I/O Board

    FILENAME:           rprt405.c
        
    MODULE NAME:        rprt405() - read input port
                            
    VERSION:            A
    
    CREATION DATE:      05/19/98
    
    CODED BY:           FJM
    
    ABSTRACT:           The module reads an input value from a single I/O port.
                        
    CALLING SEQUENCE:   status = rprt405(ptr, port);
                          where:
                            status (long)
                              The returned value of the I/O port
                              or error flag.
 							ptr (pointer to structure)
							  Pointer to the configuration block structure.
                            port (unsigned)
                              The target I/O port number.
                            
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

        This module reads a value from an I/O port, and returns the 
        current value of all points in that port. The valid value 
        of "port" is from 0 to 2; otherwise, a -1 is returned.
*/


long rprt405(c_blk, port)

struct status_blk *c_blk;
unsigned port;                  /* the I/O port number */

{

/*
    ENTRY POINT OF THE ROUTINE
*/

    if (port > 2 )		/* error checking */
       return(-1);

    return ( (long)input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->io_map[port].io_port) );
}

