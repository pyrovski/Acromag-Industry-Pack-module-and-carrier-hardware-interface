
#include "../carrier/apc8620.h"
#include "ip470.h"

/* 
{+D}
    SYSTEM:             Acromag IP470 Digital I/O Board

    FILENAME:           rprt470.c
	
    MODULE NAME:        rprt470() - read input port
			    
    VERSION:            A
    
    CREATION DATE:      10/18/95
    
    CODED BY:           FJM
    
    ABSTRACT:           The module reads an input value from a single I/O port.
			
    CALLING SEQUENCE:   status = rprt470(c_blk, port);
			  where:
			    status (long)
			      The returned value of the I/O port
			      or error flag.
			    c_blk (pointer to structure)
			      Pointer to the configuration data structure.
			    port (unsigned char)
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
	current value of all points in that port.
*/


long rprt470(c_blk, port)

struct cblk470 *c_blk;          /* configuration data structure */
byte port;                      /* the I/O port number */

{

/*
    ENTRY POINT OF ROUTINE
*/
   
   if(port > MAXPORTS)
     return(-1);

   bank_select(BANK0, c_blk);      /* select I/O bank */
   return(input_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[port].b_select));
}
