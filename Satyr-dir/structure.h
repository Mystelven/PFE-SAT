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
##############################################################################
*/

#ifndef SATYR_H
#define SATYR_H

#include "individual.h"

/************************************************************************************************/
/*                                                                                              */
/* createIndividual : create and return an individual memory space                              */
/* @return the individual memory space created and returned                                     */
/*                                                                                              */
/************************************************************************************************/
extern Individual* createIndividual();

/************************************************************************************************/
/*                                                                                              */
/* freePopulation : free all the population (all the linked list)                               */
/* @param pop the population where we want to free the memory space                             */
/*                                                                                              */
/************************************************************************************************/
extern void freePopulation(Individual* pop);

/************************************************************************************************/
/*                                                                                              */
/* freeIndividual : release an individual memory space                                          */
/* @param i the individual that we want to free the memory space                                */
/*                                                                                              */
/************************************************************************************************/
extern void freeIndividual(Individual* i);

/************************************************************************************************/
/*                                                                                              */
/* createPopulation : create a population of NUMINDIVIDUAL individuals                          */
/* @return we will return the pointer on the first element of the linked list (the population)  */
/* @see NUMINDIVIDUAL to know how many individual inside the linked list                        */
/*                                                                                              */
/************************************************************************************************/
extern Individual* createPopulation(); 

/************************************************************************************************/
/*                                                                                              */
/* delete_individual : delete the older individual of the population                            */
/* @param ind the individual that we want to delete.                                            */
/* @return the value return by the numhamming or the function delete_individual2                */
/*                                                                                              */
/************************************************************************************************/
extern int delete_individual(Individual** ind); 

/************************************************************************************************/
/*                                                                                              */
/* delete_individual2 : used by delete_individual                                               */
/* @param y the individual that we want to delete                                               */
/* @return We return numhamming if everything okay, -1 otherwise.                               */
/*                                                                                              */
/************************************************************************************************/
extern int delete_individual2(Individual* y); 

/************************************************************************************************/
/*                                                                                              */
/* insert_individual : inserts an individual in a sorted population                             */
/* @param y the individual that we want to insert                                               */
/* @param z our linkedlist where y will be insert                                               */
/*                                                                                              */
/************************************************************************************************/
extern void insert_individual(Individual* y, Individual** z); 

/************************************************************************************************/
/*                                                                                              */
/* add_individual : copy an individual in an other list of individuals                          */
/* @param x the individual where we will perform a copy                                         */
/* @param ind the linked list where we will add ind                                             */
/*                                                                                              */
/************************************************************************************************/
extern Individual* add_individual(Individual* ind, Individual* x); 

/************************************************************************************************/

#include "work.h"
#include "distance.h"
#include "initialisation.h"

#endif
