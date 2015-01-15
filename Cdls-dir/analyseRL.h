#define PHASE 1

#define ASS_TRUE 1
#define ASS_FALSE 2
#define NOT_ASS 0


#define LIT_TRUE(l, ass) ((ass) && (((l)^(ass))&1))
#define LIT_FALSE(l, ass) ((ass) && (((l)^((ass)>>1))&1))
#define LIT_UNDEF(ass) (!(ass))
#define LIT(l) ((SIGN(l))?-ABS(l):ABS(l))


/**
   This function initialize one struct dpll
   In entry :
       - h, heuristic to choice variable
   In exit :
       - s, the dpll structure
 */
void initDPLL();


/**
   Attach a clause
   In entry :
       - cl, indice clause
   In exit :
       - s, struct of type dpll
   Return : this function return 0 if the clause attaching and else the litteral propagate
 */
void attachClause(unsigned int *cl);

/**
   Propagate all enqueud facts. If a conflit arises, the conflicting clause is returned,
   otherwise NULL.

   In exit :
      - s, the dpll structure
      
   Post-condition : 
      - the propagation queue is empty, even if there was a conflict.
*/
unsigned int *propagate();


/**
   This function fallow to push a new variable 
   In entry :
       - v, new variable
   In exit :
       - dpll *s, the structure dpll
 */
void pushNewVariable(unsigned int v);


/**
   This function initialize one struct dpll
   In exit :
       - s, the dpll structure
 */
void reinitDPLL();


/**
   Add one litteral on the propagation list
   In entry :
        - l, the litteral
	- cl, the clause
   In exit :
        - s, structure dpll
 */
void uncheckedEnqueue(unsigned int l, unsigned int *cl);


/**
   Propagate one variable.

   In exit :
      - s, the dpll structure
      - vp ,propagate variable
      
   Post-condition : 
      - the propagation queue is empty, even if there was a conflict.
*/
void propagateNoDPLL(int vp);


/**
   This function reinitialize one struct dpll (no conflit)
   In exit :
       - s, the dpll structure
 */
void reinitNoDPLL();


/**
   Propagate to vivification thie function propagate all enqueud facts. 
   If a conflit arises, the conflicting clause is returned, otherwise NULL.

   In exit :
      - s, the dpll structure
      - mark, draps 
      
   Post-condition : 
      - the propagation queue is empty, even if there was a conflict.
*/
int propagateVivi(int *mark);


/**
   Detach clause
   In entry :
        - cl, a detached clause
   In exit :
        - s, the struct dpll
 */
void detachClause(unsigned int *cl);


/**
   Description:
      Analyze conflict and produce a reason clause.
   
   Pre-conditions:
      * 'cl' is assumed to conflicting clause
      
   
   Post-conditions:
      * the first litteral to clause returned is the asserting clause
   
   Effect:
      Will undo part of the trail, upto but not beyond the assumption of the current decision level
*/
unsigned int *analyseConflict(unsigned int *cl);


/**
   Backjumping processus
   In entry :
       - s, the structure dpll
   Return : this function return the backjumping level
 */
int backjumping(unsigned int *conf);


/**
 */
void cancelUntil(int level);

/**
   Permet de faire un backjump jusqu'au niveau de la variale x passé en 
   paramètre.

   In entry :
        - s, the structure dpll
        - lev, le level jusque où on remonte
	- cl, la clause de base
	
   Retourne : la fonction retourne une clause "assertive"
*/
int *backJumpLev(int lev, int cl);
