
MODCFLAGS :=

a.out:			apc8620.o cnfg408.o drvr408.o robt408.o ropt408.o rpnt408.o rprt408.o\
				rsts408.o wpnt408.o wprt408.o
			cc -o a.out apc8620.o cnfg408.o drvr408.o robt408.o ropt408.o\
				rpnt408.o rprt408.o rsts408.o wpnt408.o wprt408.o

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr408.o:		drvr408.c ip408.h
			$(CC) $(MODCFLAGS) -c drvr408.c

cnfg408.o:		cnfg408.c ip408.h
			$(CC) $(MODCFLAGS) -c cnfg408.c

robt408.o:		robt408.c ip408.h
			$(CC) $(MODCFLAGS) -c robt408.c

ropt408.o:		ropt408.c ip408.h
			$(CC) $(MODCFLAGS) -c ropt408.c

rpnt408.o:		rpnt408.c ip408.h
			$(CC) $(MODCFLAGS) -c rpnt408.c

rprt408.o:		rprt408.c ip408.h
			$(CC) $(MODCFLAGS) -c rprt408.c

rsts408.o:		rsts408.c ip408.h
			$(CC) $(MODCFLAGS) -c rsts408.c

wpnt408.o:		wpnt408.c ip408.h
			$(CC) $(MODCFLAGS) -c wpnt408.c

wprt408.o:		wprt408.c ip408.h
			$(CC) $(MODCFLAGS) -c wprt408.c




