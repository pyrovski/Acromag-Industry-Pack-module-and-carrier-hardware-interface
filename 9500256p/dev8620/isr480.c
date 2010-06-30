

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
#include "../ip480/ip480.h"



/*
{+D}
   SYSTEM:	    Acromag Input Board

   MODULE NAME:     isr_480.c - interrupt handler

   VERSION:	    A

   CREATION DATE:   05/19/98

   CODED BY:	    FM

   ABSTRACT:	    This routine performs interrupt exception handling
                    for the IP480 Input board.

   CALLING
	SEQUENCE:   This subroutine runs as a result of an interrupt occuring.

   MODULE TYPE:

   I/O RESOURCES:

   SYSTEM
	RESOURCES:

   MODULES
	CALLED:

   REVISIONS:

 DATE	   BY	    PURPOSE
-------- ----  ------------------------------------------------

{-D}
*/

/*
   MODULES FUNCTIONAL DETAILS:
*/


void isr_480(void* pAddr)

{

/*
	Local data areas
*/

volatile struct handler_data *hdp;              /* pointer to handler data structure */
volatile struct isr_data *idp;                  /* pointer to ISR data structure */
volatile unsigned char *map_ptr;                /* pointer to board I/O map */
volatile int i, j;                              /* loop control */
volatile unsigned short i_stat;                 /* interrupt status */

/*
	Entry point of routine:
	Initialize pointers
*/

	hdp = (struct handler_data *)pAddr;              /* get pointer to handler data structure */

	idp = (struct isr_data *)hdp->hd_ptr;            /* get pointer to isr data structure */

	map_ptr = (unsigned char *)idp->slot_io_address; /* get I/O address */

	i_stat = readw( (word *)(map_ptr + InterruptPending));      /* interrupt status */

	i_stat &= 0x3F00;		/* and off the unused bits and save the useful ones */
	j = 6;

printk("IP480 ISR a=%lX s=%X\n", (unsigned long)map_ptr, i_stat);

	if( i_stat )			/* any interrupt(s) pending? */
	{
	  /* check each bit for an interrupt pending state */
	  for( i = 0; i < j; i++ )			/* check each counter timer */
	  {
		if( i_stat & (1 << (i + 8)) )	/* build interrupt clear word */
			i_stat &= (~(1 << (i + 8)));/* clear interrupting bit */
		else
			i_stat |= (1 << (i + 8));	/* set bit to ignore */
	  }

/*
   User code can be added here to complete the interrupt service routine

           Example:   user_function();
*/

      writew(i_stat, (word *)(map_ptr + InterruptPending));    /* re-enable sense inputs */ 
	}
}
