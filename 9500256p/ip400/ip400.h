

/*
{+D}
    SYSTEM:		Acromag IP400 Digital Input Board

    FILENAME:		IP400.h

    MODULE NAME:	IP400.h

    VERSION:		A

    CREATION DATE:	05/19/98

    CODED BY:		FJM

    ABSTRACT:		This "inlcude" file contains all the major data
			structures and memory map used in the software

    CALLING SEQUENCE:	#include "ip400.h"

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
#define INT_STATUS     1	/* update interrupt status register */
#define INT_ENAB       2	/* update interrupt enable register */
#define INT_POLARITY   4	/* update interrupt polarity register */
#define INT_TYPE       8	/* update interrupt type register */
#define VECT	       0x10	/* update interrupt vector */

#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */
#endif /* BUILDING_FOR_KERNEL */

/*
    Defined below is the memory map template for the IP400 Board.
    This structure provides access to the various registers on the board.
*/

struct map400		   /* Memory map of the board */
{
    word in_port[3];	   /* input registers */
    word unused1[1];	   /* undefined */

    word en_reg; 	   /* Interrupt Enable Register */

    word type_reg;	   /* Interrupt Type Select Register */

    word pol_reg;	   /* Interrupt Input Polarity Register */

    word sts_reg;	   /* Status Register */

    word int_vect;	   /* Interrupt vector */
};


/*
    Defined below is the structure which is used to hold the
    board's status information.
*/

struct sblk400
{
    byte id_prom[32];	   /* board ID Prom */
    word int_status;	   /* interrupt status register */
    word enable;	   /* interrupt enable register */
    word polarity;	   /* interrupt input polarity register */
    word type;		   /* interrupt type select register */
    byte vector;	   /* interrupt vector register */
};

/*
    Defined below is the structure which is used to hold the
    board's configuration information.
*/

struct cblk400
{
    struct map400 *brd_ptr; /* base address of the board */
    struct sblk400* sblk_ptr; /*  pointer to status block info */
    char slotLetter;	   /* IP slot letter */
    int nHandle;	   /* handle to an open carrier board */
    BOOL bCarrier;	   /* flag indicating a carrier is open */
    BOOL bInitialized;	   /* flag indicating ready to talk to board */	
    word param; 	   /* parameter mask for configuring board */
    word int_status;	   /* interrupt status register */
    word enable;	   /* interrupt enable register */
    word polarity;	   /* interrupt input polarity register */
    word type;		   /* interrupt type select register */
    byte vector;	   /* interrupt vector register */
} ;







/*
	Function declarations
*/
void scfg400(struct cblk400 *);     /* routine to set up the configuration param. block */
void psts400(struct cblk400 *);     /* routine which calls the Read Status Command */
void cnfg400(struct cblk400 *);     /* routine to configure the board */
void rsts400(struct cblk400 *);     /* routine to read status information */
long rpnt400(struct cblk400 *,
			 unsigned port,         /* the port number */
			 unsigned point);		/* routine to read a input point */
long rprt400(struct cblk400 *,
			 unsigned port);		/* routine to read the input port */
long get_param(void);				/* input a parameter */

/* ISR */

void isr_400(void* pData);	/* interrupt handler for IP400 */

