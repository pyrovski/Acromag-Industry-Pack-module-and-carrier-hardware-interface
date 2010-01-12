

#include "../carrier/apc8620.h"
#include "ip320.h"

/*
{+D}
    SYSTEM:         Library Software - ip320 Board

    FILENAME:       sccd320.c

    MODULE NAME:    sccd320 - analog single channel input corrected data

    VERSION:        Rev A

    CREATION DATE:  05/19/98

    CODED BY:       F.J.M.

    ABSTRACT:       This module is used to correct input conversions
		    for the ip320 board.

    CALLING
        SEQUENCE:   sccd320(ptr);
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

    This module is used to correct the input data for the
    ip320 board.  A pointer to the Configuration Block will be passed to
    this routine.  The routine will use a pointer within the Configuration
    Block to reference the registers on the Board.
*/



void sccd320(c_blk)

struct cblk320 *c_blk;
{

/*
    declare local storage
*/

    word *raw_buf;		/* raw buffer */
    word *az_buf;		/* auto zero buffer */
    word *cal_buf;		/* calibration buffer */
    int *cor_buf;		/* corrected buffer */
    float i_span;		/* ideal span value */
    float i_zero;		/* ideal zero value */
    float calhi;		/* high calibration input voltage */
    float callo;		/* low calibration input voltage */
    float slope;		/* slope from equation 2 */
    float temp;

/*
    ENTRY POINT OF ROUTINE:
    Initialize variables
*/

    az_buf  = c_blk->s_az_buf;	/* pointer to current position */
    raw_buf = c_blk->s_raw_buf;	/* pointer to current position */
    cal_buf = c_blk->s_cal_buf; /* pointer to current position */
    cor_buf = c_blk->s_cor_buf; /* pointer to current position */

/*
    Select calibration voltages and ideal zero and span values
*/

    callo = 0.0;
    switch(c_blk->range)
    {
	case RANGE_5TO5:
	   switch(c_blk->gain)
    	   {
		case 2:
        	   calhi = 2.4500;
	  	break;

		case 4:
        	   calhi = 1.2250;
		break;

		case 8:
        	   calhi = 0.6125;
		break;

		default:		/* 1 */
        	   calhi = 4.9000;
		break;

	   }
	   i_zero = -5.0000;
	   i_span = 10.0000;
	break;

	case RANGE_0TO10:
	   callo = 0.6125;
	   switch(c_blk->gain)
    	   {
		case 2:
        	   calhi = 4.9000;
	  	break;

		case 4:
        	   calhi = 2.4500;
		break;

		case 8:
        	   calhi = 1.2250;
		break;

		default:		/* 1 */
        	   calhi = 4.9000;
		break;
	   }
	   i_zero =  0.0;
	   i_span = 10.0000;
	break;

	default:			/* RANGE_10TO10 */
	   switch(c_blk->gain)
    	   {
		case 2:
        	   calhi = 4.9000;
	  	break;

		case 4:
        	   calhi = 2.4500;
		break;

		case 8:
        	   calhi = 1.2250;
		break;

		default:		/* 1 */
        	   calhi = 4.9000;
		break;
	   }
	   i_zero = -10.0000;
	   i_span =  20.0000;
	break;
    }

    slope = ((float)c_blk->gain) * (calhi - callo) / (float)(*cal_buf - *az_buf);

    temp = (((float)c_blk->bit_constant * slope) / i_span) * ((float)*raw_buf +
	(((callo * (float)c_blk->gain) - i_zero) / slope) - (float)*az_buf);

    if( temp < 0.0)		/* round */
        temp -= 0.5;
    else
       temp += 0.5;

    *cor_buf = (int)temp;
}


