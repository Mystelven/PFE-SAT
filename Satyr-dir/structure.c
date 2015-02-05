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

#include "structure.h"

/** We will need a pointer on all the population. */
Individual* population = NULL; 

/**
 *                                                                                              
 * createIndividual : create and return an individual memory space                              
 * @return the individual memory space created and returned                                     
 * 
 */
inline Individual* createIndividual() {

	/* We create a new individual */
	Individual* i;	
	
	/* We allocate the good memory space. */
	i = (Individual *) malloc(sizeof(Individual)); 

	/* We allocate the memory for every arrays inside the structure. */
	i->clausesFalse = (int *)malloc(sizeof(int)  * (unsigned long)(numclause));
	i->makecount    = (int *)malloc(sizeof(int)  * (unsigned long)(numatom+1)); 
	i->breakcount   = (int *)malloc(sizeof(int)  * (unsigned long)(numatom+1)); 
	i->atom         = (char*)malloc(sizeof(char) * (unsigned long)(numatom+1)); 
	i->wherefalse   = (int *)malloc(sizeof(int)  * (unsigned long)(numclause)); 
	i->numtruelit   = (int *)malloc(sizeof(int)  * (unsigned long)(numclause)); 

	/* For now, the Individual is not linked to anyone. */
	i->next = NULL;

	/* For now, we don't know how many false clauses is unsolved by this individual. */
	i->numfalse = 0;

	/* For now, we don't know his number yet. */
	i->numindividual = -1;

	/* We return our individual who is created and allocated. */
	return i;
}

/**
 *                                                                                              
 * freePopulation : free all the population (all the linked list)                               
 * @param pop the population where we want to free the memory space                             
 *                                                                                              
 */
inline void freePopulation(Individual * pop) {

	/* We start with the head of the list. */
	Individual* tmp = pop;

	/* We will need another pointer. */
	Individual* tmp2 = NULL;

	/* While we didn't reach the end of the list: */
	while( tmp != NULL ) {

		/* We store the pointer. */
		tmp2 = tmp;

		/* We go to the next pointer. */
		tmp = tmp->next;
		
		/* We free the previous individual. */
		freeIndividual(tmp2);
	}

}

/**
 *                                                                                              
 * freeIndividual : release an individual memory space                                          
 * @param i the individual that we want to free the memory space                                
 *                                                                                              
 */
inline void freeIndividual(Individual * i) {  
  
	/* We release the memory for all the arrays inside the structure. */
	free(i->breakcount); 
	free(i->makecount); 
	free(i->atom); 
	free(i->wherefalse); 
	free(i->clausesFalse); 
	free(i->numtruelit); 
  
	/* We release the memory for the pointer itself. */
  	free(i);
}


/**
 *                                                                                              
 * createPopulation : create a population of NUMINDIVIDUAL individuals                          
 * @return we will return the pointer on the first element of the linked list (the population)  
 * @see NUMINDIVIDUAL to know how many individual inside the linked list                        
 *                                                                                              
 */
inline Individual* createPopulation() {

	/* We will make a loop to create all theses individuals inside the population. */
	int i;	

	/* We will create NUMINDIVIDUAL Individuals, so we need a pointer for which of them. */
	Individual* x;

	/* For every individual that we have to create. */
  	for( i = 0 ; i < NUMINDIVIDUAL ; ++i) {

  		/* We create it. */
    	x = createIndividual();

    	/* We put the good values for theses attributes. */
    	x->numindividual = cardpopulation;
    	// x->numhamming    = cardpopulation;

    	/* We incremente this number. */
    	++cardpopulation;

    	/* TRUE because it is the first initialization */
    	init(x, TRUE); 

		/* Insertion of the new individual in the population */
    	if (i == 0) population = x;    		
    	else { 

      		x->next    = population;
      		population = x;
    	} 

		/* We save the best individual */
    	if (x->numfalse <= bestnumfalse) {
			
			bestnumfalse = x->numfalse;
    	}

  	} /* end of the "for every individual" */

  	/* We return the population that we just created. */
	return population;
}

/**
 *                                                                                              
 * delete_individual : delete the older individual of the population                            
 * @param ind the individual that we want to delete.                                            
 * @return the value return by the numhamming or the function delete_individual2                
 * 
 */
inline int delete_individual(Individual **ind) {

	/* We will need a pointer to free our individual. */
	Individual* x;

	/* the value that we want to return. */
	int i;
	

	if ((*ind)->numindividual == olderindividual) {				

		/* We get the hamming number of this individual. */
		i = (*ind)->numhamming;
		
		/* It's easier to play with only one pointer than 2 ;) */
		/* And it will be useful to free this individual */
		x = *ind;
		
		/* We plug the linkedlist to the next. */
		(*ind) = (*ind)->next;
		
		/* We will free the individual X. */	
		freeIndividual(x);

	} else {

		i = delete_individual2(*ind);

	} 

	++olderindividual;	
	
	return i;
}

/**
 *                                                                                              
 * delete_individual2 : used by delete_individual                                               
 * @param y the individual that we want to delete                                               
 * @see extern int delete_individual(Individual** ind)
 * @return We return numhamming if everything okay, -1 otherwise.                              
 * 
 */
inline int delete_individual2(Individual *y) {

	/* We will need a pointer to delete an individual */
	Individual* x;

	/*  
	 * We will return -1 if something wrong happened,
	 * and the numhamming of the individual before the deleted one otherwise 
	 */
	int i = -1;
	
	/* We look for the good individual inside the linked list */
	while((y->next != NULL) && ((y->next)->numindividual != olderindividual)) {
		y = y->next;
	}

	/* If we find the good individual */
	if(y->next != NULL) {
		
		/* We get the numhamming to return it after. */
		i = y->next->numhamming;

		x = y->next;
		
		/* We plug the linkedlist to detach the deleted indivudal. */
		y->next = (y->next)->next;
		
		/* We free the memory space of our individual */
		freeIndividual(x);
	}

	/* We return numhamming if everything okay, -1 otherwise. */
	return i;
}

/**
 *                                                                                             
 * insert_individual : inserts an individual in a sorted population                             
 * @param y the individual that we want to insert                                               
 * @param z our linkedlist where y will be insert                                               
 * 
 */
inline void insert_individual(Individual * y, Individual ** z) {

	/* We will need a temporary pointer */
  	Individual * tmp;

  	/* If we need to insert at the head-place (because it's a sorted population) */
	if (y->numfalse <= (*z)->numfalse) {
		
		y->next = *z;
		*z = y;

	} else {

		tmp = *z;

		/* We search for the good position inside the linked list. */
		while((tmp->next != NULL) && (y->numfalse >= tmp->next->numfalse)) {

			tmp = tmp->next;
		}

		/* If we insert at the end*/
		if (tmp->next == NULL) {
			
			y->next = NULL;
			tmp->next = y;

		} else { /* If we are in the "middle" of the linked list. */

			y->next = tmp->next;
			tmp->next = y;
		}

	}

}

/**
 *                                                                                              
 * add_individual : copy an individual in an other list of individuals                          
 * @param x the individual where we will perform a copy                                         
 * @param ind the linked list where we will add ind                                             
 * 
 */
inline Individual* add_individual(Individual* ind, Individual* x) {

	/* We will need an index for some loop. */
  	int i;

  	/* We will need an individual because we will create a new one. */
  	Individual *p;
	
	/* We create the memory space for every data inside the structure. */
  	p = createIndividual();

  	p->numfalse = x->numfalse;

  	/* We copy every clauses. */
  	for(i = 0 ; i < numclause ; ++i) {

    	p->clausesFalse[i] =  x->clausesFalse[i];
    	p->wherefalse[i]   =  x->wherefalse[i];
    	p->numtruelit[i]   =  x->numtruelit[i];
  	}

  	/* We copy every atoms. */
  	for(i = 1 ; i < numatom+1 ; ++i) {

 	   	p->atom[i]         = x->atom[i];
    	p->breakcount[i]   = x->breakcount[i];
    	p->makecount[i]    = x->makecount[i];
  	}

  	/* We copy the number of individual, the number of hamming, ... */
  	p->numindividual = x->numindividual;
  	p->numhamming    = x->numhamming;
  	p->next          = ind;

  	/* We return the created copy of our individual. */
  	return p;
}

/************************************************************************************************/

/**
 *                                                                                              
 * select_individual : able to take from a list an individual 									
 * @param list the linked list where we will look 		                                        
 * 
 */
inline Individual* select_individual(Individual* list) {

	return add_individual(NULL,list);
}

/**
 *																								
 * init : initializes the benchmark structures 													
 * @param ind the individual that we need to initialize 										
 * @param first is it the first initialisation ? 												
 *
 */
inline void init(Individual *ind,int first) {

	int i;
	int j;
	int thetruelit = 0;
	
	for( i = 0 ; i < numclause ; ++i) {

		ind->numtruelit[i]   = 0;
		ind->clausesFalse[i] = 0;
	}
	
	ind->numfalse      = 0;
	ind->makecount[0]  = 0;
	ind->breakcount[0] = 0;

	for( i = 1 ; i < numatom+1 ; ++i) {

		ind->breakcount[i] = 0;
		ind-> makecount[i] = 0;

	}
	
	/* Only on the first initialisation. */
	if (first == TRUE) {

		for( i = 1 ; i < numatom+1 ; ++i) {
 			
 			ind->atom[i] = (char)(random() % 2);			 
		}

	}

	/* Initialize breakcount and makecount in the following: */
	for( i = 0 ; i < numclause ; ++i) {
		
		for( j = 0 ; j < size[i] ; ++j) {

			if((clause[i][j] > 0) == ind->atom[ABS(clause[i][j])]) {

				++ind->numtruelit[i];

				thetruelit = clause[i][j];

		  	}

		}

	  	if(ind->numtruelit[i] == 0) {
			
			ind->wherefalse[i]               = ind->numfalse;

			ind->clausesFalse[ind->numfalse] = i;
			
			++ind->numfalse;

			for( j = 0 ; j < size[i] ; ++j) ++ind->makecount[ABS(clause[i][j])];			

		} else {

	    	if (ind->numtruelit[i] == 1) ++ind->breakcount[ABS(thetruelit)];	      

	    	ind->wherefalse[i] = -1;
	  	}		
	}
}

