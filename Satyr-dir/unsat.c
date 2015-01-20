#include "unsat.h"

/************************************************************************************************/
/*																								*/
/* resoution : This function will perform the resolution rule on 2 clauses 'c1' and 'c2'		*/
/* @param c1 the left member of the resolution rule 											*/
/* @param c2 the right member of the resolution rule 											*/
/* @param whichVariable on which variable we will perform the resolution 						*/
/* @return 0 if the problem is still UNKNOWN and 1 if the problem is UNSAT 						*/
/* At the end, 1 clause will disappear and the 2nd one will be the result of the resolution		*/
/*																								*/
/************************************************************************************************/
int resolution(int c1, int c2,int whichVariable) {

	/* In the worst case, the resolution will be just an OR between c1 and c2 */
	long max = (long)(size[c1] + size[c2]);		

	/* We allocate the good size for this futur resolution's result. */
	int* result = (int*)( malloc( sizeof(int)*(unsigned long)max));

	int i, j, k, l;

	/* boolean to know if we need to add or not the value in the result clause. */
	char toAdd = 1;

	/* we will insert the first element in the 0th place of result */
	k = 0;


	for(i = 0; i < max; ++i) {
		result[i] = 0;
	}

	/* -------------------------------------------------- */

	/* We will, for every atom inside the C1 claude */
	for( i = 0 ; i < size[c1] ; ++i) {

		/* compare this value with every atom inside the C2 clause */
		for( j = 0 ; j < size[c2] ; ++j) {
			
			/* A v -A is equals to 1, so we don't need it for the resolution. */
			if( ( (clause[c1][i] == -1*clause[c2][j]) && (ABS(clause[c1][i]) == whichVariable) )) {

					toAdd = 0;
			}

		}

		/* We go through every element inside C2, C1 is not present. */
		if(toAdd == 1) {

			/* We will so, insert it and go for the next element */
			result[k++] = (char)clause[c1][i];

		}

		/* We refresh the value and go for the next one. */
		toAdd = 1;
	}

	/* -------------------------------------------------- */

	/* We will, for every atom inside the C2 claude */
	for( i = 0 ; i < size[c2] ; ++i) {

		/* compare this value with every atom inside the C1 clause */
		for( j = 0 ; j < size[c1] ; ++j) {
			
			/* A v -A is equals to 1, so we don't need it for the resolution. */
			if( ( (clause[c2][i] == -1*clause[c1][j]) && (ABS(clause[c2][i]) == ABS(whichVariable)) ) ) {

					toAdd = 0;
				
			}
		}

		/* If we considere that we still need to insert, we should if he is not already there. */
		if(toAdd == 1) {

			for(l = 0; l < (int)max ; ++l) {

				/* This value is already inside the result, so don't need to insert it. */
				if(result[l] == clause[c2][i]) toAdd = 0;
			}

		}

		/* Ok this value passes all the tests, we need to insert it in the result. */
		if(toAdd == 1) {

			/* We insert it and we k++ for the next value. */
			result[k++] = (char)clause[c2][i];

		}

		/* We treat this value, let's check the next one. */
		toAdd = 1;
	}

	/* -------------------------------------------------- */

	j =  0;

	/* We count how many spaces are not used */
	for( i = 0; i < (int)max ; ++i) {

		if(result[i] == 0) ++j;
	}	

	/* We will recreate the value result */
	result = (int*)realloc(result,(unsigned long)(max-j));

	clause[numclause+numresolution] = result;

	size[numclause+numresolution] = (int)(max-j);

	numresolution++;

	/* The resolution gives us the empty clause */
	if( (max-j) == 0) {

		/* The problem is so UNSAT */
		FOUND = UNSAT;
		return  1;
	}

	/* -------------------------------------------------- */

	return 0;
}


/************************************************************************************************/
/*																								*/
/* restart : We did "too much" resolution and didn't find anything yet, we restart to find...   */
/*																								*/
/************************************************************************************************/
void restart() {



}
