CC= gcc -std=c99 #compilateur utilise
CFLAGS =-Wall -W -pedantic -ansi
LFLAGS = -ffast-math -lm
EXEC =

all : test2

test2 : main2.o maillage.o cercle.o qualite.o ajout_point.o boite.o basculement_arete.o suppression.o
	$(CC) -o test2 main2.o boite.o ajout_point.o qualite.o cercle.o suppression.o basculement_arete.o maillage.o $(CFLAGS) $(LFLAGS)

test_bascul2 : main_basculement.o maillage.o cercle.o qualite.o boite.o suppression.o basculement_arete2.o
	$(CC) -o test_bascul2 main_basculement.o boite.o qualite.o cercle.o suppression.o basculement_arete2.o maillage.o $(CFLAGS) $(LFLAGS)

test3 : main3.o maillage.o cercle.o boite.o suppression.o basculement_niveau2.o
	$(CC) -o test3 main3.o boite.o cercle.o suppression.o basculement_niveau2.o maillage.o $(CFLAGS) $(LFLAGS)

main.o: main.c main.h
	$(CC) -c main.c

main2.o: main2.c main2.h
	$(CC) -c main2.c

main3.o: main3.c main3.h
	$(CC) -c main3.c

suppression.o: suppression.c maillage.o cercle.o suppression.h
	$(CC) -c suppression.c $(LFLAGS)

main_basculement.o: main_basculement.c main2.h
		$(CC) -c main_basculement.c

boite.o: boite.c boite.h
	$(CC) -c boite.c

basculement_arete.o: basculement_arete.c basculement_arete.h
	$(CC) -c basculement_arete.c

basculement_arete2.o: basculement_arete2.c basculement_arete.h
	$(CC) -c basculement_arete2.c

maillage.o: maillage.c maillage.h
	$(CC) -c maillage.c

ajout_point.o: ajout_point.c ajout_point.h
	$(CC) -c ajout_point.c

qualite.o: qualite.c qualite.h
	$(CC) -c qualite.c

basculement_niveau2.o: basculement_niveau2.c basculement_niveau2.h
	$(CC) -c basculement_niveau2.c

cercle.o: cercle.c maillage.o cercle.h
	$(CC) -c cercle.c

clean:
	rm -rf *.o

mrproper : clean
	rm -rf $(EXEC)
