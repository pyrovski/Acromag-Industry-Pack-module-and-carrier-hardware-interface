
#include "../carrier/apc8620.h"
#include "ip230.h"

/*
{+D}
    SYSTEM:	    Library Software - ip230 Board

    FILENAME:	    wcc230.c

    MODULE NAME:    wcc230 - write IP230 calibration coefficients.

    VERSION:	    V1.0

    CREATION DATE:  05/19/98

    DESIGNED BY:    R.H.

    CODED BY:	    R.H.

    ABSTRACT:	    This module is used to write calibration coefficients.

    CALLING
	SEQUENCE:   wcc230(ptr);
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

    This module is used to perform writes to the calibration
    coefficients for the ip230 board.  A pointer to the Configuration
    Block will be passed to this routine.  The routine will use a pointer
    within the Configuration Block together with offsets to reference the
    registers on the Board and will transfer information from the
    Board to the Configuration Block.
*/



void wcc230(c_blk, channels, data, off_gain)
struct cblk230 *c_blk;
int channels;
int data;
int off_gain;
{

/*
    declare local storage
*/

    int cmd;
    int msb;
    int lsb;
    int i, j, k;
/*
    ENTRY POINT OF ROUTINE:

    read gain/offset calibration information
*/

    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->reserved, 0xA300 );	/* magic number to enable writes */

    msb = data >> 8;
    lsb = data & 0x00ff;

    k = 0;

    switch ( c_blk->range[channels] )
    {
      case BIPOLAR_5:
      {
       switch(channels)
       {
	 case 1:
	 j = 0x2400;
	 break;

	 case 2:
	 j = 0x2800;
	 break;

	 case 3:
	 j = 0x2c00;
	 break;

	 case 4:
	 j = 0x3000;
	 break;

	 case 5:
	 j = 0x3400;
	 break;

	 case 6:
	 j = 0x3800;
	 break;

	 case 7:
	 j = 0x3c00;
	 break;

	 default:			/* ch 0 default */
	 j = 0x2000;
	 break;
       }
       break;
      }

      case UNIPOLAR_10:
      {
       switch(channels)
       {
	 case 1:
	 j = 0x4400;
	 break;

	 case 2:
	 j = 0x4800;
	 break;

	 case 3:
	 j = 0x4c00;
	 break;

	 case 4:
	 j = 0x5000;
	 break;

	 case 5:
	 j = 0x5400;
	 break;

	 case 6:
	 j = 0x5800;
	 break;

	 case 7:
	 j = 0x5c00;
	 break;

	 default:			/* ch 0 default */
	 j = 0x4000;
	 break;
       }
       break;
      }
      default: 				/* BIPOLAR_10 */
      {
        switch(channels)
        {
	  case 1:
	  j = 0x0400;
	  break;

	  case 2:
	  j = 0x0800;
	  break;

	  case 3:
	  j = 0x0c00;
	  break;

	  case 4:
	  j = 0x1000;
	  break;

	  case 5:
	  j = 0x1400;
	  break;

	  case 6:
	  j = 0x1800;
	  break;

	  case 7:
	  j = 0x1c00;
	  break;

	  default:			/* ch 0 default */
	  j = 0x0000;
	  break;
	}
      }
      break;
    }

    if(off_gain == 2)	    /* add offset for gain coef */
     k = 0x0200;

    i = j | k;	 	    /* OR the intial address and the offset together */

    cmd = i | msb;	    /* Or in the data to complete the command */
    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd ); /* write it out to the calibration access register */

    while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 2 ) == 0 );	/* wait for flag */


/* LSByte */

    i += 0x0100;	/* increment the address by 100H */
    cmd = i | lsb;	/* OR in the LSB data and write it out */

    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, cmd ); /* write it out to the calibration access register */

    while(( input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat ) & 2 ) == 0 );	/* wait for flag */

    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->reserved, 0 );	/* disable writes */
  }
