
MODCFLAGS :=

a.out:		apc8620.o cd231.o drvr231.o coef231.o mode231.o rccid231.o wro231.o rdo231.o program.o

			cc -o a.out apc8620.o cd231.o drvr231.o coef231.o mode231.o rccid231.o wro231.o rdo231.o program.o


apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr231.o:		drvr231.c ip231.h
			$(CC) $(MODCFLAGS) -c drvr231.c

cd231.o:		cd231.c ip231.h
			$(CC) $(MODCFLAGS) -c cd231.c

coef231.o:		coef231.c ip231.h
			$(CC) $(MODCFLAGS) -c coef231.c

mode231.o:		mode231.c ip231.h
			$(CC) $(MODCFLAGS) -c mode231.c

rccid231.o:		rccid231.c ip231.h
			$(CC) $(MODCFLAGS) -c rccid231.c

wro231.o:		wro231.c ip231.h
			$(CC) $(MODCFLAGS) -c wro231.c

rdo231.o:		rdo231.c ip231.h
			$(CC) $(MODCFLAGS) -c rdo231.c

program.o:		program.c ip231.h
			$(CC) $(MODCFLAGS) -c program.c


