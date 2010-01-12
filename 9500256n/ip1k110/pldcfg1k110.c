
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../carrier/apc8620.h"
#include "ip1k110.h"

#include IPARRAY	/* contains data array See HFileGenerator.c */


/*
{+D}
        SYSTEM:         Acromag IP1k110 Digital I/O Board

        FILENAME:       pldcfg1k110.c

        VERSION:        1.0

        CREATION DATE:  10/01/01

        DESIGNED BY:    JB & FM

        CODED BY:       JB & FM

        ABSTRACT:       This module is used to configure the PLD by loading
                        data in IP1k110[] over the IP bus.

        CALLING
            SEQUENCE:   status = int PLDConfig1k110( struct cblk1k110 *c_blk);
                        Where:
                          c_blk (pointer to structure)
                                Pointer to configuration block structure

        MODULE TYPE:    integer

        I/O
          RESOURCES:

        SYSTEM
          RESOURCES:

        REVISIONS:

  DATE      BY     PURPOSE
---------  ----   -------------------------------------------------------

{-D}
*/

/*
        MODULES FUNCTIONAL DETAILS:
*/


#define MAX_TRIES		1000



int PLDConfig1k110( struct cblk1k110 *c_blk)

{

/*
         Declare local data areas
*/

int status;  
int i;
int length;
word temp;
 
/*
        Entry point of routine
*/

 /* set CPLD control reg bit 0 to begin config */
 output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->control, 1);
 output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->control, 1);

 status = 0;

/* Starting from configuration mode.
   Bit 0 of the control register must be set to put the module into
   configuration mode. After bit 0 has been set, poll bit 0 until it
   reads as a logic high.  This may take up to 5uS after the
   configuration mode write.

   Bit 1 of the control register monitors the status of the Altera PLD
   during configuration.  The status signal must remain high during the
   configuration.  Poll the status register, if bits 1 & 0 are both high,
   a program byte may now be written to the configuration data register.
*/

 for(i = 0; i <= MAX_TRIES; i++ )
 {
     temp = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->control);
     temp &= 0xf;
     if(temp == 0x03)
	break;

     /* The user may place a one or two millisecond sleep here */
     /*  tsleep(0x80000001);	/ * Os9 */
     /*  Sleep((DWORD)1);	/ * Windows */
     /*  taskDelay( 2 );            / * VxWorks */
	 usleep(15);
 }

 if( i > MAX_TRIES )            /* should now be 3 */
   return((int) -2);         	/* not ready to configure error */

 length = sizeof(IP1K1X0);	/* get length of data array */
 
 for( i = 0; i < length; i++)
 {
 	/* poll nStatus, nConfig */
    temp = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->control);
    if( (temp & 0xF) != 0x03)   /* a control value != 3 indicates configuration is done */
       break;

    /* Write byte to configuration data register */
    /* Note: The configuration data register and io_port[0] occupy */
    /*       the same address.  The configuration data register is */
    /*       only available in configuration mode. */
    output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->io_port[0], (word)IP1K1X0[i]);
 }
 return(status);
}



