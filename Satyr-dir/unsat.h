#ifndef UNSAT_H
#define UNSAT_H

#include "structure.h"

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
int resolution(int c1, int c2, int whichVariable );

/************************************************************************************************/
/*																								*/
/* restart : We did "too much" resolution and didn't find anything yet, we restart to find...   */
/*																								*/
/************************************************************************************************/
void restart();

#endif
