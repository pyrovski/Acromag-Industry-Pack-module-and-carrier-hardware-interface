

/*
{+D}
    SYSTEM:         Software for ip482
    
    FILE NAME:      ip482.h

    VERSION:	    A

    CREATION DATE:  10/10/03

    DESIGNED BY:    F.M.

    CODED BY:	    F.M.

    ABSTRACT:	    This module contains the definitions and structures
                    used by the ip482 library.

    CALLING
	SEQUENCE:

    MODULE TYPE:    header file

    I/O RESOURCES:

    SYSTEM
	RESOURCES:

    MODULES
	CALLED:

    REVISIONS:

DATE	 BY	PURPOSE
-------  ----	------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

    This module contains the definitions and structures used by
    the ip482 library.
*/

/*
    DEFINITIONS:
*/




/*
   INTERRUPT VECTOR
*/

#ifndef Success
#define	Success			0
#define	ParameterOutOfRange	1	/* Parameter in error */
#define	InvalidPointer		2	/* Flag NULL pointers */
#define	DataDirection		3	/* data direction error */
#define	TimeOut			4	/* time-out error */
#endif /* Success */


#ifndef BUILDING_FOR_KERNEL
#define VECTOR0      0xB0       /* interrupt vector for port 0 */

#define INT_REQUEST_0  0	/* interrupt request 0 line on carrier */
#define INT_REQUEST_1  1	/* interrupt request 1 line on carrier */


/*
	Mode and option selection enumerations
*/

enum
{
	None	  = 0,          /* disable counter */
	QuadPosition  = 1,      /* quadrature position measurement */
	PulseWidthMod = 2,      /* pulse width modulation */
	Watchdog  = 3,          /* watchdog function */
	EventCounting   = 4,    /* event counting/frequency measurement */
	InPulseWidthMeasure = 5,/* input pulse width measurement */
	InPeriodMeasure  = 6,   /* input period measurement */
	OneShot   = 7,          /* One-Shot output pluse */

	OutPolLow = 0,          /* output polarity active low */
	OutPolHi  = 1,          /* output polarity active high */

	InABCPolDisabled = 0,	/* disabled */
	InABCPolLow  = 1,       /* input polarity active low */
	InABCPolHi   = 2,       /* input polarity active high */
	InABCUpDown  = 3,       /* up/down */
	InABCGate_off  = 3,     /* gate off */
	InAX4  = 3,
	InCQuadMax  = 4,

	InC0_5Mhz    = 0,       /* internal 0.5MHZ@8MHZ, 2MHZ@32MHZ  clock */
	InC1Mhz      = 1,       /* internal 1MHZ@8MHZ, 4MHZ@32MHZ clock */
	InC2Mhz      = 2,       /* internal 2MHZ@8MHZ, 8MHZ@32MHZ clock */
	InC4Mhz      = 3,       /* internal 4MHZ@8MHZ, 16MHZ@32MHZ clock */
	InC8Mhz      = 4,       /* internal 8MHZ@8MHZ, 32MHZ@32MHZ clock */
	ExClock      = 5,       /* external clock */

	DebounceOn = 1,		/* Debounce enabled */
	DebounceOff = 0,	/* Debounce disabled */

	IntDisable= 0,          /* disable interrupt */
	IntEnable = 1,          /* interrupt enabled */

	IpCarrierClock8MHZ = 0,	/* 8MHZ ip clock from carrier */
	IpCarrierClock32MHZ = 1	/* 32MHZ ip clock from carrier */
};

#endif /* BUILDING_FOR_KERNEL */

struct map482		   /* Memory map of the counter I/O board section */
{
	word control;	   /* control register */
	word InterruptStatusReg;
	word CounterTrigger;
	word CounterStop;
	word CounterControl1;
	word CounterControl2;
	word CounterControl3;
	word CounterControl4;
	word CounterControl5;
	word CounterControl6;
	word CounterControl7;
	word CounterControl8;
	word CounterControl9;
	word CounterControl10;
	word CounterReadBack1;
	word CounterReadBack2;
	word CounterReadBack3;
	word CounterReadBack4;
	word CounterReadBack5;
	word CounterReadBack6;
	word CounterReadBack7;
	word CounterReadBack8;
	word CounterReadBack9;
	word CounterReadBack10;
	word CounterConstantA1;
	word CounterConstantA2;
	word CounterConstantA3;
	word CounterConstantA4;
	word CounterConstantA5;
	word CounterConstantA6;
	word CounterConstantA7;
	word CounterConstantA8;
	word CounterConstantA9;
	word CounterConstantA10;
	word CounterConstantB1;
	word CounterConstantB2;
	word CounterConstantB3;
	word CounterConstantB4;
	word CounterConstantB5;
	word CounterConstantB6;
	word CounterConstantB7;
	word CounterConstantB8;
	word CounterConstantB9;
	word CounterConstantB10;
	word DigitalInput;		/* low bits only */	
	word DigitalOut;		/* low bits only */	
	word InterruptVectorReg;	/* low 8 bits only */
};



struct ip482
{
    struct map482 *brd_ptr;	/* base address of the I/O board */
    char slotLetter;            /* IP slot letter */
    int nHandle;		/* handle to an open board */
    BOOL bCarrier;		/* flag indicating a carrier is open */
    BOOL bInitialized;		/* flag indicating ready to talk to board */	
    byte m_Mode[10];		/* the counter mode */
    BOOL m_OutputPolarity[10];	/* output polarity */
    byte m_InputAPolarity[10];	/* input polarity A */
    byte m_InputBPolarity[10];	/* input polarity B */
    byte m_InputCPolarity[10];	/* input polarity C */
    byte m_ClockSource[10];	/* clock source */
    BOOL m_Debounce[10];	/* debounce */
    BOOL m_InterruptEnable[10];	/* interrupt enable */
    word m_CounterConstantA[10];/* constant registers are write only copies are here */
    word m_CounterConstantB[10];/* constant registers are write only copies are here */
    BOOL m_IpClock;	/* ip clock from Ip */
    byte m_InterruptVector;	/* interrupt vector */
    word event_status;		/* interrupt event status */
    byte counter_num;		/* counter being serviced */
    byte id_prom[33];		/* id prom contents used by read status */
};




/*
    DECLARE MODULES CALLED:
*/

void trig(struct ip482 *);				/* routine to software trigger */
void stop_trig(struct ip482 *);			/* routine to software stop */
void setconf(struct ip482 *);			/* routine to set up the configuration param. block */
void selectcnt(struct ip482 *);			/* routine to examine/change the current counter */
void selectcon(struct ip482 *);			/* routine to examine/change the counter constant*/
void readstat(struct ip482 *);			/* routine which calls the Read Status Command */
word build_control(struct ip482 *,
			int counter);				/* builds counter control words */
void dig_io(struct ip482 *);


int ConfigureCounterTimer(struct ip482 *,
			int counter);				/* Configure counter timer */
int ReadCounter(struct ip482 *,
			int counter, word *val);	/* Read counter */
int WriteCounterConstant(struct ip482 *,
			int counter);				/* Write counter constant to board register */
int StopCounter(struct ip482 *,
			int counter);				/* Stop a counter */
int DisableInterrupt(struct ip482 *,
			int counter);				/* Disable counter interrupts */
int GetInterruptVector(struct ip482 *,
			byte *vector);				/* get interrupt vector */
int SetInterruptVector(struct ip482 *,
			byte vector);				/* set interrupt vector */
int GetMode(struct ip482 *,
			int counter, byte *mode);	/* get mode */
int SetMode(struct ip482 *,
			int counter, byte mode);	/* set mode */
int GetInterruptEnable(struct ip482 *,
			int counter, BOOL *enable);	/* get interrupt enable */
int SetInterruptEnable(struct ip482 *,
			int counter, BOOL enable);	/* set interrupt enable */
int GetClockSource(struct ip482 *,
			int counter, byte *source);	/* get clock source */
int SetClockSource(struct ip482 *,
			int counter, byte source);	/* set clock source */
int GetOutputPolarity(struct ip482 *,
			int counter, BOOL *polarity);/* get output polarity */
int SetOutputPolarity(struct ip482 *,
			int counter, BOOL polarity);/* set output polarity */
int GetInputAPolarity(struct ip482 *,
			int counter, byte *polarity);/* get input A polarity */
int SetInputAPolarity(struct ip482 *,
			int counter, BOOL polarity);/* set input A polarity */
int GetInputBPolarity(struct ip482 *,
			int counter, byte *polarity);/* get input B polarity */
int SetInputBPolarity(struct ip482 *,
			int counter, BOOL polarity);/* set input B polarity */
int GetInputCPolarity(struct ip482 *,
			int counter, byte *polarity);/* get input C polarity */
int SetInputCPolarity(struct ip482 *,
			int counter, BOOL polarity);/* set input C polarity */
int StartCounter(struct ip482 *,
			int counter);				/* start counter */
int StartSimultaneousCounters(struct ip482 *,
			word mask);					/* start simultaneous counters */
int StopCounter(struct ip482 *,
			int counter);				/* stop counter */
int StopSimultaneousCounters(struct ip482 *,
			word mask);					/* stop simultaneous counters */
int GetCounterConstantA(struct ip482 *,
			int counter, word *val);	/* get constant */
int SetCounterConstantA(struct ip482 *,
			int counter, word val);		/* set constant */
int GetCounterConstantB(struct ip482 *,
			int counter, word *val);	/* get constant */
int SetCounterConstantB(struct ip482 *,
			int counter, word val);		/* set constant */
int ReadCounterControl(struct ip482 *,
			int counter, word *val);
int WriteCounterControl(struct ip482 *,
			int counter, word val);
int GetIpClock(struct ip482 *,
			BOOL *value);				/* get Ip clock */
int SetIpClock(struct ip482 *,
			BOOL value);				/* set Ip clock */
long get_param(void);					/* input a parameter */


/*
    declare interrupt handler
*/

void isr_482(void* pData);	/*  Interrupt Handler for ip482 */
