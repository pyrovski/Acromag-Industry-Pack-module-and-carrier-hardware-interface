
/*
{+D}
    SYSTEM:         Software for IP480
    
    FILE NAME:      ip480.h

    VERSION:	    A

    CREATION DATE:  05/19/98

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M.

    ABSTRACT:	    This module contains the definitions and structures
                    used by the IP480 library.

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
    the IP480 library.
*/

/*
    DEFINITIONS:
*/

typedef unsigned short UWORD;

#ifndef Success
#define	Success			0
#define	ParameterOutOfRange	1	/* Parameter in error */
#define	InvalidPointer		2	/* Flag NULL pointers */
#define	DataDirection		3	/* data direction error */
#define	TimeOut			4	/* time-out error */
#endif /* Success */

#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */



/*
	Mode and option selection enumerations
*/

enum
	{
		None	  = 0,          /* disable counter */
		OutPulse  = 1,          /* output pulse waveform */
		OutSquare = 2,          /* output square waveform */
		Watchdog  = 3,          /* watchdog function */
		InEvent   = 4,          /* input event */
		InWidth   = 5,          /* input width */
		InPeriod  = 6,          /* input period */
		OneShot   = 7,          /* One-Shot output pluse */

		OutPolLow = 0,          /* output polarity active low */
		OutPolHi  = 1,          /* output polarity active high */

		InPolLow  = 0,          /* input polarity active low */
		InPolHi   = 1,          /* input polarity active high */

		InTrig    = 0,          /* internal trigger */
		ExtPolLow = 1,          /* external trigger polarity */
		ExtPolHi  = 2,          /* external trigger polarity */

		IntDisable= 0,          /* disable interrupt */
		IntEnable = 1,          /* interrupt enabled */

		CtrSize16 = 0,          /* counter 16 bit */
		CtrSize32 = 1,          /* counter 32 bit */

		InC1Mhz   = 0,          /* internal 1MHZ clock */
		InC4Mhz   = 1,          /* internal 4MHZ clock */
		InC8Mhz   = 2,          /* internal 8MHZ clock */
		ExClock   = 3,          /* external clock */

		WDIntLd   = 0,          /* watchdog timer autoloaded (internal ) */
		WDExtLd   = 1,          /* watchdog loaded when externally triggered */

		Latch32   = 1,          /* latch lower 16 bits on read of 32 bit reg */

		Reset	  = 1,

		LatchOn	  = 1,		/* 32 bit data latch enabled */
		LatchOff  = 0,		/* 32 bit data latch disabled */

		DebounceOn = 1,		/* Debounce enabled */
		DebounceOff = 0		/* Debounce disabled */
	};



enum			   /* IP480 direct register offsets in bytes */
	{
		CounterControl1  = 0,
		CounterControl2  = 2,
		CounterControl3  = 4,
		CounterControl4  = 6,
		CounterControl5  = 8,
		CounterControl6  = 10,
		
		CounterReadBack1 = 12,
		CounterReadBack2 = 14,
		CounterReadBack3 = 16,
		CounterReadBack4 = 18,
		CounterReadBack5 = 20,
		CounterReadBack6 = 22,

		CounterConstant1 = 24,
		CounterConstant2 = 26,
		CounterConstant3 = 28,
		CounterConstant4 = 30,
		CounterConstant5 = 32,
		CounterConstant6 = 34,

		TriggerControl   = 36,	/* bits 05...00 */
		ResetRegister	 = 38,	/* bit 0 */
		ReadBackLatch    = 40,
		InterruptPending = 42,	/* bits13...08 */
		InterruptVectorReg = 44
	};





struct cblk480
 {
    byte *brd_ptr;               /* base address of board */
    UWORD m_CounterConstant[7];	 /* constant registers are write only copies are here */
    byte m_Mode[7];		 /* the counter mode */
    BOOL m_Debounce[7];		 /* only 1 & 2 have debounce */
    BOOL m_OutputPolarity[7]; 	 /* output polarity */
    BOOL m_InputPolarity[7];	 /* input polarity */
    byte m_Trigger[7];		 /* triggering internal/external-polarity */
    BOOL m_CounterSize[7];	 /* 16/32 bit counter size flags */
    BOOL m_ClockSource[7];	 /* clock source */
    BOOL m_WDLoad[7];		 /* watchdog timer internal/external loaded */
    BOOL m_InterruptEnable[7];	 /* interrupt enable */
    byte m_InterruptVector;	 /* interrupt vector */
    byte event_status;		 /* interrupt event status */
    int num_chan;		 /* is this a 2 or 6 channel board */
    byte id_prom[33];		 /* id prom contents used by read status */
    byte counter_num;		 /* counter being serviced */
    char slotLetter;		 /* IP slot letter */
    int nHandle;		 /* handle to an open carrier board */
    BOOL bCarrier;		 /* flag indicating a carrier is open */
    BOOL bInitialized;		 /* flag indicating ready to talk to board */	
};





/*
    DECLARE MODULES CALLED:
*/

void trig480(struct cblk480 *);		/* routine to software trigger the IP480 */
void setconf480(struct cblk480 *);		/* routine to set up the configuration param. block */
void selectcnt480(struct cblk480 *);		/* routine to examine/change the current counter */
void selectcon480(struct cblk480 *);		/* routine to examine/change the counter constant*/
void readstat480(struct cblk480 *);		/* routine which calls the Read Status Command */
void rmid480(struct cblk480 *);		/* reads and stores the contents of the id prom */
BOOL islongcounter480(struct cblk480 *,
			int counter);				/* test for 32bit counter */
UWORD build_control480(struct cblk480 *,
			int counter);				/* builds counter control words */

int ConfigureCounterTimer480(struct cblk480 *,
 			int counter);				/* Configure counter timer */
int ReadCounter480(struct cblk480 *,
			int counter, ULONG *val);	/* Read 16/32 bit counter */
int WriteCounterConstant480(struct cblk480 *,
			int counter);				/* Write counter constant to board register */
int StopCounter480(struct cblk480 *,
			int counter);				/* Stop a counter */
int DisableInterrupt480(struct cblk480 *,
			int counter);				/* Disable counter interrupts */
int GetMode480(struct cblk480 *,
			int counter,
			byte *mode);				/* get mode */
int SetMode480(struct cblk480 *,
			int counter, byte mode);	/* set mode */
int GetInterruptVector480(struct cblk480 *,
			byte *vector);				/* get interrupt vector */
int SetInterruptVector480(struct cblk480 *,
			byte vector);	/* set interrupt vector */
int GetInterruptEnable480(struct cblk480 *,
			int counter, BOOL *enable);	/* get interrupt enable */
int SetInterruptEnable480(struct cblk480 *,
			int counter, BOOL enable);	/* set interrupt enable */
int GetCounterSize480(struct cblk480 *,
			int counter, BOOL *size);	/* get counter size 16/32 bit */
int SetCounterSize480(struct cblk480 *,
			int counter, BOOL size);	/* set counter size 16/32 bit */
int GetTriggerSource480(struct cblk480 *,
			int counter, byte *trigger);/* get trigger source */	
int SetTriggerSource480(struct cblk480 *,
			int counter, byte trigger);	/* set trigger source */	
int GetClockSource480(struct cblk480 *,
			int counter, byte *source);	/* get clock source */
int SetClockSource480(struct cblk480 *,
			int counter, byte source);	/* set clock source */
int GetDebounce480(struct cblk480 *,
			int counter, BOOL *debounce);/* get debounce */
int SetDebounce480(struct cblk480 *,
			int counter, BOOL debounce); /* set debounce */
int GetWatchdogLoad480(struct cblk480 *,
			int counter, BOOL *load);	/* get Watchdog load */
int SetWatchdogLoad480(struct cblk480 *,
			int counter, BOOL load);	/* set Watchdog load */
int GetOutputPolarity480(struct cblk480 *,
			int counter, BOOL *polarity);	/* get output polarity */
int SetOutputPolarity480(struct cblk480 *,
			int counter, BOOL polarity);	/* set output polarity */
int GetInputPolarity480(struct cblk480 *,
			int counter, BOOL *polarity);	/* get input polarity */
int SetInputPolarity480(struct cblk480 *,
			int counter, BOOL polarity);	/* set input polarity */
int StartCounter480(struct cblk480 *,
			int counter);				/* start counter */
int StartSimultaneousCounters480(struct cblk480 *,
			byte mask);					/* start simultaneous counters */
int GetCounterConstant480(struct cblk480 *,
			int counter, ULONG *val);	/* get constant */
int SetCounterConstant480(struct cblk480 *,
			int counter, ULONG val);	/* set constant */
long get_param(void);		/* input a parameter */
/* ISR */

void isr_480(void* pData);	/* interrupt handler for IP480 */

