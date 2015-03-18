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

#ifndef SUBJECT_H
#define SUBJECT_H

#include "interval.h"

#include <string.h>

/**
 * This structure represents a Subject.
 * It has authorized intervals and a name.
 */ 
typedef struct struct_subject {

	/* The name of the subject */
	char* subjectName;

	/* The array of authorized intervals; */
	Interval** slots;

	/* The number of authorized intervals. */
	unsigned int nbSlots;

	/* How many intervals of this subject we need. 
	 * (useful to clean the final result) */
	unsigned long nbCopy;

} Subject;


/**
 * This function is the toString() of our structure
 * it will display the name and all the authorized intervals. 
 * @param output the stream where we want to print the information.
 * @param subject the subject that we want to display. 
 */
void displaySubject(FILE * output, Subject* subject);


/**
 * This function is the constructor of our structure.
 * We need a name and to know how many times we need this course to be true at the end.
 * @param name the futur name of our subject.
 * @param copy the number of times we need this course to be true.
 * @return a pointer to our new structure.
 */
Subject* createSubject(const char* name,unsigned long copy);

/**
 * This function is the destructor of our structure.
 * We need the pointer on our subject to destroy it.
 * @param subject the Subject that we want to destroy.
 */
void deleteSubject(Subject* subject);


/**
 * This function is to add an interval inside the Interval's array.
 * @param subject the subject who will have a new interval.
 * @param interval, the interval that we want to copy.
 */
void addInterval(Subject* subject, Interval* interval);

#endif
