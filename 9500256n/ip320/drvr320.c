
#include "../carrier/apc8620.h"
#include "ip320.h"


#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */


struct scan_array s_array[SA_SIZE]; 


/*
{+D}
    SYSTEM:         IP320 Software

    FILENAME:       drvr320.c

    MODULE NAME:    main - main routine of example software.

    VERSION:        REV A

    CREATION DATE:  05/19/98

    DESIGNED BY:    F.J.M.

    CODED BY:       F.J.M.

    ABSTRACT:       This module is the main routine for the example program
		    which demonstrates how the IP320 Library is used.

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
        which demonstrates how the IP320 Library is used.
*/

int main()
{


/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[32];		/* command line input buffer */
    unsigned finished;		/* flag to exit program */
    int item;			/* menu item selection variable */
    long addr;			/* integer to hold board address */
    int flag;			/* general flag for exiting loops */
    int i;			/* loop index */
    byte temp_mode;		/* mode storage */
    float s;			/* span value */
    float z;			/* zero value */
    float u;
    int scale_flag;

    struct cblk320 c_block320;    /* configuration block */

    word az_data[SA_SIZE];	/* allocate data storage area */
    word cal_data[SA_SIZE];	/* allocate data storage area */
    word raw_data[SA_SIZE];	/* allocate data storage area */
    int cor_data[SA_SIZE];	/* allocate data storage area */

/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/

    flag = 0;		/* indicate board address not yet assigned */
    finished = 0;	/* indicate not finished with program */
    scale_flag = 0;	/* default to single channel */

/*
    initialize the scan array. This array may be any
    reasonable size the user wishes to support in his own code.
    This example uses a size of 20 for convenience.
    NON-SEQUENTIAL and REPEATED channels may be entered to produce
    the desired scanning.   By default, the first 20 channels are
    configured with gain=1.
*/
    for( i = 0; i < SA_SIZE; i++ )
    {
	s_array[ i ].gain = 1;	/*  gain=1 */
	s_array[ i ].chn = i;	/*  channel */
    }

    for(i = 0; i < SA_SIZE;i++)
    {
	az_data[i] = 0;			/* clear auto zero buffer */
	cal_data[i] = 0;		/* clear calibration buffer */
	raw_data[i] = 0;		/* clear raw input buffer */
	cor_data[i] = 0;		/* clear corrected data buffer */
    }

/*
    Initialize the Configuration Parameter Block to default values.
*/

    c_block320.range = RANGE_5TO5;		/* default +-5 V */
    c_block320.trigger = STRIG;		/* 0 = software triggering */
    c_block320.mode = DIF;			/* mode */
    c_block320.gain = GAIN_X1;		/* gain for analog input */
    c_block320.average = 1;		/* number of samples to average */
    c_block320.channel = 0;		/* default channel */
    c_block320.data_mask = BIT12;		/* A/D converter data mask */
    c_block320.bit_constant = CON12;	/* constant for data correction */
    c_block320.s_raw_buf = &raw_data[0];	/* raw buffer start */
    c_block320.s_az_buf = &az_data[0];	/* auto zero buffer start */
    c_block320.s_cal_buf = &cal_data[0];	/* calibration buffer start */
    c_block320.s_cor_buf = &cor_data[0];	/* corrected buffer start */
    c_block320.sa_start = &s_array[0];	/* address of start of scan array */
    c_block320.sa_end = c_block320.sa_start;	/* address of end of scan array */

    c_block320.bCarrier = FALSE;		/* indicate no carrier initialized and set up yet */
    c_block320.bInitialized = FALSE;	/* indicate not ready to talk to IP */
    c_block320.slotLetter = SLOT_A;
    c_block320.nHandle = 0;		/* make handle to a closed carrier board */

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
    if(CarrierOpen(0, &c_block320.nHandle) != S_OK)
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
      printf("\nIP320 Library Demonstration  Rev. A\n");
      printf(" 1. Exit this Program\n");
      printf(" 2. Set Board Base Address\n");
      printf(" 3. Set IP Slot Letter\n");
      printf(" 4. Set Up Configuration Block Parameters\n");
      printf(" 5. Read Status Command\n");
      printf(" 6. Read Single Channel Data\n");
      printf(" 7. Acquire Auto Zero Values For Scan Array\n");
      printf(" 8. Acquire Raw Input Data Values For Scan Array\n");
      printf(" 9. Acquire Calibration Data For Scan Array\n");
      printf("10. Correct Data For Single Channel\n");
      printf("11. Correct Data For Scan Array\n");
      printf("12. Display Auto Zero Data\n");
      printf("13. Display Raw Input Data\n");
      printf("14. Display Calibration Data\n");
      printf("15. Display Corrected Data\n");
      printf("16. Display Corrected Data In Volts\n");
      printf("17. Clear All Data Buffers\n");
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
		  SetCarrierAddress(c_block320.nHandle, addr);	/* Set Carrier Address */
		}

	 	GetCarrierAddress(c_block320.nHandle, &addr);	/*  Read back carrier address */
		printf("address: %lx\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
            SetCarrierAddress(c_block320.nHandle, addr);	/* Set Carrier Address */
			if(CarrierInitialize(c_block320.nHandle) == S_OK)		/* Initialize Carrier */
			{
				c_block320.bCarrier = TRUE;
			}
			flag = 1;
		  }
		  else
		  {
		    flag = 0;
		  }
		 }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
            break;

	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block320.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block320.bCarrier = FALSE;
			}
			else
			{
				c_block320.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block320.nHandle, c_block320.slotLetter, &addr) != S_OK)
				{
				   printf("\nUnable to Get Ipack Address.\n");
				   c_block320.bInitialized = FALSE;
				}
				else	              
				{	
			           c_block320.brd_ptr = (struct map320 *)addr;
				   c_block320.bInitialized = TRUE;
				}
			}
		}
	    break;

        case 4: /* set up configuration block parameters */
	    if(!c_block320.bInitialized)
		printf("\n>>> ERROR: BOARD NOT SET UP <<<\n");
	    else
		setconf320(&c_block320);
            break;

        case 5:     /* read board status command */
	    if(!c_block320.bInitialized)
		printf("\n>>> ERROR: BOARD NOT SET UP <<<\n");
	    else
		readstat320(&c_block320); /* read board status */
            break;

        case 6: /* read single channel */
	    if(!c_block320.bInitialized)
	        printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	        ainsc320(&c_block320);  /* acquire input data */
	    scale_flag = 0;	    /* flag single channel conversion */
            break;

        case 7: /* read auto zero values */
	    if(!c_block320.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
	       temp_mode = c_block320.mode;
	       c_block320.mode = AZV;  /* mode */
               ainmc320(&c_block320);  /* acquire input data */
	       c_block320.mode = temp_mode;
	    }
            break;

        case 8: /* read raw input data values */
	    if(!c_block320.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
               ainmc320(&c_block320);  /* acquire input data */

	    scale_flag = 1;	    /* flag scan array conversion */
            break;

        case 9: /* read calibration data values */
	    if(!c_block320.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
	       temp_mode = c_block320.mode;
	       c_block320.mode = CAL;  /* mode */
               ainmc320(&c_block320);  /* acquire input data */
	       c_block320.mode = temp_mode;
	    }
            break;

        case 10: /* calculate corrected data value for single channel*/
	    if(!c_block320.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
               sccd320(&c_block320);	/* correct input data */
            break;

        case 11: /* calculate corrected data value for multi channel*/
	    if(!c_block320.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
               mccd320(&c_block320);	/*correct input data */
            break;

        case 12:	/* display autozero data */
            for(i = 0; i < SA_SIZE;i++)
	    {

/*
    check for modulo 8 to see if we need to print title info.
*/
                if((i & 0x7) == 0)
		{
                    printf("\naz data[");
                    printf("%x",(i & 0xf0) >> 4);
                    printf("%x] ",i & 0xf);
                }
/*
    print the data with leading zeros.
*/
                printf(" %x",(az_data[i] & 0xf000) >> 12);
                printf("%x",(az_data[i] & 0x0f00) >> 8);
                printf("%x",(az_data[i] & 0x00f0) >> 4);
                printf("%x",az_data[i] & 0x000f);
            }
            printf("\n");
            break;

        case 13:        /* display raw input data */
            for(i = 0; i < SA_SIZE;i++)
	    {

/*    check for modulo 8 to see if we need to print title info. */
                if((i & 0x7) == 0)
		{
                    printf("\nraw data[");
                    printf("%x",(i & 0xf0) >> 4);
                    printf("%x] ",i & 0xf);
                }
/*    print the data with leading zeros. */
                printf(" %x",(raw_data[i] & 0xf000) >> 12);
                printf("%x",(raw_data[i] & 0x0f00) >> 8);
                printf("%x",(raw_data[i] & 0x00f0) >> 4);
                printf("%x",raw_data[i] & 0x000f);
            }
            printf("\n");
            break;

        case 14:        /* display calibration data */
            for(i = 0; i < SA_SIZE;i++)
	    {

/*    check for modulo 8 to see if we need to print title info. */
                if((i & 0x7) == 0)
		{
                    printf("\ncal data[");
                    printf("%x",(i & 0xf0) >> 4);
                    printf("%x] ",i & 0xf);
                }
/*    print the data with leading zeros. */
                printf(" %x",(cal_data[i] & 0xf000) >> 12);
                printf("%x",(cal_data[i] & 0x0f00) >> 8);
                printf("%x",(cal_data[i] & 0x00f0) >> 4);
                printf("%x",cal_data[i] & 0x000f);
            }
            printf("\n");
            break;

        case 15:        /* display corrected data */
            for(i = 0; i < SA_SIZE;i++)
	    {

/*    check for modulo 8 to see if we need to print title info. */
                if((i & 0x7) == 0)
		{
                    printf("\ncor data[");
                    printf("%X",(i & 0xf0) >> 4);
                    printf("%X] ",i & 0xf);
                }
/*    print the data with leading zeros. */
                printf(" %lX",(cor_data[i] & 0xf0000) >> 16);
                printf("%lX",(cor_data[i] & 0xf000) >> 12);
                printf("%lX",(cor_data[i] & 0x0f00) >> 8);
                printf("%lX",(cor_data[i] & 0x00f0) >> 4);
                printf("%lX",cor_data[i] & 0x000f);
            }
            printf("\n");
            break;

        case 16:        /* display corrected data in volts */
	    switch(c_block320.range)
	    {
		case RANGE_5TO5:
		   z = -5.0000;
		   s = 10.0000;
		break;

		case RANGE_0TO10:
		   z =  0.0;
		   s = 10.0000;
		break;

		default:	/*  RANGE_10TO10 */
		   z = -10.0000;
		   s =  20.0000;
		break;
	    }

	    for(i = 0; i < SA_SIZE; i++)
	    {

/*    check for modulo 8 to see if we need to print title info. */

	      if((i & 0x7) == 0)
	      {
	          printf("\nVolts[");
	          printf("%x",(i & 0xf0) >> 4);
	          printf("%x] ",i & 0xf);
	      }

	      if(cor_data[i] > c_block320.bit_constant)
		u = (float)cor_data[i] - (float)CON16;
	      else
		u = (float)cor_data[i];

	      if(scale_flag == 0)	/* single channel */
	        printf("%2.3f ",(u / (float)c_block320.bit_constant * s + z) / (float)c_block320.gain);
	      else
		printf("%2.3f ",(u / (float)c_block320.bit_constant * s + z) / (float)s_array[i].gain);
	    }
            printf("\n");
            break;
        case 17:        /* clear all data buffers */
	    for(i = 0; i < SA_SIZE;i++)
	    {
		az_data[i] = 0;		/* clear auto zero buffer */
		cal_data[i] = 0;	/* clear calibration buffer */
		raw_data[i] = 0;	/* clear raw input buffer */
		cor_data[i] = 0;	/* clear corrected data buffer */
	    }
            break;

        }   /* end of switch */
    }   /* end of while */
    DisableInterrupts(c_block320.nHandle);
    if(c_block320.bCarrier)
	CarrierClose(c_block320.nHandle);

    printf("\nEXIT PROGRAM\n");

}   /* end of main */



/*
{+D}
    SYSTEM:         IP320 Software

    FILENAME:       drvr320.c

    MODULE NAME:    get_param - get a parameter from the console

    VERSION:        V1.0

    CREATION DATE:  3/24/89

    DESIGNED BY:    RTL

    CODED BY:       RTL

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
    SYSTEM:         IP320 Software

    FILENAME:       libip320.c

    MODULE NAME:    setconf320 - set configuration block contents.

    VERSION:        V1.1

    CREATION DATE:  3/24/89

    DESIGNED BY:    RTL

    CODED BY:       RTL

    ABSTRACT:       Routine which is used to enter parameters into
		    the Configuration Block.

    CALLING
        SEQUENCE:   setconf320(c_block320)
                    where:
                        c_block320 (structure pointer)
                          The address of the configuration param. block

    MODULE TYPE:    void

    I/O RESOURCES:  

    SYSTEM
        RESOURCES:  

    MODULES
        CALLED:     get_param()     input a parameter from console

    REVISIONS:      

  DATE    BY        PURPOSE
-------  ----   ------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:
*/

void setconf320(c_blk)
struct cblk320 *c_blk;
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
    int item;                   /* menu item variable */
    unsigned finished, finished2;/* flags to exit loops */
    int i;                      /* loop index */
    int elements;               /* number of elements in scan array */

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
        printf("\n\nConfiguration Block Parameters\n\n");
        printf(" 1. Return to Previous Menu\n");
        printf(" 2. Board Pointer:      %lX\n",(unsigned long)c_blk->brd_ptr);
        printf(" 3. Input Jumper Range: %X\n",c_blk->range);
        printf(" 4. Triggering:         %X\n",c_blk->trigger);
        printf(" 5. Mode:               %X\n",c_blk->mode);
        printf(" 6. Gain:               %X\n",c_blk->gain);
        printf(" 7. Average:            %X\n",c_blk->average);
        printf(" 8. Channel:            %X\n",c_blk->channel);
        printf(" 9. Data Mask:          %04X\n",c_blk->data_mask);
        printf("10. Data Constant:      %04lX\n",c_blk->bit_constant);
        printf("11. Set Up Scan Array\n");
        printf("    Scan Array Start:   %lX\n",(unsigned long)c_blk->sa_start);
        printf("    Scan Array End:     %lX\n",(unsigned long)c_blk->sa_end);
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

        case 3: /* input jumper range */
            c_blk->range = (byte)get_param();
            break;

        case 4: /* triggering */
            c_blk->trigger = (byte)get_param();
            break;

        case 5: /* mode of operation */
	    c_blk->mode = (byte)get_param();
            break;

        case 6: /* gain of analog input */
            c_blk->gain = (word)get_param();
            break;

        case 7: /* average */
            c_blk->average = (word)get_param();
            break;

        case 8: /* channel */
            c_blk->channel = (word)get_param();
            break;

        case 9: /* data mask */
            c_blk->data_mask = (word)get_param();
            break;

        case 10: /* data constant */
            c_blk->bit_constant = (long)get_param();
            break;

        case 11:    /* set up scan array */
            finished2 = 0;
/*
  compute the number of elements in the scan array by doing pointer arithmetic
*/
            elements = (c_blk->sa_end - c_blk->sa_start) + 1;

            while(!finished2)
	    {
                printf("\nSet Up Scan Array\n\n");
                printf(" 1. Return to Previous Menu\n");
                printf(" 2. No. of elements (%d decimal max): %d\n",SA_SIZE,elements);
                for(i = 0; i < elements; i++)
		{
                  printf("%2d. Ch & Gain Codes for Array Index %2X:%3X%3X\n",
                    (i+3),i,(c_blk->sa_start+i)->chn,(c_blk->sa_start+i)->gain);
                }
                printf("\nselect: ");
                scanf("%d",&item);
                switch(item)
		{
                case 1: /* return to previous menu */
                    finished2++;
                    break;

                case 2: /* get no. of elements */

                    elements = (word)get_param();
                    if(elements < 1)
                        elements = 1;
                    else
		    {
                      if(elements > SA_SIZE)
                         elements = SA_SIZE;
                    }
                    c_blk->sa_end = c_blk->sa_start + (elements - 1);
                    break;
 
                default:
                    if(item > 2 && item <= 2 + elements)
		    {
                        printf("channel number - ");
                        (c_blk->sa_start+item-3)->chn = (word)get_param();
                        printf("gain (1,2,4,8) - ");
                        (c_blk->sa_start+item-3)->gain = (word)get_param();
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
    SYSTEM:         IP320 Software

    FILENAME:       libip320.c

    MODULE NAME:    readstat320 - read board status by using rsts320().

    VERSION:        V1.0

    CREATION DATE:  3/24/89

    DESIGNED BY:    RTL

    CODED BY:       RTL

    ABSTRACT:       Routine which is used to "Read Board Status" and to
		    print the results to the console.

    CALLING
        SEQUENCE:   readstat320(&c_block320)
                    where:
                        c_block320 (structure pointer)
                          The address of the config param. block

    MODULE TYPE:    void

    I/O RESOURCES:  

    SYSTEM
        RESOURCES:  

    MODULES
        CALLED:     get_param()     input a parameter from console.
                    rsts320()       Read Board Status Command.

   REVISIONS:      

  DATE    BY        PURPOSE
-------  ----   ------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:
*/

void readstat320(c_blk)
struct cblk320 *c_blk;
{


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
        rsts320(c_blk);          /* Read Status Command */     
        printf("\n\nBoard Status Information\n\n");
        printf("Board Control Register: %04X\n",c_blk->control);
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

