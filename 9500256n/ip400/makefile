
MODCFLAGS :=

a.out:		apc8620.o cnfg400.o drvr400.o rpnt400.o rprt400.o rsts400.o
			cc -o a.out apc8620.o cnfg400.o drvr400.o rpnt400.o rprt400.o rsts400.o

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr400.o:		drvr400.c ip400.h
			$(CC) $(MODCFLAGS) -c drvr400.c

cnfg400.o:		cnfg400.c ip400.h
			$(CC) $(MODCFLAGS) -c cnfg400.c

rpnt400.o:		rpnt400.c ip400.h
			$(CC) $(MODCFLAGS) -c rpnt400.c

rprt400.o:		rprt400.c ip400.h
			$(CC) $(MODCFLAGS) -c rprt400.c

rsts400.o:		rsts400.c ip400.h
			$(CC) $(MODCFLAGS) -c rsts400.c
