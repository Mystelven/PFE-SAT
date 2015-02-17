/*##############################################################################
# 
# Makefile for SATyr - Valentin Montmirail - Polytech Tours, France
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

#include "unsat.h"

/**
 *
 *  displayResolution: useful for debug, it will display the 2 clauses choosen
 *  for the resolution, the variable used and also the result of the operation.
 *  @param c1 the first clause used for the resolution
 *  @param c2 the second clause used for the resolution
 *  @param var the variable to perform the resolution.
 * 
 */ 
inline void displayResolution(int c1,int c2,int var) {

	int i;

	printf("Resolution entre %d et %d sur %d\n",c1,c2,var);

	printf("C1 : ");
	for(i = 0; i < size[c1]; i++){
		printf(" %d",clause[c1][i]);
	}

	printf("\nC2 : ");
	for(i = 0; i < size[c2]; i++){
		printf(" %d",clause[c2][i]);
	}
	printf("\nRes: ");
	if(FOUND != UNSAT) {
		for(i = 0; i < size[numclause+numresolution-1]; i++){
			printf(" %d",clause[numclause+numresolution-1][i]);
		}
		printf("\n\n");
	}
}

/**
 *																								
 * contains : This function will test if the variable is include inside the clause 				
 * @param cl the clause who maybe contains the variable 										
 * @param variable the variable that we need to check 											
 * @return TRUE if the clause contains the variable, FALSE otherwise. 							
 *																								
 */
inline char contains(int cl, int variable) {

	int i = 0;

	for(i = 0; i < size[cl]; ++i) {

		if(clause[cl][i] == variable) return TRUE;
	}

	return FALSE;
}


/**
 * 
 * tryToProveUNSAT: 
 * This procedure will try to prove that the problem has no solution.
 *
 */
inline void tryToProveUNSAT() {

	if(random() % numclause < 1) restart();

	/* If have gone to far, we restart everything, we will not find anything in this branch... */
	if(numresolution >= (numclause*(COEF_FOR_NBRESOLUTION-1))) { 
		restart(); 
	}

	int c1 = -1;
	int c2 = -1;
	int i,j,k;
	int min = BIG;

	int var = -1;

	for(i = 0; i < numclause+numresolution; ++i) {
		for(j = 0; j < numclause+numresolution; ++j) {

			if(min >= resolutionTable[i][j]) {
				c1 = i;
				c2 = j;
				min = resolutionTable[i][j];
			} 
		}
	}

	if(c1 == -1 || c2 == -1) {
		
		restart();
		return ;
	}
	
	for(i = 0; i < size[c1] ; ++i) {
 		
 		if( contains(c2,-1*clause[c1][i]) == TRUE) {
 			var = ABS(clause[c1][i]);
 			break;
 		}

 	}

 	resolution(c1,c2,var,TRUE);

// 	displayResolution(c1,c2,var);

 	if(isTautology() == TRUE) {
 		numresolution--;
 		return ;
 	}

 	resolutionTable[c1][c2] = BIG;
 	resolutionTable[c2][c1] = BIG;

 	i = numclause+numresolution-1;
 	
 	for(j = 0; j < numclause+numresolution; ++j) {

 		for(k = 0; k < size[i] ; ++k) {
 			if( contains(j,-1*clause[i][k]) == TRUE) {
 				var = ABS(clause[i][k]);
 				break;
 			}
 		}

 		if(var == -1) {
 				
 			resolutionTable[i][j] = BIG;
 				
 		} else {

 			resolution(i,j,var,FALSE);
 		}

 		resolutionTable[j][i] = resolutionTable[i][j];
 		var = -1;
 	}
 	
}

/**
 * 
 * isTautology will check if the last resolution is a tautology or not.
 * if not, then the clause is not worth it.
 * @return TRUE if the last resolution is a tautology, FALSE otherwise.
 *
 */
inline int isTautology() {

	int i,j;
	int var;

	/* We will go through all the variable inside the result of our resolution. */
	for(i = 0; i < size[numclause+numresolution-1]; ++i) {

		/* We take the i-th variable */
		var = clause[numclause+numresolution-1][i];
		
		/* And we will check for every other variable inside the clause, if there is var and ~var */
		for(j = 0; j < size[numclause+numresolution-1]; ++j) {
			
			/* If (... var or ~var ... ), then the clause is a tautology, and we don't need it. */
			if(-1*var == clause[numclause+numresolution-1][j] ) 
				return TRUE;
		}
	}

	/* None of the variable inside the clause creates a tautology, the clause is useful. */
	return FALSE;
}

/**
 *																								
 * resoution : This function will perform the resolution rule on 2 clauses 'c1' and 'c2'		
 * @param c1 the left member of the resolution rule 											
 * @param c2 the right member of the resolution rule 											
 * @param whichVariable on which variable we will perform the resolution 						
 * @param forReal is a boolean to know if we will store the result of not 						
 * @return 0 if the problem is still UNKNOWN and 1 if the problem is UNSAT 					
 * At the end, 1 clause will disappear and the 2nd one will be the result of the resolution		
 *
 */
inline int resolution(int c1, int c2,int whichVariable,int forReal) {

	/* In the worst case, the resolution will be just an OR between c1 and c2 */
	long max = (long)(size[c1] + size[c2]);		

	/* We allocate the good size for this futur resolution's result. */
	int* tmp = (int*)( malloc( sizeof(int)*(unsigned long)max));

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
			if( ( (clause[c1][i] == -1*clause[c2][j]) && (ABS(clause[c1][i]) == whichVariable) )) {

					toAdd = 0;
			}	
		}

		/* We go through every element inside C2, C1 is not present. */
		if(toAdd == 1) {

			/* We will so, insert it and go for the next element */
			tmp[k++] = (char)clause[c1][i];

		}

		/* We refresh the value and go for the next one. */
		toAdd = 1;
	}

	/* -------------------------------------------------- */

	/* We will, for every atom inside the C2 clause */
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
				if(tmp[l] == clause[c2][i]) toAdd = 0;
			}
		}

		/* Ok this value passes all the tests, we need to insert it in the result. */
		if(toAdd == 1) {

			/* We insert it and we k++ for the next value. */
			tmp[k++] = (char)clause[c2][i];

		}

		/* We treat this value, let's check the next one. */
		toAdd = 1;
	}

	for(i = k; i < max; ++i) {
		tmp[i] = 0;
	}

	/* -------------------------------------------------- */

	resolutionTable[c1][c2] = k;

	/* The resolution gives us the empty clause */
	if( k == 0) {

		/* The problem is so UNSAT */
		FOUND = UNSAT;
		return  1;
	}

	if(forReal == TRUE) {

		clause[numclause+numresolution] = tmp;
		size[numclause+numresolution]   = k;

		++numresolution;

	} else {
	
		free(tmp);
	}

	/* -------------------------------------------------- */

	return 0;
}

/**
 *																								
 * restart : We did "too much" resolution and didn't find anything yet, we restart to find...   
 *			 We reinitialize also the resolution table
 *																								
 */
inline void restart() {

	int i = 0;

	for(i  = 0 ; i < numresolution; i++) {

		free(clause[numclause+i]);		
		size[numclause+i] = 0;	
	}
	
	numresolution = 0;

	initResolutionTable();
}


/**
 * 
 * initResolutionTable: This function will initialize our big array that will be used to know
 * which clauses we will use for the next resolution.
 *
 */
void initResolutionTable() {

	int i,j,k;

	int var = BIG;

 	for(i = 0; i < numclause; ++i) {

 		for(j = i+1; j < numclause; ++j) {

 			for(k = 0; k < size[i] ; ++k) {
 				if( contains(j,-1*clause[i][k]) == TRUE) {
 					var = ABS(clause[i][k]);
 					break;
 				}
 			}

 			if(var == -1) {
 				
 				resolutionTable[i][j] = BIG;
 				
 			} else {

 				resolution(i,j,var,FALSE);
 			}

 			resolutionTable[j][i] = resolutionTable[i][j];
 			var = -1;
 		}

 	}

}
