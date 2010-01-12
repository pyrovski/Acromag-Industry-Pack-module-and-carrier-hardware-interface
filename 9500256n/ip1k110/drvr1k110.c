

#include "../carrier/apc8620.h"
#include "ip1k110.h"


#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */


/*
{+D}
    SYSTEM:         Software for IP1k110

    FILENAME:       drvr1k110.c

    MODULE NAME:    main - main routine of example software.

    VERSION:        B

    CREATION DATE:  10/01/01

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       This module is the main routine for the example program
                    which demonstrates how the IP1k110 Library is used.

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
03/16/05 FJM    Added enhanced carrier API functions


{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

	This module is the main routine for the example program
	which demonstrates how the IP1k110 Library is used.
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
    int hflag;           /* interrupt handler installed flag */
    struct sblk1k110 s_block; /* allocate status param. blk */
    struct cblk1k110 c_block; /* configuration block */
    struct handler_data hdata;/* interrupt handler data structure */
    int pld_flag;        /* PLD in config mode = 0, in user mode = 1 */
    char mod_buff[10];   /* model number string */

/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/
 
    pld_flag = 0;     /* PLD in config mode = 0 */
    flag = 0;         /* indicate board address not yet assigned */
    finished = 0;     /* indicate not finished with program */
    hflag = 0;        /* indicate interrupt handler not installed yet */
    hstatus = 0;

/*
    Initialize the Configuration Parameter Block to default values.
*/

    memset( &c_block, 0, sizeof(struct cblk1k110));

    c_block.vector = VECTOR;       /* interrupt vector */
    c_block.bCarrier = FALSE;      /* indicate no carrier initialized and set up yet */
    c_block.bInitialized = FALSE;  /* indicate not ready to talk to IP */
    c_block.slotLetter = SLOT_A;

    s_block.model = 0;             /* model unselected */
    s_block.direction = 0;         /* direction */
    s_block.int_status = 0;        /* pending interrupts to clear */
    s_block.enable = 0;            /* interrupt enable (per bit) */
    s_block.polarity = 0;          /* interrupt polarity */
    s_block.type = 0;              /* interrupt type */
    s_block.vector = 0;

    hdata.h_pid = getpid();    /* save it in the interrupt handler data structure */
    hdata.hd_ptr = (char *)&c_block;/* put in address of c_block structure also */
    c_block.sblk_ptr = (struct sblk1k110*)&s_block;

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
    if(CarrierOpen(0, &c_block.nHandle) != S_OK)
    {
	printf("\nUnable to Open instance of carrier.\n");
	finished = 1;	 /* indicate finished with program */
    }
    else
	flag = 1;

    strcpy(&mod_buff[0],"Ip1k1xx");	/* generic model number string */

/*
    Enter main loop
*/      

    while(!finished) {

	printf("\n\n%s Library Demonstration  Version A\n\n", &mod_buff[0]);
	printf(" 1. Exit this Program\n");
	printf(" 2. Set Carrier Base Address\n");
	printf(" 3. Set IP Slot Letter\n");
  	printf(" 4. PLD Configuration\n");
  	printf(" 5. Set Up Configuration Block Parameters\n");
	printf(" 6. Configure Board Command\n");
	printf(" 7. Read Status Command and ID\n");
	printf(" 8. Enable Interrupts\n");
	printf(" 9. Disable Interrupts\n");
	printf("10. Read Input Point\n");
	printf("11. Read Input Port\n");
	printf("12. Write Output Point\n");
	printf("13. Write Output Port\n");

	printf("\nSelect: ");
	scanf("%ld",&item);

    if( ( item == 6 || item > 7) && pld_flag == 0 ) /* still in configuration mode */
	{
           printf("Please Configure PLD Now.\n");
           item = 0;
	}
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
		  SetCarrierAddress(c_block.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
                  SetCarrierAddress(c_block.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block.nHandle) == S_OK)/* Initialize Carrier */
		  {
		        c_block.bCarrier = TRUE;
			SetInterruptLevel(c_block.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;

				
	    }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
	    break;

	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block.bCarrier = FALSE;
			}
			else
			{
				c_block.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block.nHandle, c_block.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block.bInitialized = FALSE;
				}
				else	              
				{	
			                c_block.brd_ptr = (struct map1k110 *)addr;
					c_block.bInitialized = TRUE;
				}
			}
		}
	break;

	case 4: /* PLD configuration */
          if(!c_block.bInitialized)
		 printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
          else
          {
			rsts1k110(&c_block); /* get board's I.D. info into structure */
			switch( c_block.sblk_ptr->id_prom[5] )
			{
			case 0x44:
				pld_flag = 1;		/* do not ask to configure PLD for Ip1k125 */
				printf("\nModel Ip1k125 detected... Skipping PLD configuration.\n");
				strcpy(&mod_buff[0],"Ip1k125");	/* model number string */
			break;

			case 0x42:
			case 0x43:
				printf("\nModel Ip1k110 detected... Now configuring PLD.\n");
				strcpy(&mod_buff[0],"Ip1k110");	/* model number string */
				if( PLDConfig1k110( &c_block) == 0)
				    pld_flag = 1;	/* Ip1k110 PLD in user mode = 1 */
			break;

			case 0x48:
			case 0x49:
				printf("\nModel IP-EP20x detected... Now configuring PLD.\n");
				strcpy(&mod_buff[0],"Ip-EP20X");	/* model number string */
				if( PLDConfig1k110( &c_block) == 0)
				    pld_flag = 1;	/* Ip1k110 PLD in user mode = 1 */
			break;

			default:
				printf("\nUnrecognized Model I.D. Aborting PLD configuration.\n");
				strcpy(&mod_buff[0],"");	/* step on model number string */
			    pld_flag = 0;				/* config mode = 0 */
			break;
			}			  
         }
	break;

	case 5: /* set up configuration block parameters */

	    scfg1k110(&c_block);
	    break;

	case 6:     /* configure board command */
	
            if(!c_block.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
/*
    Check for pending interrupts and check the
    "interrupt handler attached" flag.  If interrupts are pending or
    if interrupt handlers are not attached, then print an error message.
    If both conditions were false, then go ahead and execute the command.
*/

		if( input_word( c_block.nHandle, (word*)&c_block.brd_ptr->sts_reg ) && 0x00FF )
		      printf(">>> ERROR: INTERRUPTS ARE PENDING <<<\n");
		else
		{
		   if( hflag == 0 && ( c_block.enable ))
			printf(">>> ERROR: INTERRUPT HANDLER NOT ATTACHED <<<\n");
		   else
		        cnfg1k110(&c_block); /* configure the board */
		}
	    }
	    break;

	case 7:     /* read board status command */
	
            if(!c_block.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
		psts1k110(&c_block); /* read board status */
	    break;

	case 8:     /* enable interrupts */
		EnableInterrupts( c_block.nHandle );
		hflag = 1;
        break;

	case 9: /* detach exception handlers */
		hflag = 0;
		DisableInterrupts(c_block.nHandle);
	 break;

	case 10: /* Read Digital Input Point */

            if(!c_block.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Input port number   (0 - 2): ");
		scanf("%d",&port);
		printf("\nEnter Input point number (0 - 15): ");
		scanf("%d",&point);
		status = rpnt1k110(&c_block,port,point);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
		else
		    printf("\nValue of port %d point %d: %lX\n",port,point,status);
	    }
	    break;

	case 11: /* Read Digital Input Port */

            if(!c_block.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Input port number  (0 - 2):  ");
		scanf("%d",&port);
		status = rprt1k110(&c_block,port);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
		else
		    printf("\nValue of port %d: %lX\n",port,status);
	    }
	    break;


	case 12: /* Write Digital Point */

            if(!c_block.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Output port number (0 - 2):  ");
		scanf("%d",&port);
		printf("\nEnter I/O point number  (0 - 15): ");
		scanf("%d",&point);
		printf("\nEnter point value (0 - 1): ");
		scanf("%x",&val);
		status = wpnt1k110(&c_block,port,point,val);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
	    }
	    break;

	case 13: /* Write Digital Port */

            if(!c_block.bInitialized)
		printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
	    else
	    {
		printf("\nEnter Output port number (0 - 2):  ");
		scanf("%d",&port);
		printf("\nEnter output value in hex (0000 - FFFF): ");
		scanf("%x",&val);
		status = wprt1k110(&c_block,port,val);
		if(status == -1)
		    printf("\n>>> ERROR: PARAMETER OUT OF RANGE <<<\n");
	    }
	    break;

		case 0:  /* The following is an example of how to address the memory space on an IP module. */
		ip_mem_ram_test(&c_block);
	    break;

	}   /* end of switch */
    }   /* end of while */

/*
    disable interrupts from IP module
*/
    if(pld_flag)            		/* PLD was configured */
    {
      c_block.param = 0xFF;		/* parameter mask */
      c_block.int_status = 0xff;        /* pending interrupts to clear */
      c_block.enable = 0;               /* interrupt enable (per bit) */
      cnfg1k110(&c_block);		/* configure the board */
    }


    DisableInterrupts(c_block.nHandle);
    if(c_block.bCarrier)
	CarrierClose(c_block.nHandle);

    printf("\nEXIT PROGRAM\n");

}   /* end of main */



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr1k110.c

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



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr1k110.c

    MODULE NAME:    scfg1k110 - set configuration block contents.

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to enter parameters into
		    the Configuration Block.
 
    CALLING
	SEQUENCE:   scfg1k110(c_block)
		    where:
			c_block (structure pointer)
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

void scfg1k110(c_blk)

struct cblk1k110 *c_blk;
{

/*
    DECLARE LOCAL DATA AREAS:
*/
    int item;                       /* menu item variable */
    unsigned finished;		    /* flags to exit loops */
    int i;                          /* loop index */
    int bvalue;					/* storage for retreived data */
    word speed;
    int status;

/*
    ENTRY POINT OF ROUTINE:
*/
    finished = 0;
    while(!finished)
    {
		printf("\n\nConfiguration Block Parameters\n\n");

		printf(" 1. Return to Previous Menu\n");
		printf(" 2. Board Pointer:  %08lX\n",(unsigned long)c_blk->brd_ptr);
		printf(" 3. Parameter Mask:   %04X\n",c_blk->param);
		printf(" 4. Select Model:       %02X\n",c_blk->model);
		printf(" 5. Int. Status:        %02X\n",c_blk->int_status);
		printf(" 6. Int. Enable:        %02X\n",c_blk->enable);
		printf(" 7. Int. Polarity:      %02X\n",c_blk->polarity);
		printf(" 8. Int. Type:          %02X\n",c_blk->type);
		printf(" 9. Data Direction:    %03X\n",c_blk->direction);
		printf("10. Int. Vector:        %02X\n",c_blk->vector);
		printf("11. Module Reset:       %02X\n",c_blk->reset);
		status = GetIPClockControl(c_blk->nHandle, c_blk->slotLetter, &speed);
		printf("12. Carrier Clock Speed:%02X\n",speed);
   
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

		case 4: /* Select Model */
		    c_blk->model = (word)get_param();
		break;

		case 5: /* interrupt status register */
		    c_blk->int_status = (word)get_param();
	 	break;

		case 6: /* interrupt enable reg. */
		    c_blk->enable = (word)get_param();
		break;

		case 7: /* interrupt polarity reg. */
		    c_blk->polarity = (word)get_param();
		break;

		case 8: /* interrupt type reg. */
		    c_blk->type = (word)get_param();
		break;

		case 9: /* direction reg. */
		    c_blk->direction = (word)get_param();
		break;

		case 10: /* exception vector number */
		    c_blk->vector = (byte)get_param();
		break;

		case 11: /* reset flag */
		    c_blk->reset = (byte)get_param();
		break;
		
		case 12: /* Ip clock select */
		    printf("0 - 8MHZ IP Clock From Carrier\n");
		    printf("1 - 32MHZ IP Clock From Carrier\n\n");
		    bvalue = (int)get_param();

		    /* call carrier API to get slot clock speed info */
		    status = GetIPClockControl(c_blk->nHandle, c_blk->slotLetter, &speed);
		    if( status )
			printf("\nSorry... Carrier can not change the IP clock setting\n");
		    else   /* call carrier API to set slot clock speed */
			SetIPClockControl(c_blk->nHandle, c_blk->slotLetter, bvalue);
		break;
	}
    }
}



/*
{+D}
    SYSTEM:         Software

    FILENAME:       drvr1k110.c

    MODULE NAME:    psts1k110 - print board status information

    VERSION:        A

    CREATION DATE:  05/19/98

    DESIGNED BY:    FJM

    CODED BY:       FJM

    ABSTRACT:       Routine which is used to cause the "Read Board Status"
		    command to be executed and to print out the results to
		    the console.

    CALLING
	SEQUENCE:   psts1k110(&c_block)
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


void psts1k110(c_blk)
struct cblk1k110 *c_blk;
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
	rsts1k110(c_blk); /* get board's status info into structure */
	printf("\n\nBoard Status Information");
	printf("\nModel:                        %02X",c_blk->sblk_ptr->model);
	printf("\nData Direction Register:     %03X",c_blk->sblk_ptr->direction );
	printf("\nInterrupt Status Register:    %02X",c_blk->sblk_ptr->int_status);
	printf("\nInterrupt Enable Register:    %02X",c_blk->sblk_ptr->enable);
	printf("\nInterrupt Polarity Register:  %02X",c_blk->sblk_ptr->polarity );
	printf("\nInterrupt Type Register:      %02X",c_blk->sblk_ptr->type );
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





/*
	The following is an example of how to address the memory space on an IP module.
*/



int ip_mem_ram_test(c_blk)

struct cblk1k110 *c_blk;

{

/*
         Declare local data areas
*/

char buf[256];                          /* input/output buffer */
int status;                             /* routine return error status */
int c,l; 								/* loop counters */
unsigned short dut_data;                /* device under test data */
unsigned char bdut_data;
unsigned char *test_address;            /* address to test at */
unsigned short *s_address;              /* address to test at */
unsigned short *w_address;              /* address to write to */
unsigned short s_data;                  /* device under test data */
unsigned short s_mask;                  /* write pattern */
unsigned char *address1;                /* I/O address of card */
unsigned char *address2;                /* MEM address of card */
long flag;								/* general flag for exiting loops */


/*
        Entry point of routine
*/

    GetCarrierID(c_blk->nHandle, &s_mask);	/* see if carrier can support memory I/O */
    if( ( s_mask & CARRIER_MEM ) == 0)
    {
	printf("\nSorry... this carrier does not support memory I/O\n");
	return( -6);
    }

    flag = 1;         /* indicate memory address not yet assigned */
    do 
    {
	if(flag == 0)
	{
	  printf("\n\nenter carrier memory base address in hex: ");
	  scanf("%lx",&address2);
/*
	Set Carrier Memory Address
	Get standard address space mappings for VME bus from VxWorks config.h
	Set the standard address space for VME bus
*/
	  SetMemoryAddress( c_blk->nHandle, (long)address2);	/* Set Carrier Memory Address */
	}
	GetMemoryAddress(c_blk->nHandle, (long*)&address2);
	printf("Memory Address: %lX\n",address2);
	printf("is this value correct(y/n)?: ");
	scanf("%s",buf);
	if( buf[0] == 'y' || buf[0] == 'Y' )
	{
	  SetMemoryAddress( c_blk->nHandle, (long)address2);	/* Set Carrier Memory Address */
	  flag = 1;
	}
	else
	  flag = 0;

    }while( buf[0] != 'y' && buf[0] != 'Y' );

   /* to get the IO space address of this IP slot */
   GetIpackAddress(c_blk->nHandle, c_blk->slotLetter, (long*)&address1);

   /* Get the MEM IO address space for this IP slot */
   GetIpackMemoryAddress(c_blk->nHandle, c_blk->slotLetter, (long*)&address2);

   status = 0;
   test_address = (unsigned char *) address2;
   w_address = (unsigned short *) address2;
   s_address = (unsigned short *) address2;

/*
        Clear memory
*/

   for(l = 0; l < 1024; l++)           /* step to next word */
      output_word(c_blk->nHandle, (word*)&w_address[l], 0);   /* clear IP memory */


   for(l = 0; l < 1024; l++)           /* check for 0 */
   {
     dut_data = input_word(c_blk->nHandle, (word*)&s_address[l]);
     if(dut_data != 0)
     {
       status = -1;
       sprintf(buf,"FAIL\nIP%c memory %X read %04X  Expected 0000 ",c_blk->slotLetter,s_address+l,dut_data);
       printf(buf);
     }
   }

/*
        Fill with 0x010A
*/

   if(status == 0)
   {
     for(l = 0; l < 1024; l++)
       output_word(c_blk->nHandle, (word*)&w_address[l], 0x010A);

     for(l = 0; l < 1024; l++)         /* check for 010A */
     {
       dut_data = input_word(c_blk->nHandle, (word*)&s_address[l]);
       if(dut_data != 0x010A)
       {
         status = -2;
         sprintf(buf,"FAIL\nIP%c memory %X read %02X  Expected 010A ",c_blk->slotLetter,s_address+l,dut_data);
         printf(buf);
       }
     }
   }

/*
        Fill with 0x0205
*/

   if(status == 0)
   {
     for(l = 0; l < 1024; l++)
        output_word(c_blk->nHandle, (word*)&w_address[l], 0x0205);

     for(l = 0; l < 1024; l++)         /* check for 0x0205 */
     {
       dut_data = input_word(c_blk->nHandle, (word*)&s_address[l]);
       if(dut_data != 0x0205)
       {
         status = -3;
         sprintf(buf,"FAIL\nIP%c memory %X read %02X  Expected 0205 ",c_blk->slotLetter,s_address+l,dut_data);
         printf(buf);
       }
     }
   }

   if(status == 0)
     printf("OK!");
   else
     printf("\n");

   return(status);
}


