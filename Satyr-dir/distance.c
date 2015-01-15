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

#include "distance.h"

/************************************************************************************************/
/*                                                                                              */
/* distance : give the hamming distance                                                         */
/* @param tmp we will compute the distance between tmp and tab                                  */
/* @param tab we will compute the distance between tmp and tab                                  */
/* @return the hamming distance between tmp and tab.                                            */
/*                                                                                              */
/************************************************************************************************/
inline float distance(char * tmp, char *tab) {

  /* We will need an index to compute every clause of tab and tmp. */
  int i;

  /* for now the distance equals 0. */
  float distance = 0.0;
    
  /* We compute the hamming distance of every clauses. */
  for(i = 0 ; i < numclause ; ++i) distance = distance + ABS(tmp[i]-tab[i]); 

  /* We return the hamming distance */
  return distance;
}

/************************************************************************************************/
/*                                                                                              */
/* hamming_distance : computes the hamming distance between 2 individuals                       */
/* @param p1 the index of the first individual                                                  */
/* @param p2 the index of the second individual                                                 */
/* @param ind the linked list of every individuals.                                             */
/* @return the hamming distance between p1 and p2                                               */
/*                                                                                              */
/************************************************************************************************/
inline int hamming_distance(int p1, int p2,Individual * ind) {

  int i;
  int sol = 0;

  char* s1;
  char* s2;
	
  if (p1 == p2) return 0;	

  s1 = (char*)malloc(sizeof(char) * ((unsigned long)numatom+1));
  
  s2 = (char*)malloc(sizeof(char) * ((unsigned long)numatom+1));
	 
  /* if we find p1 or p2 inside the linked list. */
  while ((ind->numhamming != p1) && (ind->numhamming != p2)) ind = ind->next;
  
  if (p1 == ind->numhamming) {

    for( i = 1 ; i < numatom+1 ; ++i) s1[i] = ind->atom[i];

  } else {   

    for( i = 1; i < numatom+1 ; ++i) s2[i] = ind->atom[i];

  }

  ind = ind->next;
	
  /* if we find p1 or p2 inside the linked list. */
  while ((ind->numhamming != p1) && (ind->numhamming != p2)) ind = ind->next;
  
  if (p1 == ind->numhamming) {

    for( i = 1 ; i < numatom+1 ; ++i) s1[i] = ind->atom[i];

  } else   {

    for( i = 1 ; i < numatom+1 ; ++i) s2[i] = ind->atom[i];

  }

  for( i = 1 ; i < numatom+1 ; ++i) {
    
    if (s1[i] != s2[i]) {
      ++sol;
    }

  }
	
  /* We don't need theses temporary s1 anymore. */
  free(s1);

  /* We don't need theses temporary s2 anymore. */ 
  free(s2);

  /* We return the hamming distance between s1 and s2. */
  return sol;
}


