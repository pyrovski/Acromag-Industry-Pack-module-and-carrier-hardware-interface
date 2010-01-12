
#include "../carrier/apc8620.h"
#include "ip405.h"

/*
{+D}
    SYSTEM:         Library Software - ip405 Board

    FILENAME:       rsts405.c

    MODULE NAME:    rsts405 - read status of ip405 board

    VERSION:        A

    CREATION DATE:  01/03/95

    DESIGNED BY:    FJM

    CODED BY:       FJM
    
    ABSTRACT:       This module is used to read status of the ip405 board.

    CALLING
        SEQUENCE:   rsts405(ptr);
                    where:
                        ptr (pointer to structure)
                            Pointer to the status block structure.

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

    This module is used to perform the read status function for the ip405
    board.  A pointer to the Status Block will be passed to this routine.
    The routine will use a pointer within the Status Block together with
    offsets to reference the registers on the Board and will transfer the 
    status information from the Board to the Status Block.
*/



void rsts405(s_blk)
struct status_blk *s_blk;
{
/*
    declare local storage
*/
    int i;              /* loop index */
    word idProm[12];    /* holds ID prom */

/*
    ENTRY POINT OF ROUTINE
    read board id information
*/

    ReadIpackID(s_blk->nHandle, s_blk->slotLetter, &idProm[0], sizeof idProm / 2); /* read from carrier */

    for(i = 0; i < 12; i++ )
 	   s_blk->id_prom[i] = (byte)idProm[i];
}
