#include "../Headers/Clause.h"


void initClause(Clause* clause, unsigned int clauseSize) {

	/* We affect the clauseSize to our number of variable in the clauses. */
	clause->nbVariables = clauseSize;
	
	/* We allocate the good space for our array of variables. */
	clause->arrayOfVariables = (Variable*)malloc(sizeof(Variable)*clause->nbVariables);

	/* We test our malloc result. */
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
