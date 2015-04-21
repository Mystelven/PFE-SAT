#ifndef PROBLEM_H
#define PROBLEM_H

#include "resource.h"

typedef struct _problem {

	unsigned int nbShops;
	unsigned int nbJobs;
	Resource**   resources;

} Problem;


Problem* createProblem(unsigned int nbJ, unsigned int nbS);

void addTime(Problem* problem, unsigned int numShop, unsigned int numJob, unsigned int value);

void displayProblem(Problem* problem);


#endif
