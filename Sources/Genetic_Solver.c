#include "../Headers/Genetic_Solver.h"

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


unsigned int isSolutionG(Genetic_Solver* solver, Problem* problem) {

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

	mutation(solver);
}

void crossing(Genetic_Solver* solver) {



}

double fitness(Genetic_Solver* solver) {

	return 0;
}

void mutation(Genetic_Solver* solver) {

	unsigned int i = (unsigned int)rand() % solver->nbIndividual;

	for(i = 0; i < solver->nbIndividual; i++) {
		randomMove(solver->population[i]);
	}
}
