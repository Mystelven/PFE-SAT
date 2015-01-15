#include "unsat.h"

/************************************************************************************************/
/*																								*/
/* resoution : This function will perform the resolution rule on 2 clauses 'c1' and 'c2'		*/
/* @param c1 the left member of the resolution rule 											*/
/* @param c2 the right member of the resolution rule 											*/
/* @return 0 if the problem is still UNKNOWN and 1 if the problem is UNSAT 						*/
/* At the end, 1 clause will disappear and the 2nd one will be the result of the resolution		*/
/*																								*/
/************************************************************************************************/
int resolution(int c1, int c2) {

	/* In the worst case, the resolution will be just an OR between c1 and c2 */
	long max = (long)(size[c1] + size[c2]);		

	/* We allocate the good size for this futur resolution's result. */
	char* result = (char*)( calloc( sizeof(char)*(unsigned long)max,0));

	int i, j, k, l;

	/* boolean to know if we need to add or not the value in the result clause. */
	char toAdd = 1;

	/* we will insert the first element in the 0th place of result */
	k = 0;

	/* -------------------------------------------------- */

	/* We will, for every atom inside the C1 claude */
	for( i = 0 ; i < size[c1] ; ++i) {

		/* compare this value with every atom inside the C2 clause */
		for( j = 0 ; j < size[c2] ; ++j) {
			
			/* A v -A is equals to 1, so we don't need it for the resolution. */
			if(unsatClause[c1][i] == -1*unsatClause[c2][j] || unsatClause[c1][i] == 0 || unsatClause[c2][i] == 0) {

				toAdd = 0;
			}

		}

		/* We go through every element inside C2, C1 is not present. */
		if(toAdd == 1) {

			/* We will so, insert it and go for the next element */
			result[k++] = (char)unsatClause[c1][i];

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
			if(unsatClause[c2][i] == -1*unsatClause[c1][j] || unsatClause[c1][i] == 0 || unsatClause[c2][i] == 0) {

				toAdd = 0;
			}
		}

		/* If we considere that we still need to insert, we should if he is not already there. */
		if(toAdd == 1) {

			for(l = 0; l < (int)max ; ++l) {

				/* This value is already inside the result, so don't need to insert it. */
				if(result[l] == unsatClause[c2][i]) toAdd = 0;
			}

		}

		/* Ok this value passes all the tests, we need to insert it in the result. */
		if(toAdd == 1) {

			/* We insert it and we k++ for the next value. */
			result[k++] = (char)unsatClause[c2][i];

		}

		/* We treat this value, let's check the next one. */
		toAdd = 1;
	}

	/* -------------------------------------------------- */

	for(i = 0 ; i < (int)max ; ++i) {

		printf("%i ",result[i]);
	}

	printf("\n\n");

	j =  0;

	/* We count how many spaces are not used */
	for( i = 0; i < (int)max ; ++i) {

		if(result[i] == 0) ++j;
	}	

	/* We will recreate the value result */
	result = (char*)realloc(result,(unsigned long)(max-j));

	/* The resolution gives us the empty clause */
	if( (max-j) == 0) {

		/* The problem is so UNSAT */
		FOUND = 1;
		return  1;
	}

	/* i is equals to the new size of result. */
	
	for( i = 0; i < (int)(max - j) ; i++) {
		printf("%i ",result[i]);
	}

	int* tmp = unsatClause[c1];

	(*unsatClause)[c1] = (int*)realloc(tmp,sizeof(int)*(max-j) );	
	
	free(tmp);
	size[c1] = (max-j);

	/* We desactive the clause C2, it doesn't exist anymore. */
	for(i = 0; i < size[c2] ; ++i) {
		unsatClause[c2] = 0;		
	}

	size[c2] = 0;

	for(i = 0 ; i < size[c1]; ++i) {
		unsatClause[c1][i] = (int)result[i];
	}

	for( i = 0 ; i < numclause ; ++i) { 
		for(j = 0; j < size[i]; ++j) {
			printf("%d ",unsatClause[i][j]);
		}
		printf("\n");
	}

	/* -------------------------------------------------- */

	exit(0);

	return 0;
}
