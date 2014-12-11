#ifndef SOLVER_H
#define SOLVER_H

#include "Problem.h"

/** 
 * We define our structure for our Solver.
 * a solver will propose an array of solution, 
 * and we will test on the Problem if it's correct
 */
struct _Solver {

	unsigned int nbVariables;

	int* arrayOfSolutions;

	double fitness;
};

/** We rename our structure to avoid to tape struct _Solver everywhere... */
typedef struct _Solver Solver;

void computeFitness(Solver* solver,Problem* problem);

Solver* initSolver(Problem* problem);

unsigned int isSolution(Solver* solver, Problem* problem);

void randomMove(Solver* solver);

unsigned int isOpen(Solver* solver, Clause* clause);

void displaySolution(Solver* solver);

#endif

