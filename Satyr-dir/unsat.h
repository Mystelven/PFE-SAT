/*##############################################################################
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

#ifndef UNSAT_H
#define UNSAT_H

#include "structure.h"

/************************************************************************************************/
/*																								*/
/* resoution : This function will perform the resolution rule on 2 clauses 'c1' and 'c2'		*/
/* @param c1 the left member of the resolution rule 											*/
/* @param c2 the right member of the resolution rule 											*/
/* @param whichVariable on which variable we will perform the resolution 						*/
/* @return 0 if the problem is still UNKNOWN and 1 if the problem is UNSAT 						*/
/* At the end, 1 clause will disappear and the 2nd one will be the result of the resolution		*/
/*																								*/
/************************************************************************************************/
extern int resolution(int c1, int c2, int whichVariable );

/************************************************************************************************/
/*																								*/
/* restart : We did "too much" resolution and didn't find anything yet, we restart to find...   */
/*																								*/
/************************************************************************************************/
extern void restart();


extern char contains(int cl, int variable);

/************************************************************************************************/
/*																								*/
/* getWhichVariableToPerformResolution : Get the most probable variable for a resolution 		*/
/* @param population the population where that we give us the good clauses						*/
/* @return the value of the litteral where we will perform a resolution 						*/
/*																								*/
/************************************************************************************************/
extern int getWhichVariableToPerformResolution(Individual* population);

/************************************************************************************************/
/*																								*/
/* performResolutionProof : We will try to prove that the problem has no solution 				*/
/* @param population the population where that we give us the good clauses						*/
/*																								*/
/************************************************************************************************/
extern void performResolutionProof(Individual* population);


/************************************************************************************************/
/*																								*/
/* getWhichClausesToPerformResolution : We get the 2 clauses for the resolution 				*/
/* @param population the population where that we give us the good clauses						*/
/* @param whichVariable on which variable we will perform the resolution..						*/
/* @return a 2-cases array for the clauses where we will perform the resolution 				*/
/*																								*/
/************************************************************************************************/
extern int* getWhichClausesToPerformResolution(Individual* population,int whichVariable);

#endif
