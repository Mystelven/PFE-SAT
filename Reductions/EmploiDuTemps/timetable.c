/*
##############################################################################
# 
# TimeTable - Valentin Montmirail - Polytech Tours, France
# Copyright (c) 2015.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
##############################################################################*/

#include "cnfUtils.h"

int main(int argc, char* argv[]) {

	char* filename;
	
	char* solver;

	char cmd[255];

	/* We will log a lot of informations in this file. */
	FILE* log = stdout;//fopen("display.log","w+");

	if(argc > 2) {
		
		filename = argv[1];

		solver   = argv[2];

	} else {
		
		fprintf(stderr,"You should give in input the plan file and the wanted solver.\n");
		fprintf(stderr,"The solver should also display the result exactly as it should : \n");
		fprintf(stderr,"s [UN]SATISFIABLE");
		fprintf(stderr,"v {{i,-i}}\n\n");
		
		fprintf(stderr,"The execution should be like this : ./timetable file.input path-to-solver\n");
		
		exit(0);
	}

	Planning * planning = readInputFile(filename);

	displayPlanning(log,planning);
	
	filename = createCNF(planning);

	fprintf(log,"[INFO] - The output file is now : %s\n",filename);
	fprintf(log,"[INFO] - We will now try to solve the CNF file\n");
		strcat(cmd,solver);
		strcat(cmd," ");
		strcat(cmd,filename);
		strcat(cmd," > solution.out");

	fprintf(log,"[INFO] - commande : %s",cmd);

	system(cmd);

	unsigned int* solution = getSolutionSchedule(planning,"solution.out");

	/* We display the solution on the log stream and also on the standard output stream. */
	displaySolutionSchedule(stdout,planning,solution);

	fprintf(log,"\n");	
	
	fclose(log);
	
	return 0;
}
