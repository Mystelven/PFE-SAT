#ifndef SOLVER_H
#define SOLVER_H


/** 
 * We define our structure for our Solver.
 * a solver will propose an array of solution, 
 * and we will test on the Problem if it's correct
 */
struct _Solver {

	unsigned int nbVariables;

	int* arrayOfSolutions;

};

/** We rename our structure to avoid to tape struct _Solver everywhere... */
typedef struct _Solver Solver;




#endif
