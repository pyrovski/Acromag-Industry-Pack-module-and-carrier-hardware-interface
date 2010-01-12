
#include "../carrier/apc8620.h"
#include "ip470.h"


#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */


/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr470.c

    MODULE NAME:    main - main routine of example software.

    VERSION:        B

    CREATION DATE:  10/16/95

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       This module is the main routine for the example program
		    which demonstrates how the IP470 Library is used.

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
01/11/07 FJM   Add support x86_64

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

	This module is the main routine for the example program
	which demonstrates how the IP470 Library is used.
*/




int main()
{


/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[40];  /* command line input buffer */
    int item;           /* menu item selection variable */
    long status;        /* returned status of driver routines */
    int hstatus;        /* interrupt handler returned status */
    unsigned finished;  /* flag to exit program */
    long addr;          /* long to hold board address */
    int flag;           /* general flag for exiting loops */
    int i;              /* loop index */
    int point;          /* I/O point number */
    int port;           /* I/O port number */
    int val;            /* value of port or point */
    int hflag;          /* interrupt handler installed flag */
    struct cblk470 c_block470; /* configuration block */
    struct handler_data hdata;  /* interrupt handler data structure */

    
/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/

    flag = 0;         /* indicate board address not yet assigned */
    finished = 0;     /* indicate not finished with program */
    hflag = 0;        /* indicate interrupt handler not installed yet */

/*
    Initialize the Configuration Parameter Block to default values.
*/

    c_block470.param = 0;          /* parameter mask */
    c_block470.e_mode = 0;         /* e_mode flag */
    c_block470.mask_reg = 0;       /* output mask register */
    c_block470.deb_control = 0;    /* debounce control register */
    c_block470.deb_clock = 1;      /* debounce control register */
    c_block470.enable = 0;         /* interrupt enable register */
    c_block470.vector = VECTOR;    /* interrupt vector */

    c_block470.bCarrier = FALSE;	/* indicate no carrier initialized and set up yet */
    c_block470.bInitialized = FALSE;/* indicate not ready to talk to IP */
    c_block470.slotLetter = SLOT_A;
    c_block470.nHandle = 0;	/* make handle to a closed carrier board */
    
    for(i = 0; i != 2; i++)
    {
      c_block470.ev_control[i] = 0;/* event control registers */
      c_block470.deb_duration[i] = 0;/* debounce duration registers */
    }

    hdata.h_pid = getpid();		/* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block470;	/* put in address of c_block470 structure also */

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
    if(CarrierOpen(0, &c_block470.nHandle) != S_OK)
    {
	printf("\nUnable to Open instance of carrier.\n");
	finished = 1;	 /* indicate finished with program */
    }
    else
	flag = 1;


/*
    Enter main loop
*/      

    while(!finished) {

	printf("\n\nIP470 Library Demonstration Version A\n\n");
	printf(" 1. Exit this Program\n");
	printf(" 2. Set IP Module Base Address\n");
	printf(" 3. Set IP Slot Letter\n");
	printf(" 4. Set Up Configuration Parameters\n");
	printf(" 5. Configure Board Command\n");
	printf(" 6. Read Status Command and ID\n");
	printf(" 7. Enable Interrupts\n");
	printf(" 8. Disable Interrupts\n");
	printf(" 9. Read Input Point\n");
	printf("10. Read Input Port\n");
	printf("11. Write Output Point\n");
	printf("12. Write Output Port\n");

	printf("\nSelect: ");
	scanf("%d",&item);

/*
    perform the menu item selected.
*/  
	switch(item) {

	case 1: /* exit program command */

	    printf("Exit program(y/n)?: ");
	    scanf("%s",cmd_buff);
	    if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		finished++;
	    break;
	
	case 2: /* set IP address command */
	    do 
	    {
		if(flag == 0)
		{
		  printf("\n\nenter base address of carrier board in hex: ");
		  scanf("%lx",&addr);
		  /* Set Carrier Address for Open Carrier Device */
		  SetCarrierAddress(c_block470.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block470.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
          SetCarrierAddress(c_block470.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block470.nHandle) == S_OK)/* Initialize Carrier */
		  {
		        c_block470.bCarrier = TRUE;
			SetInterruptLevel(c_block470.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;

				
	    }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
	    break;

	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block470.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block470.bCarrier = FALSE;
			}
			else
			{
				c_block470.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block470.nHandle, c_block470.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block470.bInitialized = FALSE;
				}
				else	              
				{	
			        c_block470.brd_ptr = (struct map470 *)addr;
					c_block470.bInitialized = TRUE;
				}
			}
		}
	break;

	case 4: /* set up configuration parameters */

	    scfg470(&c_block470);
	    break;

	case 5: /* configure board command */
	
            if(!c_block470.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
/*
    Check the "interrupt handler attached" flag. If interrupt handlers
    are not attached, then print an error message.
*/

              if( (c_block470.enable == 1 || c_block470.enable == 3 ) && hflag == 0 )
		   printf(">>> ERROR: INTERRUPT HANDLER NOT ATTACHED <<<\n");
	       else  /* select enhanced mode & configure */
		   cnfg470(&c_block470); /* configure the board */
	    }
	    break;

	case 6:     /* read board status command */
	
            if(!c_block470.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
		psts470(&c_block470); /* read board status */
	    break;

    case 7:     /* enable interrupts */
		EnableInterrupts( c_block470.nHandle );
		hflag = 1;
    break;

	case 8: /* detach exception handlers */
		hflag = 0;
		DisableInterrupts(c_block470.nHandle);
	 break;

	 case 9: /* Read Digital Input Point */

        if(!c_block470.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Input port number   (0 - 5): ");
		scanf("%d",&port);
		printf("\nEnter Input point number  (0 - 7): ");
		scanf("%d",&point);
		status = rpnt470(&c_block470,port,point);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
		else
		    printf("\nValue of port %d point %d: %lx\n",port,point,status);
	    }
	    break;

	case 10: /* Read Digital Input Port */

            if(!c_block470.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Input port number  (0 - 5): ");
		scanf("%d",&port);
		status = rprt470(&c_block470,port);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
		else
		    printf("\nValue of port %d: %lx\n",port,status);
	    }
	    break;

	case 11: /* Write Digital Point */

            if(!c_block470.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Output port number (0 - 5): ");
		scanf("%d",&port);
		printf("\nEnter I/O point number   (0 - 7): ");
		scanf("%d",&point);
		printf("\nEnter point value (0 - 1): ");
		scanf("%x",&val);
		status = wpnt470(&c_block470,port,point,val);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
	    }
	    break;

	case 12: /* Write Digital Port */

            if(!c_block470.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Output port number  (0 - 5):  ");
		scanf("%d",&port);
		printf("\nEnter output value in hex (0 - FF): ");
		scanf("%x",&val);
		status = wprt470(&c_block470,port,val);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
	    }
	    break;
	}   /* end of switch */
    }   /* end of while */

/*
    disable interrupts from IP module
*/
    if(	c_block470.bInitialized == TRUE)	/* module address was set */
    {
      c_block470.param = RESET_INTEN; /* parameter mask */
      c_block470.enable = 0;
      c_block470.enable |= RESET;
      cnfg470(&c_block470);          /* configure the board */
    }

    DisableInterrupts(c_block470.nHandle);
    if(c_block470.bCarrier)
	CarrierClose(c_block470.nHandle);

    printf("\nEXIT PROGRAM\n");
}   /* end of main */



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr470.c

    MODULE NAME:    get_param - get a parameter from the console

    VERSION:        A

    CREATION DATE:  10/16/95

    DESIGNED BY:    RTL

    CODED BY:       RTL

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

    printf("enter hex parameter: ");
    scanf("%lx",&temp);
    printf("\n");
    return(temp);
}


/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr470.c

    MODULE NAME:    scfg470 - set configuration block contents.

    VERSION:        A

    CREATION DATE:  10/16/95

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to enter parameters into
		    the Configuration Block.
 
    CALLING
	SEQUENCE:   scfg470(c_block470)
		    where:
			c_block470 (structure pointer)
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

void scfg470(c_blk)

struct cblk470 *c_blk;
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
    int item;                       /* menu item variable */
    unsigned finished,finished2;    /* flags to exit loops */
    int i;                          /* loop index */

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
        printf("\n\nConfiguration Parameters\n\n");
	printf(" 1. Return to Previous Menu\n");
	printf(" 2. Board Pointer:    %04lX\n",(unsigned long)c_blk->brd_ptr);
        printf(" 3. Parameter Mask:     %02X\n",c_blk->param);
        printf(" 4. Enhanced Mode Flag: %02X\n",c_blk->e_mode);
        printf(" 5. Write Mask:         %02X",c_blk->mask_reg);
        printf("\n 6. Event Control:      ");
	for(i = 0; i != 2; i++)
	   printf("%02X  ",c_blk->ev_control[i]); /* event control registers */
        printf("\n 7. Debounce Clock:     %02X",c_blk->deb_clock);
        printf("\n 8. Debounce Control:   %02X",c_blk->deb_control);
        printf("\n 9. Debounce Duration:  ");
	for(i = 0; i != 2; i++)
	   printf("%02X  ",c_blk->deb_duration[i]); /* debounce duration registers */
        printf("\n10. Reset/Int. Enable:  %02X",c_blk->enable);
        printf("\n11. Interrupt Vector:   %02X\n",c_blk->vector);
    
	printf("\nSelect: ");
	scanf("%d",&item);

	switch(item)
	{
	
	case 1: /* return to previous menu */
	    finished++;
	    break;

	case 2: /* board address */
	    printf("BOARD ADDRESS CAN BE CHANGED ONLY IN THE MAIN MENU\n");
	    break;

        case 3: /* parameter mask */
	    c_blk->param = (word)get_param();
	    break;

        case 4: /* enhanced mode */
            c_blk->e_mode = (word)get_param();
	    break;

        case 5: /* write mask */
	    c_blk->mask_reg = (byte)get_param();
	break;

        case 6: /* event control */
	    finished2 = 0;
	    while( !finished2 )
	    {
	      printf("\nSet Up Event Control Array\n\n");
	      printf("1. Return to Previous Menu\n");
	      for(i = 0; i < 2; i++)
	      {
		printf("%d. Event Control Code for Array Index %d:%02X\n",
		   (i + 2),i,c_blk->ev_control[i]);
	      }
	      printf("\nselect: ");
	      scanf("%d",&item);
	      switch(item)
	      {
	       case 1: /* return to previous menu */
		    finished2++;
	       break;
		    
	       default:
		    if(item > 1 && item < 4)
		    {
		       printf("\nControl code - ");
		       c_blk->ev_control[item - 2] = (byte)get_param();
		    }
	       break;
	      }
	    }
	break;

        case 7: /* debounce clock */
	    c_blk->deb_clock = (byte)get_param();
	break;

        case 8: /* debounce control */
	    c_blk->deb_control = (byte)get_param();
	break;

        case 9: /* debounce duration */
	    finished2 = 0;
	    while( !finished2 )
	    {
	      printf("\nSet Up Debounce Duration Array\n\n");
	      printf("1. Return to Previous Menu\n");
	      for(i = 0; i < 2; i++)
	      {
		printf("%d. Debounce Duration Code for Array Index %d:%02X\n",
		   (i + 2),i,c_blk->deb_duration[i]);
	      }
	      printf("\nselect: ");
	      scanf("%d",&item);
	      switch(item)
	      {
	       case 1: /* return to previous menu */
		    finished2++;
	       break;
		    
	       default:
		    if(item > 1 && item < 4)
		    {
		       printf("\nDebounce Duration code - ");
		       c_blk->deb_duration[item - 2] = (byte)get_param();
		    }
	       break;
	      }
	    }
	break;

        case 10: /* interrupt enable reg. */
	    c_blk->enable = (byte)get_param();
	    break;

        case 11: /* exception vector number */
	    c_blk->vector = (byte)get_param();
	    break;
	}
    }
}



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr470.c

    MODULE NAME:    psts470 - print board status information

    VERSION:        A

    CREATION DATE:  10/16/95

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to cause the "Read Board Status"
		    command to be executed and to print out the results to
		    the console.

    CALLING
	SEQUENCE:   psts470(&c_block470)
		    where:
			c_block470 (structure pointer)
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

void psts470(c_blk)
struct cblk470 *c_blk;
{

/*
    DEFINITIONS:
*/


/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;           /* menu item variable */
    unsigned finished;  /* flags to exit loops */
    int i;

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
	rmid470(c_blk); /* get board's ID info into structure */
	rsts470(c_blk); /* get board's status info into structure */
	printf("\n\nBoard Status Information\n");
        printf("\nInterrupt Enable Register:   %02X",c_blk->enable);
	printf("\nInterrupt Vector Register:   %02X",c_blk->vector);

	printf("\n\nBoard ID Information\n");
	printf("\nIdentification:              ");
	for(i = 0; i < 4; i++)          /* identification */
	   printf("%c",c_blk->id_prom[i]);
	printf("\nManufacturer's ID:           %X",(byte)c_blk->id_prom[4]);
	printf("\nIP Model Number:             %X",(byte)c_blk->id_prom[5]);
	printf("\nRevision:                    %X",(byte)c_blk->id_prom[6]);
	printf("\nReserved:                    %X",(byte)c_blk->id_prom[7]);
	printf("\nDriver I.D. (low):           %X",(byte)c_blk->id_prom[8]);
	printf("\nDriver I.D. (high):          %X",(byte)c_blk->id_prom[9]);
	printf("\nTotal I.D. Bytes:            %X",(byte)c_blk->id_prom[10]);
	printf("\nCRC:                         %X",(byte)c_blk->id_prom[11]);

	printf("\n\n1. Return to Previous Menu");
	printf("\n2. Read Status Again");
	printf("\n\nselect: ");
	scanf("%d",&item);

	switch(item){

	case 1: /* return to previous menu */
	    finished++;
	    break;

	}
    }
}
