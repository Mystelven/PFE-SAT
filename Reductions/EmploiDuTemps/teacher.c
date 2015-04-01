#include "teacher.h"

/**
 * This function is the constructor of our structure.
 * @return a pointer to our new structure.
 */
Teacher* createTeacher() {

	Teacher* result = (Teacher*)malloc(sizeof(Teacher) * 1);

	result->sizeArray = 20;

	result->array_intervalPossible =(Interval**)malloc(sizeof(Interval*)*result->sizeArray);

	result->nbInterval = 0;

	return result;
}

/**
 * addIntervalToTeacher : this function allow to add the reference of an interval to a teacher.
 * like this, it means that the teacher is able to teach during this interval. 
 * @param teacher The teacher that is able to teach something at one time.
 * @param interval the interval authorized for the teacher.
 */
void addIntervalToTeacher(Teacher* teacher, Interval* interval) {

	if(teacher->nbInterval == teacher->sizeArray) {
		
		teacher->array_intervalPossible = realloc(teacher->array_intervalPossible, teacher->sizeArray*2*sizeof(Interval*));
		teacher->sizeArray *= 2;
	}

	teacher->array_intervalPossible[teacher->nbInterval] = interval;

	++teacher->nbInterval;
}
