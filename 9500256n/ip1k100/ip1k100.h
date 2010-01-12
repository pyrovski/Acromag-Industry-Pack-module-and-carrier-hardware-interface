


/*
{+D}
    SYSTEM:             Acromag IP1k100 Digital I/O Board

    FILENAME:           IP1k100.h

    MODULE NAME:        IP1k100.h

    VERSION:            A

    CREATION DATE:      10/04/01

    CODED BY:           FM

    ABSTRACT:           This "inlcude" file contains all the major data
                        structures and memory map used in the software

    CALLING SEQUENCE:

    MODULE TYPE:        include file

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE      BY      PURPOSE
    --------   -----    ---------------------------------------------------

{-D}
*/


/*
    Parameter mask bit positions
*/
#ifndef BUILDING_FOR_KERNEL
#define SEL_MODEL      1        /* selelct model */
#define INT_STATUS     2        /* interrupt status registers */
#define INT_ENAB       4        /* interrupt enable registers */
#define INT_POLARITY   8        /* interrupt polarity registers */
#define INT_TYPE     0x10       /* interrupt type registers */
#define VECT         0x20       /* interrupt vector */
#define DIR          0x40       /* direction */
#define RESET        0x80       /* reset */

#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */
#endif /* BUILDING_FOR_KERNEL */

/*
    Defined below is the memory map template for the IP1k100 Board.
    This structure provides access to the various registers on the board.
*/

struct map1k100            /* Memory map of the I/O board */
{
    word control;          /* control register */
    word io_port[3];       /* input/output registers */
    word dir_reg;          /* Direction Register */
    word en_reg;           /* Interrupt Enable Register */
    word type_reg;         /* Interrupt Type Select Register */
    word sts_reg;          /* Status Register */
    word pol_reg;          /* Interrupt Input Polarity Register */
    word int_vect;         /* Interrupt vector */
    word mem_data;         /* memory data register */
    word mem_addr;         /* memory address register */
    word clk_shft[2];      /* clock shift high/low registers */
    word length;           /* Clock Gen program word length register */
    word trigger;          /* Clock Gen trigger register */
};

/*
    Defined below is the structure which is used to hold the
    board's status information.
*/

struct sblk1k100
{
    byte id_prom[32];   /* board ID Prom */
    byte model;         /* model selected */
    word direction;     /* direction registers */
    byte enable;        /* interrupt enable registers */
    byte type;          /* interrupt type select registers */
    byte int_status;    /* interrupt status registers */
    byte polarity;      /* interrupt input polarity registers */
    byte vector;        /* interrupt vector register */
};




/*
    Defined below is the structure which is used to hold the
    board's configuration information.
*/

struct cblk1k100
{
    struct sblk1k100 *sblk_ptr; /* pointer to status block structure */
    struct map1k100 *brd_ptr;   /* base address of the I/O board */
    char slotLetter;            /* IP slot letter */
    int nHandle;                /* handle to an open carrier board */
    BOOL bCarrier;              /* flag indicating a carrier is open */
    BOOL bInitialized;          /* flag indicating ready to talk to board */	
    word param;                 /* parameter mask for configuring board */
    word model;                 /* model selected */
    word direction;             /* direction registers */
    word enable;                /* interrupt enable registers */
    word type;                  /* interrupt type select registers */
    word int_status;            /* interrupt status registers */
    word polarity;              /* interrupt input polarity registers */
    byte vector;                /* interrupt vector register */
    byte reset;                 /* reset flag */
};






/* Declare functions called */

int PLDConfig1k100( struct cblk1k100 *);/* configure PLD */
void scfg1k100(struct cblk1k100 *);		/* routine to set up the configuration param. block */
void psts1k100(struct cblk1k100 *);		/* routine which calls the Read Status Command */
void cnfg1k100(struct cblk1k100 *);		/* routine to configure the board */
void rsts1k100(struct cblk1k100 *);		/* routine to read status information */
long rpnt1k100(struct cblk1k100 *,
		unsigned port, unsigned point);	/* routine to read a input point */
long rprt1k100(struct cblk1k100 *,
		unsigned port);					/* routine to read the input port */
long wpnt1k100(struct cblk1k100 *,
		unsigned port,					/* the I/O port number */
		unsigned point,					/* the I/O point of a port */
		unsigned value);				/* routine to write to a output point */
long wprt1k100(struct cblk1k100 *,
		unsigned port, unsigned value);	/* routine to write to the output port */
long get_param(void);   /* input a parameter */

/* ISR */

void isr_1k100(void* pData);	/* interrupt handler for IP1k100 */


