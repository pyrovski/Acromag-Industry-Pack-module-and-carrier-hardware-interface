
#include "../carrier/apc8620.h"
#include "ip409.h"

/* 
{+D}
    SYSTEM:             Acromag IP409 Digital I/O Board

    FILENAME:           rprt409.c
        
    MODULE NAME:        rprt409() - read input port
                            
    VERSION:            A
    
    CREATION DATE:      05/19/98
    
    CODED BY:           FJM
    
    ABSTRACT:           The module reads an input value from a single I/O port.
                        
    CALLING SEQUENCE:   status = rprt409(ptr, port);
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
        of "port" is from 0 to 1; otherwise, a -1 is returned.
*/


long rprt409(c_blk, port)

struct cblk409 *c_blk;
unsigned port;                  /* the I/O port number */

{

/*
    ENTRY POINT OF THE ROUTINE
*/

    if (port > 1 )		/* error checking */
	  return(-1);
    
    return((long)input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->io_port[port] ) & 0x0000FFFF );
}
