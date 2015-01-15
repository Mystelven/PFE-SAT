#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "heap.h"
#include "cdls.h"

extern int numatom;

extern double *weightVar; 
double weightUp = 1.00; 
double weightDown = 1/0.95;
int *indice; 
int *heap;

int *nPlaceTab;
int *nPlaceInd;

double minWeight;

int father;
int tmp;
int ret;


#define COMPARE(x,y) (weightVar[x] > weightVar[y])


/**
   Initialize the heap
 */
void initHeap()
{
  weightVar = (double *) malloc(sizeof(double) * (numatom + 1) );
  indice = (int *) malloc(sizeof(int) * (numatom + 2));
  heap = (int *) malloc(sizeof(int) * (numatom + 2));

  *heap = 0;

  for(tmp = 1 ; tmp <= numatom ; tmp++)
    {
      weightVar[tmp] = 0.0;
      indice[tmp] = RMV;
    }
  *heap = 0;

  minWeight = 0;
}/* initHeap */

/**
   Permet d'afficher le tas
 */
void affiche()
{
  int i;
  for( i = 1 ;  i<= *heap ; i++)
    {
      printf("(%4d %4d)", i, heap[i]);
      if(!((i - 1) %10)) printf("\n");
    }
  printf("\n");
}/* affiche */

int appartient(int e)
{
  int i;
  for( i =  1 ; (i<=*heap) && (e!=heap[i]) ; i++);
  return i <= *heap;
}

/**
   Increase the weightVar of weightUp
   In entry :
      - e, variable increased 
 */
void increase(int e)
{
  if((weightVar[e] += weightUp) > 1e+100)
    {
      for( tmp = 1 ; tmp <= numatom ; tmp++ ) weightVar[tmp] *= 1e-100;
      weightUp *= 1e-100;
    }

  if(indice[e] == RMV) return;
  getUp(indice[e]);
}/* increase */


/**
   Increase the weightVar of weightUp
   In entry :
      - e, variable increased 
 */
void increasePond(int e, double f)
{
  if((weightVar[e] += (weightUp * f)) > 1e+100)
    {
      for( tmp = 1 ; tmp <= numatom ; tmp++ ) weightVar[tmp] *= 1e-100;
      weightUp *= 1e-100;
    }

  if(indice[e] == RMV) return;
  getUp(indice[e]);
}/* increase */

/**
   Increase the weightVar of weightUp * f each variable of clause
   In entry :
      - e, variable increased 
      - f, the factor
 */
void increaseClauseVariable(unsigned int *cl, double f)
{
  int i;
  unsigned int e;

  for( i = 1 ; i <= *cl ; i++)
    {
      e = ABS(cl[i]);
      if((weightVar[e] += (f * weightUp)) > 1e+100)
	{
	  for( tmp = 1 ; tmp <= numatom ; tmp++ ) weightVar[tmp] *= 1e-100;
	  weightUp *= 1e-100;
	}
      
      if(indice[e] == RMV) return;
      getUp(indice[e]);
    }
}/* increase */


/**
   Increase the weightVar of weightUp
   In entry :
      - e, variable increased 
 */
void decrease()
{
  weightUp *= weightDown;
}/* increase */


/**
   Add one element of head
   In entry :
       - e, one element
   In exit :
       - t, head
 */
void addHeap(int e)
{
  if(indice[e] != RMV) return;
  assert((e>0) && (e <= numatom ));
  indice[e] = *heap + 1;
  heap[*heap + 1] = e;
  (*heap)++;
  getUp(*heap);
}/* addHeap */


/**
   Organize element of heap
   In entry :
       - l, level of element
   In exit :
       - t, head
 */
void getUp(int i)
{
  int x = heap[i];
  while ((i != 1) && (COMPARE(x, FT(heap, i))))
    {      
      heap[i] = FT(heap, i);
      indice[heap[i]] = i;
      i = PERE(i);
    }
  heap[i] = x;
  indice[x] = i;
}/* getUp */


/**
   Delete minus of the heap
   In exit :
      - t, heap
   Return, this function return the minus of the heap
 */
int delMin()
{
  if(*heap)
    {
      ret = heap[1];
      indice[ret] = RMV;
      
      heap[1] = heap[(*heap)--];      
      if(*heap > 1)
	{
	  indice[heap[1]] = 1;	  
	  getDown(1);
	}
      return ret;
    }else return 0;
}/* delMin */



/**
   Orgenize the heap after call of delMin function
   In entry :
      - pos, position of the element to organize
   In exit :
      - t, heap
 */
void getDown(int i)
{
  int x = heap[i];
  int child;
  while (LEFT(i) <= (*heap))
    {
      child = ((RIGHT(i) <= *heap) && (COMPARE(heap[RIGHT(i)], heap[LEFT(i)]))) ? RIGHT(i) : LEFT(i);
      if (!COMPARE(heap[child], x))
	break;
      heap[i] = heap[child];
      indice[heap[i]] = i;
      i = child;
    }
  heap[i] = x;
  indice[x] = i;
}/* getDown */


/**
   Priority file
   Return the next element the most prioritary if exist, 0 otherwise.
 */
int nextOrderHeap()
{
  if(*heap)
    {      
      ret = delMin();
      return ret;
    }
  return 0;
}/* nextOrderHeap */


