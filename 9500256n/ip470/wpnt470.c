
#include "../carrier/apc8620.h"
#include "ip470.h"

/* 
{+D}
    SYSTEM:             Acromag IP470 Digital I/O  Board

    FILENAME:           wpnt470.c
	
    MODULE NAME:        wpnt470() - write output point
			    
    VERSION:            A
    
    CREATION DATE:      10/18/95
    
    CODED BY:           FJM
	
    ABSTRACT:           Module writes an output value to a single I/O point.
    
    CALLING SEQUENCE:   status = wpnt470(c_blk, port, point, value);
			  where:
			    status (long)
			      The returned error status.
			    cblk (pointer to structure)
			      Pointer pointer to configuration structure.
			    port (unsigned char)
			      The target I/O port number.
			    point (unsigned char)
			      The target I/O point number
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

	This module writes a value (0 or 1) to a point of an output port.
*/
    
long wpnt470(c_blk, port, point, value)

struct cblk470 *c_blk;      /* configuration block structure */
byte port;                  /* the I/O port number */
byte point;                 /* the I/O point of a port */
byte value;                 /* the output value */

{

/*
    DECLARE LOCAL DATA AREAS:
*/

    byte bpos;              /* bit position */
    byte temp;              /* the I/O port number */

/*
    ENTRY POINT OF ROUTINE
*/
	if(port > MAXPORTS)
	  return(-1);

	if(point > MAXBITS)
	  return(-1);
	
	if(value > POINTVAL)
	  return(-1);
   
	bank_select(BANK0, c_blk);      /* select I/O bank */
        
	bpos = 1 << point;
	value <<= point;

    temp = input_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[port].b_select);
	temp = ( temp & ~bpos ) | value;
    output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[port].b_select,temp);

	return(0);
}
