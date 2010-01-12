
#include "../carrier/apc8620.h"
#include "ip482.h"

/*
{+D}
    SYSTEM:         Library Software - ip482 Board

    FILENAME:       rmid482.c

    MODULE NAME:    rmid482 - read I.D. of ip482 board

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    F.J.M

    CODED BY:       F.J.M.
    
    ABSTRACT:       This module is used to read the I.D. of the ip482 board.

    CALLING
        SEQUENCE:   rmid482(ptr);
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

    This module is used to perform the read status function
    for the ip482 board.  A pointer to the Configuration Block will
    be passed to this routine.  The routine will use a pointer
    within the Configuration Block together with offsets
    to reference the registers on the Board and will transfer the 
    status information from the Board to the Configuration Block.
*/



void rmid482(c_blk)
struct ip482 *c_blk;
{
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
