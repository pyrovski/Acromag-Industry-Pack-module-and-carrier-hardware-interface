
MODCFLAGS :=

a.out:		apc8620.o cd230.o contreg.o rmid230.o timer.o rwwavmem.o wro230.o wcc230.o drvr230.o

			cc -o a.out apc8620.o cd230.o drvr230.o contreg.o rmid230.o\
			timer.o rwwavmem.o wro230.o wcc230.o -lm

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr230.o:		drvr230.c ip230.h
			$(CC) $(MODCFLAGS) -c drvr230.c

cd230.o:		cd230.c ip230.h
			$(CC) $(MODCFLAGS) -c cd230.c

contreg.o:		contreg.c ip230.h
			$(CC) $(MODCFLAGS) -c contreg.c

rmid230.o:		rmid230.c ip230.h
			$(CC) $(MODCFLAGS) -c rmid230.c

timer.o:		timer.c ip230.h
			$(CC) $(MODCFLAGS) -c timer.c

rwwavmem.o:		rwwavmem.c ip230.h
			$(CC) $(MODCFLAGS) -c rwwavmem.c

wro230.o:		wro230.c ip230.h
			$(CC) $(MODCFLAGS) -c wro230.c

wcc230.o:		wcc230.c ip230.h
			$(CC) $(MODCFLAGS) -c wcc230.c



