/*
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
##############################################################################*/

#ifndef INITIALISATION_H
#define INITIALISATION_H

#include "structure.h"

#include "unsat.h"

/************************************************************************************************/
/*																								*/
/* parameters : read all the user parameters 													*/
/* @param argc the number of argument passed to the program										*/
/* @param argv every char* for every argument 													*/
/* @return 0 if everything was good, something else otherwise 									*/
/*																								*/
/************************************************************************************************/
extern int parameters(char *argv[]); 

/************************************************************************************************/
/*																								*/
/* initprob : read the benchmark file 															*/
/*																								*/
/************************************************************************************************/
extern void initprob();

/************************************************************************************************/
/*																								*/
/* init : initializes the benchmark structures 													*/
/* @param ind the linked list of every individuals												*/
/* @param first is it the first initialisation ? 												*/
/*																								*/
/************************************************************************************************/
extern void init(Individual *ind,int first);


extern void initResolutionTable();

#endif
