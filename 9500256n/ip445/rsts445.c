
#include "../carrier/apc8620.h"
#include "ip445.h"

/*
{+D}
    SYSTEM:         Library Software - ip445 Board

    FILENAME:       rsts445.c

    MODULE NAME:    rsts445 - read status of ip445 board

    VERSION:        A

    CREATION DATE:  03/06/96

    DESIGNED BY:    BDB

    CODED BY:       BDB
    
    ABSTRACT:       This module is used to read status of the ip445 board.

    CALLING
        SEQUENCE:   rsts445(ptr);
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

    This module is used to perform the read status function for the ip445
    board.  A pointer to the Status Block will be passed to this routine.
    The routine will use a pointer within the Status Block together with
    offsets to reference the registers on the Board and will transfer the 
    status information from the Board to the Status Block.
*/



void rsts445(s_blk)
struct sblk445 *s_blk;

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


