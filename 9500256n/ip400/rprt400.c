
#include "../carrier/apc8620.h"
#include "ip400.h"

/* 
{+D}
    SYSTEM:             Acromag IP400 Digital Input Board

    FILENAME:           rprt400.c
        
    MODULE NAME:        rprt400() - read input port
                            
    VERSION:            A
    
    CREATION DATE:      05/19/98
    
    CODED BY:           FJM
    
    ABSTRACT:           The module reads an input value from a single port.
                        
    CALLING SEQUENCE:   status = rprt400(ptr, port);
                          where:
                            status (long)
                              The returned value of the port
                              or error flag.
							ptr (pointer to structure)
							  Pointer to the configuration block structure.
                            port (unsigned)
                              The target port number.
                            
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

        This module reads a value from an input port, and returns the 
        current value of all points in that port. The valid value 
        of "port" is from 0 to 2; otherwise, a -1 is returned.
*/


long rprt400(c_blk, port)

struct cblk400 *c_blk;
unsigned port;                  /* the port number */

{

/*
    ENTRY POINT OF THE ROUTINE
*/

    if (port > 2 )		/* error checking */
       return(-1);

    return ( (long)input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->in_port[port]) );
}
