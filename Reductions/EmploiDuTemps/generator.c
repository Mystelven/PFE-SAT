#include <stdio.h>
#include <stdlib.h>


int main(int argc, char ** argv) {

	FILE* output = stdout;

	if(argc > 1) {
		output = fopen(argv[1],"w+");
	}

	int nbSallesMax  = 2;
	int nbCourses 	 = 1;



	char** courses   = (char**)malloc(sizeof(char*)*nbCourses);
	int* nbByCourse  = (int*)malloc(sizeof(int)*nbCourses);
	int nbDaysAWeek  = 5;
	int nbBornesADay = 6; // 6 for all day, 3 for only morning ;)
	int result 		 = 0;

	
	courses[0] = "Java ";
	nbByCourse[0] = 6;

	courses[1] = "C++  ";
	nbByCourse[1] = 18;

	courses[2] = "C#   ";
	nbByCourse[2] = 6;

	courses[3] = "PHP  ";
	nbByCourse[3] = 8;	

	
	fprintf(output,"c We authorized %d rooms.\n",nbSallesMax);
	fprintf(output,"c We have week of %d days.\n",nbDaysAWeek);
	for(int i = 0; i < nbCourses; i++) {
		fprintf(output,"c %s with %d * 2h\n",courses[i],nbByCourse[i]);
		result += nbByCourse[i];
	}

	fprintf(output,"p %d\n",result);

	int	correspondances[5][6] = {
								{8,10,12,14,16,18} , 
								{32,34,36,38,40,42}, 
								{56,58,60,62,64,66}, 
								{80,82,84,86,88,90}, 
								{104,106,108,110,112,114} 
							};

for(int c = 0; c < nbCourses; c++) {
	fprintf(output,"%s %d ",courses[c],nbByCourse[c]);
	for(int i = 0; i  < nbDaysAWeek; i++) {
		for(int j = 0; j < nbBornesADay-1; j++) {
			for(int n = 1 ; n < nbSallesMax+1; n++) {
				/* On travaille pas entre 12 et 14h*/
				if(j != 2) {
					fprintf(output,"[%1.3f-%1.3f] ",n+(correspondances[i][j]/1000.0),n+(correspondances[i][j+1]/1000.0));
				}

			}
		}
	}
	fprintf(output,"\n");
}

	return 0;
}
