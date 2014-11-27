#ifndef CLAUSE_H
#define CLAUSE_H

#include "Variable.h"

/** 
 * We define our structure for our clause.
 * a clause is defined as an array of nbVariables Variable.
 */
struct _Clause {

	unsigned int nbVariables;

	Variable* arrayOfVariables;

};

/** We rename our structure to avoid to tape struct _Clause everywhere... */
typedef struct _Clause Clause;

/**
 * This is the constructor of the "pseudo-class" 
 * @param clause the clause that we will initialize.
 */
void initClause(Clause* clause);

/**
 * To display a clause basically means: to display every variables inside.
 * @param clause the clause that we will display.
 */
void displayClause(Clause* clause);


void addVariable(Clause* clause, Variable* p);

#endif 
