#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv) {

	FILE* output = stdout;

	if(argc > 1) {
		output = fopen(argv[1],"w+");
	}

	unsigned long nbSallesMax  	 = 2;
	unsigned long nbCourses 	 = 4;


	char** courses   		    = (char**)malloc(sizeof(char*)*nbCourses);
	unsigned int* nbByCourse    = (unsigned int*)malloc(sizeof(unsigned int)*nbCourses);
	unsigned long nbDaysAWeek  	= 5;
	unsigned long nbBornesADay 	= 6; // 6 for all day, 3 for only morning ;)
	unsigned long result 	   	= 0;

	unsigned long nbTeachersMax = 3;
	int** coursesByTeacher  = (int**)malloc(sizeof(int*)*100);

	for(unsigned long i = 0; i < 100; i++) {
		coursesByTeacher[i] = (int*)malloc(sizeof(int)*(nbCourses+1));	
	}
	
	courses[0] = strdup("Java ");
	nbByCourse[0] = 3;

	courses[1] = strdup("C++ ");
	nbByCourse[1] = 4;

	courses[2] = strdup("C# ");
	nbByCourse[2] = 4;

	courses[3] = strdup("PHP ");
	nbByCourse[3] = 4;	

	for(int i = 0; i < (int)nbTeachersMax; i++) {

		coursesByTeacher[i][0] =   0; 
		coursesByTeacher[i][1] =   1; 
		coursesByTeacher[i][2] =   2;
		coursesByTeacher[i][3] =   3; 
		coursesByTeacher[i][4] =   -1; 
	}


	
	fprintf(output,"c We authorized %lu rooms.\n",nbSallesMax);
	fprintf(output,"c We have %lu teachers.\n",nbTeachersMax);
	fprintf(output,"c We have week of %lu days.\n",nbDaysAWeek);
	for(unsigned long i = 0; i < nbCourses; i++) {
		fprintf(output,"c %s with %u * 2h\n",courses[i],nbByCourse[i]);
		result += nbByCourse[i];
	}

	fprintf(output,"p %4lu %4lu \n",result,nbSallesMax);

	int	correspondances[5][6] = {
								{8,10,12,14,16,18} , 
								{32,34,36,38,40,42}, 
								{56,58,60,62,64,66}, 
								{80,82,84,86,88,90}, 
								{104,106,108,110,112,114} 
								};


for(unsigned long c = 0; c < nbCourses; c++) {
	
	fprintf(output,"%s %4u ",courses[c],nbByCourse[c]);							

	for(unsigned long t = 0 ; t < nbTeachersMax; t++) {
		
		unsigned long k = 0;
		
		while(coursesByTeacher[t][k] != -1) {

			//  We don't need to consider this combinaison. The teacher T is not able to teach the course C.
			if (coursesByTeacher[t][k] != (int)c) {
					
				k++;
				continue;
			}
			
			for(unsigned long i = 0; i  < nbDaysAWeek; i++) {
				for(unsigned long j = 0; j < nbBornesADay-1; j++) {
					for(unsigned long n = 1 ; n < nbSallesMax+1; n++) {
					
						/* On travaille pas entre 12 et 14h*/
						if(j != 2) {
							double x = (t+1)+(n/100000.0)+(correspondances[i][j  ]/1000.0);
							double y = (t+1)+(n/100000.0)+(correspondances[i][j+1]/1000.0);
							fprintf(output,"[%lf-%lf] ",x,y);
						}
					}
				}	
			}

			k++;
		}
	}

	fprintf(output,"\n");
}
	
	fclose(output);
	return 0;
}
