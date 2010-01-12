
#include "../carrier/apc8620.h"
#include "ip230.h"

/*
{+D}
    SYSTEM:	    Library Software - ip230 Board

    FILENAME:	    rmid230.c

    MODULE NAME:    rcmd230 - read IP230 I.D..

    VERSION:	    V1.0

    CREATION DATE:  11-14-96

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is used to read I.D. prom of the ip230 board.

    CALLING
	SEQUENCE:   rmid230(ptr);
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

    This module is used to perform the read the I.D. A pointer to the
    Configuration Block will be passed to this routine.  The routine will
    use a pointer within the Configuration Block together with offsets to
    reference the registers on the Board and will transfer information from
    the Board to the Configuration Block.
*/



void rmid230(c_blk)
struct cblk230 *c_blk;
{

/*
    declare local storage
*/

	int i;	            /* loop index */
	word idProm[12];    /* holds ID prom */

/*
    ENTRY POINT OF ROUTINE
    read board id information
*/

    ReadIpackID(c_blk->nHandle, c_blk->slotLetter, &idProm[0], sizeof idProm / 2 ); /* read from carrier */

    for(i = 0; i < 12; i++ )
 	   c_blk->id_prom[i] = (byte)idProm[i];
}


/*
{+D}
    SYSTEM:         Library Software - ip230 Board

    FILENAME:       rcc230.c

    MODULE NAME:    rcc230 - read IP230 calibration coefficients.

    VERSION:        V1.0

    CREATION DATE:  11-14-96

    DESIGNED BY:    R.H.

    CODED BY:       R.H.
    
    ABSTRACT:       This module is used to read calibration
                    coefficients ip230 board.

    CALLING
        SEQUENCE:   rcc230(ptr);
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

  DATE    BY        PURPOSE
-------  ----   ------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

    This module is used to perform the read the I.D. and calibration
    coefficients for the ip230 board.  A pointer to the Configuration
    Block will be passed to this routine.  The routine will use a pointer
    within the Configuration Block together with offsets to reference the
    registers on the Board and will transfer information from the
    Board to the Configuration Block.
*/



void rcc230(c_blk, channels)
struct cblk230 *c_blk;
int channels;
{

/*
    declare local storage
*/

    word cmd;
    word off_lsb;
    word off_msb;
    word gain_lsb;
    word gain_msb;
    int i;                      /* loop index */
    word j;

/*
    ENTRY POINT OF ROUTINE:
    read gain/offset calibration information

    read offset
*/

    j = 0x0000;
    for(i = 0; i < channels; i++)   /* get bipolar 10v coef. now */
    {
      cmd = 0x8000 | j;
      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      off_msb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;

      j += 0x0100;
      cmd = 0x8000 | j;
      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      off_lsb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;

      c_blk->coef_bufb10[i][0] = off_msb | (off_lsb >> 8);


/* read gain */

      j += 0x0100;
      cmd = 0x8000 | j;
      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      gain_msb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;

      j += 0x0100;
      cmd = 0x8000 | j;

      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd);

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      gain_lsb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;

      c_blk->coef_bufb10[i][1] = gain_msb | (gain_lsb >> 8);

      j += 0x0100;
    }
      



    j = 0x2000;
    for(i = 0; i < channels; i++)   /* get bipolar 5v coef. now */
    {
      cmd = 0x8000 | j;
      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      off_msb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;
      
      j += 0x0100;
      cmd = 0x8000 | j;
      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      off_lsb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;

      c_blk->coef_bufb5[i][0] = off_msb | (off_lsb >> 8);
      

      j += 0x0100;
      cmd = 0x8000 | j;
      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      gain_msb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;

      j += 0x0100;
      cmd = 0x8000 | j;
      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      gain_lsb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;

      c_blk->coef_bufb5[i][1] = gain_msb | (gain_lsb >> 8);

      j += 0x0100;
    }


    j = 0x4000;
    for(i = 0; i < channels; i++)   /* get unipolar 10v coef. now */
    {
      cmd = 0x8000 | j;
      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      off_msb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;
      
      j += 0x0100;
      cmd = 0x8000 | j;

      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      off_lsb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;

      c_blk->coef_bufu10[i][0] = off_msb | (off_lsb >> 8);
      

      j += 0x0100;
      cmd = 0x8000 | j;

      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while( ( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      gain_msb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;

      j += 0x0100;
      cmd = 0x8000 | j;

      output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd );

      while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 1 ) == 0 );	/* wait for flag */

      gain_lsb = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 0xff00;

      c_blk->coef_bufu10[i][1] = gain_msb | (gain_lsb >> 8);

      j += 0x0100;
    }
 }


