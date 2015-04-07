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

#ifndef TEACHER_H
#define TEACHER_H

#include "subject.h"

/**
 * This structure represents the final object, that will be written in CNF file.
 * It contains an array of subject (where each of them has an array of intervals).
 */
 typedef struct struct_teacher {

	/** The structure needs an array of possible-taught Subject. */
 	Interval** array_intervalPossible;

	/* We need to know how many subjects are inside. */
 	unsigned int nbInterval;

	/** We need also the maximum space inside the array (to make it bigger if needed) */
 	unsigned long sizeArray;

 } Teacher;


/**
 * createTeacher : This function is the constructor of our structure.
 * @return a pointer to our new structure.
 */
 Teacher* createTeacher();


/**
 * addIntervalToTeacher : this function allow to add the reference of an interval to a teacher.
 * like this, it means that the teacher is able to teach during this interval. 
 * @param teacher The teacher that is able to teach something at one time.
 * @param interval the interval authorized for the teacher.
 */
 void addIntervalToTeacher(Teacher* teacher, Interval* interval);

#endif
