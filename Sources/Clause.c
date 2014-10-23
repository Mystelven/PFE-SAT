#include "../Headers/Clause.h"


void initClause(Clause* clause, unsigned int clauseSize) {

	clause->nbVariables = clauseSize;
	
	clause->arrayOfVariables = (Variable*)malloc(sizeof(Variable)*clause->nbVariables);
	if(clause->arrayOfVariables == NULL) {

		/* We want to be sure that the malloc didn't fail. */
		perror("Problem with the allocation of variables in initClause(Clause* clause, unsigned int clauseSize");
		exit(-2);
	}

}


void displayClause(Clause* clause) {

	/* To display a clause basically means that for every variable inside the clause. */
	for(unsigned int i = 0; i < clause->nbVariables; ++i) {

		/* We display the variable inside. */
		displayVariable(&clause->arrayOfVariables[i]);
	}
}
