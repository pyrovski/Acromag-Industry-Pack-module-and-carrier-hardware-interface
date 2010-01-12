
#include "../carrier/apc8620.h"
#include "ip340.h"

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
struct cblk340 *c_blk;
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
		output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->reserved, MagicCode);
		address |= (word)data;		/* ... OR in the data to write */
	}
	else						/* else a read cycle... */
		address |= 0x8000;		/* ... set read cycle bit */

	output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_acc, address); /* do read or write cycle */

	/* if a write cycle, remove the magic code from the coefficient write enable register */
	if( cycle == Write )
		output_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->reserved, 0);
}



/* Write ReferenceValue */

void WriteReferenceValue( c_blk, Value )
struct cblk340 *c_blk;
double Value; 
{
   char ValueString[32];
   int length, i;

      memset(ValueString, 0, sizeof(ValueString));

	sprintf(ValueString,"%f",Value);		/* float to string */
	length = strlen(ValueString);			/* get length */

	for(i = 0; i < (length + 1); i++)
	{
	   /* Write out the string */
	   WriteCoefficientAccess( c_blk, (word)i, (byte)ValueString[i], Write );
	   sleep( 1 );				/* sleep time */
	}
}



/* Read the ReferenceValue. */

double ReadReferenceValue( c_blk )
struct cblk340 *c_blk;
{
   char ValueString[32];
   int i;
   word data;
   double result;

      memset(ValueString, 0, sizeof(ValueString));

	for( i = 0; i < sizeof(ValueString); i++ )
	{
	  /* Read numeric string */
	  WriteCoefficientAccess( c_blk, (word)i, 0, Read );	/* read data at address */
	  do
	  {
	    data = input_word(c_blk->nHandle, (word*)&c_blk->brd_ptr->cal_coef_stat);
	  } while( ( data & 1 ) == 0 );				/* wait for read complete */

          data >>= 8;
          data &= 0x00FF;

	  ValueString[i] = (char)data;

	  if(ValueString[i] == 0)				/* quit if end of string */
		break;
	}
        sscanf(ValueString,"%lf",&result);

	return (result);
}

