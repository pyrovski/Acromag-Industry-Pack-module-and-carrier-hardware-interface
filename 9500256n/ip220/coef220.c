
#include "../carrier/apc8620.h"
#include "ip220.h"

/*
{+D}
        SYSTEM:		IP220

        NAME:           coef220.c

        VERSION:        V1.0

        CREATION DATE:  05/19/98
        
        DESIGNED BY:    F.J.M.

        CODED BY:       F.J.M.

        ABSTRACT:

        CALLING
	 SEQUENCE:      status = coef220(zero, span, gain, offset)

			*zero - Pointer to float the zero value
			*span - Pointer to float the span value
			*gain - Pointer to char the gain coefficient
			*offset - Pointer to char the offset coefficient

        MODULE TYPE:    int

        I/O RESOURCES:          

        SYSTEM RESOURCES:       

        MODULES CALLED:

        REVISIONS:

DATE            BY              PURPOSE
-----+-----------+------------------------------------------------------

{-D}
*/

/*
	MODULE FUNCTIONAL DETAILS:
*/

int coef220(zero, span, gain, offset)

float *zero;
float *span;
signed char *gain;
signed char *offset;


{

/*
	DECLARE EXTERNAL DATA AREAS
*/

/*
	DECLARE LOCAL DATA AREAS
*/

float slope, gain_error;

/*
	DECLARE THE MODULES CALLED
*/


/*
	ENTRY POINT OF ROUTINE
*/

    if((*span - *zero) == 0.0)
      return( -1);
     
    /* do gain calculation */
    slope = 4095.0 / (*span - *zero);
    gain_error = ((20.0 * slope) / 4096.0) - 1;
    *gain = (signed char)(gain_error * 4096.0 * 4.0);

    /* do offset calculation */

    slope *= -1.0;
    *offset = (signed char)(((slope * *zero) - 2048.0) * 4.0);
    return(0);
}
