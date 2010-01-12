
#include "../carrier/apc8620.h"
#include "ip330.h"

/*
{+D}
    SYSTEM:	    Library Software - ip330 Board

    FILENAME:	    mccd330.c

    MODULE NAME:    mccd330 - analog multi channel input corrected data

    VERSION:	    Rev A

    CREATION DATE:  11/07/95

    CODED BY:	    F.J.M.

    ABSTRACT:	    This module is used to correct input conversions
		    for the ip330 board.

    CALLING
	SEQUENCE:   int mccd330(ptr);
		    where:
			return (int)
			    -1 = Divide by zero detected 0 otherwise.
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

    This module is used to correct the input data for the
    ip330 board.  A pointer to the Configuration Block will be passed to
    this routine.  The routine will use a pointer within the Configuration
    Block to reference the registers on the Board.
*/



int mccd330(c_blk)

struct cblk330 *c_blk;
{

/*
    declare local storage
*/

    word *raw_buf;		/* raw buffer */
    word *az_buf;		/* auto zero buffer */
    word *cal_buf;		/* calibration buffer */
    int *cor_buf;		/* corrected buffer */
    double i_span;              /* ideal span value */
    double i_zero;              /* ideal zero value */
    double calhi;               /* high calibration input voltage */
    double callo;               /* low calibration input voltage */
    double slope;               /* slope from equation 2 */
    double temp;
    int chan, loop;             /* current channel and conversion loop */
    byte ch_gain, av;		/* channel gain setting */

/*
    ENTRY POINT OF ROUTINE:
    Initialize variables
*/

    az_buf  = c_blk->s_az_buf;  /* pointer to buffer */
    raw_buf = c_blk->s_raw_buf; /* pointer to buffer */
    cal_buf = c_blk->s_cal_buf; /* pointer to buffer */
    cor_buf = c_blk->s_cor_buf; /* pointer to buffer */

    chan = loop = c_blk->s_channel; /* start channel/conversion loop */

    while(1)			/* correct data in scan array */
    {
/*
    In differential mode the input gains for channels 0-F are also used
    for channels 10-1F
*/
      if(c_blk->acq_mode == DI_SELECT && loop > 0xf)
        chan &= 0xf;

      /* read channel gain */
      ch_gain = get_gain(c_blk, chan) & 3;

      if((cal_buf[ch_gain] - az_buf[ch_gain]) == 0) /* check for divide by zero */
         return(-1);                              /* if so, abort */
/*
    Select calibration voltages and ideal zero and span values
*/

      callo = 0.0;
      switch(c_blk->range)
     {
	case RANGE_0TO5:
	   switch(ch_gain)
	   {
		case GAIN_X2:
		   callo = 0.6125;
		   calhi = 2.4500;
		break;

		case GAIN_X4:
		   callo = 0.6125;
		   calhi = 1.2250;
		break;

		case GAIN_X8:
		   calhi = 0.6125;
		break;

		default:		/* GAIN_X1 */
		   callo = 0.6125;
		   calhi = 4.9000;
		break;
	   }
	   i_zero = 0.0000;
	   i_span = 5.0000;
	break;

	case RANGE_5TO5:
	   switch(ch_gain)
	   {
		case GAIN_X2:
		   calhi = 2.4500;
		break;

		case GAIN_X4:
		   calhi = 1.2250;
		break;

		case GAIN_X8:
		   calhi = 0.6125;
		break;

		default:		/* GAIN_X1 */
		   calhi = 4.9000;
		break;
	   }
	   i_zero = -5.0000;
	   i_span = 10.0000;
	break;

	case RANGE_0TO10:
	   callo = 0.6125;
	   switch(ch_gain)
	   {
		case GAIN_X4:
		   calhi = 2.4500;
		break;

		case GAIN_X8:
		   calhi = 1.2250;
		break;

		default: 		/* GAIN_X1 or GAIN_X2 */
		   calhi = 4.9000;
		break;
	   }
	   i_zero =  0.0;
	   i_span = 10.0000;
	break;

	default:			/* RANGE_10TO10 */
	   switch(ch_gain)
	   {
		case GAIN_X4:
		   calhi = 2.4500;
		break;

		case GAIN_X8:
		   calhi = 1.2250;
		break;

		default:		/* GAIN_X1 or GAIN_X2 */
		   calhi = 4.9000;
		break;
	   }
	   i_zero = -10.0000;
	   i_span =  20.0000;
	break;
      }

    av = (1 << ch_gain);		/* convert ch_gain 0,1,2,3 to 1,2,4,8 format */
    slope = ((double)av) * (calhi - callo) / (double)(cal_buf[ch_gain] - az_buf[ch_gain]);

    temp = (((double)CON16 * slope) / i_span) * ((double)raw_buf[loop] +
           (((callo * (double)av) - i_zero) / slope) - (double)az_buf[ch_gain]);

    if( temp < 0.0)		/* round */
        temp -= 0.5;
    else
        temp += 0.5;

    cor_buf[loop] = (int)temp;

    chan++;
    if(++loop > 31)                     /* convert channels in array */
	break;
    }
    return(0);
}

