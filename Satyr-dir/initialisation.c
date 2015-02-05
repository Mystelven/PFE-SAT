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

#include "initialisation.h"

/**
 *																								
 * parameters : read all the user parameters 													
 * @param argv every char* for every argument 													
 * @return 0 if everything was good, something else otherwise 									
 *
 */
inline int parameters(char* argv[]) {

  /* -------------------------------- */

  /* Initial tabou research */
  ltinit  =  -1;
  ltinitpc =  0;

  /* Real tabou research */
  ltrech   = -1;
  ltrechpc =  0;

  /* -------------------------------- */

  maxtry = 999;
  MAXTRY = 999;

  /* Useful for the barycentre's calcul. */
  BESTINDIVIDUAL =  20;

  /* number of individuals inside the population. */
  NUMINDIVIDUAL  = 100;

  /* The first argument is the filename of the problem. */
  benchmark_name = argv[1];

  /* -------------------------------- */

  if (benchmark_name == NULL) {
	
	printf("\n");
  	printf("\tYou should give the path to the CNF file formated as follow :  \n\n");
  	printf("\tp cnf nbVariables nbClauses\n");
  	printf("\txi xj xk ... 0\n");
  	printf("\txi xl ... 0 \n");
  	printf("\n\tso your call looks like : ./satyr path_to_file\n\n");
	exit(0);

  }
	
  return 0;
}


/**
 *																							
 * initprob : read the benchmark file 															
 *
 */
inline void initprob() {

	int i;
	int j;

	int lastc;
	int nextc;
	
	int* storeptr = NULL;
	int freestore;
	
	int lit;

	FILE * bench_file;

	/* ------------------------------------------------------------------------------------ */
	
	/* We open the file in the read-mode, we don't need other rights. */
	if ((bench_file = fopen(benchmark_name, "r")) == NULL) {

		fprintf(stderr, "We can't open the file: %s\n", benchmark_name);
		exit(1);
	}		

	/* We skip the commentaries */
	while ((lastc = fgetc(bench_file)) == 'c') {

		while ((nextc = fgetc(bench_file)) != EOF && nextc != '\n') {

		}
	}

	ungetc(lastc,bench_file);

	/* We store the number of atoms and the number of clauses. */
	if (fscanf(bench_file,"p cnf %i %i",&numatom,&numclause) != 2) {

		fprintf(stderr,"The input inside the file is not correct\n");
		fprintf(stderr,"the first useful line should be: p cnf numVariables numClauses\n");
		exit(-1);
	}

	/* ------------------------------------------------------------------------------------ */
	
	/* We initialize all the structures. */
	if(( ltinitpc != 0) && (ltinit == -1)) ltinit   = numatom*ltinitpc/100;
	if(( ltinitpc == 0) && (ltinit != -1)) ltinitpc = ltinit*100/numatom;

	if(( ltinitpc == 0) && (ltinit == -1)) {
		ltinitpc = 5;
		ltinit   = (int)(0.05 * numatom);
	}

	if(( ltrechpc != 0) && (ltrech == -1)) ltrech   = numatom*ltrechpc/100;
	if(( ltrechpc == 0) && (ltrech != -1)) ltrechpc = ltrech*100/numatom;

	if(( ltrechpc == 0) && (ltrech == -1)) {
		ltrechpc = 10;
		ltrech   = (int)(0.1 * numatom);
	}

	/* ------------------------------------------------------------------------------------ */

	numoccurence = (int * ) malloc( sizeof(int  ) * (unsigned long)(2*numatom+1   ));
	occurence    = (int **) malloc( sizeof(int *) * (unsigned long)(2*numatom+1   ));

	/* We allocate twice the size needed in order to perform easily resolution proof for UNSAT. */
	clause       = (int **) malloc( sizeof(int *) * (unsigned long)(2*numclause   ));
	size         = (int * ) malloc( sizeof(int  ) * (unsigned long)(2*numclause   ));
	
	barycentre   = (char**) malloc( sizeof(char*) * (unsigned long)(BESTINDIVIDUAL));

	for( i = 0 ; i < BESTINDIVIDUAL ; ++i) {

		barycentre[i] = malloc( sizeof(char) * (unsigned long) (numclause));
	}
	
	/* ------------------------------------------------------------------------------------ */

	/* Storage work */
	freestore = 0;

	for( i = 0 ; i < (2*numatom+1) ; ++i) {
		numoccurence[i] = 0;
	}

	for( i = 0 ; i < numclause ; ++i) {

		size[i] = -1;

		if (freestore < MAXLENGTH) {

			storeptr      = (int *) malloc( sizeof(int) * STOREBLOCK );
			freestore = STOREBLOCK;

		}

		clause[i]      = storeptr;

		
		/* For every clauses, we will store all the variables */
		do {

			++size[i];

			if(size[i] > MAXLENGTH) {
				printf("ERROR - One clause has more than %d variables (change the code if it's not an error)\n",MAXLENGTH);
				exit(-1);
			}

			if (fscanf(bench_file,"%i ",&lit) != 1) {

				fprintf(stderr,"The input inside the file is not correct\n");
				fprintf(stderr,"every clause line should be: vi vi_1 vi_2 ... 0 \n");
				exit(-1);
			}

			/* because 0 is the last symbol of each line */
			if(lit != 0) { 

				/* clause[i][size[i]] = j; */
				*(storeptr++) = lit; 

				freestore--;
				
				++numoccurence[lit+numatom]; 
			}

		} while(lit != 0);
	}
	
	/* Something went wrong with the characters (or format) */
	if(size[0] == 0) {

		fprintf(stderr,"ERROR - incorrect problem format or extraneous characters\n");
		exit(-1);
	}

	/* space memory of occurence structure */
	for( i = 0 ; i < 2*numatom+1 ; ++i ) {

		if (freestore < numoccurence[i]) {

			storeptr = (int *) malloc( sizeof(int) * STOREBLOCK );
			freestore = STOREBLOCK;
		}

		occurence[i] = storeptr;

		freestore -= numoccurence[i];

		storeptr += numoccurence[i]; 
		
		numoccurence[i] = 0;
	}

	/* filling of occurence structure */
	for( i = 0 ; i < numclause ; ++i) {

		for( j = 0 ; j < size[i] ; ++j) {

			occurence[clause[i][j]+numatom][numoccurence[clause[i][j]+numatom]] = i;

			++numoccurence[clause[i][j]+numatom];
		}
		
	}


	fclose(bench_file);
}
