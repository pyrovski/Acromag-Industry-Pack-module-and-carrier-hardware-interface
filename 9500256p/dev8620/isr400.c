
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
#include "../ip400/ip400.h"

/*
{+D}
   SYSTEM:          Acromag IP400 I/O Board

   MODULE NAME:     isr400.c - interrupt handler

   VERSION:         B

   CREATION DATE:  6/8/98

   CODED BY:        BDB

   ABSTRACT:        This routine performs interrupt exception handling
                    for the IP400 I/O board.

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
01/11/07 FJM   Add support x86_64

{-D}
*/

/*
   MODULES FUNCTIONAL DETAILS:
*/


void isr_400(void* pAddr)

{

/*
	Local data areas
*/

int  j;									/* loop control */
volatile word type, enable, int_status;
volatile word b_mask;                   /* bit interrupt bit mask */
volatile struct handler_data *hdp;		/* pointer to handler data structure */
volatile struct isr_data *idp;			/* pointer to ISR data structure */
volatile struct map400 *map_ptr;		/* pointer to board I/O map */

/*
	Entry point of routine:
        Initialize pointers
*/
	/*  Initialize pointers */
	hdp = (struct handler_data *)pAddr; /* get pointer to handler data structure */

	idp = (struct isr_data *)hdp->hd_ptr; /* get pointer to isr data structure */

	map_ptr = (struct map400 *)idp->slot_io_address; /* get I/O address */

 	type = readw( &map_ptr->type_reg); /* interrupt type */

 	enable = readw( &map_ptr->en_reg); /* interrupt enable */

 	int_status = readw( &map_ptr->sts_reg); /* interrupt status */

printk("\nIP400 ISR a=%lX t=%X e=%X s=%X\n", (unsigned long)map_ptr, type, enable, int_status);


	if(int_status)		/* non-zero if interrupt pending */
	{
	    /* Check each bit of the status register for a interrupt pending */
	    for( j = 0; j < 12; j++ )		/* prioritization bit 11 to 0 */
  	    {   
	        b_mask = 0x800 >> j;		/* form interrupting mask */

	        if( b_mask & int_status)	/* non-zero if interrupt pending */
	        {
			  if( b_mask & type)		/* non-zero if COS selected */
			  {  
				/* nothing for COS now */
			  }
			  else		/* LEVEL selected */
			  {
			    enable &= (~b_mask);	/* disable this bit */
			  }
			}
	    }
        writew( enable, &map_ptr->en_reg );
        writew( int_status, &map_ptr->sts_reg );/* clear pending interrupts */
	}
}

