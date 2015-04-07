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
 
#include "planning.h"

/**
 * This function is the constructor of our structure.
 * @return a pointer to our new structure.
 */
 Planning* createPlanning() {

 	Planning* result = (Planning*)malloc(sizeof(Planning) * 1);

 	result->sizeArray = 50;

 	result->array_subjects =(Subject**)malloc(sizeof(Subject*)*result->sizeArray);

 	result->nbSubjects = 0;

 	return result;
 }

/**
 * This function is the toString() of our structure
 * @param output the stream where we want to print the information.
 * @param planning the Planning that we want to display.
 */
 void displayPlanning(FILE* output, Planning* planning) {

 	unsigned long i = 0;
 	
 	for(i = 0; i < planning->nbSubjects ; ++i) {

 		displaySubject(output,planning->array_subjects[i]);

 	}	
 }

/**
 * This function is to add an Subject inside the Subject's array.
 * @param planning the Planning who will have a new subject.
 * @param subject, the subject that we want to insert.
 */
 void addSubject(Planning* planning, Subject* subject) {

 	if(planning->nbSubjects >= planning->sizeArray) {

 		planning->array_subjects = realloc(planning->array_subjects,planning->sizeArray*2);
 		planning->sizeArray *= 2;
 	}

 	planning->array_subjects[planning->nbSubjects++] = subject;
 }


/**
 * This function is the destructor of our structure.
 * We need the pointer on our Planning to destroy it.
 * @param planning the Planning that we want to destroy.
 */
 void deletePlanning(Planning* planning) {

 	unsigned long i = 0;

 	for(i = 0; i < planning->nbSubjects ; ++i) {

 		deleteSubject(planning->array_subjects[i]);
 	}

 	free(planning);
 }

 void initializeAllTeachers(Planning * planning) {

 	unsigned int a = 0;
 	unsigned int c = 0;

 	printf("[INFO] - We will try to initialize %lu teachers\n", planning->nbTeachers);
 	printf("[INFO] - We have %d subjects \n", planning->nbSubjects);

 	for(unsigned int i = 0; i < planning->nbTeachers; i++) {

 		planning->array_teachers[i] = createTeacher();

 		printf("[INFO] - We initialize the teacher number %d\n",(i+1));

 		for(a = 0; a < planning->nbSubjects; ++a) {
 			for(c = 0 ; c < planning->array_subjects[a]->nbSlots; ++c) {
 				
 				unsigned int intpart = 0;
 				intpart = (unsigned int)planning->array_subjects[a]->slots[c]->start;


 				if(intpart == (i+1)) {
 					addIntervalToTeacher(planning->array_teachers[i],planning->array_subjects[a]->slots[c]);			
 				}
 			}
 		}
 		
 	}
 }


 void cleaningTeacherNumberOfIntervals(Planning* planning) {

 	unsigned int a = 0;
 	unsigned int c = 0;

 	for(a = 0; a < planning->nbSubjects; ++a) {
 		for(c = 0 ; c < planning->array_subjects[a]->nbSlots; ++c) {
 			
 			unsigned int intpart = 0;
 			intpart = (unsigned int)planning->array_subjects[a]->slots[c]->start;

 			planning->array_subjects[a]->slots[c]->start -= intpart;
 			planning->array_subjects[a]->slots[c]->end   -= intpart;			
 		}
 	}
 }
