
#include "../carrier/apc8620.h"
#include "ip230.h"

/*
{+D}
    SYSTEM:	    Library Software - ip230 Board

    FILENAME:	    cd230.c

    MODULE NAME:    cd230 - analog output corrected data

    VERSION:	    Rev A

    CREATION DATE:  11/18/96

    CODED BY:	    R.H.

    ABSTRACT:	    This module is used to correct output conversions
		    for the ip230 board.

    CALLING
	SEQUENCE:   cd230(ptr, channel);
		    where:
			ptr (pointer to structure)
			    Pointer to the configuration block structure.
			channel (channel to act on)

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

    This module is used to correct the input data for the
    ip230 board.  A pointer to the Configuration Block will be passed to
    this routine.  The routine will use a pointer within the Configuration
    Block to reference the registers on the Board.
*/


void cd230(c_blk,channel)

struct cblk230 *c_blk;
int channel;

{

/*
    declare local storage
*/

    double f_cor;
    int i_data;
    int range;
    short offset;
    short gain;
/*
    ENTRY POINT OF ROUTINE:
    Initialize variables
*/

    i_data = c_blk->ideal_buf[channel];
    range = c_blk->range[channel];

    /* default range == BIPOLAR_10 */
    offset = (short)c_blk->coef_bufb10[channel][0];
    gain = (short)c_blk->coef_bufb10[channel][1] & 0xffff;

    if(range == BIPOLAR_5)
    {
     offset = (short)c_blk->coef_bufb5[channel][0];
     gain = (short)c_blk->coef_bufb5[channel][1] & 0xffff;
    }

    if( range == UNIPOLAR_10 )
    {
      offset = (short)c_blk->coef_bufu10[channel][0];
      gain = (short)c_blk->coef_bufu10[channel][1] & 0xffff;
      i_data &= 0xFFFF;		/* remove sign extension for unipolar only*/
      i_data ^= 0x8000;		/* convert bipolar to unipolar */
    }

    f_cor = ((1.0 + ((double)gain / (4.0 * (double)65536.0))) *
	    (double)i_data + ((double)offset / 4.0));

    if( range == UNIPOLAR_10 )
     {
	f_cor -= 32768.0;
     }

     if( f_cor < 0.0)		/* round */
	   f_cor -= 0.5;
     else
	   f_cor += 0.5;

    c_blk->cor_buf[channel] = (short)f_cor;
}
