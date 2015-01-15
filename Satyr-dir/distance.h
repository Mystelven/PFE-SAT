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

#ifndef DISTANCE_H
#define DISTANCE_H

#include "structure.h"

/************************************************************************************************/
/*                                                                                              */
/* distance : give the hamming distance                                                         */
/* @param tmp we will compute the distance between tmp and tab                                  */
/* @param tab we will compute the distance between tmp and tab                                  */
/* @return the hamming distance between tmp and tab.                                            */
/*                                                                                              */
/************************************************************************************************/
extern float distance(char *tmp, char *tab); 

/************************************************************************************************/
/*                                                                                              */
/* hamming_distance : computes the hamming distance between 2 individuals                       */
/* @param p1 the index of the first individual                                                  */
/* @param p2 the index of the second individual                                                 */
/* @param ind the linked list of every individuals.                                             */
/* @return the hamming distance ind[h] and all the others individuals                           */
/*                                                                                              */
/************************************************************************************************/
extern int hamming_distance(int p1, int p2,Individual * ind);

#endif