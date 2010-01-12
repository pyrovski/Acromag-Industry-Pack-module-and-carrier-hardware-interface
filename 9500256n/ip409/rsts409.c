
#include "../carrier/apc8620.h"
#include "ip409.h"

/*
{+D}
    SYSTEM:         Library Software - ip409 Board

    FILENAME:       rsts409.c

    MODULE NAME:    rsts409 - read status of ip409 board

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    FJM

    CODED BY:       FJM
    
    ABSTRACT:       This module is used to read status of the ip409 board.

    CALLING
        SEQUENCE:   rsts409(ptr);
                    where:
                        ptr (pointer to structure)
                            Pointer to the config block structure.

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

    This module is used to perform the read status function for the ip409
    board.  A pointer to the Status Block will be passed to this routine.
    The routine will use a pointer within the Status Block together with
    offsets to reference the registers on the Board and will transfer the 
    status information from the Board to the Status Block.
*/



void rsts409(c_blk)
struct cblk409 *c_blk;
{
/*
    declare local storage
*/

    int i;              /* loop index */
    word idProm[12];    /* holds ID prom */

/*
    ENTRY POINT OF ROUTINE:

    read board id information
*/

    ReadIpackID(c_blk->nHandle, c_blk->slotLetter, &idProm[0], sizeof idProm / 2); /* read from carrier */

    for(i = 0; i < 12; i++ )
 	   c_blk->sblk_ptr->id_prom[i] = (byte)idProm[i];

    c_blk->sblk_ptr->direction[0] = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->dir_reg[0]);		 /* direction status */
    c_blk->sblk_ptr->direction[1] = (input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->dir_reg[1]) & 0x00FF); /* direction status */
    c_blk->sblk_ptr->enable[0] = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->en_reg[0]);		 /* interrupt enable */
    c_blk->sblk_ptr->enable[1] = (input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->en_reg[1]) & 0x00FF);     /* interrupt enable */
    c_blk->sblk_ptr->polarity[0] = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->pol_reg[0]);		 /* interrupt polarity */
    c_blk->sblk_ptr->polarity[1] = (input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->pol_reg[1]) & 0x00FF);  /* interrupt polarity */
    c_blk->sblk_ptr->type[0] = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->type_reg[0]);		 /* interrupt type */
    c_blk->sblk_ptr->type[1] = (input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->type_reg[1]) & 0x00FF);     /* interrupt type */
    c_blk->sblk_ptr->vector = (byte)input_word( c_blk->nHandle,(word*)&c_blk->brd_ptr->int_vect);		 /* interrupt vector */
}
