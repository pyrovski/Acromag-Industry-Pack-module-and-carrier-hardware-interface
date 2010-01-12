
#include "../carrier/apc8620.h"
#include "ip236.h"

#define Write 0
#define Read  1
#define MagicCode (word)0xA300

/*
   Write to the coefficient access register.
   Address - contains the address of the byte to read or write
   Data - data only for a write cycle
   Cycle - cycle type read or write
*/

void WriteCoefficientAccess( c_blk, address, data, cycle )
struct conf_blk *c_blk;
word address;
byte data;
byte cycle;
{
	address <<= 8;				/* position address */
	address &= 0x7f00;			/* save only address for now */
	if( cycle == Write )			/* if write cycle... */
	{
		/* Before a write cycle can be started the magic code must first */
		/* be written to the coefficient write enable register */
		output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->reserved, MagicCode );
		address |= (word)data;	/* ... OR in the data to write */
	}
	else						/* else a read cycle... */
		address |= 0x8000;		/* ... set read cycle bit */

	output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, address ); /* do read or write cycle */

	/* if a write cycle, remove the magic code from the coefficient write enable register */
	if( cycle == Write )
		output_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->reserved, 0 );
}



/* Write the gain and offset error coefficients */

void WriteCoefficients( c_blk, channel, Offset, Gain, Range )
struct conf_blk *c_blk;
int channel;
word Offset, Gain;
int Range; 
{
	int RangeOffset;

	switch( Range )
	{
		case BIPOLAR_5:
			RangeOffset = 0x20;
		break;
		case UNIPOLAR_10:
			RangeOffset = 0x40;
		break;
		default:	/* default to BIPOLAR_10 */
			RangeOffset = 0;
		break;
	}

 	/* Write MSB of gain error word */
	WriteCoefficientAccess( c_blk, (word)((channel << 2) + RangeOffset + 2), (byte)(Gain >> 8), Write );
	usleep( 10000 );					/* sleep time */
	
	/* Write LSB of gain error word */
	WriteCoefficientAccess( c_blk, (word)((channel << 2) + RangeOffset + 3), (byte)(Gain & 0x00FF), Write );
	usleep( 10000 );					/* sleep time */

	/* Write MSB of offset error word */
	WriteCoefficientAccess( c_blk, (word)((channel << 2) + RangeOffset), (byte)(Offset >> 8), Write );
	usleep( 10000 );					/* sleep time */
	
	/* Write LSB of offset error word */
	WriteCoefficientAccess( c_blk, (word)((channel << 2) + RangeOffset + 1), (byte)(Offset & 0x00FF), Write );
	usleep( 10000 );					/* sleep time */
}



/* Read the offset error for the specified channel. */

int ReadOffsetError( c_blk, channel, Range )
struct conf_blk *c_blk;
int channel;
int Range; 
{
	int RangeOffset;
	word OffsetError;
	word temp;

	switch( Range )
	{
		case BIPOLAR_5:
			RangeOffset = 0x20;
		break;
		case UNIPOLAR_10:
			RangeOffset = 0x40;
		break;
		default:	/* default to BIPOLAR_10 */
			RangeOffset = 0;
		break;
	}

	/* Read MSB of offset error word */
	WriteCoefficientAccess( c_blk, (word)((channel << 2) + RangeOffset), 0, Read );
	do
	{
	OffsetError = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat );
	} while( ( OffsetError & 1 ) == 0 );

	usleep( 10000 );					/* sleep time */
	
	/* Read LSB of offset error word */
	WriteCoefficientAccess( c_blk, (word)((channel << 2) + RangeOffset + 1), 0, Read );
	do
	{
	temp = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat );
	} while( ( temp & 1 ) == 0 );

	usleep( 10000 );					/* sleep time */

	/* remove status flags and combine bytes to form the offset error */
	OffsetError &= 0xFF00;
	temp >>= 8;
	temp &= 0x00FF;
	OffsetError |= temp;
	return (int)OffsetError;
}



/* Read the gain error for the specified channel. */

int ReadGainError( c_blk, channel, Range )
struct conf_blk *c_blk;
int channel;
int Range; 
{
	int RangeOffset;
	word GainError, temp;

	switch( Range )
	{
		case BIPOLAR_5:
			RangeOffset = 0x20;
		break;
		case UNIPOLAR_10:
			RangeOffset = 0x40;
		break;
		default:	/* default to BIPOLAR_10 */
			RangeOffset = 0;
		break;
	}

	/* Read MSB of gain error word */
	WriteCoefficientAccess( c_blk, (word)((channel << 2) + RangeOffset + 2), 0, Read );
	do
	{
	GainError = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat );
	} while( ( GainError & 1 ) == 0 );

	usleep( 10000 );					/* sleep time */
	
	/* Read LSB of gain error word */
	WriteCoefficientAccess( c_blk, (word)((channel << 2) + RangeOffset + 3), 0, Read );
	do
	{
	temp = input_word( c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat );
	} while( ( temp & 1 ) == 0 );

	usleep( 10000 );					/* sleep time */

	/* remove status flags and combine bytes to form the gain error */
	GainError &= 0xFF00;
	temp >>= 8;
	temp &= 0x00FF;
	GainError |= temp;
	return (int)GainError;
}
