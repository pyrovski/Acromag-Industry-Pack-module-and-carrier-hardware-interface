
#include "../carrier/apc8620.h"
#include "ip445.h"

/* 
{+D}
	SYSTEM:             Acromag IP445 Digital I/O Board

	FILENAME:           wprt445.c
		
	MODULE NAME:        wprt445() - write output port
							
	VERSION:            A
	
	CREATION DATE:      05/19/98
	
	CODED BY:           FJM
	
	ABSTRACT:           This module writes output values to a single I/O port.
	
	CALLING SEQUENCE:   status = wprt445(ptr, port, value);
				  where:
					status (long)
					  The returned error status.
 					ptr (pointer to structure)
					  Pointer to the configuration block structure.
					port (unsigned)
					  The target I/O port number.
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

	This module writes a 8-bit hex output value (from 00 to FF) to 
	an I/O port.
	The valid value of "port" is from 0 to 3. Otherwise, a -1 is returned.
*/


long wprt445(c_blk, port, value)

struct sblk445 *c_blk;
unsigned port;              /* the I/O port number */
byte value;                 /* the output value */

{

/*
	ENTRY POINT OF ROUTINE
*/

	if (port > 3 ) /* error checking */
	   return(-1);

	output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->io_map[port].io_port, value);
	return(0);
}
