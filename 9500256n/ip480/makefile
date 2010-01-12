
MODCFLAGS :=

a.out:			apc8620.o cnfg480.o drvr480.o rmid480.o
			cc -o a.out apc8620.o cnfg480.o drvr480.o	rmid480.o

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr480.o:		drvr480.c ip480.h
			$(CC) $(MODCFLAGS) -c drvr480.c

cnfg480.o:		cnfg480.c ip480.h
			$(CC) $(MODCFLAGS) -c cnfg480.c

rmid480.o:		rmid480.c ip480.h
			$(CC) $(MODCFLAGS) -c rmid480.c
