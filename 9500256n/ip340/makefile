
MODCFLAGS :=

a.out:		apc8620.o ain340.o cnfg340.o drvr340.o mccd340.o rsts340.o rwcc340.o

			cc -o a.out apc8620.o ain340.o cnfg340.o drvr340.o mccd340.o rsts340.o rwcc340.o

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr340.o:		drvr340.c ip340.h
			$(CC) $(MODCFLAGS) -c drvr340.c

ain340.o:		ain340.c ip340.h
			$(CC) $(MODCFLAGS) -c ain340.c

cnfg340.o:		cnfg340.c ip340.h
			$(CC) $(MODCFLAGS) -c cnfg340.c

mccd340.o:		mccd340.c ip340.h
			$(CC) $(MODCFLAGS) -c mccd340.c

rsts340.o:		rsts340.c ip340.h
			$(CC) $(MODCFLAGS) -c rsts340.c

rwcc340.o:		rwcc340.c ip340.h
			$(CC) $(MODCFLAGS) -c rwcc340.c

