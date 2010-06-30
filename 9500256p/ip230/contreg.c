
#include "../carrier/apc8620.h"
#include "ip230.h"

/*
{+D}
    SYSTEM:	    IP230/235 Software

    FILENAME:	    contreg.c

    MODULE NAME:    wav_mem_sel - routine to setup the control register.

    VERSION:	    REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is the setup routine for setting up the
		    control register.

    CALLING
	SEQUENCE:   wav_mem_sel(chan, c_blk)
			where:
			chan - is the channel of
		    memory that is to be read from or written to.
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

       This module is the setup routine for the control register.
*/

void wav_mem_sel(chan, c_blk)
  int chan;  /* channel number */
  struct cblk230 *c_blk;
{


/*
    Declare local data areas
*/

    word control;

/*
    Start Of Routine
*/
	    control = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg );

	    control &= 0xfff8; /* clear out the three bits default to chan 0 */

	    if(chan == 1)
	      control |= 0x0001;

	    if(chan == 2)
	      control |= 0x0002;

	    if(chan == 3)
	      control |= 0x0003;

	    if(chan == 4)
	      control |= 0x0004;

	    if(chan == 5)
	      control |= 0x0005;

	    if(chan == 6)
	      control |= 0x0006;

	    if(chan == 7)
	      control |= 0x0007;

	    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg, control );
      }




/*
{+D}
    SYSTEM:	    IP230/235 Software

    FILENAME:	    contreg.c

    MODULE NAME:    wav_mem_inc - routine to setup the control register.

    VERSION:	    REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is the setup routine for setting up the control register.

    CALLING
	SEQUENCE:   wav_mem_inc(choice, c_blk)
			where:
			choice - selection.
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

       This module is the setup routine for the control register.
*/

void wav_mem_inc(choice, c_blk)
  int choice;	   /* selection that was made */
  struct cblk230 *c_blk;

{

/*
    Declare local data areas
*/

    word control;


/*
    Start Of Routine
*/

	  control = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg );

	  control &= 0xffef; /* clear out bit in question default to 2 */

	  if(choice == 1)
	    control |= 0x0010;

	  output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg, control );
}




/*
{+D}
    SYSTEM:	    IP230/235 Software

    FILENAME:	    contreg.c

    MODULE NAME:    trig_mode - routine to setup the control register.

    VERSION:	    REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is the setup routine for setting up
                    the control register.

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

       This module is the setup routine for the control register.
*/

void trig_mode(choice, c_blk)
  int choice;
  struct cblk230 *c_blk;
{

/*
    Declare local data areas
*/

    word control;

/*
    Start Of Routine
*/

	  control = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg );

	  control &= 0xff9f;	/* default choice == 1 do nothing bits are clear */

	  if(choice == 2)
	    control |= 0x0020;

	  if( choice == 3 )
	    control |= 0x0040;

	  output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg, control );

}





/*
{+D}
    SYSTEM:	    IP230/235 Software

    FILENAME:	    contreg.c

    MODULE NAME:    conv_mode - routine to setup the control register.

    VERSION:	    REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is the setup routine for setting up the control register.

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

       This module is the setup routine for the control register.
*/

void conv_mode(choice, c_blk)
  int choice;
  struct cblk230 *c_blk;
{

/*
    Declare local data areas
*/

    word control;

/*
    Start Of Routine
*/

	  control = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg );

	  control &= 0xf8ff;	/* default choice == 1 do nothing, bits are clear */

	  if(choice == 2)
	    control  |= 0x0100;

	  if(choice == 3)
	    control  |= 0x0200;

	  if(choice == 4)
	    control  |= 0x0300;

	  if(choice == 5)
	    control  |= 0x0400;

	  if(choice == 6)
	    control  |= 0x0500;

	  write_vector(c_blk);
	  output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg, control );
}


/*
{+D}
    SYSTEM:	    IP230/235 Software

    FILENAME:	    contreg.c

    MODULE NAME:    soft_reset - routine to setup the control register.

    VERSION:	    REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is the setup routine for setting up the
                    control register.

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

       This module is the setup routine for the control register.
*/

void soft_reset(c_blk)
  struct cblk230 *c_blk;

{
    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg,
                 input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cont_reg) | 0x8000 );
}




/*
{+D}
    SYSTEM:	    IP230/235 Software

    FILENAME:	    contreg.c

    MODULE NAME:    range_sel - routine to setup the control register.

    VERSION:	    REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is the setup routine for setting up
                    the control register.

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

       This module is the setup routine for the control register.
*/

void range_sel(c_bk, chan, choice)
   struct cblk230 *c_bk;   /* path to configuration block */
   int chan;	 /* channel number to use */
   int choice;	 /* range selection */

{
	  c_bk->range[chan] = choice;
}





/*
{+D}
    SYSTEM:	    IP230/235 Software

    FILENAME:	    contreg.c

    MODULE NAME:    write_vector - routine to write the interrupt vector to the
				   interrupt vector register on the board.

    VERSION:	    REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is the routine to write the interrupt vector to the
		    interrupt vector register on the board.

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

    This module is the routine to write the interrupt vector to the interrupt
    vector register on the board.
*/

void write_vector(c_blk)
   struct cblk230 *c_blk;   /* path to configuration block */

{
  output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->int_vect, c_blk->interrupt_vector );
}


