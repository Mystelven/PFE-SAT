#include "teacher.h"

/**
 * This function is the constructor of our structure.
 * @return a pointer to our new structure.
 */
Teacher* createTeacher() {

	Teacher* result = (Teacher*)malloc(sizeof(Teacher) * 1);

	result->sizeArray = 20;

	result->array_subjects =(Subject**)malloc(sizeof(Subject*)*result->sizeArray);

	result->nbSubjects = 0;

	return result;
}
