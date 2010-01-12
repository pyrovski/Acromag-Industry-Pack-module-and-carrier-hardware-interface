
#include "../carrier/apc8620.h"
#include "ip236.h"

/*
{+D}
    SYSTEM:	    Library Software - ip236 Board

    FILENAME:	    cd236.c

    MODULE NAME:    cd236 - analog output corrected data

    VERSION:	    Rev A

    CREATION DATE:  11/18/96

    CODED BY:	    FJM

    ABSTRACT:	    This module is used to correct output conversions
		    for the ip236 board.

    CALLING
	SEQUENCE:   cd236(ptr);
			where:
			ptr (pointer to structure)
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

    This module is used to correct the input data for the ip236 board.
    A pointer to the Configuration Block will be passed to this routine.
    The routine will use a pointer within the Configuration Block to
    reference the registers on the Board.
*/


void cd236(c_blk)

struct conf_blk *c_blk;

{

/*
    declare local storage
*/

    double f_cor;
    int i_data;
    short offset;
    short gain;
    int i;

/*
    ENTRY POINT OF ROUTINE:
    Initialize variables
*/

    for( i = 0; i < MAXSIZE; i++)
    {
      i_data = (int)c_blk->ideal_data[c_blk->current_channel][i];

      switch(c_blk->range[c_blk->current_channel])
      {
 	case BIPOLAR_5:
	     offset = c_blk->coef_bufb5[c_blk->current_channel][0];
	     gain   = c_blk->coef_bufb5[c_blk->current_channel][1];
	break;

 	case UNIPOLAR_10:
	      offset = c_blk->coef_bufu10[c_blk->current_channel][0];
	      gain   = c_blk->coef_bufu10[c_blk->current_channel][1];
		i_data &= 0xFFFF;		/* remove sign extension for unipolar only*/
		i_data ^= 0x8000;		/* convert bipolar to unipolar */
	break;

	default:	/* BIPOLAR_10 */
	     offset = (short)c_blk->coef_bufb10[c_blk->current_channel][0];
	     gain   = (short)c_blk->coef_bufb10[c_blk->current_channel][1];
	
	break;
      }

      f_cor = ((1.0 + ((double)gain / (4.0 * (double)65536.0))) *
	    (double)i_data + ((double)offset / 4.0));

      if( f_cor < 0.0)		/* round */
 	   f_cor -= 0.5;
      else
	   f_cor += 0.5;


      if( c_blk->range[c_blk->current_channel] == UNIPOLAR_10 )
          f_cor -= 32768.0;


       if( f_cor > 32767.0 )	/* clip high */
	     f_cor = 32767.0;

       if( f_cor < -32768.0 )	/* clip low */
	     f_cor = -32768.0;

      c_blk->cor_data[c_blk->current_channel][i] = (short)f_cor;
    }
}
