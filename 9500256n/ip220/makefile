
MODCFLAGS :=

a.out:		apc8620.o cd220.o drvr220.o coef220.o mode220.o rccid220.o wro220.o

			cc -o a.out apc8620.o cd220.o drvr220.o coef220.o mode220.o rccid220.o wro220.o

apc8620.o:		../carrier/apc8620.c ../carrier/apc8620.h
			$(CC) $(MODCFLAGS) -c ../carrier/apc8620.c

drvr220.o:		drvr220.c ip220.h
			$(CC) $(MODCFLAGS) -c drvr220.c

cd220.o:		cd220.c ip220.h
			$(CC) $(MODCFLAGS) -c cd220.c

coef220.o:		coef220.c ip220.h
			$(CC) $(MODCFLAGS) -c coef220.c

mode220.o:		mode220.c ip220.h
			$(CC) $(MODCFLAGS) -c mode220.c

rccid220.o:		rccid220.c ip220.h
			$(CC) $(MODCFLAGS) -c rccid220.c

wro220.o:		wro220.c ip220.h
			$(CC) $(MODCFLAGS) -c wro220.c


