##############################################################################
# 
# Makefile for NQueens - Valentin Montmirail - Polytech Tours, France
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

EXECUTABLE = queens

OPTI       = -O3

# ----------------[ Flags for compilation of the executable ]------------------

FLAGS1   = -pedantic -Wall -Wextra -Wfloat-equal 
FLAGS2   = -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wconversion 
FLAGS3   = -Wredundant-decls -Winit-self -Wswitch-default
FLAGS4   = -Wswitch-enum -Wundef -Winline -std=c99
FLAGS    = $(FLAGS1) $(FLAGS2) $(FLAGS3) $(FLAGS4) 


all: 
	$(COMPILER) $(OPTI) $(FLAGS) -o $(EXECUTABLE) NQueens.c

clean:
	rm *.o
   

##############################################################################