/**************************************************************************************************
Copyright (c) 2014, SATyr - Valentin Montmirail - Polytech Tours, France

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#include <time.h>
#include <signal.h>

#include "Headers/Genetic_Solver.h"

#define NB_INDIVIUAL 5000

double parsingTime;

double solvingTime;

Problem problem;

Genetic_Solver* genetic_solver;

/**
 * Useful to display some informations.
 */
void displayInfo(FILE* std,Problem * problem) {

  fprintf(std,"\n\n");
  fprintf(std,"c This is SATyr \n");
	fprintf(std,"c Created by Valentin Montmirail, a student in computer engineering at Polytech Tours\n");
	fprintf(std,"c \n");
  fprintf(std,"c /====================================================================================\\\n");
  fprintf(std,"c | Number of Variables  :    %5d                                                     |\n",problem->nbVariables);
  fprintf(std,"c | Number of Clauses    :    %5d                                                     |\n",problem->nbClauses);
  fprintf(std,"c |-------------------------------------------------------------------------------------|\n");
  fprintf(std,"c | Parsing time         :   %10.3fms                                               |\n",parsingTime);
  fprintf(std,"c |=====================================================================================|\n");
}

/**
 * Useful to display some error during the parsing of the CNF file.
 */
void displayErrorArgument(FILE* std) {

  	fprintf(std,"\n");
  	fprintf(std,"\tYou should give the path to the CNF file formated as follow :  \n\n");
  	fprintf(std,"\tp cnf nbVariables nbClauses\n");
  	fprintf(std,"\txi xj xk ... 0\n");
  	fprintf(std,"\txi xl ... 0 \n");
  	fprintf(std,"\n\tso your call looks like : ./satyr path_to_file\n\n");
}


void displaySolveTime(FILE* std) {

  if(isSolutionG(genetic_solver,&problem) == 1) {
    fprintf(std,"\ns   SATISFIABLE\n");
    fprintf(std,"v   ");
    displaySolutionG(genetic_solver,&problem);
    fprintf(std,"c |-------------------------------------------------------------------------------------|\n");
    fprintf(std,"c   Solving Time         : %10.3fms\n",solvingTime);

  } else {

    fprintf(std,"\ns UNSATISFIABLE\n");

  }

  fprintf(std,"c \\====================================================================================/\n\n");
}

/**
 * Useful to catch the SIGINT signal
 * in that way, we can shut down the solver and display that we don't know if the problem
 * is SATISFIABLE or UNSATISFIABLE...
 * The solver will so, display UNKNOWN for the state of the result.
 */
void signalHandler( int signum )
{

    printf("\ns   UNKNOWN\n");
    printf("v   ");
    unsigned int i = 0;
    for(i = 0; i < genetic_solver->population[0]->nbVariables; ++i) {
      printf("? ");
    }
    printf("\n");
    printf("c |-------------------------------------------------------------------------------------|\n");
    printf("c   Solving Time         :   infinity \n");
    printf("c \\====================================================================================/\n\n");
    exit(signum);  
}


/**
 * It's the beginning of this program.
 * @param argc the number of argument for the main fonction
 * @param argv each argument in a char*
 * @return a value between [-127 ; 128] to explain the exit of the program.
 */
int main(int argc,char** argv)
{
  /* A SIGINT signal should stop the program and close everything already open. */
  signal(SIGINT, signalHandler);  

  srand(time(NULL));

  clock_t start;
  clock_t   end;

  if(argc < 2) {
    /* If we go there, that's mean that there is no argument during the call of the solver...*/
    displayErrorArgument(stderr);
    exit(-1);
  }

  start = clock();
    initProblem(&problem,argv[1]);
  end   = clock();

  parsingTime = (double)(end-start)/(CLOCKS_PER_SEC/1000);

  displayInfo(stdout,&problem);

  genetic_solver = initGeneticSolver(&problem,NB_INDIVIUAL);

  start = clock();
    while(isSolutionG(genetic_solver,&problem) != 1) {
      solveProblemG(genetic_solver,&problem);
    }
  end   = clock();

  solvingTime = (double)(end-start)/(CLOCKS_PER_SEC/1000);

  displaySolveTime(stdout);

  return 0;
}
