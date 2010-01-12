
#include "../carrier/apc8620.h"
#include "ip330.h"

/*
{+D}
    SYSTEM:	    Library Software - ip330 Board

    MODULE NAME:    calibrate ip330

    VERSION:	    V1.0

    CREATION DATE:  11/29/95

    CODED BY:	    F.J.M.

    ABSTRACT:	    This acquires the auto-zero or the calibration input
		    for the ip330 board.

    CALLING
	SEQUENCE:   calibrate330(ptr,mode);
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



void calibrate330(c_blk, mode)

struct cblk330 *c_blk;
int mode;

{

/*
    declare local storage
*/

    byte temp_int_mode; 	/* mode storage */
    byte temp_mode;		/* mode storage */
    byte temp_gain[2];		/* gain storage */
    byte temp_scan;		/* scan mode storage */
    double f_cor;
    int i, j;
    word *data;

/*
    ENTRY POINT OF ROUTINE:
*/

    temp_gain[0] = get_gain(c_blk, 0); /* save gain code */
    temp_gain[1] = get_gain(c_blk, 1); /* save gain code */
    temp_mode = c_blk->acq_mode;     /* save input mode */
    temp_scan = c_blk->scan_mode;    /* save scan */
    temp_int_mode = c_blk->int_mode; /* save interrupt mode */
    c_blk->int_mode = INT_DIS;       /* disable interrupts */

    if(mode == AZ_SELECT)
      data = c_blk->s_az_buf;	     /* auto-zero data */
    else
      data = c_blk->s_cal_buf;	     /* calibration data */

    c_blk->acq_mode = mode;	     /* input mode */
    c_blk->scan_mode = BU_SNGL;      /* new scan mode */

    for(j = 0; j < 4; j++)           /* acquire samples for each gain */
    {
      /* select auto-zero prior to a change in gain */
      output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg, DISABLE | 0x38);

      put_gain(c_blk, 0, (byte)j);   /* new gain code */
      put_gain(c_blk, 1, (byte)j);   /* new gain code */
      cnfg330(c_blk);                /* configure the board */
      output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->start_ch, 0);  /* start channel */
      output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->end_ch, 1);    /* end channel */
      
      for(f_cor = 0.0, i = 0; i < AVE_SAMPLES; i++)
      {
        output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->strt_reg, 1);  /* start scan */
        while((input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ndr_reg[0]) & 2) == 0); /* wait for data */
        f_cor += (double)input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->data[1]);
      }
      f_cor /= (double)AVE_SAMPLES;

      if( f_cor < 0.0)		/* round */
          f_cor -= 0.5;
      else
          f_cor += 0.5;

      data[j] = (word)f_cor;
    }

    c_blk->int_mode = temp_int_mode;	/* restore changes */
    c_blk->scan_mode = temp_scan;
    c_blk->acq_mode = temp_mode;
    put_gain(c_blk, 0, temp_gain[0]);
    put_gain(c_blk, 1, temp_gain[1]);
}



/*
{+D}
    SYSTEM:	    Library Software - ip330 Board

    MODULE NAME:    convert ip330

    VERSION:	    B

    CREATION DATE:  11/29/95

    CODED BY:	    F.J.M.

    ABSTRACT:	    This acquires the input conversions for the ip330 board.

    CALLING
	SEQUENCE:   convert330(ptr);
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
04/01/09  FJM   add blocking_start_convert

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:
*/



void convert330(c_blk)

struct cblk330 *c_blk;

{

/*
    declare local storage
*/

    int i, j;
    word wait_mask;		/* end of conversion flags */

/*
    ENTRY POINT OF ROUTINE:
*/

    cnfg330(c_blk);		/* configure the board */

    if(c_blk->scan_mode == DISABLE) /* stop scan request */
      return;

    wait_mask = 1 << (c_blk->e_channel & 0xF);

    if((c_blk->acq_mode == DI_SELECT &&
	(c_blk->scan_mode == BU_SNGL || c_blk->scan_mode == UN_SNGL)) ||
        (c_blk->acq_mode != DI_SELECT && c_blk->e_channel < 16))
	j = 0;
    else
	j = 1;

    if(c_blk->int_mode == INT_DIS) /* interrupts disabled */
    {
      /* If the trigger direction is set to output or the scan mode is external */
      /* trigger only, the start conversion register is used to start the scan */
      if(c_blk->trigger == TO_SELECT || c_blk->scan_mode == EX_TRIG)
          output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->strt_reg, 1); /* start scan */

      /* Non-interrupt driven scan modes just poll for the end of scan */
      /* then the data is moved from the board to the raw data array */
      /* Wait for scan complete */
      while((input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ndr_reg[j]) & wait_mask) == 0); /* wait for scan complete */
    }
    else
    {
      printf("blocking_start_convert wait...");
      fflush(stdout);
      /* Interrupt driven scan mode will block until the end of scan */
      /* then the data is moved from the board to the raw data array */
      if(c_blk->trigger == TO_SELECT || c_blk->scan_mode == EX_TRIG)
          blocking_start_convert(c_blk->nHandle, (word*)&c_blk->brd_ptr->strt_reg, 1); /* start scan */

      printf(" done!\n");
    }

/*    Move data flags and data to scan array */

      c_blk->missed_data[0] = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->mdr_reg[0]); /* copy missed data */
      c_blk->missed_data[1] = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->mdr_reg[1]);

      for( i = c_blk->s_channel; i < 32; i++)    /* move data */
         c_blk->s_raw_buf[i] = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->data[i]);
}