
#include "../carrier/apc8620.h"
#include "ip320.h"


/*
{+D}
    SYSTEM:         Library Software - ip320 Board

    FILENAME:       build_control.c

    MODULE NAME:    build control - build control word for ip320

    VERSION:        V1.0

    CREATION DATE:  05/19/98

    CODED BY:       F.J.M.

    ABSTRACT:       This builds up the control word to be used in
                    converting the ip320 board.

    CALLING
        SEQUENCE:   build_control(ptr, sa_ptr);
                    where:
                        ptr (pointer to structure)
                            Pointer to the configuration block structure.
                     sa_ptr (pointer to structure)
                            Pointer to the scan array structure.

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
*/



word build_control(c_blk, sa_ptr)

struct cblk320 *c_blk;
struct scan_array *sa_ptr;      /* pointer to channel/gain pairs */

{

/*
    declare local storage
*/

    word control;               /* board control register */
    byte cal_ch;                /* calibration channel number */

/*
    ENTRY POINT OF ROUTINE:
    Initialize local storage.
*/

    control = 0;                /* initialize control register value */

/*
        Process input range, gain and calibration channel info
*/

    switch(c_blk->range)                /* check input range */
    {
        case RANGE_5TO5:
           switch(sa_ptr->gain) /* Av of 1 by default */
           {
                case 2:
                cal_ch = CAL1;          /* number of calibration channel */
                control |= 0x0040;      /* OR in Av = 2 */
                break;

                case 4:
                cal_ch = CAL2;          /* number of calibration channel */
                control |= 0x0080;      /* OR in Av = 4 */
                break;

                case 8:
                cal_ch = CAL3;          /* number of calibration channel */
                control |= 0x00C0;      /* OR in Av = 8 */
                break;

                default:                /* gain = 1 */
                cal_ch = CAL0;          /* number of calibration channel */
                break;
           }
        break;

        default:			/* RANGE_0TO10 or RANGE_10TO10 */
           switch(sa_ptr->gain) /* Av of 1 by default */
           {
                case 2:
                cal_ch = CAL0;          /* number of calibration channel */
                control |= 0x0040;      /* OR in Av = 2 */
                break;

                case 4:
                cal_ch = CAL1;          /* number of calibration channel */
                control |= 0x0080;      /* OR in Av = 4 */
                break;

                case 8:
                cal_ch = CAL2;          /* number of calibration channel */
                control |= 0x00C0;      /* OR in Av = 8 */
                break;

                default:                /* gain = 1 */
                cal_ch = CAL0;          /* number of calibration channel */
                break;
           }
        break;
    }

/*
        Build up control word based on the mode specification
*/

    switch(c_blk->mode)                 /* default to auto-zero mode */
    {
        case DIF:                       /* differential input mode */
           control |= sa_ptr->chn;      /* OR in channel bits */
        break;

        case SEI:                       /* single ended channel mode */
           if(sa_ptr->chn < CAL0)       /* low or high channels */
           {
             control |= SEL_SELECT;     /* OR in single ended low mode bits */
             control |= sa_ptr->chn;    /* OR in channel bits */
           }
           else                         /* single ended high mode */
           {
             control |= SEH_SELECT;     /* OR in single ended high mode bits */
             control |= (sa_ptr->chn - CAL0); /* OR in high channel bits */
           }
        break;

        case CAL:                       /* calibration mode */
           control |= cal_ch;           /* OR in calibration channel bits */
        break;

        default:                        /* auto-zero mode */
           if(c_blk->range == RANGE_0TO10)
                control |= CAL3;        /* select CAL3 */
           else
                control |= AZ_SELECT;   /* OR in auto-zero bits */
        break;
    }
    return(control);                    /* return control register */
}





/*
{+D}
    SYSTEM:         Library Software - ip320 Board

    FILENAME:       ainmc320.c

    MODULE NAME:    ainmc320 - analog input mulit channel

    VERSION:        A

    CREATION DATE:  05/19/98

    CODED BY:       F.J.M.

    ABSTRACT:       This module is used to perform multi channel input
                    conversions for the ip320 board.

    CALLING
        SEQUENCE:   ainmc320(ptr);
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



void ainmc320(c_blk)

struct cblk320 *c_blk;
{


/*
    declare local storage
*/

    struct scan_array *sa_ptr;  /* pointer to channel/gain pairs */
    word *buff_ptr;             /* pointers to current position in buffer */
    float sum_data;             /* sum of all conversions */
    word i, temp;               /* loop control */
    word control_reg;           /* storage for control word of next channel */

/*
    ENTRY POINT OF ROUTINE:
    Initialize local storage.
*/

    sa_ptr = c_blk->sa_start;   /* pointer to channel/gain pairs */

    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg,
		build_control(c_blk, sa_ptr) ); /* initial control register */

    switch(c_blk->mode)				/* select data buffer */
    {
           case AZV:				/* auto-zero */
              buff_ptr = c_blk->s_az_buf;	/* pointer to current position */
           break;

           case CAL:				/* calibration */
              buff_ptr = c_blk->s_cal_buf;	/* pointer to current position */
           break;

           default:				/* differential or single ended input */
              buff_ptr = c_blk->s_raw_buf;	/* pointer to current position */
           break;
    }

/*
        Scan loop
*/

    while(1)
    {
      sum_data = 0.0;
      ++sa_ptr;                 /* point to next channel & gain pair */

/*
        Build the control word for the next channel, while the
        A/D is settling on the the current channel.
*/

      control_reg = build_control(c_blk, sa_ptr); /* control for next channel */

      if((input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg ) & CTRIG ) != 0)/* old data may be present */
        *buff_ptr = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->ai_reg );	 /* read data register to clear flag */

      for(i = 0; i < c_blk->average; i++)
      {
        if(c_blk->trigger == ETRIG)		/* check external trigger */
	  while((input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg ) & CTRIG) == 0); /* wait for trigger */
        else
          output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->strt_reg, 0xFFFF ); /* start conversion (recommended value) */


        for(temp = 0; temp != 11; temp++)	/* no flag to poll, kill some time */
	   input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg );

/*
        Read and sum the data samples
*/

        sum_data += (float)(input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->ai_reg ) & c_blk->data_mask); /* read data register */
      }

/*
        Averaged data if requested
*/

      *buff_ptr++ = (word)(sum_data/(float)c_blk->average);

      if(sa_ptr > c_blk->sa_end)        /* convert all channels in the scan */
        break;

      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg, control_reg );	/* set in new control register */
    }
}



