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

#ifndef DISPLAY_H
#define DISPLAY_H

#include "work.h"
#include <time.h>

#define CLOCK_TICK 100

/** 
 *																								
 * displayInfo : useful to display some informations 			   								
 * @param std the output where we will write informations 		   																																
 */
void displayInfo(FILE* std);

/**
 *
 * signalHandler: Useful to catch the SIGINT signal in that way,
 * we can shut down the solver and display that we don't know if the problem
 * is SATISFIABLE or UNSATISFIABLE...
 * The solver will so, display UNKNOWN for the state of the result.
 * @param signum the identification number of the signal
 *
 */
void signalHandler(int signum);

/**
 *																						
 * displayStat : We will display every generation to see the progression.				
 *
 */
extern void displayStat();

/**
 *																								
 * displayInfo : useful to display the final result after solving this problem 					
 * @param population the population we used to solve this problem
 *
 */
void displayFinalResult(Individual* population);

/**
 *                                                                                   
 * displaySolution: this function allows us to display the solution of the problem.  
 * @param pop    Represents the population where one of individual is solution       
 * 
 */
void displaySolution(Individual* population);

#endif
