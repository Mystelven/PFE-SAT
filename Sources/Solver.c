#include "../Headers/Solver.h"

inline unsigned int isSolution(Solver* solver, Problem* problem) {

	for(unsigned int i = 0; i < problem->nbClauses; i++) {

		if(isOpen(solver,&problem->arrayOfClauses[i]) == false) {

			return false;
		}
	}

	return true;
}


inline void randomMove(Solver* solver) {

	unsigned int indiceRandom = (unsigned int)rand() % (solver->nbVariables);

	indiceRandom = (unsigned int)(indiceRandom * (unsigned int)rand() % (solver->nbVariables));

	solver->arrayOfSolutions[indiceRandom] *= -1;
	
}

inline Solver* initSolver(Problem* problem) {

	Solver* solver = (Solver*)malloc(sizeof(Solver)*1);
	solver->nbVariables = problem->nbVariables;

	solver->arrayOfSolutions = (int*)malloc(sizeof(int)*solver->nbVariables);
	if(solver->arrayOfSolutions == NULL) {

		perror("Allocation problem in Solver* initSolver(Problem* problem)");
		exit(-2);
	}

	for(unsigned int i = 1; i <= solver->nbVariables; i++) {
		solver->arrayOfSolutions[i-1] = (int)i;
	}

	return solver;
}


inline unsigned int isOpen(Solver* solver,Clause* clause) {

	for(unsigned int i = 0; i < clause->nbVariables; i++) {


		unsigned int id = clause->arrayOfVariables[i].id;
		unsigned int value = clause->arrayOfVariables[i].value;

		if(solver->arrayOfSolutions[id] >= 0 && value == true) return true;

		else if(solver->arrayOfSolutions[id] < 0 && value == false) return true;

	}

	return false;
}

inline void displaySolution(Solver* solver) {

	for(unsigned int i = 0 ; i < solver->nbVariables; i++) {

		printf("%d ",solver->arrayOfSolutions[i]);
	}
	printf("\n");
}

inline void computeFitness(Solver* solver,Problem* problem) {

	double _fitness = 0;

	for(unsigned int i = 0; i < problem->nbClauses; i++) {

		if(isOpen(solver,&problem->arrayOfClauses[i]) == true) {

			++_fitness;
		}
	}

	solver->fitness = (_fitness/problem->nbClauses);
}
