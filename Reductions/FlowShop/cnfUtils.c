#include "cnfUtils.h"

#include <limits.h>


unsigned int getNbVariables(Problem* problem,unsigned int max) {

	return (max-1) * problem->nbShops * problem->nbJobs;
}

 /**
 * getNbConstraint : This function allows us to get the number of clauses.
 * @return the number of clauses in the CNF file.
 */
 unsigned int getNbConstraint(Problem* problem,unsigned int max) {

 	unsigned int nbClauses = 0;
 	nbClauses  = 0;
 	nbClauses += writeConstraintForContinuityOfJobs   (problem,max,NULL,0);
 	nbClauses += writeConstraintForResourceOnOneShop  (problem,max,NULL,0);
 	nbClauses += writeConstraintForOneShopHandleOneJob(problem,max,NULL,0);
 	nbClauses += writeConstraintAtLeastOneJob		  (problem,max,NULL,0);

 	return nbClauses;
 }

 unsigned int writeConstraintForContinuityOfJobs(Problem* problem, unsigned int timeMax, FILE* file,char forReal) {

 	unsigned int final = 0;
 	unsigned int max   = getNbVariables(problem,timeMax);

 	for(unsigned int i = 0; i < problem->nbShops ; i++) {

 		for(unsigned int j = 0; j < problem->nbJobs; j++) {

 			for(unsigned int t = 0; t < timeMax; t++) {

 				for(unsigned int k = 0; k < problem->resources[i][j].value-1; k++) {

 					if(k == t) continue;

 					unsigned int A = getIdOfVariable(problem,i,j,t  ,timeMax);
 					unsigned int B = getIdOfVariable(problem,i,j,k  ,timeMax);

 					if(A > max || B > max) continue;

					/* A <=> B : we need the task to be continuous ... 	*/
 					final += 2;

 					if(forReal == 1) {
 						fprintf(file,"-%d %d 0\n",A,B);
 						fprintf(file,"-%d %d 0\n",B,A);
 					}
 				}
 			}	
 		}
 	}

 	return final;
 }

 unsigned int writeConstraintAtLeastOneJob(Problem* problem, unsigned int timeMax, FILE* file,char forReal) {

 	unsigned int final = 0;
 	unsigned int max   = getNbVariables(problem,timeMax);

 	for(unsigned int i = 0; i < problem->nbShops ; i++) {

 		for(unsigned int j = 0; j < problem->nbJobs; j++) {

 			for(unsigned int t = 0; t < timeMax-1; t++) {

 				unsigned int A = getIdOfVariable(problem,i,j,t ,timeMax);

 				if(A > max) continue;

 				if(forReal == 1) {

 					fprintf(file,"%d ",A);
 				} 	
 			}

 			final ++;

 			if(forReal == 1) {

 				fprintf(file,"0\n");
 			}
 		}
 	}


 	return final;
 }

 unsigned int writeConstraintForResourceOnOneShop(Problem* problem, unsigned int timeMax, FILE* file,char forReal) {

 	unsigned int final = 0;
 	unsigned int max   = getNbVariables(problem,timeMax);

 	for(unsigned int i = 0; i < problem->nbShops ; i++) {

 		for(unsigned int l = 0; l < problem->nbShops ; l++) {

 			for(unsigned int j = 0; j < problem->nbJobs; j++) {

 				for(unsigned int t = 0; t < timeMax; t++) {

 					if(i == l) continue;

 					unsigned int A = getIdOfVariable(problem,i,j,t  ,timeMax);
 					unsigned int B = getIdOfVariable(problem,l,j,t  ,timeMax);

 					if(A > max || B > max) continue;

 					final ++;

 					if(forReal == 1) {

						/* -A => B */
 						fprintf(file,"-%d -%d 0\n",A,B);
 					}
 				}
 			}	
 		}
 	}

 	return final;
 }

 unsigned int writeConstraintForOneShopHandleOneJob(Problem* problem, unsigned int timeMax, FILE* file,char forReal) {

 	unsigned int final = 0;
 	unsigned int max   = getNbVariables(problem,timeMax);

 	for(unsigned int i = 0; i < problem->nbShops ; i++) {

 		for(unsigned int j = 0; j < problem->nbJobs; j++) {

 			for(unsigned int l = 0; l < problem->nbJobs; l++) {

 				for(unsigned int t = 0; t < timeMax; t++) {

 					if(l == j) continue;

 					unsigned int A = getIdOfVariable(problem,i,j,t  ,timeMax);
 					unsigned int B = getIdOfVariable(problem,i,l,t  ,timeMax);

 					if(A > max || B > max) continue;

 					final ++;

 					if(forReal == 1) {

						/* -A => B */
 						fprintf(file,"-%d -%d 0\n",A,B);
 					}
 				}
 			}	
 		}
 	}

 	return final;
 }

 unsigned int getIdOfVariable(Problem* problem,unsigned int numShop,unsigned int numJob,unsigned int valueTime,unsigned int maxTime) {

 	unsigned int res = (((numShop)*(problem->nbJobs)*(maxTime-1)) + ((numJob)*(maxTime-1)) + valueTime) +1;

 	return res;
 }

 int compare( const void* a, const void* b) {

 	unsigned int int_a = * ((unsigned int*)a);
 	unsigned int int_b = * ((unsigned int*)b);

 	if(int_a == int_b) return 0;

 	if(int_a < int_b) return -1;

 	return 1;
 }

/**
 * This function allow to get in an integer array, the id of all the intervals solutions.
 * @param s the filename of the solution.
 * @param problem the problem itself
 * @return an array of integer who contains the ID of intervals solutions.
 */
 unsigned int* getSolution(Problem* problem,const char* solution,unsigned int max) {

 	FILE* file = fopen(solution,"r+");
 	unsigned int nbVar = getNbVariables(problem,max);
 	char  line[nbVar*20];

 	unsigned int* result = (unsigned int*)malloc(sizeof(unsigned int)*getNbConstraint(problem,max));

 	int satisfiable = 0;
 	int i = 0;
 	int value = 0;
 	int j = 0;
 	char** str_variables;

 	while( fgets(line,(int)sizeof(line),file) ) {

 		if(line[0] == 'c') continue;

 		if(line[0] == 's') {

 			if(strstr(line,"UNSATISFIABLE") == NULL) {
 				
 				satisfiable = 1;
 				continue;

 			} else {

 				return NULL;
 			}

 		}

 		if(satisfiable == 1) {

 			str_variables = str_split(line, " ");

 			while(str_variables[i] != NULL) {
 				
 				value = (int)atoi(str_variables[i]);

 				if(value == 0 && strcmp(str_variables[i],"0") != 0) {
 					
 					i++;
 					continue;
 				}
 				
 				
 				if(value > 0) {
 					result[j++] = (unsigned int)value;
 				}
 				
 				
 				i++;
 			}

 			str_variables = NULL;

 		}
 	}

 	result[j] = 0;

 	qsort(result,(size_t)j,sizeof(unsigned int),compare);

 	return result;
 } 

 const char* createCNF(Problem* problem,unsigned int max) {

 	const char* filename = "flowShop.cnf";
 	unsigned int C_MAX = max;

 	unsigned int nbVariables = getNbVariables(problem,C_MAX);
 	unsigned int nbClauses = 0;

 	FILE* file = fopen(filename,"w+");

 	nbClauses = getNbConstraint(problem,C_MAX);

 	printf("[INFO] - NbVariables : %10d\n",nbVariables);
 	printf("[INFO] - NbClauses   : %10d\n",nbClauses);
 	printf("[INFO] - C_max       : %10d\n",C_MAX-1);

 	fprintf(file,"p cnf %d %d\n",nbVariables,nbClauses);

 	writeConstraintAtLeastOneJob		 (problem,C_MAX,file,1);
 	writeConstraintForContinuityOfJobs   (problem,C_MAX,file,1);
 	writeConstraintForResourceOnOneShop  (problem,C_MAX,file,1);
 	writeConstraintForOneShopHandleOneJob(problem,C_MAX,file,1);


 	fclose(file);

 	return filename;
 }
