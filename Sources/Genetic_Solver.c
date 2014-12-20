#include "../Headers/Genetic_Solver.h"
#include "./Utils.c"

#define MUTATION_PERCENT 80

double SEUIL = 0.80;

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
	
	for(i = 0; i < solver->nbIndividual; ++i) {

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

Solver* creation(Solver* parent) {

	Solver* res = recopy(parent);

	if((rand() % 100) < MUTATION_PERCENT) {
		randomMove(res);	
	}

	return res;
}

void _crossing(Genetic_Solver* solver,Problem* problem,unsigned int p1, unsigned int p2) {

	int temp;

	unsigned int crossLine = (unsigned int)rand() % problem->nbVariables;

	for(unsigned int i = crossLine; i < problem->nbVariables; ++i) {

		temp 									    = solver->population[p1]->arrayOfSolutions[i];
		solver->population[p1]->arrayOfSolutions[i] = solver->population[p2]->arrayOfSolutions[i];
		solver->population[p2]->arrayOfSolutions[i] = temp;
	}

}

inline void reproduction(Genetic_Solver* solver,Problem * problem) {

	unsigned int i = 0;

	for(i = solver->nbIndividual-1; i > 0; --i) {
		
		if(solver->population[i]->fitness >= SEUIL) {
			break;
		}
	}

	unsigned int j = solver->nbIndividual-1;
	
	Solver* temp;

	for(unsigned int k = 0; k < i-1 ; ++k) {

		temp = solver->population[k+1];
		solver->population[k+1] = creation(solver->population[k]);
		destroy(temp);

		temp = solver->population[k];
		solver->population[k] = creation(solver->population[j]);
		destroy(temp);
		
		_crossing(solver,problem,k,k+1);
		j--;
	}

}

void solveProblemG(Genetic_Solver* solver, Problem* problem) {
	
	int res = fitness(solver,problem);	

	if(res == 1) return ;

	reproduction(solver,problem);

	crossing(solver,problem);

	if((rand() % 100) < MUTATION_PERCENT) mutation(solver);	
	
}

inline void crossing(Genetic_Solver* solver,Problem* problem) {

	unsigned int p1 = (unsigned int)rand()  % solver->nbIndividual;

	unsigned int p2 = (unsigned int)rand()  % solver->nbIndividual;

	_crossing(solver,problem,p1,p2);

}

inline int fitness(Genetic_Solver* solver,Problem* problem) {

	unsigned int i = 0;

	double sum = 0;

	int res = 0;

	for(i = 0 ; i < solver->nbIndividual; ++i) {

		computeFitness(solver->population[i],problem);	

		sum += solver->population[i]->fitness;

		if(solver->population[i]->fitness  == 1.0) {

			res = 1;
			break;
		}
		
	}

	sort(solver);

	SEUIL = (sum/solver->nbIndividual)-0.001;

	return res;
}

inline void mutation(Genetic_Solver* solver) {

	unsigned int i = (unsigned int)rand() % solver->nbIndividual;

	randomMove(solver->population[i]);
}
