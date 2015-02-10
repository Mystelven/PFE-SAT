##############################################################################
# 
# Makefile for SATyr - Valentin Montmirail - Polytech Tours, France
# Copyright (c) 2015.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
##############################################################################

COMPILER   = gcc

EXECUTABLE = satyr

OPTI       = -O3

# ----------------[ Flags for compilation of the executable ]------------------

FLAGS1	 = -pedantic -Wall -Wextra -Wfloat-equal 
FLAGS2   = -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion 
FLAGS3   = -Wredundant-decls -Winit-self -Wswitch-default -lpthread -D_REENTRANT
FLAGS4   = -Wswitch-enum -Wundef -Winline -std=c99 $(OPTI)
FLAGS 	 = $(FLAGS1) $(FLAGS2) $(FLAGS3) $(FLAGS4)

# -----------------------------------------------------------------------------

# ----------------[ The object created to link inside exec ]------------------

WORK = obj/work.o
UNSAT = obj/unsat.o 
DISPLAY = obj/display.o
DISTANCE = obj/distance.o 
STRUCTURE = obj/structure.o 
INITIALISATION = obj/initialisation.o 

# -----------------------------------------------------------------------------

ALL_THE_OBJECT = $(STRUCTURE) $(INITIALISATION) $(WORK) $(UNSAT) $(DISPLAY)

##############################################################################

all: 
	make debug;
	make release;

debug: satyr
	$(COMPILER) -o debug   $(FLAGS) $(ALL_THE_OBJECT) -pthread Main.c 

release: satyr
	$(COMPILER) -o release $(ALL_THE_OBJECT) -pthread Main.c 

satyr: $(ALL_THE_OBJECT)
	$(COMPILER) -o $(EXECUTABLE) $(FLAGS) $(ALL_THE_OBJECT) -pthread Main.c 

compet: $(ALL_THE_OBJECT)
	$(COMPILER) -o $(EXECUTABLE) $(FLAGS) $(ALL_THE_OBJECT) -D BENCHMARK -pthread Main.c 

clean: 
	rm debug
	rm release
	rm obj/*.o	

## We add the check possibility, to perform the cppcheck and to generate a cppcheck.xml file.
check:	
	 cppcheck --enable=all --xml *.c */*.c */*.h 2> cppcheck.xml

# ----------------[ Every different object to create ]------------------

$(STRUCTURE): structure.c
	$(COMPILER) -c $(FLAGS) structure.c
	mv structure.o obj/

$(DISTANCE): $(STRUCTURE) distance.c
	$(COMPILER) -c $(FLAGS) distance.c
	mv distance.o obj/	

$(INITIALISATION): $(UNSAT) initialisation.c
	$(COMPILER) -c $(FLAGS) initialisation.c
	mv initialisation.o obj/	

$(WORK): $(STRUCTURE) work.c
	$(COMPILER) -c $(FLAGS) work.c
	mv work.o obj/	

$(UNSAT): $(STRUCTURE) unsat.c
	$(COMPILER) -c $(FLAGS) unsat.c
	mv unsat.o obj/			

$(DISPLAY): $(STRUCTURE) $(WORK) display.c			
	$(COMPILER) -c $(FLAGS) -pthread display.c
	mv display.o obj/		

# -----------------------------------------------------------------------------								

##############################################################################