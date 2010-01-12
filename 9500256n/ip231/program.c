
#include "../carrier/apc8620.h"
#include "ip231.h"

/*
         ****************************************************************
         * NOTE: IT IS RECOMMENDED THAT THESE  VALUES  NOT  BE  CHANGED *
         * UNLESS  YOU  ARE   FAMILIAR  WITH  MAKING   THESE  TYPES  OF *
         * CALCULATIONS   AND  HAVE   A  VOLTMETER  CAPABLE  OF  16-BIT *
         * RESOLUTION AND ACCURACY. IF THESE VALUES ARE CHANGED AND NOT *
         * PROPERLY CALCULATED, THE  BOARD  WILL HAVE TO BE RETURNED TO *
         * THE FACTORY FOR CALIBRATION.  FOR THIS REASON, THIS FUNCTION *
         * IS NOT SUPPORTED FROM THE DEMONSTRATION PROGRAM.             * 
         ****************************************************************
*/



/* Write the gain and offset error coefficients to IP231 EEPROM */


int WriteCoefficients( c_blk )
struct cblk231 *c_blk;
{
    int i, status, count;
    unsigned char readback;

    output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->ee_control, 3);	/* Enable eewrites */
	usleep( 12 );				/* sleep time */

    /* Write the calibration coefficients to EEPROM */
    for(i = 0; i < 16; i++)
    {
       /* Write offset error MSbyte */
	readback = count = 0;
	output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->cal_map[i].offset_msb, (byte)( c_blk->off_buf[i] >> 8));
	while(readback == 0)
	{
		usleep( 2000 );					/* sleep time */
		if(++count > 100)
		{
		   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->ee_control, 0);	/* disable eewrites */
		   usleep( 12 );				/* sleep time */
		   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->control_reg, 0x80);	/* reset */
		   return -1;
		}
		readback = input_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->ee_status);
	}
    usleep( 12 );				/* sleep time */

       /* Write offset error LSbyte */
	readback = count = 0;
	output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->cal_map[i].offset_lsb, (byte)c_blk->off_buf[i]);
	while(readback == 0)
	{
		usleep( 2000 );					/* sleep time */
		if(++count > 100)
		{
		   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->ee_control, 0);	/* disable eewrites */
		   usleep( 12 );				/* sleep time */
		   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->control_reg, 0x80);	/* reset */
		   return -2;
		}
		readback = input_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->ee_status);
	}
    usleep( 12 );				/* sleep time */



       /* Write gain error MSbyte */
	readback = count = 0;
	output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->cal_map[i].gain_msb, (byte)( c_blk->gain_buf[i] >> 8));
	while(readback == 0)
	{
		usleep( 2000 );					/* sleep time */
		if(++count > 100)
		{
		   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->ee_control, 0);	/* disable eewrites */
		   usleep( 12 );				/* sleep time */
		   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->control_reg, 0x80);	/* reset */
		   return -3;
		}
		readback = input_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->ee_status);
	}
    usleep( 12 );				/* sleep time */



       /* Write gain error LSbyte */
	readback = count = 0;
	output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->cal_map[i].gain_lsb, (byte)c_blk->gain_buf[i]);
	while(readback == 0)
	{
		usleep( 2000 );					/* sleep time */
		if(++count > 100)
		{
		   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->ee_control, 0);	/* disable eewrites */
		   usleep( 12 );				/* sleep time */
		   output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->control_reg, 0x80);	/* reset */
		   return -4;
		}
		readback = input_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->ee_status);
	}
    usleep( 12 );				/* sleep time */
    }


    output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->ee_control, 0);	/* disable eewrites */
    usleep( 12 );				/* sleep time */
    output_byte( c_blk->nHandle, (byte*)&c_blk->brd_ptr->control_reg, 0x80);	/* reset */
	usleep( 2000 );					/* sleep time */
    return(0);
}
