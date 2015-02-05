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

/**
 *
 *  displayResolution: useful for debug, it will display the 2 clauses choosen
 *  for the resolution, the variable used and also the result of the operation.
 *  @param c1 the first clause used for the resolution
 *  @param c2 the second clause used for the resolution
 *  @param var the variable to perform the resolution.
 * 
 */ 
extern void displayResolution(int c1,int c2,int var);

/**
 *																								
 * contains : This function will test if the variable is include inside the clause 				
 * @param cl the clause who maybe contains the variable 										
 * @param variable the variable that we need to check 											
 * @return TRUE if the clause contains the variable, FALSE otherwise. 							
 *																								
 */
extern char contains(int cl, int variable);


/**
 * 
 * tryToProveUNSAT: 
 * This procedure will try to prove that the problem has no solution.
 *
 */
extern void tryToProveUNSAT();


/**
 * 
 * isTautology will check if the last resolution is a tautology or not.
 * if not, then the clause is not worth it.
 * @return TRUE if the last resolution is a tautology, FALSE otherwise.
 *
 */
extern int isTautology();

/**
 *																								
 * resoution : This function will perform the resolution rule on 2 clauses 'c1' and 'c2'		
 * @param c1 the left member of the resolution rule 											
 * @param c2 the right member of the resolution rule 											
 * @param whichVariable on which variable we will perform the resolution 						
 * @param forReal is a boolean to know if we will store the result of not 						
 * @return 0 if the problem is still UNKNOWN and 1 if the problem is UNSAT 					
 * At the end, 1 clause will disappear and the 2nd one will be the result of the resolution		
 *
 */
extern int resolution(int c1, int c2,int whichVariable,int forReal);

/**
 *																								
 * restart : We did "too much" resolution and didn't find anything yet, we restart to find...   
 *			 We reinitialize also the resolution table
 *																								
 */
extern void restart();

#endif
