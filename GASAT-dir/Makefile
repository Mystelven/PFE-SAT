ifndef CPPFLAGS
			 CPPFLAGS:=-O3
endif

GASAT : GASAT.o
						gcc -o gaSAT GASAT.o -lm

GASAT.o : GASAT.c
				 gcc -c $(CPPFLAGS) -o GASAT.o $(OPTIONS) GASAT.c
											
clean : 
			rm GASAT.o GASAT