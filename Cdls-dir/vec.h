#define MYBLOC 1

typedef struct _vector{
  unsigned int **tab;
  int nbElt;
  int sizeMax;
}vector;

/**
   Initialize one vector
   In exit :
       - v, the vector
 */
void initVector(vector *v);

/**
   Remove one vector
   In exit :
       - v, the vector
 */
void removeVec(vector *v);

/**
   Add a element on the vector
   In entry :
       - e, the element       
   In exit :
       - v, the vector
*/
void add(vector *v, unsigned int *elt);


/**
   Push on element 
   In entry :
       - e, the element       
   In exit :
       - v, the vector
 */
void push(vector *v, unsigned int *e);

/**
   Pop on element
   In exit :
       - v, the vector
 */
void pop(vector *v);


/**
   Return the top of the vector 
   In entry :
       - v, the vector
   Return : This function return the top of the vector if exist else NULL
 */
unsigned int *top(vector *v);


/**
   Delete one element of the vector
   In entry :
       - e, the element
       - comp, function to comparable element
   In exit :
       - v, the vector
   Return : return the element delete
*/
void delete(vector *v, unsigned int *e);


/**
   Return 1 if v is empty, otherwise 0
   In entry :
      - v, the vector
 */
int isEmpty(vector v);
