

#ifndef BUILDING_FOR_KERNEL
#define BUILDING_FOR_KERNEL	/* controls conditional inclusion in file apc8620.h */
#endif


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>


#include "../carrier/apc8620.h"
#include "../ip470/ip470.h"



/*
{+D}
    SYSTEM:         Library Software

    MODULE NAME:    kbs - select bank on board

    VERSION:        A

    CREATION DATE:  04/01/09

    CODED BY:       FJM

    ABSTRACT:       This module is used to select register banks on 
                    the board.

    CALLING
	SEQUENCE:   old_bank = kbs(new_bank, struct map470 *map_ptr);
		    where:
			struct map470 *map_ptr (pointer to board)
			    Pointer to the board.
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
    
    This module is used to select register banks on the board.
*/



byte kbs(byte new_bank, volatile struct map470 *map_ptr)


{

/*
    declare local storage
*/

    byte old_bank;              /* old bank number */
    byte bank_bits;             /* bank select info */

/*
    ENTRY POINT OF ROUTINE:
*/

   bank_bits = readb(&map_ptr->port[7].b_select); /* get current */
   old_bank = ((bank_bits & 0xC0) >> 6);/* isolate bank select bits */
   
   if(old_bank == new_bank)             /* same bank ? */
     return(old_bank);                  /* no need to change bits */
   
   bank_bits &= 0x3F;                   /* save all but bank select bits */
   bank_bits |= (new_bank << 6);        /* OR in new bank bits */

   writeb(bank_bits, &map_ptr->port[7].b_select); 
   return(old_bank);
}


/*
{+D}
   SYSTEM:          Acromag I/O Board

   MODULE NAME:     isr470.c - interrupt handler

   VERSION:         A

   CREATION DATE:  04/01/09

   CODED BY:        FJM

   ABSTRACT:        These routines perform interrupt exception handling
                    for interrupts on the I/O board.

   CALLING
	SEQUENCE:   This subroutine runs as a result of an interrupt occuring.

   MODULE TYPE:

   I/O RESOURCES:

   SYSTEM
	RESOURCES:

   MODULES
	CALLED:

   REVISIONS:

 DATE      BY       PURPOSE
-------- ----  ------------------------------------------------

{-D}
*/

/*
   MODULES FUNCTIONAL DETAILS:

   For level operation, each input bit must be connected to a switch input. 
   
   The polarity of input bits is controlled in nibbles by a single bit in 
   the Event Control Register.    If this bit is a '1' positive edeges are
   sensed.  If this bit is a '0' negative edeges are sensed.
   
   Each time an input changes to the state programmed into the sense register
   an interrupt is generated.
*/




void isr_470(void* pAddr)

{

/*
	Local data areas
*/

volatile struct handler_data *hdp;              /* pointer to handler data structure */
volatile struct isr_data *idp;                  /* pointer to ISR data structure */
volatile struct map470 *map_ptr;                /* pointer to board I/O map */
volatile byte saved_bank;                       /* saved bank value */
volatile int i;                                 /* loop control */
volatile byte i_stat;                           /* interrupt status */
volatile byte p_mask;                           /* port interrupt bit mask */
volatile byte event_status;                     /* port event status mask */

/*
	Entry point of routine:
	Initialize pointers
*/

	hdp = (struct handler_data *)pAddr;              /* get pointer to handler data structure */

	idp = (struct isr_data *)hdp->hd_ptr;            /* get pointer to isr data structure */

	map_ptr = (struct map470 *)idp->slot_io_address; /* get I/O address */

	saved_bank = kbs(BANK1, map_ptr);                /* set & save bank select */
        
	i_stat = readb(&map_ptr->port[6].b_select);      /* interrupt status */

printk("IP470 ISR a=%lX s=%X\n", (unsigned long)map_ptr, i_stat);
    
	for(i = 0; i < 6; i++)                           /* check ports 0-5 for an interrupt pending */
	{
	   p_mask = ( 1 << i);                           /* form port interrupt bit mask */
	   if((p_mask & i_stat) != 0)                    /* port with interrupt pending? */
	   {
	     event_status |= readb(&map_ptr->port[i].b_select); /* interrupt sense */

	     /* write 0s to clear the interrupting bits */
	     writeb((~(event_status)),&map_ptr->port[i].b_select); /* write 0 to clear the interrupting bit */

/*
   User code can be added here to complete the interrupt service routine

           Example:   user_function();
*/

         writeb(0xFF, &map_ptr->port[i].b_select);    /* re-enable sense inputs */ 
       }
    }
	kbs(saved_bank, map_ptr);                         /* restore bank select */
}




