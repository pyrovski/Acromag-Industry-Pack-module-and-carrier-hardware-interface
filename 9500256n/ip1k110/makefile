
MODCFLAGS :=

a.out:			apc8620.o cnfg1k110.o drvr1k110.o rpnt1k110.o rprt1k110.o\
				rsts1k110.o wpnt1k110.o wprt1k110.o pldcfg1k110.o
			cc -o a.out apc8620.o cnfg1k110.o drvr1k110.o rpnt1k110.o rprt1k110.o\
				rsts1k110.o wpnt1k110.o wprt1k110.o pldcfg1k110.o

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr1k110.o:		drvr1k110.c ip1k110.h
			$(CC) $(MODCFLAGS) -c drvr1k110.c

cnfg1k110.o:		cnfg1k110.c ip1k110.h
			$(CC) $(MODCFLAGS) -c cnfg1k110.c

pldcfg1k110.o:		pldcfg1k110.c ip1k110.h
			$(CC) $(MODCFLAGS) -c pldcfg1k110.c

rpnt1k110.o:		rpnt1k110.c ip1k110.h
			$(CC) $(MODCFLAGS) -c rpnt1k110.c

rprt1k110.o:		rprt1k110.c ip1k110.h
			$(CC) $(MODCFLAGS) -c rprt1k110.c

rsts1k110.o:		rsts1k110.c ip1k110.h
			$(CC) $(MODCFLAGS) -c rsts1k110.c

wpnt1k110.o:		wpnt1k110.c ip1k110.h
			$(CC) $(MODCFLAGS) -c wpnt1k110.c

wprt1k110.o:		wprt1k110.c ip1k110.h
			$(CC) $(MODCFLAGS) -c wprt1k110.c

