#include "../Headers/Clause.h"


inline void initClause(Clause* clause) {

	/* We affect the clauseSize to our number of variable in the clauses. */
	clause->nbVariables = 0;

	clause->arrayOfVariables = NULL;

}


inline void displayClause(Clause* clause) {

	/* To display a clause basically means that for every variable inside the clause. */
	for(unsigned int i = 0; i < clause->nbVariables; ++i) {

		/* We display the variable inside. */
		displayVariable(&clause->arrayOfVariables[i]);
	}
}

inline void addVariable(Clause* clause, Variable* variable) {

	clause->nbVariables++;

	if(clause->arrayOfVariables == NULL) {

		/* We allocate the good space for our array of variables. */
		clause->arrayOfVariables = (Variable*)malloc(sizeof(Variable)*clause->nbVariables);

	} else {

		/* We allocate the good space for our array of variables. */
		clause->arrayOfVariables = (Variable*)realloc(clause->arrayOfVariables,sizeof(Variable)*clause->nbVariables);
	}

	/* We test our malloc result. */
	if(clause->arrayOfVariables == NULL) {

		/* We want to be sure that the malloc didn't fail. */
		perror("Problem with the allocation of variables in addVariable(Clause* clause, Variable* variable");
		exit(-2);
	}

	clause->arrayOfVariables[clause->nbVariables-1] = *variable;
}
