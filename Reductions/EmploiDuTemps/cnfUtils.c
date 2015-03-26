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
#include <assert.h>
#include <stdlib.h>


inline unsigned int getNbVariables(Planning* planning) {

	unsigned long i = 0;
	unsigned int result = 0;
	for(i = 0; i < planning->nbSubjects ; i++) {
		result += planning->array_subjects[i]->nbSlots;
	}	

	return result;
}

/**
 * readInputFile : This function will read the formalized input file
 * and will create a Planning structure with everything inside.
 * and ready to be transform into a CNF file.
 * @param filename the name of the input file.
 * @return a Planning structure. 
 */
Planning * readInputFile(const char* filename) {

	Planning * planning = createPlanning();

	FILE* file = fopen(filename,"r+");
	char line[32000];

	char* name;

	Subject** array_subjects = NULL;
	char* pch;
	int i = 0;
	unsigned long k = 0;
	unsigned long j = 0;
	unsigned long nb = 0;

	int nameGiven = 0;
	
	while( fgets(line,sizeof(line),file) ) {
		
		if(line[0] == 'c') continue;
		if(line[0] == 'p') {

			pch = strtok(line," ");
			pch = strtok(NULL," ");

			nb = (unsigned long)atoi(pch);
			array_subjects = (Subject**)malloc(sizeof(Subject*)*nb);
				
			
			pch = strtok(NULL," ");
			pch = strtok(NULL," ");

			nb = (unsigned long)atoi(pch);
			planning->nbTeachers = nb;
			planning->array_teachers = (Teacher**)malloc(sizeof(Teacher*)*nb);

		} else {

			pch = strtok(line," ");
			nameGiven = 0;
			
			while(pch != NULL) {
				
				if(atoi(pch) == 0 && nameGiven == 0) {

					name = pch;
			

					pch = strtok(NULL," ");
					
					nb = (unsigned long)atoi(pch);

					array_subjects[j] = createSubject(name,nb);

					for( k = 0; k < nb ; k++) {
						array_subjects[j+k] = createSubject(name,nb);
					}

					nameGiven = 1;

				} else {

					for(k = 0; k < nb ; k++) {

						double start = 0;
						double end = 0;
						sscanf(pch,"[%lf-%lf]",&start,&end);

						if(start > 0 && end > start) addInterval(array_subjects[j+k],createInterval(start,end));
						
					}
				}
				
				pch = strtok(NULL," ");
			}

			for(k = 0; k < nb ; k++) {
				addSubject(planning,array_subjects[j+k]);
			}

			i++;
		}
	}

	fclose(file);

	return planning;
}

inline void writeOneIntervalForEachClassSatisfied(FILE* file, Planning* planning) {

	unsigned int i = 0;
	unsigned int j = 0;

	for(i = 0; i < planning->nbSubjects; ++i) {
		for(j = 0 ; j < planning->array_subjects[i]->nbSlots; ++j) {
			fprintf(file,"%d ",planning->array_subjects[i]->slots[j]->id);
		}
		fprintf(file,"0\n");
	}
}

inline unsigned int getNbConstraint(Planning* planning) {

	unsigned int result = 0;
	unsigned int a = 0;
	unsigned int c = 0;

	unsigned int b = 0;
	unsigned int d = 0;


	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;

	for(i = 0; i < planning->nbSubjects; ++i) {
		for(j = 0 ; j < planning->array_subjects[i]->nbSlots; ++j) {
			for(k = j+1; k < planning->array_subjects[i]->nbSlots; ++k) {
				
				++result;
			}
		}
	}

	for(a = 0; a < planning->nbSubjects; ++a) {
		for(c = 0 ; c < planning->array_subjects[a]->nbSlots; ++c) {
			for(b = a+1; b < planning->nbSubjects; ++b) {
				for(d = 0 ; d < planning->array_subjects[b]->nbSlots; ++d) {

					 Interval* ac = planning->array_subjects[a]->slots[c];
					 Interval* bd = planning->array_subjects[b]->slots[d];

					 if(ac->end <= bd->start) {
							continue ;
						}

						if(bd->end <= ac->start) {
							continue ;
						}

					 ++result;
				}
			}
		}
	}

	for(i = 0; i < planning->nbTeachers; ++i) {

		Teacher* t = planning->array_teachers[i];

		for(j = 0; j < t->nbInterval; ++j) {
			for(k = 0; k < t->nbInterval; ++k) {

				if(j == k) continue;

				Interval* ac = t->array_intervalPossible[j];
				Interval* bd = t->array_intervalPossible[k];
				
				if(ac->end <= bd->start) {
					continue;
				}

				if(bd->end <= ac->start) {
					continue;
				}

				result++;
			}
		}
	}

	return result;
}

char** str_split(char* a_str, const char a_delim) {

    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int isSolutionExisting(Planning* planning, unsigned int* solution) {

	int i = 0;
	unsigned long j = 0;
	unsigned long k = 0;

	while(solution[i] != 0) {

		for(j = 0; j < planning->nbSubjects ; j++) {

			Subject* subject = planning->array_subjects[j];				

			for(k = 0; k < subject->nbSlots; ++k) {
				
				if(subject->slots[k]->id == solution[i]) {	
				
					return 1;
				}
			}
		}	
	}

	return 0;
}

void displaySolutionSchedule(FILE* output, Planning* planning,unsigned int* solution) {

	int i = 0;
	unsigned long j = 0;
	unsigned long k = 0;
	char* previousName = strdup("");
	unsigned long nb = 0;

	fprintf(output,"\n\n");

	while(solution[i] != 0) {

		for(j = 0; j < planning->nbSubjects ; j++) {

			Subject* subject = planning->array_subjects[j];				

			for(k = 0; k < subject->nbSlots; ++k) {
				
				if(subject->slots[k]->id == solution[i]) {	
					
					++nb;

					if(strcmp(previousName,subject->subjectName) != 0) {

						nb = 0;	
					} {
						previousName = planning->array_subjects[j]->subjectName;
					}

					if(nb < subject->nbCopy) 
					{
						fprintf(output,"%20s : ",subject->subjectName);
						displayInterval(output,subject->slots[k]);
						fprintf(output,"\n");
					}
					
				}
			}
		}	

		i++;
	}
	fprintf(output,"\n");
}

unsigned int* getSolutionSchedule(Planning* planning,const char* solution) {

	FILE* file = fopen(solution,"r+");
	char line[getNbVariables(planning)*256];

	unsigned int* result = (unsigned int*)malloc(sizeof(unsigned int)*getNbConstraint(planning));

	int satisfiable = 0;
	int i = 0;
	int value = 0;
	int j = 0;

	while( fgets(line,(int)sizeof(line),file) ) {
		
		if(line[0] == 'c') continue;

		if(line[0] == 's') {
			if(strstr(line,"UNSATISFIABLE") == NULL) {
				satisfiable = 1;
				continue;
			} else {
				printf("\n\nThis problem has been proved without any solutions\n\n");
			}
		}

		if(satisfiable == 1) {

			char** str_variables = str_split(line, ' ');

			while(str_variables[i] != NULL) {

				value = (int)atoi(str_variables[i]);
				
				if(value > 0) {
					result[j++] = (unsigned int)value;
				}
				i++;
			}
		}
	}
	
	result[j] = 0;

	return result;
}	

void writeOrNotConstraint(FILE* file, Interval* ac, Interval* bd) {
	
	if(ac->end <= bd->start) {
		return ;
	}

	if(bd->end <= ac->start) {
		return ;
	}

	fprintf(file,"-%d -%d 0\n",ac->id,bd->id);

} 

inline void writeOneIntervalOnlyByClass(FILE* file, Planning* planning) {

	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;

	for(i = 0; i < planning->nbSubjects; ++i) {
		for(j = 0 ; j < planning->array_subjects[i]->nbSlots; ++j) {
			for(k = j+1; k < planning->array_subjects[i]->nbSlots; ++k) {

				Interval* ij = planning->array_subjects[i]->slots[j];
				Interval* ik = planning->array_subjects[i]->slots[k];
				
				fprintf(file,"-%d -%d 0\n",ij->id,ik->id);
			}
		}
	}
}

inline void writeForTeachers(FILE* file, Planning* planning) {
	
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;

	for(i = 0; i < planning->nbTeachers; ++i) {

		printf("[INFO] - We will write constraint (or not) for the teacher %d\n",(i+1));
		Teacher* t = planning->array_teachers[i];

		for(j = 0; j < t->nbInterval; ++j) {
			for(k = 0; k < t->nbInterval; ++k) {

				if(j == k) continue;
				
				writeOrNotConstraint(file,t->array_intervalPossible[j],t->array_intervalPossible[k]);	
			}
		}
	}
}


inline void writeOneIntervalDontOverlap(FILE* file, Planning* planning) {

	unsigned int a = 0;
	unsigned int c = 0;

	unsigned int b = 0;
	unsigned int d = 0;
	
	printf("\n");
	
	for(a = 0; a < planning->nbSubjects; ++a) {
		for(c = 0 ; c < planning->array_subjects[a]->nbSlots; ++c) {
			for(b = a+1; b < planning->nbSubjects; ++b) {

				for(d = 0 ; d < planning->array_subjects[b]->nbSlots; ++d) {

					writeOrNotConstraint(file, planning->array_subjects[a]->slots[c],planning->array_subjects[b]->slots[d]);
				
				}
			}
		}
	}

}

char* createCNF(Planning* planning) {

	const char* filename = "planning.cnf";

	FILE* file = fopen(filename,"w+");

	unsigned int nbVariables  = getNbVariables(planning);
	unsigned int nbConstraint = planning->nbSubjects + getNbConstraint(planning);

	printf("\n[INFO] - We created a CNF file of this schedule problem with %d variables and %d clauses.",nbVariables,nbConstraint);
	fprintf(file,"p cnf %d %d\n",nbVariables,nbConstraint);

	writeOneIntervalForEachClassSatisfied(file,planning);

	writeOneIntervalOnlyByClass(file,planning);

	writeOneIntervalDontOverlap(file,planning);

	writeForTeachers(file,planning);

	fclose(file);

	return (char*)filename;
}
