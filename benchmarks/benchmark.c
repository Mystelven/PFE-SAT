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

#include "gnuplot_i.h"


/**
 * main : This fonction represent the main of the program.
 * it will parse all the result files for every solver.
 * and it will create a plot of the time-solving of every solver.
 * @param argc the number of arguments passed to the program.
 * @param argv the set of all the arguments.
 */
int main(int argc, char * argv[]) {

	char* output = "default.png";
	char cmd[255];

	if(argc > 1) {

		output = strcat(argv[1],".png");

	} else {

		fprintf(stderr,"You put no name, the default name is : default.png\n");
		fprintf(stderr,"If you want to change the name, just use ./benchmark [nomOutput]\n");
	}

	gnuplot_ctrl * g = gnuplot_init();

	strcpy(cmd,"set output \"");
	strcat(cmd,output);
	strcat(cmd,"\"");

	gnuplot_cmd(g, "set terminal png");
	gnuplot_cmd(g, cmd);
	gnuplot_plot_equation(g, "exp(x)", "Benchmark");
	gnuplot_close(g);

	return 0 ;
}

