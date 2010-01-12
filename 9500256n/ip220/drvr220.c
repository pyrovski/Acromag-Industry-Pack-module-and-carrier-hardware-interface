
#include "../carrier/apc8620.h"
#include "ip220.h"


#define INTERRUPT_LEVEL 5   /* level or bus IRQ - may not be used on all systems */
#define VECTOR 192          /* interrupt vector - may not be used on all systems */



/*
{+D}
    SYSTEM:         IP220 Software

    FILENAME:       drvr220.c

    MODULE NAME:    main - main routine of example software.

    VERSION:        REV A

    CREATION DATE:  05/19/98

    DESIGNED BY:    F.J.M.

    CODED BY:       F.J.M.

    ABSTRACT:       This module is the main routine for the example program
		    which demonstrates how the IP220 Library is used.

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
        which demonstrates how the IP220 Library is used.
*/

int main()
{

/*
    DECLARE LOCAL DATA AREAS:
*/

    char cmd_buff[32];		/* command line input buffer */
    unsigned finished;		/* flag to exit program */
    int item;			/* menu item selection variable */
    unsigned finished2;		/* flag to exit a loop */
    long addr;			/* board address */
    int flag;			/* general flag for exiting loops */
    int i;			/* loop index */
    int temp;
    int channel;
    float zero, span;		/* storage for zero and span info */
    char o, g;			/* storage for temp offset/gain info */
    struct cblk220 c_block220;    /* configuration block */

    char off_data[A_SIZE];	/* allocate data storage area */
    char gain_data[A_SIZE];	/* allocate data storage area */
    word ideal_data[A_SIZE];	/* allocate data storage area */
    word cor_data[A_SIZE];	/* allocate data storage area */

/*
    ENTRY POINT OF ROUTINE:
    INITIALIZATION
*/

    flag = 0;		 /* indicate board address not yet assigned */
    finished = 0;	 /* indicate not finished with program */

    for(i = 0; i < A_SIZE;i++)
    {
	off_data[i] = 0;		/* clear offset buffer */
	gain_data[i] = 0;		/* clear gain buffer */
	ideal_data[i] = 0;		/* clear ideal buffer */
	cor_data[i] = 0;		/* clear corrected buffer */
    }


/*
    Initialize the Configuration Parameter Block to default values.
*/

    c_block220.mode = TM;			/* mode */
    c_block220.bit_constant = CON12;	/* constant for data correction */
    c_block220.ideal_buf = &ideal_data[0];	/* ideal value */
    c_block220.off_buf = &off_data[0];	/* offset buffer start */
    c_block220.gain_buf = &gain_data[0];	/* gain buffer start */
    c_block220.cor_buf = &cor_data[0];	/* corrected */
    c_block220.bCarrier = FALSE;		/* indicate no carrier initialized and set up yet */
    c_block220.bInitialized = FALSE;	/* indicate not ready to talk to IP */
    c_block220.slotLetter = SLOT_A;
    c_block220.nHandle = 0;		/* make handle to a closed carrier board */

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
    if(CarrierOpen(0, &c_block220.nHandle) != S_OK)
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
      printf("\nIP220 Library Demonstration  Rev. A\n");
      printf(" 1. Exit this Program\n");
      printf(" 2. Set Board Base Address\n");
      printf(" 3. Set IP Slot Letter\n");
      printf(" 4. Transparent Mode Select\n");
      printf(" 5. Simultaneous Mode Select\n");
      printf(" 6. Simultaneous Trigger\n");
      printf(" 7. Read Calibration Coefficients and I.D.\n");
      printf(" 8. Write Ideal Data To Output\n");
      printf(" 9. Write Corrected Data To Output\n");
      printf("10. Display Ideal/Corrected Data, Offset/Gain Coefficients\n");
      printf("11. Clear All Data Buffers\n");
      printf("12. Alter Offset/Gain Coefficients\n");
      printf("Select: ");
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
		  SetCarrierAddress(c_block220.nHandle, addr);	/* Set Carrier Address */
		}
		GetCarrierAddress(c_block220.nHandle, &addr);	/* Read back carrier address */
		printf("address: %lX\n",addr);
		printf("is this value correct(y/n)?: ");
		scanf("%s",cmd_buff);
		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
		{
          SetCarrierAddress(c_block220.nHandle, addr);	/* Set Carrier Address */
		  if(CarrierInitialize(c_block220.nHandle) == S_OK)/* Initialize Carrier */
		  {
		        c_block220.bCarrier = TRUE;
			SetInterruptLevel(c_block220.nHandle, INTERRUPT_LEVEL);/* Set carrier interrupt level */
		  }
		  flag = 1;
		}
		else
		  flag = 0;
				
	    }while( cmd_buff[0] != 'y' && cmd_buff[0] != 'Y' );
            break;

	case 3: /* set IP Slot Letter */
		if(flag == 0 || c_block220.bCarrier == FALSE)
			printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
		else
		{
			printf("\n\nEnter IP slot letter (A, B etc...): ");
			scanf("%s",cmd_buff);
			cmd_buff[0] = toupper(cmd_buff[0]);
			if(cmd_buff[0] < 'A' || cmd_buff[0] > GetLastSlotLetter())
			{
				printf("\nInvalid Slot Letter!\n");
				c_block220.bCarrier = FALSE;
			}
			else
			{
				c_block220.slotLetter = cmd_buff[0];
																					
/*
	Get the IPACK's base address based on the Slot letter,
        and initialize the IPACK's data structure with the returned address
*/
				if(GetIpackAddress(c_block220.nHandle, c_block220.slotLetter, &addr) != S_OK)
				{
					printf("\nUnable to Get Ipack Address.\n");
					c_block220.bInitialized = FALSE;
				}
				else	              
				{	
			                c_block220.brd_ptr = (struct map220 *)addr;
					c_block220.bInitialized = TRUE;
				}
			}
		}
	break;

        case 4:				/* Transparent Mode Select */
            if(!c_block220.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
	      c_block220.mode = TM;	/* mode */
              mode_select220(&c_block220);
	    }
            break;

        case 5:				/* Simultaneous Mode Select */
            if(!c_block220.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
	      c_block220.mode = SM;	/* mode */
              mode_select220(&c_block220);
	    }
            break;

        case 6:				/* Simultaneous Output Trigger */
            if(!c_block220.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
	      c_block220.mode = SMOT;	/* mode */
              mode_select220(&c_block220);
	    }
            break;

        case 7: /* Read Calibration Coefficients and I.D. */
            if(!c_block220.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	      readstat220(&c_block220);	/* read and display status */

            break;

        case 8: /* Write Ideal Data To Output */
            if(!c_block220.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
gloop1:
		printf("\nEnter channel number (HEX)      : ");
		scanf("%x", &channel);
                if(channel > 16 || channel < 0)
		  goto gloop1;

		printf("\nEnter right justified data (HEX): ");
		scanf("%x", &temp);
		ideal_data[channel] = (word)temp;
		wro220(&c_block220, channel, (ideal_data[channel] << 4));
	    }
            break;

        case 9: /* Write Corrected Data To Output */
            if(!c_block220.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
gloop2:
		printf("\nEnter channel number (HEX)      : ");
		scanf("%x", &channel);
                if(channel > 16 || channel < 0)
		  goto gloop2;

		printf("\nEnter right justified data (HEX): ");
		scanf("%x", &temp);
		ideal_data[channel] = (word)temp;

		cd220(&c_block220, channel);	/* correct data */

		/* Check for end points - can not correct data for end points */

		if(ideal_data[channel] == 0 || ideal_data[channel] == 0xFFF)
		    cor_data[channel] = ideal_data[channel];

		wro220(&c_block220, channel, (cor_data[channel] << 4));
	    }
            break;

        case 10:	/* Display Ideal/Corrected Data, Offset/Gain Coefficients */
	    printf("\n  ");
	    for(i = 0; i != 8; i++)
	      printf("    CH %02d",i);

	    printf("\nID");
	    for(i = 0; i != 8; i++)
	      printf("     %04X",ideal_data[i]);

	    printf("\nCD");
	    for(i = 0; i != 8; i++)
	      printf("     %04X",cor_data[i]);

	    printf("\nGD");
	    for(i = 0; i != 8; i++)
	      printf("       %02X",(gain_data[i] & 0xFF));

	    printf("\nOD");
	    for(i = 0; i != 8; i++)
 	      printf("       %02X",(off_data[i] & 0xFF));

	    printf("\n\n  ");

	    for(i = 8; i != 16; i++)
	      printf("    CH %02d",i);

	    printf("\nID");
	    for(i = 8; i != 16; i++)
	      printf("     %04X",ideal_data[i]);

	    printf("\nCD");
	    for(i = 8; i != 16; i++)
	      printf("     %04X",cor_data[i]);

	    printf("\nGD");
	    for(i = 8; i != 16; i++)
	      printf("       %02X",(gain_data[i] & 0xFF));

	    printf("\nOD");
	    for(i = 8; i != 16; i++)
 	      printf("       %02X",(off_data[i] & 0xFF));

            break;

        case 11:        /* clear all data buffers */
	    for(i = 0; i < A_SIZE;i++)
	    {
		off_data[i] = 0;	/* clear offset buffer */
		gain_data[i] = 0;	/* clear gain buffer */
		ideal_data[i] = 0;	/* clear ideal buffer */
		cor_data[i] = 0;	/* clear corrected buffer */
	    }
            break;

        case 12:        /* alter data buffers */
            if(!c_block220.bInitialized)
                printf("\n>>> ERROR: BOARD ADDRESS NOT SET <<<\n");
            else
	    {
	      finished2 = 0;
	      channel = 0;
	      while(!finished2)
	      {
	        printf("\n\nAlter Gain/Offset Coefficients\n\n");
	        printf("\nCurrent Channel Number:     %X",channel);
	        printf("\nCurrent Gain Coefficient:   %X", (gain_data[channel] & 0xFF));
	        printf("\nCurrent Offset Coefficient: %X\n\n", (off_data[channel] & 0xFF));

	        printf("1. Return to Previous Menu\n");
		printf("2. Change Gain Coefficient\n");
		printf("3. Change Offset Coefficient\n");
		printf("4. Change Channel Number\n");
		printf("5. Calculate New Gain/Offset Cofficient\n");
	        printf("\nselect: ");
	        scanf("%d",&item);

	        switch(item)
	        {
	          case 1: /* return to previous menu */
	            finished2++;
	          break;

	          case 2: /* get gain */
			printf("\nEnter coefficient (HEX)       : ");
			scanf("%x", &temp);
			gain_data[channel] = (char)temp;
	          break;

	          case 3: /* get offset */
			printf("\nEnter coefficient (HEX)       : ");
			scanf("%x", &temp);
			off_data[channel] = (char)temp;
	          break;

	          case 4: /* Select new channel */
			printf("\nEnter channel number (HEX)    : ");
			scanf("%x", &channel);
        	  break;

	          case 5: /* Cal channel */
			wro220(&c_block220, channel, 0);
			printf("\nEnter measured output value from DVM connected to channel %x: ",channel);
			scanf("%f",&zero);
			wro220(&c_block220, channel, 0xFFF0);
			printf("\nEnter measured output value from DVM connected to channel %x: ",channel);
			scanf("%f",&span);
			coef220(&zero, &span, (signed char *)&o, (signed char *)&g);	/* compute coefficients */
			printf("\nOffset Coefficient = %X",(o & 0xFF));
			printf("\nGain   Coefficient = %X",(g & 0xFF));
			printf("\n\nDo you wish to update the gain/offset data arrays Y or N : ");
			scanf("%s",cmd_buff);
            		if( cmd_buff[0] == 'y' || cmd_buff[0] == 'Y' )
			{
			  off_data[channel] = o;
			  gain_data[channel] = g;
			}
        	  break;
       	    	}
	      }
	    }
            break;
        }   /* end of switch */
    }   /* end of while */

    DisableInterrupts(c_block220.nHandle);
    if(c_block220.bCarrier)
	CarrierClose(c_block220.nHandle);

    printf("\nEXIT PROGRAM\n");

}   /* end of main */



/*
{+D}
    SYSTEM:         IP220 Software

    FILENAME:       drvr220.c

    MODULE NAME:    get_param - get a parameter from the console

    VERSION:        V1.0

    CREATION DATE:  05/19/98

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

    int temp;

/*
    print prompt string
*/  

    printf("enter parameter: ");
    scanf("%X",&temp);
    printf("\n");

    return((long)temp);
}


/*
{+D}
    SYSTEM:         IP320 Software

    FILENAME:       drvr220.c

    MODULE NAME:    readstat220 - read board calibration and I.D..

    VERSION:        V1.0

    CREATION DATE:  05/19/98

    DESIGNED BY:    F.J.M.

    CODED BY:       F.J.M

    ABSTRACT:       Routine which is used to "Read Calibration and I.D." and to
		    print the results to the console.

    CALLING
        SEQUENCE:   readstat220(&c_block220)
                    where:
                        c_block220 (structure pointer)
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

void readstat220(c_blk)
struct cblk220 *c_blk;
{

/*
    DECLARE EXTERNAL DATA AREAS:
*/


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
        rccid220(c_blk);
        printf("\n\nBoard Information\n\n");
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

        printf("\n\n1. Return to Previous Menu\n");
        printf("2. Read Again\n");
        printf("\nselect: ");
        scanf("%d",&item);

        switch(item){

        case 1: /* return to previous menu */
            finished++;
            break;
        }
    }
}
