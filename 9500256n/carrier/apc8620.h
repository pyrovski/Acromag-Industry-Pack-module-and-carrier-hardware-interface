
/*
{+D}
    SYSTEM:			Linux

    FILENAME:	    APC8620.h

    MODULE NAME:    Functions common to the APC8620 example software.

    VERSION:	    H

    CREATION DATE:  06/06/01

    DESIGNED BY:    FJM

    CODED BY:	    FJM

    ABSTRACT:       This file contains the definitions, structures
                    and prototypes for APC8620A.
                    The APC8620A is a enhanced version of the APC8620.
                    This library may be used with both enhanced (APC8620A)
                    and non-enhanced (APC8620) carriers.
                    Calling the enhanced functions will not harm existing
                    non-enhanced (APC8620) hardware.
                    The enhanced functions check to be sure of the carrier's
                    capabilities.
                    CarrierInitialize() must be called before calling any
                    enhanced function.
                    Enhanced functions will return "E_NOT_IMPLEMENTED" for
                    a non-enhanced (APC8620) carrier.

    CALLING
	SEQUENCE:

    MODULE TYPE:

    I/O RESOURCES:

    SYSTEM
	RESOURCES:

    MODULES
	CALLED:

    REVISIONS:

  DATE	  BY	    PURPOSE
-------  ----	------------------------------------------------
12/22/04 FJM    Fedora core FC3 update.
01/20/05 FJM    Extended carrier function library interface.
09/27/05 FJM    Fedora core FC4 update.
12/06/06 FJM    Removed handler_data from IP.h files - Moved handler_data here
01/11/07 FJM    Add support x86_64
04/01/09 FJM    blocking_start_convert
08/02/09 FJM    Add configuration register access.

{-D}
*/


/*
    MODULES FUNCTIONAL DETAILS:

	This file contains the definitions, structures and prototypes for APC8620.
*/



#ifndef BUILDING_FOR_KERNEL
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <inttypes.h>
/* Required for FC4 */
#include <stdlib.h>     /* malloc */
#include <string.h>     /* memset */
#endif /* BUILDING_FOR_KERNEL */

typedef int BOOL;
typedef uint32_t ULONG;
typedef unsigned char byte;	/*  custom made byte data type */
typedef unsigned short word;	/* custom made word data type */
typedef int CSTATUS;	/*  Custom made CSTATUS data type, used 
							as return value from the carrier functions. */

#define TRUE	1	/* Boolean value for true */
#define FALSE	0	/* Boolean value for false */

#define SLOT_A 	0x41	/* Value for slot A */
#define SLOT_B	0x42	/* Value for slot B */
#define SLOT_C 	0x43	/* Value for slot C */
#define SLOT_D	0x44	/* Value for slot D */
#define SLOT_E	0x45	/* Value for slot E */

#define SLOT_A_IO_OFFSET	 0x0180		/*  Slot A IO space addr. offset from carrier base addr. */
#define SLOT_A_ID_OFFSET	 0x0040		/*  Slot A ID space addr. offset from carrier base addr. */
#define SLOT_A_MEM_OFFSET	 0x0		/*  Slot A MEM space addr. */
#define SLOT_B_IO_OFFSET	 0x0200		/*  Slot B IO space addr. offset from carrier base addr. */
#define SLOT_B_ID_OFFSET	 0x0080		/*  Slot B ID space addr. offset from carrier base addr. */
#define SLOT_B_MEM_OFFSET	 0x00800000	/*  Slot B MEM space addr. */
#define SLOT_C_IO_OFFSET	 0x0280		/*  Slot C IO space addr. offset from carrier base addr. */
#define SLOT_C_ID_OFFSET	 0x00C0		/*  Slot C ID space addr. offset from carrier base addr. */
#define SLOT_C_MEM_OFFSET	 0x01000000	/*  Slot C MEM space addr. */
#define SLOT_D_IO_OFFSET	 0x0300		/*  Slot D IO space addr. offset from carrier base addr. */
#define SLOT_D_ID_OFFSET	 0x0100		/*  Slot D ID space addr. offset from carrier base addr. */
#define SLOT_D_MEM_OFFSET	 0x01800000	/*  Slot D MEM space addr. */
#define SLOT_E_IO_OFFSET	 0x0380		/*  Slot E IO space addr. offset from carrier base addr. */
#define SLOT_E_ID_OFFSET	 0x0140		/*  Slot E ID space addr. offset from carrier base addr. */
#define SLOT_E_MEM_OFFSET	 0x02000000	/*  Slot E MEM space addr. */

#define MAX_CARRIERS 4	/* maximum number of carriers */

#define SOFTWARE_RESET 0x0100		/*  Value to OR with control register to reset carrier */
#define TIME_OUT_INT_ENABLE 0x0008	/* IP access time out interrupt enable */
#define APC_INT_ENABLE	0x0004		/* IP module interrupt enable */
#define APC_INT_PENDING_CLEAR 0x0020	/* IP Module interrupt pending bit, clear interrupts */

#define IPA_INT0_PENDING 0x0001		/* IP A Int 0 Interrupt Pending bit */
#define IPA_INT1_PENDING 0x0002		/* IP A Int 1 Interrupt Pending bit */
#define IPB_INT0_PENDING 0x0004		/* IP B Int 0 Interrupt Pending bit */
#define IPB_INT1_PENDING 0x0008		/* IP B Int 1 Interrupt Pending bit */
#define IPC_INT0_PENDING 0x0010		/* IP C Int 0 Interrupt Pending bit */
#define IPC_INT1_PENDING 0x0020		/* IP C Int 1 Interrupt Pending bit */
#define IPD_INT0_PENDING 0x0040		/* IP D Int 0 Interrupt Pending bit */
#define IPD_INT1_PENDING 0x0080		/* IP D Int 1 Interrupt Pending bit */
#define IPE_INT0_PENDING 0x0100		/* IP E Int 0 Interrupt Pending bit */
#define IPE_INT1_PENDING 0x0200		/* IP E Int 1 Interrupt Pending bit */
#define TIME_OUT_PENDING 0x0400		/* Time out interrupt pending */

#define CARRIER_INT_MASK 0x3FF		/* pending mask */

							
/*	Carrier attributes */

#define VME_CARRIER			1 << 4		/* VME type carrier */
#define ISA_CARRIER			2 << 4		/* ISA type carrier */
#define PCI_CARRIER			3 << 4		/* PCI type carrier */

#define CARRIER_MEM			1 << 2		/* carrier supports memory space */
#define CARRIER_CLK			1			/* carrier supports 32MHz IP clocking */


/* 
	CSTATUS return values
	Errors will have most significant bit set and are preceded with an E_.
	Success values will be succeeded with an S_.
*/
#define ERROR 0x8000 /* general */
#define E_OUT_OF_MEMORY 	0x8001	/*  Out of memory status value */
#define E_OUT_OF_CARRIERS	0x8002	/*  All carrier spots have been taken */
#define E_INVALID_HANDLE	0x8003	/*  no carrier exists for this handle */
#define E_INVALID_SLOT		0x8004	/*  no slot available with this number */
#define E_NOT_INITIALIZED	0x8006	/*  carrier not initialized */
#define E_NOT_IMPLEMENTED   0x8007;	/*  Function is not implemented */
#define E_NO_INTERRUPTS 	0x8008;	/*  Carrier will be unable to handle interrupts */
#define S_OK			    0x0000	/*  Everything worked successfully */



/*
	APC8620	PCI Carrier information
*/
#define APC8620_VENDOR_ID 0x10b5	/* PLX vendor ID */
#define APC8620_DEVICE_ID 0x1024	/* Acromag's device ID */

/* 
	PCI Carrier data structure
*/

typedef struct
{
	int nHandle;					/* handle of this carrier structure */
	long lBaseAddress;				/* pointer to base address of carrier board */
	int nIntLevel;					/* Interrupt level of Carrier */
	int nInteruptID;				/* ID of interrupt handler */
	BOOL bInitialized;				/* Carrier initialized flag */
	BOOL bIntEnabled;				/* interrupts enabled flag */
	int nCarrierDeviceHandle;		/* handle to an open carrier device */
	word uCarrierID;				/* Carrier Identification value from open */
	long lMemBaseAddress;			/* pointer to base address of carrier board memory space */
	int nDevInstance;				/* Device Instance */
	char devname[64];				/* device name */
}CARRIERDATA_STRUCT;

typedef struct
{
	word controlReg;	/*  Status/Control Register */
	word intPending;	/*  Interrupt Pending Register */
	word slotAInt0;		/*  Slot A interrupt 0 select space */
	word slotAInt1;		/*  Slot A interrupt 1 select space */
	word slotBInt0;		/*  Slot B interrupt 0 select space */
	word slotBInt1;		/*  Slot B interrupt 1 select space */
	word slotCInt0;		/*  Slot C interrupt 0 select space */
	word slotCInt1;		/*  Slot C interrupt 1 select space */
	word slotDInt0;		/*  Slot D interrupt 0 select space */
	word slotDInt1;		/*  Slot D interrupt 1 select space */
	word slotEInt0;		/*  Slot E interrupt 0 select space */
	word slotEInt1;		/*  Slot E interrupt 1 select space */
	word IPClockControl;/* IP Clock Control Register */
}PCI_BOARD_MEMORY_MAP;


/*
    Defined below is the Interrupt Handler Data Structure.
    The interrupt handler is provided with a pointer that points
    to this structure.  From this the handler has a link back to
    its related process and common data area.  The usage of the members
    is not absolute.  Their typical uses are described below.
*/
struct handler_data
    {
    int h_pid;          /* Handler related process i.d. number.
                           Needed to know if handler is going to wake up,
                           put to sleep, or suspend a process.*/
    char *hd_ptr;       /* Handler related data pointer.
                           Needed to know if handler is going to access
                           a process' data area. */
    };


/* Interrupt service routine data structure */
struct isr_data					/* can be enlarged as required */
{
	unsigned long slot_io_address;	/* this slots I/O address */
	unsigned long slot_mem_address;	/* this slots mem address */
};





/*
	Function Prototypes
*/
CSTATUS GetCarrierAddress(int nHandle, long* pAddress);
CSTATUS SetCarrierAddress(int nHandle, long lAddress);
CSTATUS GetIpackAddress(int nHandle, char chSlot, long* pAddress);
CSTATUS ReadIpackID(int nHandle, char chSlot, word* pWords, int nWords);
CSTATUS EnableInterrupts(int nHandle);
CSTATUS DisableInterrupts(int nHandle);
CSTATUS SetInterruptLevel(int nHandle, word uLevel);
CSTATUS GetInterruptLevel(int nHandle, word* pLevel);
CSTATUS SetInterruptHandler(int nHandle, char chSlot, int nRequestNumber,
							int nVector, void(*pHandler)(void*), void* pData);
CSTATUS InitCarrierLib(void);
CSTATUS CarrierOpen(int nDevInstance, int* pHandle);
CSTATUS CarrierClose(int nHandle);
CSTATUS CarrierInitialize(int nHandle);
byte GetLastSlotLetter(void);
CSTATUS AccessVPD( int nHandle, ULONG Address, ULONG* Data, ULONG Cycle );


/* carrier enhancements */
CSTATUS GetMemoryAddress(int nHandle, long* pAddress);
CSTATUS SetMemoryAddress(int nHandle, long lAddress);
CSTATUS GetIpackMemoryAddress(int nHandle, char chSlot, long* pAddress);
CSTATUS SetIPClockControl(int nHandle, char chSlot, word uControl);
CSTATUS GetIPClockControl(int nHandle, char chSlot, word* pControl);
CSTATUS GetCarrierID(int nHandle, word* pCarrierID);
CSTATUS SetAutoAckDisable(int nHandle, word uState);
CSTATUS GetAutoAckDisable(int nHandle, word* pState);
CSTATUS GetTimeOutAccess(int nHandle, word* pState);
CSTATUS GetIPErrorBit(int nHandle, word* pState);

/*  Functions used by above functions */
void AddCarrier(CARRIERDATA_STRUCT* pCarrier);
void DeleteCarrier(int nHandle);
CARRIERDATA_STRUCT* GetCarrier(int nHandle);
byte input_byte(int nHandle, byte*);/* function to read an input byte */
word input_word(int nHandle, word*);/* function to read an input word */
void output_byte(int nHandle, byte*, byte);	/* function to output a byte */
void output_word(int nHandle, word*, word);	/* function to output a word */
long input_long(int nHandle, long*);			/* function to read an input long */
void output_long(int nHandle, long*, long);	/* function to output a long */
void blocking_start_convert(int nHandle, word *p, word v);
