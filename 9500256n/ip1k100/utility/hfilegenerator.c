
/* #include "stdafx.h" */
#include <stdio.h>
#include <ctype.h>
#include <string.h>     /* memset Required for FC4 */

/*
{+D}
        SYSTEM:         Acromag IP1k1x0 Digital I/O Board

        FILENAME:       HFileGenerator.c

        VERSION:        3.0

        CREATION DATE:  08/15/01

        DESIGNED BY:    JB & FM

        CODED BY:       JB & FM

        ABSTRACT:       This module is used to create a '.h' source
	                file that contains the data parsed from a
			'Intel.hex' file. This file can be used to
			"compile in" the configuration data for the PLD.
			The user then writes the data to the board.

        CALLING
            SEQUENCE:   status = GenHFile( char *namebuf1, char *namebuf2)

        MODULE TYPE:    integer

        I/O
          RESOURCES:

        SYSTEM
          RESOURCES:

        REVISIONS:

  DATE      BY     PURPOSE
---------  ----   -------------------------------------------------------
09/27/05   FJM    Fedora core FC4 update.

{-D}
*/

#define MAX_BUF_SIZE 	1024

#define MAX_HEXFILE_LENGTH		345000.0

int GenHFile( namebuf1, namebuf2)

char *namebuf1;
char *namebuf2;

{
/*
         Declare local data areas
*/

int status;  
int i;  
int len;
int type;
long addr;
int temp;
int Checksum;
long byte_count;
long nSegment;
int pct_complete;
char byte_string[5];
char HexStringBuf[MAX_BUF_SIZE];  
char *pHexString; 
FILE *fptr, *foptr;
 
/*
        Entry point of routine
*/

 status = 0;
 byte_count = -1;
 len = 0;
 addr = 0;
 nSegment = 0;
 pct_complete = 0;

 foptr = fopen( namebuf2, "w" );/* open '.h' output file for write */ 
 if( foptr == 0 )
     return((int)-2);           /* could not open output file */

 fptr = fopen( namebuf1, "r" ); /* open hex file for read */ 
 if( fptr == 0 )
   return((int)-1);         	/* could not open file error */
 else 
 {
   fprintf(foptr, "\n/* Data array for IP1K1X0 */\n\nunsigned char IP1K1X0[ ] = {\n");

   printf("\n  Percent Complete:  %2.2d\n", pct_complete);
   while (status == 0)
   {
     memset( HexStringBuf, 0, sizeof(HexStringBuf));
     memset( byte_string, 0, sizeof(byte_string));

     for(i = 0; i < MAX_BUF_SIZE ; i++ )	 /* read complete line */
     {
       HexStringBuf[i] = fgetc(fptr);
       if(HexStringBuf[i] == EOF || HexStringBuf[i] == '\n')
          break;
     }
     HexStringBuf[i] = 0;
     /* calc checksum */
     for(Checksum = 0, i = 1; i < (int)(strlen(HexStringBuf) - 3); i++ )
     {
       byte_string[0] = HexStringBuf[i];
       byte_string[1] = HexStringBuf[++i];
       sscanf(byte_string,"%X",&temp);
       Checksum += temp;
     }
     /* get transmitted checksum to temp */
     byte_string[0] = HexStringBuf[i++];
     byte_string[1] = HexStringBuf[i];
     sscanf(byte_string,"%X",&temp);

     Checksum = ((~Checksum) + 1) & 0xFF;	/* only an 8 bit sum */
     /* checksums match? */
     if( Checksum != temp ) 
     {
       printf("\nBad Checksum...  Calc = %x Read = %x\n", Checksum, temp );
       status = 1; /* Bad checksum */
       break;
     }

     pHexString = &HexStringBuf[0]; /* pointer into string */
     if(*pHexString++ != ':')       /* offset 0 */
     {
       status = 2; /* Record not Contiguous */
       break;
     }

     /* Determine Record Length */
     byte_string[0] = *pHexString++;/* offset 1 */
     byte_string[1] = *pHexString++;
     sscanf(byte_string,"%X",&len);

     /* Determine the Address Range of current record */
     byte_string[0] = *pHexString++;/* offset 3 */
     byte_string[1] = *pHexString++;
     byte_string[2] = *pHexString++;
     byte_string[3] = *pHexString++;
     sscanf(byte_string,"%X",&addr);

     /* Determine Record Type */
     memset( byte_string, 0, sizeof(byte_string));
     byte_string[0] = *pHexString++;/* offset 7 */
     byte_string[1] = *pHexString++;
     sscanf(byte_string,"%X",&type);

     if((len + addr == 0) && (type == 1))        /* last line of file? */
       goto we_are_done; 

     if(type == 4)  /* Check to see if it is a new segment record */
     {
        /* Get 16 bit Segment number */
		byte_string[0] = *pHexString++;/* offset 9 */
		byte_string[1] = *pHexString++;
		byte_string[2] = *pHexString++;
		byte_string[3] = *pHexString++;
		sscanf(byte_string,"%X",&nSegment);
	 }

     /* Add the lsw and msw of the address*/
     addr = addr + nSegment*0x10000;

     /* Scan the remaining bytes of each record. (Data section) */
     memset( byte_string, 0, sizeof(byte_string));
     for(i = 0; i < len; i++)
     {
       if(type != 4) /* records of this type have already been read from the file */
       {
         byte_string[0] = *pHexString++;
         byte_string[1] = *pHexString++;
       }
       if(type == 0)	/* data records */
       {
         if( byte_count != -1 )			/* skip comma the first time */
             fprintf(foptr, ",");
        
         if( (byte_count++ & 0x1f) == 0x1f)
             fprintf(foptr, "\n");

         /* Write data byte to output file */
         sscanf(byte_string,"%X",&temp);
         fprintf(foptr, "0x%X",(unsigned char)temp);

/* The user could modify this routine to write the data
   stream directly to the Ip1k1x00 module as follows:

   1. Bit 0 of the control register must be set to put the module into
      configuration mode. After bit 0 has been set, poll bit 0 until it
      reads as a logic high.  This may take up to 5uS after the
      configuration mode write.
      Note: This only needs to be done once at the start of the routine.

   2. Bit 1 of the control register monitors the status of the Altera PLD
      during configuration.  The status signal must remain high during the
      configuration.  Poll the status register, if bits 1 & 0 are both high,
      a program byte may now be written to the configuration data register.

   Example fragmment:

         c = input_word(&brd_ptr->control); read the control register of the Ip1k1x00 module
         c &= 0xf;                          only LS nibble needed
         if(c != 3)                         should now be 3 
             break;                         error exit write loop

         output_word(&brd_ptr->config_data, (word)databyte); write byte to configuration data register
*/

         if((byte_count & 0x7ff) == 0x7ff) 
         {  /* approx one out of 2000 loops will make it here */
/*            pct_complete = (int)((((float)byte_count) / MAX_HEXFILE_LENGTH	) * 100.0);
            printf("\b\b%2.2d", pct_complete);
*/
            printf(".");
            fflush(stdout);
         }
       }
     }
   }
 }

we_are_done:
 fprintf(foptr, "\n}; /* End data array for IP1K1X0 */\n");
 fclose(foptr);
 fclose(fptr);

 if( status == 0)
 {
   pct_complete = 100;
   printf("\n  Percent Complete:  %2.2d\n", pct_complete);
   fflush(stdout);
 }
 return(status);
}


int main(argc, argv)

int argc;
char *argv[];

{

char namebuf1[80];
char namebuf2[80];

  printf("\n\nHFileGenerator.exe V3.0 - Copyright 2001 - 2004, Acromag, Inc.\n");

  if(argc != 3)
  {
     printf("\nGenerates 'C' style .h output file from an Intel.hex input file");
     printf("\n\nSYNTAX: HFileGenerator <InputFilename.hex> <OutputFilename.h>\n\n\n");
  }
  else
  {
     sscanf(*++argv,"%s",namebuf1);	/* get source filename */
     sscanf(*++argv,"%s",namebuf2);	/* get destination filename */
     return(GenHFile(namebuf1,namebuf2));
  }
  return(-1);
}
