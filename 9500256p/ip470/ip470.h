

/*
{+D}
    SYSTEM:         IP470 Software

    FILE NAME:      ip470.h

    VERSION:        V1.0

    CREATION DATE:  09/03/95

    DESIGNED BY:    FM

    CODED BY:       FM

    ABSTRACT:       This module contains the definitions and structures
		    used by the ip470 library.
    CALLING
	SEQUENCE:

    MODULE TYPE:    header file

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

    This module contains the definitions and structures used by
    the ip470 library.
*/

/*
    DEFINITIONS:
*/



#ifndef BUILDING_FOR_KERNEL
#define CLOSED  FALSE
#define OPEN    TRUE

#define MAXPORTS 5
#define MAXBITS  7
#define POINTVAL 1

#define RESET 2                 /* bit position in int. enable reg. */
#define INTEN 1                 /* bit position in int. enable reg. */

/*
   INTERRUPT VECTOR
*/

#define VECTOR0      0xB0       /* interrupt vector for port 0 */

#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */

/*
    Parameter mask bit positions
*/

#define ENHANCED       1        /* enhanced mode configuration */
#define MASK           2        /* write mask register */
#define EVCONTROL      4        /* event control register */
#define DEBCLOCK       8        /* debounce clock register */
#define DEBCONTROL  0x10        /* debounce control register */
#define DEBDURATION 0x20        /* debounce duration register */
#define RESET_INTEN 0x40        /* interrupt enable register */
#define VECT        0x80        /* interrupt vector register */
#endif /* BUILDING_FOR_KERNEL */

#define BANK0   (byte)0
#define BANK1   (byte)1
#define BANK2   (byte)2

/*
   DATA STRUCTURES
*/
#ifdef SELECT_BIG_ENDIAN		/* Big endian structure */
struct map470
{
  struct
   {
     byte nu0;                  /* common to all banks */
     byte b_select;             /* bank select register */
   } port[8];
   byte nu1[0x0F];              /* not used */
   byte ier;                    /* interrupt enable register */
   byte nu2[0x0F];              /* not used */
   byte ivr;                    /* interrupt vector register */
};
#else							/* Little endian structure */
struct map470
{
  struct
   {
     byte b_select;             /* bank select register */
     byte nu0;                  /* common to all banks */
   } port[8];
   byte nu1[0x0E];              /* not used */
   byte ier;                    /* interrupt enable register */
   byte nu2[0x0F];              /* not used */
   byte ivr;                    /* interrupt vector register */
};
#endif /* SELECT_BIG_ENDIAN */


/*
    Defined below is the structure which is used to hold the
    board's configuration information.
*/

struct cblk470
{
    struct map470 *brd_ptr; /* base address of the I/O board */
    char slotLetter;	   /* IP slot letter */
    int nHandle;	   /* handle to an open carrier board */
    BOOL bCarrier;	   /* flag indicating a carrier is open */
    BOOL bInitialized;	   /* flag indicating ready to talk to board */	
    word param;             /* parameter mask for configuring board */
    byte e_mode;            /* enhanced operation flag */
    byte mask_reg;          /* output port mask register */
    byte ev_control[2];     /* event control registers */
    byte deb_control;       /* debounce control register */
    byte deb_duration[2];   /* debounce duration registers */
    byte deb_clock;         /* debounce clock select register */
    byte enable;            /* interrupt enable register */
    byte vector;            /* interrupt vector register */
    byte id_prom[32];       /* board ID Prom */
};






/* Declare functions called */

byte bank_select(byte new_bank,
			struct cblk470 *);		/* select register banks on the IP470 board */
void scfg470(struct cblk470 *);     /* routine to set up the configuration param. block */
void psts470(struct cblk470 *);     /* routine which calls the Read Status Command */
void cnfg470(struct cblk470 *);     /* routine to configure the board */
void rsts470(struct cblk470 *);     /* routine to read status information */
void rmid470(struct cblk470 *);     /* read module I.D. */
long rpnt470(struct cblk470 *,
			byte port, byte point);	/* routine to read a input point */
long rprt470(struct cblk470 *,
			byte port);				/* routine to read the input port */
long wpnt470(struct cblk470 *,
			byte port,				/* the I/O port number */
			byte point,				/* the I/O point of a port */
			byte value);			/* routine to write to a output point */
long wprt470(struct cblk470 *,
			byte port, byte value);	/* routine to write to the output port */
long get_param(void);   /* input a parameter */

/* ISR */

void isr_470(void* pData);	/* interrupt handler for IP470 */

