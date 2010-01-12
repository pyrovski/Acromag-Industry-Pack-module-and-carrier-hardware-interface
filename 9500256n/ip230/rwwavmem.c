
#include "../carrier/apc8620.h"
#include "ip230.h"


/*
{+D}
    SYSTEM:	    IP230/235 Software

    FILENAME:	    rwwavmem.c

    MODULE NAME:    wav_mem - routine to do reads and writes to the
		    waveform memory.

    VERSION:	    REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is the routine that does reads and
		    writes to the waveform memory.

    CALLING
	SEQUENCE:

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

       This module is the routine that performs reads and writes to the
       waveform memory.
*/

word wav_mem(rw, addr, wdata, c_blk)
   int rw;		/* write 0 = read, 1 = write */
   word addr;		/* address to write to */
   word wdata;		/* data to be written to a memory location */
   struct cblk230 *c_blk;

{

/*
    Start Of Routine
*/

   addr <<= 1;

   if(rw == 1)	/* write */
   {
    if((input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg ) & 0x0010 ) == 0 )
      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->wav_mem_add, addr );

    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->wav_mem_dat, wdata );
    return(wdata);
   }
   else		/* read */
   {
    if((input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg ) & 0x0010 ) == 0 )
      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->wav_mem_add, addr );

    return( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->wav_mem_dat ));		/* read */
   }
}

