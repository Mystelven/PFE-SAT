#include "problem.h"

Problem* createProblem(unsigned int nbJ, unsigned int nbS) {

	Problem* problem = (Problem*)malloc(sizeof(Problem)*1);

	problem->nbShops = nbS;
	problem->nbJobs  = nbJ;

	problem->resources = (Resource**)malloc(sizeof(Resource*)*nbS);

	for(unsigned int i = 0; i < nbJ; i++) {
		problem->resources[i] = (Resource*)malloc(sizeof(Resource)*nbJ);
	}

	return problem;
}

void addTime(Problem* problem, unsigned int numShop, unsigned int numJob, unsigned int value) {


	problem->resources[numShop][numJob].numShop = numShop;
	problem->resources[numShop][numJob].numJob  = numJob;
	problem->resources[numShop][numJob].value   = value;

}


void displayProblem(Problem* problem) {

	printf("\n");
	printf("\tNbShops : %5d\n",problem->nbShops);
	printf("\tNbJobs  : %5d\n\t",problem->nbJobs);

	for(unsigned int i = 0; i < problem->nbShops; i++) {

		for(unsigned int j = 0; j < problem->nbJobs; j++) {

			displayResource(&problem->resources[i][j]);
			printf(" ");
		}

		printf("\n\t");
	}
	printf("\n");
}
