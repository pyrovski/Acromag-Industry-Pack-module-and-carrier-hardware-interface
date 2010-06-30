

/* #define SELECT_BIG_ENDIAN */	/* define this to compile for BIG_ENDIAN applications */

/*
{+D}
    SYSTEM:         Software for IP220

    FILE NAME:      ip220.h

    VERSION:        V1.0

    CREATION DATE:  05/18/98

    DESIGNED BY:    F.J.M.

    CODED BY:       F.J.M.

    ABSTRACT:       This module contains the definitions and structures
		    used by the IP220 library.

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
    the IP220 library.
*/

/*
    DEFINITIONS:
*/


#ifndef BUILDING_FOR_KERNEL
#define A_SIZE 16      /* array size */


/* mode definitions */

#define TM	    1   /* Transparent mode */
#define SM	    2   /* Simultaneous mode */
#define SMOT	    3   /* Simultaneous Output Trigger mode */

#define CON12    (long)4096	/* constant for data correction equation */
#endif /* BUILDING_FOR_KERNEL */




/*
    STRUCTURES:

    Defined below is the memory map template for the IP220 Board.
    This data structure provides access to the various registers on the board.
*/

#ifdef SELECT_BIG_ENDIAN	    /* Big endian structure */
struct map220
{
    word dac_reg[16];	    /* DAC registers */

    word trans_reg;	    /* transparent mode register */

    word simul_reg;	    /* simultaneous register */

    word siot_reg;	    /* simultaneous output trigger register */

    word unused1[13];       /* undefined */

    struct cal_coef220	    /* calibration coefficients */
    {
        char unused1;	    /* undefined */
	char offset;	    /* offset error */
        char unused2;	    /* undefined */
	char gain;	    /* gain error */
    } cal_map[16];
};
#else			    /* Little endian structure */
struct map220		    /* Memory map of the I/O board */
{
    word dac_reg[16];	    /* DAC registers */

    word trans_reg;	    /* transparent mode register */

    word simul_reg;	    /* simultaneous register */

    word siot_reg;	    /* simultaneous output trigger register */

    word unused1[13];       /* undefined */

    struct cal_coef220	    /* calibration coefficients */
    {
	char offset;	    /* offset error */
        char unused1;	    /* undefined */
	char gain;	    /* gain error */
        char unused2;	    /* undefined */
    } cal_map[16];
};
#endif /* SELECT_BIG_ENDIAN */    

/*
    Defined below is the structure which is used for the read analog command
*/

struct cblk220
{
    struct map220 *brd_ptr; /* pointer to base address of board */
    byte id_prom[33];	/* id prom contents. */
    byte mode;		/* the mode */
    long bit_constant;	/* constant for data correction equation */
    char *off_buf;	/* offset buffer start */
    char *gain_buf;	/* gain buffer start */
    word *ideal_buf;	/* ideal buffer start */
    word *cor_buf;	/* corrected buffer start */
    char slotLetter;	/* IP slot letter */
    int nHandle;	/* handle to an open carrier board */
    BOOL bCarrier;	/* flag indicating a carrier is open */
    BOOL bInitialized;	/* flag indicating ready to talk to board */	
};


/* Declare functions called */

void readstat220( struct cblk220 * );		/* read and display I.D. & gain/offset info */
void cd220( struct cblk220 *,
		   int channel );				/* correct DAC output data */
int coef220( float *, float *, signed char *,
			signed char *);				/* calculate gain/offset coefficients */
long get_param(void);					/* input a parameter */
void rccid220( struct cblk220 *);		/* Read Calibration Coefficients Command */
void mode_select220( struct cblk220 *);	/* mode selection */
void wro220( struct cblk220 *,
			word channel,               /* analog output channel number */
			word data);                 /* data to write to the output channel */
