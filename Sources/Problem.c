#include "../Headers/Problem.h"




void initProblem(const char* filename) {

	 /* We will need a file to read the CNF instance and to create our Problem. */
	 FILE* file = NULL;

	 /* We need only the right to read the file, nothing more. */
	 file = fopen(filename, "r");

	 if(file == NULL) {

	 	/* We didn't success to open the file, so we display an error message to help the user. */
	 	perror("The path you give to the CNF file is not correct");
	 	perror("You should verify that you have the right to read this file.");
	 	perror("Or if the file exists for real...");
	 	exit(-3);
	 }


}
