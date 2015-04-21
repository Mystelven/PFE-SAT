#ifndef CNF_UTILS_H
#define CNF_UTILS_H

#include "utils.h"

const char* createCNF(Problem* problem,unsigned int max);

unsigned int getNbVariables(Problem* problem,unsigned int max);

unsigned int getIdOfVariable(Problem* problem,unsigned int numShop,unsigned int numJob,unsigned int valueTime,unsigned int C_MAX);

unsigned int writeConstraintForContinuityOfJobs(Problem* problem,unsigned int timeMax,FILE* file,char forReal);

unsigned int writeConstraintForOneShopHandleOneJob(Problem* problem, unsigned int timeMax, FILE* file,char forReal);

unsigned int writeConstraintForResourceOnOneShop(Problem* problem, unsigned int timeMax, FILE* file,char forReal);

unsigned int writeConstraintAtLeastOneJob(Problem* problem, unsigned int timeMax, FILE* file,char forReal);

/**
 * This function allow to get in an integer array, the id of all the intervals solutions.
 * @param s the filename of the solution.
 * @param problem the problem who contains the all problem.
 * @return an array of integer who contains the ID of intervals solutions.
 */
 unsigned int* getSolution(Problem* problem,const char* solution,unsigned int max);

 /**
 * getNbConstraint : This function allows us to get the number of clauses.
 * @return the number of clauses in the CNF file.
 */
 unsigned int getNbConstraint(Problem* problem,unsigned int max);

#endif
