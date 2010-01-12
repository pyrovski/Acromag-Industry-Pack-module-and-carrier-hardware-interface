

/*
{+D}
    SYSTEM:		Acromag IP409 Digital I/O Board

    FILENAME:		IP409.h

    MODULE NAME:	IP409.h

    VERSION:		A

    CREATION DATE:	05/19/98

    CODED BY:		FJM

    ABSTRACT:		This "inlcude" file contains all the major data
			structures and memory map used in the software

    CALLING SEQUENCE:	#include "ip409.h"

    MODULE TYPE:	include file

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/


/*
    Parameter mask bit positions
*/
#ifndef BUILDING_FOR_KERNEL
#define INT_STATUS     1	/* interrupt status registers */
#define INT_ENAB       2	/* interrupt enable registers */
#define INT_POLARITY   4	/* interrupt polarity registers */
#define INT_TYPE       8	/* interrupt type registers */
#define VECT	     0x10	/* interrupt vector */
#define DIR	     0x20	/* direction */

#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */
#endif /* BUILDING_FOR_KERNEL */


/*
    Defined below is the memory map template for the IP409 Board.
    This structure provides access to the various registers on the board.
*/

struct map409		   /* Memory map of the I/O board */
{
    word control;	   /* control register */
    word io_port[2];	   /* input/output registers */
    word dir_reg[2]; 	   /* Direction Registers */
    word en_reg[2]; 	   /* Interrupt Enable Registers */
    word type_reg[2];	   /* Interrupt Type Select Registers */
    word sts_reg[2];	   /* Status Registers */
    word pol_reg[2];	   /* Interrupt Input Polarity Registers */
    word int_vect;	   /* Interrupt vector */
};


/*
    Defined below is the structure which is used to hold the
    board's status information.
*/

struct sblk409
{
    byte id_prom[32];	   /* board ID Prom */
    word direction[2];	   /* direction registers */
    word enable[2];	   /* interrupt enable registers */
    word type[2];	   /* interrupt type select registers */
    word int_status[2];	   /* interrupt status registers */
    word polarity[2];	   /* interrupt input polarity registers */
    byte vector;	   /* interrupt vector register */
};




/*
    Defined below is the structure which is used to hold the
    board's configuration information.
*/

struct cblk409
{
    struct sblk409 *sblk_ptr; /* pointer to status block structure */
    struct map409 *brd_ptr; /* base address of the I/O board */
    char slotLetter;	   /* IP slot letter */
    int nHandle;	   /* handle to an open carrier board */
    BOOL bCarrier;	   /* flag indicating a carrier is open */
    BOOL bInitialized;	   /* flag indicating ready to talk to board */	
    word param;	 	   /* parameter mask for configuring board */
    word direction[2];	   /* direction registers */
    word enable[2];	   /* interrupt enable registers */
    word type[2];	   /* interrupt type select registers */
    word int_status[2];	   /* interrupt status registers */
    word polarity[2];	   /* interrupt input polarity registers */
    byte vector;	   /* interrupt vector register */
};






/* Declare functions called */

void scfg409(struct cblk409 *);		/* routine to set up the configuration param. block */
void psts409(struct cblk409 *);		/* routine which calls the Read Status Command */
void cnfg409(struct cblk409 *);		/* routine to configure the board */
void rsts409(struct cblk409 *);		/* routine to read status information */
long rpnt409(struct cblk409 *,
			unsigned port,			/* the I/O port number */
			unsigned point);		/* routine to read a input point */
long rprt409(struct cblk409 *,
			unsigned port);			/* routine to read the input port */
long wpnt409(struct cblk409 *,
			unsigned port,			/* the I/O port number */
			unsigned point,			/* the I/O point of a port */
			unsigned value			/* the output value */
							 );     /* routine to write to a output point */
long wprt409(struct cblk409 *,
			unsigned port,			/* the I/O port number */
			unsigned value);		/* routine to write to the output port */
long get_param(void);				/* input a parameter */
/* ISR */

void isr_409(void* pData);	/* interrupt handler for IP409 */

