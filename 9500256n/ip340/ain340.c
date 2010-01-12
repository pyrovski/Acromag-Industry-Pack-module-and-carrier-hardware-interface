
#include "../carrier/apc8620.h"
#include "ip340.h"

/*
{+D}
    SYSTEM:		   Library Software - ip340 Board

    MODULE NAME:	   calibrate ip340

    VERSION:		   V1.0

    CREATION DATE:	   07/28/99

    CODED BY:		   F.M.

    ABSTRACT:		   This acquires the auto-zero or the calibration input
		      	   for the ip340 board.
    CALLING
	SEQUENCE:	   calibrate340(ptr,mode);
			    where:
				ptr (pointer to structure)
				    Pointer to the configuration block structure.
				mode (int)
				    mode auto-zero or calibration.
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



void calibrate340(c_blk, mode)

struct cblk340 *c_blk;
int mode;

{


/*
    declare local storage
*/

    struct cblk340 temp_blk;
    struct cblk340 *t_blk;
    double f_cor[16];
    sint i, temp_data;
    sint f_samples[16];

/*
    ENTRY POINT OF ROUTINE:
*/

    t_blk = &temp_blk;
    memcpy( t_blk, c_blk, sizeof( temp_blk ) ); /* copy structure */
    memset(&f_cor[0], 0, sizeof(f_cor));	/* zero out */
    memset(&f_samples[0], 0, sizeof(f_samples));

    t_blk->int_mode = INT_DIS;		/* disable interrupts */
    t_blk->threshold = 256;			/* new threshold */
    t_blk->acq_mode = mode;			/* input mode */
    t_blk->scan_mode = CONTIN;		/* scan mode */
    t_blk->channel_enable = 0xFFFF;	/* enable all */
    t_blk->trigger = T_DISABLE;		/* no triggering */

    cnfg340(t_blk);				/* generate control register value */

    output_word(c_blk->nHandle, (word*)&t_blk->brd_ptr->strt_reg, (word) 1); /* start conversions */
    while((input_word(c_blk->nHandle, (word*)&t_blk->brd_ptr->cntl_reg) & 0x200) == 0); /* wait for data */

    output_word(c_blk->nHandle, (word*)&t_blk->brd_ptr->cntl_reg, (input_word(c_blk->nHandle, (word*)&t_blk->brd_ptr->cntl_reg) & 0xFFFC)); /* stop scan */

    while((input_word(c_blk->nHandle,(word*)&t_blk->brd_ptr->cntl_reg) & 0x100) != 0) /* while FIFO not empty get data */
    {
      temp_data = (sint)input_word(c_blk->nHandle, (word*)&t_blk->brd_ptr->fifo_data);   /* must read data first then channel tag */
      i = (sint)input_word(c_blk->nHandle, (word*)&t_blk->brd_ptr->chan_tag) & 0xF;  /* get channel number for data */
      f_cor[i] += (double)temp_data;
	f_samples[i] += 1;				   /* count samples */
    }

    for(i = 0; i < 16; i++)
    {
	f_cor[i] /= (double)f_samples[i];

      if( f_cor[i] < 0.0)		/* round */
 	   f_cor[i] -= 0.5;
      else
	   f_cor[i] += 0.5;

      if(mode == AZ_SELECT)
        c_blk->s_az_buf[i] = (short)f_cor[i];  /* auto-zero data */
      else
        c_blk->s_cal_buf[i] = (short)f_cor[i]; /* calibration data */
    }
}



/*
{+D}
    SYSTEM:	    Library Software - ip340 Board

    MODULE NAME:    convert ip340

    VERSION:	    B

    CREATION DATE:  11/29/95

    CODED BY:	    F.J.M.

    ABSTRACT:	    This acquires the input conversions for the ip340 board.

    CALLING
	SEQUENCE:   convert340(ptr);
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
--------  ----	------------------------------------------------
04/01/09  FJM   blocking_start_convert

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:
*/



void convert340(c_blk)

struct cblk340 *c_blk;

{

    void move_data();

/*
    declare local storage
*/
word i;
/*
    ENTRY POINT OF ROUTINE:
*/

    cnfg340(c_blk); /* configure the board */

    if(c_blk->scan_mode == DISABLE) /* stop scan request */
      return;

    if(c_blk->int_mode == INT_DIS) /* interrupts disabled */
    {
      /* If the trigger direction is set to output or disabled */
      /* start conversion register is used to start the scan */
      if(c_blk->trigger == TO_SELECT || c_blk->trigger == T_DISABLE)
         output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->strt_reg, (word)1); /* start scan */

      /* Non-interrupt driven scan just polls for the end of scan */
      /* then the data is moved from the board to the raw data array */
      /* Wait for scan complete */
      while((input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg) & 0x200) == 0); /* wait for data */
	}
    else
    {
      printf("blocking_start_convert wait...");
      fflush(stdout);
      /* Interrupt driven scan mode will block until the end of scan */
      /* then the data is moved from the board to the raw data array */
      if(c_blk->trigger == TO_SELECT || c_blk->trigger == T_DISABLE)
          blocking_start_convert(c_blk->nHandle, (word*)&c_blk->brd_ptr->strt_reg, (word)1); /* start scan */

      printf(" done!\n");
    }

    /* move data */
    move_data(c_blk);
}






