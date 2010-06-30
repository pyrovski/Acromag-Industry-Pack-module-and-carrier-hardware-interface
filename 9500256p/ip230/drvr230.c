#include "../carrier/apc8620.h"
#include "ip230.h"
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */


/*
{+D}
    SYSTEM:         IP230/235 Software

    FILENAME:       drvr230.c

    MODULE NAME:    main - main routine of example software.

    VERSION:        REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:       R.H.

    ABSTRACT:       This module is the main routine for the example program
		    which demonstrates how the IP230/235 Library is used.

    CALLING
        SEQUENCE:   

    MODULE TYPE:    void

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

        This module is the main routine for the example program
        which demonstrates how the IP230/235 Library is used.
*/



  int num_chan;    /* number of channels on board */
  int model;                /* model type storage */
  float max, min, half_span, v, dv, rad, midpoint, mem_span;


int main(){


/*
    Declare External Data Areas
*/
    extern int num_chan;       /* channel count storage*/
    extern int model;          /* model type storage */

/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[32];		/* command line input buffer */
    unsigned finished;		/* flag to exit program */
    int item;			/* menu item selection variable */
    int hstatus;		/* returned status of routines */
    float volts, temp;
    float ideal_volt_span;
    unsigned finished2;		/* flag to exit a loop */
    long addr;			/* integer to hold board address */
    int flag;			/* general flag for exiting loops */
    int i;			/* loop index */
    int channel;
    struct cblk230 c_block230;    /* configuration block */
    int address;                /* address storage for read and write memory */
    int data;                   /* data storage for read and write memory */
    int time;                   /* storage for coversion timer register */
    int tim;                    /* storage for the pre scaler */
    int hflag;                  /* interrupt handler installed flag */
    struct handler_data hdata;  /* interrupt data structure (see exception.h) */

    word ideal_data[A_SIZE8];	/* allocate data storage area */
    word cor_data[A_SIZE8];   	/* allocate data storage area */


/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/

    flag = 0;		/* indicate board address not yet assigned */
    finished = 0;	/* indicate not finished with program */
    hflag = 0;          /* indicate interrupt handler not installed yet */
    ideal_volt_span = VOLT_SPAN_B10; /* default */

    for(i = 0; i < A_SIZE8;i++)
    {
		ideal_data[i] = 0;		/* clear ideal buffer */
		cor_data[i] = 0;		/* clear corrected buffer */
        c_block230.range[i] = BIPOLAR_10;
    }

/*
    Initialize the Configuration Parameter Block to default values.
*/

    c_block230.mode = 0;			/* mode */
    c_block230.bit_constant = CON16;	/* constant for data correction */
    c_block230.interrupt_flag = 0;		/* default to no interrupts */
    c_block230.interrupt_vector = VECTOR;	/* default interrupt vector */

    c_block230.bCarrier = FALSE;	/* indicate no carrier initialized and set up yet */
    c_block230.bInitialized = FALSE;/* indicate not ready to talk to IP */
    c_block230.slotLetter = SLOT_A;
    c_block230.nHandle = 0;	/* make handle to a closed carrier board */

/*
     Get model type
*/
type:
    printf("1 - IP230-4\n");
    printf("2 - IP230-8\n");
    printf("3 - IP235-4\n");
    printf("4 - IP235-8\n\n");
    printf("Enter Model: ");
    scanf("%d", &model);
    printf("\n");
    if(model > 4 || model < 1)
    {
     printf("Illegal Model Entered, Try Again!\n");
     goto type;
    }

    if(model == 1 || model == 3)
     num_chan = A_SIZE4;
    else
     num_chan = A_SIZE8;

    hdata.h_pid = getpid();		/* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block230;	/* put in address of c_block230 structure also */

/*
	Initialize the Carrier library
*/
    if(InitCarrierLib() != S_OK)
    {
	printf("\nUnable to initialize the carrier library. Exiting program.\n");
	exit(0);
    }

/*
	Open an instance of a carrier device 
*/
    if(CarrierOpen(0, &c_block230.nHandle) != S_OK)
    {
	printf("\nUnable to Open instance of carrier.\n");
	finished = 1;	 /* indicate finished with program */
    }
    else
	flag = 1;


/*
    Enter main loop
*/      

    while(!finished)
    {
      if(model == 3 || model == 4) 
       printf("\nIP235 Library Demonstration  Rev. A \n");
      else
       printf("\nIP230 Library Demonstration  Rev. A \n");
      printf(" 1. Exit this Program\n");
      printf(" 2. Set Board Base Address\n");
      printf(" 3. Set IP Slot Letter\n");

      if(model == 3 || model == 4)
       printf(" 4. Read ID Prom and Event Status\n");
      else
       printf(" 4. Read ID Prom\n");
      printf(" 5. Control Register\n");
      printf(" 6. Write Ideal Data To Output\n");
      printf(" 7. Write Corrected Data To Output\n");
      printf(" 8. Start Conversion\n");
      printf(" 9. Display Ideal/Corrected Data, Offset/Gain Coefficients\n");
      printf("10. Clear All Data Buffers\n");
      if(model == 3 || model == 4)
       printf("11. Display Control/Timer Registers\n");
      else
       printf("11. Display Control Register\n");

      if(model == 3 || model == 4)
      {
       printf("12. Write Waveform Memory\n");
       printf("13. Read Waveform Memory\n");
       printf("14. Write to Conversion Timer\n");
       printf("15. Write to Timer Prescaler\n");
       printf("16. Enable Interrupts\n");
      }
      printf("Select: ");
      scanf("%d",&item);

/*
    perform the menu item selected.
*/  

      switch(item)
      {
        case 1: /* exit program command */
            printf("Exit program(y/n)?: ");
            scanf("%s",cmd_buff);
            if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
                finished++;
            break;
        

	case 2: /* set board address command */
	    do 
	    {
		if(flag == 0)
		{
		  printf("\n\nenter base address of carrier board in hex: ");
		  scanf("%lx",&addr);
		  /* Set Carrier Address for Open Carrier Device */
		  SetCarrierAddress(c_block230.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block230.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
          SetCarrierAddress(c_block230.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block230.nHandle) == S_OK)/* Initialize Carrier */
		  {
		        c_block230.bCarrier = TRUE;
			SetInterruptLevel(c_block230.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;

				
            }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
        break;


	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block230.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block230.bCarrier = FALSE;
			}
			else
			{
				c_block230.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block230.nHandle, c_block230.slotLetter, &addr) != S_OK)
				{
				  printf("\nUnable to Get Ipack Address.\n");
				  c_block230.bInitialized = FALSE;
				}
				else	              
				{	
				  c_block230.brd_ptr = (struct map230 *)addr;
				  c_block230.bInitialized = TRUE;
				}
			}
		}
            printf("\nPlease Wait, Reading Calibration Coefficients....\n");
           rcc230(&c_block230, num_chan); /* read the calibration coef. into an array */
	break;


        case 4: /* Read and ID Prom  and event status*/
            if(!c_block230.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
             readstat(&c_block230);

            break;    


        case 5:				/* Control Register Setup and Range Selection */
            if(!c_block230.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	        cont_reg(&c_block230, hflag);	/* control register config routine */
            break;



        case 6: /* Write Ideal Data To Output */
            if(!c_block230.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
gloop1:
	      printf("\nEnter channel number 0 - %d     : ",num_chan - 1);
    	      scanf("%x", &channel);
              printf("\n");
              if(channel >= num_chan || channel < 0)
              {
               printf("\nChannel out of range.\n");
               goto gloop1;
              }
vloop1:
              printf("Enter desired voltage: eg:1.25   ");
              scanf("%f", &volts);
              printf("\n");
              if(volts < -10.0 || volts > 10.0)
              {
               printf("\nVoltage out of board range.\n");
               goto vloop1;
              }

             ideal_volt_span = VOLT_SPAN_B10;		/* default */

             if(c_block230.range[channel] == BIPOLAR_5)
               ideal_volt_span = VOLT_SPAN;

             if(c_block230.range[channel] == UNIPOLAR_10)
               ideal_volt_span = VOLT_SPAN;

             temp =((float)CON16 * volts) / ideal_volt_span;

             if( temp < 0.0)		/* round */
                 temp -= 0.5;
             else
                 temp += 0.5;

             ideal_data[channel] = (word)temp;

             if(c_block230.range[channel] == UNIPOLAR_10)
              ideal_data[channel] ^= 0x8000;

  	     wro230(channel,(short)(ideal_data[channel]), &c_block230);
	    }
        break;


        case 7: /* Write Corrected Data To Output */
            if(!c_block230.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
gloop2:
	      printf("\nEnter channel number 0 - %d     : ",num_chan - 1);
              scanf("%x", &channel);
              printf("\n");
              if(channel >= num_chan || channel < 0)
              {
                 printf("\nChannel out of range.\n");
                 goto gloop2;
              }
vloop2:
              printf("Enter desired voltage: ie: 1.25    ");
              scanf("%f", &volts);
              printf("\n");
              if(volts < -10.0 || volts > 10.0)
              {
               printf("\nVoltage out of board range.\n");
               goto vloop2;
              }

             if(c_block230.range[channel] == BIPOLAR_10)
               ideal_volt_span = VOLT_SPAN_B10;

             if(c_block230.range[channel] == BIPOLAR_5)
               ideal_volt_span = VOLT_SPAN;

             if(c_block230.range[channel] == UNIPOLAR_10)
               ideal_volt_span = VOLT_SPAN;

             temp =((float)CON16 * volts) / ideal_volt_span;

             if( temp < 0.0)		/* round */
                 temp -= 0.5;
             else
                 temp += 0.5;

             ideal_data[channel] = (word)temp;

             if(c_block230.range[channel] == UNIPOLAR_10)
               ideal_data[channel] ^= 0x8000;
 
             c_block230.ideal_buf[channel] = (short)ideal_data[channel]; 

	     cd230(&c_block230, channel);	/* correct data */ 

             cor_data[channel] = (word)c_block230.cor_buf[channel];

	     wro230(channel, c_block230.cor_buf[channel], &c_block230);
	    }
            break;


        case 8:  /* Start Conversion */
            if(!c_block230.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	        start_con(&c_block230);
           break;


        case 9:	/* Display Ideal/Corrected Data, Offset/Gain Coefficients */
	    printf("\n  ");
	    for(i = 0; i != num_chan; i++)
	      printf("    CH %02d",i);

	    printf("\nID");
	    for(i = 0; i != num_chan; i++)
	      printf("     %04x",ideal_data[i]);

	    printf("\nCD");
	    for(i = 0; i != num_chan; i++)
	      printf("     %04x",cor_data[i]);

	    printf("\nGD");
	    for(i = 0; i != num_chan; i++)
          {
           if(c_block230.range[i] == BIPOLAR_5)
	      printf("     %04x",(c_block230.coef_bufb5[i][1] & 0xFFFF));
           if(c_block230.range[i] == BIPOLAR_10)
	      printf("     %04x",(c_block230.coef_bufb10[i][1] & 0xFFFF));
           if(c_block230.range[i] == UNIPOLAR_10)
	      printf("     %04x",(c_block230.coef_bufu10[i][1] & 0xFFFF));
          }

	    printf("\nOD");
	    for(i = 0; i != num_chan; i++)
          {
           if(c_block230.range[i] == BIPOLAR_5)
	      printf("     %04x",(c_block230.coef_bufb5[i][0] & 0xFFFF));
           if(c_block230.range[i] == BIPOLAR_10)
	      printf("     %04x",(c_block230.coef_bufb10[i][0] & 0xFFFF));
           if(c_block230.range[i] == UNIPOLAR_10)
	      printf("     %04x",(c_block230.coef_bufu10[i][0] & 0xFFFF));
          }

            break;


        case 10:        /* clear all data buffers */
	    for(i = 0; i < num_chan; i++)
	    {
		ideal_data[i] = 0;	/* clear ideal buffer */
		cor_data[i] = 0;	/* clear corrected buffer */
	    }
            break;


        case 11:      /* display the control and timer registers */
            if(!c_block230.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
            {
             printf("\nControl Register:               %04X\n",
			(int)input_word( c_block230.nHandle, (word*)&c_block230.brd_ptr->cont_reg ));
             if(model == 3 || model == 4)
             {
               printf("Timer Prescaler Register:       %02X\n",
			(int)input_word( c_block230.nHandle, (word*)&c_block230.brd_ptr->tim_pres ));
               printf("Conversion Timer Register:      %04X\n",
			(int)input_word( c_block230.nHandle, (word*)&c_block230.brd_ptr->conv_tim ));
             }
            }
           break;


        case 12:                                /* write to waveform memory */
            if(!c_block230.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
 		finished2 = 0;
		while(!finished2)
		{
		   printf("\n\nWaveform Memory\n\n");

		   printf("\n 1. Return to previous menu");
		   printf("\n 2. Sine Wave Data");
		   printf("\n 3. Triangle Wave Data");
		   printf("\n 4. Square Wave Data");
		   printf("\n 5. Zero Fill Data");
		   printf("\n 6. Enter Individual Data");

		   printf("\nSelect: ");
		   scanf("%d",&item);

		   switch(item)
		   {
 		      case 1: /* return to previous menu */
		            finished2++;
		      break;

		      case 2: /* sine */
            		if(!c_block230.bInitialized)
              		  printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            		else
	    		{
		gloop3:
	 		     printf("\nEnter channel number 0 - %d     : ",num_chan - 1);
              		     scanf("%x", &channel);
              		     printf("\n");
              		     if(channel >= num_chan || channel < 0)
              		     {
                	     printf("\nChannel out of range.\n");
                 	     goto gloop3;
              		     }

	      		     switch(c_block230.range[channel])
	      		     {
				case BIPOLAR_5:
					max = (float)4.999847;
					min = -5.0;
	                		ideal_volt_span = VOLT_SPAN;
					break;

				case UNIPOLAR_10:
					max = (float)9.999847;
					min = 0.0;
	                		ideal_volt_span = VOLT_SPAN;
					break;

				default: 	/* BIPOLAR_10 */
	                		ideal_volt_span = VOLT_SPAN_B10;
					max = (float)9.999695;
					min = -10.0;
					break;

	      		     }

	      		half_span = ( max - min ) / 2.0;
              		midpoint = ( max + min ) / 2.0;
	      		mem_span = 6.283185308 / ( MAX_WAVEFORM_ADDRESS - MIN_WAVEFORM_ADDRESS );

	      /* Index through waveform memory, calculating the sin value and
	      the corresponding voltage for the range. */

	      	for( i = MIN_WAVEFORM_ADDRESS; i <= MAX_WAVEFORM_ADDRESS; i++ )
	      	{
			rad = ( i - MIN_WAVEFORM_ADDRESS ) * mem_span;
			rad = sin( rad );
			v = rad * half_span + midpoint;

                        temp =((float)CON16 * v) / ideal_volt_span;

                        if( temp < 0.0)		/* round */
                            temp -= 0.5;
                        else
                            temp += 0.5;

                       	c_block230.ideal_buf[channel] = (word)temp;

			if(c_block230.range[channel] == UNIPOLAR_10)
			        c_block230.ideal_buf[channel] ^= 0x8000;
 
			cd230(&c_block230, channel);

	                wav_mem( 1, i, c_block230.cor_buf[channel], &c_block230);
	      }
	    }
		      break;

		      case 3: /* Triangle Wave */
            		if(!c_block230.bInitialized)
                		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            		else
	    		{
		gloop5:
	      			printf("\nEnter channel number 0 - %d     : ",num_chan - 1);
             			scanf("%x", &channel);
              			printf("\n");
              			if(channel >= num_chan || channel < 0)
              			{
                 			printf("\nChannel out of range.\n");
                 			goto gloop5;
              			}

	      			switch(c_block230.range[channel])
	      			{
					case BIPOLAR_5:
						max = (float)4.8;
						min = -4.8;
	                			ideal_volt_span = VOLT_SPAN;
					break;

					case UNIPOLAR_10:
						max = (float)9.8;
						min = 0.1;
	                			ideal_volt_span = VOLT_SPAN;
					break;

					default: 	/* BIPOLAR_10 */
	                			ideal_volt_span = VOLT_SPAN_B10;
						max = (float)9.8;
						min = -9.8;
					break;
	
	      			}
	      			dv = ( max - min ) / 65535.0;		/* calculate 1 DAC LSB's voltage */
	      			midpoint = ( max + min ) / 2.0;
	      			dv = 2.0 * ( max - min ) / (double)( MAX_WAVEFORM_ADDRESS - MIN_WAVEFORM_ADDRESS );

	      			v = midpoint;				/* start at midpoint and go up*/
	      			for( i = MIN_WAVEFORM_ADDRESS; i <= MAX_WAVEFORM_ADDRESS; i++ )
	      			{
					if( v >= max || v <= min )
					dv *= -1.0;		/* reached a rail, ramp other way*/
					v += dv;

                    temp =((float)CON16 * v) / ideal_volt_span;

                    if( temp < 0.0)		/* round */
                        temp -= 0.5;
                    else
                        temp += 0.5;

                   	c_block230.ideal_buf[channel] = (word)temp;

				if(c_block230.range[channel] == UNIPOLAR_10)
		       			c_block230.ideal_buf[channel] ^= 0x8000;
 
				cd230(&c_block230, channel);

               			wav_mem( 1, i, c_block230.cor_buf[channel], &c_block230);
	      			}
	    		}
		      break;

		      case 4: /* Square Wave */
            		if(!c_block230.bInitialized)
                		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            		else
	    		{
		gloop4:
	      		printf("\nEnter channel number 0 - %d     : ",num_chan - 1);
              		scanf("%x", &channel);
              		printf("\n");
              		if(channel >= num_chan || channel < 0)
              		{
                 		printf("\nChannel out of range.\n");
                 		goto gloop4;
              		}

	      		switch(c_block230.range[channel])
	      		{
				case BIPOLAR_5:
					max = (float)4.999847;
					min = -5.0;
	                		ideal_volt_span = VOLT_SPAN;
				break;

				case UNIPOLAR_10:
					max = (float)9.999847;
					min = 0.0;
	                		ideal_volt_span = VOLT_SPAN;
				break;

				default: 	/* BIPOLAR_10 */
	                		ideal_volt_span = VOLT_SPAN_B10;
					max = (float)9.999695;
					min = -10.0;
				break;
	      		}
	    	      }
				
				mem_span = ( MAX_WAVEFORM_ADDRESS - MIN_WAVEFORM_ADDRESS + 1 ) / 2;

				for( i = MIN_WAVEFORM_ADDRESS; i <= mem_span - 1; i++ )
				{
					v = max;

                    temp =((float)CON16 * v) / ideal_volt_span;

                    if( temp < 0.0)		/* round */
                        temp -= 0.5;
                    else
                        temp += 0.5;

                   	c_block230.ideal_buf[channel] = (word)temp;

					if(c_block230.range[channel] == UNIPOLAR_10)
		        			c_block230.ideal_buf[channel] ^= 0x8000;
 
					cd230(&c_block230, channel);

                			wav_mem( 1, i, c_block230.cor_buf[channel], &c_block230);

				}
				for( i = mem_span; i <= MAX_WAVEFORM_ADDRESS; i++ )
				{
					v = min;

                    temp =((float)CON16 * v) / ideal_volt_span;

                    if( temp < 0.0)		/* round */
                        temp -= 0.5;
                    else
                        temp += 0.5;

                   	c_block230.ideal_buf[channel] = (word)temp;

					if(c_block230.range[channel] == UNIPOLAR_10)
		        			c_block230.ideal_buf[channel] ^= 0x8000;
 
					cd230(&c_block230, channel);

                			wav_mem( 1, i, c_block230.cor_buf[channel], &c_block230);
				}

		      break;

		      case 5: /* Zero Fill */

	                 if(!c_block230.bInitialized)
                		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            		else
	    		{
		gloop6:
	      		printf("\nEnter channel number 0 - %d     : ",num_chan - 1);
              		scanf("%x", &channel);
              		printf("\n");
              		if(channel >= num_chan || channel < 0)
              		{
                 		printf("\nChannel out of range.\n");
                 		goto gloop6;
              		}


	      		switch(c_block230.range[channel])
	      		{
				case UNIPOLAR_10:
					midpoint = -32768;
				break;
				default: 	/* BIPOLAR_10 */
					midpoint = 0;
				break;
	      		}

				for( i = MIN_WAVEFORM_ADDRESS; i <= MAX_WAVEFORM_ADDRESS; i++ )
				{
		       			wav_mem( 1, i, (short)midpoint, &c_block230);
				}
	
	    	       }

		      break;

		      case 6: /* Individual Data */
		try_again2:
           		printf("What Address: 000 to 7ff\n");
           		scanf("%d", &address);
           		if(address < 0 || address > 0x7ff)
           		{
            			printf("\nAddress Out Of Range\n");
            			goto try_again2;
           		}

           		printf("Enter Data:\n");
           		scanf("%x", &data);
           		wav_mem( 1, address, data, &c_block230);
	      		break;
	           }
                }
            }
           break;


        case 13:                                /* read from waveform memory */
            if(!c_block230.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
try_again:
           printf("What Address: 000 to 7ff\n");
           scanf("%x", &address);
           printf("Address is %x\n", address);
           if(address < 0 || address > 0x7ff)
           {
            printf("\nAddress Out Of Range\n");
            goto try_again;
           }
           data = wav_mem( 0, address, 0x0000, &c_block230);
           printf("Data at Address %x = %x\n", address, data);
	    }
            break;


        case 14: /* Write to the Conversion Timer */
            if(!c_block230.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
              {
               printf("Current Value in Conversion Timer: %04X\n",
			(int)input_word( c_block230.nHandle, (word*)&c_block230.brd_ptr->conv_tim ));
tloop1:
               printf("Enter Value in Hex: 0000 to ffff:  ");
               scanf("%x", &time);
               if(time < 0 || time > 0xffff)
               {
                printf("Value out of range\n\a");
                goto tloop1;
               }
               conv_time(time,&c_block230);
               printf("\n");
              }
            break;


        case 15: /* Write to the Timer Prescaler */
            if(!c_block230.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
              {
               printf("Current Value in Timer Prescaler: %02X\n",
			(int)input_word( c_block230.nHandle, (word*)&c_block230.brd_ptr->tim_pres ) & 0xff);
tloop2:
               printf("Enter Value in Hex: 35 to ff:  ");
               scanf("%x", &tim);
               if(tim < 0x35 || tim > 0xff)
               {
                printf("Value not in range\n\a");
                goto tloop2;
               }
               tim_pre(tim,&c_block230);
               printf("\n");
              }
            break;


        case 16:     /* Enable Interrupts */
			hstatus = EnableInterrupts(c_block230.nHandle);
			if(hstatus != S_OK)
			{
			  printf(">>> ERROR WHEN ENABLING INTERRUPTS <<<\n");
			  hflag = 0;
			}
			else
              hflag = 1;

        break;
        
	}   /* end of switch */
     }   /* end of while */


/*
    Reset board to disable interrupts from all counters on this IP module
*/
    if(c_block230.bInitialized)    /* module address was set */
      soft_reset(&c_block230);      /* reset board */

    DisableInterrupts(c_block230.nHandle);
    if(c_block230.bCarrier)
	CarrierClose(c_block230.nHandle);

    printf("\nEXIT PROGRAM\n");
    return 0;
}   /* end of main */





/*
{+D}
    SYSTEM:         IP230/235 Software

    FILENAME:       drvr230.c

    MODULE NAME:    cont_reg- routine to setup the control register.

    VERSION:        REV A

    CREATION DATE:  11-07-96

    DESIGNED BY:    R.H.

    CODED BY:       R.H.

    ABSTRACT:       This module is the setup routine for setting up
                    the control register.

    CALLING
        SEQUENCE:   

    MODULE TYPE:    void

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

       This module is the setup routine for the control register.
*/

void cont_reg(c_blk, hflag)
  struct cblk230 *c_blk;
  int hflag;

{


/*
    Decalre External Data Areas
*/
    extern int model;       /* model type storage */


/*
    DECLARE LOCAL DATA AREAS:
*/
     unsigned done;      /* finished flag */
     int channel;        /* channel number storage */
     int item, item2;           /* selection storage */
     int choice;
     int vector;

/*
   Start of routine
*/

   done = 0;

     while(!done)
     {
      printf(" 1. Return to previous menu\n");
      printf(" 2. Set Trigger Control\n");
      printf(" 3. Set Conversion Mode\n");
      printf(" 4. Software Reset\n");
      printf(" 5. Set Ranges\n");
      if(model == 3 || model == 4)
      {
       printf(" 6. Set Waveform Memory Select\n");
       printf(" 7. Set Waveform Memory Increment Mode\n");
       printf(" 8. Set Interrupt Vector\n\n");
      }
      printf("Select: ");
      scanf("%d",&item);

/*
    perform the menu item selected.
*/  

      switch(item)
      {
        case 1: /* exit this menu */
            done++;
            break;


        case 2:           /* set trigger mode */
          printf("1 - Return to previous menu\n");
          printf("2 - External, Internal and Software Triggers Enabled\n");
          printf("3 - External Trigger Enabled\n");
          printf("4 - External Trigger Output On Software or Internal Trigger\n\n");
          printf("select Item:");
          scanf("%d", &item2);
          printf("\n");
          switch(item2)
          {
           case 1:
              break;

           case 2:
              choice = 1;
              trig_mode(choice, c_blk);
              break;
           case 3:
              choice = 2;
              trig_mode(choice, c_blk);
              break;
           case 4:
              choice = 3;
              trig_mode(choice, c_blk);
              break;
           }
          break;


        case 3:            /* set conversion mode */
          printf("1 - Return to previous menu\n");
          printf("2 - DAC Conversions Disabled\n");
          printf("3 - Single Conversion from DAC Registers\n");
          if(model == 3 || model == 4)
          {
           printf("4 - Single Conversion from Waveform Memory\n");
           printf("5 - Cycle Through Waveform Memory Once\n");
           printf("6 - Cycle Through Waveform Memory Once and Generate Interrupt\n");
           printf("7 - Continuously Cycle Through Waveform Memory\n\n");
          }
          printf("Select Item:");
          scanf("%d", &item2 );
          printf("\n");
          if(item2 == 6 && hflag == 0)
           printf("\nBE SURE TO ATTACH INTERRUPT HANDLERS!\a\a\n");
          switch(item2)
          {
           case 1:
              break;

           case 2:
              choice = 1;
              conv_mode(choice, c_blk);
              break;


           case 3:
              choice = 2;
              conv_mode(choice, c_blk);
              break;


           case 4:
              choice = 3;
              conv_mode(choice, c_blk);
              break;


           case 5:
              choice = 4;
              conv_mode(choice, c_blk);
              break;


           case 6:
              choice = 5;
              conv_mode(choice, c_blk);
              break;


           case 7:
              choice = 6;
              conv_mode(choice, c_blk);
              break;
           }
          break;

      case 4:
        soft_reset(c_blk);
        break;

      case 5:          /* select range for calibration */
         if(model == 1 || model == 3)
          printf("Which Channel? 0 - 3: ");
         else
          printf("Which Channel? 0 - 7: ");
         scanf("%d", &channel);
         printf("\n");
         printf("0: -5V to +5V\n");
         printf("1: -10V to +10V\n");
         printf("2:  0 to +10V\n\n");
         printf("Select Range: ");
         scanf("%d", &item );
         printf("\n");

         range_sel(c_blk, channel, item); 
         break;


        case 6:
         if(model == 1 || model == 3)
          printf("Which Channel? 0 - 3: ");
         else
          printf("Enter Channel Number 0 - 7: ");
           scanf("%d", &channel);
           wav_mem_sel(channel, c_blk);
           printf("\n");
           break;
     
        case 7:                 /* set memory increment mode */
          printf("1 - Return to previous menu\n");
          printf("2 - Enable Automatic Increment of Waveform Memory\n");
          printf("3 - Disable Automatic Increment of Waveform Memory\n\n");
          printf("Select Item:");
          scanf("%d", &item2);
          printf("\n");
          switch(item2)
          {
           case 1:
             break;

           case 2:
             choice = 1;
             wav_mem_inc(choice, c_blk);
             break;

           case 3:
             choice = 2;
             wav_mem_inc(choice, c_blk);
             break;
           }
          break;


        case 8:        /* set interrupt vector */
          printf("Current Interrupt Vector: %02x\n", (int)c_blk->interrupt_vector);
          printf("\n1. Return to previous menu.\n");
          printf("2. Set new interrupt vector.\n");
          printf("Select:");
          scanf("%d",&item2);
          switch(item2)
           {
            case 1:
               break;


            case 2:
              printf("\n\nEnter new interrupt vector:"); 
              scanf("%x", &vector);
              c_blk->interrupt_vector = vector & 0xFF;
              write_vector(c_blk);
              printf("\n");
              break;
            }
          break;
   }
 }
}




/*
{+D}
    SYSTEM:         IP320 Software

    FILENAME:       drvr220.c

    MODULE NAME:    readstat - read board calibration and I.D..

    VERSION:        V1.0

    CREATION DATE:  01/17/94

    DESIGNED BY:    F.J.M.

    CODED BY:       F.J.M

    ABSTRACT:       Routine which is used to "Read Calibration and I.D." and to
		    print the results to the console.

    CALLING
        SEQUENCE:   readstat(&c_block230)
                    where:
                        c_block230 (structure pointer)
                          The address of the configuration param. block

    MODULE TYPE:    void

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
*/

void readstat(c_blk)
struct cblk230 *c_blk;
{


/*
    DECLARE EXTERNAL DATA AREAS:
*/
    extern int model;

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;           /* menu item variable */
    int i;              /* loop index */
    unsigned finished;  /* flags to exit loops */

/*
    ENTRY POINT OF ROUTINE:
*/

    finished = 0;
    while(!finished)
    {
        rmid230(c_blk);
        printf("\n\nBoard Information\n\n");
	printf("Identification:         ");
        for(i = 0; i < 4; i++)		/* identification */
	   printf("%c",c_blk->id_prom[i]);
	printf("\nManufacturer's I.D.:    %X",(byte)c_blk->id_prom[4]);
	printf("\nIP Model Number:        %X",(byte)c_blk->id_prom[5]);
	printf("\nRevision:               %X",(byte)c_blk->id_prom[6]);
	printf("\nReserved:               %X",(byte)c_blk->id_prom[7]);
	printf("\nDriver I.D. (low):      %X",(byte)c_blk->id_prom[8]);
	printf("\nDriver I.D. (high):     %X",(byte)c_blk->id_prom[9]);
	printf("\nTotal I.D. Bytes:       %X",(byte)c_blk->id_prom[10]);
	printf("\nCRC:                    %X",(byte)c_blk->id_prom[11]);
      if(model == 3 || model == 4)
       printf("\n\n\nEvent Status:           %d",c_blk->interrupt_flag);
        printf("\n\n1. Return to Previous Menu\n");
        printf("2. Read Again\n");
      if(model == 3 || model == 4)
        printf("3. Clear Event Status\n");
        printf("\nselect: ");
        scanf("%d",&item);

        switch(item){

        case 1: /* return to previous menu */
            finished++;
            break;

        case 3: /* clear event status */
            c_blk->interrupt_flag = 0;
            break;
        }
     }
 }





