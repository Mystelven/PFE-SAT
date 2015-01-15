#define NPLACE 0
#define RMV -1


#define FG(t, i) ((t)[2 * (i)])
#define FD(t, i) ((t)[2 * (i) + 1])
#define FT(t, i) ((t)[(i) >> 1])
#define LEAF(t, i) ((2 * (i)) > *(t))
#define PGF(t, pos) (((2*(pos) + 1) > *(t))?(2*(pos)):(((t)[2*(pos)] > (t)[2*(pos)+1])?(2*(pos)):(2*(pos)+1)))


#define LEFT(i) ((i)<<1)
#define RIGHT(i) (((i)<<1) + 1)
#define PERE(i) ((i)>>1)

/**
   Add one element of head
   In entry :
       - e, one element
   In exit :
       - t, head
 */
void addHeap(int e);

/**
   Organize element of heap
   In entry :
       - l, level of element
   In exit :
       - t, head
 */
void getUp( int l);


/**
   Delete minus of the heap
   In exit :
      - t, heap
   Return, this function return the minus of the heap
 */
int delMin();

/**
   Orgenize the heap after call of delMin function
   In entry :
      - pos, position of the element to organize
   In exit :
      - t, heap
 */
void getDown(int pos);

/**
   Initialize the heap
 */
void initHeap();

/**
   Increase the weightVar of weightUp
   In entry :
      - e, variable increased 
 */
void increase(int e);

/**
   Increase the weightVar of weightUp
   In entry :
      - e, variable increased 
 */
void increasePond(int e, double f);

/**
   Priority file
   Return the next element the most prioritary if exist, 0 otherwise.
 */
int nextOrderHeap();


/**
   Reinitialize heap
 */
void reinitHeap();


/**
   Increase the weightVar of weightUp
   In entry :
      - e, variable increased 
 */
void decrease();


/**
   Increase the weightVar of weightUp * f each variable of clause
   In entry :
      - e, variable increased 
      - f, the factor
 */
void increaseClauseVariable(unsigned int *cl, double f);
