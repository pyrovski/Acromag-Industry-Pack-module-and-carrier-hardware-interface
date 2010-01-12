

/* #define SELECT_BIG_ENDIAN */	/* define this to compile for big_endian applications */


/*
{+D}
    SYSTEM:         Software for IP236

    FILE NAME:      ip236.h

    VERSION:        V1.0

    CREATION DATE:  12-28-98

    DESIGNED BY:    F.M.

    CODED BY:       F.M.

    ABSTRACT:       This module contains the definitions and structures
		    used by the IP236 library.

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
    the IP236 library.
*/

/*
    DEFINITIONS:
*/

#define NUM_CHAN 8		     /* number of channels present */

#define MAXSIZE	128		     /* data buffer size */



#ifndef BUILDING_FOR_KERNEL
#define BIPOLAR_5   0
#define BIPOLAR_10  1
#define UNIPOLAR_10 2

#define VOLT_SPAN_B10 20	     /* constant for ideal volt span of bipolar 10V board */
#define VOLT_SPAN     10 	     /* ideal volt span for all other types */


#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */
#endif /* BUILDING_FOR_KERNEL */

/*
    STRUCTURES:

    Defined below is the memory map template for the IP236 Board.
    This data structure provides access to the various registers on the board.
*/

#ifdef SELECT_BIG_ENDIAN	    /* Big ENDIAN structure */
struct map236
{
    byte reset_reg;	    /* reset register bit 7 */
    byte start_status;	    /* conversion start / status register */
    byte int_status;	    /* interrupt status register */
    byte int_vector;	    /* interrupt vector register */
    word cal_coef_acc;      /* calibration coefficients access */
    word cal_coef_stat;     /* calibration coefficient status */
    struct dac
    {
	byte prescaler;	    /* conversion prescaler registers */
	byte control;	    /* conversion control registers */
	word counter;	    /* conversion counter registers */
	word fifo_data;	    /* FIFO data registers */
    } dac_map[8];

    word reserved;          /* reserved for factory use */
};
#else			    /* Little ENDIAN structure */
struct map236
{
    byte start_status;	    /* conversion start / status register */
    byte reset_reg;	    /* reset register bit 7 */
    byte int_vector;	    /* interrupt vector register */
    byte int_status;	    /* interrupt status register */
    word cal_coef_acc;      /* calibration coefficients access */
    word cal_coef_stat;     /* calibration coefficient status */
    struct dac
    {
	byte control;	    /* conversion control registers */
	byte prescaler;	    /* conversion prescaler registers */
	word counter;	    /* conversion counter registers */
	word fifo_data;	    /* FIFO data registers */
    } dac_map[8];

    word reserved;          /* reserved for factory use */
};
#endif /* SELECT_BIG_ENDIAN */




/*
    Defined below is the structure which is used for the configuration command
*/

struct conf_blk
{
    struct map236 *brd_ptr;		/* pointer to base address of board */
    char slotLetter;		        /* IP slot letter */
    int nHandle;			/* handle to an open carrier board */
    BOOL bCarrier;			/* flag indicating a carrier is open */
    BOOL bInitialized;			/* flag indicating ready to talk to board */	
    byte id_prom[33];			/* id prom contents */
    int current_channel;		/* current channel number */
    int mode[NUM_CHAN];			/* single or continuous */
    int trigger[NUM_CHAN];		/* channel triggering */
    int interrupt_enable[NUM_CHAN];	/* interrupt enable */
    int threshold[NUM_CHAN];		/* empty threshold */
    int range[NUM_CHAN];		/* channel range storage */
    int prescaler[NUM_CHAN];		/* timer prescaler */
    word counter[NUM_CHAN];		/* conversion counter */
    short coef_bufb10[NUM_CHAN][2];	/* offset/gain buffer start */
    short coef_bufu10[NUM_CHAN][2];	/* offset/gain buffer start */
    short coef_bufb5[NUM_CHAN][2];	/* offset/gain buffer start */
    short ideal_data[NUM_CHAN][MAXSIZE];/* allocate data storage area */
    short cor_data[NUM_CHAN][MAXSIZE];  /* allocate data storage area */
    short *head_ptr[NUM_CHAN];		/* head pointer of write buffer */
    short *tail_ptr[NUM_CHAN];		/* tail pointer of write buffer */
    short *current_ptr[NUM_CHAN];	/* current data pointer of write buffer */
    int write_size[NUM_CHAN];		/* write buffer data size */
    byte interrupt_vector;		/* storage for interrupt vector */
    byte interrupt_flag;		/* interrupt status flag */
};






/*
    DECLARE MODULES CALLED:
*/

void wro236(struct conf_blk *,int channel);	/* performs the write output function */
void rmid236(struct conf_blk *);				/* read I.D. */
void cd236(struct conf_blk *);					/* correct DAC output data */
void cnfg236(struct conf_blk *);				/* configure hardware */
long get_param(void);							/* input a parameter */
void setconf236(struct conf_blk *);				/* get/set configuration parameters */

int ReadOffsetError(struct conf_blk *,
					int channel, int Range);	/* Read the offset error for the specified channel */
int ReadGainError(struct conf_blk *,
					int channel, int Range);	/* Read the gain error for the specified channel */
void WriteCoefficients(struct conf_blk *,
				int channel,word Offset,
				word Gain, int Range);			/* Write the gain and offset error coefficients */
void readid(struct conf_blk *);					/* "Read I.D." and print result to the console */
void reset_board(struct conf_blk *);			/* reset the board */
void reset_channel(struct conf_blk *);			/* reset a channel on the board */
void start_single(struct conf_blk *);			/* start conversions on the board */
void start_multiple(struct conf_blk *, byte mask);		/* start conversions on the board */
void stop_multiple(struct conf_blk *, byte mask);		/* reset one or more channels on the board */

/* ISR */

void isr_236(void* pData);	/* interrupt handler for IP236 */

