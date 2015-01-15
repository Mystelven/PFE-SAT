#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "cdls.h"
#include "analyseRL.h"
#include "heap.h"

extern int *atom;		        /* value of each atom */
extern unsigned int ** clause;		/* clauses to be satisfied */
extern unsigned int ** false;			/* clauses which are false */
extern unsigned int *falseBin;
extern int numfalse;			/* number of false clauses */
extern int numfalseBin;			/* number of bin false clauses */
extern int nbPC;		
extern int *isAssign;
extern int *breakcount;	                /* number of clauses that become unsat if var if flipped */
extern int *makecount; 	                /* number of clauses that become sat if var if flipped */
extern int *changed;    	        /* step at which atom was last flipped */
extern int lastNumflipRestart;
extern int nbAugmentation;
extern int *variableAugmentation;
extern int nbAlea;
extern binaryStruct *binClause;

extern BIGINT numflip;		        /* number of changes so far */
extern unsigned int *bestAtom;

int numerator = 50;//NOVALUE;	/* make random flip with numerator/denominator frequency */
int denominator = 100;		

int wp_numerator = 1;
int wp_denominator = 100;


long int rdm1, rdm2;
unsigned int myBin[3] = {2,0,0};

/**
   Augmentation boundary
 */
void increaseBoundary(unsigned int var)
{
  increasePond(var, log((1.0 / (double) variableAugmentation[var]) + 2));
}/* increaseBoundary */

/**
   Permet de gérer l'augmentation
 */
void augmentation(unsigned int *cl)
{
  int i;

  for( i = 1 ; i <= *cl ; i++)
    {
      if(makecount[ABS(cl[i])] == (numfalse + numfalseBin))
	{
	  nbAugmentation++;	      
	  variableAugmentation[ABS(cl[i])]++;
	  increaseBoundary(ABS(cl[i]));
	}
    }
}/* augmentation */


/**
   Permet de retourner une varaible choisi de manière aléatoire dans un clause
 */
unsigned int varRandom(unsigned int *cl)
{
  int i, tmp;

  nbAlea++;  
  tmp = rdm2 % *cl;
  for(i = 1 ; i <= *cl ;  i++)
    if(isAssign[ABS(cl[((i + tmp) % *cl) + 1])] == NOT_ASS)
      return cl[((i + tmp) % *cl) + 1];
  return NOVALUE; /* ne doit jamais arriver */
}/* varRandom */


/**
   Choisi une clause binaire fausse de manière aléatoire et la stocke dans myBin
 */
unsigned int *chooseBinaryFalse()
{    
  assert(*falseBin);
  myBin[1] = falseBin[(rdm1 % *falseBin) + 1];  
  myBin[2] = falseBin[(rdm2 % *falseBin) + 1]; 
  return myBin;
}/* chooseBinaryFalse */


/**
   Choise la clause prochaine clause fausse et initialise les deux nombres aléatoire
 */
unsigned int *chooseFalseClause()
{
  rdm1 = random();
  rdm2 = random();
  if((rdm1 % (numfalse + numfalseBin)) < numfalse)
    {
      unsigned int *cl = false[rdm1 % numfalse];
      cl[*cl + WEIGHT]++;
      return cl;
    }
  else return chooseBinaryFalse();;
}/* chooseFalseClause */



/**
   La métaheuristique est rnovlety
 */
int rnoveltyHyb(void (* fixe)())
{
  int diff, birthdate;
  int diffdiff;
  int youngest = 0, youngest_birthdate, best = 0, second_best = 0, best_diff, second_best_diff;
  int i;
  int tmp = 0;
  
  unsigned int *cl;

  cl = chooseFalseClause();
  if(!(rdm1 % 100)) return varRandom(cl);

  youngest_birthdate = -1;
  best_diff = -BIG;
  second_best_diff = -BIG;

  for(i = 1 ; i <= *cl ; i++)
    {
      if(isAssign[ABS(cl[i])] != NOT_ASS) continue; else tmp++;
           
      diff = makecount[ABS(cl[i])] - breakcount[ABS(cl[i])];
      birthdate = changed[ABS(cl[i])];
      if (birthdate > youngest_birthdate)
	{
	  youngest_birthdate = birthdate;
	  youngest = cl[i];
	}
      if (diff > best_diff || (diff == best_diff && changed[ABS(cl[i])] < changed[ABS(best)])) 
	{
	  /* found new best, demote best to 2nd best */
	  second_best = best;
	  second_best_diff = best_diff;
	  best = cl[i];
	  best_diff = diff;
	}
      else if (diff > second_best_diff || (diff == second_best_diff && changed[ABS(cl[i])] < changed[ABS(second_best)]))
	{
	  /* found new second best */
	  second_best = cl[i];
	  second_best_diff = diff;
	}
    }


  if(best_diff <= 0) 
    {
      if(--lastNumflipRestart < 0) return NOVALUE;
      else if(rdm1 & 1) return varRandom(cl);
    }
  
  if(tmp == 1) return best;
  if (best != youngest) return best;
  
  diffdiff = best_diff - second_best_diff;
  
  if (numerator*2 < denominator && diffdiff > 1) return best;
  if (numerator*2 < denominator && diffdiff == 1)
    {
      if ((rdm1 % denominator) < 2*numerator) return second_best;
      return best;
    }
  if (diffdiff == 1) return second_best;
  if ((rdm2 % denominator) < 2*(numerator - (denominator/2))) return second_best;
  
  return best;
}/* rnoveltyHyb */


/**
   Version hybride avec novelty   
 */
int noveltyHyb(void (* fixe)())
{
  int diff, birthdate;
  int youngest = numflip, youngest_birthdate, best = 0, second_best = 0, best_diff, second_best_diff;
  int i;
  int tmp = 0;

  unsigned int *cl;

  cl = chooseFalseClause();
  
  youngest_birthdate = -1;
  best_diff = -BIG;
  second_best_diff = -BIG;

  for(i = 1 ; i <= *cl ; i++)
    {
      if(isAssign[ABS(cl[i])] != NOT_ASS) continue;
      tmp++;
      
      diff = makecount[ABS(cl[i])] - breakcount[ABS(cl[i])];
      birthdate = changed[ABS(cl[i])];
      
      if (birthdate > youngest_birthdate)
	{
	  youngest_birthdate = birthdate;
	  youngest = cl[i];
	}
      
      if (diff > best_diff || (diff == best_diff && changed[ABS(cl[i])] < changed[ABS(best)]))
	{
	  /* found new best, demote best to 2nd best */
	  second_best = best;
	  second_best_diff = best_diff;
	  best = cl[i];
	  best_diff = diff;
	}
      else
	{
	  if (diff > second_best_diff || (diff == second_best_diff && changed[ABS(cl[i])] < changed[ABS(second_best)]))
	    {
	      /* found new second best */
	      second_best = cl[i];
	      second_best_diff = diff;
	    }
	}
    }

  
  if(best_diff <= 0) 
    {
      if(--lastNumflipRestart < 0) return NOVALUE;
      else if(rdm1 & 1) return varRandom(cl);
    }
  
  if ((best != youngest) || (tmp<2)) return best;
  if ((rdm2 % denominator < numerator)) return second_best;	
  return best;
}/* noveltyHyb */


/**
 * Base d'une approche hybride basé sur l'heuristique best.
 */
int bestHyb()
{
  int diff;
  int i;

  unsigned int *cl;

  register int numbest = 0;
  register int bestvalue;
  
  cl = chooseFalseClause();

  bestvalue = -BIG;
  
  for(i = 1 ; i <= *cl ; i++)
    {
      if(isAssign[ABS(cl[i])] != NOT_ASS) continue;        
      
      diff = makecount[ABS(cl[i])] - breakcount[ABS(cl[i])];      
            
      if (diff >= bestvalue)
	{
	  if (diff > bestvalue)
	    {
	      numbest = 0;
	      bestvalue = diff;
	    }
	  bestAtom[numbest++] = cl[i];
	}
    }  
  
  
  if(bestvalue <= 0) 
    {
      if(--lastNumflipRestart < 0) return NOVALUE;
      else if(rdm1 & 1) return varRandom(cl);
    }

  if(!numbest) 
    {
      lookClause(cl);
      assert(numbest);
    }
  if (numbest == 1) return bestAtom[0];
  return bestAtom[rdm2 % numbest];
}/* bestHyb */


/**
   Approche hybride basé sur noveltyPlus
 */
int noveltyPlusHyb(void (* fixe)())
{
  int diff, birthdate;
  int youngest = 0, youngest_birthdate, best = 0, second_best = 0, best_diff, second_best_diff;
  int i;

  int tmp = 0;
  unsigned int *cl;
  cl = chooseFalseClause();

  if ((rdm2 % wp_denominator < wp_numerator)) return varRandom(cl);

  youngest_birthdate = -1;
  best_diff = -BIG;
  second_best_diff = -BIG;

  for(i = 1 ; i <= *cl ; i++)
    {
      if(isAssign[ABS(cl[i])] != NOT_ASS) continue;
      tmp++;
      
      diff = makecount[ABS(cl[i])] - breakcount[ABS(cl[i])];
      birthdate = changed[ABS(cl[i])];
      if (birthdate > youngest_birthdate)
	{
	  youngest_birthdate = birthdate;
	  youngest = cl[i];
	}
      
      if (diff > best_diff || (diff == best_diff && changed[ABS(cl[i])] < changed[ABS(best)])) 
	{
	  /* found new best, demote best to 2nd best */
	  second_best = best;
	  second_best_diff = best_diff;
	  best = cl[i];
	  best_diff = diff;
	}
      else if (diff > second_best_diff || (diff == second_best_diff && changed[ABS(cl[i])] < changed[ABS(second_best)]))
	{
	  /* found new second best */
	  second_best = cl[i];
	  second_best_diff = diff;
	}
    }

  if(best_diff <= 0) 
    {
      if(--lastNumflipRestart < 0) return NOVALUE;
      else if(rdm1 & 1) return varRandom(cl);
    }

  if (best != youngest || (tmp < 2)) return best;
  if ((rdm1 % denominator < numerator)) return second_best;
  return best;
}

/**
   Approche hybride basé sur rnovelty+
 */
int rnoveltyPlusHyb(void (* fixe)())
{
  int diff, birthdate;
  int diffdiff;
  int youngest = 0, youngest_birthdate, best = 0, second_best = 0, best_diff, second_best_diff;
  int i;

  int tmp = 0;
  unsigned int *cl;
  cl = chooseFalseClause();
   
  if ((rdm1 % wp_denominator < wp_numerator)) return varRandom(cl);

  youngest_birthdate = -1;
  best_diff = -BIG;
  second_best_diff = -BIG;

  for(i =  1 ; i <= *cl ; i++)
    {
      if(isAssign[ABS(cl[i])] != NOT_ASS) continue;
      tmp++;
      
      diff = makecount[ABS(cl[i])] - breakcount[ABS(cl[i])];
      birthdate = changed[ABS(cl[i])];
      if (birthdate > youngest_birthdate)
	{
	  youngest_birthdate = birthdate;
	  youngest = cl[i];
	}
      if (diff > best_diff || (diff == best_diff && changed[ABS(cl[i])] < changed[ABS(best)]))
	{
	  /* found new best, demote best to 2nd best */
	  second_best = best;
	  second_best_diff = best_diff;
	  best = cl[i];
	  best_diff = diff;
	}
      else if (diff > second_best_diff || (diff == second_best_diff && changed[ABS(cl[i])] < changed[second_best]))
	{
	  /* found new second best */
	  second_best = cl[i];
	  second_best_diff = diff;
	}
    }

  if(best_diff <= 0) 
    {
      if(--lastNumflipRestart < 0) return NOVALUE;
      else if(rdm1 & 1) return varRandom(cl);
    }
  
  if (best != youngest) return best;

  diffdiff = best_diff - second_best_diff;

  if ((numerator * 2 < denominator && diffdiff > 1) || (tmp < 2)) return best;
  if (numerator * 2 < denominator && diffdiff == 1)
    {
      if ((rdm2 % denominator) < 2*numerator) return second_best;
      return best;
    }
  if (diffdiff == 1) return second_best;
  if ((rdm2 % denominator) < 2*(numerator-(denominator/2))) return second_best;

  return best;
}/* rnoveltyPlusHyb */
