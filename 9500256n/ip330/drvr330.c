
#include "../carrier/apc8620.h"
#include "ip330.h"


#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */



/*
{+D}
    SYSTEM:	    IP330 Software

    FILENAME:	    drvr330.c

    MODULE NAME:    main - main routine of example software.

    VERSION:	    B

    CREATION DATE:  11/07/95

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M.

    ABSTRACT:	    This module is the main routine for the example program
		    which demonstrates how the IP330 Library is used.

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
--------  ----	------------------------------------------------
04/01/09  FJM   blocking_start_convert

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

	This module is the main routine for the example program
	which demonstrates how the IP330 Library is used.
*/

int main()
{

/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[32];		/* command line input buffer */
    unsigned finished;		/* flag to exit program */
    int item;                   /* menu item selection variable */
    int hstatus;		/* returned status */
    long addr;			/* holds board address */
    int flag;			/* general flag for exiting loops */
    int i, j;                   /* loop index */
    double s;                   /* span value */
    double z;                   /* zero value */
    int hflag;                  /* interrupt handler installed flag */
    struct handler_data hdata;  /* interrupt data structure (see exception.h) */
    struct cblk330 c_block330;    /* configuration block */

    word az_data[4];            /* allocate data storage area */
    word cal_data[4];           /* allocate data storage area */
    word raw_data[SA_SIZE];	/* allocate data storage area */
    int cor_data[SA_SIZE];	/* allocate data storage area */
/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/

    flag = 0;		/* indicate board address not yet assigned */
    finished = 0;	/* indicate not finished with program */
    hflag = 0;          /* indicate interrupt handler not installed yet */
    
    for(i = 0; i < SA_SIZE;i++)
    {
	raw_data[i] = 0;		/* clear raw input buffer */
	cor_data[i] = 0;		/* clear corrected data buffer */
    }

    for(i = 0; i < 4;i++)
    {
        az_data[i] = 0;                 /* clear auto zero buffer */
        cal_data[i] = 0;                /* clear calibration buffer */
    }

/*
    Initialize the Configuration Parameter Block to default values.
*/

    c_block330.range = RANGE_5TO5;         /* default +-5 V */
    c_block330.acq_mode = DI_SELECT;       /* mode */
    c_block330.scan_mode = DISABLE;	/* scan mode */
    c_block330.s_channel = 0;              /* start channel */
    c_block330.e_channel = 0xf;            /* end channel*/
    c_block330.data_format = SB_SELECT;	/* A/D data format */
    c_block330.timer_ps = 0xff;            /* prescaler */
    c_block330.conv_timer = 0x15;          /* counter */
    c_block330.timer_en = TIMER_OFF;	/* timer off */
    c_block330.trigger = TO_SELECT;	/* trigger I/O is output */
    c_block330.int_mode = INT_DIS; 	/* disable interrupt mode */
    c_block330.vector = VECTOR;            /* interrupt vector */
    c_block330.s_raw_buf = &raw_data[0];	/* raw buffer start */
    c_block330.s_az_buf = &az_data[0];	/* auto zero buffer start */
    c_block330.s_cal_buf = &cal_data[0];   /* calibration buffer start */
    c_block330.s_cor_buf = &cor_data[0];   /* corrected buffer start */

    c_block330.new_data[0] = 0;            /* new data flags */
    c_block330.new_data[1] = 0;            /* new data flags */
    c_block330.missed_data[0] = 0;         /* missed data flags */
    c_block330.missed_data[1] = 0;         /* missed data flags */
    c_block330.control = 0;                /* control register used by read only*/

    c_block330.bCarrier = FALSE;	/* indicate no carrier initialized and set up yet */
    c_block330.bInitialized = FALSE;/* indicate not ready to talk to IP */
    c_block330.slotLetter = SLOT_A;
    c_block330.nHandle = 0;	/* make handle to a closed carrier board */

    hdata.h_pid = getpid();		/* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block330;	/* put in address of c_block330 structure also */

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
    if(CarrierOpen(0, &c_block330.nHandle) != S_OK)
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
      printf("\n\n\nIP330 Library Demonstration  Rev. A\n\n");
      printf(" 1. Exit this Program                    11. Correct Data For Scan Array\n");
      printf(" 2. Set Board Base Address               12. Display Auto Zero Data\n");
      printf(" 3. Set IP Slot Letter                   13. Display Calibration Data\n");
      printf(" 4. Set Up Configuration Parameters      14. Display Raw Input Data\n");
      printf(" 5. Read Status Command                  15. Display Corrected Data\n");
      printf(" 6. Enable Interrupts                    16. Display Corrected Data In Volts\n");
      printf(" 7. Disable Interrupts                   17. Clear All Data Buffers\n");
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
		  SetCarrierAddress(c_block330.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block330.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
          SetCarrierAddress(c_block330.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block330.nHandle) == S_OK)/* Initialize Carrier */
		  {
		        c_block330.bCarrier = TRUE;
			SetInterruptLevel(c_block330.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;

				
	    }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
          break;

	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block330.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block330.bCarrier = FALSE;
			}
			else
			{
				c_block330.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block330.nHandle, c_block330.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block330.bInitialized = FALSE;
				}
				else	              
				{	
			                c_block330.brd_ptr = (struct map330 *)addr;
					c_block330.bInitialized = TRUE;
				}
			}
		}
	break;

	case 4: /* set up configuration block parameters */
	    setconf330(&c_block330);
	    break;

        case 5:     /* read board status command */
            if(!c_block330.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
                readstat330(&c_block330); /* read board status */
	    break;
        case 6:     /* attach exception handler */
		EnableInterrupts( c_block330.nHandle );
		hflag = 1;
        break;

        case 7: /* detach exception handlers */
	   DisableInterrupts(c_block330.nHandle);
         break;

        case 8: /* read auto zero values */
            if(!c_block330.bInitialized)
            {
              printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
              break;
            }
              
            calibrate330(&c_block330, AZ_SELECT);   /* get auto-zero values */
            break;

        case 9: /* read calibration data values */
            if(!c_block330.bInitialized)
            {
              printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
              break;
            }
            
            calibrate330(&c_block330, CAL_SELECT);  /* get calibration values */
            break;

        case 10: /* read raw input data values */
            if(!c_block330.bInitialized)
            {
              printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
              break;
            }
            if(hflag == 0 && c_block330.int_mode != 0)
            {
              printf("\n>>> ERROR: NO INTERRUPT HANDLERS ATTACHED <<<\n");
              break;
            }
 
            convert330(&c_block330);                /* convert the board */
            if(c_block330.missed_data[0] != 0)   /* check for missed data */
               printf("\n<<< missed data >>> register 0 = %0x",c_block330.missed_data[0]);

            if(c_block330.missed_data[1] != 0)   /* check for missed data */
               printf("\n<<< missed data >>> register 1 = %0x",c_block330.missed_data[1]);
             break;

        case 11: /* calculate corrected data value for multi channel*/
            mccd330(&c_block330);       /* correct input data */
	    break;

        case 12:        /* display autozero data */
            printf("\naz data, gain x1 %04X",az_data[0]);
            printf(", gain x2 %04X",az_data[1]);
            printf(", gain x4 %04X",az_data[2]);
            printf(", gain x8 %04X\n",az_data[3]);
            break;

        case 13:        /* display calibration data */
            printf("\ncal data, gain x1 %04X",cal_data[0]);
            printf(", gain x2 %04X",cal_data[1]);
            printf(", gain x4 %04X",cal_data[2]);
            printf(", gain x8 %04X\n",cal_data[3]);
            break;

        case 14:        /* display raw input data */
	    for(i = 0; i < SA_SIZE;i++)
	    {

/*
    check for modulo 8 to see if we need to print title info.
*/
		if((i & 0x7) == 0)
		{
		    printf("\nraw data[");
		    printf("%x",(i & 0xf0) >> 4);
		    printf("%x] ",i & 0xf);
		}
/*
    print the data with leading zeros.
*/
		printf(" %x",(raw_data[i] & 0xf000) >> 12);
		printf("%x",(raw_data[i] & 0x0f00) >> 8);
		printf("%x",(raw_data[i] & 0x00f0) >> 4);
		printf("%x",raw_data[i] & 0x000f);
	    }
	    printf("\n");
	    break;
        
        case 15:        /* display corrected data */
	    for(i = 0; i < SA_SIZE;i++)
	    {

/*
    check for modulo 8 to see if we need to print title info.
*/
		if((i & 0x7) == 0)
		{
		    printf("\ncor data[");
		    printf("%x",(i & 0xf0) >> 4);
		    printf("%x] ",i & 0xf);
		}
/*
    print the data with leading zeros.
*/
		printf(" %lx",(cor_data[i] & 0xf0000) >> 16);
		printf("%lx",(cor_data[i] & 0xf000) >> 12);
		printf("%lx",(cor_data[i] & 0x0f00) >> 8);
		printf("%lx",(cor_data[i] & 0x00f0) >> 4);
		printf("%lx",cor_data[i] & 0x000f);
	    }
	    printf("\n");
	    break;

        case 16:        /* display corrected data in volts */
	    switch(c_block330.range)
	    {
                case RANGE_0TO5:
                   z = 0.0000;
                   s = 5.0000;
                break;
                
		case RANGE_5TO5:
		   z = -5.0000;
		   s = 10.0000;
		break;

		case RANGE_0TO10:
		   z =  0.0;
		   s = 10.0000;
		break;

                default:
                   z = -10.0000;        /* RANGE_10TO10 */
		   s =  20.0000;
		break;
	    }

            for(i = j = 0; i < SA_SIZE; i++, j++)
	    {

/*
    check for modulo 4 to see if we need to print title info.
*/

              if((i & 0x3) == 0)
	      {
                  printf("\nVolts[");
		  printf("%x",(i & 0xf0) >> 4);
		  printf("%x] ",i & 0xf);
	      }
/*
    In diferential mode the input gains for channels 0-F are also used
    for channels 10-1F
*/
              if(c_block330.acq_mode == DI_SELECT && i > 0xf)
                j &= 0xf;

              printf("%12.6f ",((double)cor_data[i] / (double)CON16 * s + z) /
                    (double)(1 << get_gain(&c_block330, j)));
	    }
            break;

        case 17:        /* clear all data buffers */
	    for(i = 0; i < SA_SIZE;i++)
	    {
                raw_data[i] = 0;        /* clear raw input buffer */
		cor_data[i] = 0;	/* clear corrected data buffer */
            }
            for(i = 0; i < 4;i++)
	    {
		az_data[i] = 0;		/* clear auto zero buffer */
		cal_data[i] = 0;	/* clear calibration buffer */
            }
	    break;
        }   /* end of switch */
    }	/* end of while */

/*
    disable interrupts from IP module
*/
    if(c_block330.bInitialized)
    {
      c_block330.scan_mode = 0;      /* stop scan */
      c_block330.int_mode = 0;       /* disable interrupts */
      cnfg330(&c_block330);          /* configure the board */
    }

    DisableInterrupts(c_block330.nHandle);
    if(c_block330.bCarrier)
	CarrierClose(c_block330.nHandle);

    printf("\nEXIT PROGRAM\n");
}   /* end of main */




/*
{+D}
    SYSTEM:	    IP330 Software

    FILENAME:	    drvr330.c

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
    SYSTEM:	    IP330 Software

    FILENAME:	    ip330.c

    MODULE NAME:    setconf330 - set configuration block contents.

    VERSION:	    A

    CREATION DATE:  11/07/95

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:	    Routine which is used to enter parameters into
		    the Configuration Block.

    CALLING
	SEQUENCE:   setconf330(c_block330)
		    where:
			c_block330 (structure pointer)
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

void setconf330(c_blk)
struct cblk330 *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;			/* menu item variable */
    unsigned finished, finished2;/* flags to exit loops */
    int i;			/* loop index */
    int elements;		/* number of elements in scan array */

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
        printf("\n\nConfiguration Parameters\n\n");
	printf(" 1. Return to Previous Menu\n");
	printf(" 2. Board Pointer:	%lX\n",(unsigned long)c_blk->brd_ptr);
        printf(" 3. Input Range:        %X\n",c_blk->range);
        printf(" 4. Acquisition Mode:   %X\n",c_blk->acq_mode);
        printf(" 5. Scan Mode:          %X\n",c_blk->scan_mode);
        printf(" 6. Start Channel:      %X\n",c_blk->s_channel);
        printf(" 7. End Channel:        %X\n",c_blk->e_channel);
        printf(" 8. Data Format:        %X\n",c_blk->data_format);
        printf(" 9. Timer Prescaler:    %X\n",c_blk->timer_ps);
        printf("10. Conversion Timer:   %04X\n",c_blk->conv_timer);
        printf("11. Timer Enable:       %X\n",c_blk->timer_en);
        printf("12. Trigger Direction:  %X\n",c_blk->trigger);
        printf("13. Interrupt Mode:     %X\n",c_blk->int_mode);
        printf("14. Interrupt Vector:   %X\n",c_blk->vector);
        printf("15. Set Up Gain Array\n");
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

        case 3: /* input range */
	    c_blk->range = (byte)get_param();
	    break;

        case 4: /* acquisition mode */
            c_blk->acq_mode = (byte)get_param();
	    break;

        case 5: /* scan mode */
	    c_blk->scan_mode = (byte)get_param();
	    break;

        case 6: /* s_channel */
            do
            {
              c_blk->s_channel = (byte)get_param();
            }while(c_blk->s_channel > 31);
	    break;

        case 7: /* e_channel */
            do
            {
              c_blk->e_channel = (byte)get_param();
            }while(c_blk->e_channel > 31);
            break;

        case 8: /* data format */
	    c_blk->data_format = (byte)get_param();
	    break;

        case 9: /* timer prescaler */
            c_blk->timer_ps = (byte)get_param();
	    break;

        case 10: /* conversion timer */
            c_blk->conv_timer = (word)get_param();
	    break;

        case 11: /* timer enable */
	    c_blk->timer_en = (byte)get_param();
	    break;

        case 12: /* trigger direction */
	    c_blk->trigger = (byte)get_param();
	    break;

        case 13: /* interrupt mode */
            c_blk->int_mode = (byte)get_param();
	    break;

        case 14: /* vector */
            c_blk->vector = (byte)get_param();
	    break;

        case 15: /* set up gain array */
	    finished2 = 0;
	    elements = (c_blk->e_channel - c_blk->s_channel) + 1;

	    while(!finished2)
	    {
		printf("\nSet Up Gain Array\n\n");
		printf(" 1. Return to Previous Menu\n");
		for(i = 0; i < elements; i++)
		{
                  printf("%2d. Gain Code for Channel %02X:%02X\n",(i+2),
                    c_blk->s_channel + i,(unsigned)get_gain(c_blk,c_blk->s_channel + i));
		}

		printf("\nselect: ");
		scanf("%d",&item);
		switch(item)
		{
		case 1: /* return to previous menu */
		    finished2++;
		    break;

		default:
		    if(item > 1 && item <= 1 + elements)
		    {
                      printf("gain code - %02X ",(unsigned)get_gain(c_blk, c_blk->s_channel + (item - 2)));
                      put_gain(c_blk, c_blk->s_channel + (item - 2), (byte)(get_param() & 3));
		    }
		    break;
		}
	    }
	    break;
        }
    }
}



/*
{+D}
    SYSTEM:	    IP330 Software

    FILENAME:	    ip330.c

    MODULE NAME:    readstat330 - read board status by using rsts330().

    VERSION:	    A

    CREATION DATE:  11/07/95

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:	    Routine which is used to "Read Board Status" and to
		    print the results to the console.

    CALLING
        SEQUENCE:   readstat330(&c_block330)
		    where:
                        c_block330 (structure pointer)
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

void readstat330(c_blk)
struct cblk330 *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;		/* menu item variable */
    int i;		/* loop index */
    unsigned finished;  /* flags to exit loops */

/*
    ENTRY POINT OF ROUTINE:
*/

    finished = 0;
    while(!finished)
    {

        rsts330(c_blk);          /* Read Status Command */
	printf("\n\nBoard Status Information\n\n");
        printf("Board Control Register: %04X\n",c_blk->control);
        printf("New Data Ch 00-15:      %04X\n",c_blk->new_data[0]);
        printf("New Data Ch 16-31:      %04X\n",c_blk->new_data[1]);
        printf("Missed Data Ch 00-15:   %04X\n",c_blk->missed_data[0]);
        printf("Missed Data Ch 16-31:   %04X\n",c_blk->missed_data[1]);

	rmid330(c_blk);		/*Read Module ID*/
	printf("\nIdentification: 	");
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


