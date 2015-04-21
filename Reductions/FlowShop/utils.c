#include "utils.h"
#include <assert.h>
#include <stdlib.h>

#define SIZE_MAX_FLOWSHOP 5000

/**
 * str_split : this function is useful to be able to split an array of string according to a set of delimiters.
 * @param s the array of string.
 * @param ct the set of delimiters.
 * @return the new array of string, split according to different delimiters.
 */
char ** str_split (char *s, const char *ct)
{
   char **tab = NULL;

   if (s != NULL && ct != NULL)
   {
      int i;
      char *cs = NULL;
      int size = 1;

/* (1) */
      for (i = 0; (cs = strtok (s, ct)); i++)
      {
         if (size <= i + 1)
         {
            void *tmp = NULL;

/* (2) */
            size <<= 1;
            tmp = realloc (tab, sizeof (*tab) * (unsigned long)size);
            if (tmp != NULL)
            {
               tab = tmp;
            }
            else
            {
               fprintf (stderr, "Memoire insuffisante\n");
               free (tab);
               tab = NULL;
               exit (EXIT_FAILURE);
            }
         }
/* (3) */
         tab[i] = cs;
         s = NULL;
      }
      tab[i] = NULL;
   }
   return tab;
}

Problem* readFile(char* filename) {
  
    FILE* file = fopen(filename,"r+");
    Problem* problem = NULL;
    char* line;

    line = (char*)malloc(sizeof(char)*SIZE_MAX_FLOWSHOP);

    if(line == NULL) {
    	perror("malloc problem in utils.c");
    	exit(-2);
    }

    int definitionProblemDone = 0;
    unsigned int numS,numJ;
    unsigned int value;

    while( fgets(line,SIZE_MAX_FLOWSHOP,file) ) {

      if(line == NULL) continue;

    	if(line[0] == 'c') {

    		continue;
    	}

    	if(definitionProblemDone == 0) {
    		
    		if (sscanf(line,"%d %d",&numS,&numJ) != 2) {

    			fprintf(stderr,"Problem, the first line which is not a comment line should be : \n");
    			fprintf(stderr,"numberOfJobs numberOfShops\n");
    			exit(-1);
    		}
    		
    		problem = createProblem(numS,numJ);

    		definitionProblemDone = 1;
    		numS = 0;
    		numJ = 0;

    		continue;
    	}

    	char** str_variables = str_split(line," ");

    	while(str_variables[numJ] != NULL) {

       		value = (unsigned int)atoi(str_variables[numJ]);
         	
         	if(value > 0) {

         		addTime(problem,numS,numJ,value);

         	}

          
      		numJ++;
      	}

      
    	numS++;
    	numJ = 0;
 	}

 	fclose(file);

	return problem;
}
