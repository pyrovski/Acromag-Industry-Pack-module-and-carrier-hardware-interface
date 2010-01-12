
#include "../carrier/apc8620.h"
#include "ip340.h"

#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */


/*
{+D}
    SYSTEM:	    IP340 Software

    FILENAME:	    drvr340.c

    MODULE NAME:    main - main routine of example software.

    VERSION:	    C

    CREATION DATE:  11/07/95

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M.

    ABSTRACT:	    This module is the main routine for the example program
		    which demonstrates how the IP340 Library is used.

    CALLING
	SEQUENCE:

    MODULE TYPE:    void

    I/O RESOURCES:

    SYSTEM
	RESOURCES:

    MODULES
	CALLED:

    REVISIONS:

  DATE	  BY	    PURPOSE
-------  ----  ------------------------------------------------
01/11/07 FJM   Add support x86_64
04/01/09 FJM   blocking_start_convert

{-D}
*/

    sint raw_data[16][SA_SIZE];  /* allocate data storage area */
    long cor_data[16][SA_SIZE];  /* allocate data storage area */
    sint count[16];		 /* number of samples in array */
    sint az_data[16];		 /* allocate data storage area */
    sint cal_data[16];		 /* allocate data storage area */

/*
    MODULES FUNCTIONAL DETAILS:

	This module is the main routine for the example program
	which demonstrates how the IP340 Library is used.
*/

int main()
{

/*
    DECLARE EXTERNAL DATA AREAS:
*/

/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[32];		 /* command line input buffer */
    unsigned finished;		 /* flag to exit program */
    int current_channel;	 /* current display channel */
    double value;
    int item;			 /* menu item selection variable */
    long addr;			 /* holds board address */
    int flag;			 /* general flag for exiting loops */
    int i, j;			 /* loop index */
    int hflag;			 /* interrupt handler installed flag */
    struct handler_data hdata;	 /* interrupt data structure (see exception.h) */
    struct cblk340 c_block340;	 /* configuration block */
    int hstatus;		 /* returned status */


/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/

    flag = 0;		/* indicate board address not yet assigned */
    finished = 0;	/* indicate not finished with program */
    hflag = 0;		/* indicate interrupt handler not installed yet */

    memset(raw_data, 0, sizeof(raw_data));
    memset(cor_data, 0, sizeof(cor_data));
    memset(az_data, 0, sizeof(az_data));
    memset(cal_data, 0, sizeof(cal_data));
    memset(count, 0, sizeof(count));

    for(j = 0; j < 16; j++)
    {
      c_block340.s_raw_buf[j] = &raw_data[j][0];	/* raw buffer start */
      c_block340.s_cor_buf[j] = &cor_data[j][0];	/* corrected buffer start */
    }

/*
    Initialize the Configuration Parameter Block to default values.
*/

    c_block340.s_count = &count[0];	/* sample counts */
    c_block340.acq_mode = DI_SELECT;	/* mode */
    c_block340.scan_mode = DISABLE;	/* scan mode */
    c_block340.channel_enable = 0;	/* all disabled */
    c_block340.threshold = 0x1ff;	/* threshold */
    c_block340.ref_v = 0.0;		/* span reference */
    c_block340.conv_timer0 = 0x7fff;	/* counter 0 */
    c_block340.conv_timer1 = 0x80;	/* counter 1 */
    c_block340.trigger = T_DISABLE;	/* no trigger */
    c_block340.int_mode = INT_DIS;		/* disable interrupt mode */
    c_block340.vector = VECTOR;		/* interrupt vector */
    c_block340.s_az_buf = &az_data[0];	/* auto zero buffer start */
    c_block340.s_cal_buf = &cal_data[0];	/* calibration buffer start */

    c_block340.control = 0;		/* control register used by read only*/

    c_block340.bCarrier = FALSE;		/* indicate no carrier initialized and set up yet */
    c_block340.bInitialized = FALSE;	/* indicate not ready to talk to IP */
    c_block340.slotLetter = SLOT_A;
    c_block340.nHandle = 0;		/* make handle to a closed carrier board */

    hdata.h_pid = getpid();		/* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block340;	/* put in address of c_block340 structure also */

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
    if(CarrierOpen(0, &c_block340.nHandle) != S_OK)
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
      printf("\n\n\nIP340/IP341 Library Demonstration  Rev. A\n\n");
      printf(" 1. Exit this Program                 11. Correct Data\n");
      printf(" 2. Set Board Base Address            12. Display Auto Zero Data\n");
      printf(" 3. Set IP Slot Letter                13. Display Calibration Data\n");
      printf(" 4. Set Up Configuration Parameters   14. Display Raw Input Data\n");
      printf(" 5. Read Status Command               15. Display Corrected Data\n");
      printf(" 6. Enable Interrupts                 16. Display Corrected Data In Volts\n");
      printf(" 7. Disable Interrupts                17. Clear All Data Buffers\n");
      printf(" 8. Acquire Auto Zero Data\n");
      printf(" 9. Acquire Calibration Data\n");
      printf("10. Acquire Raw Input Data\n");
      printf("\nSelect: ");
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
		  SetCarrierAddress(c_block340.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block340.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
          SetCarrierAddress(c_block340.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block340.nHandle) == S_OK)/* Initialize Carrier */
		  {
		      c_block340.bCarrier = TRUE;
			SetInterruptLevel(c_block340.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;


	    }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
	  break;

	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block340.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block340.bCarrier = FALSE;
			}
			else
			{
				c_block340.slotLetter = cmd_buff[0];

/*
	Get the IPACK's base address based on the Slot letter,
	and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block340.nHandle, c_block340.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block340.bInitialized = FALSE;
				}
				else
				{
				    c_block340.brd_ptr = (struct map340 *)addr;
					c_block340.bInitialized = TRUE;
				}
			}
		}
	  break;

	case 4: /* set up configuration block parameters */
	    setconf340(&c_block340);
	  break;

	case 5:     /* read board status command */
	    if(!c_block340.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
		readstat340(&c_block340); /* read board status */
	    break;

        case 6:     /* attach exception handler */
		EnableInterrupts( c_block340.nHandle );
		hflag = 1;
        break;
        
        case 7: /* detach exception handlers */
	    DisableInterrupts(c_block340.nHandle);
	    hflag = 0;
         break;

	case 8: /* read auto zero values */
	    if(!c_block340.bInitialized)
	    {
	      printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	      break;
	    }

	    rsts340(&c_block340);  /* Read Status Command also reads voltage reference */

	    calibrate340(&c_block340, AZ_SELECT);   /* get auto-zero values */
	    break;

	case 9: /* read calibration data values */
	    if(!c_block340.bInitialized)
	    {
	      printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	      break;
	    }

	    calibrate340(&c_block340, CAL_SELECT);  /* get calibration values */
	    break;

	case 10: /* read raw input data values */
	    if(!c_block340.bInitialized)
	    {
	      printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	      break;
	    }
	    if(hflag == 0 && c_block340.int_mode != 0)
	    {
	      printf("\n>>> ERROR: NO INTERRUPT HANDLERS ATTACHED <<<\n");
	      break;
	    }

	    convert340(&c_block340);		      /* convert the board */
	     break;

	case 11: /* calculate corrected data value for multi channel*/
	    mccd340(&c_block340);	     /* correct input data */
	    break;

	case 12:	/* display autozero data */
		printf("\naz data [00] ");
		for( i = 0; i < 16; i++)
		{
		  if( i == 8 )
			printf("\naz data [08] ");

		  printf("  %04X",az_data[i]& 0xffff);
		}
	    break;

	case 13:	/* display calibration data */
		printf("\ncal data [00] ");
		for( i = 0; i < 16; i++)
		{
		  if( i == 8 )
			printf("\ncal data [08] ");

		  printf("  %04X",cal_data[i]& 0xffff);
		}
	    break;

	case 14:	/* display raw input data */

	  printf("\n\nEnter channel number in hex: ");
	  scanf("%x",&current_channel);
	    current_channel &= 0xf;	/* limit range */
	    for( i = 0; i < 512; i++)
	    {
/*
    check for modulo 8 to see if we need to print title info.
*/
		if((i & 0x7) == 0)
		{
		    printf("\nCh %X  raw data[",current_channel);
		    printf("%X",(i & 0xF00) >> 8);
		    printf("%X",(i & 0xF0) >> 4);
		    printf("%X] ",i & 0xf);
		}

/*
    print the data with leading zeros.
*/
		printf(" %X",(raw_data[current_channel][i] & 0xf000) >> 12);
		printf("%X",(raw_data[current_channel][i] & 0x0f00) >> 8);
		printf("%X",(raw_data[current_channel][i] & 0x00f0) >> 4);
		printf("%X",raw_data[current_channel][i] & 0x000f);

		if( i == 167 || i == 335 || i == 511)
		{
		    printf("\n\nEnter 0 to return to previous menu or Data Block to View  1, 2, 3.   Select: ");
		    scanf("%d",&j);
		    switch(j)
		    {
			case 1:
				i = -1;
			break;
			case 2:
				i = 167;
			break;
			case 3:
				i = 335;
			break;
			default:
				goto quit_raw;
			break;
		    }
		}

	    }
quit_raw:
	    printf("\n");
	    break;

	case 15:	/* display corrected data */
	  printf("\n\nEnter channel number in hex: ");
	  scanf("%x",&current_channel);
	    current_channel &= 0xf;	/* limit range */

	    for(i = 0; i < SA_SIZE;i++)
	    {

/*
    check for modulo 8 to see if we need to print title info.
*/
		if((i & 0x7) == 0)
		{
		    printf("\nCh %X  cor data[",current_channel);
		    printf("%X",(i & 0xF00) >> 8);
		    printf("%X",(i & 0xf0) >> 4);
		    printf("%X] ",i & 0xf);
		}
/*
    print the data with leading zeros.
*/
		printf(" %lX",(cor_data[current_channel][i] & 0xf0000) >> 16);
		printf("%lX",(cor_data[current_channel][i] & 0xf000) >> 12);
		printf("%lX",(cor_data[current_channel][i] & 0x0f00) >> 8);
		printf("%lX",(cor_data[current_channel][i] & 0x00f0) >> 4);
		printf("%lX",cor_data[current_channel][i] & 0x000f);

		if( i == 167 || i == 335 || i == 511)
		{
		    printf("\n\nEnter 0 to return to previous menu or Data Block to View  1, 2, 3.   Select: ");
		    scanf("%d",&j);
		    switch(j)
		    {
			case 1:
				i = -1;
			break;
			case 2:
				i = 167;
			break;
			case 3:
				i = 335;
			break;
			default:
				goto quit_cor;
			break;
		    }
		}

	    }
quit_cor:
	    printf("\n");
	    break;

	case 16:	/* display corrected data in volts */
	  printf("\n\nEnter channel number in hex: ");
	  scanf("%x",&current_channel);
	    current_channel &= 0xf;	/* limit range */
	    for(i = j = 0; i < SA_SIZE; i++, j++)
	    {

/*
    check for modulo 4 to see if we need to print title info.
*/

	      if((i & 0x3) == 0)
	      {
		  printf("\nCh %X  Volts[",current_channel);
		printf("%X",(i & 0xF00) >> 8);
		  printf("%x",(i & 0xf0) >> 4);
		  printf("%x] ",i & 0xf);
	      }

            printf("%12.6f ",(double)cor_data[current_channel][i] * (double)305.2e-6);

		if( i == 83 || i == 167 || i == 251 || i == 335 || i == 419 || i == 511)
		{
		    printf("\n\nEnter 0 returns to previous menu or View Data Block 1, 2, 3, 4, 5, 6. Select: ");
		    scanf("%d",&j);
		    switch(j)
		    {
			case 1:
				i = -1;
			break;
			case 2:
				i = 83;
			break;
			case 3:
				i = 167;
			break;
			case 4:
				i = 251;
			break;
			case 5:
				i = 335;
			break;
			case 6:
				i = 419;
			break;
			default:
				goto quit_volt;
			break;
		    }
		}

	    }
quit_volt:
	    break;

	case 17:	/* clear all data buffers */

	     memset(raw_data, 0, sizeof(raw_data));
	     memset(cor_data, 0, sizeof(cor_data));
	     memset(az_data, 0, sizeof(az_data));
	     memset(cal_data, 0, sizeof(cal_data));
	     memset(count, 0, sizeof(count));

	    break;

	case 18:	/* write reference value */

	  printf("\n\nEnter floating point reference value: ");
	  scanf("%lf",&value);
	    WriteReferenceValue( &c_block340, value );
	    break;

	}   /* end of switch */
    }	/* end of while */


/*
    disable interrupts from IP module
*/
    if(c_block340.bInitialized)
    {
      c_block340.scan_mode = 0;	  /* stop scan */
      c_block340.int_mode = 0;	  /* disable interrupts */
      cnfg340(&c_block340);	  /* configure the board */
    }

    DisableInterrupts(c_block340.nHandle);
    if(c_block340.bCarrier)
	CarrierClose(c_block340.nHandle);

    printf("\nEXIT PROGRAM\n");
}   /* end of main */




/*
{+D}
    SYSTEM:	    IP340 Software

    FILENAME:	    drvr340.c

    MODULE NAME:    get_param - get a parameter from the console

    VERSION:	    V1.0

    CREATION DATE:  3/24/89

    DESIGNED BY:    RTL

    CODED BY:	    RTL

    ABSTRACT:	    Routine which is used to get parameters

    CALLING
	SEQUENCE:   get_param();

    MODULE TYPE:    long

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


long get_param()
{

/*
    declare local storage.
*/

    long temp;

/*
    print prompt string
*/
    printf("enter hex parameter: ");

/*
    get input
*/
    scanf("%lx",&temp);

/*
    output a linefeed
*/
    printf("\n");

    return(temp);
}

/*
{+D}
    SYSTEM:	    IP340 Software

    FILENAME:	    ip340.c

    MODULE NAME:    setconf340 - set configuration block contents.

    VERSION:	    A

    CREATION DATE:  11/07/95

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:	    Routine which is used to enter parameters into
		    the Configuration Block.

    CALLING
	SEQUENCE:   setconf340(c_block340)
		    where:
			c_block340 (structure pointer)
			  The address of the configuration param. block

    MODULE TYPE:    void

    I/O RESOURCES:

    SYSTEM
	RESOURCES:

    MODULES
	CALLED:     get_param()     input a parameter from console

    REVISIONS:

  DATE	  BY	    PURPOSE
-------  ----	------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:
*/

void setconf340(c_blk)
struct cblk340 *c_blk;
{

/*
    DEFINITIONS:
*/


/*
    DECLARE EXTERNAL DATA AREAS:
*/


/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;			/* menu item variable */
    unsigned finished;		/* flag to exit loop */

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
	printf("\n\nConfiguration Parameters\n\n");
	printf(" 1. Return to Previous Menu\n");
	printf(" 2. Board Pointer:        %lx\n",(unsigned long)c_blk->brd_ptr);
	printf(" 3. Acquisition Mode:     %x\n",c_blk->acq_mode);
	printf(" 4. Scan Mode:            %x\n",c_blk->scan_mode);
	printf(" 5. Enter Channel Bitmap: %x\n",c_blk->channel_enable & 0xffff);
	printf(" 6. Conversion Timer0:    %08lx\n",c_blk->conv_timer0);
	printf(" 7. Conversion Timer1:    %08lx\n",c_blk->conv_timer1);
	printf(" 8. FIFO Threshold:       %04x\n",c_blk->threshold);
	printf(" 9. External Trigger:     %x\n",c_blk->trigger);
	printf("10. Interrupt Mode:       %x\n",c_blk->int_mode);
	printf("11. Interrupt Vector:     %x\n",c_blk->vector);
	printf("\nselect: ");
	scanf("%d",&item);
	switch(item)
	{
	case 1: /* return to previous menu */
	    finished++;
	    break;

	case 2: /* board address */
	    printf("ADDRESS CAN BE CHANGED ONLY IN THE MAIN MENU\n");
	    break;

	case 3: /* acquisition mode */
	    c_blk->acq_mode = (byte)get_param();
	    break;

	case 4: /* scan mode */
	      c_blk->scan_mode = (byte)get_param();
	    break;

	case 5: /* channel bitmap */
	    c_blk->channel_enable = (word)get_param();
	    break;

	case 6: /* conversion timer */
	    c_blk->conv_timer0 = (long)get_param();
		c_blk->conv_timer0 &= 0x0FFFFF;
	    break;

	case 7: /* conversion timer */
	    c_blk->conv_timer1 = (long)get_param();
		c_blk->conv_timer1 &= 0x0FFFFF;
	    break;

	case 8: /* threshold */
	      c_blk->threshold = (word)get_param();
	      c_blk->threshold &= 0x01FF;
	    break;

	case 9: /* trigger */
	    c_blk->trigger = (byte)get_param();
	    break;

	case 10: /* interrupt mode */
	    c_blk->int_mode = (byte)get_param();
	    break;

	case 11: /* vector */
	    c_blk->vector = (byte)get_param();
	    break;
	}
    }
}



/*
{+D}
    SYSTEM:	    IP340 Software

    FILENAME:	    ip340.c

    MODULE NAME:    readstat340 - read board status by using rsts340().

    VERSION:	    A

    CREATION DATE:  11/07/95

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:	    Routine which is used to "Read Board Status" and to
		    print the results to the console.

    CALLING
	SEQUENCE:   readstat340(&c_block340)
		    where:
			c_block340 (structure pointer)
			  The address of the configuration param. block

    MODULE TYPE:    void

    I/O RESOURCES:

    SYSTEM
	RESOURCES:

    MODULES
	CALLED:     get_param()     input a parameter from console.
		    rsts340()	    Read Board Status Command.

   REVISIONS:

  DATE	  BY	    PURPOSE
-------  ----	------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:
*/

void readstat340(c_blk)
struct cblk340 *c_blk;
{

/*
    DEFINITIONS:
*/


/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;		/* menu item variable */
    int i;		/* loop index */
    unsigned finished;  /* flags to exit loops */

/*
    DECLARE MODULES CALLED
*/
    long get_param();	/* input a parameter */
    void rsts340();	/* Read Board Status Command */

/*
    ENTRY POINT OF ROUTINE:
*/

    finished = 0;
    while(!finished)
    {
	rsts340(c_blk); 	 /* Read Status Command */
	printf("\n\nBoard Status Information\n\n");
	printf("Board Control Register: %04x\n",c_blk->control);

        printf("Reference Constant: %16.6f\n",c_blk->ref_v);

        printf("Identification: 	");
	for(i = 0; i < 4; i++)		/* identification */
           printf("%c",c_blk->id_prom[i]);
        printf("\nManufacturer's I.D.:    %x",(byte)c_blk->id_prom[4]);
        printf("\nIP Model Number:        %x",(byte)c_blk->id_prom[5]);
        printf("\nRevision:               %x",(byte)c_blk->id_prom[6]);
        printf("\nReserved:               %x",(byte)c_blk->id_prom[7]);
        printf("\nDriver I.D. (low):      %x",(byte)c_blk->id_prom[8]);
        printf("\nDriver I.D. (high):     %x",(byte)c_blk->id_prom[9]);
        printf("\nTotal I.D. Bytes:       %x",(byte)c_blk->id_prom[10]);
        printf("\nCRC:                    %x",(byte)c_blk->id_prom[11]);
	printf("\n\n1 Return to Previous Menu\n");
	printf("2 Read Status Again\n");
	printf("\nselect: ");
	scanf("%d",&item);

	switch(item){

	case 1: /* return to previous menu */
	    finished++;
	    break;
	}
    }
}


