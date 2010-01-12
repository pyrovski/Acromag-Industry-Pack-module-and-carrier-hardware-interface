
MODCFLAGS :=

a.out:			apc8620.o drvr405.o rsts405.o rpnt405.o rprt405.o wpnt405.o wprt405.o
			cc -o a.out apc8620.o drvr405.o rsts405.o rpnt405.o rprt405.o wpnt405.o wprt405.o

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr405.o:		drvr405.c ip405.h
			$(CC) $(MODCFLAGS) -c drvr405.c

rsts405.o:		rsts405.c ip405.h
			$(CC) $(MODCFLAGS) -c rsts405.c

rpnt405.o:		rpnt405.c ip405.h
			$(CC) $(MODCFLAGS) -c rpnt405.c

rprt405.o:		rprt405.c ip405.h
			$(CC) $(MODCFLAGS) -c rprt405.c

wpnt405.o:		wpnt405.c ip405.h
			$(CC) $(MODCFLAGS) -c wpnt405.c

wprt405.o:		wprt405.c ip405.h
			$(CC) $(MODCFLAGS) -c wprt405.c






