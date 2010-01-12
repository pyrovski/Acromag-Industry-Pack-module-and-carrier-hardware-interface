
#include "../carrier/apc8620.h"
#include "ip236.h"

/*
{+D}
    SYSTEM:	    Library Software - IP236

    FILENAME:	    wro236.c

    MODULE NAME:    wro236 - write analog output for IP236 board.

    VERSION:	    Rev A

    CREATION DATE:  12-28-98

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M

    ABSTRACT:	    This module is used to perform a single write output function
		    for the IP236 board.
    CALLING
	SEQUENCE:   wro236(c_blk,channel);
		    where:
			c_blk (prt)
			    pointer to configuration structure.
			channel (int)
			    Channel to write to.

    MODULE TYPE:    int

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

    This module is used to perform a single write output function for the
    IP236 board.  A structure pointer to the board memory map, the analog
    output channel number value, and the analog output data value will be
    passed to this routine.  The routine writes the output data to the
    analog output on the board.
*/


void wro236(c_blk, channel)
  int channel;		       /* channel to write */
  struct conf_blk *c_blk;

{
/*
    ENTRY POINT OF ROUTINE:
    Write the output data to the output channel on the board.
*/

/*  use the FIFO full bit to write the maximum number of samples to the buffer */

	if( c_blk->mode[channel] )	/* continuous conversion mode ? */
	{
	    while( !(input_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->start_status ) & ( 1 << channel)))
	    {
		output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->dac_map[channel].fifo_data, *c_blk->current_ptr[channel]++);
		if(c_blk->current_ptr[channel] == c_blk->tail_ptr[channel])
			c_blk->current_ptr[channel] = c_blk->head_ptr[channel];
	    }
	}
	else					/* single */
	{
	    /*  Check FIFO full bit before write */
	    if( !(input_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->start_status ) & ( 1 << channel)))
		output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->dac_map[channel].fifo_data, *c_blk->head_ptr[channel]);
	}
}





/*
{+D}
    SYSTEM:	    Library Software - IP236

    FILENAME:	    wro236.c

    MODULE NAME:    start_single - Start conversion for IP236 board.

    VERSION:	    Rev A

    CREATION DATE:  12-28-98

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M

    ABSTRACT:	    This module is used to start single channel conversions
                    for the IP236 board.

    CALLING
	SEQUENCE:   start_single();

    MODULE TYPE:      void

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

    This module is used to start conversions on the IP236 board.
*/

void start_single(c_blk)
  struct conf_blk *c_blk;

{

/*
    ENTRY POINT OF ROUTINE:
    Write the output data to the output channel on the board
*/

    output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->start_status, (1 << c_blk->current_channel));
}



/*
{+D}
    SYSTEM:	    Library Software - IP236

    FILENAME:	    wro236.c

    MODULE NAME:    start_multiple - Start conversion for IP236 board.

    VERSION:	    Rev A

    CREATION DATE:  12-28-98

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M

    ABSTRACT:	    This module is used to start multiple channel conversions
                    for the IP236 board.

    CALLING
	SEQUENCE:   start_multiple(c_blk, mask);

    MODULE TYPE:      void

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

    This module is used to start conversions on the IP236 board.
*/

void start_multiple(c_blk, mask)
  struct conf_blk *c_blk;
  byte mask;

{

/*
    ENTRY POINT OF ROUTINE:
    Write the output data to the output channel on the board
*/

    output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->start_status, mask );
}



/*
{+D}
    SYSTEM:	    Library Software - IP236

    FILENAME:	    wro236.c

    MODULE NAME:    reset_board - Reset all channels of IP236 board.

    VERSION:	    Rev A

    CREATION DATE:  12-28-98

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M

    ABSTRACT:	    This module is used to reset the IP236 board.

    CALLING
	SEQUENCE:   reset_board(c_blk);

    MODULE TYPE:      void

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

    This module is used to reset the IP236 board.
*/

void reset_board(c_blk)
  struct conf_blk *c_blk;

{
   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->reset_reg, 0xff );
}



/*
{+D}
    SYSTEM:	    Library Software - IP236

    FILENAME:	    wro236.c

    MODULE NAME:    reset_channel - Reset a channel of IP236 board.

    VERSION:	    Rev A

    CREATION DATE:  12-28-98

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M

    ABSTRACT:	    This module is used to reset a channel on the
		    IP236 board.

    CALLING
	SEQUENCE:   reset_channel(c_blk);

    MODULE TYPE:      void

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

    This module is used to reset a channel on the IP236 board.
*/

void reset_channel(c_blk)
  struct conf_blk *c_blk;

{
   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->reset_reg, (1 << c_blk->current_channel));
}




/*
{+D}
    SYSTEM:	    Library Software - IP236

    FILENAME:	    wro236.c

    MODULE NAME:    stop_multiple - Reset channels of IP236 board.

    VERSION:	    Rev A

    CREATION DATE:  12-28-98

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M

    ABSTRACT:	    This module is used to reset channels on the IP236 board.

    CALLING
	SEQUENCE:   stop_multiple(c_blk,mask);

    MODULE TYPE:      void

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

    This module is used to reset one or more channels on the board.
*/

void stop_multiple(c_blk, mask)
  struct conf_blk *c_blk;
  byte mask;

{
   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->reset_reg, mask );	/* reset channel mask */
}
