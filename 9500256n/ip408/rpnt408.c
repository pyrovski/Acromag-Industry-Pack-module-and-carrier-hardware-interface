
#include "../carrier/apc8620.h"
#include "ip408.h"

/* 
{+D}
    SYSTEM:             Acromag IP408 Digital I/O Board

    FILENAME:           rpnt408.c
        
    MODULE NAME:        rpnt408() - read input point
                            
    VERSION:            A
    
    CREATION DATE:      01/03/95
    
    CODED BY:           FJM
    
    ABSTRACT:           The module reads a input value from a single I/O point.

    CALLING SEQUENCE:   status=rpnt408(ptr,port,point);
                          where:
                            status (long)
                              The returned value of the I/O point
                              or error flag.
							ptr (pointer to structure)
							  Pointer to the configuration block structure.
                            port (unsigned)
                              The target I/O port number.
                            point (unsigned)
                              The target I/O point.
                            
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

        This module reads a value from an I/O point of an I/O port, and 
        returns the current value of the point. The valid values of 
        "port" are 0 to 1 and "point" are from 0 to 15; otherwise,
        a -1 is returned.  If no error, either 0 or 1 is returned.
*/

long rpnt408(c_blk, port, point)

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
        if ( (unsigned)input_word(c_blk->nHandle,(word*)&map_ptr->in_port[port]) & (1 << point) )
            return(1);
        else 
            return(0);
    }
}


