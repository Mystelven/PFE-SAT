/**************************************************************************************************/
/**        GASAT is a hybrid genetic algorithm for the satisfiability problem (SAT).             **/
/**        It's incorporates local search within the genetic framework.                          **/
/**                                                                                              **/
/**        Frédéric Lardeux, Frédéric Saubion and Jin-Kao Hao                                    **/
/**        lardeux@univ-angers.fr                                                                **/
/**                                                                                              **/
/**        LERIA, Universite d'Angers                                                            **/
/**        2, Bd Lavoisier, F-49045 Angers Cedex 01                                              **/
/**************************************************************************************************/




/************************************/
/* Standard includes                */
/************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <limits.h>
#include <signal.h>

#include <sys/times.h>
#include <sys/time.h>
#include <time.h>



#define random() rand()
#define srandom(seed) srand(seed)


/************************************/
/* Constant parameters              */
/************************************/

#define STOREBLOCK 2000000	/* size of block to malloc each time */
#define TRUE 1
#define FALSE 0

#define MAXLENGTH 500           /* maximum number of literals which can be in any clause */

/************************************/
/* Internal constants               */
/************************************/

#define Var(CLAUSE, POSITION) (ABS(clause[CLAUSE][POSITION]))

static int scratch;
#define ABS(x) ((scratch=(x))>0?(scratch):(-scratch))

static float fscratch;
#define ABSF(x) ((fscratch=(x))>0?(fscratch):(-fscratch))

#define BIG 100000000



/**************************************************/
/* Main data structures                           */
/**************************************************/

// variables used for the time
long begintime, endtime, mess,temptime;
struct tms *a_tms;

//GASAT parameters
char * benchmark_name; //benchmark's name
int insertion; //condition of insertion for the new individual (1->insertion since condition; 2->insertion if it's better than the worse of the possible parents)
int crossover; //number of the cross (1-9, the best is the 3)
int select_best_individual;  //selection of the possible parents. If 0 then all the individuals can be selected, else if 1 only the BESTINDIVIDUAL are selected
int tabu_search; //If 0 then no tabu search (only cross) else if 1 then tabu search is executed
int distance_between_2_parents; //minimal distance between two possible parents
int tabu_diversification; //if 0 then no diversification mecanism in tabu search, else if 1 then diversification mecanism
int BESTINDIVIDUAL; //number of individuals where the two parents are selected
int NUMINDIVIDUAL; //number of individuals in the population
float timelimit; //time limit in second
int degree; //if 1 then mechanism of dgre in tabu search, if 0 nothing
int maxtimefalse; //a clause can be the last false clause maxtimefalse times before to be put to TRUE
int diversification_step; //number of floor that it put to TRUE
char * file_ham; //file where diversity of population is written after each cross
char * file_result; //file where solutions are written
int ltinit; //length of tabou list for the preliminary search
int ptinit; //maxflip for the preliminary search
int ltrech; //length of tabou list for the real search
int ptrech; //maxflip for the real search
int ltinitpc; //length of tabou list for the preliminary search (percent of numatom) 
int ltrechpc; //length of tabou list for the real search (percent of numatom)
int MAXTRY; //max number of crosses
char * initfile; //init file name
char * popfile; //save the initial configuration of the population
int initoptions; // if TRUE then all the individual are initialized with the same assignment
int result_bit; //the best assignment is written in 0/1
int result_lit; //the best assignment is written in lit/-lit
int savepop; //if 1 then the initial population is written in popfile

//population counters
int cardpopulation; //number of individual at each moment
int youngerindividual; //identification of the younger individual
int bestnumfalse; //number of false clauses for the best individual
int cardnumfalse; //number of individuals with bestnumfalse false clauses
int worsetemporarynumfalse; //number of false clauses for the worse individual in the BESTINDIVIDUAL individuals
int olderindividual; //identification of the older individual

//benchmark counters
int numatom; //number of atoms in the set of CNF clauses
int numclause; //number of clauses in the set of CNF clauses
int ** clause;	/* clauses to be satisfied,  indexed as clause[clause_num][literal_num] */
int *size;	/* length of each clause */
int **occurence;	/* where each literal occurs, indexed as occurence[literal+numatom][occurence_num] */
int *numoccurence;	/* number of times each literal occurs */
char * solution; //save the best configuration

//flip counters
long allflip; //number of flips 
long bestflip; //little number of flips for the best solution

//statistic counters
int varfc; //number of var in false clauses silutaneously in th two parents who are flipped 
int varid; //number of var who have the same value in both parents and who are set in the child
int varrand; //number of variable randomly valued in the child
int vartc; //number of var in true clauses silutaneously in th two parents who are conserved
int **hammingtab; //hamming distance table
float *ham[8];// stock all the parameters for all the crosses
int *ent[2]; //entropy table

//algorithm counters
int FOUND; //0 if no solution found else  1
char ** barycentre; // it used to select the BESTINDIVIDUAL individual but whit a minimal diversity
int maxtry; //number of tries between the start (for MAXTRY downto 0)

/*STRUCTURE of INDIVIDUAL*/
typedef struct struct_individual {
  char *atom;		/* value of each atom */ 
  int *false;		/* clauses which are false */
  int *numtruelit;	/* number of true literals in each clause */
  int *wherefalse;	/* where each clause is listed in false */
  int *breakcount;	/* number of clauses that become unsat if var is flipped */
  int *makecount;	/* number of clauses that become sat if var is flipped */
  int numfalse;	      	/* number of false clauses */
  int numindividual;    /*indentification of the individual*/
  int numhamming;       /*number between 0 to 99*/
  struct struct_individual *next;
}Individual;


/************************************/
/* Global flags and parameters      */
/************************************/

Individual *population = NULL;  

/* Randomization */

int seed;			/* seed for random */


/************************************/
/* Forward declarations             */
/************************************/

//Initialization
void init(Individual *ind,char *initfile, int initoptions,int first); //initialization of the individuals
void initprob(void); //integration of the set of CNF clauses 
int parameters(int argc,char *argv[]); //read parameters

//Structures
void createPopulation(); //creation of the population
Individual *createIndividual(); //creation of individuals
void freeIndividual(Individual * i); //delete the individual
Individual *add_individual(Individual *ind, Individual *x); //add an individual in the population
void insert_individual(Individual * y, Individual ** z); //add an individual in the population with respect to the sort 
int delete_individual(Individual **ind); //delete an individual
int delete_individual2(Individual *y); //delete an individual
int replace2_10(Individual *y,int num);
int replace10(Individual **ind, int num);

//Work
Individual * cross(Individual* p1, Individual* p2); //cross of two individuals
int crossover_operator(Individual **r); //select and cross two individual in the population
Individual * select_best_individual_function(Individual *ind); //select the best individuals in the population
void flipatom(Individual *ind,int toflip); //flip the atom toflip in an individual
Individual * tabu (Individual *ind,int tabu_length, int maxflip); //tabu search apply to the individual
Individual * initial_sort (Individual *ind); //sort all the individuals of the population in relation to the number of false clauses
void save_solution(Individual *ind);

//Distance
float distance(char *tmp, char *tab, int i); //hamming distace between individuals
void hamming_distance_tab(int h, Individual * ind);
int hamming_distance(int p1, int p2,Individual * ind);
float average_hamming_distance();
void copy_distance();
float average_num_false_clause(Individual * ind);
float entropy();

//Edit
void print_values(Individual *ind); //edit the results
void readme(); //help for command line


/************************************/
/* main                             */
/************************************/
	  
int main(int argc,char *argv[]){
	int i,j,h,count;
	Individual * p, *x;
	char * temp;
	
	if (argc == 1)
		readme();
	else {
		seed = getpid();
		srandom(seed);
		if (parameters(argc, argv) == 0) exit(0);   		
   
		//Time structures
		a_tms = ( struct tms *) malloc( sizeof (struct tms));
		mess=times(a_tms); begintime = a_tms->tms_utime;
   
		//Load the benchmark
		initprob();
		
		//Statistic structures
		if (file_ham != "no"){
			hammingtab = (int **) malloc( sizeof(int*) * NUMINDIVIDUAL );
			for (i=0;i<NUMINDIVIDUAL;i++)
				hammingtab[i] = (int *) malloc( sizeof(int) * NUMINDIVIDUAL );			
			for (i=0;i<8;i++)
				ham[i] =(float *) malloc( sizeof(float) * MAXTRY);			
			for (i=0;i<2;i++)
				ent[i] = (int *) malloc( sizeof(int) * (numatom+1));
			for (i=1;i<numatom+1;i++){
				ent[0][i] = 0;
				ent[1][i] = 0;
			}
		}
   
		//Initialisation
		FOUND = 0; //Assignment not found
		cardpopulation = 0;   
		olderindividual = 0;
		cardnumfalse = 0;
		bestnumfalse = numclause;
		allflip = 0;
		varfc = 0;
		vartc = 0;
		varid = 0;
		varrand = 0;

		//Creation of the population
		createPopulation();

		if (((timelimit != -1) && ((double)(temptime-begintime)/CLK_TCK < (double)timelimit)) ||(timelimit == -1)){
			if (FOUND != 1){
				youngerindividual = cardpopulation-1;
				population = initial_sort (population); 
				if (file_ham != "no"){
					//Statistic operations
					for(i=0;i<NUMINDIVIDUAL;i++)
						hamming_distance_tab(i,population);	//initialisation of the hamming matrix
					ham[0][MAXTRY-maxtry] = average_num_false_clause(population); 
					ham[1][MAXTRY-maxtry] = (float)bestnumfalse;
					ham[2][MAXTRY-maxtry] = entropy();
					ham[3][MAXTRY-maxtry] = average_hamming_distance(); 
					ham[4][MAXTRY-maxtry] = 0;
					ham[5][MAXTRY-maxtry] = 0;
					ham[6][MAXTRY-maxtry] = 0;
					ham[7][MAXTRY-maxtry] = 0;
				}
				
				//time control
				if (timelimit != -1){
					mess=times(a_tms);
					temptime = a_tms->tms_utime;
				}
				
				//general process
				while ((bestnumfalse != 0) //if bestnumfalse=0 then an assignment is found
							 && (maxtry != 0) //if maxtry=0 all the crossovers are executed
							 && (FOUND != 1) //if FOUND=1 then an assignment is found
							 && ((timelimit == -1) || ((double)(temptime-begintime)/CLK_TCK < timelimit)) //time control
							 ){
					
					h = crossover_operator(&population);
					//Statistic operations
					if (file_ham != "no"){
						if (h != -1) //if h=-1 then the child was not inserted in the population
							hamming_distance_tab(h,population);
						ham[3][MAXTRY-maxtry] = average_hamming_distance(); 
						ham[0][MAXTRY-maxtry] = average_num_false_clause(population);
						ham[1][MAXTRY-maxtry] = (float)bestnumfalse;
						ham[2][MAXTRY-maxtry] = entropy();
						ham[4][MAXTRY-maxtry] = varfc;
						ham[5][MAXTRY-maxtry] = varid;
						ham[6][MAXTRY-maxtry] = varrand;
						ham[7][MAXTRY-maxtry] = vartc;
					}
					maxtry--;
					//time control
					if (timelimit != -1){
						mess=times(a_tms);
						temptime = a_tms->tms_utime;
					}
				}
				if (FOUND != 1) {mess=times(a_tms); endtime = a_tms->tms_utime;}
				if( file_ham != "no") copy_distance();
			}
		}
		if (FOUND == 1) {
			mess=times(a_tms); 
			endtime = a_tms->tms_utime;
			printf("SATISFIABLE, time : %5.3f seconds , %d flips\n",(double)(endtime-begintime)/CLK_TCK,bestflip);
		}
		else {
			mess=times(a_tms); 
			endtime = a_tms->tms_utime;
			if( file_result != "no") print_values(population);
			printf("UNSATISFIABLE, time :  %5.3f seconds \nBest solution with %d false clause(s) in %d flips\n",(double)(endtime-begintime)/CLK_TCK,bestnumfalse,bestflip);
		}
		
		//free memory
		for(i=0;i<BESTINDIVIDUAL;i++)
			free(barycentre[i]);
		free(barycentre);
	}
}


/***********************************************/
/* init : initializes the benchmark structures */
/***********************************************/

void init(Individual *ind,char *initfile, int initoptions,int first){
	int i, j, thetruelit, lit, bit, num_ind;
	FILE * infile;
	char * end;

	//initialization
	for(i=0;i<numclause;i++){
		ind->numtruelit[i] = 0;
		ind->false[i] = 0;
	}
	ind->numfalse = 0;
	ind->makecount[0] = 0;
	ind->breakcount[0] = 0;
	for(i=1;i<numatom+1;i++){
		ind->breakcount[i] = 0;
		ind-> makecount[i] = 0;
	}
	
	//Only the first initialization
	if (first == TRUE){
		if (initoptions == FALSE){
			//random initialization
			if (savepop == 1)
				if ((infile = fopen(popfile, "a")) == NULL){
					fprintf(stderr, "Cannot open %s\n", popfile);
					exit(1);
				}
			for(i=1;i<numatom+1;i++){
 				ind->atom[i] = random()%2;			 
				//				ind->atom[i] = 0;			
				if (savepop == 1)
					fprintf(infile,"%d ",ind->atom[i]);
			}
			if (savepop == 1){
				fprintf(infile,"end\n");
				fclose(infile);		
			}	
		}			
		else{
			//initialization with a file
			if ((infile = fopen(initfile, "r")) == NULL){
				fprintf(stderr, "Cannot open %s\n", initfile);
				exit(1);
			}
			for (num_ind=1;num_ind < cardpopulation;num_ind++)
				while (fgetc(infile) != '\n');
			i=0;
			bit = -1;
			while (fscanf(infile, "%d ", &lit)==1){
				i++;
				if (ABS(lit)>numatom){
					fprintf(stderr, "Bad init file %s, %d\n", initfile,lit);
					exit(1);
				}
				if (i == 1){
					if (lit == 1)
						ind->atom[1]=1;
					else
						if (lit == 0){
							bit = 1;
							ind->atom[1]=0;
						}
						else{
							bit = 0;
							ind->atom[1]=-1;
						}
				}													
				else{
					if ((bit = -1) && ((lit == 0) || (lit == 1)))
						bit = 1;
					else
						bit = 0;
					if (bit == 0){
						if (lit<0) ind->atom[-lit]=0;
						else ind->atom[lit]=1;
					}
					else
						ind->atom[i]=lit;
				}
			}
			if (i==0){
				fprintf(stderr, "Bad init file %s\n", initfile);
				exit(1);
			}
			close(infile);
		}
	}
	/* Initialize breakcount and makecount in the following: */
	for(i=0;i<numclause;i++){
		for(j=0;j<size[i];j++){
			if((clause[i][j] > 0) == ind->atom[ABS(clause[i][j])]){
				ind->numtruelit[i]++;
				thetruelit = clause[i][j];
		  }
		}
	  if(ind->numtruelit[i] == 0){
			ind->wherefalse[i] = ind->numfalse;
			ind->false[ind->numfalse] = i;
			ind->numfalse++;
			for(j = 0;j < size[i];j++)
				ind->makecount[ABS(clause[i][j])]++;			
		}
	  else {
	    if (ind->numtruelit[i] == 1)	      
				ind->breakcount[ABS(thetruelit)]++;	      
	    ind->wherefalse[i] = -1;
	  }	
	}
}


/**************************************/
/* initprob : read the benchmark file */
/**************************************/
void initprob(void){
	int i;
	int j;
	int lastc;
	int nextc;
	int *storeptr;
	int freestore;
	int lit;
	FILE * bench_file;
	
	/*commentaries and parameters*/
	if ((bench_file = fopen(benchmark_name, "r")) == NULL){
		fprintf(stderr, "Cannot open %s\n", benchmark_name);
		exit(1);
	}		
	while ((lastc = fgetc(bench_file)) == 'c')
		while ((nextc = fgetc(bench_file)) != EOF && nextc != '\n');	
	ungetc(lastc,bench_file);
	if (fscanf(bench_file,"p cnf %i %i",&numatom,&numclause) != 2){
		fprintf(stderr,"Bad input file\n");
		exit(-1);
	}
	
	//initialization of structures
	if(( ltinitpc != 0) && (ltinit == -1)) ltinit = numatom*ltinitpc/100;
	if(( ltinitpc == 0) && (ltinit != -1)) ltinitpc = ltinit*100/numatom;
	if(( ltinitpc == 0) && (ltinit == -1)) {
		ltinitpc = 5;
		ltinit = 0.05 * numatom;
	}
	if(( ltrechpc != 0) && (ltrech == -1)) ltrech = numatom*ltrechpc/100;
	if(( ltrechpc == 0) && (ltrech != -1)) ltrechpc = ltrech*100/numatom;
	if(( ltrechpc == 0) && (ltrech == -1)) {
		ltrechpc = 10;
		ltrech = 0.1 * numatom;
	}
	if (ptinit == -1) ptinit = 1000;
	if( ptrech == -1) ptrech = 10000;  
	
	solution = (char *)malloc(sizeof(char) * (numatom+1)); 
	numoccurence = (int *) malloc( sizeof(int) * (2*numatom+1));
	occurence = (int **) malloc( sizeof(int*) * (2*numatom+1));
	clause = (int **) malloc( sizeof(int*) * numclause );
	size = (int *) malloc( sizeof(int) * numclause );
	barycentre = (char**)malloc(sizeof(char*)*BESTINDIVIDUAL);
	for(i=0;i<BESTINDIVIDUAL;i++)
		barycentre[i] = malloc(sizeof(char)*numclause);
	
	//stockage work
	freestore = 0;
	for(i=0;i<2*numatom+1;i++)
		numoccurence[i] = 0;
	for(i=0;i<numclause;i++){
		size[i] = -1;
		if (freestore < MAXLENGTH) {
			storeptr = (int *) malloc( sizeof(int) * STOREBLOCK );
			freestore = STOREBLOCK;
		}
		clause[i] = storeptr;
		do{
			size[i]++;
			if(size[i] > MAXLENGTH){
				printf("ERROR - clause too long\n");
				exit(-1);
			}
			if (fscanf(bench_file,"%i ",&lit) != 1){
				fprintf(stderr, "Bad input file\n");
				exit(-1);
			}
			if(lit != 0){ /* because 0 is the last symbol of each line*/
				*(storeptr++) = lit; /*clause[i][size[i]] = j; */
				freestore--;
				numoccurence[lit+numatom]++; 
			}
		}
		while(lit != 0);
	}
	if(size[0] == 0){
		fprintf(stderr,"ERROR - incorrect problem format or extraneous characters\n");
		exit(-1);
	}
	/* space memory of occurence structure */
	for(i=0;i<2*numatom+1;i++){
		if (freestore < numoccurence[i]){
			storeptr = (int *) malloc( sizeof(int) * STOREBLOCK );
			freestore = STOREBLOCK;
		}
		occurence[i] = storeptr;
		freestore -= numoccurence[i];
		storeptr += numoccurence[i]; 
		numoccurence[i] = 0;
	}
	/* filling of occurence structure */
	for(i = 0;i < numclause;i++)
		for(j = 0;j < size[i];j++){
			occurence[clause[i][j]+numatom][numoccurence[clause[i][j]+numatom]] = i;
			numoccurence[clause[i][j]+numatom]++;
		}
	fclose(bench_file);
}

/*******************************************************************/
/* createIndividual : create and return an individual memory space */
/*******************************************************************/
Individual *createIndividual(){
	Individual *i;	
	i = (Individual *) malloc(sizeof(Individual)); 
	i->false = (int *)malloc(sizeof(int) * (numclause));
	i->makecount = (int *)malloc(sizeof(int) * (numatom+1)); 
	i->breakcount = (int *)malloc(sizeof(int) * (numatom+1)); 
	i->atom = (char *)malloc(sizeof(char) * (numatom+1)); 
	i->wherefalse = (int *)malloc(sizeof(int) * (numclause)); 
	i->numtruelit = (int *)malloc(sizeof(int) * (numclause)); 
	i->next = NULL;
	i->numfalse = 0;
	i->numindividual = -1;
	return i;
}

/*******************************************************************/
/* freeIndividual : release an individual memory space */
/*******************************************************************/
void freeIndividual(Individual * i){  
  free(i->breakcount); 
  free(i->makecount); 
  free(i->atom); 
  free(i->wherefalse); 
  free(i->false); 
  free(i->numtruelit); 
  free(i);
}


/***********************************************************************/
/* createPopulation : create a population of NUMINDIVIDUAL individuals */
/***********************************************************************/
void createPopulation(){
  int i,j;
  Individual *x;

  for(i=0; i<NUMINDIVIDUAL; i++){
    x=createIndividual();
    x->numindividual = cardpopulation;
    x->numhamming = cardpopulation;
    cardpopulation++;
    init(x,initfile, initoptions,TRUE); //TRUE because it is the first initialization
		//improvement of all the individuals
    if ((tabu_search == 1) && (ptinit != 0))
			x = tabu(x,ltinit,ptinit);
		//Statistics processus
		if (file_ham != "no"){ 
			for (j=1;j<numatom+1;j++)
				ent[x->atom[j]][j]++;
		}
		//Time control
    if (timelimit != -1){ 
      mess=times(a_tms);
      temptime = a_tms->tms_utime;
    }
		//Insertion of the new individual inthe population
    if (i == 0)
      population = x;    		
    else{ 
      x->next=population;
      population=x;
    }  
		//Save the best individual
    if (x->numfalse <= bestnumfalse){
      if (x->numfalse == bestnumfalse) cardnumfalse++;
      else {
				bestflip = allflip;
				bestnumfalse = x->numfalse;
				cardnumfalse = 1;
				save_solution(x);
      }
    }
    if ((FOUND == 1) || ((timelimit != -1) && ((double)(temptime-begintime)/CLK_TCK >= timelimit))) break;
  }
}


/*****************************************************************************/
/* cross : cross two individuals and return a child. The best variable in the false clauses for the two parent is flipped and for the other variables, if they are the same value for the two parents then the value is kept. The other values are randomly valued.*/
/*****************************************************************************/
Individual * cross(Individual* p1,Individual* p2){
  int i,j,best,numbest,lit; 
  Individual *child;
  
	//Creation and initialization of the child 
  child=createIndividual();  
  child->numindividual = youngerindividual+1;
  cardpopulation++;
  youngerindividual++;
  for(i=1;i<numatom+1;i++)
		child->atom[i]=-1;

	//Statistic counters
	varfc = 0;
	vartc = 0;
	varid = 0;
	varrand = 0;

	    
  /* cross 5*/
	if (crossover == 5){
		for (i=0 ;i<numclause ;i++){
			//work on falseclauses
			if ((p1->wherefalse[i] != -1) && (p2->wherefalse[i] != -1)){
				best = -BIG;
				for(j=0;j<size[i];j++)
					if (child->atom[Var(i,j)] == -1) 
						if (p1->makecount[Var(i,j)]-p1->breakcount[Var(i,j)]+p2->makecount[Var(i,j)]-p2->breakcount[Var(i,j)] > best) {
							numbest = Var(i,j); 
							best = p1->makecount[numbest]-p1->breakcount[numbest]+p2->makecount[numbest]-p2->breakcount[numbest];
						}
				if (best != -BIG) {
					child->atom[numbest]=1-p2->atom[numbest];
					varfc++;
					allflip++;
				}
			}
		}
		for (i=0 ;i<numclause ;i++){
			//work on true clauses
			if ((p1->wherefalse[i] == -1) && (p2->wherefalse[i] == -1)){
				best = BIG;
				for(j=0;j<size[i];j++)
					if (child->atom[Var(i,j)] == -1)
						//attention, tous les cas ne sont pas considérés
						if (!(((clause[i][j] > 0) && (p1->atom[Var(i,j)] == FALSE) && (p2->atom[Var(i,j)] == FALSE)) ||
								((clause[i][j] < 0) && (p1->atom[Var(i,j)] == TRUE) && (p2->atom[Var(i,j)] == TRUE))
								)){
							if (best >p1->makecount[Var(i,j)]-p1->breakcount[Var(i,j)]+p2->makecount[Var(i,j)]-p2->breakcount[Var(i,j)]){
								numbest = Var(i,j); 
								lit = clause[i][j];
								best = p1->makecount[numbest]-p1->breakcount[numbest]+p2->makecount[numbest]-p2->breakcount[numbest];					 
							}
						}
				if (best != BIG){
					if ((lit > 0) == p1->atom[numbest])
						child->atom[numbest] = p1->atom[numbest]; 
					else
						child->atom[numbest] = p2->atom[numbest]; 
					vartc++;
				}
			}
		}
		for (i=0; i<numatom+1; i++)
			if (child->atom[i] == -1){
				if (p1->atom[i] == p2->atom[i]){
					child->atom[i] = p1->atom[i];
					varid++;
				}
				else {
					child->atom[i] = random()%2;
					varrand++;
					printf("random\n");
				}
			}
	}
	/* end cross 5 */

  /* cross 51*/
	if (crossover == 51){
		for (i=0 ;i<numclause ;i++){
			//work on falseclauses
			if ((p1->wherefalse[i] != -1) && (p2->wherefalse[i] != -1)){
				best = -BIG;
				for(j=0;j<size[i];j++)
					if (child->atom[Var(i,j)] == -1){
						if (p1->makecount[Var(i,j)]-p1->breakcount[Var(i,j)]+p2->makecount[Var(i,j)]-p2->breakcount[Var(i,j)] > best) {
							numbest = Var(i,j); 
							best = p1->makecount[numbest]-p1->breakcount[numbest]+p2->makecount[numbest]-p2->breakcount[numbest];
						}
					}
					else {
						best = -BIG;
						break;
					}
				if (best != -BIG) {
					child->atom[numbest]=1-p2->atom[numbest];
					varfc++;
					allflip++;
				}
			}
		}
		for (i=0 ;i<numclause ;i++){
			//work on true clauses
			if ((p1->wherefalse[i] == -1) && (p2->wherefalse[i] == -1)){
				best = BIG;
				for(j=0;j<size[i];j++)
					if (child->atom[Var(i,j)] == -1){
						if (((clause[i][j] > 0) == p1->atom[Var(i,j)]) || ((clause[i][j] > 0) == p2->atom[Var(i,j)])){
							if (best > p1->makecount[Var(i,j)]-p1->breakcount[Var(i,j)]+p2->makecount[Var(i,j)]-p2->breakcount[Var(i,j)]){
								numbest = Var(i,j); 
								lit = clause[i][j];
								best = p1->makecount[numbest]-p1->breakcount[numbest]+p2->makecount[numbest]-p2->breakcount[numbest];
							}
						}
					}
					else{
						if((clause[i][j] > 0) == child->atom[Var(i,j)]){
							best = BIG;
							break;
						}
					}
				if (best != BIG){
					if ((lit > 0) == p1->atom[numbest])
						child->atom[numbest] = p1->atom[numbest]; 
					else
						child->atom[numbest] = p2->atom[numbest]; 						
					vartc++;
				}
			}
		}
		for (i=0; i<numatom+1; i++)
			if (child->atom[i] == -1){
				if (p1->atom[i] == p2->atom[i]){
					child->atom[i] = p1->atom[i];
					varid++;
				}
				else {
					child->atom[i] = random()%2;
					varrand++;
				}
			}
	}
	/* end cross 51 */

	/*cross 7 : uniform */
	if (crossover == 7){
		for (i=1;i<numatom+1;i++){
			j = random()%2;
			if (j == 0) child->atom[i] = p1->atom[i];
			else child->atom[i] = p2->atom[i];
			if (p1->atom[i] != p2->atom[i])
				varrand++;
		}
	}
	/*end cross 7 */

	
	/* cross 9 (fleurent ferland):*/
	if (crossover == 9){
		//work on false clauses of p1 and true clauses of p2
		for (i=0 ;i<numclause ;i++){
			if ((p1->wherefalse[i] != -1) && (p2->wherefalse[i] == -1)){
				for (j=0;j<size[i];j++)
					if (child->atom[Var(i,j)] == -1) 
						child->atom[Var(i,j)]=p2->atom[Var(i,j)];
			}
		}
		//work on false clauses of p2 and true clauses of p1
		for (i=0 ;i<numclause ;i++){
			if ((p2->wherefalse[i] != -1) && (p1->wherefalse[i] == -1)){
				for (j=0;j<size[i];j++)
					if (child->atom[Var(i,j)] == -1) 
						child->atom[Var(i,j)]=p1->atom[Var(i,j)];
			}
		}
		//work on the other variables
		for (i=1;i<numatom+1;i++)
			if (child->atom[i] == -1){
				if (random()%2 == 0)
					child->atom[i] = p1->atom[i];
				else
					child->atom[i] = p2->atom[i];
				if (p1->atom[i] != p2->atom[i])
					varrand++;
			}
	}
	/* end cross 9*/

	if (crossover == 3){  
		for (i=0 ;i<numclause ;i++){
			if ((p1->wherefalse[i] != -1) && (p2->wherefalse[i] != -1)){ //false clauses
				best = -BIG;
				for(j=0;j<size[i];j++)
					if (child->atom[Var(i,j)] == -1){
						if (p1->makecount[Var(i,j)]-p1->breakcount[Var(i,j)]+p2->makecount[Var(i,j)]-p2->breakcount[Var(i,j)] > best) {
							numbest = Var(i,j); 
							best = p1->makecount[numbest]-p1->breakcount[numbest]+p2->makecount[numbest]-p2->breakcount[numbest];
						}
					}
					else{
						best = -BIG;
						break;
					}		 
				if (best != -BIG) {
					child->atom[numbest]=1-p2->atom[numbest];
					varfc++;
					allflip++;
				}
			}		 
		}
		for (i=1; i<numatom+1; i++)
			if (child->atom[i] == -1){
				if (p1->atom[i] == p2->atom[i]){ //same value
					child->atom[i] = p1->atom[i];
					varid++;
				}
				else{
					child->atom[i] = random()%2; //random value
					varrand++;
				}
			}
	}

	//Initialization of the breakcount and makecount structures
  init(child,initfile, initoptions, FALSE); //FALSE because its not the first initialization
	return child;
}


/****************************************************************/
/* save_solution : save an individual as the best solution found*/
/****************************************************************/
void save_solution(Individual *ind){
	int i;
	for (i=1; i< numatom+1; i++)
		solution[i] = ind->atom[i];
}

/******************************************************************************************/
/* flipatom : flip the variable toflip in the individual ind and update all the structures*/
/******************************************************************************************/
void flipatom(Individual *ind,int toflip){
	int i, j;			
	int toenforce;		
	register int cli;
	register int lit;
	int numocc;
	register int sz;
	register int * litptr;
	int * occptr;
	
	if(ind->atom[toflip] > 0)
		toenforce = -toflip;
	else
		toenforce = toflip;
	ind->atom[toflip] = 1-ind->atom[toflip];
	
	numocc = numoccurence[numatom-toenforce];
	occptr = occurence[numatom-toenforce];
	for(i = 0; i < numocc ;i++){
		/* cli = occurence[numatom-toenforce][i]; */
		cli = *(occptr++);			
	  if (--(ind->numtruelit[cli]) == 0){
			ind->false[ind->numfalse] = cli;
			ind->wherefalse[cli] = ind->numfalse;
			ind->numfalse++;
			/* Decrement toflip's breakcount */
			ind->breakcount[toflip]--;	      
			/* Increment the makecount of all vars in the clause */
			sz = size[cli];
			litptr = clause[cli];
			for (j=0; j<sz; j++){
				/* lit = clause[cli][j]; */
				lit = *(litptr++);
				ind->makecount[ABS(lit)]++;
			}
	  }
	  else if (ind->numtruelit[cli] == 1){
			/* Find the lit in this clause that makes it true, and inc its breakcount */
			sz = size[cli];
			litptr = clause[cli];
			for (j=0; j<sz; j++){
				/* lit = clause[cli][j]; */
				lit = *(litptr++);
				if((lit > 0) == ind->atom[ABS(lit)]){
		      ind->breakcount[ABS(lit)]++;
		      break;
				}
			}
	  }
	}	
	numocc = numoccurence[numatom+toenforce];
	occptr = occurence[numatom+toenforce];
	for(i = 0; i < numocc; i++){
		/* cli = occurence[numatom+toenforce][i]; */
		cli = *(occptr++); 	 
	  if (++(ind->numtruelit[cli]) == 1){
			ind->numfalse--;
			ind->false[ind->wherefalse[cli]] = ind->false[ind->numfalse];	     	       
			ind->wherefalse[ind->false[ind->numfalse]] = ind->wherefalse[cli];
			/* Increment toflip's breakcount */
			ind->breakcount[toflip]++;
			/* Decrement the makecount of all vars in the clause */
			sz = size[cli];
			litptr = clause[cli];
			for (j=0; j<sz; j++){
				/* lit = clause[cli][j]; */
				lit = *(litptr++);
				ind->makecount[ABS(lit)]--;
			}
	  }
	  else if (ind->numtruelit[cli] == 2){
			/* Find the lit in this clause other than toflip that makes it true,
				 and decrement its breakcount */
			sz = size[cli];
			litptr = clause[cli];
			for (j=0; j<sz; j++){
				/* lit = clause[cli][j]; */
				lit = *(litptr++);
				if(((lit > 0) == ind->atom[ABS(lit)]) && (toflip != ABS(lit))){
		      ind->breakcount[ABS(lit)]--;
		      break;
				}
			}
	  }
	}
}

/*********************************************************************************************************************************/
/* tabu : execute a tabu search on the individual ind with a tabu list of length tabu_length and a maximum flip number of maxflip*/
/*********************************************************************************************************************************/
Individual * tabu (Individual *ind,int tabu_length, int maxflip){
  int numflip = 0;
  int *tabu_atom;
  int ii,i, max, bestatom,j,jj,sumplus,summoins,p_best_atom,cnt;
	char * tabusol;
	int  tabunumfalse;

  //diversification variables
  int best=BIG;
  int batom,bflip;
  int lastnumfalse;
  int cpt,tmp,npossible;
  int *fausse;//represents how many times a clause was the unique false clause
  int *interdit;//represents the variables which can't be flipped same with the aspiration
  int *possible;//represents the variables which can be flipped (only one is flipped)
  int *best_atom;//represents the best variables after the degree mecanism. One is flipped
  int longinterdit = numatom / 10;// length of the tabu list
  int *V_atom[4];//used for the degree mecanism
  float resultplus,resultmoins,sol;//used for the degree mecanism


	//Initialization
  fausse = (int*)malloc(sizeof(int) * numclause);
  interdit = (int *)malloc(sizeof(int) * (numatom+1));
  possible = (int *)malloc(sizeof(int) * (numatom+1));
  best_atom = (int *)malloc(sizeof(int) * (numatom+1));
  tabu_atom = (int *)malloc(sizeof(int) * (numatom+1));
  tabusol = (char *)malloc(sizeof(char) * (numatom+1));
  for (i=0;i<4;i++)
    V_atom[i] = (int *)malloc(sizeof(int) * (2*numatom+1));
  for (i=1; i<numatom+1; i++){
    tabu_atom[i] = -tabu_length;
    interdit[i] = -longinterdit;
  }
  for (i=0; i<numclause; i++)
    fausse[i] = 0;
	for(i=1;i< numatom+1;i++)
		tabusol[i] = ind->atom[i];
	tabunumfalse = ind->numfalse;

	//time control
	if (timelimit != -1){
    mess=times(a_tms);
    temptime = a_tms->tms_utime;
  }
	
	//tabu search
	//break conditions : numfalse = 0
	//                   max flip is reached
	//                   limit time is reached
  while ((ind->numfalse > 0) 
				 && (numflip < maxflip) 
				 && ((timelimit == -1) || ((double)(temptime-begintime)/CLK_TCK < timelimit))
				 ){
    for (i=1;i<numatom+1;i++)
      possible[i] = -1;
		npossible = 0;
    max = -BIG;// represent the best number of true clauses created with the flip of the variable
    cnt = 0;
    for (i=1; i<numatom+1; i++){
      if (numflip - longinterdit >= interdit[i]){  //the variable i is not forbidden
				if (numflip - tabu_length >= tabu_atom[i]){//the variable i is not tabu
					if (ind->makecount[i] - ind->breakcount[i] >= max){
						if (ind->makecount[i] - ind->breakcount[i] > max){
							max = ind->makecount[i] - ind->breakcount[i];
							for (j=1;j<i;j++)
								possible[j] = -1;
							possible[i] = 1; // the variable i is the best variable to flip
							npossible = 1;
						}
						else{
							possible[i] = 1;	// the variable i is one of the best variable to flip
							npossible++;
						}
					}
				}
				/*aspiration*/
				else {//the variable i is tabu
					if (ind->numfalse - ( ind->makecount[i] - ind->breakcount[i] ) < tabunumfalse){
						max = ind->makecount[i] - ind->breakcount[i];
						for (j=1;j<i;j++)
							possible[j] = -1;
						possible[i] = 1;
						npossible = 1;
					}
				}
      }
    }
		
		if (degree == 1){
			//choice of the best variable among the best to flip with the degree mechanism
			p_best_atom = 0;//number of the best variables (indice in the array best_atom)
			if (npossible == 1){
				for(ii=1;ii<numatom+1;ii++)
					if (possible[ii] == 1){
						best_atom[0] = ii;
						break;
					}
			}
			else {
				for (ii=0;ii<4;ii++)
					for (jj=0;jj<2*numatom+1;jj++)
						V_atom[ii][jj] = 0;
				sol = -BIG;//the variable who has the best sol can be flipped
				for (ii=1;ii<numatom+1;ii++){
					sumplus = 0;
					summoins = 0;
					resultplus = 0;
					resultmoins = 0;
					if (possible[ii] == 1){// the variable ii is one of the best
						for (j=0;j<numoccurence[ii+numatom];j++){//work on clauses where ii appears positively
							tmp = ind->numtruelit[occurence[ii+numatom][j]];
							if (tmp < 4)//we only work on degree inferior to 4
								V_atom[tmp][ii+numatom]++;//this literal has incremented his counter of positive degree
						}
						for (j=0;j<numoccurence[numatom-ii];j++){//work on clauses where ii appears negatively
							tmp = ind->numtruelit[occurence[numatom-ii][j]];
							if (tmp < 4)
								V_atom[tmp][numatom-ii]++;
						}
						if (ind->atom[ii] == 1){//the variable ii is TRUE
							//for (jj=2;jj<4;jj++){
							for (jj=1;jj<4;jj++){ //new
								//resultplus = resultplus + jj*V_atom[jj][numatom+ii];
								resultplus = resultplus + jj*(V_atom[jj][numatom+ii]-1);//new
								sumplus = sumplus + V_atom[jj][numatom+ii];//number of time where ii appear positively in a clause
							}
							//for (jj=1;jj<3;jj++){
							for (jj=0;jj<3;jj++){ //new
								//resultmoins = resultmoins + jj*V_atom[jj][numatom-ii];
								resultmoins = resultmoins + jj*(V_atom[jj][numatom-ii]+1);//new
								summoins = summoins + V_atom[jj][numatom-ii];//number of time where ii appear negatively in a  clause
							}
							if (sumplus == 0) resultplus = 0;//else division by 0
							else resultplus = resultplus/(float)sumplus;
							if (summoins == 0) resultmoins = 0;//else division by 0
							else resultmoins = resultmoins/(float)summoins;
							//debut new
							if (sol <= resultplus+resultmoins){
								if (sol < resultplus+resultmoins){
									p_best_atom = 0;// the variable is the best
									best_atom[p_best_atom] = ii;// the variable is put in first position in the array (indice 0)
									sol = resultplus+resultmoins;// new value to perform
								}
								else{
									p_best_atom++;// it's not the only best variable
									best_atom[p_best_atom] = ii;// it's put in the p_best_atom position
								}
							}
							//fin new
/* 							if (sol <= resultplus-resultmoins){ */
/* 								if (sol < resultplus-resultmoins){ */
/* 									p_best_atom = 0;// the variable is the best */
/* 									best_atom[p_best_atom] = ii;// the variable is put in first position in the array (indice 0) */
/* 									sol = resultplus-resultmoins;// new value to perform */
/* 								} */
/* 								else{ */
/* 									p_best_atom++;// it's not the only best variable */
/* 									best_atom[p_best_atom] = ii;// it's put in the p_best_atom position */
/* 								} */
/* 							} */
						}
						else {//the variable ii is FALSE
							for (jj=0;jj<3;jj++){//new
								// for (jj=1;jj<3;jj++){
								resultplus = resultplus + jj*(V_atom[jj][numatom+ii]+1);//new
								//resultplus = resultplus + jj*V_atom[jj][numatom+ii];
								sumplus = sumplus + V_atom[jj][numatom+ii];//number of time where ii appear positively in a clause
							}
							for (jj=1;jj<4;jj++){//new
								//for (jj=2;jj<4;jj++){
								resultmoins = resultmoins + jj*(V_atom[jj][numatom-ii]-1);//new
								//resultmoins = resultmoins + jj*V_atom[jj][numatom-ii];
								summoins = summoins + V_atom[jj][numatom-ii];//number of time where ii appear negatively in a  clause
							}
							if (sumplus == 0) resultplus = 0;//else division by 0
							else resultplus = resultplus/(float)sumplus;
							if (summoins == 0) resultmoins = 0;//else division by 0
							else resultmoins = resultmoins/(float)summoins;
							//debut new
							if (sol <= resultmoins+resultplus){
								if (sol < resultmoins+resultplus){
									p_best_atom = 0;// the variable is the best
									best_atom[p_best_atom] = ii;// the variable is put in first position in the array (indice 0)
									sol = resultmoins+resultplus;// new value to augment
								}
								else{
									p_best_atom++;// it's not the only best variable
									best_atom[p_best_atom] = ii; // it's put in the p_best_atom position
								}
							}
							//fin new
/* 							if (sol <= resultmoins-resultplus){ */
/* 								if (sol < resultmoins-resultplus){ */
/* 									p_best_atom = 0;// the variable is the best */
/* 									best_atom[p_best_atom] = ii;// the variable is put in first position in the array (indice 0) */
/* 									sol = resultmoins-resultplus;// new value to augment */
/* 								} */
/* 								else{ */
/* 									p_best_atom++;// it's not the only best variable */
/* 									best_atom[p_best_atom] = ii; // it's put in the p_best_atom position */
/* 								} */
/* 							} */
						}
					}
				}		
			}	
			//end of the degree mechanism

			
			if (p_best_atom == 0) bestatom = best_atom[0];//only one variable is possible
			else bestatom = best_atom[random()%(p_best_atom+1)];//if some variables are possible we take one randomly
		}
		else{  //degree == 0
			sol = (random()%npossible) + 1;					
			for (i=1;i<numatom+1;i++){
				if (possible[i] == 1){
					sol--;
					if (sol == 0)
						break;
				}
			}
			bestatom = i;
		}
		tabu_atom[bestatom] = numflip;
		flipatom(ind, bestatom);// the variable ii is flipped
		if (ind->numfalse < tabunumfalse){
			for(i=1;i<numatom+1;i++)
				tabusol[i] = ind->atom[i];
			tabunumfalse = ind->numfalse;
			bflip = numflip;
		}
		
		//diversification mechanism (if a clause is often the last false clause, we force it to be TRUE
		if (tabu_diversification == 1){
			if (ind->numfalse == 1) {
				fausse[ind->false[0]]++;//incrementation of the counter for the last false clause
			}
			if (fausse[ind->false[0]] >= maxtimefalse){// if the limit is reached (here maxtimefalse) we force it
				best = -BIG;
				batom = 0;
				for(cpt=0;cpt<diversification_step;cpt++){// we force recursively diversification_step times
					lastnumfalse = ind->numfalse;
					for(j=0;j<numclause;j++){
						if (ind->numtruelit[j] == 0){ //the variable j is FALSE
							for(i=0;i<size[j];i++)// we search the variable with the flip who gives the biggest number of false clauses
								if ((ind->breakcount[Var(j,i)] - ind->makecount[Var(j,i)] > best) && (numflip - longinterdit >= interdit[Var(j,i)])){
									best = ind->breakcount[Var(j,i)] - ind->makecount[Var(j,i)];
									batom = Var(j,i);
								}
							if (batom != 0){
								tabu_atom[batom] = numflip;//the variable batom is put in the tabu list
								interdit[batom] = numflip;//the variable batom is put in the forbidden list
								flipatom(ind,batom);
								numflip++;
								fausse[j] = 0;
							}
						}
						lastnumfalse--;
						if (lastnumfalse == 0) break;
					}
				}
			}
			//end diversification mechanism
		}
		
		numflip++;//incrementation of the flip number
		//time control
		if (timelimit != -1){ 
			mess=times(a_tms);
			temptime = a_tms->tms_utime;
		}
	}
	for(i=1;i<numatom+1;i++)
		ind->atom[i] = tabusol[i];

	init(ind,initfile, initoptions, FALSE);
	
	allflip = allflip + bflip;
	
	if (ind->numfalse < bestnumfalse){
		save_solution(ind);
		bestnumfalse = ind->numfalse;
		bestflip = allflip;
	}

	if (ind->numfalse == 0) {
		mess=times(a_tms); endtime = a_tms->tms_utime;
		FOUND = 1;
		if( file_result != "no") print_values(ind);
	}
	
	free(fausse);
	free(possible);
	free(interdit);
	free(tabu_atom);
	free(tabusol);
	free(best_atom);
	for (i=0;i<4;i++)
		free(V_atom[i]);
	return ind;
}

/*****************************************************************************************************************/
/* crossover_operator : selects 2 parents, crosses them and improves the child before insert it in the population*/
/*****************************************************************************************************************/
int crossover_operator(Individual **r){
  int h,i,j,k,cpt,max,numparent;
  Individual *x, *y, *z, *t,*p1, *p2,*tmp,*tmp2;
  

  if (select_best_individual != 2){
		if ((select_best_individual == 1) && (BESTINDIVIDUAL != NUMINDIVIDUAL)){
      x = select_best_individual_function(*r);
      i = random()%BESTINDIVIDUAL;
      j = random()%BESTINDIVIDUAL;
      while (j == i) j = random()%BESTINDIVIDUAL;			
			tmp = x;
		}
		else if (select_best_individual == 0){
			i = random()%NUMINDIVIDUAL;
			j = random()%NUMINDIVIDUAL;
			while (j == i) j = random()%NUMINDIVIDUAL;
			tmp = *r;
			tmp2 = tmp;
			for (cpt = 0;cpt < NUMINDIVIDUAL / 3;cpt++)
				tmp2 = tmp2->next;
			worsetemporarynumfalse = tmp2->numfalse;
		}
		k = 0;
		cpt = 0;
		while ((tmp != NULL) && (cpt < 2)){
			if (k == i) {p1=add_individual(NULL,tmp);cpt++;}
			if (k == j) {p2=add_individual(NULL,tmp);cpt++;}
			k++;
			tmp = tmp->next;
		}    
		
		if ((select_best_individual == 1) && (BESTINDIVIDUAL != NUMINDIVIDUAL))
			while (x != NULL){
				z = x;
				x = x->next;
				freeIndividual(z);
			}
	}
	else{
		i = NUMINDIVIDUAL+1;
		j = NUMINDIVIDUAL+1;
		max = i;
		for (k=0;k<BESTINDIVIDUAL;k++){
			cpt=random()%NUMINDIVIDUAL;
			while ((cpt == i) || (cpt == j)) 
				cpt=random()%NUMINDIVIDUAL;
			if (cpt < max) {
				if (i < j) j = cpt;
				else i = cpt;
				if (i < j) max = j;
				else max = i;
			}
		}
		tmp = (*r);
		cpt = 0;
		worsetemporarynumfalse = -1;
		while ((cpt < max+1) || (cpt < BESTINDIVIDUAL+1)){
			if (cpt == i){
				p1=add_individual(NULL,tmp);
				if (worsetemporarynumfalse < p1->numfalse) worsetemporarynumfalse = p1->numfalse;
			}
			if (cpt == j){
				p2=add_individual(NULL,tmp);
				if (worsetemporarynumfalse < p2->numfalse) worsetemporarynumfalse = p2->numfalse;
			}
			cpt++;
			if (tmp->numfalse > worsetemporarynumfalse) worsetemporarynumfalse = tmp->numfalse;
			tmp = tmp->next;
		}
	}
	

  y = cross(p1,p2);
  if (tabu_search == 1) {
		y = tabu(y,ltrech,ptrech);
	}

  freeIndividual(p1);
  freeIndividual(p2);
      
  if (insertion == 2){  
    if (y->numfalse < worsetemporarynumfalse) { 
			h = delete_individual(&(*r)); 
      cardpopulation--;
      if (y->numfalse <= bestnumfalse){
				if (y->numfalse == bestnumfalse) cardnumfalse++;
				else {
					bestflip = allflip;
					bestnumfalse = y->numfalse;
					cardnumfalse = 1;
					save_solution(y);
				}
      }
      y->numhamming = h;
      insert_individual(y,&(*r));
      return h;
    }
    else {
      freeIndividual(y);
      cardpopulation--;
      youngerindividual--;
      return -1;
    }
	}
  else {
		h = delete_individual(&(*r)); 
    cardpopulation--;
    if (y->numfalse <= bestnumfalse){
      if (y->numfalse == bestnumfalse) cardnumfalse++;
      else {
				bestflip = allflip;
				bestnumfalse = y->numfalse;
				cardnumfalse = 1;
				save_solution(y);
      }
    }
    y->numhamming = h;
    insert_individual(y,&(*r));
		return h;
  }
}

/********************************************************************/
/* delete_individual : delete the older individual of the population*/
/********************************************************************/
int delete_individual(Individual **ind){
	Individual *x;
	int i,j;
	if ((*ind)->numindividual == olderindividual) {
		//entropy
		if (file_ham != "no")
			for (j=1;j<numatom+1;j++)
				ent[(*ind)->atom[j]][j]--;					

		i = (*ind)->numhamming;
		x = *ind;
		(*ind) = (*ind)->next;
		freeIndividual(x);
	}
	else {
		i = delete_individual2(*ind);
	} 
	olderindividual++;	
	return i;
}

/*************************************************/
/* delete_individual2 : used by delete_individual*/
/*************************************************/
int delete_individual2(Individual *y){
  Individual *x;
  int j,i;
	
	while((y->next != NULL) && ((y->next)->numindividual != olderindividual))
		y = y->next;
	if (y->next == NULL) {
		printf("(y->next == NULL)\n");
	}
	else {
		//entropy
		if (file_ham != "no")
			for (j=1;j<numatom+1;j++)
				 ent[(y->next)->atom[j]][j]--;
		
		i = y->next->numhamming;
		x = y->next;
		y->next = (y->next)->next;
		freeIndividual(x);
		return i;
	}
}

/*******************************************************************************************************************/
/* insert_individual : inserts an individual in a sorted population */
/*******************************************************************************************************************/
void insert_individual(Individual * y, Individual ** z){
  Individual * tmp;
  int i;

  //entropy
	if (file_ham != "no"){
		for(i=1;i<numatom+1;i++)
			ent[y->atom[i]][i]++;
	}

	if (y->numfalse <= (*z)->numfalse) {
		y->next = *z;
		*z = y;
	}
	else{
		tmp = *z;
		while((tmp->next != NULL) && (y->numfalse >= tmp->next->numfalse))
			tmp = tmp->next;
		if (tmp->next == NULL) {
			y->next = NULL;
			tmp->next = y;
		}
		else { 
			y->next = tmp->next;
			tmp->next = y;
		}
	}
}

/**********************************************************************/
/* add_individual : copy an individual in an other list of individuals*/
/**********************************************************************/
Individual *add_individual(Individual *ind, Individual *x){
  int i;
  Individual *p;
	
  p=createIndividual();
  p->numfalse = x->numfalse;
  for(i=0;i<numclause;i++){
    p->false[i] = x->false[i];
    p->wherefalse[i] =  x->wherefalse[i];
    p->numtruelit[i] =  x->numtruelit[i];
  }
  for(i=1;i<numatom+1;i++){
    p->atom[i] = x->atom[i];
    p->breakcount[i] = x->breakcount[i];
    p->makecount[i] = x->makecount[i];
  }
  p->numindividual = x->numindividual;
  p->numhamming = x->numhamming;
  p->next = ind;
  return p;
}


/*********************************************************************************************************************/
/* initial_sort : after the creation of the population, all the individuals are sorted by the number of false clauses*/
/*********************************************************************************************************************/
Individual * initial_sort (Individual *individual){
  Individual *ind, *sol, *temp;
  int max, num,j;
	
  sol = NULL;	
  while (individual != NULL){
    ind = individual;
    max = -1;
    num = 0;
    while (ind != NULL){
      if (ind->numfalse >= max){
				if (ind->numfalse > max){
					max = ind->numfalse;
					num = 1;
				}
				else num++;
      }
      ind = ind->next;
    }
    while ((individual != NULL) && (individual->numfalse == max)){
      temp = individual;
      individual = individual->next;
      temp->next = sol;
      sol = temp;
      num--;
    }
		
    if (num != 0){
      ind = individual;
      while ((num != 0) && (ind->next != NULL)){
				if ((ind->next)->numfalse == max){ 
					temp = ind->next;
					ind->next = (ind->next)->next;
					temp->next = sol;
					sol = temp;
					num--;
				}
				else ind = ind->next;
      }
    }
  }
  return sol;
}

/****************************************************************************************************************************/
/* select_best_individual_function : selects the BESTINDIVIDUAL individuals of the population and return this sub-population*/
/****************************************************************************************************************************/
Individual *select_best_individual_function(Individual *ind){ 
  int i,j,num,d; 
  char *tab;
  Individual *tmp;
	
	
  tab = (char *)malloc(sizeof(char) * numclause); 
  num = cardpopulation;
	
  for(j=0;j<numclause;j++)
    barycentre[0][j] = 1;
	
  i = 0;
  while((i<BESTINDIVIDUAL) && (BESTINDIVIDUAL-i < num)){
		if (i == 0){
      for(j=0;j<ind->numfalse;j++)
				barycentre[i][ind->false[j]] = 0;
			tmp = add_individual(NULL,ind);
      i++;
    }
    else {
      for(j=0;j<numclause;j++)
				tab[j] = 1;
      for(j=0;j<ind->numfalse;j++)
				tab[ind->false[j]] = 0;
      
      for (j=0;j<i;j++){
				d = distance(tab, barycentre[j], i);
				if (d < distance_between_2_parents) break;
      }
      if (d >= distance_between_2_parents) {
				for (j=0;j<numclause;j++)
					barycentre[i][j] = tab[j];
				tmp = add_individual(tmp,ind);
				i++;
      }
    }
    ind = ind->next;
    num--;
  }

	if (i < BESTINDIVIDUAL){
		insertion = 1;
    while (i< BESTINDIVIDUAL ){
      tmp = add_individual(tmp,ind);
      ind = ind->next;
      i++;
    }
  }
  else insertion = 2;
	
  worsetemporarynumfalse = tmp->numfalse;
  free(tab);
	
	return tmp;
}


/***************************************/
/* distance : give the hamming distance*/
/***************************************/
float distance(char * tmp, char *tab, int i){
	int j,k;
	float distance;
  distance = 0;
	for(j=0;j<numclause;j++)
		distance = distance + ABS(tmp[j]-tab[j]);	
	return distance;
}
   

/********************************************/
/* parameters : read all the user parameters*/
/********************************************/
int parameters(int argc,char *argv[]){
  char * temp;
  int count;
	
	//initialization
  count = argc;	
	degree = 1;
	maxtimefalse = 5;
  maxtry = -1;
  ltinit =  -1;
  ltinitpc =  0;
  ptinit = -1;
  ltrech = -1;
  ltrechpc = 0;
  ptrech = -1;  
  file_result = "no";
  file_ham = "no";
  insertion = -1;
  crossover = -1;
  select_best_individual = -1;
  tabu_search = -1;
  distance_between_2_parents = -1;
  tabu_diversification = -1;
  BESTINDIVIDUAL = -1;
  NUMINDIVIDUAL = -1;
  timelimit = -1;
	benchmark_name ="";
	diversification_step = 10;
	initoptions = FALSE;
	result_bit = 0;
	result_lit = 0;
	savepop = 0;
	
	//bad parameters
	/*if (count % 2 == 0) {
		readme();
		exit(0);
	}*/
	benchmark_name = argv[1];
/*
  while (count != 1) {
    temp = argv[count-2];
    if (strcmp(temp,"-MC") == 0) {
      maxtry = atoi(argv[count-1]);
      MAXTRY = maxtry;
    }
		else
			if (strcmp(temp,"-B") == 0) 
			else
				if (strcmp(temp,"-LT1") == 0) ltinit = atoi(argv[count-1]);
				else
					if (strcmp(temp,"-TL") == 0) timelimit = atof(argv[count-1]);
					else
						if (strcmp(temp,"-LT1pc") == 0) {						
							ltinitpc = atoi(argv[count-1]);
						}
						else
							if (strcmp(temp,"-MF1") == 0) ptinit = atoi(argv[count-1]);
							else
								if (strcmp(temp,"-LT2") == 0) ltrech = atoi(argv[count-1]);
								else
									if (strcmp(temp,"-LT2pc") == 0) {
										ltrechpc = atoi(argv[count-1]);
									}
									else			
										if (strcmp(temp,"-MF2") == 0) ptrech = atoi(argv[count-1]);
										else
											if (strcmp(temp,"-FRb") == 0){
												file_result = argv[count-1];
												result_bit = 1;
											}
											else
												if (strcmp(temp,"-FRl") == 0){
													file_result = argv[count-1];
													result_lit = 1;
												}
												else
													if (strcmp(temp,"-FD") == 0) file_ham = argv[count-1];
													else
														if (strcmp(temp,"-CI") == 0) insertion = atoi(argv[count-1]);
														else
															if (strcmp(temp,"-CR") == 0) crossover = atoi(argv[count-1]);
															else
																if (strcmp(temp,"-SI") == 0) select_best_individual = atoi(argv[count-1]);
																else
																	if (strcmp(temp,"-T") == 0) tabu_search = atoi(argv[count-1]);
																	else
																		if (strcmp(temp,"-DM") == 0) distance_between_2_parents = atoi(argv[count-1]);
																		else
																			if (strcmp(temp,"-DT") == 0) tabu_diversification = atoi(argv[count-1]);
																			else
																				if (strcmp(temp,"-IF") == 0) {
																					initoptions = TRUE;
																					initfile = argv[count-1];
																				}
																				else
																					if (strcmp(temp,"-FP") == 0) {
																						savepop = 1;
																						popfile = argv[count-1];
																					}
																					else
																					if (strcmp(temp,"-DVT") == 0) diversification_step = atoi(argv[count-1]);
																					else
																						if (strcmp(temp,"-BI") == 0) {
																							BESTINDIVIDUAL = atoi(argv[count-1]);
																							if (BESTINDIVIDUAL < 2) {
																								printf("The minimal number for a cross is 2 parents. Please enter a new number for BI \n");
																								exit(0);
																							}
																						}
																						else
																							if (strcmp(temp,"-DG") == 0) degree = atoi(argv[count-1]);
																							else
																								if (strcmp(temp,"-MTF") == 0) maxtimefalse = atoi(argv[count-1]);
																								else
																									if (strcmp(temp,"-NI") == 0)NUMINDIVIDUAL = atoi(argv[count-1]);
																									else{//unknow parameter
																										readme();
																										exit(0);
																									}
		count = count-2;
	}*/
	
	//standard parameters
	if( maxtry == -1) {
		maxtry = 5000; 
		MAXTRY = 5000;
	}
	if( insertion == -1) insertion =2;
	if( crossover == -1)  crossover =3;
	if( select_best_individual == -1) select_best_individual =1;
	if( tabu_search == -1) tabu_search =1;
	if( distance_between_2_parents == -1) distance_between_2_parents =1;
	if( tabu_diversification == -1) tabu_diversification =1;
	if( BESTINDIVIDUAL == -1) BESTINDIVIDUAL =15;
	if( NUMINDIVIDUAL == -1) NUMINDIVIDUAL =100;
	if (BESTINDIVIDUAL > NUMINDIVIDUAL) {
		printf("BESTINDIVIDUAL must be inferior to NUMINDIVIDUAL. Please enter a new number for BI or NI \n");
		exit(0);
	}
	if (strcmp(benchmark_name,"") == 0) {
		printf("You must put a benchmark (-B benchmark)\n");
		exit(0);
	}
	
	return 1;
}

/*******************************************************/
/* print_values : print the parameters and the solution*/
/*******************************************************/
void print_values(Individual *ind) {
	FILE* fp_out;
	int i,p1,p2;
	int carac,flip;
	int last = 0;
	char * nomfichier = file_result;
	char nb[30];
	float time,c1;
	double Stime, SCtime, Scross, SCcross, Sflip, SCflip, Vtime, Vcross, Vflip, timeexec;
	
	if ((fp_out = fopen(nomfichier, "r")) == NULL){
		p1 = 0;
		p2 = 0;
		c1 = 0;
		flip = 0;
		time = 0;
		Vflip = 0;
		Vtime = 0;
		Vcross = 0;	
		printf("The file %s is created.\n",nomfichier);
	}
	else{
		while (fgetc(fp_out) != EOF){
			while ((carac = fgetc(fp_out)) != 't'){
				while ((carac = fgetc(fp_out)) != '\n');
			}
			while ((carac = fgetc(fp_out)) != 'r'){
				while ((carac = fgetc(fp_out)) != '\n');
			}
			for (i = 0; i<13; i++)
				carac = fgetc(fp_out);
			nb[0] = carac;
			i = 1;
			while ((carac = fgetc(fp_out)) != '/'){
				nb[i] = carac;
				i++;
			}
			nb[i] = '\0';
			p1 = atoi(nb);
			
			carac = fgetc(fp_out);
			nb[0] = carac;
			i = 1;
			while ((carac = fgetc(fp_out)) != ' '){
				nb[i] = carac;
				i++;
			}
			nb[i] = '\0';
			p2 = atoi(nb);
			
			while ((carac = fgetc(fp_out)) != 'p');
			for (i = 0; i<40; i++)
				carac = fgetc(fp_out);
			nb[0] = carac;
			i = 1;
			while ((carac = fgetc(fp_out)) != ' '){
				nb[i] = carac;
				i++;
			}
			nb[i] = '\0';
			time = atof(nb);
			
			for (i=0;i<40;i++)
				carac = fgetc(fp_out);
			i = 0;
			nb[i] = carac;
			i = 1;
			while ((carac = fgetc(fp_out)) != ','){
				nb[i] = carac;
				i++;
			}
			nb[i] = '\0';
			c1 = atof(nb);
			
			for (i=0;i<32;i++)
				carac = fgetc(fp_out);
			i = 0;
			nb[i] = carac;
			i = 1;
			while ((carac = fgetc(fp_out)) != ','){
				nb[i] = carac;
				i++;
			}
			nb[i] = '\0';
			flip = atof(nb);
			for (i=0;i<28;i++)
				carac = fgetc(fp_out);
			i = 0;
			nb[i] = carac;
			i = 1;
			while ((carac = fgetc(fp_out)) != ','){
				nb[i] = carac;
				i++;
			}
			nb[i] = '\0';
			Vtime = atof(nb);
			for (i=0;i<32;i++)
				carac = fgetc(fp_out);
			i = 0;
			nb[i] = carac;
			i = 1;
			while ((carac = fgetc(fp_out)) != ','){
				nb[i] = carac;
				i++;
			}
			nb[i] = '\0';
			Vcross = atof(nb);
			for (i=0;i<30;i++)
				carac = fgetc(fp_out);
			i = 0;
			nb[i] = carac;
			i = 1;
			while ((carac = fgetc(fp_out)) != '\n'){
				nb[i] = carac;
				i++;
			}
			nb[i] = '\0';
			Vflip = atof(nb);
		}
		fclose(fp_out);			
	}
	fp_out = fopen(nomfichier, "a");
	
	fprintf(fp_out, "c -----------------------------------------------------------\n"); 
	fprintf(fp_out, "c BENCHMARK : %s --->",benchmark_name); 
	if (FOUND == 1) {
		timeexec = (double)(endtime-begintime)/CLK_TCK;
		Stime = (float)p1 * time;
		Sflip = (float)p1 * (float)flip;
		Scross = (float)p1 * c1;
		if (p1 != 0){
			SCtime = (float)p1 * Vtime + Stime * Stime / (float)p1;
			SCflip = (float)p1 * Vflip + Sflip * Sflip / (float)p1;
			SCcross = (float)p1 * Vcross + Scross * Scross / (float)p1;
			Vtime =sqrt((float) (SCtime + timeexec*timeexec - ((Stime + timeexec)*(Stime + timeexec)/(float)(p1 + 1)))/(float)(p1 + 1));
			Vcross = sqrt((float)(SCcross + (MAXTRY-maxtry)*(MAXTRY-maxtry) - (Scross + (float)(MAXTRY-maxtry))*(Scross + (float)(MAXTRY-maxtry))/(float)(p1 + 1))/(float)(p1 + 1));
			Vflip = sqrt((float)(SCflip + (float)bestflip*(float)bestflip - (Sflip + (float)bestflip)*(Sflip + (float)bestflip)/(float)(p1 + 1))/(float)(p1 + 1));
		}
		fprintf(fp_out, " ASSIGNMENT FOUND\n"); 
		fprintf(fp_out, "c NumAtom = %d, NumClause = %d, MC = %d, LT1 = %d (%d percent), MF1 = %d, LT2 = %d (%d percent), MF2 = %d, CR = %d, NI = %d, BI = %d, DT = %d, T = %d, CI = %d, TL = %f, DM = %d, DG = %d, MTF = %d",numatom,numclause,MAXTRY,ltinit,ltinitpc,ptinit,ltrech,ltrechpc,ptrech,crossover,NUMINDIVIDUAL,BESTINDIVIDUAL,tabu_diversification,tabu_search,insertion,timelimit,distance_between_2_parents,degree,maxtimefalse);
		if (initoptions == TRUE) fprintf(fp_out, ", initial population : %s\n",initfile);
		else fprintf(fp_out, "\n");
		fprintf(fp_out, "c Number of crossings before finding : %d\n",MAXTRY-maxtry);
		fprintf(fp_out, "c Number of flip before finding : %d\n",bestflip);
		fprintf(fp_out, "t time : %5.3f seconds \n",timeexec);
		if (result_bit == 1)
			for (i=1; i<numatom+1; i++) 
				fprintf(fp_out, "%d ", solution[i]);
		else
			for (i=1; i<numatom+1; i++)
				if (solution[i] == 0)
					fprintf(fp_out, "%d ", -i);
				else
					fprintf(fp_out, "%d ", i);
		fprintf(fp_out, "\n"); 
		fprintf(fp_out, "r Statistics %d/%d, %2.2f percent, average running time of succes %5.3f seconds, average cross number of succes %5.3f, average flips number of succes %5.3f, standard deviation of time %2.3f, standard deviation of crosses %2.3f, standard deviation of flips %2.3f\n",p1+1,p2+1,((float)(p1+1)/(float)(p2+1))*100, (Stime+timeexec)/(float)(p1+1),(Scross+(float)(MAXTRY-maxtry))/(float)(p1+1),(double)(Sflip+bestflip)/(float)(p1+1), Vtime, Vcross, Vflip); 
	}
	else{
		fprintf(fp_out, "c ASSIGNMENT NOT FOUND\n"); 
		fprintf(fp_out, "c NumAtom = %d, NumClause = %d, MC = %d, LT1 = %d (%d percent), MF1 = %d, LT2 = %d (%d percent), MF2 = %d, CR = %d, NI = %d, BI = %d, DT = %d, T = %d, CI = %d, TL = %f, DM = %d, DG = %d, MTF = %d",numatom,numclause,MAXTRY,ltinit,ltinitpc,ptinit,ltrech,ltrechpc,ptrech,crossover,NUMINDIVIDUAL,BESTINDIVIDUAL,tabu_diversification,tabu_search,insertion,timelimit,distance_between_2_parents,degree,maxtimefalse);
		if (initoptions == TRUE) fprintf(fp_out, ", initial population : %s\n",initfile);
		else fprintf(fp_out, "\n");
		fprintf(fp_out, "t time : %5.3f seconds \n",(double)(endtime-begintime)/CLK_TCK);
		fprintf(fp_out, "c Best configuration with %d false clause(s) in %d flips\n",ind->numfalse,bestflip);
		if (result_bit == 1)
			for (i=1; i<numatom+1; i++) 
				fprintf(fp_out, "%d ", solution[i]);
		else
			for (i=1; i<numatom+1; i++)
				if (solution[i] == 0)
					fprintf(fp_out, "%d ", -i);
				else
					fprintf(fp_out, "%d ", i);
		fprintf(fp_out, "\n"); 
		fprintf(fp_out, "r Statistics %d/%d, %2.2f percent, average running time of succes %5.3f seconds, average cross number of succes %5.3f, average flip number of succes %5.3f, standard deviation of time %2.3f, standard deviation of crosses %2.3f, standard deviation of flips %2.3f\n",p1,p2+1,((float)p1/(float)(p2+1))*100,time,(float)c1,(float)flip,Vtime, Vcross,Vflip); 
	}     
	fclose(fp_out);			
} 


/******************************************************/
/* hamming_distance_tab : update the hamming structure*/
/******************************************************/
void hamming_distance_tab(int h, Individual * ind){
  int i,temp;
  for(i=0;i<NUMINDIVIDUAL;i++){
    temp = hamming_distance(h,i,ind);
    hammingtab[h][i] = temp;
    hammingtab[i][h] = temp;
  }
}

/*************************************************************************/
/* hamming_distance : computes the hamming distance between 2 individuals*/
/*************************************************************************/
int hamming_distance(int p1, int p2,Individual * ind){
  int i,sol;
  char *s1,*s2;
	
  if (p1 == p2) return 0;	
  s1 = (char*)malloc(sizeof(char) * (numatom+1));
  s2 = (char*)malloc(sizeof(char) * (numatom+1));
	
  while ((ind->numhamming != p1) && (ind->numhamming != p2))
    ind = ind->next;
  
  if (p1 == ind->numhamming)
    for(i=1; i<numatom+1;i++)
      s1[i] = ind->atom[i];
  else   
    for(i=1; i<numatom+1;i++)
      s2[i] = ind->atom[i];
  ind = ind->next;
	
  while ((ind->numhamming != p1) && (ind->numhamming != p2))
		ind = ind->next;
  
  if (p1 == ind->numhamming)
    for(i=1; i<numatom+1;i++)
      s1[i] = ind->atom[i];
  else   
    for(i=1; i<numatom+1;i++)
      s2[i] = ind->atom[i];
	
  sol = 0;
  for(i=1; i<numatom+1;i++)
    if (s1[i] != s2[i])
      sol++;
	
  free(s1);
  free(s2);
  return sol;
}


/*************************************************************************************/
/* average_hamming_distance : computes the average hamming distance of the population*/
/*************************************************************************************/
float average_hamming_distance(){
  int i,j,t;
  t = 0;
  for(i=1;i<NUMINDIVIDUAL;i++)
    for(j=0;j<i;j++){
      t = t+hammingtab[i][j];
    }
	return (float)(t*2)/(float)((NUMINDIVIDUAL-1) * NUMINDIVIDUAL);
}

/***************************************************/
/* copy_distance : writes the statistic in file_ham*/
/***************************************************/
void copy_distance(){
  FILE* fp_out;
  int i,p1,p2,j;
  
  fp_out = fopen(file_ham, "a");
  for(i=0;i<MAXTRY;i++){
    fprintf(fp_out,"%d ",i);
		for(j=0;j<8;j++)
      fprintf(fp_out,"%4.3f ",ham[j][i]);
    fprintf(fp_out,"\n");
  }
  fclose(fp_out);
}


/*******************************************************************************************/
/* average_num_false_clause : computes the average number of false clause in the population*/
/*******************************************************************************************/
float average_num_false_clause(Individual * ind){
  int i;
  i = 0;
  while (ind != NULL){
    i = i + ind->numfalse;
    ind = ind->next;
  }
  return (float)i/NUMINDIVIDUAL;
}

/***************************************************/
/* entropy : computes the entropy of the population*/
/***************************************************/
float entropy(){
  int i,j;
  double t,s;

  s = 0;
  for (i=1;i<numatom+1;i++)
    for (j=0;j<2;j++){
      t = (double)(ent[j][i])/(double)NUMINDIVIDUAL;
      if (t != 0)
				s = s + (t*log((double)t));      
    }
  s = -s / ((double)numatom*(double)log((double)2));
  return s;
}

/**********/
/* readme */
/**********/
void readme(){
  printf("GASAT -B [-MC -LT1 -LT1pc -MF1 -LT2 -LT2pc -MF2 -FR[l|b] -FP -IF -FD -CI -CR -SI -T -DM -DT -BI -NI -TL -DG -MTF]\n");
	printf("with\n");
	printf("\tB  : benchmark\n");
	printf("\tMC  : maximal number of crosses\n");
	printf("\tLT1 : length of tabu list for the initialization of the population\n");
	printf("\tLT1pc : length of tabu list for the initialization of the population (percent of number atom)\n");
	printf("\tMF1 : maximal number of flip for the initialization of the population\n");
	printf("\tLT2 : length of tabu list for the process\n");
	printf("\tLT2pc : length of tabu list for the process (percent of number atom)\n");
	printf("\tMF2 : maximal number of flip for the process\n");
	printf("\tFRb  : file where result is written with a string of 0/1\n");
	printf("\tFRl  : file where result is written with a string of lit/-lit\n");
	printf("\tFD  : file where diversity of population is written after each cross\n");
	printf("\tFP  : file where initial population is written\n");
	printf("\tIF  : file where initial population is read\n");
	printf("\tCI : condition of insertion for the new individual (1->insertion since condition; 2->insertion if it's better than the worse of the possible parents)\n");
	printf("\tCR : number of the cross (1-9, the best is the 3)\n");
	printf("\tSI : Selection of the possible parents. If 0 then all the individuals can be selected, else if 1 then 2 individuals are randomly selected in the best 15 else if 2 then 15 individuals are randomly selected and the best 2 are selected\n");
	printf("\tT : If 0 then no tabu search (only cross) else if 1 then tabu search is executed\n");
	printf("\tDM : minimal distance between two possible parents\n");
	printf("\tDT : if 0 then no diversification mecanism in tabu search, else if 1 then diversification mecanism\n");
	printf("\tBI : number of individuals where the two parents are selected\n");
	printf("\tNI : number of individuals in the population\n");
	printf("\tTL : time limit in seconds (-1 unlimited)\n");
	printf("\tDG : mechanism of degree (1->yes, 0->no)\n");
	printf("\tMTF : maximum number of time that a clause is the last false clause before a diversification mechanism\n");
	printf("\tDVT : number of time where the false clauses are put to true in the diversification mechanism\n\n");
	printf(" default parameters :\n");
	printf("GASAT -MC 1000 -LT1pc 5 -MF1 1000 -LT2pc 10 -MF2 10000 -CI 2 -CR 3 -SI 1 -T 1 -DM 1 -DT 1 -BI 15 -NI 100 -TL -1 -DG 1 -MTF 5 -DVT 5 \n");
}
