
#include "../carrier/apc8620.h"
#include "ip408.h"

/*
{+D}
    SYSTEM:         Library Software - ip408 Board

    FILENAME:       rsts408.c

    MODULE NAME:    rsts408 - read status of ip408 board

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    FJM

    CODED BY:       FJM
    
    ABSTRACT:       This module is used to read status of the ip408 board.

    CALLING
        SEQUENCE:   rsts408(ptr);
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

    This module is used to perform the read status function for the ip408
    board.  A pointer to the Status Block will be passed to this routine.
    The routine will use a pointer within the Status Block together with
    offsets to reference the registers on the Board and will transfer the 
    status information from the Board to the Status Block.
*/


void rsts408(c_blk)
struct cblk408 *c_blk;
{
/*
    declare local storage
*/
	int i;	            /* loop index */
	word idProm[12];    /* holds ID prom */

/*
    ENTRY POINT OF ROUTINE
*/

    c_blk->sblk_ptr->enable = input_byte(c_blk->nHandle,(byte*)&c_blk->brd_ptr->en_reg);	/* interrupt enable */
    c_blk->sblk_ptr->polarity = input_byte(c_blk->nHandle,(byte*)&c_blk->brd_ptr->pol_reg);	/* interrupt polarity */
    c_blk->sblk_ptr->type = input_byte(c_blk->nHandle,(byte*)&c_blk->brd_ptr->type_reg);	/* interrupt type */
    c_blk->sblk_ptr->vector = input_byte(c_blk->nHandle,(byte*)&c_blk->brd_ptr->int_vect);	/* interrupt vector */

/*
    read board id information
*/

    ReadIpackID(c_blk->nHandle, c_blk->slotLetter, &idProm[0], sizeof idProm / 2 ); /* read from carrier */

    for(i = 0; i < 12; i++ )
 	   c_blk->sblk_ptr->id_prom[i] = (byte)idProm[i];
}




