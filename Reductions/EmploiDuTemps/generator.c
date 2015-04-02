/*
 * TimeTable - Valentin Montmirail - Polytech Tours, France
 *  Copyright (c) 2015.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
 * OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
 
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

	unsigned long nbTeachersMax = 2;
	int** coursesByTeacher  = (int**)malloc(sizeof(int*)*100);

	for(unsigned long i = 0; i < 100; i++) {
		coursesByTeacher[i] = (int*)malloc(sizeof(int)*(nbCourses+1));	
	}

	/* -------------------------------------------------- */
	
		courses[0] = strdup("Java_TD_");
		nbByCourse[0] = 6;

		courses[1] = strdup("C#_CM");
		nbByCourse[1] = 8;

		courses[2] = strdup("C++_TD_");
		nbByCourse[2] = 6;	

		courses[3] = strdup("PHP_CM");
		nbByCourse[3] = 6;


	/* -------------------------------------------------- */

	for(int i = 0; i < (int)nbTeachersMax; i++) {
		for(int j = 0; j < (int) nbCourses; j++) {

			coursesByTeacher[i][j] =    j;
		}
		coursesByTeacher[i][nbCourses] = -1;
	}

	
	fprintf(output,"c We authorized %lu rooms.\n",nbSallesMax);
	fprintf(output,"c We have %lu teachers.\n",nbTeachersMax);
	fprintf(output,"c We have week of %lu days.\n",nbDaysAWeek);
	
	for(unsigned long i = 0; i < nbCourses; i++) {
		fprintf(output,"c %10s with %u * 2h\n",courses[i],nbByCourse[i]);
		result += nbByCourse[i];
	}

	fprintf(output,"c \n");

	for(int i = 0 ; i < (int)nbTeachersMax; i++) {

		fprintf(output,"c Teacher n° %d can teach : [",(i+1));
		
		for(int j = 0; j < (int)nbCourses+1; j++) {

			if(coursesByTeacher[i][j] == -1) break;

			fprintf(output,"%s ",courses[coursesByTeacher[i][j]]);
		}

		fprintf(output,"]\n");
	}

	fprintf(output,"p %lu %lu %lu \n",result,nbSallesMax,nbTeachersMax);

	int	correspondances[5][6] = {
								{8,10,12,14,16,18} , 
								{32,34,36,38,40,42}, 
								{56,58,60,62,64,66}, 
								{80,82,84,86,88,90}, 
								{104,106,108,110,112,114} 
								};


for(unsigned long c = 0; c < nbCourses; c++) {
	
	fprintf(output,"%s %u ",courses[c],nbByCourse[c]);							

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
							double x = (t+1)+(correspondances[i][j  ]/1000.0);
							double y = (t+1)+(correspondances[i][j+1]/1000.0);
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
