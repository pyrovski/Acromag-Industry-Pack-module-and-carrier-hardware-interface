
#include "../carrier/apc8620.h"
#include "ip400.h"
/* 
{+D}
    SYSTEM:             Acromag IP400 Digital Input Board

    FILENAME:           rpnt400.c
        
    MODULE NAME:        rpnt400() - read input point
                            
    VERSION:            A
    
    CREATION DATE:      05/19/98
    
    CODED BY:           FJM
    
    ABSTRACT:           The module reads a input value from a single point.

    CALLING SEQUENCE:   status=rpnt400(ptr,port,point);
                          where:
                            status (long)
                              The returned value of the point
                              or error flag.
							ptr (pointer to structure)
							  Pointer to the configuration block structure.
                            port (unsigned)
                              The target port number.
                            point (unsigned)
                              The target point.
                            
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

        This module reads a value from an input point of a port, and 
        returns the current value of the point. The valid values of 
        "port" are 0 to 2 and "point" are from 0 to 15; otherwise,
        a -1 is returned.  If no error, either 0 or 1 is returned.
*/

long rpnt400(c_blk, port, point)

struct cblk400 *c_blk;
unsigned port;              /* the port number */
unsigned point;             /* the point of a port */

{
    
/*
    ENTRY POINT OF ROUTINE
*/

    if (port > 2 || point > 15 )	/* error checking */
       return(-1);

    if ( input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->in_port[port]) & (1 << point) )
            return(1);
    else 
            return(0);
}
