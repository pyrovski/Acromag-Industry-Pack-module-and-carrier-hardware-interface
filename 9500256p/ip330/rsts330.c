
#include "../carrier/apc8620.h"
#include "ip330.h"

/*
{+D}
    SYSTEM:         Library Software - ip330 Board

    FILENAME:       rsts330.c

    MODULE NAME:    rsts330 - read status of ip330 board

    VERSION:        A

    CREATION DATE:  11/07/95

    DESIGNED BY:    F.J.M

    CODED BY:       F.J.M.
    
    ABSTRACT:       This module is used to read status of the ip330 board.

    CALLING
        SEQUENCE:   rsts330(ptr);
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
    for the ip330 board.  A pointer to the Configuration Block will
    be passed to this routine.  The routine will use a pointer
    within the Configuration Block together with offsets
    to reference the registers on the Board and will transfer the 
    status information from the Board to the Configuration Block.
*/



void rsts330(c_blk)
struct cblk330 *c_blk;
{

/*
    ENTRY POINT OF ROUTINE:
    Initialize local storage.  Initialize pointer to board memory map.
*/

    c_blk->control = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg);

    /* read new data registers */
    c_blk->new_data[0] = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ndr_reg[0]);
    c_blk->new_data[1] = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->ndr_reg[1]);

    /* read missed data registers */
    c_blk->missed_data[0] = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->mdr_reg[0]);
    c_blk->missed_data[1] = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->mdr_reg[1]);
}





/*
{+D}
    SYSTEM:         Library Software - ip330 Board

    FILENAME:       rmid330.c

    MODULE NAME:    rmid330 - read status of ip320 board

    VERSION:        V1.0

    CREATION DATE:  12/14/93

    DESIGNED BY:    F.J.M

    CODED BY:       F.J.M.
    
    ABSTRACT:       This module is used to read I.D. of the ip330 board.

    CALLING
        SEQUENCE:   rmid330(ptr);
		    where:
			ptr (pointer to structure)
                            Pointer to the configuration structure.

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
    for the ip3330 board.  The routine will use a pointer
    within the configuration block together with offsets
    to reference the registers on the Board and will transfer the 
    status information from the Board to the configuration structure.
*/



void rmid330(c_blk)
struct cblk330 *c_blk;
{

/*
    declare local storage
*/
	int i;	            /* loop index */
	word idProm[12];    /* holds ID prom */

/*
    ENTRY POINT OF ROUTINE

    read board id information
*/

    c_blk->control = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cntl_reg );

    ReadIpackID(c_blk->nHandle, c_blk->slotLetter, &idProm[0], sizeof idProm / 2 ); /* read from carrier */

    for(i = 0; i < 12; i++ )
 	  c_blk->id_prom[i] = (byte)idProm[i];
}

