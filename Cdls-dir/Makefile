CC = gcc -Wall -ffloat-store -O3
CCC = gcc -c -Wall -ffloat-store -O3 

CCA = gcc -Wall
CCCA = gcc -c -Wall

CCO = gcc -Wall -ffloat-store -O3 -D NDEBUG
CCCO = gcc -c -Wall -ffloat-store -O3 -D NDEBUG

all:	cdls

heap.o:heap.c heap.h
	$(CCC) heap.c

vec.o:vec.h vec.c 
	$(CCC) vec.c	

analyseRL.o:analyseRL.c analyseRL.h vec.o
	$(CCC) analyseRL.c

metaheuristique.o:metaheuristique.c metaheuristique.h
	$(CCC) metaheuristique.c

cdls:cdls.h cdls.c analyseRL.o vec.o heap.o metaheuristique.o
	$(CC)  cdls.c -o cdls analyseRL.o vec.o heap.o metaheuristique.o -lm

debug:
	$(CCCA) -g heap.c
	$(CCCA) -g metaheuristique.c
	$(CCCA) -g vec.c	
	$(CCCA) -g analyseRL.c
	$(CCA) -g cdls.c -o cdls analyseRL.o vec.o heap.o metaheuristique.o -lm

profil:
	$(CCC)  metaheuristique.c	
	$(CCC)  heap.c
	$(CCC)  vec.c	
	$(CCC)  analyseRL.c
	$(CC)   cdls.c -o cdls analyseRL.o vec.o heap.o metaheuristique.o -lm

compet:
	$(CCCO)  metaheuristique.c	
	$(CCCO)  heap.c
	$(CCCO)  vec.c	
	$(CCCO)  analyseRL.c
	$(CCO)   cdls.c -o cdls analyseRL.o vec.o heap.o metaheuristique.o -lm

clean:
	rm -f cdls
	rm *.o
	rm *~

