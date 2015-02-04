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

#ifndef WORK_H
#define WORK_H

#include "structure.h"

/************************************************************************************************/
/*                                                                                              */
/* initial_sort : after the creation of the population,                                         */
/* all the individuals are sorted by the number of false clauses                                */
/* @param individual the linked list that we need to sort                                       */
/* @return the same linked list, but now sorted.                                                */
/*                                                                                              */
/************************************************************************************************/
Individual * initial_sort (Individual *ind); 
 
/************************************************************************************************/
/*                                                                                              */
/* crossover_operator : selects 2 parents,                                                      */
/* crosses them and improves the child before insert it in the population                       */
/* @param r the linked list of the population                                                   */
/* @return a positive value if everything is okay, -1 otherwise                                 */
/*                                                                                              */
/************************************************************************************************/
int crossover_operator(Individual **r);

/************************************************************************************************/
/*                                                                                              */
/* tabu : execute a tabu search on the individual ind with a tabu list                          */
/* of length tabu_length and a maximum flip number of maxflip                                   */
/* @param ind the individual on who we will perform a tabu search                               */
/* @param tabu_length the length of the tabu list                                               */
/* @param maxflip the maximum flip number                                                       */
/* @return the individual after the tabu search                                                 */
/*                                                                                              */
/************************************************************************************************/
Individual * tabu (Individual *ind,int tabu_length, int maxflip); 

/************************************************************************************************/
/*                                                                                              */
/* flipatom : flip the variable toflip in the individual ind and update all the structures      */
/* @param ind the individual on who we will perform a flip                                      */
/* @param toflip which atom we will flip                                                        */
/*                                                                                              */
/************************************************************************************************/
void flipatom(Individual *ind,int toflip);

/************************************************************************************************/
/*                                                                                              */
/* cross : cross two individuals and return a child.                                            */
/* The best variable in the false clauses for the two parent                                    */
/* is flipped and for the other variables,                                                      */
/* if they are the same value for the two parents then the value is kept.                       */
/* The other values are randomly valued.                                                        */
/* @param p1 the first parent                                                                   */
/* @param p2 the second parent                                                                  */
/* @return the child which is the result of a cross between first and second parent             */
/*                                                                                              */
/************************************************************************************************/
Individual * cross(Individual* p1, Individual* p2);

/************************************************************************************************/
/*                                                                                              */
/* getSolution : This function allows us to get the individual which is the solution            */
/* @param pop    Represents the population where one of individual is solution                  */
/* @return a pointer on the individual which is the solution of the problem                     */
/*                                                                                              */
/************************************************************************************************/
Individual* getSolution(Individual* population);

#endif
