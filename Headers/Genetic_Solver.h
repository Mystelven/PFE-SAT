#ifndef GENETIC_SOLVER_H
#define GENETIC_SOLVER_H

#include "Solver.h"

/** 
 * We define our structure for our Genetic_Solver.
 * It's composed from a number of individual
 * and the population of solver.
 */
struct _Genetic_Solver {

	unsigned int nbIndividual;

	Solver** population;

};

/** We rename our structure to avoid to tape struct _Genetic_Solver everywhere... */
typedef struct _Genetic_Solver Genetic_Solver;

Genetic_Solver* initGeneticSolver(Problem* problem,unsigned int nb);

unsigned int isSolutionG(Genetic_Solver* solver, Problem* problem);

void displaySolutionG(Genetic_Solver* solver,Problem* problem);

void solveProblemG(Genetic_Solver* solver, Problem* problem);

void crossing(Genetic_Solver* solver);

void mutation(Genetic_Solver* solver);

double fitness(Genetic_Solver* solver);

#endif
