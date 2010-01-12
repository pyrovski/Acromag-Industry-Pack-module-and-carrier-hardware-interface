
#include "../carrier/apc8620.h"
#include "ip230.h"

/*
{+D}
    SYSTEM:	    Library Software - IP230/235

    FILENAME:	    wro230.c

    MODULE NAME:    wro230 - write analog output for IP230/235 board.

    VERSION:	    Rev A

    CREATION DATE:  05/19/98

    DESIGNED BY:    F.J.M./R.H.

    CODED BY:	    F.J.M/R.H.

    ABSTRACT:	    This module is used to perform the write output function
		    for the IP230/235 board.

    CALLING
	SEQUENCE:   wro230(channel,data, c_blk);
		    where:
			channel (unsigned short)
			    Value of the analog output channel number (0-7).
			data (unsigned short)
			    Value of the analog output data.
                channel (unsigned short)
		    c_blk
			    Pointer to the configuration block structure.

    MODULE TYPE:    void

    I/O RESOURCES:

    SYSTEM
	RESOURCES:

    MODULES
	CALLED:

    REVISIONS:

  DATE	  BY	    PURPOSE
-------  ----	------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

    This module is used to perform the write output function
    for the IP230/235 board.  A structure pointer to the board
    memory map, the analog output channel number value, and the
    analog output data value will be passed to this routine.  The routine
    writes the output data to the analog output channel register on
    the board.
*/


void wro230(channel,data, c_blk)
  int channel;		       /* analog output channel number */
  short data;		       /* data to write to the output channel */
  struct cblk230 *c_blk;

{

/*
    ENTRY POINT OF ROUTINE:

    Write the output data to the output channel on the board.
*/
    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->dac_reg[channel], data );
}








/*
{+D}
    SYSTEM:	    Library Software - IP230/235

    FILENAME:	    wro230.c

    MODULE NAME:    start_con - Start conversions for IP230/235 board.

    VERSION:	    Rev A

    CREATION DATE:  05/19/98

    DESIGNED BY:    F.J.M./R.H.

    CODED BY:	    F.J.M/R.H.

    ABSTRACT:	    This module is used to start conversions for the IP230/235 board.

    CALLING
	SEQUENCE:   start_con( c_blk);
			    c_blk
			    Pointer to the configuration block structure.


    MODULE TYPE:    void

    I/O RESOURCES:

    SYSTEM
	RESOURCES:

    MODULES
	CALLED:

    REVISIONS:

  DATE	  BY	    PURPOSE
-------  ----	------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

    This module is used to start conversions on the IP230/235 board.
*/

void start_con(c_blk)
struct cblk230 *c_blk;

{

/*
    ENTRY POINT OF ROUTINE:

    Write the output data to the output channel on the board
*/
    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->start_conv, 0x0001 );
}

