
MODCFLAGS :=

a.out:			apc8620.o cnfg1k100.o drvr1k100.o rpnt1k100.o rprt1k100.o\
				rsts1k100.o wpnt1k100.o wprt1k100.o pldcfg1k100.o
			cc -o a.out apc8620.o cnfg1k100.o drvr1k100.o rpnt1k100.o rprt1k100.o\
				rsts1k100.o wpnt1k100.o wprt1k100.o pldcfg1k100.o

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr1k100.o:		drvr1k100.c ip1k100.h
			$(CC) $(MODCFLAGS) -c drvr1k100.c

cnfg1k100.o:		cnfg1k100.c ip1k100.h
			$(CC) $(MODCFLAGS) -c cnfg1k100.c

pldcfg1k100.o:		pldcfg1k100.c ip1k100.h
			$(CC) $(MODCFLAGS) -c pldcfg1k100.c

rpnt1k100.o:		rpnt1k100.c ip1k100.h
			$(CC) $(MODCFLAGS) -c rpnt1k100.c

rprt1k100.o:		rprt1k100.c ip1k100.h
			$(CC) $(MODCFLAGS) -c rprt1k100.c

rsts1k100.o:		rsts1k100.c ip1k100.h
			$(CC) $(MODCFLAGS) -c rsts1k100.c

wpnt1k100.o:		wpnt1k100.c ip1k100.h
			$(CC) $(MODCFLAGS) -c wpnt1k100.c

wprt1k100.o:		wprt1k100.c ip1k100.h
			$(CC) $(MODCFLAGS) -c wprt1k100.c
