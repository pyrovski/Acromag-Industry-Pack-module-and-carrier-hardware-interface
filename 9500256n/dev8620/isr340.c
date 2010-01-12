
#ifndef BUILDING_FOR_KERNEL
#define BUILDING_FOR_KERNEL	/* controls conditional inclusion in file carrier.h */
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
#include "../ip340/ip340.h"

/*
{+D}
   SYSTEM:          Acromag Board

   MODULE NAME:     isr_340.c - interrupt handler

   VERSION:         A

   CREATION DATE:   04/01/09

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
*/


void isr_340(void* pAddr)

{

volatile struct handler_data *hdp;		/* pointer to handler data structure */
volatile struct isr_data *idp;			/* pointer to ISR data structure */
volatile struct map340 *map_ptr;		/* pointer to board I/O map */
volatile word control;                          /* control reg */

/*
	Entry point of routine:
	Initialize pointers
*/

	hdp = (struct handler_data *)pAddr; /* get pointer to handler data structure */

	idp = (struct isr_data *)hdp->hd_ptr; /* get pointer to isr data structure */

	map_ptr = (struct map340 *)idp->slot_io_address; /* get I/O address */

	control = readw( &map_ptr->cntl_reg );		/* control reg */

	control &= (~( 1<<6 ));				/* mask interrupts */

    writew( control, &map_ptr->cntl_reg );/* clear pending interrupts */

printk("\nIP34X ISR a=%lX\n", (unsigned long)map_ptr);

}
