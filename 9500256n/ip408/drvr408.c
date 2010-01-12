
#include "../carrier/apc8620.h"
#include "ip408.h"

#define INTERRUPT_LEVEL 5	/* level or bus IRQ - may not be used on all systems */
#define VECTOR 192		/* interrupt vector - may not be used on all systems */


/*
{+D}
    SYSTEM:         Software for IP408

    FILENAME:       drvr408.c

    MODULE NAME:    main - main routine of example software.

    VERSION:        B

    CREATION DATE:  01/05/95

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       This module is the main routine for the example program
		    which demonstrates how the IP408 Library is used.

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
        which demonstrates how the IP408 Library is used.
*/



int main()
{
    

/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[40];   /* command line input buffer */
    int item;            /* menu item selection variable */
    int status;          /* returned status of driver routines */
    CSTATUS hstatus;     /* interrupt handler returned status */
    unsigned finished;   /* flag to exit program */
    long addr;           /* hold board address */
    long flag;           /* general flag for exiting loops */
    unsigned point;      /* I/O point number */
    unsigned port;       /* I/O port number */
    unsigned val;        /* value of port or point */
    int hflag;		 /* interrupt handler installed flag */
    struct sblk408 s_block408;  /* allocate status param. blk */
    struct cblk408 c_block408;  /* configuration block */
    struct handler_data hdata;/* interrupt handler structure */

/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/

    flag = 0;	 	 /* indicate board address not yet assigned */
    finished = 0;	 /* indicate not finished with program */
    hflag = 0;   	 /* indicate interrupt handler not installed yet */

/*
    Initialize the Configuration Parameter Block to default values.
*/

    c_block408.param = 0;          /* parameter mask */
    c_block408.int_status = 0;     /* pending interrupts to clear */
    c_block408.enable = 0;         /* interrupt enable (per bit) */
    c_block408.polarity = 0;       /* interrupt polarity */
    c_block408.type = 0;           /* interrupt type */
    c_block408.vector = VECTOR;    /* interrupt vector base */

    c_block408.bCarrier = FALSE;	/* indicate no carrier initialized and set up yet */
    c_block408.bInitialized = FALSE;/* indicate not ready to talk to IP */
    c_block408.slotLetter = SLOT_A;
    c_block408.nHandle = 0;	/* make handle to a closed carrier board */

    s_block408.int_status = 0;     /* pending interrupts to clear */
    s_block408.enable = 0;         /* interrupt enable (per bit) */
    s_block408.polarity = 0;       /* interrupt polarity */
    s_block408.type = 0;           /* interrupt type */
    s_block408.vector = 0;         /* interrupt vector base */

    hdata.h_pid = getpid();		/* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block408;	/* put in address of c_block408 structure also */
    c_block408.sblk_ptr = (struct sblk408*)&s_block408;

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
    if(CarrierOpen(0, &c_block408.nHandle) != S_OK)
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

        printf("\n\nIP408 Library Demonstration  Version A\n\n");

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
        printf("11. Read Output Point (from output data latch)\n");
        printf("12. Read Output Port  (from output data latch)\n");
        printf("13. Write Output Point\n");
        printf("14. Write Output Port\n");

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
        
	case 2: /* set board address command */
	    do 
	    {
		if(flag == 0)
		{
		  printf("\n\nenter base address of carrier board in hex: ");
		  scanf("%lx",&addr);
		  /* Set Carrier Address for Open Carrier Device */
		  SetCarrierAddress(c_block408.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block408.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
		  SetCarrierAddress(c_block408.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block408.nHandle) == S_OK)/* Initialize Carrier */
		  {
		    c_block408.bCarrier = TRUE;
			SetInterruptLevel(c_block408.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;

	    }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
	break;

	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block408.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block408.bCarrier = FALSE;
			}
			else
			{
				c_block408.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block408.nHandle, c_block408.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block408.bInitialized = FALSE;
				}
				else	              
				{	
			        c_block408.brd_ptr = (struct map408 *)addr;
					c_block408.bInitialized = TRUE;
				}
			}
		}
	break;

        case 4: /* set up configuration block parameters */

            scfg408(&c_block408);
        break;

        case 5:     /* configure board command */
        
            if(!c_block408.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
			{
/*
    Check for pending interrupts and check the
    "interrupt handler attached" flag.  If interrupts are pending or
    if interrupt handlers are not attached, then print an error message.
    If both conditions were false, then go ahead and execute the command.
*/

              if( input_byte ( c_block408.nHandle,&c_block408.brd_ptr->sts_reg ))
                  printf(">>> ERROR: INTERRUPTS ARE PENDING <<<\n");
		      else
			  {
                 if( hflag == 0 )
                     printf(">>> ERROR: INTERRUPT HANDLER NOT ATTACHED <<<\n");
		         else
                     cnfg408(&c_block408); /* configure the board */
              }
            }
        break;

        case 6:     /* read board status command */
        
			if(!c_block408.bInitialized)
				printf("\n>>> ERROR: BOARD NOT SET UP <<<\n");
			else
			{
				psts408(&c_block408);  /* read board */
			}
			break;

        case 7:     /* enable interrupts */
		EnableInterrupts( c_block408.nHandle );
		hflag = 1;
        break;

        case 8: /* detach exception handlers */
	    hflag = 0;
	    DisableInterrupts(c_block408.nHandle);
        break;

        case 9: /* Read Digital Input Point */

            if(!c_block408.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
                printf("\nEnter Input port number   (0 - 1): ");
                scanf("%d",&port);
                printf("\nEnter Input point number (0 - 15): ");
                scanf("%d",&point);
                status = rpnt408(&c_block408,port,point);
                if(status == -1)
                    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
                else
                    printf("\nValue of port %d point %d: %X\n",port,point,status);
            }
            break;

        case 10: /* Read Digital Input Port */

            if(!c_block408.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
                printf("\nEnter Input port number  (0 - 1):  ");
                scanf("%d",&port);
                status = rprt408(&c_block408,port);
                if(status == -1)
                    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
                else
                    printf("\nValue of port %d: %X\n",port,status);
            }
            break;

        case 11: /* Read Output Point */

            if(!c_block408.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
                printf("\nEnter Output port number   (0 - 1):  ");
                scanf("%d",&port);
                printf("\nEnter Output point number (0 - 15): ");
                scanf("%d",&point);
                status = robt408(&c_block408,port,point);
                if(status == -1)
                  printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
                else
                  printf("\nValue of port %d point %d: %X\n",port,point,status);
            }
            break;

        case 12: /* Read Output Port */

            if(!c_block408.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
                printf("\nEnter Output port number  (0 - 1):  ");
                scanf("%d",&port);
                status = ropt408(&c_block408,port);
                if(status == -1)
                  printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
                else
                  printf("\nValue of port %d: %X\n",port,status);
            }
            break;

        case 13: /* Write Digital Point */

            if(!c_block408.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
                printf("\nEnter Output port number (0 - 1):  ");
                scanf("%d",&port);
                printf("\nEnter I/O point number  (0 - 15): ");
                scanf("%d",&point);
                printf("\nEnter point value (0 - 1): ");
                scanf("%x",&val);
                status = wpnt408(&c_block408,port,point,val);
                if(status == -1)
                    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
            }
            break;

        case 14: /* Write Digital Port */

            if(!c_block408.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
                printf("\nEnter Output port number (0 - 1):  ");
                scanf("%d",&port);
                printf("\nEnter output value in hex (0 - FFFF): ");
                scanf("%x",&val);
                status = wprt408(&c_block408,port,val);
                if(status == -1)
                    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
            }
            break;
        }   /* end of switch */
    }   /* end of while */
	
/*
    disable interrupts from IP module
*/
    if(c_block408.bInitialized)	  /* module address was set */
    {
      c_block408.param = 0xff;       /* parameter mask */
      c_block408.int_status = 0xff;  /* pending interrupts to clear */
      c_block408.enable = 0;         /* interrupt enable (per bit) */
      cnfg408(&c_block408);	  /* configure the board */
    }

    DisableInterrupts(c_block408.nHandle);
    if(c_block408.bCarrier)
	CarrierClose(c_block408.nHandle);

    printf("\nEXIT PROGRAM\n");

}   /* end of main */



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr408.c

    MODULE NAME:    get_param - get a parameter from the console

    VERSION:        A

    CREATION DATE:  01/05/95

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

    int temp;

    printf("enter hex parameter: ");
    scanf("%x",&temp);
    printf("\n");
    return((long)temp);
}



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr408.c

    MODULE NAME:    scfg408 - set configuration block contents.

    VERSION:        A

    CREATION DATE:  01/05/95

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to enter parameters into
		    the Configuration Block.
 
    CALLING
        SEQUENCE:   scfg408(c_block408)
                    where:
                        c_block408 (structure pointer)
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

void scfg408(c_blk)

struct cblk408 *c_blk;
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
    unsigned finished;		    /* flags to exit loops */

    
/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
        printf("\n\nConfiguration Block Parameters\n\n");

        printf(" 1. Return to Previous Menu\n");
        printf(" 2. Board Pointer:    %lX\n",(unsigned long)c_blk->brd_ptr);
        printf(" 3. Parameter Mask:   %X\n",c_blk->param);
        printf(" 4. Int. Status:      %X\n",c_blk->int_status);
        printf(" 5. Int. Enable:      %X\n",c_blk->enable);
        printf(" 6. Int. Polarity:    %X\n",c_blk->polarity);
        printf(" 7. Int. Type:        %X\n",c_blk->type);
        printf(" 8. Int. Vector Base: %X\n",c_blk->vector);
    
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
            c_blk->int_status = (byte)get_param();
            break;

        case 5: /* interrupt enable reg. */
            c_blk->enable = (byte)get_param();
            break;

        case 6: /* interrupt polarity reg. */
            c_blk->polarity = (byte)get_param();
            break;

        case 7: /* interrupt type reg. */
            c_blk->type = (byte)get_param();
            break;

        case 8: /* point 0 exception vector number */
            c_blk->vector = (byte)get_param();
            break;
        }
    }
}



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr408.c

    MODULE NAME:    psts408 - print board status information

    VERSION:        A

    CREATION DATE:  4/19/89

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to cause the "Read Board Status"
		    command to be executed and to print out the results to
		    the console.

    CALLING
        SEQUENCE:   psts408(&c_block408)
                    where:
                        c_block408 (structure pointer)
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

void psts408(c_blk)
struct cblk408 *c_blk;
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

	rsts408(c_blk);          /* Read Command */
   
        printf("\n\nBoard Status Information");
        printf("\nInterrupt Status Register:   %X",c_blk->sblk_ptr->int_status);
        printf("\nInterrupt Enable Register:   %X",c_blk->sblk_ptr->enable);
        printf("\nInterrupt Polarity Register: %X",c_blk->sblk_ptr->polarity);
        printf("\nInterrupt Type Register:     %X",c_blk->sblk_ptr->type);
        printf("\nInterrupt Vector Register:   %X",c_blk->sblk_ptr->vector);

        printf("\n\nBoard ID Information");
	printf("\nIdentification:              ");
        for(i = 0; i < 4; i++)		/* identification */
	   printf("%c",c_blk->sblk_ptr->id_prom[i]);
	printf("\nManufacturer's ID:           %X",(byte)c_blk->sblk_ptr->id_prom[4]);
	printf("\nIP Model Number:             %X",(byte)c_blk->sblk_ptr->id_prom[5]);
	printf("\nRevision:                    %X",(byte)c_blk->sblk_ptr->id_prom[6]);
	printf("\nReserved:                    %X",(byte)c_blk->sblk_ptr->id_prom[7]);
	printf("\nDriver I.D. (low):           %X",(byte)c_blk->sblk_ptr->id_prom[8]);
	printf("\nDriver I.D. (high):          %X",(byte)c_blk->sblk_ptr->id_prom[9]);
	printf("\nTotal I.D. Bytes:            %X",(byte)c_blk->sblk_ptr->id_prom[10]);
	printf("\nCRC:                         %X",(byte)c_blk->sblk_ptr->id_prom[11]);

        printf("\n\nRead Board Status & ID Information\n");
        printf(" 1. Return to Previous Menu\n");
        printf(" 2. Read Status Again\n");
/*	printf(" 3. Clear Interrupt Status Flags\n");*/  /* no async I/O */
	printf("\nselect: ");
	scanf("%d",&item);

	switch(item){

	case 1: /* return to previous menu */
	    finished++;
	    break;

	case 3: /* clear flags */
		c_blk->sblk_ptr->int_status = 0;
	    break;
        }
    }
}












