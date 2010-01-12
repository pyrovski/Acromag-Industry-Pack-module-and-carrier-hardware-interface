
#include "../carrier/apc8620.h"
#include "ip340.h"

/*
{+D}
    SYSTEM:	    Library Software - ip340 Board

    MODULE NAME:    build control - build control word for ip340

    VERSION:	    V1.0

    CREATION DATE:  11/09/95

    CODED BY:	    F.J.M.

    ABSTRACT:	    This builds up the control word to be used in
		    converting the ip340 board.

    CALLING
	SEQUENCE:   build_control(ptr);
		    where:
			ptr (pointer to structure)
			    Pointer to the configuration block structure.

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
*/



word build_control(c_blk)

struct cblk340 *c_blk;

{

/*
    declare local storage
*/

    word control;		/* board control register */

/*
    ENTRY POINT OF ROUTINE:
    Build up control word
*/

    control = DISABLE;				/* initialize control register value */

    if( c_blk->scan_mode == SINGLE)		/* SINGLE scan mode bits */
       control |= SINGLE;

    if( c_blk->scan_mode == CONTIN)		/* CONTIN scan mode bits */
       control |= CONTIN;



    if( c_blk->acq_mode == AZ_SELECT )	/* select acquisition mode */
	  control |= (1 << 2);

    if( c_blk->acq_mode == CAL_SELECT )	/* calibration mode */
	   control |= (1 << 3);



    if( c_blk->trigger == TI_SELECT )	/* select input */
	  control |= (1 << 4);


    if( c_blk->trigger == TO_SELECT )	/* select output */
	  control |= (1 << 5);



    if( c_blk->int_mode == INT_FIFO )	/* select interrupt */
	  control |= (1 << 6);

    return(control);			/* return control register */
}




/*
{+D}
    SYSTEM:	    Library Software - ip330 Board

    MODULE NAME:    cnfg330 - configure ip330

    VERSION:	    V1.0

    CREATION DATE:  11/21/95

    CODED BY:	    F.J.M.

    ABSTRACT:	    This configures the ip330 board.

    CALLING
	SEQUENCE:   cnfg330(ptr);
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
*/



void cnfg340(c_blk)

struct cblk340 *c_blk;

{


word build_control();		/* generate control register value */


/*
    ENTRY POINT OF ROUTINE:
*/
    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg, RESET_BOARD);/* stop and clear registers and buffers */
    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg, build_control(c_blk));  /* initialize control register value */

    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->vect_reg, (word)c_blk->vector);
    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->chnl_cntl, c_blk->channel_enable);

    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->timer_low_lsw, (word)c_blk->conv_timer0);
    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->timer_low_msw, (word)(c_blk->conv_timer0 >> 16));

    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->timer_high_lsw, (word)c_blk->conv_timer1);
    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->timer_high_msw, (word)(c_blk->conv_timer1 >> 16));

    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->threshold, c_blk->threshold);

}



