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

CPP11    = 

CPPUNIT  = $(CPPUNIT1) $(CPPUNIT2)

FLAGS1	 = -pedantic -Wall -Wextra -Wold-style-cast -Woverloaded-virtual -Wfloat-equal 
FLAGS2   = -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion 
FLAGS3   = -Wshadow -Wredundant-decls -Winit-self -Wswitch-default 
FLAGS4   = -Wswitch-enum -Wundef -Winline $(CPP11)

FLAGS 	 = $(FLAGS1) $(FLAGS2) $(FLAGS3) $(FLAGS4)

MAIN	 = satyr

COMPILER = g++ 

SOURCES  = ./Sources/

OBJECT   = ./obj/

VARIABLE = $(SOURCES)Variable.cpp

CLAUSE   = $(SOURCES)Clause.cpp

PROBLEM  = $(SOURCES)Problem.cpp

LOGGER   = $(SOURCES)FileLogger.cpp

SOLVER   = $(SOURCES)Solver.cpp

TEST 	 = $(SOURCES)SatTest.cpp

##############################################################################

all: $(OBJECT)Solver.o $(OBJECT)Problem.o $(OBJECT)Clause.o $(OBJECT)Variable.o $(OBJECT)FileLogger.o
	$(COMPILER) $(CPP11) -o $(MAIN) $(OBJECT)Variable.o $(OBJECT)Clause.o $(OBJECT)Problem.o $(OBJECT)Solver.o $(OBJECT)FileLogger.o  Main.cpp

$(OBJECT)Variable.o:
	$(COMPILER) -c $(FLAGS) $(VARIABLE)
	mv Variable.o $(OBJECT)

$(OBJECT)Clause.o: $(OBJECT)Variable.o
	$(COMPILER) -c $(FLAGS) $(CLAUSE)
	mv Clause.o $(OBJECT)

$(OBJECT)Problem.o: $(OBJECT)Clause.o
	$(COMPILER) -c $(FLAGS) $(PROBLEM)
	mv Problem.o $(OBJECT)

$(OBJECT)Solver.o: $(OBJECT)Problem.o
	$(COMPILER) -c $(FLAGS) $(SOLVER)
	mv Solver.o $(OBJECT)

$(OBJECT)FileLogger.o:
	$(COMPILER) -c $(FLAGS) $(LOGGER)
	mv FileLogger.o $(OBJECT)

##############################################################################

doc:
	doxygen

documentation:
	doxygen

## We add the check possibility, to perform the cppcheck and to generate a cppcheck.xml file.
check:
	cppcheck --suppress=missingIncludeSystem --enable=all --inconclusive --xml --xml-version=2 *.cpp */*.cpp */*.hpp 2> cppcheck.xml;

clean:
	rm -rf $(OBJECT)*.o
	rm -rf Documentation

##############################################################################