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

#include "interval.h"

/**
 * This function is the toString() of our structure
 * it will display the id : [start - end]
 * @param output the stream where we want to print the information.
 * @param interval the interval that we want to display.
 */
 void displayInterval(FILE* output, Interval* interval) {

	/* We display on the standard stream. */
   fprintf(output,"%4d : [%5.5f - %5.5f]",interval->id,interval->start,interval->end);
}

/**
 * This function is the constructor of our structure.
 * We need a start and an end date to create it.
 * @param s the start date of our new interval.
 * @param e the end date of our new interval.
 * @return a pointer to our new structure.
 */
 Interval* createInterval(double s, double e) {

	/* We allocate the good memory-space. */	
   Interval* result = malloc(sizeof(Interval) * 1);

	/* We put the good start date */
   result->start = s;

	/* We put the good end date */
   result->end   = e;	

	/* We return our pointer on this new interval. */
   return result;
}

/**
 * This function is the destructor of our structure.
 * We need the pointer on our interval to destroy it.
 * @param interval the interval that we want to destroy.
 */
 void deleteInterval(Interval* interval) {

	/* We desallocate the memory used. */
   free(interval);
}

/**
 * This function is the constructor by copy.
 * it will create a new interval with the exact same value for start and end.
 * But of course, the ID will be different, it has to be unique.
 * @param interval the interval that we want to copy.
 * @return the copy of our interval (with an unique ID)
 */
 Interval* copyInterval(Interval* interval) {
   
	/* We have to know how many they are. */
   static unsigned int nbIntervals = 1;

	/* We create an interval with the good dates. */
   Interval* result = createInterval(interval->start,interval->end);
   
	/* We incremente his ID. */
   result->id = nbIntervals++;

	/* We return the pointer on this new interval. */
   return result;
}
