#include "utils.h"
#include "cnfUtils.h"
#include <time.h>

unsigned int * previous;
unsigned int * solution;
unsigned int 	  C_max;
Problem* 		problem;

void signal_callback_handler(int signum) {

    printf("\ns C_MAX : %d\n\n",++C_max);

    while(previous[signum] != 0) {
    	if(previous[signum] > 0) {
    		printf("%d \n",previous[signum]);
    		signum++;
    	}
    }
    printf("\n");
	free(problem);
	exit(0);
}

int main(int argc,char** argv) {

    clock_t start,end;

	if(argc < 3) {

		fprintf(stderr,"\n[ERROR] - You need at least the FlowShop problem and the solver\n");
		fprintf(stderr,"[ERROR] - You must execute it like ./flowshop $problem $solver $cMaxInit \n");
		exit(-1);
	}

	char* filename = argv[1];
	char* solver   = argv[2];
	char cmd[255];

	signal(SIGINT,signal_callback_handler);
	signal(SIGKILL,signal_callback_handler);
	signal(SIGTERM,signal_callback_handler);
	signal(SIGQUIT,signal_callback_handler);

	C_max = (unsigned int)atoi(argv[3]);

	Problem* problem = readFile(filename);

	displayProblem(problem);

	start = clock();

	do {

		printf("\n");
		printf("[INFO] - Solver      : %s\n",solver);
		printf("[INFO] - Filename    : %s\n",filename);

		
		const char* cnfFile = createCNF(problem,C_max);

		strcat(cmd,solver);
		strcat(cmd," ");
		strcat(cmd,cnfFile);
		strcat(cmd," > solution.out");

	    system(cmd);

	    for(int i= 0; i < 255; i++) cmd[i] = '\0';

	    solution = getSolution(problem,"solution.out",C_max);

	    if(solution != NULL) {

	    	previous = solution;
	    	printf("[INFO] - SATISFIABLE");

	    } else {
	    	printf("[INFO] - UNSATISFIABLE");
	    }

	    printf("\n");

	    system("rm solution.out");

	    if(solution != NULL) C_max--;

	    end = clock();

		printf("[INFO] - Time        : %7.0f ms\n\n",(double)((end - start)/(CLOCKS_PER_SEC/1000)) );

	} while(solution != NULL);

    int i = 0;

    printf("\ns C_MAX* : %d\n\n",C_max);



    while(previous[i] != 0) {
    	if(previous[i] > 0) {
    		printf("%d \n",previous[i]);
    		i++;
    	}
    }
    printf("\n");

	free(problem);

   	return 0;
}
