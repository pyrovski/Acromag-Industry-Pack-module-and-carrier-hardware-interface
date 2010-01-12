

/*
{+D}
    SYSTEM:         Software for IP320

    FILE NAME:      ip320.h

    VERSION:        V1.0

    CREATION DATE:  05/19/98

    DESIGNED BY:    F.J.M.

    CODED BY:       F.J.M.

    ABSTRACT:       This module contains the definitions and structures
		    used by the IP320 library.

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
    the IP320 library.
*/

/*
    DEFINITIONS:
*/

#ifndef BUILDING_FOR_KERNEL
#define SA_SIZE 20      /* scan array size */

/* mode and gain code definitions */

#define DIF	    1   /* code for differential channel mode */
#define SEI	    2   /* code for single ended channel mode */
#define AZV	    3   /* code for auto-zero mode */
#define CAL	    4   /* code for calibration mode */

#define GAIN_X1     1   /* code for gain = 1 */
#define GAIN_X2     2   /* code for gain = 2 */
#define GAIN_X4     4   /* code for gain = 4 */
#define GAIN_X8     8   /* code for gain = 8 */

#define RANGE_5TO5   1 	/* input range */
#define RANGE_10TO10 2 	/* input range */
#define RANGE_0TO10  3	/* special auto zero value for this range */

#define BIT12	(word)0xFFF0	/* 12 bit data mask */

#define CON12    (long)4096	/* constant for data correction equation */
#define CON16    (long)65536	/* constant for data correction equation */

#define STRIG	     0	/* software trigger */
#define ETRIG	     1	/* external trigger */

/*  board control register bit positions */

#define SEL_SELECT  0x0100	/* single ended low select bit mask */
#define SEH_SELECT  0x0200	/* single ended high select bit mask */
#define AZ_SELECT   0x0300	/* auto-zero select bit mask */
#define CTRIG	    0x8000	/* conversion trigger */

#define CAL0	    20		/* 4.9V cal Voltage */
#define CAL1	    21		/* 2.459V cal Voltage */
#define CAL2	    22		/* 1.225V cal Voltage */
#define CAL3	    23		/* 0.6125V cal Voltage */
#endif /* BUILDING_FOR_KERNEL */

/*
    STRUCTURES:

    Defined below is the memory map template for the IP320 Board.
    This data structure provides access to the various registers on the board.
*/

struct map320
{
    word cntl_reg;	    /* board control register */

    word unused1[7];        /* undefined */
    word strt_reg;	    /* start conversion register */

    word unused2[7];        /* undefined */
    word ai_reg;	    /* analog input data register */
};

    

/*
    Defined below is the structure which is used for the read analog command
*/

struct cblk320
{
    struct map320 *brd_ptr; /* pointer to base address of board */
    byte range;		/* input range jumper setting of the board */
    byte trigger;	/* triggering option software/external */
    byte mode;		/* the mode */
    word average;	/* number of samples to average */
    word channel;	/* the channel */
    word gain;		/* the gain code */
    word data_mask;	/* bit mask for 12 bit/16 bit A/D converters */
    long bit_constant;	/* constant for data correction equation */
    word *s_raw_buf;	/* raw buffer start */
    word *s_az_buf;	/* auto zero buffer start */
    word *s_cal_buf;	/* calibration buffer start */
    int *s_cor_buf;	/* corrected buffer start */
    struct scan_array *sa_start; /* ptr to array of chan. & gain pairs */
    struct scan_array *sa_end;   /* ptr to end of chan. & gain pairs in array */
    char slotLetter;	/* IP slot letter */
    int nHandle;	/* handle to an open carrier board */
    BOOL bCarrier;	/* flag indicating a carrier is open */
    BOOL bInitialized;	/* flag indicating ready to talk to board */
    word control;	/* board control register. */
    byte id_prom[33];	/* id prom contents. */
};



/*
    Defined below is the structure for the scan array.  The scan
    array is an array of channel & gain pairs which are used
    when scanning a group of non-sequential input channels or when
    scanning channels using different gain settings.
*/

struct scan_array
{
    word chn;   /* channel number. */
    word gain;  /* gain setting corresponding to above channel. */
};



/*
    DECLARE MODULES CALLED:
*/

void setconf320(struct cblk320 *);	/* routine to set up the configuration param. block */
void readstat320(struct cblk320 *);	/* routine which calls the Read Status Command */
void ainsc320(struct cblk320 *);	/* routine to read analog inputs */
void ainmc320(struct cblk320 *);	/* routine to read analog inputs */
void sccd320(struct cblk320 *);	/* routine to correct input data */
void mccd320(struct cblk320 *);	/* routine to correct input data */
void rsts320(struct cblk320 *);	/* read status */
long get_param(void);				/* input a parameter */

