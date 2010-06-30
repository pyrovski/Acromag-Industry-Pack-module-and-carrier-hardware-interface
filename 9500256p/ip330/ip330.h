

/* #define SELECT_BIG_ENDIAN */	/* define this to compile for big_endian applications */

/*
{+D}
    SYSTEM:	    Software for IP330

    FILE NAME:	    ip330.h

    VERSION:	    A

    CREATION DATE:  11/07/95

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M.

    ABSTRACT:	    This module contains the definitions and structures
		    used by the IP330 library.

    CALLING
	SEQUENCE:

    MODULE TYPE:    header file

    I/O RESOURCES:

    SYSTEM
	RESOURCES:

    MODULES
	CALLED:

    REVISIONS:

  DATE	  BY	    PURPOSE
-------  ----	------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

    This module contains the definitions and structures used by
    the IP330 library.
*/

/*
    DEFINITIONS:
*/

/*
   Use the short typedef when binary twos complement output
   data format is selected in the control word of the IP330.

typedef short word;
*/
/*
   Use the unsigned short typedef when straight binary output
   data format is selected in the control word of the IP330.

typedef unsigned short word;
*/




/* definitions */
#ifndef BUILDING_FOR_KERNEL
#define SA_SIZE 32	/* # of elements in data arrays */
#define AVE_SAMPLES 64

#define GAIN_X1      0  /* code for gain = 1 */
#define GAIN_X2      1  /* code for gain = 2 */
#define GAIN_X4      2  /* code for gain = 4 */
#define GAIN_X8      3  /* code for gain = 8 */

#define RANGE_0TO5   0  /* input range */
#define RANGE_5TO5   1	/* input range */
#define RANGE_10TO10 2	/* input range */
#define RANGE_0TO10  3	/* special auto zero value for this range */

#define CON16	 (long)65536	/* constant for data correction equation */

#define TC_SELECT   0		/* two's compliment */
#define SB_SELECT   1		/* straight binary */

#define TI_SELECT   0		/* trigger direction (input)*/
#define TO_SELECT   1		/* trigger direction (output)*/

#define DI_SELECT   0		/* differential select */
#define SE_SELECT   1		/* single ended select */
#define AZ_SELECT   2		/* auto-zero select */
#define CAL_SELECT  3		/* calibration select */

#define DISABLE     0		/* disable scan */
#define UN_CONT     1		/* uniform continuous */
#define UN_SNGL     2		/* uniform single */
#define BU_CONT     3		/* burst continuous */
#define BU_SNGL     4		/* burst single */
#define EX_TRIG     5		/* external trigger */

#define TIMER_OFF   0		/* disable timer */
#define TIMER_ON    1		/* enable timer */

#define INT_DIS     0		/* disable interrupt */
#define INT_AEC     1		/* interrupt after each channel */
#define INT_AGC     2		/* interrupt after group of channels */

#define START_CONV  0x0001	/* start conversion */

#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */
#endif /* BUILDING_FOR_KERNEL */

/*
    STRUCTURES:

    Defined below is the memory map template for the IP330 Board.
    This data structure provides access to the various registers on the board.
*/

#ifdef SELECT_BIG_ENDIAN/* Big endian structure */
struct map330
{
    word cntl_reg;	    /* board control register */
    byte tpc_reg;	    /* timer prescaler register */
    byte vect_reg;	    /* interrupt vector register */
    word ctc_reg;	    /* conversion timer count register */
    byte end_ch;	    /* end channel register */
    byte start_ch;	    /* start channel register */
    word ndr_reg[2];    /* new data register */
    word mdr_reg[2];    /* missed data register */
    word strt_reg;	    /* start conversion register */
    word nused1[7];	    /* not used */
    byte gain[32];	    /* gain array */
    word data[32];	    /* data area */
};
#else			    /* Little endian structure */
struct map330
{
    word cntl_reg;	    /* board control register */
    byte vect_reg;	    /* interrupt vector register */
    byte tpc_reg;	    /* timer prescaler register */
    word ctc_reg;	    /* conversion timer count register */
    byte start_ch;	    /* start channel register */
    byte end_ch;	    /* end channel register */
    word ndr_reg[2];    /* new data register */
    word mdr_reg[2];    /* missed data register */
    word strt_reg;	    /* start conversion register */
    word nused1[7];	    /* not used */
    byte gain[32];	    /* gain array */
    word data[32];	    /* data area */
};
#endif /* SELECT_BIG_ENDIAN */

/*
    Defined below is the structure which is used for the read analog command
*/

struct cblk330
{
    struct map330 *brd_ptr; /* pointer to base address of board */
    char slotLetter;	    /* IP slot letter */
    int nHandle;	    /* handle to an open carrier board */
    BOOL bCarrier;	    /* flag indicating a carrier is open */
    BOOL bInitialized;	    /* flag indicating ready to talk to board */	
    int *s_cor_buf;	    /* corrected buffer start */
    word *s_raw_buf;	    /* raw buffer start */
    word *s_az_buf;	    /* auto zero data storage areas */
    word *s_cal_buf;	    /* calibration buffers */
    word conv_timer;	    /* conversion timer */
    byte vector;	    /* interrupt vector */
    byte data_format;	    /* SB or BTC */
    byte trigger;	    /* triggering option software/external */
    byte acq_mode;	    /* the acquisition mode */
    byte scan_mode;	    /* the scan mode */
    byte timer_ps;	    /* timer prescaler */
    byte timer_en;	    /* timer enable */
    byte int_mode;	    /* the interrupt mode */
    byte range; 	    /* input range setting of the board */
    byte s_channel;	    /* the start channel */
    byte e_channel;	    /* the end channel */
    word new_data[2];	    /* new data bit map */
    word missed_data[2];    /* missed data bit map */
    word control;	    /* board control register used by read status */
    byte id_prom[33];	    /* id prom contents used by read status */
};






/* Declare functions called */

void rmid330(struct cblk330 *);		/* read I.D PROM */
void rsts330(struct cblk330 *);		/* read board status */
void cnfg330(struct cblk330 *);		/* configure board */
void setconf330(struct cblk330 *);		/* routine to set up the configuration param. block */
void put_gain(struct cblk330 *,
			int channel, byte gain);	/* write gain code to IP330 */
byte get_gain(struct cblk330 *,
			int channel);				/* read gain code from IP330 */
void readstat330(struct cblk330 *);		/* routine which calls the Read Status Command */
int mccd330(struct cblk330 *);			/* routine to correct input data */
void calibrate330(struct cblk330 *,
			int mode);					/* get calibration data */
void convert330(struct cblk330 *);		/* get input data */
long get_param(void);					/* input a parameter */

/* ISR */

void isr_330(void* pData);	/* interrupt handler for IP330 */

