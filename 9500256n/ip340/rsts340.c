
#include "../carrier/apc8620.h"
#include "ip340.h"

/*
{+D}
    SYSTEM:         Library Software - ip340 Board

    FILENAME:       rsts340.c

    MODULE NAME:    rsts340 - read status of ip340 board

    VERSION:        A

    CREATION DATE:  07/07/99

    DESIGNED BY:    F.J.M

    CODED BY:       F.J.M.
    
    ABSTRACT:       This module is used to read status of the ip340 board.

    CALLING
        SEQUENCE:   rsts340(ptr);
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
    for the ip340 board.  A pointer to the Configuration Block will
    be passed to this routine.  The routine will use a pointer
    within the Configuration Block together with offsets
    to reference the registers on the Board and will transfer the 
    status information from the Board to the Configuration Block.
*/



void rsts340(c_blk)
struct cblk340 *c_blk;
{
/*
    declare local storage
*/

    int i;              /* loop index */
    word idProm[12];    /* holds ID prom */


    double ReadReferenceValue();


/*
    ENTRY POINT OF ROUTINE:
    Initialize local storage.  Initialize pointer to board memory map.
*/

    c_blk->control = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg );

    ReadIpackID(c_blk->nHandle, c_blk->slotLetter, &idProm[0], sizeof idProm / 2 ); /* read from carrier */

    for(i = 0; i < 12; i++ )
 	  c_blk->id_prom[i] = (byte)idProm[i];

/*
    Read the reference voltage value and convert to reference constant
*/
    c_blk->ref_v = ( ReadReferenceValue( c_blk ) * 3276.8f);
}

