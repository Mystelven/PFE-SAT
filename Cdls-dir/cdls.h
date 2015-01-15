/********************************************************************/
/* Please select only one of following flags: UNIX, OSX, ANSI or NT */
/* Description:                                                     */
/*    UNIX:  uses some non-POSIX unix time routines, compiles       */
/*           under Linux                                            */
/*    OSX:   Mac OS X like Unix, but 64 bit ints not supported      */
/*    ANSI:  use POSIX time routines, can compile under all         */
/*           unix and NT, time accuracy is 1sec                     */ 
/*    NT:    use NT/Win32 multimedia routines, compile under        */
/*           NT only, time accuracy is 1ms                          */
/*           Uses NT "timeGetTime" function:                        */
/*             Header: Decl. in Mmsystem.h; include Windows.h.      */
/*             Library: Use Winmm.lib.                              */
/********************************************************************/


/* Define BIGINT to be the type used for the "cutoff" variable.
   Under gcc "long long int" gives a 64 bit integer.
   Program will still function using a 32-bit value, but it
   limit size of cutoffs that can be specified. */

#define BIGINT long long int

/************************************/
/* Constant parameters              */
/************************************/

#define SAT 1
#define UNS 2
#define OUT 3

#define WP_RAND 0.01
#define BN_SUP_INT 1
#define BN_INF_INT 0.01
#define UP_INT 1.1
#define DOWN_INT 0.97
#define MUL_RAND 10000

/* Constant for heuristic */
#define NUMTRUELIT 1
#define WHEREFALSE 2
#define WATCH1 3
#define WATCH2 4
#define WEIGHT 5
#define NB_EXTEND 6


#define NIV_CRI 1


#ifdef DYNAMIC
  #define STOREBLOCK 20000000	/* size of block to malloc each time */
  #define MAXCLAUSE 5000000	/* maximum possible number of clauses */
#else
  #define STOREBLOCK 20000000	/* size of block to malloc each time */
  #define MAXCLAUSE 5000000	/* maximum possible number of clauses */
#endif

#define TRUE 1
#define FALSE 0

#define MAXLENGTH 200000           /* maximum number of literals which can be in any clause */
#define ALEA 0


#define NOVALUE 0
#define BIG 100000000
#define BLOC 100
#define MAXCLAUSEBLOC 4096	/* bloc size of alloc for clause */


#define  ToInt(p) (p)
#define SIGN(p) ((p)&1)
#define ar(p) ((p)>> 1)

#define ABS(x) (x>>1)
#define NEG(x) ((x) ^ 1)
#define WASS_TRUE(l,ass) (((l) & 1) ^ (ass))
#define WASS_FALSE(l,ass) (!(((l) & 1) ^ (ass)))
#define EQ(a,b)  (!((a) ^(b)))
#define DIF(a,b)  (((a) ^(b)))

typedef struct _liste{
  int val;
  struct _liste *next;
}liste;

typedef struct _weightCl{
  int ind;
  double weightAg;
}weightCl;

void flipatom(unsigned int toflip);

void termineProg(int res,char *comment);

void setWeightClause(int cl, double weight);

void updateWeights();

void simplify();

void initWSAT();
void wsat();
unsigned int partialInterpretationDerived();
unsigned int criticalPath();
void lookClause();

/**
   Refresh data base
 */
void cleaningDB();

/**
   Return the next decision variable
 */
unsigned int pickBranchLit();

/**
   Supprime un littéral du tableau falseBin
 */
void delLitBinFalse(unsigned int var);

/**
   Ajoute un littéral (tel que la variable n'est pas présent) dans falseBin
 */
void addLitBinFalse(unsigned int var);



#define RESBIN 1

#define MINSIN 5
#define NBBIN 4

typedef struct{
  unsigned int res[4];
  int nbBin;
  int nbBinInit;
  unsigned int *tabBin;
}binaryStruct;


void debug();
