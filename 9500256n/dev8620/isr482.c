
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
#include "../ip482/ip482.h"


/*
{+D}
   SYSTEM:	    Acromag Input Board

   MODULE NAME:     isr_482.c - interrupt handler

   VERSION:	    B

   CREATION DATE:   10/10/03

   CODED BY:	    FM

   ABSTRACT:	    This routine performs interrupt exception handling
                    for the ip482 board.

   CALLING
	SEQUENCE:   This subroutine runs as a result of an interrupt occuring.

   MODULE TYPE:

   I/O RESOURCES:

   SYSTEM
	RESOURCES:

   MODULES
	CALLED:

   REVISIONS:

DATE	 BY	   PURPOSE
-------- ----  ------------------------------------------------
01/11/07 FJM   Add support x86_64

{-D}
*/

/*
   MODULES FUNCTIONAL DETAILS:
*/


void isr_482(void* pAddr)

{

/*
	Local data areas
*/

volatile int i;
volatile word i_stat;
volatile struct handler_data *hdp;		/* pointer to handler data structure */
volatile struct isr_data *idp;			/* pointer to ISR data structure */
volatile struct map482 *map_ptr;		/* pointer to board I/O map */

/*
	Entry point of routine:
*/

	hdp = (struct handler_data *)pAddr; /* get pointer to handler data structure */

	idp = (struct isr_data *)hdp->hd_ptr; /* get pointer to isr data structure */

	map_ptr = (struct map482 *)idp->slot_io_address; /* get I/O address */

 	i_stat = readw( &map_ptr->InterruptStatusReg); /* interrupt status */

printk("\nIP482 ISR a=%lX s=%X\n", (unsigned long)map_ptr, i_stat);

	if( i_stat )                       /* any interrupt(s) pending? */
	{
	  /* check each counter for an interrupt pending state */
	  for( i = 0; i < 10; i++ )
	  {
		if( i_stat & (1 << i ))    /* check for individual counter interrupt */
		{
				     /* call individual counter interrupt here */
		}
	  }
      writew( i_stat, &map_ptr->InterruptStatusReg );/* clear pending interrupts */
	}
}

