
#include "../carrier/apc8620.h"
#include "ip480.h"


#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */



/*
{+D}
    SYSTEM:         IP480 Software

    FILENAME:       drvr480.c

    MODULE NAME:    main - main routine of example software.

    VERSION:	    REV B

    CREATION DATE:  05/19/98

    DESIGNED BY:    F.J.M.

    CODED BY:	    F.J.M.

    ABSTRACT:	    This module is the main routine for the example program
                    which demonstrates how the IP480 Library is used.

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
-------  ----	------------------------------------------------
04/01/09  FJM   blocking_start_convert

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

	This module is the main routine for the example program
        which demonstrates how the IP480 Library is used.
*/

int main()
{


/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[32];		/* command line input buffer */
    unsigned finished;		/* flag to exit program */
    UWORD cont_reg;
    int item;                   /* menu item selection variable */
    int hstatus;		/* returned status */
    long addr;			/* integer to hold board address */
    int flag;			/* general flag for exiting loops */
    int i;                      /* loop index */
    int hflag;                  /* interrupt handler installed flag */
    ULONG ulong_value;
    struct handler_data hdata;  /* interrupt data structure (see exception.h) */
    struct cblk480 c_block480;    /* configuration block */

/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/

    flag = 0;		/* indicate board address not yet assigned */
    finished = 0;	/* indicate not finished with program */
    hflag = 0;          /* indicate interrupt handler not installed yet */
    
/*
    Initialize the Configuration Parameter Block to default values.
*/

    for( i = 0; i < 7; i++ )
    {
        c_block480.m_CounterConstant[i] = 0xffff;     /* constant registers */
        c_block480.m_Mode[i] = 0;                     /* the counter mode */
        c_block480.m_OutputPolarity[i] = FALSE;       /* output polarity */
        c_block480.m_InputPolarity[i] = FALSE;        /* input polarity */
        c_block480.m_Trigger[i] = 0;                  /* triggering internal/external */
        c_block480.m_CounterSize[i] = 0;              /* counter size */
        c_block480.m_ClockSource[i] = 0;              /* clock source */
        c_block480.m_WDLoad[i] = 0;                   /* watchdog timer internal/external loaded */
        c_block480.m_InterruptEnable[i] = FALSE;      /* interrupt enable */
        c_block480.m_Debounce[i] = DebounceOff;       /* Debounce disabled only 1 & 2 used */
    }
    c_block480.m_InterruptVector = VECTOR;            /* interrupt vector */
    c_block480.event_status = 0;                      /* clear event status */
    c_block480.counter_num = 1;
    c_block480.num_chan = 2;                          /* default to 2 channel board */
    c_block480.bCarrier = FALSE;		/* indicate no carrier initialized and set up yet */
    c_block480.bInitialized = FALSE;	/* indicate not ready to talk to IP */
    c_block480.slotLetter = SLOT_A;
    c_block480.nHandle = 0;		/* make handle to a closed carrier board */

    hdata.h_pid = getpid();		/* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block480;	/* put in address of c_block480 structure also */

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
    if(CarrierOpen(0, &c_block480.nHandle) != S_OK)
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
      printf("\n\nIP480 Library Demonstration  Rev. A\n\n");
      printf(" 1. Exit this Program\n");
      printf(" 2. Set Board Base Address\n");
      printf(" 3. Set IP Slot Letter\n");
      printf(" 4. Issue Software Reset to Board\n");
      printf(" 5. Read Module I.D.\n");
      printf(" 6. Examine/Change Current Counter\n");
      printf(" 7. Set Up Configuration Parameters\n");
      printf(" 8. Configure Control Register\n");
      printf(" 9. Enable Interrupts\n");
      printf("10. Disable Interrupts\n");
      printf("11. Software Trigger Control\n");
      printf("12. Display Counter Control Registers\n");
      printf("13. Display Read Back Registers\n");
      printf("14. Examine/Change Counter Constant\n");
      printf("15. Write Counter Constant Register\n");
      printf("16. Stop Counter\n");
      printf("17. Disable Interrupt\n");
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
		  SetCarrierAddress(c_block480.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block480.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
          SetCarrierAddress(c_block480.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block480.nHandle) == S_OK)/* Initialize Carrier */
		  {
		        c_block480.bCarrier = TRUE;
			SetInterruptLevel(c_block480.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;

				
	    }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
        break;

	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block480.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block480.bCarrier = FALSE;
			}
			else
			{
				c_block480.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block480.nHandle, c_block480.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block480.bInitialized = FALSE;
				}
				else	              
				{	
			        c_block480.brd_ptr = (byte *)addr;
					c_block480.bInitialized = TRUE;
				}
			}
		}
	break;

        case 4: /* Reset board */
            if(!c_block480.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	        output_word( c_block480.nHandle, (word *)(c_block480.brd_ptr + ResetRegister), Reset);
        break;

        case 5:     /* read board I.D. command */
            if(!c_block480.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
                readstat480(&c_block480); /* read board status */
        break;


        case 6: /* counter number */
            selectcnt480(&c_block480);
        break;


        case 7: /* set up configuration block parameters */
	    setconf480(&c_block480);
        break;


        case 8: /* configure */
            if(!c_block480.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
	        ConfigureCounterTimer480(&c_block480, c_block480.counter_num);
        /*      printf("\n\nControl Word: %03x\n",build_control480(&c_block480,c_block480.counter_num));*/
	    }
        break;

        case 9:     /* attach exception handler */
		EnableInterrupts( c_block480.nHandle );
		hflag = 1;
        break;
        
        case 10: /* detach exception handlers */
		hflag = 0;
		DisableInterrupt480(&c_block480,c_block480.counter_num);
		DisableInterrupts( c_block480.nHandle );
        break;

        case 11: /* Software trigger control */
            if(!c_block480.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	        trig480(&c_block480);
        break;

        case 12: /* Read Counter Control registers */
            if(!c_block480.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
               printf("\nCounter Control Registers = ");
               cont_reg = input_word( c_block480.nHandle, (word *)(c_block480.brd_ptr + CounterControl1));
               printf("%04X ",cont_reg);
               cont_reg = input_word( c_block480.nHandle, (word *)(c_block480.brd_ptr + CounterControl2));
               printf("%04X ",cont_reg);
               cont_reg = input_word( c_block480.nHandle, (word *)(c_block480.brd_ptr + CounterControl3));
               printf("%04X ",cont_reg);
               cont_reg = input_word( c_block480.nHandle, (word *)(c_block480.brd_ptr + CounterControl4));
               printf("%04X ",cont_reg);
               cont_reg = input_word( c_block480.nHandle, (word *)(c_block480.brd_ptr + CounterControl5));
               printf("%04X ",cont_reg);
               cont_reg = input_word( c_block480.nHandle, (word *)(c_block480.brd_ptr + CounterControl6));
               printf("%04X \n",cont_reg);
            }
        break;

        case 13: /* Display Read Back registers */
            if(!c_block480.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
              printf("\nRead Back Registers = ");
              for(i = 1; i < 7; i++)
              {
               if(c_block480.m_CounterSize[i] && (i == 1 || i == 3 || i == 5))
               {
               }
               else
               {
               ReadCounter480(&c_block480, i, &ulong_value);
               printf("%04lX ",ulong_value);
               }
              }
              printf("\n");
	    }
        break;
  
        case 14: /* select counter constant */
            selectcon480(&c_block480);
        break;

        case 15: /* Write counter constant */
            if(!c_block480.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	        WriteCounterConstant480(&c_block480, c_block480.counter_num);
        break;

        case 16: /* Disable the current channel */
            if(!c_block480.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	        StopCounter480(&c_block480, c_block480.counter_num);
        break;

        case 17: /* Disable the interrupt on the current channel */
            if(!c_block480.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	        DisableInterrupt480(&c_block480, c_block480.counter_num);
        break;

        }   /* end of switch */
    }	/* end of while */

/*
    Reset board to disable interrupts from all counters on this IP module
*/
    if(	c_block480.bInitialized == TRUE)	/* module address was set */
	{
       output_word( c_block480.nHandle, (word *)(c_block480.brd_ptr + ResetRegister), Reset);
       DisableInterrupt480(&c_block480,c_block480.counter_num);
       DisableInterrupts( c_block480.nHandle );
	}
    if(c_block480.bCarrier)
	CarrierClose(c_block480.nHandle);

    printf("\nEXIT PROGRAM\n");
}   /* end of main */




/*
{+D}
    SYSTEM:         IP480 Software

    FILENAME:       drvr480.c

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
    SYSTEM:         IP480 Software

    FILENAME:       ip480.c

    MODULE NAME:    setconf480 - set configuration block contents.

    VERSION:	    A

    CREATION DATE:  07/11/96

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:	    Routine which is used to enter parameters into
		    the Configuration Block.

    CALLING
	SEQUENCE:   setconf480(c_block480)
		    where:
			c_block480 (structure pointer)
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

void setconf480(c_blk)
struct cblk480 *c_blk;
{

/*
    DEFINITIONS:
*/


/*
    DECLARE LOCAL DATA AREAS:
*/
    int status;
    int item;		      /* menu item variable */
    int counter;              /* counter number */
    unsigned finished;        /* flag to exit loop */
    byte byte_value;          /* storage for retieved data */
    BOOL bool_value;          /* storage for retreived data */

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
        counter = c_blk->counter_num;
        printf("\n\n\n\n");
        printf("\n\nCurrent Counter %x Configuration Parameters\n\n",counter);
	printf(" 1. Return to Previous Menu\n");
        printf(" 2. Board Pointer:         %lX\n",(unsigned long)c_blk->brd_ptr);
        status = GetMode480(c_blk, counter, &byte_value);
        printf(" 3. Counter Mode:          %X\n",byte_value);
        status = GetOutputPolarity480(c_blk, counter, &bool_value);
        printf(" 4. Output Polarity:       %X\n",bool_value);
        status = GetInputPolarity480(c_blk, counter, &bool_value);
        printf(" 5. Input/Event Polarity:  %X\n",bool_value);
        status = GetTriggerSource480(c_blk, counter, &byte_value);
        printf(" 6. Trigger Source:        %X\n",byte_value);
        status = GetCounterSize480(c_blk, counter, &bool_value);
        printf(" 7. Counter Size:          %X\n",bool_value);
        status = GetClockSource480(c_blk, counter, &byte_value);
        printf(" 8. Clock Source:          %X\n",byte_value);
        status = GetWatchdogLoad480(c_blk, counter, &bool_value);
        printf(" 9. Watchdog Load:         %X\n",bool_value);
        status = GetInterruptVector480(c_blk, &byte_value);
        printf("10. Interrupt Vector:      %X\n",byte_value);
        status = GetInterruptEnable480(c_blk, counter, &bool_value);
        printf("11. Interrupt Enable:      %X\n",bool_value);
	if( counter == 1 || counter == 2 )	/* valid range ? */
	{
          status = GetDebounce480(c_blk, counter, &bool_value);
          printf("12. Debounce:              %X\n",bool_value);
	}

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
 
        case 3: /* mode */
            printf("0 - Disabled\n");
            printf("1 - Output Pulse\n");
            printf("2 - Output Squarewave\n");
            printf("3 - Watchdog Timer\n");
            printf("4 - Input Event Counter\n");
            printf("5 - Input Pulse Width Measurement\n");
            printf("6 - Input Period Measurement\n");
            printf("7 - One-shot Output Pulse\n\n");
            byte_value = (byte)get_param();
            SetMode480(c_blk, counter, byte_value);
	    break;

        case 4: /* output polarity */
            printf("0 - Output Polarity Low\n");
            printf("1 - Output Polarity High\n\n");
            bool_value = (BOOL)get_param();
            SetOutputPolarity480(c_blk, counter, bool_value);
	    break;

        case 5: /* input polarity */
            printf("0 - Input Polarity Low\n");
            printf("1 - Input Polarity High\n\n");
            bool_value = (BOOL)get_param();
            SetInputPolarity480(c_blk, counter, bool_value);
	    break;

        case 6: /* trigger source*/
            printf("0 - Internal Trigger\n");
            printf("1 - External Trigger, Polarity Low\n");
            printf("2 - External Trigger, Polarity High\n\n");
            byte_value = (byte)get_param();
            SetTriggerSource480(c_blk, counter, byte_value);
            break;

        case 7: /* counter size */
            printf("0 - 16-bit Counter\n");
            printf("1 - 32-bit Counter\n\n");
            bool_value = (BOOL)get_param();
            SetCounterSize480(c_blk, counter, bool_value);
	    break;

        case 8: /* clock source */
            printf("0 - Internal 1MHz Clock\n");
            printf("1 - Internal 4MHz Clock\n");
            printf("2 - Internal 8MHz Clock\n");
            printf("3 - External Clock\n\n");
            byte_value = (byte)get_param();
            SetClockSource480(c_blk, counter, byte_value);
	    break;

        case 9: /* watchdog int./ext load */
            printf("0 - Internal (Auto) Load of Counter\n");
            printf("1 - External Triggered Load of Counter\n\n");
            bool_value = (BOOL)get_param();
            SetWatchdogLoad480(c_blk, counter, bool_value);
	    break;

        case 10: /* interrupt vector */
            byte_value = (byte)get_param();
            SetInterruptVector480(c_blk, byte_value);
	    break;

        case 11: /* interrupt enable */
            printf("0 - Interrupts Disabled\n");
            printf("1 - Interrupts Enabled\n\n");
            bool_value = (BOOL)get_param();
            SetInterruptEnable480(c_blk, counter, bool_value);
            break;

        case 12: /* debounce */
	    if( counter < 1 || counter > 2 )	/* valid range ? */
		break;
            printf("1 - Debounce Enabled\n\n");
            bool_value = (BOOL)get_param();
            SetDebounce480(c_blk, counter, bool_value);
            break;
         }
    }
}



/*
{+D}
    SYSTEM:         IP480 Software

    FILENAME:       ip480.c

    MODULE NAME:    readstat480 - read board status by using rmid480().

    VERSION:	    A

    CREATION DATE:  11/07/95

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:	    Routine which is used to "Read Board Status" and to
		    print the results to the console.

    CALLING
        SEQUENCE:   readstat480(&c_block480)
		    where:
                        c_block480 (structure pointer)
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

void readstat480(c_blk)
struct cblk480 *c_blk;
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
    int item;		/* menu item variable */
    int i;		/* loop index */
    unsigned finished;  /* flags to exit loops */

/*
    ENTRY POINT OF ROUTINE:
*/

    rmid480(c_blk);            /* read the modules id prom */
    if(c_blk->id_prom[5] == 0x16)
     c_blk->num_chan = 6;
    else
     c_blk->num_chan = 2;

    finished = 0;
    while(!finished)
    {
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
        printf("2 Read Again\n");
	printf("\nselect: ");
	scanf("%d",&item);

	switch(item){

	case 1: /* return to previous menu */
	    finished++;
	    break;

	}
    }
}


/*
{+D}
    SYSTEM:         IP480 Software

    FILENAME:       ip480.c

    MODULE NAME:    trig480 - Software trigger for IP480.

    VERSION:	    A

    CREATION DATE:  07/11/96

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:       Routine triggers counter/timers on the IP480.

    CALLING
        SEQUENCE:   trig480(c_block480)
		    where:
			c_block480 (structure pointer)
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

void trig480(c_blk)
struct cblk480 *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item, trig;             /* menu item variable */
    unsigned finished;          /* flag to exit loop */

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
        printf("\n\nSoftware Triggering Options\n\n");
        printf(" 1. Return to Previous Menu\n");
        printf(" 2. Trigger Single Counter/Timer\n");
        printf(" 3. Trigger Multiple Counter/Timers\n");
        printf(" 4. Trigger Single Counter/Timer & block\n");
        printf("\nselect: ");
        rewind(stdin);
        scanf("%d",&item);
	switch(item)
	{
	case 1: /* return to previous menu */
	    finished++;
	    break;

        case 2: /* Single trigger */
            printf("Enter Counter Number to Trigger (1 - 6): ");
            scanf("%d",&trig);
            StartCounter480(c_blk, trig);
	    break;

        case 3: /* Multiple trigger */
            printf("Enter Hex Mask for Multiple Trigger: ");
            scanf("%x",&trig);
            StartSimultaneousCounters480(c_blk, (byte)trig);
	    break;
				
        case 4: /* Blocking Single trigger */
            printf("Enter Counter Number to Trigger (1 - 6): ");
            scanf("%d",&trig);
			printf("Blocking Start Counter wait...");
			fflush(stdout);
			BlockingStartCounter(c_blk, trig);
			printf(" done!\n");
         }
    }
}


/*
{+D}
    SYSTEM:         IP480 Software

    FILENAME:       ip480.c

    MODULE NAME:    selectcnt480 - Select counter for IP480.

    VERSION:	    A

    CREATION DATE:  07/11/96

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:       Routine selects counter/timers on the IP480.

    CALLING
        SEQUENCE:   selectcnt480(c_block480)
		    where:
			c_block480 (structure pointer)
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

void selectcnt480(c_blk)
struct cblk480 *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item, cntr;             /* menu item variable */
    unsigned finished;          /* flag to exit loop */

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
        printf("\n\nCurrent Counter: %x\n\n",c_blk->counter_num);
	printf(" 1. Return to Previous Menu\n");
        printf(" 2. Change Counter\n");
        printf("\nselect: ");
        scanf("%d",&item);
	switch(item)
	{
	case 1: /* return to previous menu */
	    finished++;
	    break;

        case 2: /* Select counter */
            printf("Enter New Counter Number (1 - 6): ");
            scanf("%d",&cntr);
            c_blk->counter_num = (byte)(cntr);
	    break;
         }
    }
}


/*
{+D}
    SYSTEM:         IP480 Software

    FILENAME:       ip480.c

    MODULE NAME:    selectcon480 - Select counter constant for IP480.

    VERSION:	    A

    CREATION DATE:  07/11/96

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:       Routine selects counter constants for the IP480.

    CALLING
        SEQUENCE:   selectcon480(c_block480)
		    where:
			c_block480 (structure pointer)
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

void selectcon480(c_blk)
struct cblk480 *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;                   /* menu item variable */
    int counter;                /* counter number stroage */
    unsigned finished;          /* flag to exit loop */
    ULONG value;

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
      counter = c_blk->counter_num;
      printf("\n\nCurrent Counter:          %X\n",counter);
      GetCounterConstant480(c_blk, counter, &value);
      printf("Current Contents of Counter Constant Register: %lX\n\n", value);

      printf(" 1. Return to Previous Menu\n");
      printf(" 2. Change Constant\n");
      printf("\nselect: ");

      scanf("%d",&item);
      switch(item)
      {
      case 1: /* return to previous menu */
          finished++;
      break;

      case 2: /* Select constant */
      value = (ULONG)get_param();
      SetCounterConstant480(c_blk, counter, value);
      break;
      }
    }
}


