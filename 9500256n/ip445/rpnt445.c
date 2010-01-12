
#include "../carrier/apc8620.h"
#include "ip445.h"

/* 
{+D}
	SYSTEM:             Acromag IP445 Digital I/O Board

	FILENAME:           rpnt445.c
		
	MODULE NAME:        rpnt445() - read input point
							
	VERSION:            A
	
	CREATION DATE:      03/06/96
	
	CODED BY:           BDB
	
	ABSTRACT:           The module reads a input value from a single I/O point.

	CALLING SEQUENCE:   status=rpnt445(ptr,port,point);
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
	"port" are 0 to 3 and "point" are from 0 to 7; otherwise,
	a -1 is returned.  If no error, either 0 or 1 is returned.
*/

long rpnt445(c_blk, port, point)

struct sblk445 *c_blk;
unsigned port;              /* the I/O port number */
unsigned point;             /* the I/O point of a port */

{

/*
	ENTRY POINT OF ROUTINE
*/

	if (port > 3 || point > 7 )    /* error checking */
	   return(-1);

	if ((input_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->io_map[port].io_port)) & (1 << point))
		return(1);
	else 
		return(0);
}
