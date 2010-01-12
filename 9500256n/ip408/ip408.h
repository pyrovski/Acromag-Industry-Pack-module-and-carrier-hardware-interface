

/* #define SELECT_BIG_ENDIAN */	/* define this to compile for big_endian applications */

/*
{+D}
    SYSTEM:		Acromag IP408 Digital I/O Board

    FILENAME:		IP408.h

    MODULE NAME:	IP408.h

    VERSION:		A

    CREATION DATE:	01/04/95

    CODED BY:		FJM

    ABSTRACT:		This "inlcude" file contains all the major data
			structures and memory map used in the software

    CALLING SEQUENCE:	#include "ip408.h"

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
#define INT_STATUS     1	/* interrupt status register */
#define INT_ENAB       2	/* interrupt enable register */
#define INT_POLARITY   4	/* interrupt polarity register */
#define INT_TYPE       8	/* interrupt type register */
#define VECT	       0x10	/* interrupt vector */

#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */
#endif /* BUILDING_FOR_KERNEL */

/*
    Defined below is the memory map template for the IP408 Board.
    This structure provides access to the various registers on the board.
*/

#ifdef SELECT_BIG_ENDIAN   /* Big ENDIAN structure */
struct map408		   /* Memory map of the I/O board */
{
    word in_port[2];	   /* input registers */
    word out_port[2];	   /* output registers */

    byte unused1;	   /* undefined */
    byte en_reg; 	   /* Interrupt Enable Register */

    byte unused2;	   /* undefined */
    byte type_reg;	   /* Interrupt Type Select Register */

    byte unused3;	   /* undefined */
    byte pol_reg;	   /* Interrupt Input Polarity Register */

    byte unused4;	   /* undefined */
    byte sts_reg;	   /* Status Register */

    byte unused5;	   /* undefined */
    byte int_vect;	   /* Interrupt vector */
};
#else			   /* Little ENDIAN structure */
struct map408		   /* Memory map of the I/O board */
{
    word in_port[2];	   /* input registers */
    word out_port[2];	   /* output registers */

    byte en_reg; 	   /* Interrupt Enable Register */
    byte unused1;	   /* undefined */

    byte type_reg;	   /* Interrupt Type Select Register */
    byte unused2;	   /* undefined */

    byte pol_reg;	   /* Interrupt Input Polarity Register */
    byte unused3;	   /* undefined */

    byte sts_reg;	   /* Status Register */
    byte unused4;	   /* undefined */

    byte int_vect;	   /* Interrupt vector */
    byte unused5;	   /* undefined */
};
#endif /* SELECT_BIG_ENDIAN */

/*
    Defined below is the structure which is used to hold the
    board's status information.
*/

struct sblk408
{
    byte id_prom[32];	   /* board ID Prom */
    byte int_status;	   /* interrupt status register */
    byte enable;	   /* interrupt enable register */
    byte polarity;	   /* interrupt input polarity register */
    byte type;		   /* interrupt type select register */
    byte vector;	   /* interrupt vector register */
};

/*
    Defined below is the structure which is used to hold the
    board's configuration information.
*/

struct cblk408
{
    struct map408 *brd_ptr; /* base address of the I/O board */
    struct sblk408 *sblk_ptr;/* Status block pointer */
    char slotLetter;	   /* IP slot letter */
    int nHandle;	   /* handle to an open carrier board */
    BOOL bCarrier;	   /* flag indicating a carrier is open */
    BOOL bInitialized;	   /* flag indicating ready to talk to board */	
    word param; 	   /* parameter mask for configuring board */
    byte int_status;	   /* interrupt status register */
    byte enable;	   /* interrupt enable register */
    byte polarity;	   /* interrupt input polarity register */
    byte type;		   /* interrupt type select register */
    byte vector;	   /* interrupt vector register */
};






/* Declare functions called */

void scfg408(struct cblk408 *);		/* routine to set up the configuration param. block */
void psts408(struct cblk408 *);		/* routine which calls the Read Status Command */
void cnfg408(struct cblk408 *);		/* routine to configure the board */
void rsts408(struct cblk408 *);		/* routine to read status information */
long rpnt408(struct cblk408 *,
		    unsigned port,
		    unsigned point);		/* routine to read a input point */
long rprt408(struct cblk408 *,
		    unsigned port);		/* routine to read the input port */
long wpnt408(struct cblk408 *,
		    unsigned port,
		    unsigned point,
		    unsigned  value);		/* routine to write to a output point */
long wprt408(struct cblk408 *,
		    unsigned port,
		    unsigned  value);		/* routine to write to the output port */
long robt408(struct cblk408 *,
		    unsigned port,
		    unsigned point);		/* routine to read a output point */
long ropt408(struct cblk408 *,
		    unsigned port);		/* routine to read the output port */
long get_param(void);			/* input a parameter */

byte input_byte(int nHandle, byte*);/* function to read an input byte */
word input_word(int nHandle, word*);/* function to read an input word */
void output_byte(int nHandle, byte*, byte);	/* function to output a byte */
void output_word(int nHandle, word*, word);	/* function to output a word */
long input_long(int nHandle, long*);			/* function to read an input long */
void output_long(int nHandle, long*, long);	/* function to output a long */


/* ISR */

void isr_408(void* pData);	/* interrupt handler for IP408 */






