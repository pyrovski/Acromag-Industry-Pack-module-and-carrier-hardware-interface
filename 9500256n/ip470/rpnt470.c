
#include "../carrier/apc8620.h"
#include "ip470.h"

/* 
{+D}
    SYSTEM:             Acromag IP470 Digital I/O Board

    FILENAME:           rpnt470.c
	
    MODULE NAME:        rpnt470() - read input point
			    
    VERSION:            A
    
    CREATION DATE:      10/18/95
    
    CODED BY:           FJM
    
    ABSTRACT:           The module reads a input value from a single I/O point.

    CALLING SEQUENCE:   status=rpnt470(c_blk,port,point);
			  where:
			    status (long)
			      The returned value of the I/O point
			      or error flag.
			    c_blk (pointer to structure)
			      Pointer to the configuration data structure.
			    port (unsigned char)
			      The target I/O port number.
			    point (unsigned char)
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
	returns the current value of the point.
*/

long rpnt470(c_blk, port, point)

struct cblk470 *c_blk;      /* configuration data structure */
byte port;                  /* the I/O port number */
byte point;                 /* the I/O point of a port */

{
    
/*
    ENTRY POINT OF ROUTINE
*/

   if(port > MAXPORTS)
     return(-1);

   if(point > MAXBITS)
     return(-1);
   
   bank_select(BANK0, c_blk);      /* select I/O bank */
   
   if(input_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[port].b_select) & (1 << point))
      return(1);
   else 
      return(0);
}
