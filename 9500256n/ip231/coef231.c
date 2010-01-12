
#include "../carrier/apc8620.h"
#include "ip231.h"

/*
{+D}
        SYSTEM:		IP231

        NAME:           coef231.c

        VERSION:        V1.0

        CREATION DATE:  09/24/04
        
        DESIGNED BY:    F.J.M.

        CODED BY:       F.J.M.

        ABSTRACT:

        CALLING
	 SEQUENCE:      status = coef231(zero, span, gain, offset)

			*zero - Pointer to float the zero value
			*span - Pointer to float the span value
			*gain - Pointer to short the gain coefficient
			*offset - Pointer to short the offset coefficient

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

int coef231(zero, span, gain, offset)

float *zero;
float *span;
short *gain;
short *offset;


{


/*
	DECLARE LOCAL DATA AREAS
*/

float slope, gain_error;


/*
	ENTRY POINT OF ROUTINE
*/

    if((*span - *zero) == 0.0)
      return( -1);
     
    /* do gain calculation */
    slope = (CON16 - 1) / (*span - *zero);
    gain_error = ((20.0 * slope) / CON16) - 1;
    *gain = (short)(gain_error * CON16 * 4.0);

    /* do offset calculation */

    slope *= -1.0;
    *offset = (short)(((slope * *zero) - ( CON16/2 )) * 4.0);
    return(0);
}
