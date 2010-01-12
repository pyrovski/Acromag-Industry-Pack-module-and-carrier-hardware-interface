
/* 
{+D}
    SYSTEM:             Acromag IP405 Digital I/O Board

    FILENAME:           ip405.h

    MODULE NAME:        ip405.h

    VERSION:            A

    CREATION DATE:      05/19/98

    CODED BY:           FJM

    ABSTRACT:           This file contains the common data structures used
			by the library software package.

    CALLING SEQUENCE:   #include "ip405.h"

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
    MODULES FUNCTION DETAILS

    This file contains the common data structures for the library software
    package. Two structures are defined here. The first one, map405, is
    the exact memory map of the I/O board.
    The second one, sblk405, is the structure of the I/O board status
    information parameter block.
*/

/*
    Definitions
*/


/*
    Defined below is the memory map template for the IP405 Board.
    This data structure provides access to the various registers on the board.
*/

struct map405		    /* Memory map of the I/O board */
{
    struct io405
    {
       word io_port;	    /* IO registers */
       word unused1[7];     /* undefined */
    } io_map[3];
};


/*
    Defined below is the structure for the Status Command function.
*/

struct status_blk
{
    struct map405 *brd_ptr;     /* pointer to base address of board. */
    byte id_prom[33];		/* id prom contents. */
    char slotLetter;	/* IP slot letter */
    int nHandle;	/* handle to an open carrier board */
    BOOL bCarrier;	/* flag indicating a carrier is open */
    BOOL bInitialized;	/* flag indicating ready to talk to board */	
};

/*
    DECLARE MODULES CALLED:
*/

void psts405(struct status_blk *,
			int handle, char slot);		/* routine which calls the Read I.D. Command */
void rsts405(struct status_blk *);		/* routine to read I.D. information */
long rpnt405(struct status_blk *,
			unsigned port,				/* the I/O port number */
			unsigned point				/* the I/O point of a port */
							);			/* routine to read a point */
long rprt405(struct status_blk *,
			unsigned port);				/* routine to read a port */
long wpnt405(struct status_blk *,
			unsigned port,				/* the I/O port number */
			unsigned point,				/* the I/O point of a port */
			unsigned value				/* the output value */
							 );			/* routine to write a point */
long wprt405(struct status_blk *,
			unsigned port,				/* the I/O port number */
			unsigned value				/* the output value */
							 );			/* routine to write a port */
long get_param(void);					/* input a parameter */
