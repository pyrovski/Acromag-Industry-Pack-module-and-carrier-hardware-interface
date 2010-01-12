
#include "../carrier/apc8620.h"
#include "ip400.h"

/*
{+D}
    SYSTEM:         Library Software - ip400 Board

    FILENAME:       rsts400.c

    MODULE NAME:    rsts400 - read status of ip400 board

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    FJM

    CODED BY:       FJM
    
    ABSTRACT:       This module is used to read status of the ip400 board.

    CALLING
        SEQUENCE:   rsts400(ptr);
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

    This module is used to perform the read status function for the ip400
    board.  A pointer to the Status Block will be passed to this routine.
    The routine will use a pointer within the Status Block together with
    offsets to reference the registers on the Board and will transfer the 
    status information from the Board to the Status Block.
*/



void rsts400(c_blk)
struct cblk400 *c_blk;
{

/*
    declare local storage
*/

    word idProm[12];    /* holds ID prom */
    int i;              /* loop index */

/*
    ENTRY POINT OF ROUTINE:
*/

    c_blk->sblk_ptr->enable = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->en_reg);	/* interrupt enable */
    c_blk->sblk_ptr->polarity = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->pol_reg);	/* interrupt polarity */
    c_blk->sblk_ptr->type = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->type_reg);	/* interrupt type */
    c_blk->sblk_ptr->vector = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->int_vect);	/* interrupt vector */

/*
    read board id information
*/

    ReadIpackID(c_blk->nHandle, c_blk->slotLetter, &idProm[0], sizeof idProm / 2); /* read from carrier */

    for(i = 0; i < 12; i++ )
 	   c_blk->sblk_ptr->id_prom[i] = (byte)idProm[i];
}

