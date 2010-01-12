
#include "../carrier/apc8620.h"
#include "ip400.h"


#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */

/*
{+D}
    SYSTEM:         IP Software

    FILENAME:       drvr400.c

    MODULE NAME:    main - main routine of example software.

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       This module is the main routine for the example program
		    which demonstrates how the IP400 Library is used.

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
        which demonstrates how the IP400 Library is used.
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
    unsigned point;      /* I/O point number */
    unsigned port;       /* I/O port number */
    int hflag;           /* interrupt handler installed flag */
    
    struct sblk400 s_block400;  /* allocate status param. blk */
    struct cblk400 c_block400;  /* configuration block */
    struct handler_data hdata; /* interrupt handler data structure */

/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/

    flag = 0;         /* indicate board address not yet assigned */
    finished = 0;     /* indicate not finished with program */
    hflag = 0;        /* indicate interrupt handler not installed yet */

/*
    Initialize Configuration and Status Parameter Block to default values.
*/

    s_block400.int_status = 0;     /* pending interrupts to clear */
    s_block400.enable = 0;         /* interrupt enable (per bit) */
    s_block400.polarity = 0;       /* interrupt polarity */
    s_block400.type = 0;           /* interrupt type */
    s_block400.vector = 0;         /* interrupt vector base */

    c_block400.param = 0;          /* parameter mask */
    c_block400.int_status = 0;     /* pending interrupts to clear */
    c_block400.enable = 0;         /* interrupt enable (per bit) */
    c_block400.polarity = 0;       /* interrupt polarity */
    c_block400.type = 0;           /* interrupt type */
    c_block400.vector = VECTOR;    /* interrupt vector base */

    c_block400.bCarrier = FALSE;	/* indicate no carrier initialized and set up yet */
    c_block400.bInitialized = FALSE;/* indicate not ready to talk to IP */
    c_block400.slotLetter = SLOT_A;
    c_block400.nHandle = 0;	/* make handle to a closed carrier board */

    hdata.h_pid = getpid();		/* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block400;	/* put in address of c_block400 structure also */
    c_block400.sblk_ptr = (struct sblk400*)&s_block400;

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
    if(CarrierOpen(0, &c_block400.nHandle) != S_OK)
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

        printf("\n\nIP400 Library Demonstration  Version A\n\n");

        printf(" 1. Exit this Program\n");
        printf(" 2. Set Carrier Base Address\n");
        printf(" 3. Set IP Slot Letter\n");
        printf(" 4. Set Up Configuration Block Parameters\n");
        printf(" 5. Configure Board Command\n");
        printf(" 6. Read Status Command & ID\n");
        printf(" 7. Enable Interrupts\n");
        printf(" 8. Disable Interrupts\n");
        printf(" 9. Read Input Point\n");
        printf("10. Read Input Port\n");

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
		  SetCarrierAddress(c_block400.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block400.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
          SetCarrierAddress(c_block400.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block400.nHandle) == S_OK)/* Initialize Carrier */
		  {
		        c_block400.bCarrier = TRUE;
			SetInterruptLevel(c_block400.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;

				
	    }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
	    break;

	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block400.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block400.bCarrier = FALSE;
			}
			else
			{
				c_block400.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block400.nHandle, c_block400.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block400.bInitialized = FALSE;
				}
				else	              
				{	
			                c_block400.brd_ptr = (struct map400 *)addr;
					c_block400.bInitialized = TRUE;
				}
			}
		}
	    break;

        case 4: /* set up configuration block parameters */

            scfg400(&c_block400);
            break;

        case 5:     /* configure board command */
        
	    if(!c_block400.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
/*
    Check for pending interrupts and check the
    "interrupt handler attached" flag.  If interrupts are pending or
    if interrupt handlers are not attached, then print an error message.
    If both conditions were false, then go ahead and execute the command.
*/

                if( input_word ( c_block400.nHandle, (word*)&c_block400.brd_ptr->sts_reg ))
                    printf(">>> ERROR: INTERRUPTS ARE PENDING <<<\n");
		else
		{
                    if( hflag == 0 )
                        printf(">>> ERROR: INTERRUPT HANDLER NOT ATTACHED <<<\n");
		    else
                        cnfg400(&c_block400); /* configure the board */
                }
            }
            break;

        case 6:     /* read board status command */
        
	    if(!c_block400.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
		psts400(&c_block400);  /* read board */
            break;

        case 7:     /* enable interrupts */
		EnableInterrupts( c_block400.nHandle );
		hflag = 1;
        break;

        case 8: /* detach exception handlers */
	    hflag = 0;
	    DisableInterrupts(c_block400.nHandle);
        break;

        case 9: /* Read Digital Input Point */

	    if(!c_block400.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
                printf("\nEnter Input port number   (0 - 2): ");
                scanf("%d",&port);
                printf("\nEnter Input point number (0 - 15): ");
                scanf("%d",&point);
                status = rpnt400(&c_block400,port,point);
                if(status == -1)
                    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
                else
                    printf("\nValue of port %d point %d: %lX\n",port,point,status);
            }
            break;

        case 10: /* Read Digital Input Port */

	    if(!c_block400.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
                printf("\nEnter Input port number  (0 - 2):  ");
                scanf("%d",&port);
                status = rprt400(&c_block400,port);
                if(status == -1)
                    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
                else
                    printf("\nValue of port %d: %lX\n",port,status);
            }
            break;
        }   /* end of switch */
    }   /* end of while */


/*
    disable interrupts from IP module
*/
    if(c_block400.bInitialized)	  /* module address was set */
    {
      c_block400.param = 0xff;       /* parameter mask */
      c_block400.int_status = 0xfff; /* pending interrupts to clear */
      c_block400.enable = 0;         /* interrupt enable (per bit) */
      cnfg400(&c_block400);	  /* configure the board */
    }

    DisableInterrupts(c_block400.nHandle);
    if(c_block400.bCarrier)
	CarrierClose(c_block400.nHandle);

    printf("\nEXIT PROGRAM\n");

}   /* end of main */



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr400.c

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

    printf("enter hex parameter: ");
    scanf("%lx",&temp);
    printf("\n");
    return(temp);
}



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr400.c

    MODULE NAME:    scfg400 - set configuration block contents.

    VERSION:        A

    CREATION DATE:  01/12/95

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to enter parameters into
		    the Configuration Block.
 
    CALLING
        SEQUENCE:   scfg400(c_block400)
                    where:
                        c_block400 (structure pointer)
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

void scfg400(c_blk)

struct cblk400 *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;                       /* menu item variable */
    unsigned finished;    /* flags to exit loops */

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
            c_blk->int_status = (word)get_param();
            break;

        case 5: /* interrupt enable reg. */
            c_blk->enable = (word)get_param();
            break;

        case 6: /* interrupt polarity reg. */
            c_blk->polarity = (word)get_param();
            break;

        case 7: /* interrupt type reg. */
            c_blk->type = (word)get_param();
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

    FILENAME:       drvr400.c

    MODULE NAME:    psts400 - print board status information

    VERSION:        A

    CREATION DATE:  01/12/95

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to cause the "Read Board Status"
		    command to be executed and to print out the results to
		    the console.

    CALLING
        SEQUENCE:   psts400(&s_block400)
                    where:
                        s_block400 (structure pointer)
                          The address of the status param. block
                         
    MODULE TYPE:    void
    
    I/O RESOURCES:  
    
    SYSTEM
        RESOURCES:  
        
    MODULES
        CALLED:     rsts400()      Read Board Status Command.

    REVISIONS:      
    
  DATE    BY        PURPOSE
-------  ----   ------------------------------------------------

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:
    
*/

void psts400(c_blk)
struct cblk400 *c_blk;
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
	rsts400(c_blk);          /* Read Command */

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
/*	printf(" 3. Clear Interrupt Status Flags\n"); */ /* no async I/O */
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
