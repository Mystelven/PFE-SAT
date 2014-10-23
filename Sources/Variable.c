#include "../Headers/Variable.h"

/**
 * This is the constructor of the "pseudo-class" 
 * @param variable the pointer on the variable we want to allocate properly. (if not already done).
 * @param id the next id for our variable.
 * @param value the next value of our variable: {false, true}
 */
void initVariable(Variable * variable, unsigned int id, unsigned int value) {

	variable->id = id;
	variable->value = value;

}

/**
 * This is the procedure to display our variable according to the CNF format;
 * That means that for the i-th variable it will be displayed : i if value equals true
 * and -i if value equals false.
 * @param variable the pointer on the variable that we will display.
 */
void displayVariable(Variable * variable) {

	if(variable->value == true) {

		/* A true variable is displayed as the value of her ID. */
		printf(" %d ",variable->id);

	} else {

		/* A false variable is displayed as the negative value of her ID. */
		printf("-%d ",variable->id);

	}

}
