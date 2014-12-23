#ifndef GENETIC_SOLVER_H
#define GENETIC_SOLVER_H

#include "Solver.h"

/** 
 * We define our structure for our Genetic_Solver.
 * It's composed from a number of individual
 * and the population of solver.
 */
struct _Genetic_Solver {

	/* We have to know how many individual are inside the population. */
	unsigned int nbIndividual;

	/* Every individual is an Solver*, so we need an array of it. */
	Solver** population;

};

/** We rename our structure to avoid to tape struct _Genetic_Solver everywhere... */
typedef struct _Genetic_Solver Genetic_Solver;


/**
 * This function corresponds to the constructor of the "class" Genetic_Solver
 * @param problem the Genetic Solver will need the definition of the Problem to initialize every individual.
 * @param nb it's the number of individual inside the population.
 * @return a pointer to a full initialized Genetic_Solver.
 */
Genetic_Solver* initGeneticSolver(Problem* problem,unsigned int nb);


/**
 * This function will check if one individual inside the population is the solution of the problem.
 * @param solver the global population of the solver.
 * @param problem the definition of the problem.
 * @return true if one individual is solution, false otherwise.
 */
unsigned int isSolutionG(Genetic_Solver* solver, Problem* problem);


/**
 * This function will check if there is one individual solution inside the population,
 * and it will display it if it is the case.
 * @param solver, the global poppulation of the solver.
 * @param problem the definition of the solver.
 */
void displaySolutionG(Genetic_Solver* solver,Problem* problem);


/**
 * This function will solve the problem, it will use principle of genetic algorithm.
 * @param solver the global population of the solver.
 * @param problem the definition of the problem.
 */
void solveProblemG(Genetic_Solver* solver, Problem* problem);

/**
 * This function will perform crossing inside the population with the most adapted individual.
 * @param solver the global population of the solver.
 * @param problem the definition of the problem.
 */
void crossing(Genetic_Solver* solver,Problem* problem);


/**
 * This function will perform a mutation on one random individual inside the population.
 * @param solver the global population of the solver.
 * @param problem the definition of the problem.
 */
void mutation(Genetic_Solver* solver);


/**
 * This function will compute the fitness of individual to know who is the most adapted.
 * @param solver the global population of the solver.
 * @param problem the definition of the problem.
 * @return the fitness of the population.
 */
int fitness(Genetic_Solver* solver,Problem* problem);


void reproduction(Genetic_Solver* solver,Problem * problem);


void _crossing(Genetic_Solver* solver,Problem* problem,unsigned int p1, unsigned int p2);


#endif
