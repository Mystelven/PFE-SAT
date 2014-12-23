#include "../Headers/Problem.h"

#include <string.h>

#define MAX_SIZE 500

inline void allocationOfClause(Problem* problem,const char* chaine, unsigned int i) {

	char* ligne = strdup(chaine);

	char* element = strtok(ligne," ");

	Variable* variable = NULL;	

	while(element) {

		int id = atoi(element);

		if(id < 0) {
			
			variable = initVariable((unsigned int)(id*-1),false);
		
		}
		else if(id > 0) {
			
			variable = initVariable((unsigned int)id,true);
		}


		if(id != 0) {

			addVariable(&problem->arrayOfClauses[i],variable);

		}

		element = strtok(NULL," ");
	}

	free(ligne);
	free(element);
}

inline void allocateProblem(Problem* problem,const char* chaine) {

	/* We don't care about "p cnf" so we shift 5 chars. */
	char* ligne = strdup(chaine);

	char* element = strtok(ligne+5," ");

	unsigned int i = 0;

	while(element) {

	 	if(i == 0) {
	 		/* The first number is the number of variables. */
	 		problem->nbVariables = (unsigned int)atoi(element);

	 	} else if(i==1){

	 		/* The second number is the number of clauses. */
	 		problem->nbClauses = (unsigned int)atoi(element);
	 	}

	 	i++;
	 	element = strtok(NULL, " ");
	}

	problem->arrayOfClauses = (Clause*)malloc(sizeof(Clause)*problem->nbClauses);

	/* We test our malloc result. */
	if(problem->arrayOfClauses == NULL) {

		/* We want to be sure that the malloc didn't fail. */
		perror("Problem with the allocation of clauses in initProblem(Problem* problem, const char* filename)");
		exit(-2);
	}

	for(unsigned int boucle =0 ; boucle < problem->nbClauses; boucle++) {
		initClause(&problem->arrayOfClauses[boucle]);
	}

	free(ligne);
	free(element);
}

inline void initProblem(Problem* problem, const char* filename) {

	/* We will need a file to read the CNF instance and to create our Problem. */
	FILE* file = NULL;

	/* We need only the right to read the file, nothing more. */
	file = fopen(filename, "r");

	char chaine[MAX_SIZE] = "";

	unsigned int i = 0;

	if(file == NULL) {

	 	/* We didn't success to open the file, so we display an error message to help the user. */
	 	perror("The path you give to the CNF file is not correct");
	 	perror("You should verify that you have the right to read this file.");
	 	perror("Or if the file exists for real...");
	 	exit(-3);
	}

	while(fgets(chaine,MAX_SIZE,file) != NULL) {

	 	/* if the first character is a c, we don't care about it. */
	 	/* Because it's a commentary, and commentary are useful only for humans. */
	 	if(chaine[0] == 'c') {
	 		continue;
	 	}

	 	/** 
 		 * This line is really important, because it's where we define:
 		 * The format of Problem (CNF or not ?)
 		 * The number of variables in the Problem
 		 * The number of clauses in the Problem.
 	 	 */
	 	if(chaine[0] == 'p') {

	 		allocateProblem(problem, chaine);

	 	} else {

	 		allocationOfClause(problem,chaine,i);
	 		i++;
	 	}

	};

	fclose(file);
}

inline void displayProblem(Problem* problem) {

	for(unsigned int i = 0; i < problem->nbClauses; i++) {

		displayClause(&problem->arrayOfClauses[i]);
		printf("\n");
	}
}
