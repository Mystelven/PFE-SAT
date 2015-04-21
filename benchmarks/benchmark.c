/*
##############################################################################
# 
# Benchmark Plot - Valentin Montmirail - Polytech Tours, France
# Copyright (c) 2015.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
##############################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>

#include "gnuplot_i.h"

#define NB_INSTANCES 9000

double* Y_array;
int numInstance = 1;

/**
 * str_split : this function is useful to be able to split an array of string according to a set of delimiters.
 * @param a_str the array of string.
 * @param a_delim the set of delimiters.
 * @return the new array of string, split according to different delimiters.
 */
 char** str_split(char* a_str, const char a_delim) {

    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
      if (a_delim == *tmp)
      {
        count++;
        last_comma = tmp;
     }
     tmp++;
  }

    /* Add space for trailing token. */
  count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
  count++;

  result = malloc(sizeof(char*) * count);

  if (result)
  {
   size_t idx  = 0;
   char* token = strtok(a_str, delim);

   while (token)
   {
     assert(idx < count);
     *(result + idx++) = strdup(token);
     token = strtok(0, delim);
  }
  assert(idx == count - 1);
  *(result + idx) = 0;
}

return result;
}

double* getX_array() {

	double* result = malloc(sizeof(double)*NB_INSTANCES);

	for(int i = 0 ; i < NB_INSTANCES; i++) {
		result[i] = i;
	}

	return result;
}

double* getY_array() {

	Y_array = malloc(sizeof(double)*NB_INSTANCES);

	Y_array[0] = 0;

	return Y_array;
}

void treatmentResultSolvingTime(char* solver, char* result) {

	char* ptr;
	double ret = 0;

	if(strcmp(solver,"Glucose") == 0) {

		ret = strtod(result+25,&ptr);
		printf("%d : %lf\n",numInstance,ret);

		Y_array[numInstance] = Y_array[numInstance-1] + ret;
		numInstance++;
		
	} 

	else if(strcmp(solver,"Walksat") == 0) {

    ret = strtod(result+24,&ptr);
    ret *= 10;
    printf("%d : %lf\n",numInstance,ret);

    Y_array[numInstance] = Y_array[numInstance-1] + ret;
    numInstance++;
	} 

	else if(strcmp(solver,"GASAT") == 0) {

    ret = strtod(result+7,&ptr);
    printf("%d : %lf\n",numInstance,ret);

    Y_array[numInstance] = Y_array[numInstance-1] + ret;
    numInstance++;
	} 

	else if(strcmp(solver,"Zchaff") == 0) {
		
		ret = strtod(result+21,&ptr);
		printf("%d : %lf\n",numInstance,ret);

		Y_array[numInstance] = Y_array[numInstance-1] + ret;
		numInstance++;

	} else {	

		ret = strtod(result+23,&ptr);
		printf("%d : %lf\n",numInstance,ret);

		Y_array[numInstance] = Y_array[numInstance-1] + ret;
		numInstance++;
	}

}



void treatmentFile(char* directory, char* filename,char* solver) {

	char cmd[1000];

	if(strcmp(solver,"Glucose") == 0) {

		/* Here is the cmd that we need to execute to isolate the solving time at Glucose. */
		sprintf(cmd,"cat %s/%s | grep 'CPU time'",directory,filename);
	} 

	else if(strcmp(solver,"Walksat") == 0) {

		/* Here is the cmd that we need to execute to isolate the solving time at Walksat. */
    sprintf(cmd,"cat %s/%s | grep 'total elapsed seconds'",directory,filename);
	} 

	else if(strcmp(solver,"GASAT") == 0) {

		/* Here is the cmd that we need to execute to isolate the solving time at GaSAT. */
    sprintf(cmd,"cat %s/%s | grep 'time'",directory,filename);
	} 

	else if(strcmp(solver,"Zchaff") == 0) {

		/* Here is the cmd that we need to execute to isolate the solving time at Zchaff. */
		sprintf(cmd,"cat %s/%s | grep 'Run Time'",directory,filename);

	} else {
		
		/* Here is the cmd that we need to execute to isolate the solving time at Glucose. */		
		sprintf(cmd,"cat %s/%s | grep 'Solving'",directory,filename);
	}
	
  FILE *fp;
  char path[1035];

  /* Open the command for reading. */
  fp = popen(cmd, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path)-1, fp) != NULL) {

    	treatmentResultSolvingTime(solver,path);
  }

  /* close */
  pclose(fp);
}

void listdir(const char *name, int level,char* solver)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            
            listdir(path, level + 1,solver);
        }
        else {

        		treatmentFile((char*)name,entry->d_name,solver);
        }

    } while (entry = readdir(dir));

    closedir(dir);
}

void formatSolved(char* solver) {

	char directory[100];

	sprintf(directory,"./solved/%s",solver);

	listdir(directory,0,solver);
}


/**
 * main : This fonction represent the main of the program.
 * it will parse all the result files for every solver.
 * and it will create a plot of the time-solving of every solver.
 * @param argc the number of arguments passed to the program.
 * @param argv the set of all the arguments.
 */
 int main(int argc, char * argv[]) {

 	char* output = (char*)"default.png";

 	char* solver = strdup(argv[1]);

 	char cmd[255];

 	if(argc > 1) {

 		output = strcat(argv[1],".png");

 	} else {

 		fprintf(stderr,"You put no name, the default name is : default.png\n");
 		fprintf(stderr,"If you want to change the name, just use ./benchmark [nomOutput]\n");
 	}

 	gnuplot_ctrl * g  = gnuplot_init();

 	gnuplot_cmd(g,"set terminal png");

 	strcpy(cmd,"set output \"");
 	strcat(cmd,output);
 	strcat(cmd,"\"");

 	gnuplot_cmd(g, cmd);

 	gnuplot_set_xlabel(g,(char*)("number of problems (u)"));
 	gnuplot_set_ylabel(g,(char*)("solving time (s)"));

 	double* x = getX_array();
	double* y = getY_array();

 	formatSolved(solver);

  printf("temps final : %lf\n",y[NB_INSTANCES-1]);

  y[NB_INSTANCES-1] = 50000;

 	gnuplot_plot_xy(g, x, y,  NB_INSTANCES, solver) ;

 	gnuplot_close(g);

 	return 0 ;
 }

