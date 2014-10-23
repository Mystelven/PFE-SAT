#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdio.h>
#include <stdlib.h>

/* We define false equals to 0. */
#define false 0

/* We define true equals to 1. */
#define true  1

/** 
 * We define our structure for our variable.
 * a variable is defined by an ID and a value in {false, true}
 */
struct _Variable {

	/* The identifier of our variable. */
	unsigned int id;

	/* The value of our varaible in {false, true} */
	unsigned int value;
};

/** We rename our structure to avoid to tape struct everywhere... */
typedef struct _Variable Variable;

/**
 * This is the constructor of the "pseudo-class" 
 * @param variable the pointer on the variable we want to allocate properly. (if not already done).
 * @param id the next id for our variable.
 * @param value the next value of our variable: {false, true}
 */
void initVariable(Variable * variable, unsigned int id, unsigned int value);


/**
 * This is the procedure to display our variable according to the CNF format;
 * That means that for the i-th variable it will be displayed : i if value equals true
 * and -i if value equals false.
 * @param variable the pointer on the variable that we will display.
 */
void displayVariable(Variable * variable);

#endif
