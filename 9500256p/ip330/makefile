
MODCFLAGS :=

a.out:		apc8620.o ain330.o cnfg330.o drvr330.o mccd330.o rsts330.o

			cc -o a.out apc8620.o ain330.o cnfg330.o drvr330.o mccd330.o rsts330.o

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr330.o:		drvr330.c ip330.h
			$(CC) $(MODCFLAGS) -c drvr330.c

ain330.o:		ain330.c ip330.h
			$(CC) $(MODCFLAGS) -c ain330.c

cnfg330.o:		cnfg330.c ip330.h
			$(CC) $(MODCFLAGS) -c cnfg330.c

mccd330.o:		mccd330.c ip330.h
			$(CC) $(MODCFLAGS) -c mccd330.c

rsts330.o:		rsts330.c ip330.h
			$(CC) $(MODCFLAGS) -c rsts330.c
