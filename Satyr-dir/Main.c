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

#include "display.h"
#include "unsat.h"

/**
 *																								
 * main : This function represents the main of the program										
 * @param argc the number of argument passed to the program										
 * @param argv every char* for every argument 													
 * @return 0 if everything was good, something else otherwise 									
 *
 */
int main(int argc,char *argv[]) {
	
	int i;
	
	/* If we don't even have the path for the CNF, something is wrong... */
	if (argc == 1) {
		
  		printf("\n");
  		printf("\tYou should give the path to the CNF file formated as follow :  \n\n");
  		printf("\tp cnf nbVariables nbClauses\n");
  		printf("\txi xj xk ... 0\n");
  		printf("\txi xl ... 0 \n");
  		printf("\n\tso your call looks like : ./satyr path_to_file\n\n");
		exit(0);
	} 
	
	/* a signal should stop the program and close everything already open. */
  	signal(SIGINT,  signalHandler);  
  	signal(SIGTERM, signalHandler);  
  	signal(SIGQUIT, signalHandler);  
  	signal(SIGALRM, signalHandler);
		
	/* for the random seed, we get the pid of the program. */
	seed = getpid();
	
	/* We initialize this program with this seed. */
	srandom((unsigned int)seed);
	
	/* We will parse every arguments, if something is not correct, we shut down the program. */
	if (parameters(argv) != 0) {

		printf("\n");
		printf("Something is incorrect with the parsing of arguments\n");
		exit(0);  
	}

	/* We initalize the time structure. */
	a_tms = ( struct tms *) malloc( sizeof (struct tms));

	/* and we get the time of "right now" */
   	times(a_tms); 	
	
	begintime = (long)a_tms->tms_utime;
		initprob();
	parsingTime = (double)(clock()-(long unsigned int)begintime)/(long unsigned int)(CLOCKS_PER_SEC/1000);

	/* We will display all the informations about the benchmark and things. */
	displayInfo(stdout);

	/* We, for now, didn't find a solution (or the absence of solution) yet. */
	FOUND = FALSE; 
	
	cardpopulation    = 0;   
	bestnumfalse      = numclause;
	allflip           = 0;
	NUMINDIVIDUAL     = MIN( (numatom/10)+10 , 100);
	
	double p = (rand() % 1000)+500;
	maxtry = ((int)(numatom*p)/2);
  	MAXTRY = ((int)(numatom*p)/2);

  	i = 0;
  	int j;

	/* Allocation of the big array to know which resolution we should perform. */
 	resolutionTable = (int**)malloc(sizeof(int*) * (unsigned long)(2*numclause));
 	for(i = 0; i < numclause*2; ++i) resolutionTable[i] = (int*)malloc(sizeof(int) * (unsigned long)(2*numclause));
 	
 	for(i = 0; i < numclause*2; ++i) {
 		for(j = 0; j < numclause*2; ++j) {
 			resolutionTable[i][j] = BIG;
 		}
 	}

	/* and we get the time of "right now" */
	times(a_tms); 	

	/* After this, we start to count how many times we need to solve the problem */
	begintime = (long)a_tms->tms_utime;

	/* Creation of the population */
	Individual* population = createPopulation();

	youngerindividual = cardpopulation-1;

	/* We perform an initial sort */
	population = initial_sort (population); 

	clausesResolutions = (int*)(malloc(sizeof(int)*(unsigned long)(numclause*2)));

	initResolutionTable();

	/* We will search after a solution. */
	while ( (FOUND != SAT) && (FOUND != UNSAT))  {
		
		#ifndef BENCHMARK
			
			/* When we are on training-mode, no need to stay forever... */
			if(maxtry == 0) {
				break;
			}

			displayStat();

		#endif

		/* We perform a crossover on the population. */
		if(random() % 100 > 50) crossover_operator(&population);

		else  			  		tryToProveUNSAT();

		/* We didn't success, we go on the next try. */
		maxtry--;
	}

		
	/* We display all the statistics. */
	displayStat();

	/* We get the time of "right now" */
	times(a_tms); 

	/* We finally get a solution, we know how many times it took to solve the problem. */
	endtime = (long)a_tms->tms_utime;

	/* We display the final result of this problem. */
	displayFinalResult(population);
			
	/* We free all the memory we used. */
	for(i=0; i< BESTINDIVIDUAL; ++i) free(barycentre[i]);

	free(barycentre);

	return 0;
}
