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

#include "work.h"

/**
 *                                                                                             
 * initial_sort : after the creation of the population,                                         
 * all the individuals are sorted by the number of false clauses                                
 * @param individual the linked list that we need to sort                                       
 * @return the same linked list, but now sorted.                                                
 * 
 */
 inline Individual * initial_sort (Individual *individual) {

  /* It will be our pointer to go through the list. */
  Individual* ind;
  
  /* It will be our new list (sorted by ind->numfalse) */
  Individual* sol = NULL;

  /* It will be our pointer to play with the chain of individuals. */
  Individual* temp;

  int max;

  int num;

  /* We will go through the all linked list */
  while (individual != NULL) {

    /* We store for now the head. */
    ind = individual;
    
    max = -1;

    num =  0;
    
    /* While we didn't go through the all linked list. */
    while (ind != NULL) {

      /* if we find as good or better */
      if (ind->numfalse >= max) {

        /* If it's definitely better */
				if (ind->numfalse > max) {
					
          /* We update the maximum */
          max = ind->numfalse;
					
          /* and the number of people equals to this one is 1. */
          num = 1;

          /* We find just as good, we sum how many they are (for after) */
				} else num++;
      }

      /* We go to the next element of the linked list. */
      ind = ind->next;
    }

    /* For now, we know what is the maximum and how many they are with this value. */

    /* We go on all theses "maximum" who are equals to each others; */
    while ((individual != NULL) && (individual->numfalse == max)) {

      temp = individual;
      
      individual = individual->next;
      
      temp->next = sol;
      
      sol = temp;
      
      /* We know how many they are. */
      num--;
    }
		
    /* If it's not finished yet, some elements are not sorted yet */
    if (num != 0) {

      /* We restart again with the head of the linked list */
      ind = individual;

      /* While there is still element to sort */
      while ((num != 0) && (ind->next != NULL)) {

        /* if they are equals to our max */
				if ((ind->next)->numfalse == max){ 
					 
          /* We sort them */
          temp = ind->next;
          ind->next = (ind->next)->next;
          temp->next = sol;
          sol = temp;
					
          /* And we decremente the number of elements. */
          num--;


				} else {
          
          /* and the other case, we go to the next element. */
          ind = ind->next;
        }

      } 

      /* After this, every element is sorted, and the head of this sorted list is "sol" */

    }

  } /* While we didn't reach the end of the list. */


  /* We return the head of the sorted list. */
  return sol;

}

/**
 *
 * crossover_operator : selects 2 parents,                                                      
 * crosses them and improves the child before insert it in the population                       
 * @param r the linked list of the population                                                   
 * @return a positive value if everything is okay, -1 otherwise                                 
 * 
 */
 inline int crossover_operator(Individual **r) {

  int i;
  int j;
  int k;

  Individual * child;
  Individual * p1;
  Individual * p2;
  Individual * tmp;

  /* ----------------------------------------------------------------- */
  /* ---                SELECTION OF THE 2 PARENTS                 --- */
  /* ----------------------------------------------------------------- */

      i = random() % NUMINDIVIDUAL;
      j = random() % NUMINDIVIDUAL;

      /* Just in case to avoid any problems. */
      while (j == i) j = random() % NUMINDIVIDUAL;

      tmp  =  *r;
      
      for (k = 0; k < NUMINDIVIDUAL/3; ++k) tmp = tmp->next;

      worsetemporarynumfalse = tmp->numfalse;

      k    =   0;
      tmp  =  *r;

      /* We will need the 2 parents. */
      while ( tmp != NULL || (k < i && k < j) ) {
      
        if (k == i) p1 = select_individual(tmp);

        if (k == j) p2 = select_individual(tmp);
      
        k++;
      
        tmp = tmp->next;
      }    

  /* ----------------------------------------------------------------- */
  /* ---           CREATION AND AMELIORATION OF THE CHILD          --- */
  /* ----------------------------------------------------------------- */

      /* We perform a crossing between p1 and p2 */
      child = cross(p1,p2);
      
      /* and then a tabu search with this new individual. */
      child = tabu(child, ltrech , 1000);

      freeIndividual(p1);
      freeIndividual(p2);

  /* ----------------------------------------------------------------- */  
  /* ---      LET'S CHECK IF THE CHILD IS WORTH TO BE INSERTED     --- */
  /* ----------------------------------------------------------------- */      
      
      if (child->numfalse < worsetemporarynumfalse) { 

        child->numhamming = delete_individual(r); 
        
        /* We update the display of the statistic, the child is the new best. */
        if (child->numfalse < bestnumfalse) {

          bestnumfalse = child->numfalse;
        }

        /* The child is worth to be inserted. */
        insert_individual(child,r);
        
      } else {

        freeIndividual(child);

        --youngerindividual;

      }

  /* ----------------------------------------------------------------- */      
  
  --cardpopulation;

  return 0;

}

/**
 *                                                                                              
 * tabu : execute a tabu search on the individual ind with a tabu list                          
 * of length tabu_length and a maximum flip number of maxflip                                   
 * @param ind the individual on who we will perform a tabu search                               
 * @param tabu_length the length of the tabu list                                               
 * @param maxflip the maximum flip number                                                       
 * @return the individual after the tabu search                                                 
 * 
 */
 inline Individual * tabu (Individual *ind,int tabu_length, int maxflip){

  register int  numflip = 0;
  
  int* tabu_atom;
  
  int  i, max, bestatom,j,cnt;

  char* tabusol;

  int   tabunumfalse;

  /* diversification variables */

  int   bflip = 0;
  int   npossible;
  int*  fausse;             // represents how many times a clause was the unique false clause
  int*  interdit;           // represents the variables which can't be flipped same with the aspiration
  int*  possible;           // represents the variables which can be flipped (only one is flipped)
  int*  best_atom;          // represents the best variables after the degree mecanism. One is flipped

  int   longinterdit = numatom / 10; // length of the tabu list
  
  int*  V_atom[4];          // used for the degree mecanism
  float sol;                // used for the degree mecanism


  /* Allocation of every variables */
  fausse    = (int  *)malloc(sizeof(int)   * (unsigned long)(numclause));
  interdit  = (int  *)malloc(sizeof(int)   * (unsigned long)(numatom+1));
  possible  = (int  *)malloc(sizeof(int)   * (unsigned long)(numatom+1));
  best_atom = (int  *)malloc(sizeof(int)   * (unsigned long)(numatom+1));
  tabu_atom = (int  *)malloc(sizeof(int)   * (unsigned long)(numatom+1));
  tabusol   = (char *)malloc(sizeof(char)  * (unsigned long)(numatom+1));

  for( i = 0 ; i < 4 ; ++i) V_atom[i] = (int *)malloc(sizeof(int) * (unsigned long)(2*numatom+1));
  
  for( i = 1 ; i < numatom+1 ; ++i) {

    tabu_atom[i] = -tabu_length;
    interdit[i] = -longinterdit;

  }
  
  for( i = 0 ; i < numclause ; ++i) fausse[i] = 0;
  
  for( i = 1 ; i < numatom+1 ; ++i) tabusol[i] = ind->atom[i];
  
  tabunumfalse = ind->numfalse;

  /* -------------------------------------------------------------------------------------------- */

  //tabu search
  //break conditions : numfalse = 0
  //                   max flip is reached
  while ( (ind->numfalse > 0) && (numflip < maxflip) ) {

    for( i = 1 ; i < numatom+1 ; ++i) possible[i] = -1;

    npossible = 0;

    max = -BIG; // represent the best number of true clauses created with the flip of the variable
    
    cnt = 0;
    
    for( i = 1 ; i < numatom+1 ; ++i) {

      if (numflip - longinterdit >= interdit[i]) {  // the variable i is not forbidden

        if (numflip - tabu_length >= tabu_atom[i]) { // the variable i is not tabu

          if (ind->makecount[i] - ind->breakcount[i] >= max) {

            if (ind->makecount[i] - ind->breakcount[i] > max) {
              
              max = ind->makecount[i] - ind->breakcount[i];

              for ( j = 1 ; j < i ; ++j) possible[j] = -1;
              
              possible[i] = 1; // the variable i is the best variable to flip
              
              npossible   = 1;

            } else {

              possible[i] = 1;  // the variable i is one of the best variable to flip
              ++npossible;

            }

          }
        }

        /* aspiration */

        else { //the variable i is tabu

          if (ind->numfalse - ( ind->makecount[i] - ind->breakcount[i] ) < tabunumfalse) {

            max = ind->makecount[i] - ind->breakcount[i];
            
            for (j = 1 ; j < i ; ++j) possible[j] = -1;

            possible[i] = 1;

            npossible   = 1;
          }

        }

      } /* the variable i is not forbidden */

    } /* for every atoms */
    
    sol = (random()%npossible) + 1;  

    for ( i = 1 ; i < numatom+1 ; ++i) {

      if (possible[i] == 1) {

        sol--;
          
        if (sol <= 0) break;

      }

    }

    bestatom = i;
    
    tabu_atom[bestatom] = numflip;

    flipatom(ind, bestatom); // the variable ii is flipped
    
    if (ind->numfalse < tabunumfalse) {

      for( i = 1 ; i < numatom+1 ; ++i) tabusol[i] = ind->atom[i];

      tabunumfalse = ind->numfalse;

      bflip = numflip;

    }
    
    ++numflip; // incrementation of the flip number
  }

  for( i = 1 ; i < numatom+1 ; ++i) ind->atom[i] = tabusol[i];

  init(ind, FALSE);
  
  allflip = allflip + (unsigned long long)bflip;
  
  if (ind->numfalse < bestnumfalse) {
    
    bestnumfalse = ind->numfalse;
  }

  if (ind->numfalse == 0) {
    
    FOUND = SAT;
  }
  
  free(fausse);
  free(possible);
  free(interdit);
  free(tabu_atom);
  free(tabusol);
  free(best_atom);

  for ( i = 0 ; i < 4 ; ++i) free(V_atom[i]);

  return ind;
}


/**
 *                                                                                              
 * flipatom : flip the variable toflip in the individual ind and update all the structures      
 * @param ind the individual on who we will perform a flip                                      
 * @param toflip which atom we will flip                                                        
 * 
 */
 inline void flipatom(Individual *ind,int toflip) {

  int i, j;     
  int toenforce;    
  int numocc;

  /* registers are faster than memory to access (so better for theses variables) */
  register int cli;
  
  /* registers are faster than memory to access (so better for theses variables) */
  register int lit;
    
  /* registers are faster than memory to access (so better for theses variables) */
  register int sz;

  /* registers are faster than memory to access (so better for theses variables) */
  register int * litptr;
  
  int * occptr;
  
  if(ind->atom[toflip] > 0) toenforce = -toflip;
  else                      toenforce =  toflip;

  ind->atom[toflip] = 1-ind->atom[toflip];
  
  numocc = numoccurence[numatom-toenforce];

  occptr = occurence[numatom-toenforce];

  for(i = 0 ; i < numocc ; ++i) {
    
    cli = occurence[numatom-toenforce][i]; 

    if (--(ind->numtruelit[cli]) == 0) {

      ind->clausesFalse[ind->numfalse] = cli;
      ind->wherefalse[cli] = ind->numfalse;
      
      ++ind->numfalse;

      /* Decrement toflip's breakcount */
      --ind->breakcount[toflip];        
      
      /* Increment the makecount of all vars in the clause */
      sz = size[cli];
      
      litptr = clause[cli];
      
      for (j = 0; j < sz ; ++j) {

        lit = clause[cli][j];

        ind->makecount[ABS(lit)]++;
      }


    } else if (ind->numtruelit[cli] == 1) {

      /* Find the lit in this clause that makes it true, and inc its breakcount */
      sz = size[cli];

      litptr = clause[cli];

      for (j = 0 ; j < sz ; ++j) {

        lit = clause[cli][j];
        
        if((lit > 0) == ind->atom[ABS(lit)]) {

          ind->breakcount[ABS(lit)]++;
          break;
        }

      }

    } /* end of else if((ind->numtruelit[cli] == 1) */

  } /* end of the for-loop on every occurence */


  numocc = numoccurence[numatom+toenforce];
  
  occptr = occurence[numatom+toenforce];
  
  for(i = 0 ; i < numocc ; ++i) {

    /* cli = occurence[numatom+toenforce][i]; */
    cli = *(occptr++);   

    if(cli > numclause) break;  
    
    if (++(ind->numtruelit[cli]) == 1) {

      --ind->numfalse;
      ind->clausesFalse[ind->wherefalse[cli]]           = ind->clausesFalse[ind->numfalse];              
      ind->wherefalse[ind->clausesFalse[ind->numfalse]] = ind->wherefalse[cli];

      /* Increment toflip's breakcount */
      ind->breakcount[toflip]++;
      
      /* Decrement the makecount of all vars in the clause */
      sz = size[cli];
      
      litptr = clause[cli];
      
      for( j = 0 ; j < sz ; ++j) {

        /* lit = clause[cli][j]; */
        lit = *(litptr++);
        
        ind->makecount[ABS(lit)]--;

      }

    } else if (ind->numtruelit[cli] == 2) {

      /* Find the lit in this clause other than toflip that makes it true,
         and decrement its breakcount */

      sz     = size[cli];
      litptr = clause[cli];
      
      for( j = 0 ; j < sz ; ++j) {

        lit = clause[cli][j];
        
        if(((lit > 0) == ind->atom[ABS(lit)]) && (toflip != ABS(lit))) {

          ind->breakcount[ABS(lit)]--;
          break;
        }

      }

    } /* end of "else if (ind->numtruelit[cli] == 2)" */

  } /* end of the for-loop on every occurence */

} /* end of the function flipatom */

/**
 *                                                                                              
 * cross : cross two individuals and return a child.                                            
 * The best variable in the false clauses for the two parent                                    
 * is flipped and for the other variables,                                                      
 * if they are the same value for the two parents then the value is kept.                       
 * The other values are randomly valued.                                                        
 * @param p1 the first parent                                                                   
 * @param p2 the second parent                                                                  
 * @return the child which is the result of a cross between first and second parent             
 * 
 */
 inline Individual * cross(Individual* p1,Individual* p2) {
  
  int i;

  /* Creation and initialization of the child */
  Individual* child    = createIndividual(); 

  /* this child is now the youngest. */
  child->numindividual = youngerindividual+1;
  
  /* We have one element more in the card. */
  ++cardpopulation;
  
  /* the number of younger is now +1 */
  ++youngerindividual;

  for ( i = 1 ; i < numatom+1 ; ++i) {

    if ((random() % 2) == 0) child->atom[i] = p1->atom[i];

    else                     child->atom[i] = p2->atom[i];
  }

  /* FALSE because its not the first initialization */
  init(child, FALSE); 

  return child;
}


/**
 *                                                                                              
 * getSolution : This function allows us to get the individual which is the solution            
 * @param pop    Represents the population where one of individual is solution                  
 * @return a pointer on the individual which is the solution of the problem                     
 * 
 */
 inline Individual* getSolution(Individual* pop) {

  /* We start with the first element of the linked list. */
  Individual* tmp = pop;

  /* While we didn't reach the end of the list. */
  while(tmp != NULL) {

    /* If one of them solve every clauses, we return it */
    if(tmp->numfalse == 0) return tmp;

    /* We go to the next element. */
    tmp = tmp->next;
  }

  /* We go through all linkedlist, no element is a solution. */
  return NULL;
}


