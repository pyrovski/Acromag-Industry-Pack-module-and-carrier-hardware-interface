
#include "../carrier/apc8620.h"
#include "ip445.h"

/* 
{+D}
	SYSTEM:             Acromag IP445 Digital I/O  Board

	FILENAME:           wpnt445.c
		
	MODULE NAME:        wpnt445() - write output point
							
	VERSION:            A
	
	CREATION DATE:      05/19/98
	
	CODED BY:           BDB
		
	ABSTRACT:           Module writes an output value to a single I/O point.
	
	CALLING SEQUENCE:   status = wpnt445(ptr, port, point, value);
				  where:
					status (long)
					  The returned error status.
 					ptr (pointer to structure)
					  Pointer to the configuration block structure.
					port (unsigned)
					  The target I/O port number.
					point (unsigned)
					  The target I/O point number
					value (byte)
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

	This module writes a value (0 or 1) to an I/O point of an I/O port.
	The valid values of "port" are 0 to 3 and "point" are from 0 to 7.
	Otherwise, a -1 is returned.
*/
	
long wpnt445(c_blk, port, point, value)

struct sblk445 *c_blk;
unsigned port;              /* the I/O port number */
unsigned point;             /* the I/O point of a port */
byte value;                 /* the output value */

{

/*
	DECLARE LOCAL DATA AREAS:
*/
	byte bpos;              /* bit position */

/*
	ENTRY POINT OF ROUTINE
*/

	if (port > 3 || point > 7 || value > 1)    /* error checking */
	   return(-1);

	bpos = 1 << point;
	value <<= point;
	output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->io_map[port].io_port, 
		((input_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->io_map[port].io_port ) & ~bpos) | value));
	return(0);
}
