
/*
{+D}
    SYSTEM:		    Software for IP340

    FILE NAME:		    ip340.h

    VERSION:		    A

    CREATION DATE:	    07/07/99

    DESIGNED BY:	    F.J.M.

    CODED BY:		    F.J.M.

    ABSTRACT:		    This module contains the definitions and structures
			    used by the IP340 library.

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
    the IP340 library.
*/

/*
    DEFINITIONS:
*/

typedef short int sint;


/* definitions */
#ifndef BUILDING_FOR_KERNEL
#define SA_SIZE 512		/* # of elements in data arrays */
#define AVE_SAMPLES 32

#define RESET_BOARD (unsigned short)0x8000 /* reset */

#define T_DISABLE   0		/* EXT. trigger disabled */
#define TI_SELECT   1		/* trigger direction (input)*/
#define TO_SELECT   2		/* trigger direction (output)*/

#define DI_SELECT   0		/* channel select */
#define AZ_SELECT   1		/* auto-zero select */
#define CAL_SELECT  2		/* calibration select */

#define DISABLE     0		/* disable conversions */
#define SINGLE      1		/* single conversion mode */
#define CONTIN      2		/* continuous conversion mode */

#define INT_DIS     0		/* disable interrupt */
#define INT_FIFO    1		/* interrupt FIFO threshold */

#define START_CONV  0x0001	/* start conversion */

#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */
#endif /* BUILDING_FOR_KERNEL */


/*
    STRUCTURES:

    Defined below is the memory map template for the IP340 Board.
    This data structure provides access to the various registers on the board.
*/


struct map340
{
    word cntl_reg;	    /* board control register */
    word chnl_cntl;	    /* channel enable control register */
    word timer_low_lsw;	    /* low conversion timer registers */
    word timer_low_msw;	    /* low conversion timer registers - only low 8 bits used */
    word timer_high_lsw;    /* high conversion timer register */
    word timer_high_msw;    /* high conversion timer register - only low 8 bits used */
    word threshold;	    /* FIFO full interrupt threshold */
    word vect_reg;	    /* interrupt vector register - only low 8 bits used */
    word strt_reg;	    /* start conversion register */
    sint fifo_data;	    /* data */
    word chan_tag;	    /* channel tag */
    word cal_coef_acc;      /* calibration coefficients access */
    word cal_coef_stat;     /* calibration coefficient status */
    word reserved;          /* reserved for factory use */
};


/*
    Defined below is the structure which is used for the read analog command
*/

struct cblk340
{
    struct map340 *brd_ptr;	/* pointer to base address of board */
    char slotLetter;		/* IP slot letter */
    int nHandle;		/* handle to an open carrier board */
    BOOL bCarrier;		/* flag indicating a carrier is open */
    BOOL bInitialized;		/* flag indicating ready to talk to board */	
    sint *s_count;		/* number of samples in each array */
    sint *s_raw_buf[16];	/* raw buffer start */
    sint *s_az_buf;		/* auto zero data storage areas */
    sint *s_cal_buf;		/* calibration buffers */
    long *s_cor_buf[16];	/* corrected buffer start */
    unsigned long conv_timer0;	/* conversion timer */
    unsigned long conv_timer1;	/* conversion timer */
    word control;		/* board control register used by read status */
    word channel_enable;	/* channels bitmap */
    word threshold;		/* FIFO threshold */
    double ref_v;		/* 5 Volt reference constant */
    byte vector;		/* interrupt vector */
    byte trigger;		/* triggering option software/external */
    byte acq_mode;		/* the acquisition mode */
    byte scan_mode;		/* the scan mode */
    byte int_mode;		/* the interrupt mode */
    byte id_prom[33];		/* id prom contents used by read status */
};







/* Declare functions called */

double ReadReferenceValue(struct cblk340 *);
void WriteReferenceValue(struct cblk340 *,
						 double Value);
void rsts340(struct cblk340 *);		/* read board status */
void cnfg340(struct cblk340 *);		/* configure board */
void setconf340(struct cblk340 *);		/* routine to set up the configuration param. block */
void readstat340(struct cblk340 *);		/* routine which calls the Read Status Command */
int mccd340(struct cblk340 *);			/* routine to correct input data */
void calibrate340(struct cblk340 *,
			int mode);					/* get calibration data */
void convert340(struct cblk340 *);		/* get input data */
long get_param(void);					/* input a parameter */
/* ISR */

void isr_340(void* pData);	/* interrupt handler for IP340 */

