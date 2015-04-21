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

/** 
 *																								
 * displayInfo : useful to display some informations 			   								
 * @param std the output where we will write informations 		   																																
 */
inline void displayInfo(FILE* std) {

	fprintf(std,"\n\n");
	fprintf(std,"c This is SATyr \n");
	fprintf(std,"c Created by Valentin Montmirail, a student in computer engineering at Polytech Tours\n");
	fprintf(std,"c \n");

		
	fprintf(std,"c /====================================================================================\\\n");
	fprintf(std,"c | Number of Variables  :    %5d                                                     |\n",numatom);
	fprintf(std,"c | Number of Clauses    :    %5d                                                     |\n",numclause);
	fprintf(std,"c |-------------------------------------------------------------------------------------|\n");
	fprintf(std,"c | Parsing time         :   %7.3fms                                                  |\n",parsingTime);
	fprintf(std,"c |=====================================================================================|\n");
}

/************************************************************************************************/

/**
 *
 * signalHandler: Useful to catch the SIGINT signal in that way,
 * we can shut down the solver and display that we don't know if the problem
 * is SATISFIABLE or UNSATISFIABLE...
 * The solver will so, display UNKNOWN for the state of the result.
 * @param signum the identification number of the signal
 *
 */
inline void signalHandler(int signum) {

	pthread_kill(thread,signum);
	pthread_kill(display,signum);

    printf("\ns   INDETERMINATE\n");
    printf("v   ");
    
    int i = 0;
    for(i = 0; i < numatom; ++i) { printf("? "); }
    printf("0");
    printf("\n");
    printf("c |-------------------------------------------------------------------------------------|\n");
	printf("c | Time 	         :         ???? seconds");
	printf("           				|\n");
	printf("c | After                :    %10llu flips",allflip);
	printf("           				|\n");
	printf("c | Best solution with   :  %5d false clauses",bestnumfalse);
	printf("           				/\n");
    printf("c \\====================================================================================/\n\n");

    exit(signum);  
}

/************************************************************************************************/


/**
 *																						
 * displayStat : We will display every generation to see the progression.				
 *
 */
inline void displayStat() {

	printf("c | Gener : %4d | ",(MAXTRY-maxtry)+1);
	printf("best :  %3d    | ",bestnumfalse);
	printf("resolutions : %7d | ",numresolution);
	printf("flips: %7llu flips       ",allflip);
	printf(" |\n");
}

/************************************************************************************************/


/**
 *																								
 * displayInfo : useful to display the final result after solving this problem 					
 * @param population the population we used to solve this problem
 *
 */
inline void displayFinalResult(Individual* population) {

	if (FOUND == SAT) {

		printf("c |-------------------------------------------------------------------------------------|\n");

		printf("s   SATISFIABLE\n");

		displaySolution(population);

		printf("c |-------------------------------------------------------------------------------------|\n");
		printf("c | Solving Time 	 :   %5.4f seconds",(double)(endtime-begintime)/CLOCK_TICK);
		printf("           					|\n");
		printf("c | After                : %10llu flips",allflip);
		printf("           					|\n");
		printf("c | Generation number    : %3d",(MAXTRY-maxtry)+1);
		if((MAXTRY-maxtry)+1 == 1)
			printf("st generation   ");
		else if((MAXTRY-maxtry)+1 == 2)
			printf("nd generation   ");
		else if((MAXTRY-maxtry)+1 == 3)
			printf("rd generation   ");
		else
			printf("th generation   ");
		printf("           				|\n");
		printf("c | With                 : %4d individuals",NUMINDIVIDUAL);
		printf("           					|\n");
		printf("c \\====================================================================================/\n\n");
	}

	else if(FOUND == UNSAT) {

		printf("c |-------------------------------------------------------------------------------------|\n");

		printf("s   UNSATISFIABLE\n");

		printf("c |-------------------------------------------------------------------------------------|\n");
		printf("c | Solving Time     :   %5.4f seconds",(double)(endtime-begintime)/CLOCK_TICK);
		printf("           				|\n");
		printf("c | After                : %10llu flips",allflip);
		printf("           					|\n");
		printf("c | Generation number    : %3d",(MAXTRY-maxtry)+1);
		if((MAXTRY-maxtry)+1 == 1)
			printf("st generation   ");
		else if((MAXTRY-maxtry)+1 == 2)
			printf("nd generation   ");
		else if((MAXTRY-maxtry)+1 == 3)
			printf("rd generation   ");
		else
			printf("th generation   ");
		printf("           				|\n");
		printf("c | With                 : %4d individuals",NUMINDIVIDUAL);
		printf("           					|\n");
		printf("c | Best solution with   :  %5d false clauses",bestnumfalse);
		printf("           				/\n");
		printf("c \\====================================================================================/\n\n");
		
	} else {

		signalHandler(-1);
	}
}

/**
 *                                                                                   
 * displaySolution: this function allows us to display the solution of the problem.  
 * @param pop    Represents the population where one of individual is solution       
 * 
 */
inline void displaySolution(Individual* pop) {

  /* We get the solution of the problem. */
  Individual* s = getSolution(pop);

  /* We start the solution with a 'v' as expected by the SAT norm. */
  printf("v ");

  /* For every variables inside the solution. */
  for(int i = 1; i < numatom+1 ; ++i) {

    /* We display the number i  when the value is true. */
    if(s->atom[i] != 0) printf("%d ",   i);

    /* We display the number -i when the value is false. */
    else                printf("%d ",-1*i);

  }
  printf("0");
  printf("\n");
}

