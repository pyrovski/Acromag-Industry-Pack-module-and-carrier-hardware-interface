
#include "../carrier/apc8620.h"
#include "ip236.h"


#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */


/*
{+D}
    SYSTEM:         IP236 Software

    FILENAME:       drvr236.c

    MODULE NAME:    main - main routine of example software.

    VERSION:        REV A

    CREATION DATE:  12-28-98

    DESIGNED BY:    R.H.

    CODED BY:       R.H.

    ABSTRACT:       This module is the main routine for the example program
 		    which demonstrates how the IP236 Library is used.

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
        which demonstrates how the IP236 Library is used.
*/


int main()
{


/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[80];		/* command line input buffer */
    unsigned finished, finished1; /* flag to exit program */
    int hstatus;		/* returned status of routines */
    int item;			/* menu item selection variable */
    float ideal_volt_span;	/* storage span info */
    float f_data;
    long addr;			/* board address */
    int flag;			/* general flag for exiting loops */
    int i,j;			/* loop index */
    int temp;
    struct conf_blk c_block236;	/* configuration block */
    int hflag;                  /* interrupt handler installed flag */
    struct handler_data hdata;  /* interrupt data structure (see exception.h) */
    float fb[MAXSIZE];		/* storage buffer for floats */
    float fs, zs, slope, ge;

/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/

    flag = 0;		/* indicate board address not yet assigned */
    finished = 0;	/* indicate not finished with program */
    hflag = 0;          /* indicate interrupt handler not installed yet */

    for(j = 0; j < MAXSIZE; j++)	/* clear float buffer */
    	fb[j] = 0.0;
    
    for(i = 0; i < NUM_CHAN;i++)
    {
      for(j = 0; j < MAXSIZE; j++)
      {
	c_block236.ideal_data[i][j] = 0;	/* clear ideal data buffer */
	c_block236.cor_data[i][j] = 0;	/* clear corrected data buffer */
      }
      c_block236.coef_bufu10[i][0] = c_block236.coef_bufu10[i][1] = 0;
      c_block236.coef_bufb10[i][0] = c_block236.coef_bufb10[i][1] = 0;
      c_block236.coef_bufb5[i][0]  = c_block236.coef_bufb5[i][1] = 0;

      /* Initialize the Configuration Parameter Block to default values */

      c_block236.mode[i] = 0;		/* mode */
      c_block236.trigger[i] = 0;		/* default triggering */
      c_block236.interrupt_enable[i] = 0;	/* interrupt enable */
      c_block236.threshold[i] = 0;	/* empty threshold */
      c_block236.range[i] = BIPOLAR_10;/* channel range storage */
      c_block236.prescaler[i] = 53;	/* timer prescaler */
      c_block236.counter[i] = 1;		/* conversion counter */
      c_block236.write_size[i] = 1;
    }
    c_block236.interrupt_flag = 0;	/* default to no interrupts */
    c_block236.current_channel = 0;	/* default channel */

    c_block236.interrupt_vector = VECTOR;	/* default interrupt vector */
    c_block236.bCarrier = FALSE;		/* indicate no carrier initialized and set up yet */
    c_block236.bInitialized = FALSE;	/* indicate not ready to talk to IP */
    c_block236.slotLetter = SLOT_A;
    c_block236.nHandle = 0;		/* make handle to a closed carrier board */

    hdata.h_pid = getpid();		/* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block236;/* put in address of c_block236 structure also */

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
    if(CarrierOpen(0, &c_block236.nHandle) != S_OK)
    {
	printf("\nUnable to Open instance of carrier.\n");
	finished = 1;	 /* indicate finished with program */
    }
    else
	flag = 1;


/* Enter main loop */

    while(!finished)
    {
      printf("\nIP236 Library Demonstration  Rev. A \n\n");
      printf(" 1. Exit this Program\n");
      printf(" 2. Set Board Base Address\n");
      printf(" 3. Set IP Slot Letter\n");
      printf(" 4. Read Module I.D./Display Event Status\n");
      printf(" 5. Read Calibration Coefficients\n");
      printf(" 6. Set/Change Channel Number\n");
      printf(" 7. Issue Software Reset to Board\n");
      printf(" 8. Set Up Configuration Parameters\n");
      printf(" 9. Configure Channel\n");
      printf("10. Enter/Change Output Values\n");
      printf("11. Correct Output Values\n");
      printf("12. Write Values to Output FIFO\n");
      printf("13. N/A\n");
      printf("14. Start/Stop Conversions\n");
      printf("15. View Data Buffers & Offset/Gain Coefficients\n");
      printf("16. Clear All Data Buffers\n");
      printf("17. Modify Offset/Gain Coefficients\n");
      printf("18. N/A\n\n");
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
		  SetCarrierAddress(c_block236.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block236.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
          SetCarrierAddress(c_block236.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block236.nHandle) == S_OK)/* Initialize Carrier */
		  {
		        c_block236.bCarrier = TRUE;
			SetInterruptLevel(c_block236.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;

            }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
        break;


	  case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block236.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block236.bCarrier = FALSE;
			}
			else
			{
				c_block236.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block236.nHandle, c_block236.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block236.bInitialized = FALSE;
				}
				else	              
				{	
			            c_block236.brd_ptr = (struct map236 *)addr;
					c_block236.bInitialized = TRUE;
				}
			}
		}
	  break;


        case 4: /* Read ID Prom */
            if(!c_block236.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
		 readid(&c_block236);
        break;

        case 5: /* Read calibration coeficients*/
            if(!c_block236.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	      {
	       for( i = 0; i < NUM_CHAN; i++ )
	       {
		  c_block236.coef_bufu10[i][0] = (word)ReadOffsetError( &c_block236, i, UNIPOLAR_10 );
                  c_block236.coef_bufu10[i][1] = (word)ReadGainError( &c_block236, i, UNIPOLAR_10 );

                  c_block236.coef_bufb5[i][0]  = (word)ReadOffsetError( &c_block236, i, BIPOLAR_5 );
                  c_block236.coef_bufb5[i][1]  = (word)ReadGainError( &c_block236, i, BIPOLAR_5 );

                  c_block236.coef_bufb10[i][0] = (word)ReadOffsetError( &c_block236, i, BIPOLAR_10 );
                  c_block236.coef_bufb10[i][1] = (word)ReadGainError( &c_block236, i, BIPOLAR_10 );
	       }
	    }
            break;    

        case 6: /* channel number */
	    finished1 = 0;
	    while(!finished1)
	    {
 	       printf("\n\nChannel: %x\n\n",c_block236.current_channel);
	       printf(" 1. Return to Previous Menu\n");
	       printf(" 2. Change Channel Number\n");
	       printf("\nSelect: ");
	       scanf("%d",&temp);
	       switch(temp)
	       {
		 case 1: /* return to previous menu */
		    finished1++;
		 break;

		 case 2: /* Select channel */
		     printf("New Channel Number (0 - %d)\n",(NUM_CHAN - 1));
		     c_block236.current_channel = (int)get_param();
		 break;
	       }
	    }
	    break;

        case 7: /* Reset channel */
            if(!c_block236.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
		    reset_board(&c_block236);
        break;

        case 8: /* set up configuration block parameters */
	      setconf236(&c_block236);
        break;

        case 9: /* configure */
            if(!c_block236.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	          cnfg236(&c_block236);
        break;

        case 10: /* Enter Ideal Data */
	    finished1 = 0;
	    while(!finished1)
	    {
	 	printf("\n\nChannel: %x\n\n",c_block236.current_channel);
		printf(" 1. Return to Previous Menu\n");
		printf(" 2. Enter Value(s)\n");
		printf("\nSelect: ");
		scanf("%d",&temp);
		switch(temp)
		{
			case 1: /* return to previous menu */
			    finished1++;
			break;

			case 2: /* enter channel values */
	 		    printf("\nEnter desired voltage: ie: 1.25, follow each with CR %d max)",MAXSIZE);
			    printf("\n  CR alone moves to next value,");
       			    printf("\n  SPACE CR moves to previous value,");
	       		    printf("\n  Q CR quits:\n\n");
			    gets( &cmd_buff[0] );	      /* purge input stream */
         	 	    for( j = 0; j < MAXSIZE; j++ )
           	            {
              		      printf("[%02X]: %f ",j,fb[j]);
              		      gets( &cmd_buff[0] );

	                      if(( cmd_buff[0] == 'Q') || ( cmd_buff[0] == 'q'))
	                           break;
	                      else
	                      {
	                        if( cmd_buff[0] == ' ' )      /* check for space */
 	                            j -= 2; 	   	      /* back up if so */

	                        if( cmd_buff[0] != 0 && cmd_buff[0] != ' ' ) /* check for CR */
	                            sscanf( &cmd_buff[0], "%f", &fb[j]);  /* save */
 	                      }
  	                    }

                            for( i = 0; i < j; i++ )	/* convert data */
                            {
			      if(c_block236.range[c_block236.current_channel] == BIPOLAR_10)
 			         ideal_volt_span = VOLT_SPAN_B10;
			      else	
			         ideal_volt_span = VOLT_SPAN;

			      f_data = (((float)65536 * fb[i]) / ideal_volt_span);

			      if( c_block236.range[c_block236.current_channel] == UNIPOLAR_10 )
			    	f_data -= 32768.0;

			      if( f_data < 0.0)		/* round */
		 		f_data -= 0.5;
		 	      else
				f_data += 0.5;

			      if( f_data > 32767.0 )	/* clip high */
				f_data = 32767.0;

			      if( f_data < -32768.0 )	/* clip low */
				f_data = -32768.0;

			      c_block236.ideal_data[c_block236.current_channel][i] = (short)f_data;
			    }

			    c_block236.write_size[c_block236.current_channel] = j;
			break;
		}
	    }
        break;

        case 11: /* Correct Data */
	    cd236(&c_block236);		/* correct data */ 
        break;

        case 12: /* Write Data to output */
            if(!c_block236.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	      {
	      finished1 = 0;
	      while(!finished1)
	      {

 	        printf("\n\nChannel: %x\n\n",c_block236.current_channel);
	        printf(" 1. Return to Previous Menu\n");
	        printf(" 2. Write Ideal Data Values\n");
	        printf(" 3. Write Corrected Data Values\n");
	        printf("\nSelect: ");
	        scanf("%d",&temp);
	        switch(temp)
	       { 
		      case 1: /* return to previous menu */
			    finished1++;
	   	      break;
		      case 2: /* Ideal */
			    c_block236.current_ptr[c_block236.current_channel] =
			    c_block236.head_ptr[c_block236.current_channel] = &c_block236.ideal_data[c_block236.current_channel][0];
			    c_block236.tail_ptr[c_block236.current_channel] = &c_block236.ideal_data[c_block236.current_channel][0] + c_block236.write_size[c_block236.current_channel];
			    wro236(&c_block236, c_block236.current_channel);
		      break;
		      case 3: /* Corrected */
			    c_block236.current_ptr[c_block236.current_channel] =
			    c_block236.head_ptr[c_block236.current_channel] = &c_block236.cor_data[c_block236.current_channel][0];
			    c_block236.tail_ptr[c_block236.current_channel] = &c_block236.cor_data[c_block236.current_channel][0] + c_block236.write_size[c_block236.current_channel];
			    wro236(&c_block236, c_block236.current_channel);
		      break;
	       }
	      }
	    }
	break;

        case 13:     /* attach exception handler */

            if(!c_block236.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
        break;

        case 14:  /* Start Conversion */
	    finished1 = 0;
	    while(!finished1)
	    {
 	       printf("\n\nChannel: %x\n\n",c_block236.current_channel);
	       printf(" 1. Return to Previous Menu\n");
	       printf(" 2. Start Current Channel\n");
	       printf(" 3. Start Multiple Channels\n");
	       printf(" 4. Stop/Reset Current Channel\n");
	       printf(" 5. Stop/Reset Multiple Channels\n");
	       printf("\nSelect: ");
	       scanf("%d",&temp);
	       switch(temp)
	       { 
		      case 1: /* return to previous menu */
			    finished1++;
	   	      break;
		      case 2: /* Current channel */
		           start_single(&c_block236);
		      break;
		      case 3: /* multiple channels */
		          printf("8 Bit Channel Mask to Start\n");
		          start_multiple(&c_block236,(byte)get_param());
		      break;
		      case 4: /* Current channel */
			   reset_channel(&c_block236);
		      break;
		      case 5: /* stop multiple channels */
		          printf("8 Bit Channel Mask to Stop\n");
		          stop_multiple(&c_block236,(byte)get_param());
		      break;
	       }
	    }
           break;

        case 15:	/* Display Ideal/Corrected Data, Offset/Gain Coefficients */
previous:
	    printf("\n\nChannel: %1d  Range: ",c_block236.current_channel);

	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_5)
		   printf("Bipolar 5  ");
	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_10)
		   printf("Bipolar 10 ");
	    if(c_block236.range[c_block236.current_channel] == UNIPOLAR_10)
		   printf("Unipolar 10");

	    printf("  Coefficients: Offset ");
	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_5)
		   printf("%04X",(c_block236.coef_bufb5[c_block236.current_channel][0] & 0xFFFF));
	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_10)
		   printf("%04X",(c_block236.coef_bufb10[c_block236.current_channel][0] & 0xFFFF));
	    if(c_block236.range[c_block236.current_channel] == UNIPOLAR_10)
		   printf("%04X",(c_block236.coef_bufu10[c_block236.current_channel][0] & 0xFFFF));

	    printf("  Gain ");
	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_5)
		   printf("%04X",(c_block236.coef_bufb5[c_block236.current_channel][1] & 0xFFFF));
	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_10)
		   printf("%04X",(c_block236.coef_bufb10[c_block236.current_channel][1] & 0xFFFF));
	    if(c_block236.range[c_block236.current_channel] == UNIPOLAR_10)
		   printf("%04X",(c_block236.coef_bufu10[c_block236.current_channel][1] & 0xFFFF));

	    printf("\n\nIdeal Data");
	    for( j = 0; j < 8; j++)
	    {
		printf("\n[%02X]",j*8);
		for(i = 0; i < 8; i++)
		      printf("    %04x",c_block236.ideal_data[c_block236.current_channel][j*8+i] & 0xFFFF);
	    }

	    printf("\n\nCorrected Data ");
	    for( j = 0; j < 8; j++)
	    {
		printf("\n[%02X]",j*8);
		for(i = 0; i < 8; i++)
		      printf("    %04x",c_block236.cor_data[c_block236.current_channel][j*8+i] & 0xFFFF);
	    }

	    printf("\n 1. Return to Previous Menu");
	    printf("\n 2. View Next Data Block");
	    printf("\nSelect: ");
	    scanf("%d",&temp);
	    if(temp == 1)
		break;

	    printf("\n\nChannel: %1d  Range: ",c_block236.current_channel);

	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_5)
		   printf("Bipolar 5  ");
	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_10)
		   printf("Bipolar 10 ");
	    if(c_block236.range[c_block236.current_channel] == UNIPOLAR_10)
		   printf("Unipolar 10");

	    printf("  Coefficients: Offset ");
	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_5)
		   printf("%04X",(c_block236.coef_bufb5[c_block236.current_channel][0] & 0xFFFF));
	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_10)
		   printf("%04X",(c_block236.coef_bufb10[c_block236.current_channel][0] & 0xFFFF));
	    if(c_block236.range[c_block236.current_channel] == UNIPOLAR_10)
		   printf("%04X",(c_block236.coef_bufu10[c_block236.current_channel][0] & 0xFFFF));

	    printf("  Gain ");
	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_5)
		   printf("%04X",(c_block236.coef_bufb5[c_block236.current_channel][1] & 0xFFFF));
	    if(c_block236.range[c_block236.current_channel] == BIPOLAR_10)
		   printf("%04X",(c_block236.coef_bufb10[c_block236.current_channel][1] & 0xFFFF));
	    if(c_block236.range[c_block236.current_channel] == UNIPOLAR_10)
		   printf("%04X",(c_block236.coef_bufu10[c_block236.current_channel][1] & 0xFFFF));

	    printf("\n\nIdeal Data");
	    for( j = 8; j < 16; j++)
	    {
		printf("\n[%02X]",j*8);
		for(i = 0; i < 8; i++)
		      printf("    %04x",c_block236.ideal_data[c_block236.current_channel][j*8+i] & 0xFFFF);
	    }

	    printf("\n\nCorrected Data ");
	    for( j = 8; j < 16; j++)
	    {
		printf("\n[%02X]",j*8);
		for(i = 0; i < 8; i++)
		      printf("    %04x",c_block236.cor_data[c_block236.current_channel][j*8+i] & 0xFFFF);
	    }

	    printf("\n 1. Return to Previous Menu");
	    printf("\n 2. View Previous Data Block");
	    printf("\nSelect: ");
	    scanf("%d",&temp);
	    if( temp == 2)
		goto previous;
	 break;


        case 16:        /* clear all data buffers */
	    for(i = 0; i < NUM_CHAN; i++)
	    {
	      for(j = 0; j < MAXSIZE; j++)
	      {
		c_block236.ideal_data[i][j] = 0;	/* clear ideal data buffer */
		c_block236.cor_data[i][j] = 0;	/* clear corrected data buffer */
	      }
	      c_block236.coef_bufu10[i][0] = c_block236.coef_bufu10[i][1] = 0;
	      c_block236.coef_bufb10[i][0] = c_block236.coef_bufb10[i][1] = 0;
	      c_block236.coef_bufb5[i][0]  = c_block236.coef_bufb5[i][1] = 0;
	    }
            break;

  
        case 17: /* modify gain & offset coefficients for a channel */
         if(flag == 0)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
         else
	 {
	    finished1 = 0;
	    while(!finished1)
	    {
 	       printf("\n\nChannel: %x\n\n",c_block236.current_channel);
	       printf(" 1. Return to Previous Menu\n");
	       printf(" 2. Change Channel Number\n");
	       printf(" 3. Change Bipolar   5 Offset & Gain\n");
	       printf(" 4. Change Bipolar  10 Offset & Gain\n");
	       printf(" 5. Change Unipolar 10 Offset & Gain\n");
	       printf(" 6. Calculate Offset & Gain from End Points\n");
	       printf(" 7. Write All Current Channel Coefficients to EEPROM\n");
	       printf("\nSelect: ");
	       scanf("%d",&temp);
	       switch(temp)
	       {
		 case 1: /* return to previous menu */
		    finished1++;
		 break;

		 case 2: /* Select channel */
		     printf("New Channel Number (0 - %d)\n",(NUM_CHAN - 1));
		     c_block236.current_channel = (int)get_param();
		 break;

		 case 3: /* Select Bipolar   5 */
		     printf("New Offset Value\n");
		     c_block236.coef_bufb5[c_block236.current_channel][0] = (short)get_param();
		     printf("New Gain Value\n");
		     c_block236.coef_bufb5[c_block236.current_channel][1] = (short)get_param();
		 break;

		 case 4: /* Select Bipolar  10 */
		     printf("New Offset Value\n");
		     c_block236.coef_bufb10[c_block236.current_channel][0] = (short)get_param();
		     printf("New Gain Value\n");
		     c_block236.coef_bufb10[c_block236.current_channel][1] = (short)get_param();
		 break;

		 case 5: /* Select Unipolar  10 */
		     printf("New Offset Value\n");
		     c_block236.coef_bufu10[c_block236.current_channel][0] = (short)get_param();
		     printf("New Gain Value\n");
		     c_block236.coef_bufu10[c_block236.current_channel][1] = (short)get_param();
		 break;

		 case 6: /* calculate */
		     printf("\nSpan 10 or 20: ");
		     scanf("%d",&temp);
		     printf("\nFull Scale Value: ");
		     scanf("%f",&fs);
		     printf("\nZero Scale Value: ");
		     scanf("%f",&zs);

		     if((fs - zs) == 0.0)
			break;

		     /* do gain calculation */
		     slope = 65536.0 / (fs - zs);
		     /* gain error */
		     ge = (((float)temp * slope) / 65536.0) - 1.0;
		     printf("\nGain = %X", ((short)((ge * 65536.0 * 4) + 0.5) & 0xFFFF));
 
		     /* do offset calculation */
		     slope *= -1;
		     printf("  Offset = %X",((short)((((slope * zs) - 32768.0) * 4) + 0.5) & 0xFFFF));
		 break;


		 case 7: /* Write values to EEPROM */
		  WriteCoefficients( &c_block236, c_block236.current_channel, c_block236.coef_bufu10[c_block236.current_channel][0],
			 c_block236.coef_bufu10[c_block236.current_channel][1], UNIPOLAR_10 );

		  WriteCoefficients( &c_block236, c_block236.current_channel, c_block236.coef_bufb5[c_block236.current_channel][0],
			 c_block236.coef_bufb5[c_block236.current_channel][1], BIPOLAR_5 );

		  WriteCoefficients( &c_block236, c_block236.current_channel, c_block236.coef_bufb10[c_block236.current_channel][0],
			 c_block236.coef_bufb10[c_block236.current_channel][1], BIPOLAR_10 );

		 break;
	       }
	    }
         }
        break;

      
        case 18: /* detach exception handlers */
		hflag = 0;
		DisableInterrupts(c_block236.nHandle);
         break;

        }   /* end of switch */
    }   /* end of while */
/*
    Reset board to disable interrupts from all counters on this IP module
*/
    if(!c_block236.bInitialized)    /* module address was set */
       reset_board(&c_block236);	  /* reset board */

    DisableInterrupts(c_block236.nHandle);
    if(c_block236.bCarrier)
	CarrierClose(c_block236.nHandle);

    printf("\nEXIT PROGRAM\n");
}   /* end of main */




/*
{+D}
    SYSTEM:         IP320 Software

    FILENAME:       drvr236.c

    MODULE NAME:    readid - read board I.D..

    VERSION:        V1.0

    CREATION DATE:  01/17/94

    DESIGNED BY:    F.J.M.

    CODED BY:       F.J.M

    ABSTRACT:       Routine which is used to "Read I.D." and to
			  print the results to the console.

    CALLING
        SEQUENCE:   readid(&c_block236)
                    where:
                        c_block236 (structure pointer)
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

void readid(c_blk)
struct conf_blk *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;           /* menu item variable */
    int i;              /* loop index */
    unsigned finished;  /* flags to exit loops */

/*
    DECLARE MODULES CALLED
*/
    long get_param();   /* input a parameter */

/*
    ENTRY POINT OF ROUTINE:
*/

    finished = 0;
    while(!finished)
    {
        rmid236(c_blk);
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
        printf("\n\n\nEvent Status:           %X",c_blk->interrupt_flag);
        printf("\n\n\n1. Return to Previous Menu\n");
        printf("2. Read Again\n");
        printf("3. Clear Event Status\n");
        printf("\nSelect: ");
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



/*
{+D}
    SYSTEM:         IP236 Software

    FILENAME:       ip236.c

    MODULE NAME:    setconf236 - set configuration block contents.

    VERSION:	    A

    CREATION DATE:  01/08/99

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:	    Routine which is used to enter parameters into
		        the Configuration Block.

    CALLING
	SEQUENCE:   setconf236(c_block236)
		    where:
			c_block236 (structure pointer)
			  The address of the configuration param. block

    MODULE TYPE:    void

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
*/

void setconf236(c_blk)
struct conf_blk *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;			/* menu item variable */
    unsigned finished;        /* flag to exit loop */

/*
    DECLARE MODULES CALLED
*/
    long get_param();	/* input a parameter */

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
        printf("\n\n\nChannel %X Configuration Parameters \n\n",c_blk->current_channel);
 	  printf(" 1. Return to Previous Menu\n");
        printf(" 2. Channel Range:         %X\n",c_blk->range[c_blk->current_channel]);
        printf(" 3. Channel Mode:          %X\n",c_blk->mode[c_blk->current_channel]);
        printf(" 4. Trigger Control:       %X\n",c_blk->trigger[c_blk->current_channel]);
        printf(" 5. Empty Threshold:       %X\n",c_blk->threshold[c_blk->current_channel]);
   	  printf(" 6. Prescaler:             %X\n",c_blk->prescaler[c_blk->current_channel]);
        printf(" 7. Counter:               %X\n",c_blk->counter[c_blk->current_channel]);
        printf(" 8. Interrupt Vector:      %X\n",c_blk->interrupt_vector);
        printf(" 9. Interrupt Enable:      %X\n",c_blk->interrupt_enable[c_blk->current_channel]);
        printf("\nSelect: ");
        scanf("%d",&item);
	switch(item)
	{
 	  case 1: /* return to previous menu */
	    finished++;
	    break;
 
          case 2: /* range */
            printf("0 - Bipolar 5\n");
            printf("1 - Bipolar 10\n");
            printf("2 - Unipolar 10\n");
            c_blk->range[c_blk->current_channel] = (int)get_param();
	    break;

          case 3: /* mode */
            printf("0 - Single Conversion\n");
            printf("1 - Continuous Conversion\n");
            printf("2 - External Trigger Input\n");
           c_blk->mode[c_blk->current_channel] = (int)get_param();
	    break;

          case 4: /* trigger */
            printf("0 - External Input\n");
            printf("1 - External Output\n");
            c_blk->trigger[c_blk->current_channel] = (int)get_param();
	    break;

          case 5: /* threshold */
            printf("0 - Disabled\n");
            printf("1 - Four Values Remaining\n");
            printf("2 - Sixteen Values Remaining\n");
            printf("3 - Sixty-four Values Remaining\n");
            c_blk->threshold[c_blk->current_channel] = (int)get_param();
	    break;

          case 6: /* prescaler*/
            printf("\nPrescaler Value 0x35 to 0xFF\n");
            c_blk->prescaler[c_blk->current_channel] = (int)get_param();
            break;

          case 7: /* counter */
            printf("\nCounter Value 0x0001 to 0xFFFF\n");
            c_blk->counter[c_blk->current_channel] = (word)get_param();
	    break;

          case 8: /* interrupt vector */
            c_blk->interrupt_vector = (byte)get_param();
	    break;

          case 9: /* interrupt enable */
            printf("0 - Interrupts Disabled\n");
            printf("1 - Interrupts Enabled\n");
            c_blk->interrupt_enable[c_blk->current_channel] = (int)get_param();
            break;
         }
    }
}


/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr236.c

    MODULE NAME:    get_param - get a parameter from the console

    VERSION:        A

    CREATION DATE:  01/12/95

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to get parameters

    CALLING
        SEQUENCE:   get_param();

    MODULE TYPE:    long

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


long get_param()
{

/*
    declare local storage.
*/

    long temp;

    printf("Enter hex parameter: ");
    scanf("%lx",&temp);
    printf("\n");
    return(temp);
}

