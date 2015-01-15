#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

#include <sys/times.h>
#include <sys/time.h>
#include <sys/types.h>


#include "vec.h"
#include "cdls.h"
#include "analyseRL.h"
#include "heap.h"
#include "metaheuristique.h"


/************************************/
/* Compilation flags                */
/***********************************/

#define LUBY 1
#define TIRETS 141
#define INIT_LUBY 64

/* WSAT variable*/
int nbRestartTotal = 0;
BIGINT numflip;		        /* number of changes so far */
int maxSat = BIG;
int nbAlea = 0;
int *breakcount;	        /* number of clauses that become unsat if var if flipped */
int *makecount; 	        /* number of clauses that become sat if var if flipped */

double numCallCDCL = 0;

int nbAugmentation = 0;
int freeFlip = 0;

int *bestInterpretation;
int *variableAugmentation;

int numfalseBin;

int maxSatNoBinaire;
int maxSatTmp = 0;
int numflipAfterAnalyse = 1000;

unsigned int *falseBin;                /* falseBin[0] = nb lit false bin */
int *whereFalseBin, *nbLitFalseBin;
unsigned int tmpBin[3];


/* No longer using constant CLK_TCK - it is deprecated! */
/* Instead: */
long ticks_per_second;

/**************************************/
/* CDCL variable                      */
/**************************************/

extern double weightUp;
extern int *posChoice;
extern unsigned int **reason;
extern int *partial_assign;
extern int *isAssign;
extern int decisionLevel;
extern int tail;
extern int *level;
extern int *vTemp;
extern unsigned int *pTmp; // tableau temporaire emprunté à analyseRL.c
extern int *vTemp; // tableau temporaire emprunté à analyseRL.c
extern int indVtmp;
extern vector *watched;
extern int qhead;


/************************************/
/* Main data structures             */
/************************************/

int numatom = 0;
int numclause = 0;
int numliterals;

int nbRemoveRed = 0;
int nbRemoveRedByBin = 0;
int nbRemoveInit = 0;
int nbRemoveLearnt = 0;

int nbFlip = 0;
int whenLearnt = 2;

int testFalseLeartn = 0;
unsigned int ** clause = NULL;		/* clauses to be satisfied */
				/* indexed as clause[clause_num][literal_num] */
int *atom;		        /* value of each atom */

int resultat = OUT;
int nbClauseAdd = 0;
int nbClauseDer = 0;
int nbConflitDer = 0;
int maxAtom = 100000;
int nbClauseBin = 0;

int nbConf = 0;
int nbCleaning = 0;
double bornconf = 100;

double cla_inc = 1.0, clause_decay = 1 / 0.999;
double *weightVar = NULL;

int numtry = 0;			/* total attempts at solutions */
int nbRepetition = 0;

/* Randomization */
unsigned int seed;  /* Sometimes defined as an unsigned long int */

struct timeval tv;
struct timezone tzp;

/* Statistics */

double expertime;
double totalTime=0;

int nbPush = 0;


/* Learnt clause */
int numclauseLearnt = 0;
unsigned int ** clauseLearnt = NULL;

/* binary clause */
binaryStruct *binClause;

/************************************/
/* Forward declarations             */
/************************************/

void parse_parameters(int argc,char *argv[]);
double elapsed_seconds(void);
void initprob(void); 
void handle_interrupt(int sig);
long super(int i);
void print_statistics_header(void);
void showInter();
void showFinal();
void removeClauseDB(int cl);
void cleaningDB();
void solve(int nbConflict);

double sizeLearnt;

int nbPC =0;
int nbCf = 0;
BIGINT nbPropagation = 0;

int var1 = 0;
int stat;

int nbCauseBeforeReduce;
int posSimplify = 0;

enum heuristics { BEST, NOVELTY, RNOVELTY, NOVELTY_PLUS, RNOVELTY_PLUS};
char * heuristic_names[] = { "best", "novelty", "rnovelty", "novelty+", "rnovelty+"};
static int (*pickcode[])() = {bestHyb, noveltyHyb, rnoveltyHyb, noveltyPlusHyb,
                              rnoveltyPlusHyb};
int heuristic = BEST;



enum heuristicsAddClause {ANC, CRITIQUE, DERIVE};
char * heuristic_extract[] = { "anc", "critique", "derive"};
static unsigned int (*pickextract[])() = {NULL, criticalPath, partialInterpretationDerived};
int heuristicAddClause = CRITIQUE;


unsigned int ***occurence;      /* where each literal occurs */
int *numoccurence;       	/* number of times each literal occurs */
unsigned int **false;			/* clauses which are false */

int *changed;    		/* step at which atom was last flipped */

unsigned int *bestAtom;
int nbConflict;


BIGINT numnullflip;		/*  number of times a clause was picked, but no  */

int numfalse;			/* number of false clauses */
int lastNumflipRestart = 0;




/***************************************************************************************/
/* Memory used     *********************************************************************/
/***************************************************************************************/

int memReadStat(int field)
{
  char name[256];
  pid_t pid = getpid();
  int     value = 0;
  sprintf(name, "/proc/%d/statm", pid);
  FILE*   in = fopen(name, "rb");
  if (in == NULL) return 0;
  for (; field >= 0; field--)
    assert(fscanf(in, "%d", &value));
  fclose(in);
  return value;
}

uint64_t memUsed() 
{
  return (uint64_t)memReadStat(0) * (uint64_t) getpagesize();
}

/*****************************************************************************************/
/* Main           ************************************************************************/
/*****************************************************************************************/

#define NBCLAUSEREDUCE 1000
#define NBADDREDUCE 1000

int main(int argc,char *argv[])
{
  ticks_per_second = sysconf(_SC_CLK_TCK);
  gettimeofday(&tv,&tzp);
  seed = (unsigned int)((( tv.tv_sec & 0177 ) * 1000000) + tv.tv_usec);
  *tmpBin = 2;
  parse_parameters(argc, argv);
  srandom(seed);

  signal(SIGINT, handle_interrupt);
  signal(SIGTERM, handle_interrupt);
  signal(SIGALRM, handle_interrupt);

  initprob();
  
  print_statistics_header();
  (void) elapsed_seconds();
  nbClauseDer = numclause;

  sizeLearnt = NBCLAUSEREDUCE;
  bornconf = 1;
  initWSAT();

  wsat();
  termineProg(OUT,"UNKNOWN");
  return 0;
}/* main */

/**
 * This function scan the next parameters (int).
 */
void scanone(int argc, char *argv[], int i, int *varptr)
{
    if (i>=argc || sscanf(argv[i],"%i", varptr)!=1){
      fprintf(stderr, "c Bad argument %s\n", i<argc ? argv[i] : argv[argc-1]);
      exit(-1);
    }
}/* scanone */

/**
 * This function scan the next parameters (BIGINT).
 */
void scanonell(int argc, char *argv[], int i, BIGINT *varptr)
{
  if (i>=argc || sscanf(argv[i],"%lli",varptr)!=1){
    fprintf(stderr, "c Bad argument %s\n", i<argc ? argv[i] : argv[argc-1]);
	exit(-1);
  }
}/* scanonell */


/**
 * Parse parameters
 */
void parse_parameters(int argc,char *argv[])
{
  int i;
  int temp;
  
  for (i=1 ; i < argc ; i++)
    {
      if (strcmp(argv[i],"-h") == 0)
        {
          printf("Remark : the differents option can be combined\n");
          printf("./cdls -metaheuristic -analysis < BENCH\n");
          printf("\tmetaheuristic : \n  \t-best\n  \t-novelty\n  \t-rnovelty\n  \t-novelty+\n  \t-rnovelty+\n");
	  printf("\tanalysis : \n  \t-ANC\n  \t-critical\n  \t-derive\n");
          printf("./cdls -seed N < BENCH\n");
          printf("./cdls -afterAnalyse N < BENCH\n");
          printf("The best combination is : ./cdls -metaheuristic -afterAnalyse 100 -derive < BENCH\n");          
          exit(0);
        }
      if (strcmp(argv[i],"-stat") == 0)
	stat = 1;
      else if (strcmp(argv[i],"-critique") == 0)
	 heuristicAddClause = CRITIQUE;
      else if (strcmp(argv[i],"-derive") == 0)
	heuristicAddClause = DERIVE;
      else if (strcmp(argv[i],"-afterAnalyse") == 0)
	{
	  scanone(argc,argv,++i, &numflipAfterAnalyse);
	}
      else if (strcmp(argv[i],"-seed") == 0)
	{
	  scanone(argc,argv,++i, &temp);
	  seed = (unsigned int)temp;
	}
      else if (strcmp(argv[i],"-p") == 0)
	{
	  scanone(argc,argv,++i,&temp);
	  numCallCDCL = temp;
	}else if (strcmp(argv[i],"-best") == 0) heuristic = BEST;
      else if (strcmp(argv[i],"-novelty") == 0) heuristic = NOVELTY;
      else if (strcmp(argv[i],"-rnovelty") == 0) heuristic = RNOVELTY;      
      else if (strcmp(argv[i],"-novelty+") == 0) heuristic = NOVELTY_PLUS;      
      else if (strcmp(argv[i],"-rnovelty+") == 0) heuristic = RNOVELTY_PLUS;  
    }
}/* parse_parameters */

/**
   Luby
 */
long super(int i)
{
    long power;
    int k;

    if (i<=0){
      fprintf(stderr, "c bad argument super(%d)\n", i);
      exit(1);
    }
    /* let 2^k be the least power of 2 >= (i+1) */
    k = 1;
    power = 2;
    while (power < (i+1)){
      k += 1;
      power *= 2;
    }
    if (power == (i+1)) return (power/2);
    return (super(i - (power/2) + 1));
}/* super */

/**
   Handle interrupt
*/
void handle_interrupt(int sig)
{
  showFinal();
  exit(0);
}/* handle_interrupt */

/**
   Compare variable
 */
static int cmpInt(const void *p1, const void *p2)
{
  int a = *(int*)p1;
  int b = *(int*)p2;
  if(a<0) a = -a;
  if(b<0) b = -b;
  return a - b;
}//cmpInt


int nbEqui = 0;
int nbPU = 0;

/**
 * Libère l'ensemble des clauses binaires avec une certaine variable
 */
void freeBinary(unsigned int var)
{
  if(binClause[var].nbBin)
    {
      free(binClause[var].tabBin);
      binClause[var].tabBin = NULL;
      binClause[var].nbBin = 0;
    }  
}/* freeBinary */
  
/**
   Init prob : parse problem
*/
void initprob(void)
{
  int i;
  int j = 0;
  unsigned int stp1;
  unsigned int stp2;
  int lastc;
  int nextc;
  int storeptr[MAXLENGTH];
  int freestore;
  int lit;
  int tau;
  int sz;
  
  while ((lastc = getchar()) == 'c' || lastc == '\n')
    {
      while ((nextc = getchar()) != EOF && nextc != '\n');
    }
  ungetc(lastc,stdin);
  if (scanf("p cnf %i %i",&numatom, &numclause) != 2)
    {
      fprintf(stderr,"c initprob 1 : Bad input file\n");
      exit(-1);
    }

  numclause += 10;
  maxAtom = numatom + 2;
  while (getchar()!='\n');

  /* initMyVariable */  
  if(!(atom = (int *) calloc(maxAtom + 1, sizeof(int)))){perror("atom"); exit(1);}  
  if(!(bestInterpretation = (int *) calloc(maxAtom + 1, sizeof(int)))){perror("atom"); exit(1);}  
  if(!(variableAugmentation = (int *) calloc(maxAtom + 1, sizeof(int)))){perror("atom"); exit(1);}
  
  initDPLL();
  
  /* on prépare pour les clauses binaires */
  binClause = (binaryStruct *) malloc(sizeof(binaryStruct) * ((maxAtom + 1) * 2));
  for( i = 0 ; i < (2 * (maxAtom + 1)) ; i++)
    {
      binClause[i].res[0] = 2;
      binClause[i].res[1] = 0; /* une valeur au pif qui sera utilisé pour l'analyse de conflit */
      binClause[i].res[2] = i; /* le littéral qui sera la raison de la propagation */
      binClause[i].res[3] = -1; /* permet de dire que la clause n'est pas une clause apprise */
      binClause[i].nbBin = 0; /* le nombre de 'clause binaire' associé */
      binClause[i].tabBin = NULL;
    }

  freestore = 0;
  numliterals = 0;
  for(i = 0 ; i < numclause ; i++)
    {
      sz = -1;
      tau=0;
      do
        {
          sz++;
	  j = scanf("%d", &lit);
          if(j != 1)
	    {
	      lit = 0;
	      
	      if(j == EOF)
		{
		  numclause = i + 1;
		  if(!(sz))
		    {
		      numclause--;
		      goto end_loop;
		    }
		}
	    }
          
          /* show lit exist to clause[i] or it's a tauto */
	  if(lit)
	    {
	      for(j=0 ; j<(sz) ; j++)
		if((storeptr[j]==lit) || (storeptr[j]==-lit)) break;
	      if(j < sz)
		{
		  if(storeptr[j] == lit)
		    {
		      sz--;
		      continue;
		    }/*redundancy*/
		  else tau = 1;/*tautologie*/
		}
	    }
          
          if(lit != 0)
            {
              storeptr[sz] = lit; /* clause[i][size[i]] = j; */
              freestore--;
              numliterals++;
            }
        }
      while(lit != 0);
      assert(sz);
      
      if(tau)
        {
          numclause--;
          i--;
        }
      else
        {
	  if(sz == 1)
	    {
	      stp1 = (storeptr[0] > 0) ? storeptr[0]<<1 :  (-storeptr[0]<<1) + 1;
	      if(LIT_FALSE(stp1, isAssign[ABS(stp1)])) termineProg(UNS, "UNSAT");
	      if(isAssign[ABS(stp1)] == NOT_ASS) uncheckedEnqueue(stp1, NULL);
	      numclause--;
	      i--;
	    }else
	    {
	      if(sz == 2)
		{
		  stp1 = (storeptr[0] > 0) ? storeptr[0]<<1 :  (-storeptr[0]<<1) + 1;
		  stp2 = (storeptr[1] > 0) ? storeptr[1]<<1 :  (-storeptr[1]<<1) + 1;
		  
		  nbClauseBin++;		  
		  binClause[stp1].tabBin = (unsigned int*) realloc(binClause[stp1].tabBin,sizeof(unsigned int)*(binClause[stp1].nbBin+1));
		  binClause[stp1].tabBin[binClause[stp1].nbBin++] = stp2;
		  
		  binClause[stp2].tabBin = (unsigned int*) realloc(binClause[stp2].tabBin,sizeof(unsigned int)*(binClause[stp2].nbBin+1));
		  binClause[stp2].tabBin[binClause[stp2].nbBin++] = stp1;		 

		  numclause--;
		  i--;
		}else
		{
		  if(!(clause = (unsigned int **) realloc(clause, sizeof(unsigned int *) * (i + 1)))){perror("clause"); exit(1);}
		  if(!(clause[i] = (unsigned int *) malloc(sizeof(unsigned int)*(sz + NB_EXTEND))))
		    {
		      perror("size");
		      exit(1);
		    }  
		  qsort(storeptr, sz, sizeof(int), cmpInt);
		  for(j = 1 ; j <= sz ; j++)
		    {
		      if(storeptr[j -1] > 0)
			clause[i][j] = storeptr[j - 1]<<1;
		      else clause[i][j] = ((-storeptr[j - 1])<<1) + 1;
		    }
		  *clause[i] = sz;
		  clause[i][sz + NUMTRUELIT] = 0;  // numtruelit
		  clause[i][sz + WHEREFALSE] = 0;  // wherefalse	
                  clause[i][sz + WATCH1] = 0;     // watch[1]
                  clause[i][sz + WATCH2] = 0;     // watch[2]
                  clause[i][sz + WEIGHT] = 0;      // weight
		  attachClause(clause[i]);
		}
	    }
	}
    }
 end_loop:  

  if(propagate()) termineProg(UNS, "UNSAT");
  posChoice[0] = tail;
  posSimplify = 0;
  initHeap();

  for(i = 1 ; i <= numatom ;i++) addHeap(i);
}/* initprob */


/**
   elapsed seconds
 */
double elapsed_seconds(void) 
{ 
   double answer;   
   answer = (double) clock() / CLOCKS_PER_SEC - totalTime;
   totalTime = (double) clock() / CLOCKS_PER_SEC;
   return answer; 
}/* elapsed_seconds */


/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/

/**
   Print statistics header
 */
void print_statistics_header(void)
{
  int i;
  fprintf(stderr, "c \nc seed = %i, numatom = %i, numclause = %i, numliterals = %i, afterAnalyse = %d, %s, %s\n",
          seed, numatom,numclause,numliterals, numflipAfterAnalyse, heuristic_extract[heuristicAddClause],
          heuristic_names[heuristic]);

  fprintf(stderr, "c ");
  for(i = 0 ; i< TIRETS ; fprintf(stderr, "-"), i++) fprintf(stderr, "\n");
  fprintf(stderr, "c | %6s | %7s | %7s | %8s | %6s | %7s | %9s | %7s |",
	  "resCDCL", "tps ", "#impl", "#learnt", "#conf", "#clean", "#propa", "#deci");
  fprintf(stderr, "| %6s | %7s | %15s | %7s | %7s |\n",
	  "tryLS", "numflip", " maxSat/maxNoBi", "nbAlea", "Equi");
  fprintf(stderr, "c ");
  for(i = 0 ; i< TIRETS ; fprintf(stderr, "-"), i++) fprintf(stderr, "\n");
}/* print_statistics_header */


/**
   Fonction permettant d'afficher les informations intermédiaire
 */
void showInter()
{
  double temps = elapsed_seconds();
  fprintf(stderr,
          "c | %7d | %7.1lf | %7d | %8d | %6d | %7d | %9lld | %7d |",
	  nbRestartTotal, temps, posChoice[0], numclauseLearnt, nbConf, nbCleaning, nbPropagation, nbPC);

  fprintf(stderr, "| %6d | %7lld | %7d/%7d | %7d | %7d |\n",
	  numtry, numflip, maxSatTmp, maxSatNoBinaire, nbAlea, nbEqui);
}/* showInter */

/**
   Fonction permettant de savoir le temps passé depuis le début du programme
   Retourne :
      - le temps passé depuis le début du programme
 */
double tps_ecouler()
{
   double answer;
   answer = (double) clock() / CLOCKS_PER_SEC;
   return answer; 
}//tps_ecouler


/**
   Fonction permettant d'afficher les résultats du test
 */
void showFinal()
{
  int i;

  showInter();
  fprintf(stderr, "c ");
  for(i = 0 ; i< TIRETS ; printf("="), i++) printf("\n");
  fprintf(stderr, "c \nc -----------------------------------\n");
  fprintf(stderr, "c # temps = %lf\n", tps_ecouler());  
  fprintf(stderr, "c # cleaningDB = %d\n",nbCleaning);
  fprintf(stderr, "c # clause binaire = %d\n",nbClauseBin);
  fprintf(stderr, "c # literal removed by litRedundant = %d\n",nbRemoveRed);
  fprintf(stderr, "c # literal removed by litRedundantBin = %d\n",nbRemoveRedByBin);
  fprintf(stderr, "c # init clause removed by simplify = %d\n",nbRemoveInit);
  fprintf(stderr, "c # learnt clause removed by simplify = %d\n",nbRemoveLearnt);
  fprintf(stderr, "c # une clause fausse était une apprise = %d\n", testFalseLeartn);
  fprintf(stderr, "c # equivalence dectecte = %d\n", nbEqui);
  uint64_t mem_used = memUsed();
  if(mem_used != 0) fprintf(stderr,"c # Memory used           : %.2f MB\nc \n", mem_used / 1048576.0);

  fprintf(stderr, "c ########################\n");
  fprintf(stderr, "c ####   ##  ###   ## ####\n");
  fprintf(stderr, "c #### #### # ## #### ####\n");
  fprintf(stderr, "c ####   ##  ###   ##  ###\n");
  fprintf(stderr, "c ########################\n");  
  fprintf(stderr, "c # restart = %d\n",nbRestartTotal);
  fprintf(stderr, "c # conflit = %d\n",nbCf);
  fprintf(stderr, "c # decision = %d\n",nbPC);
  fprintf(stderr, "c # propagation = %lld \t (%lf (prop/sec))\nc \n",nbPropagation, nbPropagation / tps_ecouler());

  fprintf(stderr, "c ########################\n");  
  fprintf(stderr, "c ### ### #   ##   ##   ##\n");  
  fprintf(stderr, "c ### # # # #### # ### ###\n");  
  fprintf(stderr, "c ###  #  ### ## # ### ###\n");  
  fprintf(stderr, "c ########################\n");  
  fprintf(stderr, "c # numtry = %d\n", numtry);
  fprintf(stderr, "c # numflips = %lld\n", numflip);
  fprintf(stderr, "c # maxSat = %d\n", maxSat);
  fprintf(stderr, "c # nbAugmentation = %d\n", nbAugmentation);
  fprintf(stderr, "c # freeFlip = %d\n", freeFlip);

  fprintf(stderr, "c \n");


  switch(resultat)
    {
    case SAT :
      {
        printf("s SATISFIABLE\n");
        for(i = 0 ; i<posChoice[0] ; i++) atom[ABS(partial_assign[i])] = (~partial_assign[i]) & 1;

        printf("v ");
        for(i = 1 ; i<(numatom + 1); i++)
          if(atom[i]) printf("%d ", i); else printf("%d ",-i);
        printf("0\n");
        exit(10);
        break;
      }
    case UNS :
      {
        printf("s UNSATISFIABLE\n");
        exit(20);
        break;
      }
    case OUT : 
      {
        printf("s UNKNOWN\n");
      }
    }  
}//showFinal


/**
   Fonction permettant de gérer la terminaison du programme
   En entrée : 
      - res, dans quel état le programme c'est terminé
      - comment, une chaine de caractÃ¨re décrivant l'état de sorti
 */
void termineProg(int res, char *comment)
{
  resultat = res;  
  kill(getpid(), SIGTERM);
}/* termineProg */


/**
 *  Realloc memory variable
 */
void realloue()
{
  if(!(clauseLearnt = (unsigned int **) realloc(clauseLearnt, sizeof(unsigned int *)*(((numclauseLearnt/MAXCLAUSEBLOC)+1)*MAXCLAUSEBLOC))))
    {
      perror("clause\n");
      exit(1);
    }
}/* realloue */


/**
 * Add new clause of data base. This clause can be falsified by the
 * current interpretation !!!!
 * 
 * @param c, one table (malloc)
 *
 * @return This function return 0 if the new clause is added,
 * otherwise >0
 *
 */
int addClauseDB(unsigned int *c)
{
  int i;
  nbClauseAdd++;

  c[*c + NUMTRUELIT] = 0;
  c[*c + WATCH1] = c[*c + WATCH2] = 0;
  for(i = 1 ; i <= *c ; i++)
    {
      if(WASS_TRUE(c[i], atom[ABS(c[i])]))
        {
          if(!c[*c + WATCH1]) c[*c + WATCH1] = ABS(c[i]); else c[*c + WATCH2] = ABS(c[i]);
          c[*c + NUMTRUELIT]++;
        }

      if(*c > 2)
        {
          occurence[c[i]] = (unsigned int **)
            realloc(occurence[c[i]], sizeof(unsigned int **) * (numoccurence[c[i]] + 1));
          occurence[c[i]][numoccurence[c[i]]++] = c;
        }
    }


  if(!c[*c + NUMTRUELIT])
    for(i = 1 ; i <= *c ; i++) makecount[ABS(c[i])]++;
  else if(c[*c + NUMTRUELIT] == 1) breakcount[c[*c + WATCH1]]++;


  if(*c == 2)
    {
      if(!c[*c + NUMTRUELIT])
        {
          nbClauseBin++;
          numfalseBin++;
          if(!(nbLitFalseBin[ABS(c[1])]++)) addLitBinFalse(c[1]);
          if(!(nbLitFalseBin[ABS(c[2])]++)) addLitBinFalse(c[2]);
        }
      
      binClause[c[1]].tabBin = (unsigned int *) 
        realloc(binClause[c[1]].tabBin, sizeof(unsigned int) * (binClause[c[1]].nbBin + 1));
      binClause[c[1]].tabBin[binClause[c[1]].nbBin++] = c[2];
      
      binClause[c[2]].tabBin = (unsigned int *)
        realloc(binClause[c[2]].tabBin, sizeof(unsigned int) * (binClause[c[2]].nbBin + 1));
      binClause[c[2]].tabBin[binClause[c[2]].nbBin++] = c[1];
      free(c);
    }else
    {
      clauseLearnt = (unsigned int **)
        realloc(clauseLearnt, sizeof(unsigned int *) * (numclauseLearnt  + 1));
      clauseLearnt[numclauseLearnt] = c;
      attachClause(c);
      ++numclauseLearnt;
      
      if(!c[*c + NUMTRUELIT])
        {
          false = (unsigned int **) 
            realloc(false, sizeof(unsigned int *) * (numclauseLearnt + numclause));
          false[numfalse] = c;
          c[*c + WHEREFALSE] = numfalse++;        
        }
      c[*c + WEIGHT] = 0;
    }
  
  return 1;
}/* addClauseDB */


/**
 * Adding a new false clause under the current complete interpretation.
 * @param cl, the clause which must to be added.
 */
void addClause(unsigned int *cl)
{
  int i;

  if(*cl == 1)
    {
      i = tail;
      uncheckedEnqueue(cl[1], NULL);
      if(propagate()) termineProg(UNS,"UNSATISFIABLE");
      for( ; i < tail ; i++)
	{      
	  if(WASS_FALSE(partial_assign[i], atom[ABS(partial_assign[i])]))
	    flipatom(partial_assign[i]); 
	}
      posChoice[0] = tail;
      free(cl);
    }else if(*cl == 2)
    {
      for(i = 0 ; i < binClause[cl[1]].nbBin ; i++)
	if(binClause[cl[1]].tabBin[i] == cl[2]) break;
      if(i < binClause[cl[1]].nbBin) free(cl); else addClauseDB(cl);
    }else addClauseDB(cl);
}/* addFalseClause */


/**
 * Remove clause learnt. The different variable such that breacount,
 * makecount, ... don't update.
 *
 * @param cl the clause which can be deleted.
 */
void removeClause(unsigned int *cl)
{
  unsigned int **pi; 
  int i;

  assert(*cl > 2);

  for(i = 1 ; i <= *cl ; i++)
    {
      /* delete in occurence */
      for(pi = occurence[cl[i]] ; *pi != cl ; pi++);
      *pi = occurence[cl[i]][--numoccurence[cl[i]]];
    }

  detachClause(cl);
  free(cl);
}/* removeClause */


int locked(unsigned int * cl){ return (reason[ABS(cl[1])] == cl) && (LIT_TRUE(cl[1], isAssign[ABS(cl[1])])) && (level[ABS(cl[1])]);}
int trueClause(unsigned int *cl){ return (LIT_TRUE(cl[1], isAssign[ABS(cl[1])])) && (!level[ABS(cl[1])]); }

/**
   Refresh data base
 */
void cleaningDB()
{
  unsigned int **i, **j, **end; 

  for(i = j = clauseLearnt, end = &clauseLearnt[numclauseLearnt] ; i != end ; i++)
    {
      if((*i)[**i + WEIGHT])
        {
          (*i)[**i + WEIGHT] = (*i)[**i + WEIGHT]>>1;
          *(j++) = *i;
        }
      else
        {
          removeClause(*i);
        }
    }
   
  numclauseLearnt = j - clauseLearnt;
  sizeLearnt += NBADDREDUCE;
}/* cleaningDB */


/**
   Return the next decision variable
 */
unsigned int pickBranchLit()
{
  unsigned var;
  int i;
  do
    {
      var = nextOrderHeap();
    }
  while(isAssign[var] != NOT_ASS && var);
  
  for(i = 1 ; i <= numatom ; i++)
    {
      assert(isAssign[var] == NOT_ASS || weightVar[i] < weightVar[var]);
    }

  assert((isAssign[var] == NOT_ASS) || !var);
  
  if(!var)
    {
      for(i = 1 ; i <= numatom; i++) assert(isAssign[i] != NOT_ASS);
      for(i = 0 ; i < tail ; i++)             
	{
	  var = partial_assign[i];
	  atom[ABS(var)] = (SIGN(var))? 0 : 1;
	}
      termineProg(SAT,"ASSIGNMENTS FOUND");
    }
  
  return (var<<1) | ((atom[var] + 1) & 1); 
}/* pickBranchLit */





/**
   Fonction permettant de supprimer les clauses qui sont satisfaite
 */
void simplify()
{
  int i, j;
  unsigned int var;
  unsigned int *cl;

  unsigned int **pi, **pj, **end;
  unsigned int *si, *sj, *send, l;

  for( ; posSimplify < posChoice[0] ; posSimplify++)
    {      
      var = partial_assign[posSimplify]; // literals which are at level null
      
      // supprimer les occurences dans les clauses binaires !!!
      for(i = 0 ; i < binClause[var].nbBin ; i++) // satisfied clauses
        {
          l = binClause[var].tabBin[i];
          for( si = sj = binClause[l].tabBin,
                 send = &binClause[l].tabBin[binClause[l].nbBin] ; si != send ; si++)
            if(*si != var) *(sj++) = *si;
          binClause[l].nbBin = sj -  binClause[l].tabBin;
        }

      freeBinary(var);
      freeBinary(NEG(var));      
      
      /* delete all clauses attached by unit litteral */
      for( i = 0 ; i<numoccurence[var] ; i++)
	{
	  cl = occurence[var][i];
          detachClause(cl);
          cl[*cl + NUMTRUELIT] = -1; // flag
          
          for( j = 1 ; j <= *cl ; j++)
	    {
	      if(cl[j] == var) continue;

	      /* delete in occurence */
	      for(pi = pj = occurence[cl[j]], end = &(occurence[cl[j]][numoccurence[cl[j]]]) ; pi != end; pi++)
		if(*pi != cl) *(pj++) = *(pi);	      
	      numoccurence[cl[j]]--;
	    }
        }

      free(occurence[var]);
      occurence[var] = NULL;
      numoccurence[var] = 0;
    }
  
  for(pi = pj = clause, end = &clause[numclause] ; pi != end ; pi++)    
    if((*pi)[*(*pi) + NUMTRUELIT] != -1) *(pj++) = *(pi); else free(*pi);    
  numclause = pj - clause;

  for(pi = pj = clauseLearnt, end = &clauseLearnt[numclauseLearnt] ; pi != end ; pi++)
    if((*pi)[*(*pi) + NUMTRUELIT] != -1) *(pj++) = *(pi); else free(*pi);
  numclauseLearnt = pj - clauseLearnt;
}/* simplify */


/**
   Initialisation de la partie Walksat
 */
void initWSAT()
{
  int i, j;
  unsigned int *cl;

  if(!(false = (unsigned int **) malloc(sizeof(unsigned int *) * numclause)))
    {perror("false"); exit(1);}
  if(!(changed = (int *)  calloc(numatom + 1, sizeof(int)))){perror("initMyVariable");exit(1);}
  
  if(!(numoccurence = (int *) calloc(2*(numatom + 1), sizeof(int)))){perror("numoccurence"); exit(1);}
  if(!(occurence = (unsigned int ***) calloc(2*(numatom + 1), sizeof(unsigned int **)))){perror("occurence"); exit(1);}
  if(!(breakcount = (int *) calloc(numatom + 1, sizeof(int)))){perror("breakcount"); exit(1);}

  if(!(makecount= (int *) calloc(numatom + 1, sizeof(int)))){perror("makecount"); exit(1);}
  if(!(bestAtom = (unsigned int *) calloc(numatom + 1, sizeof(unsigned int)))){perror("best"); exit(1);}  

  if(!(falseBin = (unsigned int *) calloc(maxAtom + 1, sizeof(unsigned int)))){perror("atom"); exit(1);}
  if(!(whereFalseBin = (int *) calloc(maxAtom + 1, sizeof(int)))){perror("atom"); exit(1);}
  if(!(nbLitFalseBin = (int *) calloc(maxAtom + 1, sizeof(int)))){perror("atom"); exit(1);}

  for(i = 0 ; i < 2*(numatom + 1) ; i++)
    {
      occurence[i] = NULL;
      numoccurence[i] = 0;
    }
  
  for(i = 0 ; i < numclause ; i++)
    {
      cl = clause[i];
      for(j = 1 ; j <= *cl ; j++)
        {          
          if(!(numoccurence[cl[j]] % BLOC))
            if(!(occurence[cl[j]] = (unsigned int **) realloc(occurence[cl[j]],(sizeof(unsigned int *) * (numoccurence[cl[j]] + BLOC)))))
              {
                fprintf(stderr,"initprob : ERROR - allocation memory for occurence\n");
                exit(-1);
              }
          
          occurence[cl[j]][numoccurence[cl[j]]++] = cl;
        }
    }
}/* initWSAT */


/**
   Init l'interprétation complète
 */
void initInterpretation(int flag)
{
  int i;
  for(i = 1 ; i <= numatom ; i++)
    {
      atom[i] = random() & 1;
    }
  for(i = 0 ; i<posChoice[0] ; i++) 
    {
      atom[ABS(partial_assign[i])] = isAssign[ABS(partial_assign[i])] & 1; 
    }
}/* initInterpretation */

/**
   Init les clauses de la base
 */
void initClauseBase()
{
  int i, j, k;
  int truelit1;
  int truelit2;
  unsigned int *cl;
  unsigned int *numtruelit;

  numfalse = 0;

  /* Initialize breakcount and makecount in the following: */
  for(i = 0, k = 0 ; (i < numclause) || (k < numclauseLearnt) ;)
    {      
      if(i < numclause) cl = clause[i++];
      else cl = clauseLearnt[k++];

      numtruelit = &cl[*cl + NUMTRUELIT];
      assert(cl);
      
      *numtruelit = 0;
      truelit1 = 0;
      truelit2 = 0;
      for(j = 1 ; j <= *cl ; j++)
	{
	  if(WASS_TRUE(cl[j], atom[ABS(cl[j])]))
	    {
	      (*numtruelit)++;
	      if (!truelit1) truelit1 = cl[j];
	      else if (truelit1 && !truelit2) truelit2 = cl[j];
	    }
	}
      
      if(!(*numtruelit))
	{
	  cl[*cl + WHEREFALSE] = numfalse;
	  false[numfalse++] = cl;
	  for(j = 1 ; j <= *cl ; j++) makecount[ABS(cl[j])]++;	  
	}
      else if(*numtruelit == 1)
	{
	  breakcount[ABS(truelit1)]++;
	  cl[*cl +  WATCH1] = ABS(truelit1);
	}
      else /*if (numtruelit[i] == 2)*/
	{
	  cl[*cl +  WATCH1] = ABS(truelit1);
	  cl[*cl + WATCH2] = ABS(truelit2);
	}
    }
}/* initClauseBase */


/**
   Init les clauses binaire
 */
void initBinClause()
{
  int i, j;
  unsigned int var, lit;

  *falseBin = 0;  
  numfalseBin = 0;
  
  for( i = 1 ; i <= numatom ; i++)
    {
      nbLitFalseBin[i] = 0;
      lit = (i<<1) | (atom[i] & 1);

      for( j = 0 ; j<binClause[lit].nbBin ; j++)
	{	  
	  var = binClause[lit].tabBin[j];
          
	  if(WASS_FALSE(var, atom[ABS(var)]))
	    {
	      makecount[ABS(var)]++;
	      numfalseBin++;
	      nbLitFalseBin[i]++;
	    }
	  else breakcount[ABS(var)]++;
	}
      
      if(nbLitFalseBin[i]) addLitBinFalse(lit);	
    }
  
  numfalseBin >>= 1;
}/* initBinClause */

/**
   Initialyze : breakcount, makecount, ....
 */
void init(int flag)
{
  int i;

  for(i = 1 ; i <= numatom ; i++)
    {
      changed[i] = -1;
      breakcount[i] = 0;
      makecount[i] = 0;
    }
  
  if(!flag) initInterpretation(flag);
  initBinClause();
  initClauseBase();  
  
  maxSatTmp = ( maxSatNoBinaire = numfalse ) + numfalseBin;
}/* init */ 

/**
   Wsat call cleaningDB
 */
void callCleaningDB()
{
  nbCleaning++;
  cleaningDB();
  cancelUntil(0);
  if(posSimplify < posChoice[0]) simplify();
  init(2);  
}/* callCleaningDB */

/**
   Supprime un littéral du tableau falseBin
 */
void delLitBinFalse(unsigned int var)
{
  falseBin[whereFalseBin[ABS(var)]] = falseBin[(*falseBin)--];
  whereFalseBin[ABS(falseBin[whereFalseBin[ABS(var)]])] = whereFalseBin[ABS(var)];
}/* delLitBinFalse */

/**
   Ajoute un littéral (tel que la variable n'est pas présent) dans falseBin
 */
void addLitBinFalse(unsigned int var)
{
  falseBin[++(*falseBin)] = var;
  whereFalseBin[ABS(var)] = *falseBin;
}/* addLitBinFalse */


/*
  new flipping function based on SAT2004 submission work 
  Rend toflip vrai, c'est à dire que si il est positive et
  qu'il est affecté à faux on le flip sinon on ne fait rien.
*/
void flipatom(unsigned int toflip)
{   
  int i, j;			
  int numocc;
  unsigned int absToflip = ABS(toflip);
  unsigned int ** occptr;
  unsigned int *cl;
  unsigned int *numtruelit;
  unsigned int var;

  variableAugmentation[ABS(toflip)]++;

  if(!toflip || WASS_TRUE(toflip, atom[ABS(toflip)]))
    {
      numnullflip++;
      return;
    }
  
  numflip++;
  atom[absToflip] = 1 ^ atom[absToflip];
  numocc = numfalseBin;
  
  for( i = 0 ; i<binClause[toflip].nbBin ; i++)
    {
      var = binClause[toflip].tabBin[i];
      if(WASS_TRUE(var, atom[ABS(var)]))
	breakcount[ABS(var)]--;
      else 
	{
	  makecount[absToflip]--;
	  makecount[ABS(var)]--;
	  breakcount[absToflip]++;
	  numfalseBin--;
	  if(!(--nbLitFalseBin[ABS(var)])) delLitBinFalse(var);
	}
    }

  numocc = nbLitFalseBin[absToflip];
  nbLitFalseBin[absToflip] = 0;

  /* on commence par les binaires */
  for( i = 0 ; i < binClause[NEG(toflip)].nbBin ; i++)
    {      
      var = binClause[NEG(toflip)].tabBin[i];
      if(WASS_TRUE(var, atom[ABS(var)]))
	breakcount[ABS(var)]++;
      else 
	{
	  makecount[absToflip]++;
	  makecount[ABS(var)]++;
	  breakcount[absToflip]--; 
	  numfalseBin++;
	  nbLitFalseBin[absToflip]++;
	  if(!(nbLitFalseBin[ABS(var)]++)) addLitBinFalse(var);	    
	}
    }

  if(numocc && nbLitFalseBin[absToflip]) falseBin[whereFalseBin[absToflip]] = NEG(toflip);
  else if(!numocc && nbLitFalseBin[absToflip]) addLitBinFalse(NEG(toflip));
  else if(numocc && (!nbLitFalseBin[absToflip])) delLitBinFalse(toflip); 
  
  /* les clauses de taille supérieur à 2 */
  toflip = NEG(toflip);
  numocc = numoccurence[toflip];
  occptr = occurence[toflip];
  changed[absToflip] = numflip;
  for(i = 0 ; i < numocc ;i++)
    {
      /* cli = occurence[MAXATOM-toenforce][i]; */
      cl = *(occptr++);
      numtruelit = &cl[*cl + NUMTRUELIT];
      
      if (--(*numtruelit) == 0)
	{
	  cl[*cl + WHEREFALSE] = numfalse;
	  false[numfalse++] = cl;
	  
	  /* Decrement toflip's breakcount */
	  breakcount[absToflip]--;
	  
	  /* Increment the makecount of all vars in the clause */
	  for (j = 1 ; j <= *cl ; j++) makecount[ABS(cl[j])]++;	  
	}
      else if(*numtruelit == 1)
	{
	  if(cl[*cl + WATCH1] == absToflip)
	    {
	      assert(cl[*cl +  WATCH1] != cl[*cl + WATCH2]);
              cl[*cl + WATCH1] = cl[*cl + WATCH2];
	    }
	  breakcount[cl[*cl +  WATCH1]]++;
	}
      else
	{ /* numtruelit[cli] >= 2 */
	  if(cl[*cl +  WATCH1] == absToflip)
	    {
	      /* find a true literal other than watch1[cli] and watch2[cli] */
	      for (j = 1 ; j<=*cl ; j++)
		{
		  var = cl[j];
		  if(WASS_TRUE(var,atom[ABS(var)]) && (DIF(ABS(var),cl[*cl +  WATCH1])) && (DIF(ABS(var),cl[*cl + WATCH2])))
		    {
		      cl[*cl + WATCH1] = ABS(var);
		      break;
		    }
		}
	    }
	  else if (cl[*cl + WATCH2] == absToflip)
	    {
	      /* find a true literal other than watch1[cli] and watch2[cli] */
	      for (j = 1 ; j <= *cl ; j++)
		{
		  var = cl[j];
		  if (WASS_TRUE(var,atom[ABS(var)]) && (DIF(ABS(var), cl[*cl + WATCH1])) && (DIF(ABS(var), cl[*cl + WATCH2])))
		    {
		      cl[*cl + WATCH2] = ABS(var);
		      break;
		    }
		}
	    }
	}
    }
  
  toflip = NEG(toflip);
  numocc = numoccurence[toflip];

  occptr = occurence[toflip];
  for(i = 0; i < numocc; i++)
    {      
      /* cli = occurence[MAXATOM+toenforce][i]; */
      cl = *(occptr++);
      numtruelit = &cl[*cl + NUMTRUELIT];

      if (++(*numtruelit) == 1)
	{
	  numfalse--;
	  false[cl[*cl + WHEREFALSE]] = false[numfalse];
          false[numfalse][*false[numfalse] + WHEREFALSE] = cl[*cl + WHEREFALSE];

	  /* Increment toflip's breakcount */
	  breakcount[absToflip] ++;
	  
	  /* Decrement the makecount of all vars in the clause */
	  for (j = 1; j <= *cl; j++) makecount[ABS(cl[j])]--;
	  cl[*cl + WATCH1] = absToflip;
	}
      else if(*numtruelit ==  2)
	{
	  cl[*cl + WATCH2] = absToflip;
	  breakcount[cl[*cl + WATCH1]]--;
	}
    } 
}/* flipatom */


int compareWeightvar (const void * a, const void * b)
{
  unsigned int v1 = *(unsigned int*) a;
  unsigned int v2 = *(unsigned int*) b;
  return weightVar[ABS(v2)] - weightVar[ABS(v1)];
}

/**
 * Function to look at clause.
 * @param cl, clause
 */
void lookClause(unsigned int *cl)
{
  int i;

  for(i = 1 ; i <= *cl ; i++)
    {
      printf("(%d, %d, %d) ", LIT(cl[i]), atom[ABS(cl[i])], isAssign[ABS(cl[i])]);
    }
  printf(" : NUMTRUELIT = %d   WEIGHT = %d TAILLE = %d\n", cl[*cl + NUMTRUELIT], cl[*cl + WEIGHT], *cl); 
}/* lookClause */


/**
 * Select one false binary clause respect to one literal.
 * @param lit, the literal.
 * @return the false clause which has been selecting.
 */
unsigned int *selectClauseUniForLit(unsigned int lit)
{
  int i;
  unsigned int tabCl[numoccurence[lit]], sz = 0, *cl;
 
  for(i = 0 ; i<numoccurence[lit] ; i++)
    {
      cl = occurence[lit][i];    
      if(cl[*cl + NUMTRUELIT] != 1) continue;
      tabCl[sz++] = i;	
    }
  
  if(!sz) return NULL;
  return occurence[lit][tabCl[random() % sz]];
}/* selectClauseFalseForLit */

/**
 * Selects one false binary clause respect to one literal and stocks
 * the result in pTmp.
 *
 * @param lit, the literal.
 */
unsigned int *selectBinUniForLit(unsigned int lit)
{
  int i;
  unsigned int l;
  tmpBin[1] = lit;
  tmpBin[2] = 0;
  unsigned int tabLit[binClause[lit].nbBin], sz = 0;

  for(i = 0 ; i < binClause[lit].nbBin ; i++)
    {
      l = binClause[lit].tabBin[i];
      if(WASS_TRUE(l, atom[ABS(l)])) continue;
      
      if(level[ABS(l)] == 0)
        {
          tmpBin[2] = l;
          break;
        }        
      tabLit[sz++] = l;
    }
  
  if(!sz) return NULL;
  tmpBin[2] = tabLit[random() % sz];
  return tmpBin;
}/* selectBinFalseForLit */


/**
 * Analysis of the derived partial interpretation.
 * @param pTail, the position of the trail for the anlysis.
 * @return the clause obtained by conflicting analysis.
 */
unsigned int *analyseCritical(int pTail)
{
  int i, sz = 0;
  unsigned int *ret = NULL, lit, *cl = NULL;
  memset(vTemp, 0, sizeof(int) * numatom + 1);
 

  /* select a false clause for the resolution */
  lit = partial_assign[pTail];

  if(pTail & 1)
    {
      cl = selectBinUniForLit(NEG(lit));
      if(!cl) cl = selectClauseUniForLit(NEG(lit));  
    }else
    {     
      cl = selectClauseUniForLit(NEG(lit));  
      if(!cl) cl = selectBinUniForLit(NEG(lit));
    }
  assert(cl);    
  
  for(i = 1 ; i <= *cl ; i++) 
    {
      if(level[ABS(cl[i])] == 0) continue;
      pTmp[sz] = cl[i];
      vTemp[ABS(pTmp[sz++])] = 2;
    }
  
  for( ; pTail >= posChoice[decisionLevel - 1] ; pTail--)
    {
      lit = partial_assign[pTail];
      if(vTemp[ABS(lit)] && ((cl = reason[ABS(lit)])))
        {
          for(i = 0 ; (i < sz) && (NEG(lit) != pTmp[i]) ; i++);
          pTmp[i] = pTmp[--sz];
          
          /* perform the resolution between pTmp and cl */
          for(i = 2 ; i <= *cl ; i++) 
            {
              if(level[ABS(cl[i])] == 0) continue;
              if(!(vTemp[ABS(cl[i])] >> 1))
                {
                  pTmp[sz++] = cl[i];
                  vTemp[ABS(cl[i])] |= 2;
                }
            }
        }
    }

  ret = (unsigned int *) calloc((sz + NB_EXTEND), sizeof(unsigned int));
  *ret = sz;
  for(i = 0 ; i < sz ; i++)
    {
      ret[i + 1] = pTmp[i];    
    }
  
  return ret;
}/* analyseCritical */


/**
 * Partial interpretation derived.
 */
unsigned int partialInterpretationDerived()
{
  unsigned int var, *clConf = NULL, *cl;
  int tailTmp, oneConf = 0;

  while(1)
    {     
      var = pickBranchLit();
      tailTmp = tail;
      pushNewVariable(var);
      nbPC++; 
      cl = propagate();
      
      while(cl)
        {
          oneConf++;
          clConf = analyseConflict(cl);         
          backjumping(clConf);
          if(*clConf > 1)
            {
              uncheckedEnqueue(clConf[1], (*clConf == 2) ? binClause[clConf[2]].res : clConf);
              addClause(clConf);
              nbCf++;                       
              cl = propagate();
            } else
            {
              addClause(clConf);
              cl = NULL;
            }
        }

      if(oneConf) break;
      for(var = partial_assign[tailTmp] ; tailTmp < tail ; var = partial_assign[tailTmp])
        {
          if(WASS_FALSE(var, atom[ABS(var)])) break;
          ++tailTmp;
        }
      
      if(tailTmp == tail) continue;
      clConf = analyseCritical(tailTmp);
      addClause(clConf);
      for(var = partial_assign[tailTmp] ; tailTmp < tail ; var = partial_assign[tailTmp]) 
        { 
          if(WASS_FALSE(var, atom[ABS(var)]))
            {
              increase(ABS(var));
              flipatom(var);
            }
          ++tailTmp;
        }
      break;
    }
  
  decrease();
  cancelUntil(0);
  return NOVALUE;
}/* partialInterpretationDerived */


/**
   Update statistics end flip
 */
void update_statistics_end_flip(void)
{
  int i;

  if(maxSatNoBinaire > numfalse) maxSatNoBinaire = numfalse;

  if(maxSatTmp > (numfalse + numfalseBin))
    {
      maxSatTmp = numfalse + numfalseBin;
      if(maxSatTmp < maxSat)
        {
          //lastNumflipRestart += 1000;
          maxSat = maxSatTmp;
        }
      for( i = 1 ; i <= numatom ; i++) bestInterpretation[i] = atom[i];
    }

  if((!numfalse) && (!numfalseBin))
    {
      for(i = 0 ; i < numclause ; i++) assert(clause[i][*clause[i] + NUMTRUELIT]);
      termineProg(SAT,"ASSIGNMENTS FOUND 1");
    } 
  assert(maxSatTmp >= maxSatNoBinaire);
  
}/* update_statistics_end_flip */


/**
 * Choice a false clause and it stock in pTmp
 * @return the size of clause
 */
int selectClauseFalse()
{
  int i;
  unsigned int *cl = NULL;
  int tabFalse[numfalse], sz = 0;

  for(i = 0 ; i<numfalse ; i++)
    if(false[i][*false[i] + WEIGHT] > 0)
      tabFalse[sz++] = i;

  cl = false[tabFalse[random() % sz]];

  for(i = 1, sz = 0 ; i <= *cl ; i++)
    if(isAssign[ABS(cl[i])] == NOT_ASS) pTmp[sz++] = cl[i];
  return sz;
}/* selectClauseFalse */

/**
 * Choice a binary clause and it stock in pTmp
 * @return 2
 */
int selectBinFalse()
{
  int i, l, best = 0;

  *pTmp = falseBin[1 + (random() % *falseBin)];
  
  for(i = 0, best = 0 ; i < binClause[*pTmp].nbBin ; i++)
    {
      l = binClause[*pTmp].tabBin[i];
      if(WASS_TRUE(l, atom[ABS(l)])) continue;
      
      if(variableAugmentation[ABS(l)] >= best)
        {
          best = variableAugmentation[ABS(l)];
          pTmp[1] = l;
        }
    }

  return 2;
}/* selectBinFalse */


/**
 * Extract clause 
 */
unsigned int criticalPath()
{
  int i, posR = -1, l, best;
  int sz = 0;
  unsigned int *ret, *cl = NULL, **occpt, var;
  int rdm1 = random();

  memset(vTemp, 0, sizeof(int) * numatom + 1);

  if((rdm1 % (numfalse + numfalseBin)) < numfalse)
    sz = selectClauseFalse();
  else sz = selectBinFalse();

  /* To recup the critical clause */
  for(i = 0 ; i<sz ; i++){vTemp[ABS(pTmp[i])] = 1 << 1;}

  while(1)
    {
      best = -1;
      for(i = 0 ; i<sz ; i++)
        {
          l = ABS(pTmp[i]);
          if(vTemp[l] & 1) continue;          
          if(variableAugmentation[l] > best)
            {
              best = variableAugmentation[l];
              posR = i;
            }
        }     
      if(best == -1) break;
      
      if(!breakcount[ABS(pTmp[posR])])
        {
          vTemp[ABS(pTmp[posR])] = 3;
          continue; // or break : I don't know
        }

      assert(posR != -1);
      vTemp[ABS(pTmp[posR])] = 1;      
      
      /* search resolution in binary clauses */
      ret = binClause[NEG(pTmp[posR])].tabBin;
      for(i = 0, best = -1 ; i < binClause[NEG(pTmp[posR])].nbBin ; i++)
        {
          if(WASS_TRUE(ret[i], atom[ABS(ret[i])])) continue;
          if(vTemp[ABS(ret[i])] >> 1)
            {
              best = -2;
              l = ret[i];
              break;
            }
          if(variableAugmentation[ABS(ret[i])] > best)
            {
              best = variableAugmentation[ABS(ret[i])];
              l = ret[i];
            }          
        }

      if(sz == 2 && best != -2)
        {
          int redon = 0;
          ret = binClause[pTmp[posR]].tabBin;
          for(i = 0 ; i < binClause[pTmp[posR]].nbBin ; i++)
            {
              if(vTemp[ABS(ret[i])] >> 1 && WASS_TRUE(ret[i], atom[ABS(ret[i])]))
                {
                  best = -2;
                  posR = (posR + 1) & 1;
                  l = pTmp[posR];
                  redon = 1;
                  break;
                }
              if(ret[i] == pTmp[(posR + 1) & 1]) redon = 1;
            }
          if(!redon && best != 2) break;
        }

      if(best != -1)
        {          
          if(!(vTemp[ABS(l)] >> 1)) pTmp[posR] = l;
          if((vTemp[ABS(l)] >> 1) || (isAssign[ABS(l)] != NOT_ASS))
            pTmp[posR] = pTmp[--sz];
          vTemp[ABS(l)] |= 2;
        }
      else /* search clause unisatisfait by pTmp */
        {
          occpt = occurence[NEG(pTmp[posR])];
          for(i = 0, best = 0 ; i<numoccurence[NEG(pTmp[posR])] ; i++)
            {
              ret = occpt[i];
              if(ret[*ret + NUMTRUELIT] != 1) continue; 
              if(ret[*ret + WEIGHT] < best) continue;
              best = ret[*ret + WEIGHT];
              cl = ret;
            }
          assert(cl);
          best = pTmp[posR];
          pTmp[posR] = pTmp[--sz];

          /* perform the resolution between pTmp and cl */
          for(i = 1 ; i <= *cl ; i++) 
            {
              if(cl[i] == NEG(best) || isAssign[ABS(cl[i])] != NOT_ASS) continue;
              if(!(vTemp[ABS(cl[i])] >> 1))
                {
                  pTmp[sz++] = cl[i];
                  vTemp[ABS(cl[i])] |= 2;
                }
            }
        }
      if(sz == 1)
        {
          break; // prefered little clauses
        }
    }

  ret = (unsigned int *) malloc(sizeof(unsigned int) * (sz + NB_EXTEND));
  *ret = sz;
  for(i = 0 ; i < sz ; i++) ret[i + 1] = pTmp[i];
  nbCf++;
  var = ret[1 + random() % *ret];
  addClause(ret);
  return var;
}/* criticalPath */


/**
 * Function allows to verifie that it is a veritable local minimum.
 *
 * @return, 0 if a local minimu and v the litteral allows to decreased
 * the number of falsified clauses.
 */
int isLocalMinimumGSAT()
{
  int i, j;
  unsigned *cl;

  for(i = 1 ; i <= *falseBin ; i++)
    {
       if(isAssign[ABS(falseBin[i])] != NOT_ASS) continue; 
      if(!breakcount[ABS(falseBin[i])]) return falseBin[i];
    }

  for(i = 0 ; i<numfalse ; i++)
    {
      cl = false[i];
      for(j = 1 ; j <= *cl ; j++) 
        {
          if(isAssign[ABS(cl[j])] != NOT_ASS) continue; 
          if(!breakcount[ABS(cl[j])]) return cl[j];
        }
    }

  return 0;
}/* isLocalMinimumGSAT */


/**
 * Fonction principal de WSAT
 */
void wsat()
{
  int unsigned var;

  if(propagate()) termineProg(UNS,"UNSATISFIABLE");
  posChoice[0] = tail;
 
  numtry = 1;
  numflip = 0;
  init(0);

 
  if((!numfalse) && (!numfalseBin)) termineProg(SAT,"ASSIGNMENTS FOUND");  
  lastNumflipRestart = numclause;
  maxSatTmp = maxSat = numfalse + numfalseBin;  
  numnullflip = 0;


  /*****************************************************************************************/
  /*                                 Second Loop                                           */
  /*****************************************************************************************/
  while((numfalse > 0) || (numfalseBin > 0))
    {           
      var = pickcode[heuristic]();
     
      if(var == NOVALUE)
        { 
          var = isLocalMinimumGSAT();
          if(!var)
            {
              var = pickextract[heuristicAddClause]();
              lastNumflipRestart = numflipAfterAnalyse;
            }
        }

      flipatom(var);
      update_statistics_end_flip();

      if(numflip >= super(numtry) * 100000)
        {
          showInter();
          simplify();
          if(numclauseLearnt >= sizeLearnt)
            {
              callCleaningDB();
            }
          init(1);  
          numflip = 0;
          lastNumflipRestart = super(numtry) * 1000;
          numtry++;
        }
    }
  
  if((!numfalse) && (!numfalseBin))
    {
      termineProg(SAT,"ASSIGNMENTS FOUND");  
    }

  expertime = elapsed_seconds();
  numflip = 0;
  termineProg(OUT,"TIME_OUT");
}/* wsat */
