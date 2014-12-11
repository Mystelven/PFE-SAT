#include "../Headers/Genetic_Solver.h"

#define MUTATION_PERCENT 100

#define CROSSING_PERCENT 100

Genetic_Solver* initGeneticSolver(Problem* problem,unsigned int nb) {

	Genetic_Solver* result = (Genetic_Solver*)malloc(sizeof(Genetic_Solver));
	result->nbIndividual = nb;
		
	result->population = (Solver**)(malloc(sizeof(Solver*)*nb));

	unsigned int i = 0;
	for(i = 0; i < nb; i++) {

		result->population[i] = initSolver(problem);
	}

	return result;
}


inline unsigned int isSolutionG(Genetic_Solver* solver, Problem* problem) {

	unsigned int i = 0;
	
	for(i = 0; i < solver->nbIndividual; i++) {

		if(isSolution(solver->population[i],problem) == true) return true;
	}

	return false;
}


void displaySolutionG(Genetic_Solver* solver,Problem* problem) {

	unsigned int i = 0;

	for(i = 0; i < solver->nbIndividual; i++) {

		if(isSolution(solver->population[i],problem)) {

			displaySolution(solver->population[i]);
			return ;
		}	
	}

}

void solveProblemG(Genetic_Solver* solver, Problem* problem) {

	unsigned int i;

	// fitness(solver,problem);

	i = (unsigned int)rand() % 100;

	if(i < MUTATION_PERCENT) {
		mutation(solver,problem);	
	}

	crossing(solver,problem);
}

inline void crossing(Genetic_Solver* solver,Problem* problem) {

	unsigned int p1 = (unsigned int)rand() % problem->nbVariables;
	unsigned int p2 = (unsigned int)rand() % problem->nbVariables;

	int temp;

	unsigned int crossLine = (unsigned int)rand() % problem->nbVariables;

	for(unsigned int i = crossLine; i < problem->nbVariables; ++i) {

		temp 									    = solver->population[p1]->arrayOfSolutions[i];
		solver->population[p1]->arrayOfSolutions[i] = solver->population[p2]->arrayOfSolutions[i];
		solver->population[p2]->arrayOfSolutions[i] = temp;
	}

}

inline void fitness(Genetic_Solver* solver,Problem* problem) {

	unsigned int i = 0;

	for(i = 0 ; i < solver->nbIndividual; ++i) {
		computeFitness(solver->population[i],problem);
	}
}

inline void mutation(Genetic_Solver* solver,Problem* problem) {

	unsigned int i = (unsigned int)rand() % problem->nbVariables;

	randomMove(solver->population[i]);
}
