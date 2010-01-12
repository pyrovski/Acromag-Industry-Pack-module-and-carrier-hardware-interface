
#include "../carrier/apc8620.h"
#include "ip470.h"

/* 
{+D}
    SYSTEM:             Acromag IP470 Digital I/O Board

    FILENAME:           wprt470.c
	
    MODULE NAME:        wprt470() - write output port
			    
    VERSION:            A
    
    CREATION DATE:      10/18/95
    
    CODED BY:           FJM
    
    ABSTRACT:           This module writes output values to a single I/O port.
    
    CALLING SEQUENCE:   status = wprt470(c_blk, port, value);
			  where:
			    status (long)
			      The returned error status.
			    c_blk (pointer to structure)
			      Pointer to configuration structure.
			    port (unsigned char)
			      The target I/O port number.
			    value (unsigned char)
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

    This module writes a 8-bit hex output value (from 00 to FF) to 
    a output port.
*/


long wprt470(c_blk, port, value)

struct cblk470 *c_blk;      /* configuration data structure */
byte port;                  /* the I/O port number */
byte value;                 /* the output value */

{

/*
    ENTRY POINT OF ROUTINE
*/
   if(port > MAXPORTS)
     return(-1);

   bank_select(BANK0, c_blk);      /* select I/O bank */
   output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[port].b_select, value);
   return(0);
}

