
#include "../carrier/apc8620.h"
#include "ip330.h"

/*
{+D}
    SYSTEM:	    Library Software - ip330 Board

    MODULE NAME:    build control - build control word for ip330

    VERSION:	    V1.0

    CREATION DATE:  11/09/95

    CODED BY:	    F.J.M.

    ABSTRACT:	    This builds up the control word to be used in
		    converting the ip330 board.

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

struct cblk330 *c_blk;

{

/*
    declare local storage
*/

    word control;		/* board control register */

/*
    ENTRY POINT OF ROUTINE:
    Build up control word
*/

    control = DISABLE;		/* initialize control register value */

    if(c_blk->data_format == SB_SELECT) /* select straight binary */
       control |= 2;

    if(c_blk->trigger == TO_SELECT)	/* select trigger output */
       control |= 4;


    switch(c_blk->acq_mode)	/* select acquisition mode */
    {
      case SE_SELECT:
	 control |= (1 << 3);
      break;

      case AZ_SELECT:		/* select auto zero */
	 switch(c_blk->range)
	 {
	   case RANGE_0TO10:	/* range 0 to 10 */
	      control |= (6 << 3);
	   break;

	   case RANGE_0TO5:	/* range 0 to 5 */
             if(get_gain(c_blk, 1) == GAIN_X8) /* gain of ch 1 */
	       control |= (7 << 3);
	     else		/* Av = 1, 2, or 4 */
	       control |= (6 << 3);
	    break;

	   default:
		control |= (7 << 3);
	   break;
	 }
      break;

      case CAL_SELECT:		/* calibration mode */
	 switch(c_blk->range)
	 {
	   case RANGE_5TO5:	/* range -5 to 5 */
	   case RANGE_0TO5:	/* range 0 to 5 */
              switch(get_gain(c_blk,1)) /* gain of ch 1 */
	      {
		case GAIN_X2:	  /* Av = 2 */
		   control |= (4 << 3);
		break;

		case GAIN_X4:	  /* Av = 4 */
		   control |= (5 << 3);
		break;

		case GAIN_X8:	  /* Av = 8 */
		   control |= (6 << 3);
		break;

		default:	  /* Av = 1 */
		   control |= (3 << 3);
		break;
	      }
	   break;

	   case RANGE_10TO10:	/* range -10 to 10 */
	   case RANGE_0TO10:	/* range 0 to 10 */
              switch(get_gain(c_blk,1)) /* gain of ch 1 */
	      {
		case GAIN_X4:	  /* Av = 4 */
		   control |= (4 << 3);
		break;

		case GAIN_X8:	  /* Av = 8 */
		   control |= (5 << 3);
		break;

		default:	  /* Av = 1 or 2 */
		   control |= (3 << 3);
		break;

	      }
	   break;
	 }
      break;

      default:			/* differential input is default */
      break;
    }


    control |= (c_blk->scan_mode << 8); /* scan mode bits */

    if(c_blk->timer_en == TIMER_ON)	/* select timer enable */
       control |= 0x0800;

    control |= (c_blk->int_mode << 12); /* interrupt mode bits */

    return(control);			/* return control register */
}




/*
{+D}
    SYSTEM:	    Library Software - ip330 Board

    MODULE NAME:    put_gain

    VERSION:	    V1.0

    CREATION DATE:  11/29/95

    CODED BY:	    F.J.M.

    ABSTRACT:       This writes the channel gain to the ip330 board.

    CALLING
        SEQUENCE:   put_gain(cblk,channel,gain);
		    where:
			ptr (pointer to structure)
                            Pointer to the configuration structure.
                        channel (int)
                            channel number to write.
                        gain (byte)
                            gain value to write.
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



void put_gain(c_blk,channel,gain)

struct cblk330 *c_blk;
int channel;
byte gain;

{

/*
    declare local storage
*/

    byte *gptr;

/*
    ENTRY POINT OF ROUTINE:
*/

    gptr = &c_blk->brd_ptr->gain[channel];  /* address of element */


#ifdef SELECT_BIG_ENDIAN	/* Big endian */
			/* no need to correct Big endian gain channel boundry */
#else			/* Little endian - must correct gain channel boundry */
    if((unsigned long)gptr & 1)	/* check for odd/even boundry */
       gptr--;
    else
       gptr++;

#endif			/* SELECT_BIG_ENDIAN */


    output_byte(c_blk->nHandle, (byte*)gptr, (gain & 3));       /* write gain code */
}




/*
{+D}
    SYSTEM:	    Library Software - ip330 Board

    MODULE NAME:    get_gain

    VERSION:	    V1.0

    CREATION DATE:  11/29/95

    CODED BY:	    F.J.M.

    ABSTRACT:       This reads the channel gain to the ip330 board.

    CALLING
        SEQUENCE:   return get_gain(cblk,channel);
		    where:
                        return (byte)
                            The gain value read.
			ptr (pointer to structure)
                            Pointer to the configuration structure.
                        channel (int)
                            channel number to read.

    MODULE TYPE:    byte

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



byte get_gain(c_blk,channel)

struct cblk330 *c_blk;
int channel;


{

/*
    declare local storage
*/

    byte *gptr;

/*
    ENTRY POINT OF ROUTINE:
*/

    gptr = &c_blk->brd_ptr->gain[channel];  /* address of element */

#ifdef SELECT_BIG_ENDIAN	/* Big endian */
			/* no need to correct Big endian gain channel boundry */
#else			/* Little endian - must correct gain channel boundry */
    if((unsigned long)gptr & 1)	/* select odd/even boundry */
       gptr--;
    else
       gptr++;

#endif			/* SELECT_BIG_ENDIAN */

    return((input_byte(c_blk->nHandle, (byte*)gptr) & 3)); /* read & return gain code */
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



void cnfg330(c_blk)

struct cblk330 *c_blk;

{

/*
    ENTRY POINT OF ROUTINE:
*/

    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg, DISABLE | 0x38); /* stop all scanning */
    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg, build_control(c_blk)); /* initialize control register value */
    output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->vect_reg, c_blk->vector);
    output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->start_ch, c_blk->s_channel);
    output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->end_ch, c_blk->e_channel);
    output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->tpc_reg, c_blk->timer_ps);
    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ctc_reg, c_blk->conv_timer);
}

