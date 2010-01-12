
#include "../carrier/apc8620.h"
#include "ip231.h"

/*
{+D}
    SYSTEM:         Library Software

    FILENAME:       mode231.c

    MODULE NAME:    mode_select231

    VERSION:        V1.0

    CREATION DATE:  09/24/04

    DESIGNED BY:    F.J.M.

    CODED BY:       F.J.M.

    ABSTRACT:       This module selects the mode of operation
		    (transparent or simultaneous writes) for the IP231 board.

    CALLING
        SEQUENCE:   mode_select231(ptr);
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
*/

void mode_select231(c_blk)

struct cblk231 *c_blk;

{


/*
    ENTRY POINT OF ROUTINE:
*/

    switch(c_blk->mode)
    {
     case SM:			/* SIMULTANEOUS MODE */
	    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->simul_reg, 0xFFFF);
        break;

     case SMOT:			/* SIMULTANEOUS OUTPUT TRIGGER */
	    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->siot_reg, 0xFFFF);
        break;

     default:			/* TRANSPARENT MODE */
	    output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->trans_reg, 0xFFFF);
        break;
    }
}
