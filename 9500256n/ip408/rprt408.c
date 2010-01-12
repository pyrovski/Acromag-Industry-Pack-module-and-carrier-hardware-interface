
#include "../carrier/apc8620.h"
#include "ip408.h"

/* 
{+D}
    SYSTEM:             Acromag IP408 Digital I/O Board

    FILENAME:           rprt408.c
        
    MODULE NAME:        rprt408() - read input port
                            
    VERSION:            A
    
    CREATION DATE:      01/03/95
    
    CODED BY:           FJM
    
    ABSTRACT:           The module reads an input value from a single I/O port.
                        
    CALLING SEQUENCE:   status = rprt408(ptr, port);
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


long rprt408(c_blk, port)

struct cblk408 *c_blk;
unsigned port;                  /* the I/O port number */

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

    if (port > 1 )		/* error checking */
       return(-1);
    else 
       return ((long)input_word(c_blk->nHandle,(word*)&map_ptr->in_port[port]) );
}



