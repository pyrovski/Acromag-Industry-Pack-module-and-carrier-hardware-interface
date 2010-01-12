
#include "../carrier/apc8620.h"
#include "ip445.h"

/* 
{+D}
	SYSTEM:             Acromag IP445 Digital I/O Board

	FILENAME:           rprt445.c
		
	MODULE NAME:        rprt445() - read input port
							
	VERSION:            A
	
	CREATION DATE:      05/19/98
	
	CODED BY:           BDB
	
	ABSTRACT:           The module reads an input value from a single I/O port.
						
	CALLING SEQUENCE:   status = rprt445(ptr, port);
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
	of "port" is from 0 to 3; otherwise, a -1 is returned.
*/


long rprt445(c_blk, port)

struct sblk445 *c_blk;
unsigned port;                  /* the I/O port number */

{
/*
	ENTRY POINT OF THE ROUTINE
*/

	if (port > 3 )      /* error checking */
	   return(-1);

	return ( (long)input_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->io_map[port].io_port) );
}
