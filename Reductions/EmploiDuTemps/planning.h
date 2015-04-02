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

#ifndef PLANNING_H
#define PLANNING_H

#include "teacher.h"

/**
 * This structure represents the final object, that will be written in CNF file.
 * It contains an array of subject (where each of them has an array of intervals).
 */
typedef struct struct_planning {

	/** The structure needs an array of Subject. */
	Subject** array_subjects;

	/* We need to know how many subjects are inside. */
	unsigned int nbSubjects;

	/** We need also the maximum space inside the array (to make it bigger if needed) */
	unsigned long sizeArray;

	/** The structure needs an array of Teachers. */
	Teacher** array_teachers;

	/** We need to know how many teachers we have. */
	unsigned long nbTeachers;

} Planning;


/**
 * This function is the constructor of our structure.
 * @return a pointer to our new structure.
 */
Planning* createPlanning();

/**
 * This function is the toString() of our structure
 * @param output the stream where we want to print the information.
 * @param planning the Planning that we want to display.
 */
void displayPlanning(FILE* output, Planning* planning);

/**
 * This function is to add an Subject inside the Subject's array.
 * @param planning the Planning who will have a new subject.
 * @param subject, the subject that we want to insert.
 */
void addSubject(Planning* planning, Subject* subject);


/**
 * This function is the destructor of our structure.
 * We need the pointer on our Planning to destroy it.
 * @param planning the Planning that we want to destroy.
 */
void deletePlanning(Planning* planning);


void initializeAllTeachers(Planning * planning);

void cleaningTeacherNumberOfIntervals(Planning* planning);

#endif
