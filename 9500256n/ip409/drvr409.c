
#include "../carrier/apc8620.h"
#include "ip409.h"


#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */


/*
{+D}
    SYSTEM:         Software for IP409

    FILENAME:       drvr409.c

    MODULE NAME:    main - main routine of example software.

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       This module is the main routine for the example program
		    which demonstrates how the IP409 Library is used.

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
	which demonstrates how the IP409 Library is used.
*/

int main()
{
    

/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[40];   /* command line input buffer */
    long item;           /* menu item selection variable */
    long status;         /* returned status of driver routines */
    long hstatus;        /* interrupt handler returned status */
    unsigned finished;   /* flag to exit program */
    long addr;           /* long to hold board address */
    long flag;           /* general flag for exiting loops */
    long i;              /* loop index */
    unsigned point;      /* I/O point number */
    unsigned port;       /* I/O port number */
    unsigned val;        /* value of port or point */
    int hflag; /* interrupt handler installed flag */
    struct sblk409 s_block409;  /* allocate status param. blk */
    struct cblk409 c_block409;  /* configuration block */
    struct handler_data hdata;/* interrupt handler data structure */

/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/
 
    flag = 0;         /* indicate board address not yet assigned */
    finished = 0;     /* indicate not finished with program */
    hflag = 0;        /* indicate interrupt handler not installed yet */
    hstatus = 0;

/*
    Initialize the Configuration Parameter Block to default values.
*/

    c_block409.param = 0;		       /* parameter mask */
    c_block409.vector = VECTOR;           /* interrupt vector */
    c_block409.bCarrier = FALSE;          /* indicate no carrier initialized and set up yet */
    c_block409.bInitialized = FALSE;      /* indicate not ready to talk to IP */
    c_block409.slotLetter = SLOT_A;
    c_block409.nHandle = 0;               /* make handle to a closed carrier board */

    s_block409.vector = 0;

    for( i = 0; i < 2; i++)
    {
	c_block409.direction[i] = 0;      /* direction */
	c_block409.int_status[i] = 0;     /* pending interrupts to clear */
	c_block409.enable[i] = 0;         /* interrupt enable (per bit) */
	c_block409.polarity[i] = 0;       /* interrupt polarity */
	c_block409.type[i] = 0;           /* interrupt type */

	s_block409.direction[i] = 0;      /* direction */
	s_block409.int_status[i] = 0;     /* pending interrupts to clear */
	s_block409.enable[i] = 0;         /* interrupt enable (per bit) */
	s_block409.polarity[i] = 0;       /* interrupt polarity */
	s_block409.type[i] = 0;           /* interrupt type */
    }

    hdata.h_pid = getpid();		/* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block409;   /* put in address of c_block409 structure also */
    c_block409.sblk_ptr = (struct sblk409*)&s_block409;

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
    if(CarrierOpen(0, &c_block409.nHandle) != S_OK)
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

	printf("\n\nIP409 Library Demonstration  Version A\n\n");
	printf(" 1. Exit this Program\n");
	printf(" 2. Set Carrier Base Address\n");
	printf(" 3. Set IP Slot Letter\n");
	printf(" 4. Set Up Configuration Block Parameters\n");
	printf(" 5. Configure Board Command\n");
	printf(" 6. Read Status Command and ID\n");
	printf(" 7. Enable Interrupts\n");
	printf(" 8. Disable Interrupts\n");
	printf(" 9. Read Input Point\n");
	printf("10. Read Input Port\n");
	printf("11. Write Output Point\n");
	printf("12. Write Output Port\n");

	printf("\nSelect: ");
	scanf("%ld",&item);

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
	
	case 2: /* set board address command */
	    do 
	    {
		if(flag == 0)
		{
		  printf("\n\nenter base address of carrier board in hex: ");
		  scanf("%lx",&addr);
		  /* Set Carrier Address for Open Carrier Device */
		  SetCarrierAddress(c_block409.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block409.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
          SetCarrierAddress(c_block409.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block409.nHandle) == S_OK)/* Initialize Carrier */
		  {
		        c_block409.bCarrier = TRUE;
			SetInterruptLevel(c_block409.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;

				
	    }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
	    break;

	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block409.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block409.bCarrier = FALSE;
			}
			else
			{
				c_block409.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block409.nHandle, c_block409.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block409.bInitialized = FALSE;
				}
				else	              
				{	
			                c_block409.brd_ptr = (struct map409 *)addr;
					c_block409.bInitialized = TRUE;
				}
			}
		}
	break;

	case 4: /* set up configuration block parameters */

	    scfg409(&c_block409);
	    break;

	case 5:     /* configure board command */
	
            if(!c_block409.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
/*
    Check for pending interrupts and check the
    "interrupt handler attached" flag.  If interrupts are pending or
    if interrupt handlers are not attached, then print an error message.
    If both conditions were false, then go ahead and execute the command.
*/

		if( input_word( c_block409.nHandle, (word*)&c_block409.brd_ptr->sts_reg[0] ) || ( input_word( c_block409.nHandle, (word*)&c_block409.brd_ptr->sts_reg[1] ) && 0x00FF))
		      printf(">>> ERROR: INTERRUPTS ARE PENDING <<<\n");
		else
		{
		   if( hflag == 0 && ( c_block409.enable[0] || c_block409.enable[1] ))
			printf(">>> ERROR: INTERRUPT HANDLER NOT ATTACHED <<<\n");
		   else
		        cnfg409(&c_block409); /* configure the board */
		}
	    }
	    break;

	case 6:     /* read board status command */
	
            if(!c_block409.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
		psts409(&c_block409); /* read board status */
	    break;

    case 7:     /* enable interrupts */
		EnableInterrupts( c_block409.nHandle );
		hflag = 1;
    break;

	case 8: /* detach exception handlers */
		hflag = 0;
		DisableInterrupts(c_block409.nHandle);
	 break;

	case 9: /* Read Digital Input Point */

            if(!c_block409.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Input port number   (0 - 1): ");
		scanf("%d",&port);
		printf("\nEnter Input point number (0 - 15): ");
		scanf("%d",&point);
		status = rpnt409(&c_block409,port,point);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
		else
		    printf("\nValue of port %d point %d: %lX\n",port,point,status);
	    }
	    break;

	case 10: /* Read Digital Input Port */

            if(!c_block409.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Input port number  (0 - 1):  ");
		scanf("%d",&port);
		status = rprt409(&c_block409,port);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
		else
		    printf("\nValue of port %d: %lX\n",port,status);
	    }
	    break;


	case 11: /* Write Digital Point */

            if(!c_block409.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Output port number (0 - 1):  ");
		scanf("%d",&port);
		printf("\nEnter I/O point number  (0 - 15): ");
		scanf("%d",&point);
		printf("\nEnter point value (0 - 1): ");
		scanf("%x",&val);
		status = wpnt409(&c_block409,port,point,val);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
	    }
	    break;

	case 12: /* Write Digital Port */

            if(!c_block409.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Output port number (0 - 1):  ");
		scanf("%d",&port);
		printf("\nEnter output value in hex (0000 - FFFF): ");
		scanf("%x",&val);
		status = wprt409(&c_block409,port,val);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
	    }
	    break;
	}   /* end of switch */
    }   /* end of while */

/*
    disable interrupts from IP module
*/
    if(c_block409.bInitialized)		/* module address was set */
    {
      c_block409.param = 0xFFFF;		/* parameter mask */
      c_block409.int_status[0] = 0xffff;   /* pending interrupts to clear */
      c_block409.int_status[1] = 0x00ff;   /* pending interrupts to clear */
      c_block409.enable[0] = c_block409.enable[1] = 0; /* interrupt enable (per bit) */
      cnfg409(&c_block409);		/* configure the board */
    }


    DisableInterrupts(c_block409.nHandle);
    if(c_block409.bCarrier)
	CarrierClose(c_block409.nHandle);

    printf("\nEXIT PROGRAM\n");

}   /* end of main */



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr409.c

    MODULE NAME:    get_param - get a parameter from the console

    VERSION:        A

    CREATION DATE:  05/19/98

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


int get_port_num()
{
 
   int i;

askagain:
   printf("\nEnter port number ( 0, 1 ) ");
   scanf("%d",&i);
   if( i > 1 || i < 0 )
     goto askagain;

   return( i );
}



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr409.c

    MODULE NAME:    scfg409 - set configuration block contents.

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to enter parameters into
		    the Configuration Block.
 
    CALLING
	SEQUENCE:   scfg409(c_block409)
		    where:
			c_block409 (structure pointer)
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

void scfg409(c_blk)

struct cblk409 *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;                       /* menu item variable */
    unsigned finished;		    /* flags to exit loops */
    int i;                          /* loop index */

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
	printf("\n\nConfiguration Block Parameters\n\n");

	printf(" 1. Return to Previous Menu\n");
	printf(" 2. Board Pointer:      %08lX\n",(unsigned long)c_blk->brd_ptr);
	printf(" 3. Parameter Mask:     %04X\n",c_blk->param);
	printf(" 4. Int. Status:        0 = %04X  1 = %04X\n",c_blk->int_status[0], c_blk->int_status[1]);
	printf(" 5. Int. Enable:        0 = %04X  1 = %04X\n",c_blk->enable[0], c_blk->enable[1] );
	printf(" 6. Int. Polarity:      0 = %04X  1 = %04X\n",c_blk->polarity[0], c_blk->polarity[1]);
	printf(" 7. Int. Type:          0 = %04X  1 = %04X\n",c_blk->type[0], c_blk->type[1]);
	printf(" 8. Data Direction:     0 = %04X  1 = %04X\n",c_blk->direction[0], c_blk->direction[1]);
	printf(" 9. Int. Vector:        %02X\n",c_blk->vector);
    
	printf("\nSelect: ");
	scanf("%d",&item);

	switch(item){
	
	case 1: /* return to previous menu */
	    finished++;
	    break;

	case 2: /* board address */
	    printf("BOARD ADDRESS CAN BE CHANGED ONLY IN THE MAIN MENU\n");
	    break;

	case 3: /* parameter mask */
	    c_blk->param = (word)get_param();
	    break;

	case 4: /* interrupt status register */
	    i = get_port_num();
	    c_blk->int_status[i] = (word)get_param();
	    break;

	case 5: /* interrupt enable reg. */
	    i = get_port_num();
	    c_blk->enable[i] = (word)get_param();
	    break;

	case 6: /* interrupt polarity reg. */
	    i = get_port_num();
	    c_blk->polarity[i] = (word)get_param();
	    break;

	case 7: /* interrupt type reg. */
	    i = get_port_num();
	    c_blk->type[i] = (word)get_param();
	    break;

	case 8: /* direction reg. */
	    i = get_port_num();
	    c_blk->direction[i] = (word)get_param();
	    break;

	case 9: /* exception vector number */
	    c_blk->vector = (byte)get_param();
	    break;
	}
    }
}



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr409.c

    MODULE NAME:    psts409 - print board status information

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to cause the "Read Board Status"
		    command to be executed and to print out the results to
		    the console.

    CALLING
	SEQUENCE:   psts409(&c_block409)
		    where:
			c_block409 (structure pointer)
			  The address of the configuration/status param. block
			 
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


void psts409(c_blk)
struct cblk409 *c_blk;
{

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
	rsts409(c_blk); /* get board's status info into structure */
	printf("\n\nBoard Status Information");
	printf("\nData Direction Registers:     0 = %04X  1 = %04X",
		c_blk->sblk_ptr->direction[0], c_blk->sblk_ptr->direction[1] );
	printf("\nInterrupt Status Registers:   0 = %04X  1 = %04X",
		c_blk->sblk_ptr->int_status[0], c_blk->sblk_ptr->int_status[1]);
	printf("\nInterrupt Enable Registers:   0 = %04X  1 = %04X",
		c_blk->sblk_ptr->enable[0], c_blk->sblk_ptr->enable[1]);
	printf("\nInterrupt Polarity Registers: 0 = %04X  1 = %04X",
		c_blk->sblk_ptr->polarity[0], c_blk->sblk_ptr->polarity[1] );
	printf("\nInterrupt Type Registers:     0 = %04X  1 = %04X",
		c_blk->sblk_ptr->type[0], c_blk->sblk_ptr->type[1] );
	printf("\nInterrupt Vector Register:    %02X",c_blk->sblk_ptr->vector);

	printf("\n\nBoard ID Information");
	printf("\nIdentification:               ");
	for(i = 0; i < 4; i++)          /* identification */
	   printf("%c",c_blk->sblk_ptr->id_prom[i]);
	printf("\nManufacturer's ID:            %02X",(byte)c_blk->sblk_ptr->id_prom[4]);
	printf("\nIP Model Number:              %02X",(byte)c_blk->sblk_ptr->id_prom[5]);
	printf("\nRevision:                     %02X",(byte)c_blk->sblk_ptr->id_prom[6]);
	printf("\nReserved:                     %02X",(byte)c_blk->sblk_ptr->id_prom[7]);
	printf("\nDriver I.D. (low):            %02X",(byte)c_blk->sblk_ptr->id_prom[8]);
	printf("\nDriver I.D. (high):           %02X",(byte)c_blk->sblk_ptr->id_prom[9]);
	printf("\nTotal I.D. Bytes:             %02X",(byte)c_blk->sblk_ptr->id_prom[10]);
	printf("\nCRC:                          %02X",(byte)c_blk->sblk_ptr->id_prom[11]);

	printf("\n\nRead Board Status & ID Information\n");
	printf(" 1. Return to Previous Menu\n");
	printf(" 2. Read Status Again\n");
/*	printf(" 3. Clear Interrupt Status Flags\n"); */ /* no async interrupts */
	printf("\nselect: ");
	scanf("%d",&item);

	switch(item){

	case 1: /* return to previous menu */
	    finished++;
	    break;

	case 3: /* clear flags */
		c_blk->sblk_ptr->int_status[0] = c_blk->sblk_ptr->int_status[1] = 0;
	    break;

	}
    }
}

