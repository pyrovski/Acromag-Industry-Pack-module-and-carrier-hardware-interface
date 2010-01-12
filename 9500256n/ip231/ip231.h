
/* #define SELECT_BIG_ENDIAN */	/* define this to compile for BIG_ENDIAN applications */


/*
{+D}
    SYSTEM:         Software for IP231

    FILE NAME:      ip231.h

    VERSION:        A

    CREATION DATE:  09/24/04

    DESIGNED BY:    F.J.M.

    CODED BY:       F.J.M.

    ABSTRACT:       This module contains the definitions and structures
		    used by the IP231 library.

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
    the IP231 library.
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

#define CON16    (long)65536	/* constant for data correction equation */
#endif /* BUILDING_FOR_KERNEL */

/*
    STRUCTURES:

    Defined below is the memory map template for the IP231 Board.
    This data structure provides access to the various registers on the board.
*/

#ifdef SELECT_BIG_ENDIAN	    /* Big endian structure */
struct map231
{
    word dac_reg[16];	    /* DAC registers */

    word trans_reg;	    /* transparent mode register */

    word simul_reg;	    /* simultaneous register */

    word siot_reg;	    /* simultaneous output trigger register */

    word dac_status;       

    byte unused;       
    byte control_reg;       
    byte ee_status;   
    byte ee_control;       
    word unused1[10];       /* undefined */

    struct cal_coef231	    /* calibration coefficients */
    {
        char offset_msb;    /* offset error MSByte */
	char offset_lsb;    /* offset error LSByte */
	char gain_msb;	    /* gain error MSByte */
	char gain_lsb;	    /* gain error LSByte */
    } cal_map[16];
};
#else			    /* Little ENDIAN structure */
struct map231		    /* Memory map of the I/O board */
{
    word dac_reg[16];	    /* DAC registers */

    word trans_reg;	    /* transparent mode register */

    word simul_reg;	    /* simultaneous register */

    word siot_reg;	    /* simultaneous output trigger register */

    word dac_status;       

    byte control_reg;       
    byte unused;       
    byte ee_control;     
    byte ee_status;   
    word unused1[10];       /* undefined */

    struct cal_coef231	    /* calibration coefficients */
    {
	char offset_lsb;    /* offset error LSByte */
        char offset_msb;    /* offset error MSByte */
	char gain_lsb;	    /* gain error LSByte */
	char gain_msb;	    /* gain error MSByte */
    } cal_map[16];
};
#endif /* SELECT_BIG_ENDIAN */    


/*
    Defined below is the structure which is used for the read analog command
*/

struct cblk231
{
    struct map231 *brd_ptr; /* pointer to base address of board */
    byte id_prom[33];	/* id prom contents. */
    byte mode;		/* the mode */
    long bit_constant;	/* constant for data correction equation */
    short *off_buf;	/* offset buffer start */
    short *gain_buf;	/* gain buffer start */
    word *ideal_buf;	/* ideal buffer start */
    word *cor_buf;	/* corrected buffer start */
    char slotLetter;	/* IP slot letter */
    int nHandle;	/* handle to an open carrier board */
    BOOL bCarrier;	/* flag indicating a carrier is open */
    BOOL bInitialized;	/* flag indicating ready to talk to board */	
};



/* Declare functions called */

void readstat231( struct cblk231 * );		/* read and display I.D. & gain/offset info */
void cd231( struct cblk231 *,
		   int channel );				/* correct DAC output data */
int coef231( float *, float *, short *,
			short *);				/* calculate gain/offset coefficients */
long get_param(void);					/* input a parameter */
void rccid231( struct cblk231 *);		/* Read Calibration Coefficients Command */
void mode_select231( struct cblk231 *);	/* mode selection */
void wro231( struct cblk231 *,
			word channel,               /* analog output channel number */
			word data);                 /* data to write to the output channel */
word rdo231(struct cblk231 *,
			word channel);				/* read output */
