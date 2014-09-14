##############################################################################
# 
# Makefile for SATyr - Valentin Montmirail - Polytech Tours, France
# Copyright (c) 2014.
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

FLAGS1	 = -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal 
FLAGS2   = -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion 
FLAGS3   = -Wshadow -Weffc++ -Wredundant-decls -Winit-self -Wswitch-default 
FLAGS4   = -Wswitch-enum -Wundef -Winline -Werror
FLAGS 	 = $(FLAGS1) $(FLAGS2) $(FLAGS3) $(FLAGS4)

MAIN	 = satyr

COMPILER = g++

SOURCES  = ./Sources/

OBJECT   = ./obj/

VARIABLE = $(SOURCES)Variable.cpp

CLAUSE   = $(SOURCES)Clause.cpp

PROBLEM  = $(SOURCES)Problem.cpp

SOLVER   = $(SOURCES)Solver.cpp

##############################################################################

all: $(OBJECT)solver.o $(OBJECT)problem.o $(OBJECT)clause.o $(OBJECT)variable.o
	$(COMPILER) -o $(MAIN) $(OBJECT)variable.o $(OBJECT)clause.o $(OBJECT)problem.o $(OBJECT)solver.o Main.cpp

$(OBJECT)variable.o:
	$(COMPILER) -c $(FLAGS) $(VARIABLE)
	mv variable.o $(OBJECT)

$(OBJECT)clause.o: $(OBJECT)variable.o
	$(COMPILER) -c $(FLAGS) $(CLAUSE)
	mv clause.o $(OBJECT)

$(OBJECT)problem.o: $(OBJECT)clause.o
	$(COMPILER) -c $(FLAGS) $(PROBLEM)
	mv problem.o $(OBJECT)

$(OBJECT)solver.o: $(OBJECT)problem.o
	$(COMPILER) -c $(FLAGS) $(SOLVER)
	mv solver.o $(OBJECT)

##############################################################################

doc:
	doxygen

## We add the check possibility, to perform the cppcheck and to generate a cppcheck.xml file.
check:
	cppcheck --enable=all --inconclusive --xml --xml-version=2 *.cpp */*.cpp 2> cppcheck.xml;

clean:
	rm -rf $(OBJECT)*.o
	rm -rf Documentation

##############################################################################