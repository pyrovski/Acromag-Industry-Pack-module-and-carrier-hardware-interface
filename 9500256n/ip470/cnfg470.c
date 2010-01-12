
#include "../carrier/apc8620.h"
#include "ip470.h"

/*
{+D}
    SYSTEM:         Software

    FILENAME:       cnfg470.c

    MODULE NAME:    cnfg470 - configure 470 board

    VERSION:        A

    CREATION DATE:  10/18/95

    CODED BY:       FJM

    ABSTRACT:       This module is used to perform the configure function
		    for the IP470 board.

    CALLING
	SEQUENCE:   cnfg470(ptr);
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

    This module is used to perform the "configure board" function for 
    the IP470 board.  A pointer to the Configuration Block will be passed 
    to this routine.  The routine will use a pointer within the 
    Configuration Block to reference the registers on the Board.  
    Based on flag bits in the Attribute and Parameter Flag words in the 
    Configuration Block, the board will be configured and various 
    registers will be updated with new information which will be 
    transfered from the Configuration Block to registers on the Board.
*/



void cnfg470(c_blk)

struct cblk470 *c_blk;

{

/*
    declare local storage
*/

   int i;
   byte bank_bits;             /* bank select info */

/*
    ENTRY POINT OF ROUTINE:
    If the I/O device is to be reset then do so.
*/

   if((c_blk->param & RESET_INTEN) && (c_blk->enable & RESET))
      output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->ier, RESET); /* set reset bit */

/*
    Check to see if the Interrupt Vector Register is to be updated.
    Update the Vector Register before enabling Global Interrupts so that
    the board will always output the correct vector upon interrupt.
*/

   if(c_blk->param & VECT )
      output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->ivr, c_blk->vector);

/*
    If in standard mode and the Mask Register is to be updated, then update it.
*/

   if((c_blk->e_mode == 0) && (c_blk->param & MASK))
   {
      bank_select(BANK0, c_blk);
      output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[7].b_select, (c_blk->mask_reg & 0x3F));
   }

/*
    Enhanced mode
*/
    
   if((c_blk->param & ENHANCED) && (c_blk->e_mode != 0))
   {
     output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[7].b_select, 0x07);
     output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[7].b_select, 0x0D);
     output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[7].b_select, 0x06);
     output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[7].b_select, 0x12);
   }

   if(c_blk->e_mode )       /* configure if enhanced mode flag is set */
   {

/*
    If the Mask Register is to be updated, then update it.
*/

     if(c_blk->param & MASK)
     {
       bank_select(BANK0, c_blk);
       output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[7].b_select, (c_blk->mask_reg & 0x3F));
     }

/*
    If the Event Control Registers are to be updated . . .
*/

     if(c_blk->param & EVCONTROL)
     {
       bank_select(BANK1, c_blk);
       output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[6].b_select, c_blk->ev_control[0]);
       /* Note: 470A version boards write ev_control[1] bits here */
       bank_bits = c_blk->ev_control[1];
       bank_bits &= 0x3F;                /* force bank select bits to zero */
       bank_bits |= (BANK1 << 6);        /* OR bank1 bits into the ev_control[1] bits before updating */
       output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[7].b_select, bank_bits);
     }

/*
    If the Debounce Control Register is to be updated, then update it.
*/

     if(c_blk->param & DEBCONTROL)
     {
       bank_select(BANK2, c_blk);
       output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[0].b_select, c_blk->deb_control);
     }

/*
    If the Debounce Duration Register is to be updated, then update it.
*/

     if(c_blk->param & DEBDURATION)
     {
       bank_select(BANK2, c_blk);
       output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[1].b_select, c_blk->deb_duration[0]);
       output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[2].b_select, c_blk->deb_duration[1]);
     }

/*
    If the Debounce Clock Register is to be updated, then update it.
*/

     if(c_blk->param & DEBCLOCK)
     {
       bank_select(BANK2, c_blk);
       output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[3].b_select, c_blk->deb_clock);
     }

/*
    If the Interrupt Enable Register is to be updated then do so.
*/

     if((c_blk->param & RESET_INTEN) && (c_blk->enable & INTEN))
     {
       bank_select(BANK1, c_blk);
       for(i = 0; i != 6; i++)
       {
         output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[i].b_select, (byte) 0);
         output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[i].b_select, (byte)0xFF);
       }
       output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->ier, INTEN);
     }
   }
}



/*
{+D}
    SYSTEM:         Library Software

    MODULE NAME:    bank_select - select bank on 470 board

    VERSION:        A

    CREATION DATE:  10/18/95

    CODED BY:       FJM

    ABSTRACT:       This module is used to select register banks on 
		    the IP470 board.

    CALLING
	SEQUENCE:   old_bank = bank_select(new_bank, c_blk);
		    where:
			cblk (pointer to structure)
			    Pointer to the configuration block structure.
			new_bank (byte)
			    New bank number 00, 01, 10 only
			old_bank (byte)
			    Old bank number 00, 01, 10 only
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
    
    This module is used to select register banks on the IP470 board.
*/



byte bank_select(new_bank, c_blk)

byte new_bank;
struct cblk470 *c_blk;

{

/*
    declare local storage
*/

    byte old_bank;              /* old bank number */
    byte bank_bits;             /* bank select info */

/*
    ENTRY POINT OF ROUTINE:
*/

   bank_bits = input_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[7].b_select); /* get current */
   old_bank = ((bank_bits & 0xC0) >> 6);/* isolate bank select bits */
   
   if(old_bank == new_bank)             /* same bank ? */
     return(old_bank);                  /* no need to change bits */

   bank_bits &= 0x3F;                   /* save all but bank select bits */
   bank_bits |= (new_bank << 6);        /* OR in new bank bits */

   output_byte(c_blk->nHandle, (byte*)&c_blk->brd_ptr->port[7].b_select, bank_bits);
   return(old_bank);
}

