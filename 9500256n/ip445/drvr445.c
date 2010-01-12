
#include "../carrier/apc8620.h"
#include "ip445.h"

#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */


/*
{+D}
	SYSTEM:         Library Software

	FILENAME:       drvr445.c

	MODULE NAME:    main - main routine of example software.

	VERSION:        A

	CREATION DATE:  05/19/98

	DESIGNED BY:    BDB

	CODED BY:       BDB

	ABSTRACT:       This module is the main routine for the example program
			which demonstrates how the IP445 Library is used.
 
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
	which demonstrates how the IP445 Library is used.
*/

int main()
{
	
/*
	DECLARE LOCAL DATA AREAS:
*/
	char cmd_buff[40];  /* command line input buffer */
	long item;           /* menu item selection variable */
	long status;         /* returned status of driver routines */
	long addr;           /* long to hold board address */
	long flag;           /* general flag for exiting loops */
    unsigned finished;   /* flag to exit program */
	unsigned port;       /* I/O port number */
	unsigned point;      /* I/O point number */
	unsigned val;            /* value of port or point */
	struct sblk445 s_block445;  /* allocate status param. blk */

/*
	ENTRY POINT OF ROUTINE:
	INITIALIZATION
*/

	flag = 0;			/* indicate board address not yet assigned */
	finished = 0;			/* indicate not finished with program */
    s_block445.bCarrier = FALSE;	/* indicate no carrier initialized and set up yet */
    s_block445.bInitialized = FALSE;	/* indicate not ready to talk to IP */
	s_block445.slotLetter = SLOT_A;
	s_block445.nHandle = 0;		/* make handle to a closed carrier board */

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
    if(CarrierOpen(0, &s_block445.nHandle) != S_OK)
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

		printf("\n\nIP445 Library Demonstration  Version A\n\n");

		printf(" 1. Exit this Program\n");
		printf(" 2. Set Carrier Base Address\n");
		printf(" 3. Set IP Slot Letter\n");
		printf(" 4. Read ID ROM Command\n");
		printf(" 5. Read Output Point (from output data latch)\n");
		printf(" 6. Read Output Port  (from output data latch)\n");
		printf(" 7. Write Output Point\n");
		printf(" 8. Write Output Port\n");
		printf(" 9. Perform a Software Reset\n");
/*    	printf("10. Access Carrier Vital Product Data EEPROM\n");*/

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
			SetCarrierAddress(s_block445.nHandle, addr);	/* Set Carrier Address */
		  }

	 	  GetCarrierAddress(s_block445.nHandle, &addr);	/*  Read back carrier address */
		  printf("address: %lx\n",addr);
		  printf("is this value correct(y/n)?: ");
		  scanf("%s",cmd_buff);
		  if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		  {
            SetCarrierAddress(s_block445.nHandle, addr);	/* Set Carrier Address */
			if(CarrierInitialize(s_block445.nHandle) == S_OK)		/* Initialize Carrier */
			{
				s_block445.bCarrier = TRUE;
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
		  if(flag == 0 || s_block445.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		  else
		  {
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				s_block445.bCarrier = FALSE;
			}
			else
			{
				s_block445.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(s_block445.nHandle, s_block445.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					s_block445.bInitialized = FALSE;
				}
				else	              
				{	
	                s_block445.brd_ptr = (struct map445 *)addr;
					s_block445.bInitialized = TRUE;
				}
			}
		  }
		break;

		case 4:     /* read board I.D. command */
			if(!s_block445.bInitialized)
				printf("\n>>> ERROR: BOARD NOT SET UP <<<\n");
			else
			{
				psts445(&s_block445);  /* read board */
			}
			break;

		case 5: /* Read Digital I/O Point */
		
			if(!s_block445.bInitialized)
				printf("\n>>> ERROR: BOARD NOT SET UP <<<\n");
			else
			{
				printf("Enter port number (0 - 3): ");
				scanf("%d",&port);
				printf("\nEnter I/O point number (0 - 7): ");
				scanf("%d",&point);
				status = rpnt445(&s_block445,port,point);
				if(status == -1)
					printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
				else
				{
					printf("\nValue of port %d, point %d: %lx\n",port
					   ,point,status);
				}
			}
			break;

		case 6: /* Read Digital I/O Port */
		
			if(!s_block445.bInitialized)
				printf("\n>>> ERROR: BOARD NOT SET UP <<<\n");
			else
			{
				printf("Enter port number (0 - 3): ");
				scanf("%d",&port);
				status = rprt445(&s_block445,port);
				if(status == -1)
					printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
				else
					printf("\nValue of port %d: %lx\n",port,status);
			}
			break;

		case 7: /* Write Digital I/O Point */

			if(!s_block445.bInitialized)
				printf("\n>>> ERROR: BOARD NOT SET UP <<<\n");
			else
			{
				printf("Enter port number (0 - 3): ");
				scanf("%d",&port);
				printf("\nEnter I/O point number (0 - 7): ");
				scanf("%d",&point);
				printf("\nEnter point output value (0 or 1): ");
				scanf("%x",&val);
				status = wpnt445(&s_block445,port,point,(byte)val);
				if(status == -1)
					printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
			}
			break;

		case 8: /* Write Digital I/O Port */

			if(!s_block445.bInitialized)
				printf("\n>>> ERROR: BOARD NOT SET UP <<<\n");
			else
			{
				printf("Enter port number (0 - 3): ");
				scanf("%d",&port);
				printf("\nEnter port output value in hex (0 - 0xFF): ");
				scanf("%x",&val);
				status = wprt445(&s_block445,port,(byte)val);
				if(status == -1)
					printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
			}
			break;
		
		case 9: /* Perform a Software Reset */

			if(!s_block445.bInitialized)
				printf("\n>>> ERROR: BOARD NOT SET UP<<<\n");
			else
			{
			status = wcntl445(&s_block445, (byte)0x01);    /* call the routine that
							   writes the control register */
			}
			break;

		case 10: /* Access VPD */
		    access_EEPROM( &s_block445 );
		    break;

		
		}   /* end of switch */
	}   /* end of while */

	if(s_block445.bCarrier)
		CarrierClose(s_block445.nHandle);

	printf("\nEXIT PROGRAM\n");
}   /* end of main */



/*
{+D}
	SYSTEM:         Library Software

	FILENAME:       drvr445.c

	MODULE NAME:    get_param - get a parameter from the console

	VERSION:        A

	CREATION DATE:  10/7/88

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

/*
	return the input value
*/

	return(temp);
}



/*
{+D}
	SYSTEM:         Library Software

	FILENAME:       drvr445.c

	MODULE NAME:    psts445 - print board information

	VERSION:        A

	CREATION DATE:  05/19/98

	DESIGNED BY:    BDB

	CODED BY:       BDB

	ABSTRACT:       Routine which is used to cause the "Read Board I.D."
			command to be executed and to print out the results to
			the console.

	CALLING
		SEQUENCE:   psts445(&s_block445)
					where:
					s_block445 (structure pointer)
					  The address of the status param. block

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

void psts445(s_blk)
struct sblk445 *s_blk;
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
	rsts445(s_blk);          /* Read Command */     
	printf("\n\nBoard I.D. Information\n\n");
	printf("Identification:         ");
	for(i = 0; i < 4; i++)      /* identification */
	   printf("%c",s_blk->id_prom[i]);
	printf("\nManufacturer's I.D.:    %x",(byte)s_blk->id_prom[4]);
	printf("\nIP Model Number:        %x",(byte)s_blk->id_prom[5]);
	printf("\nRevision:               %x",(byte)s_blk->id_prom[6]);
	printf("\nReserved:               %x",(byte)s_blk->id_prom[7]);
	printf("\nDriver I.D. (low):      %x",(byte)s_blk->id_prom[8]);
	printf("\nDriver I.D. (high):     %x",(byte)s_blk->id_prom[9]);
	printf("\nTotal I.D. Bytes:       %x",(byte)s_blk->id_prom[10]);
	printf("\nCRC:                    %x",(byte)s_blk->id_prom[11]);

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
    SYSTEM:         Software for pmclxsx

    FILENAME:       drvrlxsx.c

    MODULE NAME:    access_EEPROM - edit EEPROM information

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to modify EEPROM data.

    CALLING
	SEQUENCE:   access_EEPROM(c_blk)
		    where:
			c_block (structure pointer)
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


void access_EEPROM(c_blk)
struct sblk445 *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;           /* menu item variable */
    int i, j, cycle;
    ULONG VPDData;

/*
    ENTRY POINT OF ROUTINE:
*/

skipit:
    while(1)
    {
	printf("\n\nAccess VPD EEPROM\n\n");
	printf(" 1. Return to Previous Menu\n");
	printf(" 2. Read EEPROM\n");
	printf(" 3. Write EEPROM\n");
	printf("\nselect: ");
	scanf("%d",&item);

	switch(item)
	{

	case 1: /* return to previous menu */
		return;
	    break;

	case 2: /* read */
		cycle = 0;
	    break;

	case 3: /* write */
		cycle = 1;
	    break;
	}

        printf("\nEnter quad byte aligned EEPROM address in hex: ");
	scanf("%lx",&i);
	i &= 0x000000FC;		/* limit & align address */

	if( cycle )
	{
          printf("\nEnter EEPROM data value in hex: ");
          scanf("%lx",&VPDData);
          printf("\n\nYour selection will cause a write into EEPROM!");
          printf("\nIncorrect values written to EEPROM may cause the board to stop functioning.");
          printf("\nIf this happens the board will have to be returned to the factory.");
          printf("\n\nAre you sure? 1=yes 2=no ");
          scanf("%x",&j);
	  if( j != 1)
	  {
	     printf("\n>>> Operation aborted. <<<");
	     goto skipit;
	  }
	}

	AccessVPD( c_blk->nHandle,
			i,		/* Address */
			&VPDData,	/* place to get or put from */
			cycle);		/* Cycle write=1 read=0 */

	if( cycle == 0 )
	    printf("Quad byte aligined address: %02lX     EEPROM data: %08X", i, VPDData);

    }
}
