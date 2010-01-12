
MODCFLAGS :=

a.out:			apc8620.o cnfg470.o drvr470.o rpnt470.o rprt470.o\
				rsts470.o wpnt470.o wprt470.o
			cc -o a.out apc8620.o cnfg470.o drvr470.o\
				rpnt470.o rprt470.o rsts470.o wpnt470.o wprt470.o

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr470.o:		drvr470.c ip470.h
			$(CC) $(MODCFLAGS) -c drvr470.c

cnfg470.o:		cnfg470.c ip470.h
			$(CC) $(MODCFLAGS) -c cnfg470.c

rpnt470.o:		rpnt470.c ip470.h
			$(CC) $(MODCFLAGS) -c rpnt470.c

rprt470.o:		rprt470.c ip470.h
			$(CC) $(MODCFLAGS) -c rprt470.c

rsts470.o:		rsts470.c ip470.h
			$(CC) $(MODCFLAGS) -c rsts470.c

wpnt470.o:		wpnt470.c ip470.h
			$(CC) $(MODCFLAGS) -c wpnt470.c

wprt470.o:		wprt470.c ip470.h
			$(CC) $(MODCFLAGS) -c wprt470.c
