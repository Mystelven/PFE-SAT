/*
##############################################################################
# 
# NQueens - Valentin Montmirail - Polytech Tours, France
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
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>

/* The N*N queens variable is a global one to be accessible everywhere */
int n;

/* The name of the solver is also a global variable to get it everywhere. */
char* solver = NULL;

/** 
 * generate : this fonction will create the CNF file (or at least count the number of clauses);
 * @param p a boolean, false if we just want to count the number of clauses, true if we want to write in file.
 * @param file the final CNF file that will be created. 
 */
int generate(int p,FILE* file) {

    int i,j,k;
    int c = 0;

    /* queen on every row */
    for (j=1; j<=n; j++){
        for (i=1; i<=n; i++){
            if (p)  fprintf(file,"%i ", n*(j-1)+i);
        }
        c++; if (p) fprintf(file," 0\n");
    }

    /* no horizontal attacks */
    for (j=1; j<=n; j++){
        for (i=1; i<n; i++){
            for (k=i+1; k<=n; k++){
                c++; if (p) fprintf(file,"-%i -%i 0\n", n*(j-1)+i, n*(j-1)+k);
            }
        }
    }

    /* no vertical attacks */
    for (i=1; i<=n; i++){
        for (j=1; j<n; j++){
            for (k=j+1; k<=n; k++){
                c++; if (p) fprintf(file,"-%i -%i 0\n", n*(j-1)+i, n*(k-1)+i);
            }
        }
    }

    /* no SE attacks */
    for (i=1; i<n; i++){
        for (j=1; j<n; j++){
            for (k=1; (i+k)<=n && (j+k)<=n; k++){
                c++; if (p) fprintf(file,"-%i -%i 0\n", n*(j-1)+i, n*(j-1+k)+i+k);
            }
        }
    }

    /* no SW attacks */
    for (i=2; i<=n; i++){
        for (j=1; j<n; j++){
            for (k=1; (i-k)>=1 && (j+k)<=n; k++){
                c++; if (p) fprintf(file,"-%i -%i 0\n", n*(j-1)+i, n*(j-1+k)+i-k);
            }
        }
    }

    return c;
}


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

/**
 * createFile : this function will create the CNF file according to a number of queens.
 * @param filename : the file name of the final CNF file.
 * @param nbQueens : the number of queens in our N*N queens.
 */
void createFile(const char* filename, unsigned int nbQueens) {

	FILE* file = NULL;

    file = fopen(filename, "w+");

    fprintf(file,"p cnf %i ", n*n);

    fprintf(file,"%i",generate(0,file));

    fprintf(file,"\n");

   	generate(1,file);

   	fclose(file);
}

/**
 * displayResult: this function will display the result according to the result file.
 * @param resultFilename the result stored in a file, where there is v i, i+1, ... +j 0
 * @return 0 if everything is okay, something else otherwise.
 */
int displayResult(const char* resultFilename) {

    char * line = NULL;
    size_t len  = 0;

    ssize_t read;

    char** str_variables;

    int i     = 0;
    int found = 0;

    FILE* fp  = fopen(resultFilename, "r");

    if (fp == NULL) exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {

        if(line[0] == 'v') {
            found = 1;
            break; 
        }
    }

    fclose(fp);

    if(found == 0) {

        return -1;
    }

    str_variables = str_split(line, ' ');

    if (line) free(line);  

    printf("\t-");
    for(len = 0; len < n; len++) {
        printf("--");
    }
    printf("\n\t");

    for(len = 1; len < (n*n)+1 ; len++) {

        printf("|");

        if(str_variables[len][0] == '-') {
            printf(" ");
        } else {
            printf("0");
            
        }

        if(len % n == 0) {

            printf("|\n\t");

            printf("-");
            for(i = 0; i < n; i++) {
                printf("--");
            }

            printf("\n\t");

        }
    }

    return 0;
}

/**
 * selectSolver : this function will return the good value, 
 * according to the name of the solver.
 */
int selectSolver() {

    if(strcmp(solver,"./glucose") ==0) return 1;

    if(strcmp(solver,"./cdls") ==0)    return 2;

    return 0;
}


/**
 * main : This fonction represent the main of the program.
 * It will create the CNF file, solve it and write it inside an other file
 * and after, we display the result according to the result file.
 * @param argc the number of arguments passed to the program.
 * @param argv the set of all the arguments.
 */
int main(int argc,char** argv) {

    if (argc < 2) return(1);

    n = atoi(argv[1]);

    if(argc > 2) {
        solver = argv[2];
    } else {
        solver = "./satyr";
    }

    char* filename = "Reductions/queens.cnf";

    char* saveAs   = "Reductions/queenResult.sat";

    FILE* file = fopen(saveAs, "w+");
    fclose(file);

    printf("\n\n  ******* Welcome in %2d-Queens *******\n",n);
    printf("  Created by Valentin Montmirail,\n");
    printf("  an engineering student in Polytech Tours\n\n");

	createFile(filename,n);
    
    char* buffer = (char*)malloc(sizeof(char)*1024);

    int select = selectSolver();

    switch(select) {

        case 1:
            sprintf(buffer, "%s -model %s > %s", solver, filename, saveAs);
            break;

        case 2:
            sprintf(buffer,"%s  > %s < %s 2>&1 > %s",solver,saveAs,filename,saveAs);
            break;

        default:
            sprintf(buffer, "%s %s > %s", solver, filename, saveAs);
            break;
    }

    system(buffer);

    int result = displayResult(saveAs);

    if(result != 0) {
        printf("  %2d-Queens has no solutions possible...\n",n);
    }

    printf("\n\n  *************************************\n");

    printf("\n");

    return 0;
}