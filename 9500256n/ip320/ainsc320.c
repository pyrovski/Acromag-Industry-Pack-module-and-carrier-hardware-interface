
#include "../carrier/apc8620.h"
#include "ip320.h"

/*
{+D}
    SYSTEM:         Library Software - ip320 Board 9500-064

    FILENAME:       ainsc320.c

    MODULE NAME:    ainsc320 - analog input single channel

    VERSION:        V1.0

    CREATION DATE:  05/19/98

    CODED BY:       F.J.M.

    ABSTRACT:       This module is used to perform single channel input
		    conversions for the ip320 board.

    CALLING
        SEQUENCE:   ainsc320(ptr);
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

  DATE    BY        PURPOSE
-------  ----   ------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

    This module is used to perform the "read input" function for the
    ip320 board.  A pointer to the Configuration Block will be passed to
    this routine.  The routine will use a pointer within the Configuration
    Block to reference the registers on the Board.
*/



void ainsc320(c_blk)

struct cblk320 *c_blk;
{

/*
    declare local storage
*/
    word *buff_ptr;		/* pointer to current position in buffer */
    float sum_data;		/* sum of all conversions */
    word control;		/* board control register */
    byte cal_ch;		/* calibration channel number */
    word i, temp;		/* loop control */

/*
    ENTRY POINT OF ROUTINE:
    Initialize local storage.
*/

    control = 0;		/* initialize control register value */
/*
	Process input range, gain and calibration channel info
*/

    switch(c_blk->range)		/* check input range */
    {
	case RANGE_5TO5:
	   switch(c_blk->gain)		/* Av of 1 by default */
    	   {
		case 2:
        	cal_ch = CAL1;		/* number of calibration channel */
	  	control |= 0x0040;	/* OR in Av = 2 */
	  	break;

		case 4:
		cal_ch = CAL2;		/* number of calibration channel */
		control |= 0x0080;	/* OR in Av = 4 */
		break;

		case 8:
		cal_ch = CAL3;		/* number of calibration channel */
		control |= 0x00C0;	/* OR in Av = 8 */
		break;

		default:		/* default gain = 1 */
        	cal_ch = CAL0;		/* number of calibration channel */
		break;
	   }
	break;

	default:			/* RANGE_0TO10 or RANGE_10TO10 */
	   switch(c_blk->gain)		/* Av of 1 by default */
    	   {
		case 2:
        	cal_ch = CAL0;		/* number of calibration channel */
	  	control |= 0x0040;	/* OR in Av = 2 */
	  	break;

		case 4:
		cal_ch = CAL1;		/* number of calibration channel */
		control |= 0x0080;	/* OR in Av = 4 */
		break;

		case 8:
		cal_ch = CAL2;		/* number of calibration channel */
		control |= 0x00C0;	/* OR in Av = 8 */
		break;

		default:		/* default gain = 1 */
        	cal_ch = CAL0;		/* number of calibration channel */
		break;
	   }
	   break;
    }

/*
	Build up control word based on the mode specification
*/

    switch(c_blk->mode)
    {
	case DIF:			/* differential input mode */
	   control |= c_blk->channel;	/* OR in channel bits */
	   buff_ptr = c_blk->s_raw_buf;
	   break;

	case SEI:			/* single ended channel mode */
	   if(c_blk->channel < CAL0)	/* low or high channels */
	   {
	     control |= SEL_SELECT;	/* OR in single ended low mode bits */
	     control |= c_blk->channel;	/* OR in channel bits */
	   }
	   else				/* single ended high mode */
	   {
	     control |= SEH_SELECT;	/* OR in single ended high mode bits */
	     control |= (c_blk->channel - CAL0); /* OR in high channel bits */
	   }
	   buff_ptr = c_blk->s_raw_buf;
	   break;

	case CAL:			/* calibration mode */
	   control |= cal_ch;		/* OR in calibration channel bits */
	   buff_ptr = c_blk->s_cal_buf;
	   break;

	default:			/* auto-zero mode */
	   if(c_blk->range == RANGE_0TO10)
		control |= CAL3;	/* select CAL3 */
	   else
	        control |= AZ_SELECT;	/* OR in auto-zero bits */

	   buff_ptr = c_blk->s_az_buf;
	   break;
    }

    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg, control );	/* write out control register */

    sum_data = 0.0;

    if((input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg ) & CTRIG) != 0) /* old data may be present */
      temp = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->ai_reg );	       /* read data register to clear flag */

    for(i = 0; i < c_blk->average; i++)
    {
      if(c_blk->trigger == ETRIG)		/* check external trigger */
	while((input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg ) & CTRIG) == 0); /* wait for trigger */
      else
        output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->strt_reg, 0xFFFF ); /* start conversion (recommended value) */

      for(temp = 0; temp != 11; temp++)		/* no flag to poll, kill some time */
	input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg );

      sum_data += ((float) (input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->ai_reg ) & c_blk->data_mask)); /* read data register */
    }

    sum_data = (sum_data / (float)c_blk->average);

    if( sum_data < 0.0)		/* round */
        sum_data -= 0.5;
    else
       sum_data += 0.5;

    *buff_ptr = (word)sum_data;
}


