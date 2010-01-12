
#include "../carrier/apc8620.h"
#include "ip1k110.h"

/*
{+D}
    SYSTEM:         Library Software - ip1k110 Board

    FILENAME:       rsts1k110.c

    MODULE NAME:    rsts1k110 - read status of ip1k110 board

    VERSION:        A

    CREATION DATE:  10/01/01

    DESIGNED BY:    FJM

    CODED BY:       FJM
    
    ABSTRACT:       This module is used to read status of the ip1k110 board.

    CALLING
        SEQUENCE:   rsts1k110(ptr);
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

    This module is used to perform the read status function for the ip1k110
    board.  A pointer to the Status Block will be passed to this routine.
    The routine will use a pointer within the Status Block together with
    offsets to reference the registers on the Board and will transfer the 
    status information from the Board to the Status Block.
*/



void rsts1k110(c_blk)
struct cblk1k110 *c_blk;
{
/*
    declare local storage
*/

    int i;              /* loop index */
    word model;
    word idProm[12];    /* holds ID prom */
     
/*
    ENTRY POINT OF ROUTINE:

    read board id information
*/

    ReadIpackID(c_blk->nHandle, c_blk->slotLetter, &idProm[0], sizeof idProm / 2); /* read from carrier */

    for(i = 0; i < 12; i++ )
 	   c_blk->sblk_ptr->id_prom[i] = (byte)idProm[i];

	 /* Only read if Ip1k110/IP-EP20x is in user mode or this is an ip1k125 board */
    if( c_blk->sblk_ptr->id_prom[5] == 0x43 || c_blk->sblk_ptr->id_prom[5] == 0x44 || c_blk->sblk_ptr->id_prom[5] == 0x49 )
    {
       c_blk->sblk_ptr->direction = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->dir_reg);	 /* direction status */
       c_blk->sblk_ptr->enable = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->en_reg);	 /* interrupt enable */
       c_blk->sblk_ptr->polarity = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->pol_reg);	 /* interrupt polarity */
       c_blk->sblk_ptr->type = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->type_reg);	 /* interrupt type */
       c_blk->sblk_ptr->vector = (byte)input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->int_vect);/* interrupt vector */

       model = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->control);
       model &= 0x0700;       /* mask out all but model bits */
       switch( model )
       {
       case 0x100:	/* IP1k110-0024 or IP-EP202/4 */
            c_blk->sblk_ptr->model = 1;
          break;

          case 0x400:	/* IP1k110-2412 or IP-EP203 */
            c_blk->sblk_ptr->model = 2;
          break; 
          
          case 0x700:	/* IP1k110-4800 or IP-EP201 */
            c_blk->sblk_ptr->model = 3;
          break; 
          
          default:	/* leave it unselected */
            c_blk->sblk_ptr->model = 0;
          break;
      }
    }
}


