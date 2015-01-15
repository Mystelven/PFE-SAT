#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

/************************************************************************************************/
/*                                                                                              */
/* Standard includes                                                                            */
/*                                                                                              */
/************************************************************************************************/

  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include <math.h>
  #include <limits.h>
  #include <signal.h>
  #include <sys/times.h>
  #include <sys/time.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <time.h>

/************************************************************************************************/

#define random()           rand()
#define srandom(seed) srand(seed)

/* seed for the random generator. */
int seed; 


/************************************************************************************************/
/*                                                                                              */
/* Constant parameters                                                                          */
/*                                                                                              */
/************************************************************************************************/

/* size of block to malloc each time */
#define STOREBLOCK 20000

#define TRUE  1
#define FALSE 0

/* maximum number of literals which can be in any clause */
#define MAXLENGTH 50         

/************************************************************************************************/

/************************************************************************************************/
/*                                                                                              */
/* Internal constants                                                                           */
/*                                                                                              */
/************************************************************************************************/

#define Var(CLAUSE, POSITION) (ABS(clause[CLAUSE][POSITION]))

int scratch;

#define ABS(x) ((scratch=(x))>0?(scratch):(-scratch))

float fscratch;

#define ABSF(x) ((fscratch=(x))>0?(fscratch):(-fscratch))

#define BIG 100000000

/************************************************************************************************/
/* Variables used for the control of the time */

long begintime;
long   endtime;
long  temptime;

struct tms* a_tms;

/* We will need a variable to know how many times it takes to read the benchmark. */
double parsingTime;

/************************************************************************************************/

/* benchmark's name */
char* benchmark_name;  

/* number of individuals where the two parents are selected */
int BESTINDIVIDUAL; 

/* number of individuals in the population */
int NUMINDIVIDUAL; 

/* max number of crosses */
int MAXTRY; 

/************************************************************************************************/
/* Tabou search variables */

/* length of tabou list for the preliminary search */
int ltinit; 

/* length of tabou list for the real search */
int ltrech; 

/* length of tabou list for the preliminary search (percent of numatom) */
int ltinitpc; 

/* length of tabou list for the real search (percent of numatom) */
int ltrechpc; 

/************************************************************************************************/

/************************************************************************************************/
/* Variables for the population counter */

/* number of individual at each moment */
int cardpopulation; 

/* identification of the younger individual */
int youngerindividual; 

/* number of false clauses for the best individual */
int bestnumfalse; 

/* number of false clauses for the worse individual in the population */
int worsetemporarynumfalse; 

/* identification of the older individual */
int olderindividual; 

/************************************************************************************************/

/************************************************************************************************/
/* Variables for the benchmarks counter */

/* number of atoms in the set of CNF clauses */
int numatom; 

/* number of clauses in the set of CNF clauses */
int numclause; 

/* clauses to be satisfied,  indexed as clause[clause_num][literal_num] */
int** clause;  

/* a copy of "clause" but will be used for the UNSAT proof */
int** unsatClause;  

/* length of each clause */
int* size;  

/* where each literal occurs, indexed as occurence[literal+numatom][occurence_num] */
int** occurence;  

/* number of times each literal occurs */
int* numoccurence;

/* number of flips */
unsigned long long allflip;  

/************************************************************************************************/
/* Variables for the algorithm counter */

/* 0 if no solution found else 1 */
int FOUND; 

/* it used to select the BESTINDIVIDUAL individual but whit a minimal diversity */
char ** barycentre; 

/* number of tries between the start (for MAXTRY downto 0) */
int maxtry; 

/************************************************************************************************/


/************************************************************************************************/
/* Structure for an Individual */

typedef struct struct_individual {

  /* value of each atom */ 
  char *atom;                       

  /* clauses which are false */
  int *clausesFalse;           

  /* number of true literals in each clause */     
  int *numtruelit;                  

  /* where each clause is listed in false */
  int *wherefalse;  

  /* number of clauses that become unsat if var is flipped */                
  int *breakcount;

  /* number of clauses that become sat if var is flipped */  
  int *makecount; 

  /* number of false clauses */
  int numfalse;       

  /*indentification of the individual*/ 
  int numindividual;    

  /*number between 0 to 99*/
  int numhamming;  

  /* pointer on the next element of the linked list */     
  struct struct_individual *next;

} Individual;

/************************************************************************************************/

#endif
