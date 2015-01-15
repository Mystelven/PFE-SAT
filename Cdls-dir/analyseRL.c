#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cdls.h"
#include "vec.h"
#include "analyseRL.h"
#include "heap.h"
    
extern int *atom;
extern int *bestInterpretation;
extern int numclause;
extern int numatom;
extern BIGINT nbPropagation;
extern int nbConf;
extern binaryStruct *binClause;
extern int nbRemoveRedByBin;
extern int nbRemoveRed;
extern int nbFlip;

char *nivOp;

int *partial_assign;
int *level;
int *posChoice;
unsigned int **reason;

int decisionLevel;
int tail;
int qhead;

int *seen;
int *isAssign;
int allocVarWatcheClause;
vector *watched;

int allocateMemoryClause;


int *vTemp;
int indVtmp;
unsigned int *pTmp;
int indPtmp;


#include <stdint.h>

/**
   This function initialize one struct dpll
   In entry :
       - h, heuristic to choice variable
   In exit :
       - s, the dpll structure
   Return : this function return 0 if CNF is false, otherwise not 0
 */
void initDPLL()
{
  int i;

  nivOp = (char *) calloc(numatom + 1, sizeof(char));
  
  /* initialize variable */
  partial_assign = (int *) malloc(sizeof(int)*numatom);
  posChoice = (int *) malloc(sizeof(int)*numatom);
  isAssign = (int *) malloc(sizeof(int)*(numatom+1));

  level = (int *) malloc(sizeof(int)*(numatom+1));
  seen = (int *) malloc(sizeof(int)*(numatom+1));
  pTmp = (unsigned int *) malloc(sizeof(unsigned int) * (numatom+1)); /* tableau temporaire (litredundant) */
  vTemp = (int *) malloc(sizeof(int)*(numatom+1)); /* utilisé dans propagate */

  for(i = 0 ; i<(numatom+1) ; isAssign[i] = NOT_ASS, seen[i] = 0, level[i++] = -1);

  watched = (vector *) malloc(sizeof(vector)*(2*(numatom+1)));
  reason = (unsigned int **) malloc(sizeof(unsigned int *)*(numatom+1));

  decisionLevel = 0;
  posChoice[0] = 0; 

  for(i=0 ; i<2*(numatom+1);i++)
    initVector(&(watched)[i]);
    
  tail = 0;
  qhead = 0;
}  
/**
   This function initialize one struct dpll
   In exit :
       - s, the dpll structure
 */
void reinitDPLL()
{
  int i, var;

  for(i = posChoice[0] ; i<tail ; i++ )
    {
      var = ABS(partial_assign[i]);
      isAssign[var] = NOT_ASS;
      level[var] = -1;
      addHeap(var);
    }
    
  decisionLevel = 0;
  tail = posChoice[0];
  qhead = tail;
}/* reinit */



/**
   Attach a clause, verbotten add clause false
   In entry :
       - cl, indice clause
   In exit :
       - s, struct of type dpll
   Return : this function return 0 if the clause attaching and else the litteral propagate
 */
void attachClause(unsigned int *cl)
{  
  add(&watched[cl[1]], cl);
  if(*cl > 1)
    add(&watched[cl[2]], cl);
}/* attachClause */

/**
   Detach clause
   In entry :
        - cl, a detached clause
   In exit :
        - s, the struct dpll
 */
void detachClause(unsigned int *cl)
{
  delete(&(watched[cl[1]]), cl);
  if(*cl > 1)
    delete(&(watched[cl[2]]), cl);
}/* detachClause */



int nbClauseAppriseInGraph = 0;
extern int numclauseLearnt;
extern unsigned int ** clauseLearnt;

/**
   Add one litteral on the propagation list
   In entry :
        - l, the literal
	- cl, the clause
   In exit :
        - s, structure dpll
 */
void uncheckedEnqueue(unsigned int l, unsigned int *cl)
{  
  isAssign[ABS(l)] = (SIGN(l)) ? ASS_FALSE : ASS_TRUE;
  bestInterpretation[ABS(l)] =  isAssign[ABS(l)] & 1;
  level[ABS(l)] = decisionLevel;
  reason[ABS(l)] = cl;
  partial_assign[tail++] = l;
}/* uncheckedEnqueue */


/**
   Propagate all enqueud facts. If a conflit arises, the conflicting clause is returned,
   otherwise NULL.

   In exit :
      - s, the dpll structure
      
   Post-condition : 
      - the propagation queue is empty, even if there was a conflict.
*/
unsigned int *propagate()
{
  unsigned int *confl = NULL;
  int first;
  int k;
  unsigned int *c, var;
  unsigned int **i, **j, **end;  
  
  while (qhead < tail)
    {
      var = NEG(partial_assign[qhead]);
      qhead++;      
      
      nbPropagation++;

      for(k = 0 ; k < binClause[var].nbBin ; k++)
	{	  
	  first = binClause[var].tabBin[k];
	  
	  if(LIT_TRUE(first, isAssign[ABS(first)])) continue;

	  if(LIT_FALSE(first, isAssign[ABS(first)]))
	    {
	      binClause[var].res[1] = first;
	      confl = binClause[var].res;
	      qhead = tail;
	      return confl;
	    }
	  uncheckedEnqueue(first, binClause[var].res);
	}
      
      for(i = j = watched[var].tab, end = &(watched[var].tab[watched[var].nbElt]); i != end ; )
	{
	  c = *(i++);
	  
 	  /* the first variable is'nt propagate */
 	  if(!(c[1] ^ var))
 	    { 
	      c[1] = c[2];
	      c[2] = var;
 	    }
	  
	  //// If 0th watch is true, then clause is already satisfied.
	  first = c[1];
	  if(LIT_TRUE(first, isAssign[ABS(first)]))
	    *(j++) = c;
	  else
	    {	      
	      // Look for new watch:
	      for (k = 3; k <= *c ; k++)
		{
		  if(!(LIT_FALSE(c[k], isAssign[ABS(c[k])])))
		    {		      
		      c[2] = c[k]; c[k] = var;
		      add(&(watched[c[2]]), c);
		      goto FoundWatch; 
		    }	   
		} 
	      
	      // Did not find watch -- clause is unit under assignment:
	      *(j++) = c;

	      if(LIT_FALSE(first, isAssign[ABS(first)]))
		{
		  confl = c;
		  qhead = tail;
		  // Copy the remaining watches:
		  while (i != end)
		    *(j++) = *(i++);
		}else
		{
		  assert(isAssign[ABS(first)] == NOT_ASS);  
		  uncheckedEnqueue(first, c);
		}
	    }
	FoundWatch:;		
	}
      watched[var].nbElt = j - watched[var].tab;
    }

  return confl;
}

extern int numtry;
extern double *weightingClause;
/**
   affiche la clause d'indice cl
 */
void afficheClause(unsigned int *cl)
{
  int i;
  int var;

  fprintf(stderr, "size = %d  : ", *cl);
  for(i = 1 ; i <= *cl ; i++)
    {
      var = cl[i];
      fprintf(stderr, "(%d,%d,%d,%d) ",
	     LIT(var), level[ABS(var)], atom[ABS(var)], isAssign[ABS(var)]);
    }
  fprintf(stderr, "\n");
}


/**
   Permet de savoir si la variable 
 */
int litRedundant(int lit)
{
  int i;
  int posInit = indVtmp;
  unsigned int *cl;

  indPtmp = 0;
  pTmp[indPtmp++] = lit;
  
  while(indPtmp > 0)    {
      --indPtmp;
      cl = reason[ABS(pTmp[indPtmp])];
      assert(cl);

      for( i = 2 ; i <= *cl ; i++) /* on peut commencer à 2 car seen[cl[i]] >= 2 */
	{
	  lit = cl[i];
	  
	  if((seen[ABS(lit)] < 2) && level[ABS(lit)] > 0)
	    {
	      if(reason[ABS(lit)] && nivOp[level[ABS(lit)]])
		{
		  seen[ABS(lit)] = 2;
		  pTmp[indPtmp++] = lit;
		  vTemp[indVtmp++] = lit;
		}
	      else
		{
		  for( i = posInit ; i< indVtmp ; i++)
		    seen[ABS(vTemp[i])] = 0;
		  indVtmp = posInit;
		  return 0;
		}
	    }
	}
    }

  return 1;
}/* litRedundant */


/**
    Permet de savoir si la clause apprise est self subsumé par une clause binaire
 */
int litRedundantBin(unsigned int lit)
{
  int i;
  unsigned int* tab;

  tab = binClause[NEG(lit)].tabBin;
  for( i = 0 ; i<binClause[NEG(lit)].nbBin; i++)
    {
      if((seen[ABS(tab[i])] == 3) && LIT_FALSE(tab[i], isAssign[ABS(tab[i])]))
	return 1;	    
    }
  return 0;
}/* litRedundantBin */


/**
   Compare integer
 */
static int cmpLevel(const void *p1, const void *p2)
{
  int a = *(int*)p1;
  int b = *(int*)p2;
  return level[ABS(b)] - level[ABS(a)];
}//cmpLevel


extern int testFalseLeartn;
/**
   Description:
      Analyze conflict and produce a reason clause.
   
   Pre-conditions:
      * 'cl' is assumed to conflicting clause
      
   
   Post-conditions:
      * the first litteral to clause returned is the asserting clause
   
   Effect:
      Will undo part of the trail, up to but not beyond the assumption of the current decision level
*/
unsigned int *analyseConflict(unsigned int *cl)
{  
  int i, j, sz, pathC = 0, p = 0, tmp, saveT = tail;
  unsigned int *c;

  indVtmp = 0;
  c = cl;

  do{
    assert(c);
    sz = *c;    
    
    for(i = (p) ? 2 : 1 ; i <= sz ; i++ )
      {
	tmp = c[i];	
	
	if(!seen[ABS(tmp)] && (level[ABS(tmp)] > 0))
	  {
	    increase(ABS(tmp));
	    seen[ABS(tmp)] = 1;
	    if(level[ABS(tmp)] == decisionLevel)
	      pathC++;
	    else vTemp[indVtmp++] = tmp;
	  }
      }
    
    for(p = partial_assign[--tail] ; !seen[ABS(p)] ; p = partial_assign[--tail]);
    
    assert(tail >= posChoice[decisionLevel - 1]);
    c = reason[ABS(p)];
    pathC--;
  }while(pathC > 0);
  
  /* Simplify conflict clause: */

  j = sz = indVtmp;
  
  for( i = 0 ; i<indVtmp ; i++)
    {
      nivOp[level[ABS(vTemp[i])]]++;
      seen[ABS(vTemp[i])] = 2;      
    }
  
  for( i = 0 ; i<sz ; i++)
    {
      tmp = vTemp[i];
      if(nivOp[level[ABS(tmp)]] <= 1)
	seen[ABS(tmp)] = 3;
      else 
	{
	  if(!reason[ABS(tmp)] || !litRedundant(tmp))
	    seen[ABS(tmp)] = 3;	    
	  else
	    {
	      nivOp[level[ABS(tmp)]]--;
	      j--;
	      nbRemoveRed++;
	    }
	}
    }
#if RESBIN

  seen[ABS(p)] = 3;

  /* use binary clause */
  for( i = 0 ; i<sz ; i++)
    {
      tmp = vTemp[i];
      if(seen[ABS(tmp)] != 3) continue;
      if(litRedundantBin(tmp))
	{
	  seen[ABS(tmp)] = 2;
	  j--;
	  nbRemoveRedByBin++;
	}
    }
#endif

  /* conflict simplication finished */
  /* create clause: */
  sz = j;
  /* c[0] = sz size of clause */
  /* c[1] assertive literal */
  /* c[sz + 1] number apparition */
  /* c[sz + 2] creation */
  c = (unsigned int *) calloc((sz + NB_EXTEND + 1), sizeof(unsigned int));
  for(j = 0, i = 2 ; j<indVtmp ; j++)
    if(seen[ABS(vTemp[j])] == 3)
	c[i++] = vTemp[j];
  
  for(i = 0 ; i < indVtmp ; i++)
    {
      seen[ABS(vTemp[i])] = 0;      
      nivOp[level[ABS(vTemp[i])]] = 0;
    }

  for(i = saveT - 1 ; i >= posChoice[decisionLevel - 1] ; i--)
    {      
      tmp = ABS(partial_assign[i]);
      seen[tmp] = 0;
      level[tmp] = -1;
      isAssign[tmp] = NOT_ASS;
      addHeap(tmp);
    }

  tail = posChoice[decisionLevel - 1];
  c[1] = NEG(p); /* litteral assertive */
  *c = sz + 1;
  indVtmp = 0;

  if(*c < 3) return c;
  
  /* on place une variable de plus haut niveau en deuxième position (pour le backtrack) */
  qsort(&c[2], sz, sizeof(unsigned int), cmpLevel); 

  return c;
}/* analyseConflict */


/**
   
 */
void cancelUntil(int lev)
{
  int var;

  if(decisionLevel > lev)
    {      
      for(tail-- ; (tail >= posChoice[lev]); tail--)
	{
	  var = partial_assign[tail];          
	  addHeap(ABS(var));
	  isAssign[ABS(var)] = NOT_ASS;
	  assert(level[ABS(var)] >= lev);	  
	  level[ABS(var)] = -1;
	}
      tail++;
      qhead = tail;
      decisionLevel = lev;
    }else qhead = tail;
}/* cancelUntil */


/**
   Backjumping processus
   In entry :
       - s, the structure dpll
   Return : this function return the backjumping level
 */
int backjumping(unsigned int *conf)
{
  int lev;

  if(*conf == 1)    
    lev = 0;
  else lev = level[ABS(conf[2])];

  cancelUntil(lev);
  decisionLevel = lev;
  return lev;
}/* backjumping */



/**
   This function fallow to push a new variable 
   In entry :
       - v, new variable
   In exit :
       - dpll *s, the structure dpll
 */
void pushNewVariable(unsigned int v)
{
  posChoice[decisionLevel] = qhead;
  decisionLevel++;
  assert(decisionLevel < numatom);
  assert(isAssign[ABS(v)] == NOT_ASS);  
  uncheckedEnqueue(v, NULL);  
}/* pushNewVariable */


