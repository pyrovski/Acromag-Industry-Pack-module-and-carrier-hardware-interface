

/*
{+D}
    SYSTEM:	    Software for IP230/235

    FILE NAME:	    ip230.h

    VERSION:	    V1.0

    CREATION DATE:  05/19-98

    DESIGNED BY:    R.H.

    CODED BY:	    R. H.

    ABSTRACT:	    This module contains the definitions and structures
		    used by the IP230/235 library.

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
    the IP230/235 library.
*/

/*
    DEFINITIONS:
*/
#ifndef BUILDING_FOR_KERNEL
/* mode definitions */
#define BIPOLAR_5   0
#define BIPOLAR_10  1
#define UNIPOLAR_10 2
#define CON16	 (long)65536	/* constant for data correction equation */
#define VOLT_SPAN_B10 20        /* constant for ideal volt span of bipolar 10V board */
#define VOLT_SPAN     10        /* ideal volt span for all other types */
#define ZERO_COUNT_U10 (long)32768 /* ideal zero count for unipolar 10V board */
#define ZERO_COUNT     0        /* ideal zero count for all other boards */

#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */

#define A_SIZE8 8	        /* array size */
#define A_SIZE4 4	        /* array size */

/* Define min and max waveform memory address values */

enum
{
	MIN_WAVEFORM_ADDRESS = 0,
	MAX_WAVEFORM_ADDRESS = 2047
};
#endif /* BUILDING_FOR_KERNEL */







/*
    STRUCTURES:

    Defined below is the memory map template for the IP230/235 Board.
    This data structure provides access to the various registers on the board.
*/

struct map230
{
    word cont_reg;	    /* control register */
    word tim_pres;	    /* timer prescaler register */
    word conv_tim;	    /* conversion timer register */
    word wav_mem_dat;	    /* waveform memory data register */
    word wav_mem_add;	    /* waveform memory address register */
    word cal_coef_acc;	    /* calibration coefficients access */
    word cal_coef_stat;     /* calibration coeficient status */
    word start_conv;	    /* start conversion register */
    word dac_reg[8];	    /* dac registers 0 - 7 */
    word int_vect;	    /* interrupt vector register */
    word reserved;	    /* reserved for factory use */
};


/*
    Defined below is the structure which is used for the read analog command
*/

struct cblk230
{
    struct map230 *brd_ptr; /* pointer to base address of board */
    char slotLetter;	    /* IP slot letter */
    int nHandle;	    /* handle to an open carrier board */
    BOOL bCarrier;	    /* flag indicating a carrier is open */
    BOOL bInitialized;	    /* flag indicating ready to talk to board */	
    byte mode;		    /* the mode */
    byte id_prom[33];	    /* id prom contents. */
    long bit_constant;	    /* constant for data correction equation */
    int range[8];	    /* channel range storage */
    word coef_bufb10[8][2]; /* offset buffer start */
    word coef_bufu10[8][2]; /* offset buffer start */
    word coef_bufb5[8][2];  /* gain buffer start */
    short cor_buf[8];	    /* corrected buffer start */
    short ideal_buf[8];     /* ideal buffer start */
    word interrupt_vector;
    int interrupt_flag;
};





/*
    DECLARE MODULES CALLED:
*/
#ifndef BUILDING_FOR_KERNEL
void conv_mode(int choice, struct cblk230 *);
void rcc230(struct cblk230 *, int channels);
void write_vector(struct cblk230 *);			/* write vector to vector register on the board */
void wav_mem_inc(int choice, struct cblk230 *);/* setup memory auto increment */
void wav_mem_sel(int chan, struct cblk230 *);	/* setup waveform memory address */
void range_sel(struct cblk230 *,				/* configuration block */
			   int chan,						/* channel number to use */
			   int choice);						/* select range */
void trig_mode(int choice, struct cblk230 *);	/* setup the trigger mode */
void soft_reset(struct cblk230 * );			/* performs control register reset */
void tim_pre(int value, struct cblk230 *c_blk);/* performs writes to the timer prescaler */
void conv_time(int amount, struct cblk230 *);	/* performs writes to the conversion timer */
word wav_mem(int rw,							/* write 0 = read, 1 = write */
			word addr,							/* address to write to */
			word wdata,							/* data to be written to a memory location */
			struct cblk230 *);					/* performs reads and writes to waveform memory */
void start_con(struct cblk230 *);				/* starts conversions on the IP230/235 board */
void wro230(int channel,						/* analog output channel number */
				short data,						/* data to write to the output channel */
				struct cblk230 *);				/* performs the write output function */
void rmid230(struct cblk230 *);				/* read I.D. */
void readstat(struct cblk230 *);				/* read and display I.D. & gain/offset info */
void cd230(struct cblk230 *, int channel);		/* correct DAC output data */
void cont_reg(struct cblk230 *,int hflag);
long get_param(void);							/* input a parameter */
#endif /* BUILDING_FOR_KERNEL */
