#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "vec.h"

/**
   Initialize one vector
   In exit :
       - v, the vector
 */
void initVector(vector *v)
{
  assert(v);
  
  v->nbElt = 0;
  if(!(v->tab = (unsigned int **) malloc(sizeof(unsigned int *) * MYBLOC)))
    {
      perror("problème d'allocation mémoire 1\n");
      exit(120);
    }   
  v->sizeMax = MYBLOC;
}/* init */


/**
   Remove one vector
   In exit :
       - v, the vector
 */
void removeVec(vector *v)
{
  assert(v);

  free(v->tab);
  v->nbElt = 0;
  v->sizeMax = 0;
}/* remove */


/**
   Add a element on the vector
   In entry :
       - e, the element       
   In exit :
       - v, the vector
*/
void add(vector *v, unsigned int *elt)
{
  if(v->nbElt == v->sizeMax) 
    {
      if(!(v->tab = (unsigned int **) realloc(v->tab, (v->sizeMax + MYBLOC) * sizeof(unsigned int *) )))
	{
	  perror("problème d'allocation mémoire 2\n");
	  exit(0);
	}
      v->sizeMax += MYBLOC;
    }
  (v->tab)[(v->nbElt)++] = elt;
}/* add */



/**
   Push on element 
   In entry :
       - e, the element       
   In exit :
       - v, the vector
 */
void push(vector *v, unsigned int *e)
{
  assert(v);
  add(v,e);
}/* push */

/**
   Pop on element
   In exit :
       - v, the vector
 */
void pop(vector *v)
{
  assert(v);
  v->nbElt--;
}/* pop */

/**
   Return the top of the vector 
   In entry :
       - v, the vector
   Return ; This function return the top of the vector if exist else NULL
 */
unsigned int *top(vector *v)
{
  assert(v);
  return (v->tab)[v->nbElt-1];
}/* top */


/**
   Delete one element of the vector
   In entry :
       - e, the element
       - comp, function to comparable element
   In exit :
       - v, the vector
   Return : return the element delete
*/
void delete(vector *v, unsigned int *e)
{
#if 0
  int i;
  for(i=0 ; (i < v->nbElt) && (e != (v->tab)[i]) ; i++);
  if(i < v->nbElt)     
    (v->tab)[i] = v->tab[--(v->nbElt)];    
#else

 int i, j;
  int cpt = 0;
  for(i = 0, j = 0 ; i < v->nbElt ; i++)
    {
      if(e != (v->tab)[i])
	v->tab[j++] = v->tab[i];
      else cpt++;
    }
  v->nbElt -= cpt;
#endif

}/* delete */


/**
   Return 1 if v is empty, otherwise 0
   In entry :
      - v, the vector
 */
int isEmpty(vector v)
{
  return (v.nbElt == 0);
}/* isEmpty */







#if 0

void afficheV(vector v)
{
  int i;
  printf("-------Début---------\n");
  for(i=0;i<v.nbElt;i++)
    printf("%d\n",(int)v.tab[i]);
  printf("------- Fin ---------\n");
}

int main()
{
  char *t1 = "toto1";
  char *t2 = "toto2";
  char *t3 = "toto3";
  char *t4 = "toto4";
  char *t5 = "toto5";

  vector t;
  init(&t);

  add(&t,t1);
  affiche(t);
  push(&t,t2);
  affiche(t);
  
  printf("__________%s\n",(char *)top(&t));
  pop(&t);
  affiche(t);

  removeVec(&t);
  return 0;
}

#endif
