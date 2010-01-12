
#include "../carrier/apc8620.h"
#include "ip482.h"

/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetInterruptVector

    VERSION:		A

    CREATION DATE:      10/10/03

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


int GetInterruptVector(c_blk, vector )
   struct ip482 *c_blk;
   byte *vector;
{
	if( vector == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*vector = (byte)c_blk->m_InterruptVector;
	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetInterruptVector

    VERSION:		A

    CREATION DATE:      10/10/03

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

int SetInterruptVector(c_blk, vector)
   struct ip482 *c_blk;
   byte vector;
{
	c_blk->m_InterruptVector = vector;
	return( Success );
}


/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        ReadCounter

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FM

    ABSTRACT:           This file retrieves the read back register value.

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

  Read counter.

  Read the value of a 16 bit counter.
  The current value of the counter is placed at
  the address specified by val.
  Valid values for "counter" are 1 thru 10 for 16 bit.
  Returns parameter out of range error if the counter value is invalid.
  Returns invalid pointer error if val is NULL, else returns success.
*/


int ReadCounter(c_blk,  counter,  val )
   struct ip482 *c_blk;
   int counter;
   word *val;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( val == NULL )			/* valid range ? */
		return(	InvalidPointer );

	/* read counter */
	*val = (word)input_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterReadBack1 + (counter - 1)));

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        ReadCounterControl

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FM

    ABSTRACT:           This file retrieves the counter control register value.

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

  Read counter control reg.

  Read the value of a counter control register.
  The current value is placed at the address specified by val.
  Valid values for "counter" are 1 thru 10 for 16 bit.
  Returns parameter out of range error if the counter value is invalid.
  Returns invalid pointer error if val is NULL, else returns success.
*/


int ReadCounterControl(c_blk,  counter,  val )
   struct ip482 *c_blk;
   int counter;
   word *val;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( val == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*val = (word)input_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterControl1 + (counter - 1)));

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        WriteCounterControl

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FM

    ABSTRACT:           This file Write the counter control register value.

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

  Write counter control register.

  Write counter control register.
  The current value of 'val' is placed into the counter control reg.
  Valid values for "counter" are 1 thru 10 for 16 bit.
  Returns parameter out of range error if the counter value is invalid,
  else returns success.
*/


int WriteCounterControl(c_blk,  counter,  val )
   struct ip482 *c_blk;
   int counter;
   word val;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterControl1 + (counter - 1)), val);

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetCounterConstantA

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This file retrieves the counter constant register
                        value.

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

  Get counter constant.

  Read the counter constant.
  The current value of the counter constant register is placed at
  the address specified by val.
  Valid values for "counter" are 1 thru 10 for 16 bit.
  Returns parameter out of range error if the counter value is invalid.
  Returns invalid pointer error if val is NULL, else returns success.
*/


int GetCounterConstantA(c_blk,  counter,  val )
  struct ip482 *c_blk;
  int counter;
  word *val;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( val == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*val = (word)c_blk->m_CounterConstantA[(counter - 1)];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetCounterConstantA

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This file stores the counter constant value.

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

	Set counter constant.

  Set the counter constant.
  Valid values for "counter" are 1 thru 10 for 16 bit.
  Returns parameter out of range error if the counter value is invalid
  else returns success.
*/


int SetCounterConstantA(c_blk,  counter, val )
  struct ip482 *c_blk;
  int counter;
  word val;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

    c_blk->m_CounterConstantA[(counter - 1)] = val;  /* save */

	return( Success );
}





/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetCounterConstantB

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This file retrieves the counter constant register
                        value.

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

  Get counter constant.

  Read the counter constant.
  The current value of the counter constant register is placed at
  the address specified by val.
  Valid values for "counter" are 1 thru 10 for 16 bit.
  Returns parameter out of range error if the counter value is invalid.
  Returns invalid pointer error if val is NULL, else returns success.
*/


int GetCounterConstantB(c_blk,  counter,  val )
  struct ip482 *c_blk;
  int counter;
  word *val;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( val == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*val = (word)c_blk->m_CounterConstantB[(counter - 1)];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetCounterConstantB

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This file stores the counter constant value.

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

	Set counter constant.

  Set the counter constant.
  Valid values for "counter" are 1 thru 10 for 16 bit.
  Returns parameter out of range error if the counter value is invalid
  else returns success.
*/


int SetCounterConstantB(c_blk,  counter, val )
  struct ip482 *c_blk;
  int counter;
  word val;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

    c_blk->m_CounterConstantB[(counter - 1)] = val;  /* save */

	return( Success );
}




/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        WriteCounterConstant

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine write the stored counter constant value
                        to the board registers.

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

	Write counter constant to board register.

  Write a value to the counter constant register.
  Valid values for "counter" are 1 thru 10 for 16 bit.
  Returns parameter out of range error if the counter value is invalid
  else returns success.
*/


int WriteCounterConstant(c_blk,  counter )
  struct ip482 *c_blk;
  int counter;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

    output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterConstantA1 + (counter - 1)),
				  c_blk->m_CounterConstantA[(counter - 1)] );

    output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterConstantB1 + (counter - 1)),
				  c_blk->m_CounterConstantB[(counter - 1)] );

	return( Success );
}





/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetMode

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This file retrieves the current MODE value.

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

 Get mode value for counter

 The current value is placed at the address specified by mode.
 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the counter value is invalid.
 Returns Invalid pointer if 'mode' is NULL, else returns success.
*/

int GetMode(c_blk,  counter,  mode )
  struct ip482 *c_blk;
  int counter;
  byte *mode;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( mode == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*mode = c_blk->m_Mode[(counter - 1)];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetMode

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine stores the MODE value.

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

 Set mode

 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the 'counter' or 'mode' value
 is invalid, else returns success.
*/

int SetMode(c_blk,  counter,  mode )
  struct ip482 *c_blk;
  int counter;
  byte mode;

{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if(  mode > OneShot )	  /* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_Mode[(counter - 1)] = mode;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetInterruptEnable

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine gets the current INTERRUPT ENABLE
                        value.

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

 Get interrupt enable

 The current value is placed at the address specified by enable.
 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the counter value is invalid.
 Returns Invalid pointer if 'enable' is NULL, else returns success.
*/

int GetInterruptEnable(c_blk,  counter,  enable )
  struct ip482 *c_blk;
  int counter;
  BOOL *enable;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( enable == NULL )				/* valid range ? */
		return(	InvalidPointer );

	*enable = c_blk->m_InterruptEnable[(counter - 1)];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetInterruptEnable

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine sets the INTERRUPT ENABLE value.

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

 Set interrupt enable
 Bit 0 set in this register enables the corresponding counter interrupt.
 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the 'counter' or 'enable'
 values are invalid.
*/

int SetInterruptEnable(c_blk, counter, enable )
  struct ip482 *c_blk;
  int counter;
  BOOL enable;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if(enable < IntDisable || enable > IntEnable )	/* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_InterruptEnable[(counter - 1)] = enable;

	return( Success );
}




/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetIpClock

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine gets the current Ip clock value.

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

 Get Ip clock

 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by 'value'.
 Returns Invalid pointer if 'source' is NULL, else returns success.
*/

int GetIpClock(c_blk, value )
  struct ip482 *c_blk;
  BOOL *value;
{
	if( value == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*value = c_blk->m_IpClock;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetIpClock

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine sets the Ip clock value.

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

 Set Ip clock

 Returns parameter out of range error if the value is invalid
 else returns success.
*/

int SetIpClock(c_blk, value )
  struct ip482 *c_blk;
  BOOL value;
{
	c_blk->m_IpClock = value;

	return( Success );
}




/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetClockSource

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine gets the current CLOCK SOURCE value.

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

 Get clock source

 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by 'source'.
 Returns Invalid pointer if 'source' is NULL, else returns success.
*/

int GetClockSource(c_blk, counter,  source )
  struct ip482 *c_blk;
  int counter;
  byte *source;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( source == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*source = c_blk->m_ClockSource[(counter - 1)];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetClockSource

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine sets the CLOCK SOURCE value.

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

 Set clock source

 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 Returns parameter out of range error if the source value is invalid
 else returns success.
*/

int SetClockSource(c_blk, counter,  source )
  struct ip482 *c_blk;
  int counter;
  byte source;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	c_blk->m_ClockSource[(counter - 1)] = source;

	return( Success );
}




/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetOutputPolarity

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine gets the current OUTPUT POLARITY Value.

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

 Get output polarity

 Bit 0 set in a register enables the active LOW output polarity.
 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by polarity.
 Returns Invalid pointer if 'polarity' is NULL, else returns success.
*/

int GetOutputPolarity(c_blk, counter,  polarity )
  struct ip482 *c_blk;
  int counter;
  BOOL *polarity;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*polarity = c_blk->m_OutputPolarity[(counter - 1)];

	return( Success );
}


/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetOutputPolarity

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine sets the OUTPUT POLARITY Value.

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

 Set output polarity

 Bit 0 set in a register enables the active LOW output polarity.
 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the 'counter' or polarity values
 are invalid else returns success.
*/

int SetOutputPolarity(c_blk, counter,  polarity )
  struct ip482 *c_blk;
  int counter;
  BOOL polarity;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity < OutPolLow || polarity > OutPolHi )	/* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_OutputPolarity[(counter - 1)] = polarity;

	return( Success );
  }




/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetInputAPolarity

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine gets the current INPUT POLARITY Value.

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

 Get input A polarity

 Valid values for "counter" are 1,3,5,7,9 for 32 bit.
 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by polarity.
 Returns Invalid pointer if 'polarity' is NULL, else returns success.
*/

int GetInputAPolarity(c_blk, counter,  polarity )
  struct ip482 *c_blk;
  int counter;
  byte *polarity;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*polarity = c_blk->m_InputAPolarity[(counter - 1)];

	return( Success );
}


/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetInputAPolarity

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine sets the INPUT POLARITY Value.

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

 Set input A polarity

 Set the values of the input polarity registers.
 Returns parameter out of range error if the polarity value is invalid
 else returns success.
 Returns Invalid pointer if 'polarity' is NULL, else returns success.
*/

int SetInputAPolarity(c_blk, counter,  polarity )
  struct ip482 *c_blk;
  int counter;
  byte polarity;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity > InAX4 )	/* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_InputAPolarity[(counter - 1)] = polarity;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetInputBPolarity

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine gets the current INPUT POLARITY Value.

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

 Get input B polarity

 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by polarity.
 Returns Invalid pointer if 'polarity' is NULL, else returns success.
*/

int GetInputBPolarity(c_blk, counter,  polarity )
  struct ip482 *c_blk;
  int counter;
  byte *polarity;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*polarity = c_blk->m_InputBPolarity[(counter - 1)];

	return( Success );
}


/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetInputBPolarity

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine sets the INPUT POLARITY Value.

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

 Set input B polarity

 Set the values of the input polarity registers.
 Returns parameter out of range error if the polarity value is invalid
 else returns success.
 Returns Invalid pointer if 'polarity' is NULL, else returns success.
*/

int SetInputBPolarity(c_blk, counter,  polarity )
  struct ip482 *c_blk;
  int counter;
  byte polarity;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity > InABCPolHi )	/* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_InputBPolarity[(counter - 1)] = polarity;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        GetInputCPolarity

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine gets the current INPUT POLARITY Value.

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

 Get input C polarity

 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the 'counter' value is invalid.
 The current value is placed at the address specified by polarity.
 Returns Invalid pointer if 'polarity' is NULL, else returns success.
*/

int GetInputCPolarity(c_blk, counter,  polarity )
  struct ip482 *c_blk;
  int counter;
  byte *polarity;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity == NULL )			/* valid range ? */
		return(	InvalidPointer );

	*polarity = c_blk->m_InputCPolarity[(counter - 1)];

	return( Success );
}


/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        SetInputCPolarity

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine sets the INPUT POLARITY Value.

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

 Set input C polarity

 Set the values of the input polarity registers.
 Returns parameter out of range error if the polarity value is invalid
 else returns success.
 Returns Invalid pointer if 'polarity' is NULL, else returns success.
*/

int SetInputCPolarity(c_blk, counter,  polarity )
  struct ip482 *c_blk;
  int counter;
  byte polarity;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( polarity > InCQuadMax )	/* valid range ? */
	    return( ParameterOutOfRange );

	c_blk->m_InputCPolarity[(counter - 1)] = polarity;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        GetDebounce

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

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

 Get Debounce for counters

 The current value is placed at the address specified by mode.
  Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the counter value is invalid.
 Returns Invalid pointer if 'debounce' is NULL, else returns success.
*/

int GetDebounce(c_blk,  counter,  debounce )
  struct ip482 *c_blk;
  int counter;
  BOOL *debounce;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if( debounce == NULL )				/* valid range ? */
		return(	InvalidPointer );

	*debounce = c_blk->m_Debounce[(counter - 1)];

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg480.c

    MODULE NAME:        SetDebounce

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

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

 Set Debounce for counters

 Valid values for "counter" are 1 thru 10 for 16 bit.
 Valid values for "debounce" are 0 and 1.
 Returns parameter out of range error if the 'counter' or 'debounce' value
 is invalid, else returns success.
*/

int SetDebounce(c_blk,  counter,  debounce )
  struct ip482 *c_blk;
  int counter;
  BOOL debounce;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	if(debounce < DebounceOff || debounce > DebounceOn )	/* valid range ? */
		return( ParameterOutOfRange);

	c_blk->m_Debounce[(counter - 1)] = debounce;

	return( Success );
}



/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        StopCounter

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine stops the counter/timer function.

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

 Stop counter

 disables a counter timer.
 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the counter value is invalid
 else returns success.
*/

int StopCounter(c_blk, counter )
  struct ip482 *c_blk;
  int counter;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

    output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterStop), 1 << (counter - 1)); /* Stop counter */

	return( Success );
}


/*
{+D}
    FILENAME:           cnfg482.c

    VERSION:            A

    CREATION DATE:      04/01/09

    CODED BY:           FJM

    ABSTRACT:           This routine will issue a software trigger to one
                        channel.

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
 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the counter value is invalid
 else returns success.
*/

int BlockingStartCounter(c_blk, counter )
  struct ip482 *c_blk;
  int counter;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

    blocking_start_convert(c_blk->nHandle, (word*)&c_blk->brd_ptr->CounterTrigger, 1 << (counter - 1)); /* Trigger counter */
	return( Success );
}




/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        StopSimultaneousCounters

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine will issue a software trigger to stop
                        multiple channels.

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

 Stop Simultaneous counter

 Stops one counter, any combination, or all counter timers.
 Bit mask of counters to stop bit9 ... bit0
 Returns success.
*/

int StopSimultaneousCounters(c_blk, mask )
  struct ip482 *c_blk;
   word mask;
{
    output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterStop ), (word)(mask & 0xFFF));
	return( Success );
}




/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        DisableInterrupt

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine disables the interrupt the current
                        channel.

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

 Disable counter interrupts

 disables only timer interrupts.
 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the counter value is invalid
 else returns success.
*/

int DisableInterrupt(c_blk, counter )
  struct ip482 *c_blk;
  int counter;
{
    word control;

	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

	/* read the control value from hardware */
    control = input_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterControl1 + (counter - 1)));
	control &= 0xff7f;		/* force interrupt bit to 0 */
	/* write out stop control value */
    output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterControl1 + (counter - 1)), control);

	return( Success );
}






/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        StartCounter

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine will issue a software trigger to one
                        channel.

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

 Start a counter

 Starts one counter timer.
 Valid values for "counter" are 1 thru 10 for 16 bit.
 Returns parameter out of range error if the counter value is invalid
 else returns success.
*/

int StartCounter(c_blk, counter )
  struct ip482 *c_blk;
  int counter;
{
	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );

    output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterTrigger), 1 << (counter - 1)); /* Trigger counter */

	return( Success );
}






/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        StartSimultaneousCounters

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine will issue a software trigger to
                        multiple channels.

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

 Start Simultaneous counter

 Starts one counter, any combination, or all counter timers.
 Bit mask of counters to start bit9 ... bit0
 Returns success.
*/

int StartSimultaneousCounters(c_blk, mask )
  struct ip482 *c_blk;
   word mask;
{
    output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->CounterTrigger), (word)(mask & 0xFFF));
	return( Success );
}





/*
{+D}
    FILENAME:           cnfg482.c

    MODULE NAME:        ConfigureCounterTimer

    VERSION:		A

    CREATION DATE:      10/10/03

    CODED BY:           FJM

    ABSTRACT:           This routine will poll all of the control options,
                        create the control word and write it out to the board.

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

 Configure counter timer
*/

int ConfigureCounterTimer(c_blk, counter )
  struct ip482 *c_blk;
  int counter;
{
  word temp;

	if( counter < 1 || counter > 10 )	/* valid range ? */
		return( ParameterOutOfRange );
    
	/* stop counter and disable interrupts */
	WriteCounterControl(c_blk, counter, 0 ); /* write out a 0 control value to disable the counter */

	/* Write out the interrupt vector */
        output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->InterruptVectorReg), (word)c_blk->m_InterruptVector );

	/* Write out the Ip clock */
        temp = input_word(c_blk->nHandle, (word *)&c_blk->brd_ptr->control );
	temp |= ( c_blk->m_IpClock << 2 );
        output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->control), temp );

/* global interrupt enable */
temp = input_word(c_blk->nHandle, (word *)&c_blk->brd_ptr->control );
temp |= 1 ;
output_word(c_blk->nHandle, (word *)(&c_blk->brd_ptr->control), temp );

	/* Build up and install the new control register value */
	WriteCounterControl(c_blk, counter, build_control(c_blk, counter) );

	return( Success );
}




/*
	Helper function to generate control word for counter
*/

word build_control(c_blk, counter )
  struct ip482 *c_blk;
  int counter;
{
    word control;

    control = (word)(c_blk->m_Mode[(counter - 1)] & 7);

    if( control == QuadPosition )
    {
	control |= ((c_blk->m_InputBPolarity[(counter - 1)] & 1) << 6);/* select input B polarity */
	control |= ((c_blk->m_InputCPolarity[(counter - 1)] & 7) << 7);/* select input C polarity */
    }
    else
    {
	control |= ((c_blk->m_InputBPolarity[(counter - 1)] & 3) << 6);/* select input B polarity */
	control |= ((c_blk->m_InputCPolarity[(counter - 1)] & 3) << 8);/* select input C polarity */
    }

    control |= ((c_blk->m_InputAPolarity[(counter - 1)] & 3) << 4);/* select input a polarity */

    if(c_blk->m_OutputPolarity[(counter - 1)] == OutPolHi)	/* select output polarity */
       control |= 8;

    control |= ((c_blk->m_ClockSource[(counter - 1)] & 7) << 10);

    if(c_blk->m_Debounce[(counter - 1)] == DebounceOn)	/* select debounce only for 1 & 2 */
       control |= 0x2000;

    if(c_blk->m_InterruptEnable[(counter - 1)] == IntEnable)  /* enable/disable interrupts */
       control |= 0x8000;

    return(control);
}
