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

#include "display.h"
#include "unsat.h"

/************************************************************************************************/
/*																								*/
/* main : This function represents the main of the program										*/
/* @param argc the number of argument passed to the program										*/
/* @param argv every char* for every argument 													*/
/* @return 0 if everything was good, something else otherwise 									*/
/*																								*/
/************************************************************************************************/
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
	parsingTime = (double)(clock()-(unsigned long)begintime)/(CLOCKS_PER_SEC/1000);

	/* We will display all the informations about the benchmark and things. */
	displayInfo(stdout);

	/* We, for now, didn't find a solution (or the absence of solution) yet. */
	FOUND = FALSE; 
	
	cardpopulation    = 0;   
	bestnumfalse      = numclause;
	allflip           = 0;
	NUMINDIVIDUAL     = 100;
	
	double p = rand() % 1000;
	maxtry = (int)(numatom*p)/100;
  	MAXTRY = (int)(numatom*p)/100;  		

	/* and we get the time of "right now" */
	times(a_tms); 	

	/* After this, we start to count how many times we need to solve the problem */
	begintime = (long)a_tms->tms_utime;

	/* Creation of the population */
	Individual* population = createPopulation();

	youngerindividual = cardpopulation-1;

	/* We perform an initial sort */
	population = initial_sort (population); 

	resolution(0,1,2);

	resolution(4,2,1);

	resolution(5,3,3);

	/* We will search after a solution. */
	while ( (maxtry != 0) && (FOUND != SAT) && (FOUND != UNSAT))  {
			
		/* At every step, we display statistic informations. */
		displayStat();

		/* We perform a crossover on the population. */
		crossover_operator(&population);

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
