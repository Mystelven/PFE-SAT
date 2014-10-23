#include "../Headers/Variable.h"

/**
 * This is the constructor of the "pseudo-class" 
 * @param variable the pointer on the variable we want to allocate properly. (if not already done).
 * @param id the next id for our variable.
 * @param value the next value of our variable: {false, true}
 */
Variable* initVariable(unsigned int id, unsigned int value) {

	Variable* variable = (Variable*)malloc(sizeof(Variable)*1);

	/* We test our malloc result. */
	if(variable == NULL) {

		/* We want to be sure that the malloc didn't fail. */
		perror("Problem with the allocation of variables in initVariable(Variable * variable, unsigned int id, unsigned int value)");
		exit(-2);
	}


	variable->id = id;
	variable->value = value;

	return variable;
}

/**
 * This is the procedure to display our variable according to the CNF format;
 * That means that for the i-th variable it will be displayed : i if value equals true
 * and -i if value equals false.
 * @param variable the pointer on the variable that we will display.
 */
void displayVariable(Variable * variable) {

	if(variable == NULL) {
		perror("We are not allowed to display a variable if she is NULL");
		exit(-4);
	}

	if(variable->value == true) {

		/* A true variable is displayed as the value of her ID. */
		printf(" %d ",variable->id);

	} else {

		/* A false variable is displayed as the negative value of her ID. */
		printf("-%d ",variable->id);

	}

}
