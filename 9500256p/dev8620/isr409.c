
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
#include "../ip409/ip409.h"

/*
{+D}
   SYSTEM:          Acromag I/O Board

   MODULE NAME:     isr409.c - interrupt handler

   VERSION:         B

   CREATION DATE:   05/19/98

   CODED BY:        FM

   ABSTRACT:        This routine performs interrupt exception handling
		    for interrupts on the IP409 I/O board.

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


void isr_409(void* pAddr)

{

/*
	Local data areas
*/


int  i,j;								/* loop control */
volatile word type[2], enable[2], int_status[2];
volatile word b_mask;                   /* bit interrupt bit mask */
volatile struct handler_data *hdp;		/* pointer to handler data structure */
volatile struct isr_data *idp;			/* pointer to ISR data structure */
volatile struct map409 *map_ptr;		/* pointer to board I/O map */


/*
	Entry point of routine:
*/

	/*  Initialize pointers */
	hdp = (struct handler_data *)pAddr; /* get pointer to handler data structure */

	idp = (struct isr_data *)hdp->hd_ptr; /* get pointer to isr data structure */

	map_ptr = (struct map409 *)idp->slot_io_address; /* get I/O address */


 	type[0] = readw( &map_ptr->type_reg[0]); /* interrupt type */
 	type[1] = (readw( &map_ptr->type_reg[1]) & 0x00FF); /* interrupt type */

 	enable[0] = readw( &map_ptr->en_reg[0]); /* interrupt enable */
 	enable[1] = (readw( &map_ptr->en_reg[1]) & 0x00FF); /* interrupt enable */

 	int_status[0] = readw( &map_ptr->sts_reg[0]); /* interrupt status */
 	int_status[1] = (readw( &map_ptr->sts_reg[1]) & 0x00FF); /* interrupt status */


printk("\nIP409 ISR a=%lX t0=%X t1=%X e0=%X e1=%X s0=%X s1=%X\n",
			(unsigned long)map_ptr, type[0], type[1], enable[0], enable[1], int_status[0], int_status[1]);


	for( i = 0; i < 2; i++ )
	{
	  if( int_status[i] )	/* non-zero if interrupt pending */
	  {
	    /* Check each bit of the status register for a interrupt pending */
	    for( j = 0; j < 16; j++ )		/* prioritization bit 15 to 0 */
  	    {   
	        b_mask = 0x8000 >> j;		/* form interrupting mask */
	        if( b_mask & int_status[i] ) /* non-zero if interrupt pending */
	        {
		     if( b_mask & type[i] )  /* non-zero if COS selected */
		     {  
				/* nothing for COS now */
		     }
		     else				     /* LEVEL selected */
		     {
			  enable[i] &= (~b_mask); /* disable this bit */
		     }
			}
		} 
        writew( enable[i], &map_ptr->en_reg[i] );
        writew( int_status[i], &map_ptr->sts_reg[i] );/* clear pending interrupts */
	  }
	}
}
