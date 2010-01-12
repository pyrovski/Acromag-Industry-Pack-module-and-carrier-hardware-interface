
#include "../carrier/apc8620.h"
#include "ip445.h"

/*
{+D}
    SYSTEM:         Library Software

    FILENAME:       wcntl445.c

    MODULE NAME:    wcntl445(); - routine that writes the control register
                                on the IP445.

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    BDB

    CODED BY:       BDB
                                                         
    ABSTRACT:       This module is the routine which writes the control
                    register on the IP445.
 
    CALLING
        SEQUENCE:   status = wcntl(ptr,value);
                          where:
                            status (long)
                              The returned error status.
							ptr (pointer to structure)
							  Pointer to the configuration block structure.
                            value (byte)
                              The output value.
                        

    MODULE TYPE:    long

    I/O RESOURCES:  

    SYSTEM
        RESOURCES:  

    MODULES
        CALLED:     

    REVISIONS:      

  DATE    BY        PURPOSE
-------  ----   ------------------------------------------------

{-D}
*/

long wcntl445(c_blk, value)
struct sblk445 *c_blk;
byte    value;               /* value to write to control register */
{
/*
    ENTRY POINT OF ROUTINE
*/

    output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->cntl_reg, value);
    return(0);
}
