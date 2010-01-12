
#include "../carrier/apc8620.h"
#include "ip482.h"

#define INTERRUPT_LEVEL 5	/* level or bus IRQ - may not be used on all systems */
#define VECTOR 192		/* interrupt vector - may not be used on all systems */

/*
{+D}
    SYSTEM:         ip482/3/4 Software

    FILENAME:       drvrct.c

    MODULE NAME:    main - main routine of example software.

    VERSION:	    REV B

    CREATION DATE:  10/10/03

    DESIGNED BY:    F.M.

    CODED BY:	    F.M.

    ABSTRACT:	    This module is the main routine for the example program
                    which demonstrates how the Library is used.

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
    which demonstrates how the ip482 Library is used.
*/

int main()

{


/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[32];		/* command line input buffer */
    unsigned finished;		/* flag to exit program */
    int item;			/* menu item selection variable */
    int hstatus;		/* interrupt handler returned status */
    long addr;			/* integer to hold board address */
    int flag;			/* general flag for exiting loops */
    int i;			/* loop index */
    int hflag;			/* interrupt handler installed flag */
    word word_value;
    struct handler_data hdata;  /* interrupt data structure (see exception.h) */
    struct ip482 c_block482;	/* configuration block */

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
    c_block482.bCarrier = FALSE;	/* indicate no ip initialized and set up yet */
    c_block482.bInitialized = FALSE; /* indicate not ready to talk to ip */
    c_block482.nHandle = 0;	/* make handle to a closed ip board */
    c_block482.brd_ptr = 0;	/* NULL out address */

    for( i = 0; i < 10; i++ )
    {
      c_block482.m_CounterConstantA[i] = 0xFFFF;         /* constant registers */
      c_block482.m_CounterConstantB[i] = 0xFFFF;         /* constant registers */
      c_block482.m_Mode[i] = None;                       /* the counter mode */
      c_block482.m_OutputPolarity[i] = OutPolLow;        /* output polarity */
      c_block482.m_InputAPolarity[i] = InABCPolDisabled; /* input A polarity */
      c_block482.m_InputBPolarity[i] = InABCPolDisabled; /* input B polarity */
      c_block482.m_InputCPolarity[i] = InABCPolDisabled; /* input C polarity */
      c_block482.m_ClockSource[i] = InC0_5Mhz;           /* clock source */
      c_block482.m_InterruptEnable[i] = IntDisable;      /* interrupt enable */
      c_block482.m_Debounce[i] = DebounceOff;            /* Debounce disabled only 1 & 2 used */
    }
    c_block482.event_status = 0;                    /* clear event status */
    c_block482.counter_num = 1;
    c_block482.m_IpClock = 0;                  /* 8MHZ ip clock from carrier */

    hdata.h_pid = getpid();		/* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block482;	/* put in address of c_block482 structure also */

    SetInterruptVector(&c_block482, VECTOR);           /* interrupt vector */

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
    if(CarrierOpen(0, &c_block482.nHandle) != S_OK)
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
      printf("\nIp482/3/4 Counter/Timer Library Demonstration Rev. B\n\n");
      printf(" 1. Exit this Program\n");
      printf(" 2. Set Board Base Address\n");
      printf(" 3. Set IP Slot Letter\n");
      printf(" 4. Examine/Change Current Counter\n");
      printf(" 5. Set Up Configuration Parameters\n");
      printf(" 6. Configure Control Register\n");
      printf(" 7. Enable Interrupts\n");
      printf(" 8. Disable Interrupts\n");
      printf(" 9. Software Trigger Control\n");
      printf("10. Display Counter Control Registers\n");
      printf("11. Display Read Back Registers\n");
      printf("12. Examine/Change Counter Constant\n");
      printf("13. Write Counter Constant Register\n");
      printf("14. Stop Counter\n");
      printf("15. Disable Current Counter Interrupt\n");
      printf("16. Read Module I.D.\n");
      printf("17. Digital I/O\n");
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
		  SetCarrierAddress(c_block482.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block482.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
          SetCarrierAddress(c_block482.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block482.nHandle) == S_OK)/* Initialize Carrier */
		  {
		    c_block482.bCarrier = TRUE;
			SetInterruptLevel(c_block482.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;

	    }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
        break;

		case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block482.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block482.bCarrier = FALSE;
			}
			else
			{
				c_block482.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block482.nHandle, c_block482.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block482.bInitialized = FALSE;
				}
				else	              
				{	
			                c_block482.brd_ptr = (struct map482 *)addr;
					c_block482.bInitialized = TRUE;
				}
			}
		}
		break;

		case 4: /* counter number */
            selectcnt(&c_block482);
        break;

        case 5: /* set up configuration block parameters */
	    setconf(&c_block482);
        break;

        case 6: /* configure */
            if(!c_block482.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
	        ConfigureCounterTimer(&c_block482, c_block482.counter_num);
            /* printf("\n\nControl Word: %04x\n",build_control(&c_block482,c_block482.counter_num)); */
	    }
        break;

        case 7: /* attach exception handler */
		EnableInterrupts( c_block482.nHandle );
		hflag = 1;
        break;

        case 8: /* detach exception handler */
		hflag = 0;
		DisableInterrupts(c_block482.nHandle);
        break;

        case 9: /* Software trigger control */
            if(!c_block482.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	        trig(&c_block482);
        break;

        case 10: /* Read Counter Control registers */
            if(!c_block482.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
	      printf("\nCounter Control ");
	      for( i = 1; i < 11; i++ )
	      {
		   ReadCounterControl(&c_block482, i, &word_value );
		   printf("%02d = %04X  ",i,word_value);
		   if( i == 5 )
			printf("\nCounter Control ");
	      }
              printf("\n");
        }
        break;

        case 11: /* Display Read Back registers */
            if(!c_block482.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
              printf("\nRead Back Register ");
              for(i = 1; i < 11; i++)
              {
                ReadCounter(&c_block482, i, &word_value);
		printf("%02d = %04X  ",i,word_value);
                if( i == 5 )
	          printf("\nRead Back Register ");

              }
              printf("\n");
	    }
        break;
  
        case 12: /* select counter constant */
            selectcon(&c_block482);
        break;

        case 13: /* Write counter constant */
            if(!c_block482.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	        WriteCounterConstant(&c_block482, c_block482.counter_num);
        break;

        case 14: /* Disable the current channel */
            if(!c_block482.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	        stop_trig(&c_block482);
        break;

        case 15: /* Disable the interrupt on the current channel */
            if(!c_block482.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	        DisableInterrupt(&c_block482, c_block482.counter_num);
        break;

        case 16:     /* read status command */
            if(!c_block482.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
                readstat(&c_block482); /* read board status */
        break;

        case 17:/* Digital I/O */
            if(!c_block482.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
		dig_io(&c_block482);
       break;

        }   /* end of switch */
    }	/* end of while */

    if(	c_block482.bInitialized == TRUE)	/* module address was set */
		StopSimultaneousCounters(&c_block482, 0xffff );	/* stop all */

    DisableInterrupts(c_block482.nHandle);
    if(c_block482.bCarrier)
	CarrierClose(c_block482.nHandle);

    printf("\nEXIT PROGRAM\n");
}   /* end of main */




/*
{+D}
    SYSTEM:         ip482 Software

    FILENAME:       drvr482.c

    MODULE NAME:    get_param - get a parameter from the console

    VERSION:	    V1.0

    CREATION DATE:  10/10/03

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
    SYSTEM:         ip482 Software

    FILENAME:       ip482.c

    MODULE NAME:    setconf - set configuration block contents.

    VERSION:	    A

    CREATION DATE:  10/10/03

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:	    Routine which is used to enter parameters into
		    the Configuration Block.

    CALLING
	SEQUENCE:   setconf(c_block482)
		    where:
			c_block482 (structure pointer)
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

void setconf(c_blk)
struct ip482 *c_blk;
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
    word speed;

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
        counter = c_blk->counter_num;
        printf("\n\n\n\n");
        printf("\n\nCurrent Counter %d Configuration Parameters\n\n",counter);
		printf(" 1. Return to Previous Menu\n");
        printf(" 2. Board Pointer:         %lX\n",(unsigned long)c_blk->brd_ptr);
        status = GetMode(c_blk, counter, &byte_value);
        printf(" 3. Counter Mode:          %X\n",byte_value);
        status = GetOutputPolarity(c_blk, counter, &bool_value);
        printf(" 4. Output Polarity:       %X\n",bool_value);
        status = GetInputAPolarity(c_blk, counter, &byte_value);
        printf(" 5. Input A Polarity:      %X\n",byte_value);
        status = GetInputBPolarity(c_blk, counter, &byte_value);
        printf(" 6. Input B Polarity:      %X\n",byte_value);
        status = GetInputCPolarity(c_blk, counter, &byte_value);
        printf(" 7. Input C Polarity:      %X\n",byte_value);
        status = GetClockSource(c_blk, counter, &byte_value);
        printf(" 8. Clock Source/Special:  %X\n",byte_value);
        status = GetDebounce(c_blk, counter, &bool_value);
        printf(" 9. Debounce:              %X\n",bool_value);
        status = GetInterruptVector(c_blk, &byte_value);
        printf("10. Interrupt Vector:      %X\n",byte_value);
        status = GetInterruptEnable(c_blk, counter, &bool_value);
        printf("11. Interrupt Enable:      %X\n",bool_value);
        status = GetIpClock(c_blk, &bool_value);
        printf("12. Carrier Clock Speed:   %X\n",bool_value);

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
            printf("1 - Quadrature Position\n");
            printf("2 - Pulse Width Modulation\n");
            printf("3 - Watchdog Timer\n");
            printf("4 - Event Counter/Frequency Measurement\n");
            printf("5 - Input Pulse Width Measurement\n");
            printf("6 - Input Period Measurement\n");
            printf("7 - One-shot Output Pulse\n\n");

            byte_value = (byte)get_param();
            SetMode(c_blk, counter, byte_value);
	    break;

        case 4: /* output polarity */
            printf("0 - Output Polarity Low\n");
            printf("1 - Output Polarity High\n\n");
            bool_value = (BOOL)get_param();
            SetOutputPolarity(c_blk, counter, bool_value);
	    break;

        case 5: /* input A polarity */
            printf("0 - In A Polarity Disabled\n");
	    GetMode(c_blk, counter, &byte_value);
	    if( byte_value == QuadPosition )
	    {
	        printf("1 - X1 Encoding\n");
	        printf("2 - X2 Encoding\n");
	        printf("3 - X4 Encoding\n");
	    }
	    else
	    {
	        printf("1 - In A Polarity Active Low\n");
	        printf("2 - In A Polarity Active High\n\n");
	    }
	    byte_value = (byte)get_param();
	    SetInputAPolarity(c_blk, counter, byte_value);
	    break;

        case 6: /* input B polarity */
            printf("0 - In B Polarity Disabled\n");
	    GetMode(c_blk, counter, &byte_value);
	    if( byte_value == QuadPosition )
	       printf("1 - In B Polarity Enabled\n");
	    else
	    {
	      printf("1 - In B Polarity Active Low\n");
              printf("2 - In B Polarity Active High\n\n");
            }
            byte_value = (byte)get_param();
            SetInputBPolarity(c_blk, counter, byte_value);
            break;

        case 7: /* input C polarity */
            printf("0 - In C Polarity Disabled\n");
	    GetMode(c_blk, counter, &byte_value);
	    if( byte_value == QuadPosition )
	    {
	        printf("1 - A = 0, B = 1\n");
	        printf("2 - A = 1, B = 0\n");
	        printf("3 - A = 1, B = 1\n");
	        printf("4 - A = 0, B = 0\n");
	    }
	    else
	    {
	        printf("1 - In C Polarity Active Low\n");
	        printf("2 - In C Polarity Active High\n");
		if( byte_value == Watchdog )
	            printf("3 - In C Polarity Gate-Off Control\n");
		if( byte_value == EventCounting )
	            printf("3 - In C Polarity Up/Down Control\n");
	    }
	    printf("\n");
            byte_value = (byte)get_param();
            SetInputCPolarity(c_blk, counter, byte_value);
            break;

        case 8: /* clock source/ special function */
	    GetMode(c_blk, counter, &byte_value);
	    switch( byte_value )
	    {
		case EventCounting:
	            printf("0 - Event Counting\n");
	            printf("7 - Frequency Measurement\n\n");
		break;
		case QuadPosition:
	            printf("0 - No Interrupt\n");
	            printf("1 - Interrupt when counter equals Counter Constant A\n");
	            printf("2 - Interrupt on Index and reload on Index\n");
	            printf("3 - Interrupt on Index (no reload)\n\n");
		break;
		default:
	            printf("0 - Internal 1.25MHz Clock\n");
	            printf("1 - Internal 2.5MHz Clock\n");
	            printf("2 - Internal 5MHz Clock\n");
	            printf("3 - Internal 10MHz Clock\n");
	            printf("4 - Internal 20MHz Clock\n");
	            printf("5 - External Clock\n\n");
		break;
	    }
            byte_value = (byte)get_param();
            SetClockSource(c_blk, counter, byte_value);
	    break;

        case 9: /* debounce */
            printf("0 - Debounce Disabled\n");
            printf("1 - Debounce Enabled\n\n");
            bool_value = (BOOL)get_param();
            SetDebounce(c_blk, counter, bool_value);
            break;

        case 10: /* interrupt vector */
            byte_value = (byte)get_param();
            SetInterruptVector(c_blk, byte_value);
	    break;

        case 11: /* interrupt enable */
            printf("0 - Interrupts Disabled\n");
            printf("1 - Interrupts Enabled\n\n");
            bool_value = (BOOL)get_param();
            SetInterruptEnable(c_blk, counter, bool_value);
            break;

        case 12: /* Ip clock select */
            printf("0 - 8MHZ IP Clock From Carrier\n");
            printf("1 - 32MHZ IP Clock From Carrier\n\n");
            bool_value = (BOOL)get_param();
            /* call carrier API to get slot clock speed info */
            status = GetIPClockControl(c_blk->nHandle, c_blk->slotLetter, &speed);
            if( status )
               	printf("\nSorry... Carrier can not change the IP clock setting\n");
            else
			{
               /* call carrier API to set slot clock speed */
               SetIPClockControl(c_blk->nHandle, c_blk->slotLetter, bool_value);

               /* call IPmodule API to select clock speed */
               SetIpClock(c_blk, bool_value);
			}
            break;

         }
    }
}





/*
{+D}
    SYSTEM:         ip482 Software

    FILENAME:       ip482.c

    MODULE NAME:    trig - Software trigger for ip482.

    VERSION:	    A

    CREATION DATE:  10/10/03

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:       Routine triggers counter/timers on the ip482.

    CALLING
        SEQUENCE:   trig(c_block482)
		    where:
			c_block482 (structure pointer)
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

void trig(c_blk)
struct ip482 *c_blk;
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
        scanf("%d",&item);
	switch(item)
	{
	case 1: /* return to previous menu */
	    finished++;
	    break;

        case 2: /* Single trigger */
            printf("Enter Counter Number to Trigger (1 - 10): ");
            scanf("%d",&trig);
            StartCounter(c_blk, trig);
	    break;

        case 3: /* Multiple trigger */
            printf("Enter Hex Mask for Multiple Trigger: ");
            scanf("%x",&trig);
            StartSimultaneousCounters(c_blk, (word)trig);
	    break;
		
        case 4: /* Blocking Single trigger */
            printf("Enter Counter Number to Trigger (1 - 10): ");
            scanf("%d",&trig);
			printf("Blocking Start Counter wait...");
			fflush(stdout);
			BlockingStartCounter(c_blk, trig);
			printf(" done!\n");
		break;
         }
    }
}



/*
{+D}
    SYSTEM:         ip482 Software

    FILENAME:       ip482.c

    MODULE NAME:    stop_trig - Software stop trigger for ip482.

    VERSION:	    A

    CREATION DATE:  10/10/03

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:       Routine stop triggers counter/timers on the ip482.

    CALLING
        SEQUENCE:   stop_trig(c_block482)
		    where:
			c_block482 (structure pointer)
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

void stop_trig(c_blk)
struct ip482 *c_blk;
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
        printf("\n\nStop Counter/Timer Options\n\n");
	printf(" 1. Return to Previous Menu\n");
        printf(" 2. Stop Single Counter/Timer\n");
        printf(" 3. Stop Multiple Counter/Timers\n");
        printf("\nselect: ");
        scanf("%d",&item);
	switch(item)
	{
	case 1: /* return to previous menu */
	    finished++;
	    break;

        case 2: /* Single */
            printf("Enter Counter Number to Stop (1 - 10): ");
            scanf("%d",&trig);
            StopCounter(c_blk, trig);
	    break;

        case 3: /* Multiple */
            printf("Enter Hex Mask for Multiple Stop: ");
            scanf("%x",&trig);
            StopSimultaneousCounters(c_blk, (word)trig);
	    break;
         }
    }
}



/*
{+D}
    SYSTEM:         ip482 Software

    FILENAME:       ip482.c

    MODULE NAME:    selectcnt - Select counter for ip482.

    VERSION:	    A

    CREATION DATE:  10/10/03

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:       Routine selects counter/timers on the ip482.

    CALLING
        SEQUENCE:   selectcnt(c_block482)
		    where:
			c_block482 (structure pointer)
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

void selectcnt(c_blk)
struct ip482 *c_blk;
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
        printf("\n\nCurrent Counter: %d\n\n",c_blk->counter_num);
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
selcounter:
            printf("Enter New Counter Number (1 - 10): ");
            scanf("%d",&cntr);
	    if( cntr > 10 )
	    {
	      printf("\n");
	      goto selcounter;
	    }
            c_blk->counter_num = (byte)(cntr);
	    break;
         }
    }
}


/*
{+D}
    SYSTEM:         ip482 Software

    FILENAME:       ip482.c

    MODULE NAME:    selectcon - Select counter constant for ip482.

    VERSION:	    A

    CREATION DATE:  10/10/03

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:       Routine selects counter constants for the ip482.

    CALLING
        SEQUENCE:   selectcon(c_block482)
		    where:
			c_block482 (structure pointer)
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

void selectcon(c_blk)
struct ip482 *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;                   /* menu item variable */
    int counter;                /* counter number stroage */
    unsigned finished;          /* flag to exit loop */
    word value;

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
      counter = c_blk->counter_num;
      printf("\n\nCurrent Counter:          %d\n",counter);
      GetCounterConstantA(c_blk, counter, &value);
      printf("Counter ConstantA Register: %X\n\n", value);

      GetCounterConstantB(c_blk, counter, &value);
      printf("Counter ConstantB Register: %X\n\n", value);

      printf(" 1. Return to Previous Menu\n");
      printf(" 2. Change Counter ConstantA\n");
      printf(" 3. Change Counter ConstantB\n");
      printf("\nselect: ");

      scanf("%d",&item);
      switch(item)
      {
      case 1: /* return to previous menu */
          finished++;
      break;

      case 2: /* Select constant A */
	      value = (word)get_param();
	      SetCounterConstantA(c_blk, counter, value);
      break;
      case 3: /* Select constant B */
	      value = (word)get_param();
	      SetCounterConstantB(c_blk, counter, value);
      break;
      }
    }
}


/*
{+D}
    SYSTEM:         IP482 Software

    FILENAME:       ip482.c

    MODULE NAME:    readstat - read board status by using rmid482().

    VERSION:	    A

    CREATION DATE:  10/10/03

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:	    Routine which is used to "Read Board Status" and to
		    print the results to the console.

    CALLING
        SEQUENCE:   readstat(&c_block482)
		    where:
                        c_block482 (structure pointer)
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

void readstat(c_blk)
struct ip482 *c_blk;
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

    rmid482(c_blk);            /* read the modules id prom */
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
        printf("\nCounter/Timer Event Status:  %X",c_blk->event_status);
        printf("\n\n1 Return to Previous Menu\n");
        printf("2 Read Again\n");
        printf("\nselect: ");
        scanf("%d",&item);
        switch(item)
        {
	  case 1: /* return to previous menu */
	    finished++;
	  break;
        }
    }
}


/*
{+D}
    SYSTEM:         IP482 Software

    FILENAME:       ip482.c

    MODULE NAME:    dig_io - read/write digital I/O.

    VERSION:	    A

    CREATION DATE:  10/10/03

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:	    Read/write digital I/O.

    CALLING
        SEQUENCE:   dig_io(&c_block482)
		    where:
                        c_block482 (structure pointer)
                          The address of the param. block

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

void dig_io(c_blk)
struct ip482 *c_blk;
{


/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;		/* menu item variable */
    unsigned finished;  /* flags to exit loops */
    unsigned point;      /* I/O point number */
    unsigned port;       /* I/O port number */
    unsigned val;        /* value of port or point */
    int status;          /* returned status of routines */

/*
    ENTRY POINT OF ROUTINE:
*/

    finished = 0;
    while(!finished)
    {
        printf("\nDigital I/O\n");
	printf("\n\n1. Return to Previous Menu\n");
        printf("2. Read Point\n");
        printf("3. Read Port\n");
        printf("4. Write Output Point\n");
        printf("5. Write Output Port\n");
        printf("\nselect: ");
        scanf("%d",&item);
        switch(item)
        {
	      case 1: /* return to previous menu */
	           finished++;
	      break;
	
	      case 2: /* read point */
            if(!c_blk->bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
			{
                printf("\nEnter port 0=Input, 1=Output   (0 - 1): ");
                scanf("%d",&port);
                printf("\nEnter Input point number (0 - 8): ");
                scanf("%d",&point);
                status = rpnt482(c_blk,port,point);
                if(status == -1)
                    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
                else
                    printf("\nValue of point %d: %X\n",point,status);
            }
	      break;
	      case 3: /* read port */
            if(!c_blk->bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
			{
                printf("\nEnter port 0=Input, 1=Output   (0 - 1): ");
                scanf("%d",&port);
                status = rprt482(c_blk,port);
                if(status == -1)
                    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
                else
                    printf("\nValue: %X\n",status);
            }
	      break;

          case 4: /* Write Point */
            if(!c_blk->bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
		    {
                printf("\nEnter I/O point number  (0 - 8): ");
                scanf("%d",&point);
                printf("\nEnter point value (0 - 1): ");
                scanf("%x",&val);
                status = wpnt482(c_blk,point,val);
                if(status == -1)
                    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
            }
          break;

          case 5: /* Write Port */
            if(!c_blk->bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
		    {
                printf("\nEnter output value in hex (0 - FF): ");
                scanf("%x",&val);
                status = wprt482(c_blk,val);
                if(status == -1)
                    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
            }
          break;
		}
    }
}
