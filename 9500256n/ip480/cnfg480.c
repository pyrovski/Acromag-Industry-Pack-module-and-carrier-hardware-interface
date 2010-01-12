
#include "../carrier/apc8620.h"
#include "ip480.h"



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetInterruptVector480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This file retrieves the interrupt vector value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Get interrupt vector

 Get the value of the interrupt vector register.
 The current value is placed at the address specified by vector.
 Returns Invalid pointer if 'vector' is NULL, else returns success.
*/


int GetInterruptVector480(c_blk, vector )
   struct cblk480 *c_blk;
   byte *vector;
{
	if( vector == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*vector = c_blk->m_InterruptVector;
	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetInterruptVector480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This file stores the interrupt vector value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Set interrupt vector

 Set the value of the interrupt vector register.
 Returns success.
*/

int SetInterruptVector480(c_blk, vector)
   struct cblk480 *c_blk;
   byte vector;
{
	c_blk->m_InterruptVector = vector;
	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        ReadCounter480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This file retrieves the read back register value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

	Read 16/32 bit counter.

  Read the value of a 16 or 32 bit counter.
  The current value of the counter is placed at
  the address specified by val.
  Valid values for "counter" are 2, 4 and 6 for 32 bit.
  Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
  Returns parameter out of range error if the counter value is invalid.
  Returns invalid pointer error if val is NULL, else returns success.
*/


int ReadCounter480(c_blk,  counter,  val )
   struct cblk480 *c_blk;
   int counter;
   ULONG *val;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( val == NULL )			/* valid range ? */
		return(	InvalidPointer );


	/* determine the requested counter's data size */
	if( islongcounter480(c_blk, counter ) )    /* is 32 bit ? */
 	{
	    /* read MS word of the counter */
            *val = (ULONG)input_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterReadBack1 + ((counter - 2) << 1)));
	    *val <<= 16;					/* form MS word */

	    /* set the readback latch to read the remaining 16 bits from the counter */
            output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + ReadBackLatch), LatchOn);

            *val |= (ULONG)input_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterReadBack1 + ((counter - 1) << 1)));
	    /* clear the readback latch */
            output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + ReadBackLatch), LatchOff);
	}
	else
	{
	 /* read 16 bit word from the counter */
            *val = (ULONG)input_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterReadBack1 + ((counter - 1) << 1)));
	}
	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetCounterConstant480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This file retrieves the counter constant register
                        value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

	Get 16/32 bit counter constant.

  Read the value of a 16 or 32 bit counter constant.
  The current value of the counter constant register is placed at
  the address specified by val.
  Valid values for "counter" are 2, 4 and 6 for 32 bit.
  Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
  Returns parameter out of range error if the counter value is invalid.
  Returns invalid pointer error if val is NULL, else returns success.
*/


int GetCounterConstant480(c_blk,  counter,  val )
  struct cblk480 *c_blk;
  int counter;
  ULONG *val;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( val == NULL )			/* valid range ? */
		return(	InvalidPointer );


	/* determine the requested counter's data size */
	if( islongcounter480(c_blk, counter ) )    /* is 32 bit ? */
	{
	    /* read MS word of the counter */
	    *val = (ULONG)c_blk->m_CounterConstant[counter - 1];
	    *val <<= 16;					/* form MS word */
		/* OR in LS word */
	    *val |= (ULONG)c_blk->m_CounterConstant[counter];
	}
	else	/* read 16 bit word */
	    *val = (ULONG)c_blk->m_CounterConstant[counter]; /* LS word */

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetCounterConstant480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This file stores the counter constant value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

	Set counter constant.

  Set the counter constant.
  Valid values for "counter" are 2, 4 and 6 for 32 bit.
  Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
  Returns parameter out of range error if the counter value is invalid
  else returns success.
*/


int SetCounterConstant480(c_blk,  counter, val )
  struct cblk480 *c_blk;
  int counter;
  ULONG val;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );


	/* determine the requested counter's data size */
	if( islongcounter480(c_blk, counter ) )              /* is 32 bit ? */
	{
	    /* save values in member vars */
            c_blk->m_CounterConstant[counter - 1] = (UWORD)(val >> 16); /* save MS word */
            c_blk->m_CounterConstant[counter] = (UWORD)(val & 0xFFFF);  /* save LS word */
	}
	else
	{
	    /* save 16 bit value */
            c_blk->m_CounterConstant[counter] = (UWORD)(val & 0xFFFF); /* save LS word */
	}
	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        WriteCounterConstant480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine write the stored counter constant value
                        to the board registers.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

	Write counter constant to board register.

  Write a value to the counter constant register.
  Valid values for "counter" are 2, 4 and 6 for 32 bit.
  Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
  Returns parameter out of range error if the counter value is invalid
  else returns success.
*/


int WriteCounterConstant480(c_blk,  counter )
  struct cblk480 *c_blk;
  int counter;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );


	/* determine the requested counter's data size */
	if( islongcounter480(c_blk, counter ) )              /* is 32 bit ? */
	{
	    /* write values to hardware */
            output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterConstant1 + ((counter - 2) << 1)),
			c_blk->m_CounterConstant[counter - 1]);	/* MSword */

            output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterConstant1 + ((counter - 1) << 1)),
			c_blk->m_CounterConstant[counter]);		/* LSword */
	}
	else
	{
	 /* write 16 bit constant */
            output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterConstant1 + ((counter - 1) << 1)),
			c_blk->m_CounterConstant[counter] );
	}
	return( Success );
}





/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetMode480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This file retrieves the current MODE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Get mode value for counter

 The current value is placed at the address specified by mode.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the counter value is invalid.
 Returns Invalid pointer if 'mode' is NULL, else returns success.
*/

int GetMode480(c_blk,  counter,  mode )
  struct cblk480 *c_blk;
  int counter;
  byte *mode;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( mode == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*mode = c_blk->m_Mode[counter];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetMode480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine stores the MODE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Set mode

 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Valid values for "mode" are 0 thru 7.
 Returns parameter out of range error if the 'counter' or 'mode' value
 is invalid, else returns success.
*/

int SetMode480(c_blk,  counter,  mode )
  struct cblk480 *c_blk;
  int counter;
  byte mode;

{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if(  mode > OneShot )	  /* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_Mode[counter] = mode;

	return( Success );
}




/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetDebounce480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine gets the current DEBOUNCE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Get Debounce for counter 1 or 2 only

 The current value is placed at the address specified by mode.
 Valid values for "counter" is 2 for 32 bit.
 Valid values for "counter" are 1 and 2 for 16 bit.
 Returns parameter out of range error if the counter value is invalid.
 Returns Invalid pointer if 'debounce' is NULL, else returns success.
*/

int GetDebounce480(c_blk,  counter,  debounce )
  struct cblk480 *c_blk;
  int counter;
  BOOL *debounce;
{
	if( counter < 1 || counter > 2 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( debounce == NULL )				/* valid range ? */
		return(	InvalidPointer );

	*debounce = c_blk->m_Debounce[counter];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetDebounce480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine sets the DEBOUNCE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Set Debounce for counter 1 or 2 only

 Valid values for "counter" is 2 for 32 bit.
 Valid values for "counter" are 1 and 2 for 16 bit.
 Valid values for "debounce" are 0 and 1.
 Returns parameter out of range error if the 'counter' or 'debounce' value
 is invalid, else returns success.
*/

int SetDebounce480(c_blk,  counter,  debounce )
  struct cblk480 *c_blk;
  int counter;
  BOOL debounce;
{
	if( counter < 1 || counter > 2 )	/* valid range ? */
		return( ParameterOutOfRange );

	if(debounce < DebounceOff || debounce > DebounceOn )	/* valid range ? */
		return( ParameterOutOfRange);

	c_blk->m_Debounce[counter] = debounce;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetInterruptEnable480

    VERSION:                A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine gets the current INTERRUPT ENABLE
                        value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Get interrupt enable

 The current value is placed at the address specified by enable.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the counter value is invalid.
 Returns Invalid pointer if 'enable' is NULL, else returns success.
*/

int GetInterruptEnable480(c_blk,  counter,  enable )
  struct cblk480 *c_blk;
  int counter;
  BOOL *enable;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( enable == NULL )				/* valid range ? */
		return(	InvalidPointer );

	*enable = c_blk->m_InterruptEnable[counter];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetInterruptEnable480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine sets the INTERRUPT ENABLE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Set interrupt enable
 Bit 0 set in this register enables the corresponding counter interrupt.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' or 'enable'
 values are invalid.
*/

int SetInterruptEnable480(c_blk, counter, enable )
  struct cblk480 *c_blk;
  int counter;
  BOOL enable;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if(enable < IntDisable || enable > IntEnable )	/* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_InterruptEnable[counter] = enable;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetCounterSize480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine gets the current COUNTER SIZE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Get counter size

 Valid values for "counter" are 1, 2 and 3 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by 'size'.
 Returns Invalid pointer if 'size' is NULL, else returns success.
*/

int GetCounterSize480(c_blk, counter,  size )
  struct cblk480 *c_blk;
  int counter;
  BOOL *size;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( size == NULL )					/* valid range ? */
		return(	InvalidPointer );

	*size = c_blk->m_CounterSize[counter];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetCounterSize480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine sets the COUNTER SIZE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Set counter size

 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' or 'size'
 values are invalid.
*/

int SetCounterSize480(c_blk, counter,  size )
  struct cblk480 *c_blk;
  int counter;
  BOOL size;
{

	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( size < CtrSize16 || size > CtrSize32 )	/* valid range ? */
	    return( ParameterOutOfRange );

	/* only counters 2,4,6 can be 32 bit */
	if((counter == 1 || counter == 3 || counter == 5 ) && (size == CtrSize32))
		return( ParameterOutOfRange );

	/* if size is currently 32 bit allow flag to be set to 16 bit */
	if((counter == 1 || counter == 3 || counter == 5 ) && (c_blk->m_CounterSize[counter + 1] == CtrSize32))
		c_blk->m_CounterSize[counter + 1] = size;

	/* set size flag for high 16 bit counter */
        if((counter == 2 || counter == 4 || counter == 6 ) && (size == CtrSize32)) /* is 32 bit */
		c_blk->m_CounterSize[counter - 1] = size;

	c_blk->m_CounterSize[counter] = size;		/* 16 bit flags */
	return( Success );
}


/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetClockSource480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine gets the current CLOCK SOURCE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Get clock source

 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by 'source'.
 Returns Invalid pointer if 'source' is NULL, else returns success.
*/

int GetClockSource480(c_blk, counter,  source )
  struct cblk480 *c_blk;
  int counter;
  byte *source;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( source == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*source = c_blk->m_ClockSource[counter];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetClockSource480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine sets the CLOCK SOURCE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Set clock source

 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 Returns parameter out of range error if the source value is invalid
 else returns success.
*/

int SetClockSource480(c_blk, counter,  source )
  struct cblk480 *c_blk;
  int counter;
  byte source;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( source > ExClock )	    /* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_ClockSource[counter] = source;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetTriggerSource480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine gets the current TRIGGER SOURCE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Get trigger source

 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'trigger' value is invalid.
 The current value is placed at the address specified by 'trigger'.
 Returns Invalid pointer if 'trigger' is NULL, else returns success.
*/

int GetTriggerSource480(c_blk, counter,  trigger )
  struct cblk480 *c_blk;
  int counter;
  byte *trigger;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( trigger == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*trigger = c_blk->m_Trigger[counter];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetTriggerSource480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine sets the TRIGGER SOURCE value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Set trigger source

 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 Returns parameter out of range error if the trigger value is invalid
 else returns success.
*/

int SetTriggerSource480(c_blk, counter,  trigger )
  struct cblk480 *c_blk;
  int counter;
  byte trigger;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if(  trigger > ExtPolHi )/* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_Trigger[counter] = trigger;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetWatchdogLoad480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine gets the current WATCHDOG LOAD Value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Get Watchdog load

 Bit 0 set in a register enables the corresponding external watchdog load.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by load.
 Returns Invalid pointer if 'load' is NULL, else returns success.
*/

int GetWatchdogLoad480(c_blk, counter,  load )
  struct cblk480 *c_blk;
  int counter;
  BOOL *load;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( load == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*load = c_blk->m_WDLoad[counter];

	return( Success );
}


/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetWatchdogLoad480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine sets the WATCHDOG LOAD Value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Set Watchdog load

 Bit 0 set in a register enables the corresponding external watchdog load.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' or 'load' value
 are invalid else returns success.
*/

int SetWatchdogLoad480(c_blk, counter,  load )
  struct cblk480 *c_blk;
  int counter;
  BOOL load;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( load < WDIntLd || load > WDExtLd )	/* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_WDLoad[counter] = load;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetOutputPolarity480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine gets the current OUTPUT POLARITY Value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Get output polarity

 Bit 0 set in a register enables the active LOW output polarity.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by polarity.
 Returns Invalid pointer if 'polarity' is NULL, else returns success.
*/

int GetOutputPolarity480(c_blk, counter,  polarity )
  struct cblk480 *c_blk;
  int counter;
  BOOL *polarity;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*polarity = c_blk->m_OutputPolarity[counter];

	return( Success );
}


/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetOutputPolarity480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine sets the OUTPUT POLARITY Value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Set output polarity

 Bit 0 set in a register enables the active LOW output polarity.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' or polarity values
 are invalid else returns success.
*/

int SetOutputPolarity480(c_blk, counter,  polarity )
  struct cblk480 *c_blk;
  int counter;
  BOOL polarity;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity < OutPolLow || polarity > OutPolHi )	/* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_OutputPolarity[counter] = polarity;

	return( Success );
  }




/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetInputPolarity480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine gets the current INPUT POLARITY Value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Get input polarity

 Bit 0 set in a register enables the active LOW input polarity.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by polarity.
 Returns Invalid pointer if 'polarity' is NULL, else returns success.
*/

int GetInputPolarity480(c_blk, counter,  polarity )
  struct cblk480 *c_blk;
  int counter;
  BOOL *polarity;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*polarity = c_blk->m_InputPolarity[counter];

	return( Success );
}


/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetInputPolarity480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine sets the INPUT POLARITY Value.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Set input polarity

 Set the values of the input polarity registers.
 Bit 0 set in a register enables the active LOW input polarity.
 Returns parameter out of range error if the polarity value is invalid
 else returns success.
 Returns Invalid pointer if 'polarity' is NULL, else returns success.
*/

int SetInputPolarity480(c_blk, counter,  polarity )
  struct cblk480 *c_blk;
  int counter;
  BOOL polarity;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity < InPolLow || polarity > InPolHi )	/* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_InputPolarity[counter] = polarity;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        StopCounter480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine stops the counter/timer function.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Stop counter

 disables a counter timer.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the counter value is invalid
 else returns success.
*/

int StopCounter480(c_blk, counter )
  struct cblk480 *c_blk;
  int counter;
{
        UWORD control;

	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );


	/* read the control value from hardware */
        control = input_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterControl1 + ((counter - 1) << 1)));
	control &= 0xfff8;		/* force mode bits to 0 */
	/* write out stop control value */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterControl1 + ((counter - 1) << 1)), control);

	return( Success );
}




/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        DisableInterrupt480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine disables the interrupt the current
                        channel.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Disable counter interrupts

 disables only timer interrupts.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the counter value is invalid
 else returns success.
*/

int DisableInterrupt480(c_blk, counter )
  struct cblk480 *c_blk;
  int counter;
{
        UWORD control;

	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );


	/* read the control value from hardware */
        control = input_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterControl1 + ((counter - 1) << 1)));
	control &= 0xff7f;		/* force interrupt bit to 0 */
	/* write out stop control value */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterControl1 + ((counter - 1) << 1)), control);

	return( Success );
}






/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        StartCounter480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine will issue a software trigger to one
                        channel.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Start a counter

 Starts one counter timer.
 Valid values for "counter" are 2, 4 and 6 for 32 bit.
 Valid values for "counter" are 1, 2, 3, 4, 5 and 6 for 16 bit.
 Returns parameter out of range error if the counter value is invalid
 else returns success.
*/

int StartCounter480(c_blk, counter )
  struct cblk480 *c_blk;
  int counter;
{
	if( counter < 1 || counter > 6 )	/* valid range ? */
		return( ParameterOutOfRange );

        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + TriggerControl), 1 << (counter - 1)); /* Trigger counter */

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    VERSION:            A

    CREATION DATE:      04/01/09

    CODED BY:           FJM

    ABSTRACT:           This routine will issue a software trigger to one channel.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

    DATE       BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Blocking Start Counter

 Starts one counter timer then blocks.
 Returns parameter out of range error if the counter value is invalid
 else returns success.
*/

int BlockingStartCounter(c_blk, counter )
  struct cblk480 *c_blk;
  int counter;
{
    if( counter < 1 || counter > 6 )	/* valid range ? */
	return( ParameterOutOfRange );

    blocking_start_convert(c_blk->nHandle, (word*)(c_blk->brd_ptr + TriggerControl), 1 << (counter - 1)); /* Trigger counter */

    return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        StartSimultaneousCounters480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine will issue a software trigger to
                        multiple channels.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Start Simultaneous counter

 Starts one counter, any combination, or all counter timers.
 Bit mask of counters to start bit5 ... bit0
 Returns success.
*/

int StartSimultaneousCounters480(c_blk, mask )
  struct cblk480 *c_blk;
   byte mask;
{
        output_word( c_blk->nHandle, (word *)( c_blk->brd_ptr + TriggerControl), (UWORD)(mask & 0x3F));
	return( Success );
}





/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        ConfigureCounterTimer480

    VERSION:		A

    CREATION DATE:      02/06/97

    CODED BY:           FJM/RH

    ABSTRACT:           This routine will poll all of the control options,
                        create the control word and write it out to the board.

    CALLING SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM RESOURCES:

    MODULES CALLED:

    REVISIONS:

      DATE	BY	PURPOSE
    --------   -----	---------------------------------------------------

{-D}
*/

/*
    MODULES FUNCTION DETAILS

 Configure counter timer
*/

int ConfigureCounterTimer480(c_blk, counter )
  struct cblk480 *c_blk;
  int counter;
{

    /* stop counter and disable interrupts */
    /* determine the requested counter's data size */
    if( islongcounter480(c_blk, counter ) )              /* is 32 bit ? */
    {
	/* write out a 0 control value to disable the 32 bit counter */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterControl1 + ((counter - 1) << 1)), 0);
	/* clear any pending interrupts from 32 bit counter */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + InterruptPending), ~((1 << (counter - 1)) << 8));

	/* disable the 16 bit counter also */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterControl1 + ((counter - 2) << 1)), 0);
	/* clear any pending interrupts from 16 bit counter */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + InterruptPending), ~((1 << (counter - 2)) << 8));

	/* Write out the interrupt vector */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + InterruptVectorReg), (UWORD)c_blk->m_InterruptVector );

	/* write counter constant values to hardware */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterConstant1 + ((counter - 2) << 1)),
		c_blk->m_CounterConstant[counter - 1]);	/* MSword */

        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterConstant1 + ((counter - 1) << 1)),
		c_blk->m_CounterConstant[counter]);		/* LSword */

	/* Build up and install the new 32 bit control register value */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterControl1 + ((counter - 1) << 1)),
                build_control480(c_blk, counter));

    }
    else
    {
	/* write out stop control value */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterControl1 + ((counter - 1) << 1)), 0 );
	/* clear any pending interrupts from 16 bit counter */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + InterruptPending), (UWORD)~((1 << (counter - 1)) << 8));
	/* Write out the interrupt vector */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + InterruptVectorReg), (UWORD)c_blk->m_InterruptVector );
	/* write 16 bit counter constant */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterConstant1 + ((counter - 1) << 1)),
		c_blk->m_CounterConstant[counter] );
	/* Build up and install the new control register value */
        output_word( c_blk->nHandle, (word *)(c_blk->brd_ptr + CounterControl1 + ((counter - 1) << 1)),
                build_control480(c_blk, counter ));

   }
   return( Success );
}




/*
	Helper function  to generate control word for counter
*/

UWORD build_control480(c_blk, counter )
  struct cblk480 *c_blk;
  int counter;
{
        UWORD control;

    control = (UWORD)(c_blk->m_Mode[counter] & 7);

    if(c_blk->m_OutputPolarity[counter] == OutPolHi)	/* select output polarity */
       control |= 8;

    if(c_blk->m_InputPolarity[counter] == InPolHi)	/* select input polarity */
       control |= 0x10;

    if( c_blk->m_Trigger[counter] != InTrig)		/* extern/intern trigger source */
	{
	   control |= 0x40;

	   if(c_blk->m_Trigger[counter] == ExtPolHi)	/* select external trigger polarity */
	       control |= 0x20;
	}

    if(c_blk->m_InterruptEnable[counter] == IntEnable)  /* enable/disable interrupts */
       control |= 0x80;

    if(c_blk->m_CounterSize[counter] == CtrSize32)	/* select counter size */
       control |= 0x100;

    control |= ((c_blk->m_ClockSource[counter] & 3) << 9);

    if(c_blk->m_WDLoad[counter] == WDExtLd)		/* select watchdog loading */
       control |= 0x800;

    if(c_blk->m_Debounce[counter] == DebounceOn)	/* select debounce only for 1 & 2 */
       control |= 0x1000;

    return(control);
}



/*
	Helper function to test counter size
*/

BOOL islongcounter480(c_blk, counter )
  struct cblk480 *c_blk;
  int counter;
{
   if((counter == 2 || counter == 4 || counter == 6 ) && ( c_blk->m_CounterSize[counter] == CtrSize32)) /* is 32 bit */
	   return( TRUE );
   else
	   return( FALSE );
}
