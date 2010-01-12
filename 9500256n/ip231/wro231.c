
#include "../carrier/apc8620.h"
#include "ip231.h"

/*
{+D}
    SYSTEM:         Library Software - IP231

    FILENAME:       wro231.c

    MODULE NAME:    wro231 - write analog output for IP231 board.

    VERSION:        Rev A

    CREATION DATE:  09/24/04

    DESIGNED BY:    F.J.M.

    CODED BY:       F.J.M

    ABSTRACT:       This module is used to perform the write output function
		    for the IP231 board.

    CALLING
        SEQUENCE:   wro231(ptr,channel,data);
                    where:
        		ptr (pointer to structure)
		            Pointer to the configuration block structure.
                        channel (unsigned short)
                            Value of the analog output channel number (0-7).
                        data (unsigned short)
                            Value of the analog output data.

    MODULE TYPE:    void

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


/*
    MODULES FUNCTIONAL DETAILS:

    This module is used to perform the write output function
    for the IP231 board.  A structure pointer to the board
    memory map, the analog output channel number value, and the
    analog output data value will be passed to this routine.  The routine
    writes the output data to the analog output channel register on
    the board.
*/


void wro231(c_blk,channel,data)

struct cblk231 *c_blk;
word channel;               /* analog output channel number */
word data;                  /* data to write to the output channel */
{


	int i;

/*
    ENTRY POINT OF ROUTINE:
    Write the output data to the output channel on the board.
*/

    for( i = 25; i; i--)
    {
      if( input_word(c_blk->nHandle, (word *)&c_blk->brd_ptr->dac_status) & ( 1 << channel ))
          break;
    }

    output_word( c_blk->nHandle, (word *)&c_blk->brd_ptr->dac_reg[channel], data );
}


